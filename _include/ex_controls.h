#ifndef EX_CONTROLS_H
#define EX_CONTROLS_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QDateEdit>
#include <QTimeEdit>
#include <QDateTimeEdit>
#include <QKeyEvent>

//=========================================================================================================
namespace nayk {
//=========================================================================================================
class ExSpinBox: public QSpinBox
{
    Q_OBJECT

    Q_PROPERTY(int leadingZero READ leadingZero WRITE setLeadingZero)

public:
    explicit ExSpinBox( QWidget * parent = nullptr) : QSpinBox(parent) { }

    bool leadingZero() const { return _lZero; }
    void setLeadingZero(bool lZero);
    virtual QString textFromValue ( int value ) const;

signals:
    void returnPressed();
    void keyPressed(int key);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    bool _lZero {false};
};
//=========================================================================================================


//=========================================================================================================
} // namespace nayk

//=========================================================================================================
#endif // EX_CONTROLS_H
