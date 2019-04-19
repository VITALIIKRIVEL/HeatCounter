#include "formusercontrol.h"
#include "ui_formusercontrol.h"

Formusercontrol::Formusercontrol(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Formusercontrol)
{
    ui->setupUi(this);
}

Formusercontrol::~Formusercontrol()
{
    delete ui;
}
