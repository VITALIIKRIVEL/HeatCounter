#include "dialogisrunwithoutconnect.h"
#include "ui_dialogisrunwithoutconnect.h"
#include <QPushButton>

DialogIsRunWithoutConnect::DialogIsRunWithoutConnect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogIsRunWithoutConnect)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Отмена"));
}

DialogIsRunWithoutConnect::~DialogIsRunWithoutConnect()
{
    delete ui;
}

void DialogIsRunWithoutConnect::setText(QString str)
{
    ui->label_error->setText(str);
}
