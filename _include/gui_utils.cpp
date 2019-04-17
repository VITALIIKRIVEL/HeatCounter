#include <QApplication>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QLayoutItem>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QDateEdit>
#include <QTimeEdit>
#include <QDateTimeEdit>
#include <QSettings>
//
#include "gui_utils.h"
#include "ex_controls.h"
#include "crypto.h"

namespace nayk {
//===================================================================================================
void Gui::clearTableWidget(QTableWidget *table)
{
    if(!table) return;

    clearTableWidgetBody(table);

    // заголовок таблицы:
    for(int i=0; i<table->horizontalHeader()->count(); i++) {
        if(table->horizontalHeaderItem(i)) {
            QTableWidgetItem *item = table->horizontalHeaderItem(i);
            delete item;
        }
    }
    table->setColumnCount(0);
}
//===================================================================================================
void Gui::clearTableWidgetBody(QTableWidget *table)
{
    if(!table) return;

    while(table->rowCount() > 0) {
        removeTableWidgetRow(table, table->rowCount()-1);
    }
}
//===================================================================================================
void Gui::removeTableWidgetRow(QTableWidget *table, int row)
{
    if(!table) return;
    if((row < 0) || (row > table->rowCount()-1)) return;

    for(int j=0; j<table->columnCount(); j++) {

        QWidget *w = table->cellWidget(row, j);
        if(w) delete w;
        table->removeCellWidget(row, j);

        if(table->item(row,j)) {
            QTableWidgetItem *item = table->item(row,j);
            delete item;
        }
    }
    table->removeRow(row);
}
//===================================================================================================
void Gui::resizeTableToContents(QTableWidget *table)
{
    if(!table) return;
    // размер ячеек по содержимому:
    table->resizeColumnsToContents();
    table->resizeRowsToContents();

    // чучуть расширить ячейки:

    for(int i=0; i<table->columnCount(); i++ ) {
        table->setColumnWidth(i, table->columnWidth(i) + 8);
    }

    for(int i=0; i<table->rowCount(); i++ ) {
        table->setRowHeight(i, table->rowHeight(i) + 2);
    }
}
//============================================================================================
void Gui::swapTableRows(QTableWidget *table, int row1, int row2)
{
    if(!table) return;
    if((row1 == row2) || (row1 < 0) || (row2 < 0) || (row1 >= table->rowCount()) || (row2 >= table->rowCount())) return;

    for(auto col=0; col<table->columnCount(); ++col) {

        QWidget *w1 = table->cellWidget(row1, col);
        QWidget *w2 = table->cellWidget(row2, col);

        swapWidgets(w1, w2);

        if(w1->isActiveWindow()) {
            w2->setFocus();
        }
        else if(w2->isActiveWindow()) {
            w1->setFocus();
        }

        QTableWidgetItem *item1 = table->item(row1, col);
        QTableWidgetItem *item2 = table->item(row2, col);        

        if(item1 && item2) {
            QColor cl = item1->backgroundColor();
            QBrush br = item1->background();
            item1->setBackground( item2->background() );
            item1->setBackgroundColor( item2->backgroundColor() );
            item2->setBackground( br );
            item2->setBackgroundColor( cl );

            table->setItem( row1, col, item2 );
            table->setItem( row2, col, item1 );
        }
    }
}
//============================================================================================
void Gui::swapWidgets(QWidget *w1, QWidget *w2)
{
    if(!w1 || !w2 || (QString(w1->metaObject()->className()) != QString(w2->metaObject()->className()))) {
        return;
    }
    QString className = QString(w1->metaObject()->className());
    if(className == "QComboBox") {

        QComboBox *box1 = dynamic_cast<QComboBox*>(w1);
        QComboBox *box2 = dynamic_cast<QComboBox*>(w2);

        QList<QString> ls;
        QList<QVariant> lv;

        int indx1 = box1->currentIndex();
        int indx2 = box2->currentIndex();

        for(auto i=0; i<box1->count(); ++i) {
            ls.append( box1->itemText(i) );
            lv.append( box1->itemData(i) );
        }
        box1->clear();
        for(auto i=0; i<box2->count(); ++i) {
            box1->addItem( box2->itemText(i), box2->itemData(i) );
        }
        box2->clear();
        for(auto i=0; i<ls.size(); ++i) {
            box2->addItem( ls.at(i), lv.at(i) );
        }
        box1->setCurrentIndex(indx2);
        box2->setCurrentIndex(indx1);
    }
    else if(className == "QLineEdit") {

        QLineEdit *ed1 = dynamic_cast<QLineEdit*>(w1);
        QLineEdit *ed2 = dynamic_cast<QLineEdit*>(w2);

        QString str = ed1->text();
        ed1->setText(ed2->text());
        ed2->setText(str);
    }
    else if(className == "QSpinBox") {

        QSpinBox *sb1 = dynamic_cast<QSpinBox*>(w1);
        QSpinBox *sb2 = dynamic_cast<QSpinBox*>(w2);

        int value = sb1->value();
        int minimum = sb1->minimum();
        int maximum = sb1->maximum();
        QString suffix = sb1->suffix();
        QString prefix = sb1->prefix();

        sb1->setMinimum( sb2->minimum() );
        sb1->setMaximum( sb2->maximum() );
        sb1->setSuffix( sb2->suffix() );
        sb1->setPrefix( sb2->prefix() );
        sb1->setValue( sb2->value() );

        sb2->setMinimum( minimum );
        sb2->setMaximum( maximum );
        sb2->setSuffix( suffix );
        sb2->setPrefix( prefix );
        sb2->setValue( value );
    }
    else if(className == "ExSpinBox") {

        ExSpinBox *sb1 = dynamic_cast<ExSpinBox*>(w1);
        ExSpinBox *sb2 = dynamic_cast<ExSpinBox*>(w2);

        int value = sb1->value();
        int minimum = sb1->minimum();
        int maximum = sb1->maximum();
        QString suffix = sb1->suffix();
        QString prefix = sb1->prefix();
        bool lz = sb1->leadingZero();

        sb1->setMinimum( sb2->minimum() );
        sb1->setMaximum( sb2->maximum() );
        sb1->setSuffix( sb2->suffix() );
        sb1->setPrefix( sb2->prefix() );
        sb1->setValue( sb2->value() );
        sb1->setLeadingZero( sb2->leadingZero() );

        sb2->setMinimum( minimum );
        sb2->setMaximum( maximum );
        sb2->setSuffix( suffix );
        sb2->setPrefix( prefix );
        sb2->setValue( value );
        sb2->setLeadingZero( lz );
    }
    else if(className == "QDoubleSpinBox") {

        QDoubleSpinBox *sb1 = dynamic_cast<QDoubleSpinBox*>(w1);
        QDoubleSpinBox *sb2 = dynamic_cast<QDoubleSpinBox*>(w2);

        double value = sb1->value();
        double minimum = sb1->minimum();
        double maximum = sb1->maximum();
        int decimals = sb1->decimals();
        QString suffix = sb1->suffix();
        QString prefix = sb1->prefix();

        sb1->setMinimum( sb2->minimum() );
        sb1->setMaximum( sb2->maximum() );
        sb1->setDecimals( sb2->decimals() );
        sb1->setSuffix( sb2->suffix() );
        sb1->setPrefix( sb2->prefix() );
        sb1->setValue( sb2->value() );

        sb2->setMinimum( minimum );
        sb2->setMaximum( maximum );
        sb2->setDecimals(decimals );
        sb2->setSuffix( suffix );
        sb2->setPrefix( prefix );
        sb2->setValue( value );
    }
    else if(className == "QCheckBox") {

        QCheckBox *cb1 = dynamic_cast<QCheckBox*>(w1);
        QCheckBox *cb2 = dynamic_cast<QCheckBox*>(w2);

        QString text = cb1->text();
        bool checked = cb1->isChecked();
        cb1->setText(cb2->text());
        cb1->setChecked(cb2->isChecked());
        cb2->setText(text);
        cb2->setChecked(checked);
    }

}
//============================================================================================
bool Gui::saveControlValue(QWidget *w, const QString &fileName, const QString &prefixKey)
{
    if(!w) return false;
    QSettings ini(fileName, QSettings::IniFormat);
    if(!ini.isWritable()) return false;
    ini.setIniCodec("UTF-8");

    QString section = "TopParent", key = prefixKey + w->objectName();
    QWidget* tmpW = w->parentWidget();
    while(tmpW) {
        if(!tmpW->parentWidget()) section = tmpW->objectName();
        tmpW = tmpW->parentWidget();
    }

    QString className = QString(w->metaObject()->className());

    if(className == "QComboBox") {
        QComboBox* box = dynamic_cast<QComboBox*>(w);
        if(!box) return false;
        ini.setValue(section + "/" + key, box->currentText() );
    }
    else if(className == "QLineEdit") {
        QLineEdit* le = dynamic_cast<QLineEdit*>(w);
        if(!le) return false;
        ini.setValue(section + "/" + key, le->text() );
    }
    else if((className == "QSpinBox") || (className == "ExSpinBox")) {
        QSpinBox* sb = dynamic_cast<QSpinBox*>(w);
        if(!sb) return false;
        ini.setValue(section + "/" + key, sb->value() );
    }
    else if(className == "QDoubleSpinBox") {
        QDoubleSpinBox* db = dynamic_cast<QDoubleSpinBox*>(w);
        if(!db) return false;
        ini.setValue(section + "/" + key, db->value() );
    }
    else if(className == "QCheckBox") {
        QCheckBox* cb = dynamic_cast<QCheckBox*>(w);
        if(!cb) return false;
        ini.setValue(section + "/" + key, cb->isChecked() );
    }
    else if(className == "QRadioButton") {
        QRadioButton* rb = dynamic_cast<QRadioButton*>(w);
        if(!rb) return false;
        ini.setValue(section + "/" + key, rb->isChecked() );
    }
    else if((className == "QDateEdit") || (className == "QTimeEdit") || (className == "QDateTimeEdit")) {
        QDateTimeEdit* de = dynamic_cast<QDateTimeEdit*>(w);
        if(!de) return false;
        ini.setValue(section + "/" + key, de->dateTime() );
    }
    else return false;

    ini.sync();
    return (ini.status() == QSettings::NoError);
}
//============================================================================================
bool Gui::loadControlValue(QWidget *w, const QString &fileName, const QString &prefixKey)
{
    if(!w) return false;
    QSettings ini(fileName, QSettings::IniFormat);
    ini.setIniCodec("UTF-8");

    QString section = "TopParent", key = prefixKey + w->objectName();
    QWidget* tmpW = w->parentWidget();
    while(tmpW) {
        if(!tmpW->parentWidget()) section = tmpW->objectName();
        tmpW = tmpW->parentWidget();
    }

    if(!ini.contains(section + "/" + key)) return false;

    QString className = QString(w->metaObject()->className());

    if(className == "QComboBox") {
        QComboBox* box = dynamic_cast<QComboBox*>(w);
        if(!box) return false;
        QString str = ini.value(section + "/" + key, box->currentText() ).toString();
        for(auto i=0; i<box->count(); ++i) {
            if(box->itemText(i) == str) {
                box->setCurrentIndex(i);
                return true;
            }
        }
    }
    else if(className == "QLineEdit") {
        QLineEdit* le = dynamic_cast<QLineEdit*>(w);
        if(!le) return false;
        le->setText( ini.value(section + "/" + key, le->text() ).toString());
        return true;
    }
    else if((className == "QSpinBox") || (className == "ExSpinBox")) {
        QSpinBox* sb = dynamic_cast<QSpinBox*>(w);
        if(!sb) return false;
        sb->setValue( ini.value(section + "/" + key, sb->value() ).toInt() );
        return true;
    }
    else if(className == "QDoubleSpinBox") {
        QDoubleSpinBox* db = dynamic_cast<QDoubleSpinBox*>(w);
        if(!db) return false;
        db->setValue( ini.value(section + "/" + key, db->value() ).toDouble() );
        return true;
    }
    else if(className == "QCheckBox") {
        QCheckBox* cb = dynamic_cast<QCheckBox*>(w);
        if(!cb) return false;
        cb->setChecked( ini.value(section + "/" + key, cb->isChecked() ).toBool() );
        return true;
    }
    else if(className == "QRadioButton") {
        QRadioButton* rb = dynamic_cast<QRadioButton*>(w);
        if(!rb) return false;
        rb->setChecked( ini.value(section + "/" + key, rb->isChecked() ).toBool() );
    }
    else if((className == "QDateEdit") || (className == "QTimeEdit") || (className == "QDateTimeEdit")) {
        QDateTimeEdit* de = dynamic_cast<QDateTimeEdit*>(w);
        if(!de) return false;
        de->setDateTime( ini.value(section + "/" + key, de->dateTime() ).toDateTime() );
        return true;
    }

    return false;
}
//============================================================================================
void Gui::msgError(const QString &txt)
{
    QMessageBox::critical(getMainWindowWidget(), QObject::tr("Ошибка"), txt, QMessageBox::Ok);
}
//============================================================================================
void Gui::msgWarning(const QString &txt)
{
    QMessageBox::warning(getMainWindowWidget(), QObject::tr("Внимание"), txt, QMessageBox::Ok);
}
//============================================================================================
void Gui::msgInfo(const QString &txt)
{
    QMessageBox::information(getMainWindowWidget(), QObject::tr("Информация"), txt, QMessageBox::Ok);
}
//============================================================================================
bool Gui::msgConfirm(const QString &txt)
{
    return QMessageBox::question(getMainWindowWidget(), QObject::tr("Подтверждение"), txt,
                                 QMessageBox::Yes, QMessageBox::Cancel) == QMessageBox::Yes;
}
//============================================================================================
int Gui::msgDialog(const QString &txt)
{
    return QMessageBox::question(getMainWindowWidget(), QObject::tr("Вопрос"), txt,
                                 QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
}
//============================================================================================
void Gui::msgError(QWidget *parent, const QString &txt)
{
    QMessageBox::critical(parent, QObject::tr("Ошибка"), txt, QMessageBox::Ok);
}
//============================================================================================
void Gui::msgWarning(QWidget *parent, const QString &txt)
{
    QMessageBox::warning(parent, QObject::tr("Внимание"), txt, QMessageBox::Ok);
}
//============================================================================================
void Gui::msgInfo(QWidget *parent, const QString &txt)
{
    QMessageBox::information(parent, QObject::tr("Информация"), txt, QMessageBox::Ok);
}
//============================================================================================
bool Gui::msgConfirm(QWidget *parent, const QString &txt)
{
    return QMessageBox::question(parent, QObject::tr("Подтверждение"), txt,
                                 QMessageBox::Yes, QMessageBox::Cancel) == QMessageBox::Yes;
}
//============================================================================================
int Gui::msgDialog(QWidget *parent, const QString &txt)
{
    return QMessageBox::question(parent, QObject::tr("Вопрос"), txt,
                                 QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
}
//============================================================================================
QWidget* Gui::getMainWindowWidget()
{
    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        if ( QString(widget->metaObject()->className()) == QString("MainWindow")) return widget;
    }
    return nullptr;
}
//============================================================================================
void Gui::clearWidgets(QLayout *layout)
{
    if(!layout) return;
    while (QLayoutItem* item = layout->takeAt(0)) {
        delete item->widget();
        delete item;
    }
}
//============================================================================================
} // namespace nayk

