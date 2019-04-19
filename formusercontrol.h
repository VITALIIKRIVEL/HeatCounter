#ifndef FORMUSERCONTROL_H
#define FORMUSERCONTROL_H

#include <QWidget>

namespace Ui {
class Formusercontrol;
}

class Formusercontrol : public QWidget
{
    Q_OBJECT

public:
    explicit Formusercontrol(QWidget *parent = nullptr);
    ~Formusercontrol();

private:
    Ui::Formusercontrol *ui;
};

#endif // FORMUSERCONTROL_H
