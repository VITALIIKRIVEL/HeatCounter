#include "formviewtable.h"
#include "ui_formviewtable.h"

FormViewTable::FormViewTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormViewTable)
{
    ui->setupUi(this);

    ui->checkBox_allNotes->setChecked(false);
    on_checkBox_allNotes_clicked(false);
    ui->checkBox_allNotes->setChecked(false);
    ui->checkBox_allNotes->setVisible(false);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //заполняем комбобокс типов теплосчётчика

    ui->comboBox_type->addItem("Все");

    ui->comboBox_type->addItem("Гефест 06.V1 (00)", "00"); 	//СЭТ.469333.100-00
    ui->comboBox_type->addItem("Гефест 06.V1.М (02)", "02"); 	//СЭТ.469333.100-02

    ui->comboBox_type->addItem("Гефест 06.V1.I (04)", "04"); 	//СЭТ.469333.100-04

    ui->comboBox_type->addItem("Гефест 06.V1.IM (06)", "06"); 	//СЭТ.469333.100-06

    ui->comboBox_type->addItem("Гефест 15.V2 (08)", "08"); 	//СЭТ.469333.100-08
    ui->comboBox_type->addItem("Гефест 15.V2.М (10)", "10"); 	//СЭТ.469333.100-10

    ui->comboBox_type->addItem("Гефест 15.V2.I (12)", "12"); 	//СЭТ.469333.100-12

    ui->comboBox_type->addItem("Гефест 15.V2.IM (14)", "14"); 	//СЭТ.469333.100-14

    ui->comboBox_type->addItem("Гефест 06.V1.R (16)", "16"); 	//СЭТ.469333.100-16
    ui->comboBox_type->addItem("Гефест 06.V1.IR (18)", "18");  	//СЭТ.469333.100-18

    ui->comboBox_type->addItem("Гефест 15.V2.R (20)", "20"); 	//СЭТ.469333.100-20
    ui->comboBox_type->addItem("Гефест 15.V2.IR (22)", "22"); 	//СЭТ.469333.100-22

    ui->comboBox_type->addItem("Гефест 06.V1.F (24)", "24"); 	//СЭТ.469333.100-24
    ui->comboBox_type->addItem("Гефест 06.V1.IF (26)", "26"); 	//СЭТ.469333.100-26

    ui->comboBox_type->addItem("Гефест 15.V2.F (28)", "28"); 	//СЭТ.469333.100-28
    ui->comboBox_type->addItem("Гефест 15.V2.IF (30)", "30"); 	//СЭТ.469333.100-30

    ui->comboBox_type->addItem("Гефест 06.V1.N ()"); 	//СЭТ.469333.100-72
    ui->comboBox_type->addItem("Гефест 06.V1.N.Км"); 	//СЭТ.469333.100-72.01
    ui->comboBox_type->addItem("Гефест 06.V1.N.Кр"); 	//СЭТ.469333.100-72.02
    ui->comboBox_type->addItem("Гефест 06.V1.N.КмКр"); 	//СЭТ.469333.100-72.03
    ui->comboBox_type->addItem("Гефест 15.V2.N"); 	//СЭТ.469333.100-73
    ui->comboBox_type->addItem("Гефест 15.V2.N.Км"); 	//СЭТ.469333.100-73.01
    ui->comboBox_type->addItem("Гефест 15.V2.N.Кр"); 	//СЭТ.469333.100-73.02
    ui->comboBox_type->addItem("Гефест 15.V2.N.КмКр"); 	//СЭТ.469333.100-73.03
    ui->comboBox_type->addItem("Гефест 06.V1.L"); 	//СЭТ.469333.100-xx
    ui->comboBox_type->addItem("Гефест 06.V1.IL"); 	//СЭТ.469333.100-xx
    ui->comboBox_type->addItem("Гефест 15.V2.L"); 	//СЭТ.469333.100-xx
    ui->comboBox_type->addItem("Гефест 15.V2.IL"); 	//СЭТ.469333.100-xx


    ui->comboBox_usable->addItem(tr("Все"));
    ui->comboBox_usable->addItem(tr("Годен"));
    ui->comboBox_usable->addItem(tr("Не годен"));

    //заполняем комбобокс типов теплосчётчика/

}

FormViewTable::~FormViewTable()
{
    delete ui;
}

void FormViewTable::slotGetTableList(QStringList tableList, QString userName, QMap<int, QString> userMap,
                                     QStringList userList, QList<int> intList)
{

    //перевернуть карту - значения и ключи поменять местами

    for(int n=0; n<intList.size(); n++) {
        int intTmp = intList.at(n);
        QString str = userMap[intTmp];

        userList.append(str);

        mapWorkUser[str] = intList.at(n);
    }

    //перевернуть карту - значения и ключи поменять местами/

    //

    //заполняем комбобоксы

//    ui->comboBox_user->clear();
//    ui->comboBox_user->addItems(userList);

    //заполняем комбобоксы/

    ui->tableWidget->clear();

    while(ui->tableWidget->rowCount()>0) {
        ui->tableWidget->removeRow(0);
    }

    //инициализация таблицы 1
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->verticalHeader()->resizeSection(0, 15);
    ui->tableWidget->horizontalHeader()->resizeSection(0, 25);
    for(int i=1; i<9; i++) {
        ui->tableWidget->horizontalHeader()->resizeSection(i, 60);
    }

    ui->tableWidget->horizontalHeader()->resizeSection(1, 90);
    ui->tableWidget->horizontalHeader()->resizeSection(2, 130);
    ui->tableWidget->horizontalHeader()->resizeSection(3, 150);
    ui->tableWidget->horizontalHeader()->resizeSection(4, 90);
    ui->tableWidget->horizontalHeader()->resizeSection(5, 90);
    ui->tableWidget->horizontalHeader()->resizeSection(6, 70);
    ui->tableWidget->horizontalHeader()->resizeSection(7, 110);


//    for(int i=1; i<numberOfChannels; i++) {
//        ui->tableWidget->verticalHeader()->resizeSection(i, 15);
//    }

    QTableWidgetItem *element;

    element = new QTableWidgetItem("№");
    ui->tableWidget->setHorizontalHeaderItem(0, element);
    element = new QTableWidgetItem(tr("Сер.номер"));
    ui->tableWidget->setHorizontalHeaderItem(1, element);
    element = new QTableWidgetItem(tr("Номер ЭБ"));
    ui->tableWidget->setHorizontalHeaderItem(2, element);
    element = new QTableWidgetItem(tr("Дата/время записи"));
    ui->tableWidget->setHorizontalHeaderItem(3, element);
    element = new QTableWidgetItem(tr("Rref1"));
    ui->tableWidget->setHorizontalHeaderItem(4, element);
    element = new QTableWidgetItem(tr("Rref2"));
    ui->tableWidget->setHorizontalHeaderItem(5, element);
    element = new QTableWidgetItem(tr("Годность"));
    ui->tableWidget->setHorizontalHeaderItem(6, element);
    element = new QTableWidgetItem(tr("Инженер"));
    ui->tableWidget->setHorizontalHeaderItem(7, element);

    //
    //

    for (int var = 0; var < tableList.size(); ++var) {

        ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);

        element = new QTableWidgetItem(QString::number(var + 1));
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, element);

        //берём одну строку листа, делаем из нее тоже лист
        QStringList listCurrentRow = tableList.at(var).split(';');

        int size = listCurrentRow.size();

        if(listCurrentRow.size()<4) {
            for(int m=0; m<(4 - size); m++) {
                listCurrentRow.append(QString());
            }
        }

        //серийный номер
        element = new QTableWidgetItem(listCurrentRow.at(0));
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, element);

        //номер PCB
        element = new QTableWidgetItem(listCurrentRow.at(1));
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, element);

        QString typeStr = listCurrentRow.at(1);//тип теплосчетчика - второй байт pcb
        typeStr.remove(0, 2);
        typeStr = typeStr.left(2);

        //время/дата записи
        QString timeStr = listCurrentRow.at(2);
        timeStr.remove(timeStr.length() - 4, 4);

        QString dateStrTmp = timeStr.left(10);
        QStringList dateListTmp = dateStrTmp.split('-');
        QString timeStrDateReverse;

        QDate dateTmp;

        if(dateListTmp.size()>=3) {
           dateTmp.setDate(dateListTmp.at(0).toInt(), dateListTmp.at(1).toInt(), dateListTmp.at(2).toInt());

           QStringList timeListTwoElements = timeStr.split('T');
           timeStrDateReverse = dateListTmp.at(2) + "-" + dateListTmp.at(1) + "-" +
                                        dateListTmp.at(0) + "  " + timeListTwoElements.at(1);

        }

        element = new QTableWidgetItem(timeStrDateReverse); //(timeStr);
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, element);

        //результат проверки, разбиваем строку Json на rref1, rref2, usable, engineer
        QString jsonStr =listCurrentRow.at(3);

        jsonStr.remove('{');
        jsonStr.remove('}');
        jsonStr.remove('"');

        QStringList jsonList = jsonStr.split(',');

        int listSize = jsonList.size();

        if(jsonList.size()<4) {
            for(int m=0; m<(4 - listSize); m++) {
                jsonList.append(QString());
            }
        }

        //rref1
        QString rref1Str = jsonList.at(0);
        rref1Str.remove("Rref1: ");

        //
        if(!rref1Str.isEmpty() && rref1Str.contains('.')) {

           QStringList rref1StrListTwoElements = rref1Str.split('.');
           QString rref1FirstPartStr =  rref1StrListTwoElements.at(0);
           QString rref1SecondPartStr = rref1StrListTwoElements.at(1);

           if(rref1SecondPartStr.length()<6) { //дополняем нулями дробную часть
               int lenSecondpart = rref1SecondPartStr.length();
               int difference = 6 - lenSecondpart;

               for(int c=0; c<difference; c++) {
                   rref1SecondPartStr.append('0');
               }

               rref1Str = rref1FirstPartStr + "." + rref1SecondPartStr;

           }

        }
        //

        element = new QTableWidgetItem(rref1Str);
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, element);

        //rref2
        QString rref2Str = jsonList.at(1);
        rref2Str.remove("Rref2: ");

        //
        if(!rref2Str.isEmpty() && rref2Str.contains('.')) {

           QStringList rref2StrListTwoElements = rref2Str.split('.');
           QString rref2FirstPartStr =  rref2StrListTwoElements.at(0);
           QString rref2SecondPartStr = rref2StrListTwoElements.at(1);

           if(rref2SecondPartStr.length()<6) { //дополняем нулями дробную часть
               int lenSecondpart = rref2SecondPartStr.length();
               int difference = 6 - lenSecondpart;

               for(int c=0; c<difference; c++) {
                   rref2SecondPartStr.append('0');
               }

               rref2Str = rref2FirstPartStr + "." + rref2SecondPartStr;

           }

        }
        //

        element = new QTableWidgetItem(rref2Str);
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 5, element);

        //usable
//        QString usableStr = jsonList.at(2);
//        usableStr.remove("Usable: ");

        QString usableStr;

        if(listCurrentRow.at(5) == "1") usableStr = tr("Годен");
        if(listCurrentRow.at(5) == "0") usableStr = tr("Не годен");


        element = new QTableWidgetItem(usableStr);
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 6, element);

        //engineer
 //       QString engineerStr = jsonList.at(3);
 //       engineerStr.remove("Engineer: ");

        QString engineerStr = listCurrentRow.at(4);

        element = new QTableWidgetItem(engineerStr);
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 7, element);


        //если дата последней сформированной строки не попадает в интервал, тогда удаляем её
        if( !isAllNotesDisplay  || !dateTmp.isValid() ) {

//            int dateFragment1 = dateStart.daysTo(dateTmp);
//            int dateFragment2 = dateTmp.daysTo(dateStop);

//            bool elem1 = !(dateStart.daysTo(dateTmp)>=0 && dateTmp.daysTo(dateStop)>=0 && !ui->checkBox_date->isChecked());
//            bool elem2 = !dateTmp.isValid();
//            bool elem3 = usableStr != usableStrGlobal && ui->comboBox_usable->currentText() != "Все";
//            bool elem4 = engineerStr != userStrGlobal && ui->comboBox_user->currentText() != "Все";
//            bool elem5 = typeStr != typeCounterStr && ui->comboBox_type->currentText() != "Все";



            if( ( !(dateStart.daysTo(dateTmp)>=0 && dateTmp.daysTo(dateStop)>=0) && ui->checkBox_date->isChecked() ) ||
                 !dateTmp.isValid() ||
                 (usableStr != usableStrGlobal && ui->comboBox_usable->currentText() != "Все" ) ||
                 (engineerStr != userStrGlobal && ui->comboBox_user->currentText() != "Все") ||
                 (typeStr != typeCounterStr && ui->comboBox_type->currentText() != "Все") ) {

                     ui->tableWidget->removeRow(ui->tableWidget->rowCount() - 1);

            }
        }


    }



}

void FormViewTable::on_toolButton_updateTable_clicked()
{
   emit updateTable();
}

void FormViewTable::on_dateEdit_start_dateChanged(const QDate &date)
{
    dateStart = date;
}

void FormViewTable::on_dateEdit_stop_dateChanged(const QDate &date)
{
    dateStop = date;
}

void FormViewTable::on_checkBox_allNotes_clicked()
{

}

void FormViewTable::on_checkBox_allNotes_clicked(bool checked)
{
    isAllNotesDisplay = checked;
}

void FormViewTable::on_comboBox_user_currentIndexChanged(const QString &arg1)
{
   // userId = mapWorkUser[arg1];

    userStrGlobal = arg1;
}

void FormViewTable::on_comboBox_type_currentIndexChanged(const QString &arg1)
{
    typeCounterStr = ui->comboBox_type->currentData().toString();
}

void FormViewTable::on_comboBox_usable_currentIndexChanged(const QString &arg1)
{
    usableStrGlobal = arg1;
}

void FormViewTable::slotGetUserList(QStringList userTable)
{
    QStringList list;

    for(int u=0; u<userTable.size(); u++) {

        QStringList curRowList = userTable.at(u).split(";");
        list.append(curRowList.at(0));

    }

    ui->comboBox_user->addItem(tr("Все"));
    ui->comboBox_user->addItems(list);
}

void FormViewTable::on_toolButton_filtersReset_clicked()
{
    ui->checkBox_date->setChecked(false);
    on_checkBox_date_clicked(false);

    ui->comboBox_type->setCurrentIndex(0);
    on_comboBox_type_currentIndexChanged(ui->comboBox_type->currentText());

    ui->comboBox_usable->setCurrentIndex(0);
    on_comboBox_usable_currentIndexChanged(ui->comboBox_usable->currentText());

    ui->comboBox_user->setCurrentIndex(0);
    on_comboBox_user_currentIndexChanged(ui->comboBox_user->currentText());


}

void FormViewTable::on_checkBox_date_clicked(bool checked)
{
    isDateUse = checked;
}
