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

void FormConnectionParams::on_toolButton_OK_clicked()
{
    QStringList paramsList;
    paramsList.append(ui->lineEdit_server->text());
    paramsList.append(ui->lineEdit_dataBase->text());
    paramsList.append(ui->lineEdit_user->text());
    paramsList.append(ui->lineEdit_password->text());

    emit sendParamsConnection(paramsList);

    this->close();
}

void FormConnectionParams::slotGetConnectParamsFromMW(QStringList list)
{

    ui->lineEdit_server->setText(list.at(0));
    ui->lineEdit_dataBase->setText(list.at(1));
    ui->lineEdit_user->setText(list.at(2));
    ui->lineEdit_password->setText(list.at(3));
}
