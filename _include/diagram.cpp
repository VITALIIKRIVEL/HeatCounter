// Qt:
#include <QMouseEvent>
#include <QKeyEvent>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrinterInfo>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtWidgets/QGesture>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtGui/QMouseEvent>
#include <QtCharts/QLegend>
#include <QtCharts/QLegendMarker>
#include <QFileDialog>
// Common:
#include "convert.h"
#include "filesys.h"
#include "diagram.h"

namespace nayk {

//=========================================================================================
//
//  MultiChart
//
//==========================================================================================
MultiChart::MultiChart(QWidget *parent, const QString &chartTitle) : QChartView( parent )
{
    actionDisable = true;
    setAutoFillBackground( true );
    setPalette( Qt::white );
    setBackgroundBrush( QBrush(Qt::white) );

    chart = new QChart();
    chart->setTitle( (chartTitle.isNull() || chartTitle.isEmpty()) ? "График" : chartTitle);

    // Настройка осей графика
    axisX = new QBarCategoryAxis();
    axisX->setTitleText("X");
    chart->addAxis(axisX, Qt::AlignBottom);

    axisY = new QValueAxis();
    axisY->setTitleText("Y");
    axisY->setLabelFormat("%." + QString::number(_precise) + "f ");
    axisY->setTickCount(5);
    chart->addAxis(axisY, Qt::AlignLeft);

    // Устанавливаем график в представление
    setChart(chart);

    clear();
    updateFontsAndColors();
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);

    setRenderHint(QPainter::Antialiasing);

    actionDisable = false;
}
//==========================================================================================
MultiChart::~MultiChart()
{
    actionDisable = true;
    clear();
    if(chart) delete chart;
}
//==========================================================================================
void MultiChart::clear()
{
    for(auto i=0; i<chartList.size(); ++i) {
        QBarSeries* bs = chartList[i].bs;
        if(bs) delete bs;
        QLineSeries* ls = chartList[i].ls;
        if(ls) delete ls;
    }
    chartList.clear();
    if(chart) chart->series().clear();
    resetGraphic();
}
//==========================================================================================
void MultiChart::setChartTitleText(const QString &text)
{
    if(chart) chart->setTitle( text );
    updateFontsAndColors();
}
//==========================================================================================
void MultiChart::setXTitleText(const QString &text)
{
    if(axisX) axisX->setTitleText( text );
    updateFontsAndColors();
}
//==========================================================================================
void MultiChart::setYTitleText(const QString &text)
{
    if(axisY) axisY->setTitleText( text );
    updateFontsAndColors();
}
//==========================================================================================
void MultiChart::updateFontsAndColors()
{
    if(!chart || !axisX || !axisY) return;

    QFont font = chart->titleFont();
    font.setBold(true);
    font.setPixelSize(_fontSize + 1);
    chart->setTitleFont(font);

    font = axisX->titleFont();
    font.setPixelSize(_fontSize);
    chart->setTitleFont(font);
    axisX->setTitleFont(font);
    axisY->setTitleFont(font);

    font = chart->legend()->font();
    font.setPixelSize(_fontSize + 1);
    chart->legend()->setFont(font);

    font = axisX->labelsFont();
    font.setPixelSize(_fontSize);
    axisX->setLabelsFont(font);
    axisY->setLabelsFont(font);

    for(auto i=0; i<chartList.size(); ++i) {
        font = chartList.at(i).ls->pointLabelsFont();
        font.setPixelSize(_fontSize - 1);
        chartList[i].ls->setPointLabelsFont(font);

        font = chartList.at(i).bs->barSets().first()->labelFont();
        font.setPixelSize(_fontSize - 1);
        chartList.at(i).bs->barSets()[0]->setLabelFont(font);
    }

    for(auto i=0; i<chart->legend()->markers().size(); ++i) {

        QLegendMarker *lm = chart->legend()->markers()[i];
        font = lm->font();
        font.setPixelSize(_fontSize);
        lm->setFont(font);
    }
}
//==========================================================================================
void MultiChart::resetGraphic()
{
    zoomReset();
    if(!axisX || !axisY || !chart) return;
    if(_captions.isEmpty()) return;
    axisX->setCategories( _captions );
    axisX->setRange( _captions.first(), _captions.last() );
    axisY->setRange(qMin(minVal, 0.0), maxVal + qAbs(maxVal/20.0));
    axisY->setLabelFormat("%." + QString::number(_precise) + "f ");

    for(auto i=0; i<chartList.size(); ++i) {
        QBarSeries *bs = chartList[i].bs;
        if(bs) bs->setLabelsPrecision( _precise );
    }

    updateLegend();
}
//==========================================================================================
void MultiChart::updateLegend()
{
    if(!chart) return;
    for(auto i=0; i<chart->legend()->markers().size(); ++i) {
        QLegendMarker* lm = chart->legend()->markers()[i];
        if(!lm) continue;
        if(lm->type() == QLegendMarker::LegendMarkerTypeBar) {
            lm->setVisible(false);
            continue;
        }

        lm->setVisible(true);
        for(auto j=0; j<chartList.size(); ++j) {
            if(lm->series() == chartList.at(j).ls) {
                lm->setLabel( (chartList.at(j).visible ? "☑ " : "☐ ") + lm->series()->name() );
                break;
            }
        }
    }
}
//==========================================================================================
void MultiChart::setBarChartVisible(bool visible)
{
    _barChartVisible = visible;
    for(auto i=0; i<chartList.size(); ++i) {
        chartList[i].bs->setVisible( _barChartVisible && chartList.at(i).visible );
    }
    updateLegend();
}
//==========================================================================================
void MultiChart::setLineChartVisible(bool visible)
{
    _lineChartVisible = visible;
    for(auto i=0; i<chartList.size(); ++i) {
        chartList[i].ls->setVisible( _lineChartVisible && chartList.at(i).visible );
    }
    updateLegend();
}
//==========================================================================================
void MultiChart::on_legendMarkerClicked()
{
    QLegendMarker *lm = dynamic_cast<QLegendMarker *>( sender() );
    if(!lm) return;

    for(auto i=0; i<chartList.size(); ++i) {
        if(chartList.at(i).ls == lm->series()) {

            chartList[i].visible = !chartList.at(i).visible;
            chartList[i].bs->setVisible(_barChartVisible && chartList.at(i).visible);
            chartList[i].ls->setVisible(_lineChartVisible && chartList.at(i).visible);
            break;
        }
    }
    updateLegend();
}
//==========================================================================================
void MultiChart::setPointLabelsVisible(bool visible)
{
    for(auto i=0; i<chartList.size(); ++i) {
        chartList[i].ls->setPointLabelsVisible( visible );
        chartList[i].bs->setLabelsVisible( visible );
    }
}
//==========================================================================================
void MultiChart::addChart(const QVector<qreal> &values, const QString &title, QColor color)
{
    if(!chart) return;
    if(!axisX) return;
    if(!axisY) return;
    actionDisable = true;

    ChartStruct cs;

    cs.ls = new QLineSeries();
    cs.ls->setName(title);
    cs.ls->setPointLabelsFormat( "@yPoint" );

    QBarSet *barSet = new QBarSet(title);

    for(auto i=0; i<values.size(); ++i) {
        barSet->append( values.at(i) );
        cs.ls->append(i, values.at(i));
        maxVal = qMax( maxVal, values.at(i) );
        minVal = qMin( minVal, values.at(i) );
    }

    cs.bs = new QBarSeries();
    cs.bs->append( barSet );
    chart->addSeries( cs.bs );
    cs.bs->attachAxis( axisX );
    cs.bs->attachAxis( axisY );
    cs.bs->setLabelsFormat( "@value" );
    cs.bs->setLabelsPrecision(_precise);
    cs.bs->setLabelsPosition(QAbstractBarSeries::LabelsCenter);

    chart->addSeries( cs.ls );
    cs.ls->attachAxis( axisX );
    cs.ls->attachAxis( axisY );

    if(color.isValid()) cs.ls->setColor( color );
    barSet->setColor( cs.ls->color() );
    cs.bs->setBarWidth( cs.bs->barWidth() + cs.bs->barWidth()/3.0 );

    for(auto i=0; i<chart->legend()->markers().size(); ++i) {

        QLegendMarker *lm = chart->legend()->markers()[i];
        if(lm->series() == cs.ls ) {
            connect(lm, &QLegendMarker::clicked, this, &MultiChart::on_legendMarkerClicked);
            break;
        }
    }

    cs.ls->setVisible(_lineChartVisible);
    cs.bs->setVisible(_barChartVisible);
    cs.visible = true;

    chartList.append(cs);

    actionDisable = false;
    updateFontsAndColors();
    resetGraphic();
}
//==========================================================================================
void MultiChart::setOptions( const QStringList &xCaptions, int precise )
{
    _captions = xCaptions;
    _precise = precise;
    resetGraphic();
}
//=============================================================================================
void MultiChart::delChart(int index)
{
    if((index < 0) || (index >= chartList.size())) return;
    QBarSeries *bs = chartList[index].bs;
    if(bs) delete bs;
    QLineSeries *ls = chartList[index].ls;
    if(ls) delete ls;
    chartList.removeAt(index);
    updateLegend();
}
//=============================================================================================
bool MultiChart::exportToFile(const QString &fileName)
{
    QString fl = fileName;
    if(fileName.isNull() || fileName.isEmpty()) {
        fl = QFileDialog::getSaveFileName(0,
                                          tr("Сохранить файл"), "",
                                          tr("Файлы изображений (*.png)")
                                          );
        if(fl.isNull() || fl.isEmpty()) return false;
    }
    QPixmap pix( grab() );
    return pix.save( FileSys::changeFileExt(fl,".png") , "PNG");
}
//==========================================================================================
void MultiChart::printPreview()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPrinter::A4);
    printer.setOrientation( QPrinter::Landscape );
    printer.setFullPage( true );

    QPrintPreviewDialog preview(&printer, this, Qt::Window);
    preview.setWindowTitle( QObject::tr("Предварительный просмотр") );
    connect(&preview, &QPrintPreviewDialog::paintRequested, this, &MultiChart::print);
    preview.exec();
}
//==========================================================================================
void MultiChart::print(QPrinter *printer)
{
    QPainter painter(printer);
    render(&painter);
}
//==========================================================================================
void MultiChart::mousePressEvent(QMouseEvent *event)
{
    if (m_isTouching) return;

    if (event->button() == Qt::LeftButton) {
        m_isTouching = true;
        pos.setX( event->localPos().x() );
        pos.setY( event->localPos().y() );
        if(chart) chart->update();
        update();
    }
    else if (event->button() == Qt::MiddleButton) {
        zoomReset();
    }
    QChartView::mousePressEvent(event);
}
//==========================================================================================
void MultiChart::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_isTouching || !chart) return;

    int x = event->localPos().x();
    int y = event->localPos().y();
    chart->scroll( pos.x() - x, y - pos.y() );
    pos.setX(x);
    pos.setY(y);
    chart->update();
    update();
    QChartView::mouseMoveEvent(event);
}
//==========================================================================================
void MultiChart::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isTouching) m_isTouching = false;
    if(chart) chart->update();
    update();
    QChartView::mouseReleaseEvent(event);
}
//==========================================================================================
void MultiChart::wheelEvent(QWheelEvent *event)
{
    if(!chart) return;
    QPoint numDegrees = event->angleDelta() / 8;
    if(!numDegrees.isNull()) {
        if(numDegrees.y() > 0) chart->zoomIn();
        else if (numDegrees.y() < 0) chart->zoomOut();
    }
    chart->update();
    update();
    QChartView::wheelEvent(event);
}
//==========================================================================================
void MultiChart::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Left:
        stepRight();
        break;
    case Qt::Key_Right:
        stepLeft();
        break;
    case Qt::Key_Up:
        stepDown();
        break;
    case Qt::Key_Down:
        stepUp();
        break;
    case Qt::Key_Equal:
        zoomReset();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
}
//==========================================================================================
void MultiChart::zoomReset()
{
    if(!chart) return;
    chart->zoomReset();
    chart->update();
    update();
}
//==========================================================================================
void MultiChart::zoomIn()
{
    if(!chart) return;
    chart->zoomIn();
    chart->update();
    update();
}
//==========================================================================================
void MultiChart::zoomOut()
{
    if(!chart) return;
    chart->zoomOut();
    chart->update();
    update();
}
//==========================================================================================
void MultiChart::stepUp(qreal step)
{
    if(!chart) return;
    chart->scroll(0, (step < 0) ? _step : step);
    chart->update();
    update();
}
//==========================================================================================
void MultiChart::stepDown(qreal step)
{
    if(!chart) return;
    chart->scroll(0, -((step < 0) ? _step : step) );
    chart->update();
    update();
}
//==========================================================================================
void MultiChart::stepLeft(qreal step)
{
    if(!chart) return;
    chart->scroll(-((step < 0) ? _step : step), 0);
    chart->update();
    update();
}
//==========================================================================================
void MultiChart::stepRight(qreal step)
{
    if(!chart) return;
    chart->scroll((step < 0) ? _step : step, 0);
    chart->update();
    update();
}
//==========================================================================================
} //namespace
