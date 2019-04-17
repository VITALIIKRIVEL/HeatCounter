#include "formconnectionparams.h"
#include "ui_formconnectionparams.h"

FormConnectionParams::FormConnectionParams(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormConnectionParams)
{
    ui->setupUi(this);
}

FormConnectionParams::~FormConnectionParams()
{
    delete ui;
}
