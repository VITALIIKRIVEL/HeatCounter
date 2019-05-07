#include "dialogisrunwithoutconnect.h"
#include "ui_dialogisrunwithoutconnect.h"

DialogIsRunWithoutConnect::DialogIsRunWithoutConnect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogIsRunWithoutConnect)
{
    ui->setupUi(this);
}

DialogIsRunWithoutConnect::~DialogIsRunWithoutConnect()
{
    delete ui;
}

void DialogIsRunWithoutConnect::setText(QString str)
{
    ui->label_error->setText(str);
}
