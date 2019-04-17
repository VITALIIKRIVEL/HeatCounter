#include "dialoglogindatabase.h"
#include "ui_dialoglogindatabase.h"

DialogLoginDataBase::DialogLoginDataBase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLoginDataBase)
{
    ui->setupUi(this);

    isRememberLogin = true; //false;

    ui->checkBox_rememberLogin->setChecked(true);
    on_checkBox_rememberLogin_clicked(true);

    ui->lineEdit_loginDialog->setVisible(false);
    ui->checkBox_rememberLogin->setVisible(false);

//    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint);
}

DialogLoginDataBase::~DialogLoginDataBase()
{
    delete ui;
}

void DialogLoginDataBase::closeEvent(QCloseEvent *event)
{
    emit signalClose();
}

void DialogLoginDataBase::on_toolButton_entrance_clicked()
{
    emit sendLoginPassword(ui->comboBox->currentText(),
                           ui->lineEdit_passwordDialog->text(), isRememberLogin);
}



void DialogLoginDataBase::on_checkBox_rememberLogin_clicked()
{

}

void DialogLoginDataBase::on_checkBox_rememberLogin_clicked(bool checked)
{
    isRememberLogin = checked;
}

void DialogLoginDataBase::slotGetLogPasFromSettings(QString log, QString pas)
{

  loadedUserFromSettings = log;

  if(userList.contains(log)) ui->comboBox->setCurrentText(log);

  //  ui->lineEdit_loginDialog->setText(log);
  // ui->comboBox->currentText();
  //  ui->lineEdit_passwordDialog->setText(pas);

}

void DialogLoginDataBase::slotGetUserList(QStringList list)
{
    ui->comboBox->addItems(list);

    userList = list;

 //   if(list.contains(loadedUserFromSettings)) ui->comboBox->setCurrentText(loadedUserFromSettings);
}












