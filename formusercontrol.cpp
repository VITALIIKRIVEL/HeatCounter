#include "formusercontrol.h"
#include "ui_formusercontrol.h"
#include <QMessageBox>
#include "global.h"

Formusercontrol::Formusercontrol(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Formusercontrol)
{
    ui->setupUi(this);

    dialogSaveUserChanges = new DialogSaveUserChanges();
    dialogSaveUserChanges->setModal(true);

    ui->tableWidget_userTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->comboBox_noteType->addItem(tr("Администратор"), 1);
    ui->comboBox_noteType->addItem(tr("Оператор"), 2);
    ui->comboBox_noteType->addItem(tr("Пользователь"), 3);

    ui->checkBox_isActive->setChecked(true);
    on_checkBox_isActive_clicked(true);

    on_lineEdit_password_textChanged("");
    on_lineEdit_passwordRepeat_textChanged("");

    vectorToQuery.resize(4);
    vectorToQuery.fill("");

    connect(this, SIGNAL(sendTextToDialog(QString)), dialogSaveUserChanges, SLOT(slotGetText(QString)));
}

Formusercontrol::~Formusercontrol()
{
    delete ui;
}

 void Formusercontrol::slotGetListUserTable(QStringList list)
 {
    QStringList tableList = list;

    allUserTable = list;

    ui->tableWidget_userTable->clear();

    while(ui->tableWidget_userTable->rowCount()>0) {
        ui->tableWidget_userTable->removeRow(0);
    }

    //инициализация таблицы
    ui->tableWidget_userTable->setColumnCount(4);
    ui->tableWidget_userTable->verticalHeader()->hide();
    ui->tableWidget_userTable->verticalHeader()->resizeSection(0, 15);
    ui->tableWidget_userTable->horizontalHeader()->resizeSection(0, 25);
    for(int i=1; i<5; i++) {
        ui->tableWidget_userTable->horizontalHeader()->resizeSection(i, 100);
    }

    ui->tableWidget_userTable->horizontalHeader()->resizeSection(1, 110);
    ui->tableWidget_userTable->horizontalHeader()->resizeSection(2, 130);
    ui->tableWidget_userTable->horizontalHeader()->resizeSection(3, 100);
//    ui->tableWidget_userTable->horizontalHeader()->resizeSection(4, 100);
    //

    QTableWidgetItem *element;

    element = new QTableWidgetItem("№");
    ui->tableWidget_userTable->setHorizontalHeaderItem(0, element);
    element = new QTableWidgetItem(tr("ФИО"));
    ui->tableWidget_userTable->setHorizontalHeaderItem(1, element);
    element = new QTableWidgetItem(tr("Тип учётной записи"));
    ui->tableWidget_userTable->setHorizontalHeaderItem(2, element);
    element = new QTableWidgetItem(tr("Статус"));
    ui->tableWidget_userTable->setHorizontalHeaderItem(3, element);
    //

    for (int var = 0; var < tableList.size(); ++var) {

        ui->tableWidget_userTable->setRowCount(ui->tableWidget_userTable->rowCount() + 1);

        element = new QTableWidgetItem(QString::number(var + 1));
        ui->tableWidget_userTable->setItem(ui->tableWidget_userTable->rowCount() - 1, 0, element);

        //берём одну строку листа, делаем из нее тоже лист
        QStringList listCurrentRow = tableList.at(var).split(';');

        int size = listCurrentRow.size();

        if(listCurrentRow.size()<3) {
            for(int m=0; m<(3 - size); m++) {
                listCurrentRow.append(QString());
            }
        }

        //пользователь
        element = new QTableWidgetItem(listCurrentRow.at(0));
        ui->tableWidget_userTable->setItem(ui->tableWidget_userTable->rowCount() - 1, 1, element);

        //тип учётной записи
        QString typeStr;

        if(listCurrentRow.at(2) == "1") typeStr = "Администратор";
        if(listCurrentRow.at(2) == "2") typeStr = "Оператор";
        if(listCurrentRow.at(2) == "3") typeStr = "Пользователь";

        element = new QTableWidgetItem(typeStr);
        ui->tableWidget_userTable->setItem(ui->tableWidget_userTable->rowCount() - 1, 2, element);

        //статус
        QString statusStr;

        if(listCurrentRow.at(3) == "1") statusStr = tr("Активен");
        if(listCurrentRow.at(3) == "0") statusStr = tr("Выключен");

        element = new QTableWidgetItem(statusStr); //(timeStr);
        ui->tableWidget_userTable->setItem(ui->tableWidget_userTable->rowCount() - 1, 3, element);

    }


 }

void Formusercontrol::on_tableWidget_userTable_cellClicked(int row, int column)
{
    ui->lineEdit_password->clear();
    ui->lineEdit_passwordRepeat->clear();

    curUser = ui->tableWidget_userTable->item(row, 1)->text();
    ui->lineEdit_humanName->setText(curUser);

    int userRowPosInList;

    for(int u=0; u<allUserTable.size(); u++) {

        if(allUserTable.at(u).contains(curUser)) userRowPosInList = u;

    }
    //

    QString curStrUser = allUserTable.at(userRowPosInList);
    QStringList curListUser = curStrUser.split(";");

    curPassword = curListUser.at(1);
    curTypeOfNote = curListUser.at(2);
    curStatus = curListUser.at(3);


    for(int r=0; r<ui->comboBox_noteType->count(); r++) {

        ui->comboBox_noteType->setCurrentIndex(r);

        if(curTypeOfNote.toInt() == ui->comboBox_noteType->currentData().toInt()) {
            break;
        }

    }
    //

    if(curStatus.toInt() == 1) ui->checkBox_isActive->setChecked(true);
    if(curStatus.toInt() == 0) ui->checkBox_isActive->setChecked(false);

}

void Formusercontrol::on_lineEdit_humanName_textChanged(const QString &arg1)
{
    curUserFromLine = arg1;

 //   vectorToQuery[0] = curUserFromLine;
}

void Formusercontrol::on_lineEdit_password_textChanged(const QString &arg1)
{
    curPasswordFromLine = arg1;

  //  vectorToQuery[1] = curPasswordFromLine;
}

void Formusercontrol::on_lineEdit_passwordRepeat_textChanged(const QString &arg1)
{
    curPasswordFromLineRep = arg1;
}

void Formusercontrol::on_comboBox_noteType_currentIndexChanged(const QString &arg1)
{
    curTypeOfNoteFromCombo = QString::number(ui->comboBox_noteType->currentData().toInt());

  //  vectorToQuery[2] = curTypeOfNoteFromCombo;
}

void Formusercontrol::on_checkBox_isActive_clicked(bool checked)
{
    if(checked) {
        curStatusFromCheckBox = QString::number(1);

    }
    else {
        curStatusFromCheckBox = QString::number(0);
    }

  //  vectorToQuery[3] = curStatusFromCheckBox;
}

void Formusercontrol::on_toolButton_add_clicked()
{
    emit sendTextToDialog(tr("Добавить пользователя?"));

    global::pause(300);

    if(dialogSaveUserChanges->exec() != QDialog::Accepted) {
        return;
    }

    if(curPasswordFromLine != curPasswordFromLineRep) {
        QMessageBox::critical(this, "", tr("Не совпадают пароли"));
        return;
    }

    vectorToQuery[0] = curUserFromLine;
    vectorToQuery[1] = curPasswordFromLine;
    vectorToQuery[2] = curTypeOfNoteFromCombo;
    vectorToQuery[3] = curStatusFromCheckBox;

    emit sendVectorToQueryAdd(vectorToQuery);

}

void Formusercontrol::on_toolButton_change_clicked()
{
    emit sendTextToDialog(tr("Изменить данные пользователя?"));

    global::pause(300);

    if(dialogSaveUserChanges->exec() != QDialog::Accepted) {
        return;
    }

    if(curPasswordFromLine != curPasswordFromLineRep) {
        QMessageBox::critical(this, "", tr("Не совпадают пароли"));
        return;
    }

    vectorToQuery[0] = curUserFromLine;
    vectorToQuery[1] = curPasswordFromLine;
    vectorToQuery[2] = curTypeOfNoteFromCombo;
    vectorToQuery[3] = curStatusFromCheckBox;

    emit sendVectorToQueryChange(vectorToQuery);

}

void Formusercontrol::on_toolButton_delete_clicked()
{
    emit sendTextToDialog(tr("Удалить пользователя?"));

    global::pause(300);

    if(dialogSaveUserChanges->exec() != QDialog::Accepted) {
        return;
    }

    vectorToQuery[0] = curUserFromLine;
    vectorToQuery[1] = curPasswordFromLine;
    vectorToQuery[2] = curTypeOfNoteFromCombo;
    vectorToQuery[3] = curStatusFromCheckBox;

    emit sendVectorToQueryRemove(vectorToQuery);

}

void Formusercontrol::slotGetCurrentUser(QString user)
{

//    if(listCurrentRow.at(2) == "1") typeStr = "Администратор";
//    if(listCurrentRow.at(2) == "2") typeStr = "Оператор";
//    if(listCurrentRow.at(2) == "3") typeStr = "Пользователь";

    QString str;

    for(int m=0; m<allUserTable.size(); m++) {

        if(allUserTable.at(m).contains(user)) {
            str = allUserTable.at(m);
            break;
        }

    }

    QStringList strList = str.split(";");

    //администратор
    if(strList.at(2) == "1") {

    }

    //оператор
    if(strList.at(2) == "2") {
        this->setEnabled(false);
        ui->tableWidget_userTable->setVisible(false);
    }

    //пользователь
    if(strList.at(2) == "3") {
        this->setEnabled(false);
        ui->tableWidget_userTable->setVisible(false);
    }

}











