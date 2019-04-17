#include <QApplication>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QLayoutItem>
//
#include "ex_controls.h"

namespace nayk {
//===================================================================================================
// ExSpinBox
//===================================================================================================
void ExSpinBox::setLeadingZero(bool lZero)
{
    _lZero = lZero;
    this->update();
}
//===================================================================================================
QString ExSpinBox::textFromValue ( int value ) const
{
    if(_lZero) {
        QString s = QString::number(maximum(), displayIntegerBase());
        return QString("%1").arg(value, s.length() , displayIntegerBase(), QChar('0')).toUpper();
    }
    else
        return QString::number(value, displayIntegerBase()).toUpper();
}
//===================================================================================================
void ExSpinBox::keyPressEvent(QKeyEvent *event)
{
    emit keyPressed( event->key() );
    if(event->key() == Qt::Key_Return) emit returnPressed();
    QSpinBox::keyPressEvent(event);
}
//===================================================================================================


//===================================================================================================
} // namespace nayk
