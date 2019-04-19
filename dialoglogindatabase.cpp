#include "dialoglogindatabase.h"
#include "ui_dialoglogindatabase.h"
#include "QMenu"

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

//    formConnectionParams = new FormConnectionParams();
//    formConnectionParams->setWindowModality(Qt::WindowModal);

    //------------МЕНЮ----------------------

    menuBar = new QMenuBar(this);
    QMenu *file = new QMenu(tr("Настройки")); //Создаем меню бар и помещаем в него первый пункт меню file
//    QMenu *counter = new QMenu(tr("Теплосчётчик")); // Добавляем дополнительный пункт edit
//    QMenu *Interface = new QMenu(tr("Интерфейсы"));

    QAction *IK_Settings = new QAction(tr("Параметры соединения"), this); // Создаем экшн для нашего меню файл с пунктом new, так-же обязательно в качестве родителя оператором this указываем наше главное окно
//    QAction *searchCounter = new QAction(tr("Поиск теплосчётчика"), this); //

    connect(IK_Settings, SIGNAL(triggered()), this, SLOT(actionIK_Settings()));
//    connect(searchCounter, SIGNAL(triggered()), this, SLOT(actionSearchCounter()));

    file->addAction(IK_Settings); // Привязка экшена к меню File

    menuBar->addMenu(file);
    menuBar->show();

    //------------МЕНЮ----------------------/


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
    ui->comboBox->clear();

    ui->comboBox->addItems(list);

    userList = list;

 //   if(list.contains(loadedUserFromSettings)) ui->comboBox->setCurrentText(loadedUserFromSettings);
}

void DialogLoginDataBase::actionIK_Settings()
{
   // formConnectionParams->show();

    emit signalShowConnectParams();
}










