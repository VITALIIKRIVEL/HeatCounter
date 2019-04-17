#include "formparamsedit.h"
#include "ui_formparamsedit.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QFileDialog>

FormParamsEdit::FormParamsEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormParamsEdit)
{
    ui->setupUi(this);

    manCodeList = QStringList()<<"SET"<<"SVT"<<"ETO";
    ui->comboBox_ManCode->addItems(manCodeList);
    devTypeList = QStringList()<<"Обратный"<<"Подающий"; //<<"04"<<"0c";
    ui->comboBox_DevType->addItems(devTypeList);
    DNList = QStringList()<<"15"<<"20";
    ui->comboBox_DN->addItems(DNList);
    pulsesMode1List = QStringList()<<"0"<<"1"<<"2"<<"5"<<"9";
    ui->comboBox_PulsesMode1->addItems(pulsesMode1List);
    ui->comboBox_PulsesMode2->addItems(pulsesMode1List);
    AverageTList = QStringList()<<"0"<<"1";
    ui->comboBox_Average_T->addItems(AverageTList);
    flowRateList = QStringList()<<"17.02"<<"33.02"<<"52.00";
    ui->comboBox_FlowRate->addItems(flowRateList);
    flowNomList = QStringList()<<"0.6"<<"1.5"<<"2.5";
    ui->comboBox_FlowNom->addItems(flowNomList);
    RTD_TypeList = QStringList()<<"Pt(W100 = 1,385)"<<"Pt(W100 = 1,391)"<<"Инд. х-ка преобр."<<"Pt(W100 = 1,375)";//"0"<<"1"<<"2"<<"3";
    ui->comboBox_RTD_Type->addItems(RTD_TypeList);
    ASW_flagList = QStringList()<<"0"<<"1";
    ui->comboBox_ASW_flag->addItems(ASW_flagList);
    ui->comboBox_ASW_flag->setCurrentIndex(1);
    ui->comboBox_ASW_flag->setCurrentIndex(0);
    BaudRateList = QStringList()<<"1200"<<"2400"<<"4800"<<"9600";
//    ui->comboBox_BaudRate->addItems(BaudRateList);

    ui->comboBox_BaudRate->addItem("1200", 0xba);
    ui->comboBox_BaudRate->addItem("2400", 0xbb);
    ui->comboBox_BaudRate->addItem("4800", 0xbc);
    ui->comboBox_BaudRate->addItem("9600", 0xbd);

//    ui->comboBox_DevType->setCurrentText("0c");

    ui->lineEdit_Flow_min->setText("0.008");
    ui->lineEdit_Flow_max->setText("3.5");

    ui->lineEdit_serial->setMaxLength(8);

    ui->spinBox_Tin_min->setRange(10, 60);
    ui->spinBox_Tin_min->setValue(30);

    ui->spinBox_Tin_max->setRange(50, 150);
    ui->spinBox_Tin_max->setValue(120);

    ui->spinBox_Tout_min->setRange(10, 60);
    ui->spinBox_Tout_min->setValue(20);

    ui->spinBox_Tout_max->setRange(50, 150);
    ui->spinBox_Tout_max->setValue(120);

    ui->spinBox_Addr ->setRange(0, 252);
    ui->spinBox_P1PrimAddr->setRange(0, 252);
    ui->spinBox_P2PrimAddr->setRange(0, 252);

    ui->spinBox_ReportData->setValue(1);
    ui->spinBox_ReportData->setValue(0);

    ui->spinBox_P1PrimAddr->setValue(1);
    ui->spinBox_P1PrimAddr->setValue(0);

    ui->spinBox_P2PrimAddr->setValue(1);
    ui->spinBox_P2PrimAddr->setValue(0);
//    reportDataByteArray.clear();
    quint8 null =0;
//    reportDataByteArray.append(null);

    //
    //set default value
    ui->comboBox_Average_T->setCurrentText("1");
    ui->lineEdit_Pressure->setText("6");
    ui->lineEdit_R_RTD->setText("1000");
    ui->lineEdit_RWire1->setText("0");
    ui->lineEdit_RWire2->setText("0");

    ui->lineEdit_coef0->setText("1.000000");
    ui->lineEdit_coef1->setText("1.000000");
    ui->lineEdit_coef2->setText("1.000000");
    ui->lineEdit_coef3->setText("1.000000");
    ui->lineEdit_coef4->setText("1.000000");
    ui->lineEdit_coef5->setText("1.000000");

    ui->lineEdit_FS_DiffThrs->setText("30");

    ui->dateTimeEdit_realClock->setDisplayFormat("dd.MM.yyyy hh:mm:ss");

    ui->lineEdit_Shift_T1->setText("0.00000");
    ui->lineEdit_Shift_T2->setText("0.00000");
    ui->lineEdit_Scale_T1->setText("1.00000");
    ui->lineEdit_Scale_T2->setText("1.00000");

    ui->spinBox_Addr->setValue(1);
    ui->comboBox_BaudRate->setCurrentText("bb");
    ui->lineEdit_HMP_Trans->setText("3fff");
    ui->lineEdit_EPP_Trans->setText("000f");
    ui->lineEdit_ProtType->setText("ff");

    ui->lineEdit_Serial_P1->setText("00000000");
    ui->lineEdit_ManCode_P1->setText("0000");
    ui->lineEdit_DevType_P1->setText("00");
    ui->lineEdit_Version_P1->setText("0");
    ui->lineEdit_Initial_value_P1->setText("0");
    ui->lineEdit_Service_P1->setText("00000000");

    ui->lineEdit_Serial_P2->setText("00000000");
    ui->lineEdit_ManCode_P2->setText("0000");
    ui->lineEdit_DevType_P2->setText("00");
    ui->lineEdit_Version_P2->setText("0");
    ui->lineEdit_Initial_value_P2->setText("0");
    ui->lineEdit_Service_P2->setText("00000000");
    //
    //

    pulsesValue1ByteArray.clear();
    pulsesValue1ByteArray.append(null);
    pulsesValue1ByteArray.append(null);

    pulsesValue2ByteArray.clear();
    pulsesValue2ByteArray.append(null);
    pulsesValue2ByteArray.append(null);

    //editable(false)
//    ui->lineEdit_Pressure->setEnabled(false);
//  //    ui->lineEdit_R_RTD->setEnabled(false);
//    ui->lineEdit_RWire1->setEnabled(false);
//    ui->lineEdit_RWire2->setEnabled(false);

//    ui->lineEdit_coef0->setEnabled(false);
//    ui->lineEdit_coef1->setEnabled(false);
//    ui->lineEdit_coef2->setEnabled(false);
//    ui->lineEdit_coef3->setEnabled(false);
//    ui->lineEdit_coef4->setEnabled(false);
//    ui->lineEdit_coef5->setEnabled(false);

//    ui->lineEdit_FS_DiffThrs->setEnabled(false);

    ui->textBrowser->setVisible(false);
    ui->toolButton_writeParams->setVisible(false);
    ui->toolButton_loadTemplate->setVisible(false);

//        ui->lineEdit_AddrHex->setVisible(false);
//        ui->lineEdit_ASW_flagHex->setVisible(false);
//        ui->lineEdit_Average_THex->setVisible(false);
//        ui->lineEdit_BaudRateHex->setVisible(false);
//        ui->lineEdit_coef0->setVisible(false);
//        ui->lineEdit_coef0Hex->setVisible(false);
//        ui->lineEdit_coef1->setVisible(false);
//        ui->lineEdit_coef1Hex->setVisible(false);
//        ui->lineEdit_coef2->setVisible(false);
//        ui->lineEdit_coef2Hex->setVisible(false);
//        ui->lineEdit_coef3->setVisible(false);
//        ui->lineEdit_coef3Hex->setVisible(false);
//        ui->lineEdit_coef4->setVisible(false);
//        ui->lineEdit_coef4Hex->setVisible(false);
//        ui->lineEdit_coef5->setVisible(false);
//        ui->lineEdit_coef5Hex->setVisible(false);
//        ui->lineEdit_DevTypeHex->setVisible(false);
//        ui->lineEdit_DevType_P1->setVisible(false);
//        ui->lineEdit_DevType_P1Hex->setVisible(false);
//        ui->lineEdit_DevType_P2->setVisible(false);
//        ui->lineEdit_DevType_P2Hex->setVisible(false);
//        ui->lineEdit_Version_P2Hex->setVisible(false);
//        ui->lineEdit_Version_P2->setVisible(false);
//        ui->lineEdit_Version_P1Hex->setVisible(false);
//        ui->lineEdit_Version_P1->setVisible(false);
//        ui->lineEdit_Tout_minHex->setVisible(false);
//        ui->lineEdit_Tout_maxHex->setVisible(false);
//        ui->lineEdit_Tin_minHex->setVisible(false);
//        ui->lineEdit_Tin_maxHex->setVisible(false);
//        ui->lineEdit_Shift_T2Hex->setVisible(false);
//        ui->lineEdit_Shift_T2->setVisible(false);
//        ui->lineEdit_Shift_T1Hex->setVisible(false);
//        ui->lineEdit_Shift_T1->setVisible(false);
//        ui->lineEdit_Service_P2Hex->setVisible(false);
//        ui->lineEdit_Service_P2->setVisible(false);
//        ui->lineEdit_Service_P1Hex->setVisible(false);
//        ui->lineEdit_Service_P1->setVisible(false);
//        ui->lineEdit_serialHex->setVisible(false);
//        ui->lineEdit_Serial_P2Hex->setVisible(false);
//        ui->lineEdit_Serial_P2->setVisible(false);
//        ui->lineEdit_Serial_P1Hex->setVisible(false);
//        ui->lineEdit_Serial_P1->setVisible(false);
//        ui->lineEdit_serial->setVisible(false);
//        ui->lineEdit_Scale_T2Hex->setVisible(false);
//        ui->lineEdit_Scale_T2->setVisible(false);
//        ui->lineEdit_Scale_T1Hex->setVisible(false);
//        ui->lineEdit_Scale_T1->setVisible(false);
//        ui->lineEdit_R_RTDHex->setVisible(false);
//        ui->lineEdit_R_RTD->setVisible(false);
//        ui->lineEdit_RWire2Hex->setVisible(false);
//        ui->lineEdit_RWire2->setVisible(false);
//        ui->lineEdit_RWire1Hex->setVisible(false);
//        ui->lineEdit_RWire1->setVisible(false);
//        ui->lineEdit_RTD_TypeHex->setVisible(false);
//        ui->lineEdit_ReportDataHex->setVisible(false);
//        ui->lineEdit_PulsesValue2Hex->setVisible(false);
//        ui->lineEdit_PulsesValue2->setVisible(false);
//        ui->lineEdit_PulsesValue1Hex->setVisible(false);
//        ui->lineEdit_PulsesValue1->setVisible(false);
//        ui->lineEdit_PulsesMode2Hex->setVisible(false);
//        ui->lineEdit_PulsesMode1Hex->setVisible(false);
//        ui->lineEdit_ProtTypeHex->setVisible(false);
//        ui->lineEdit_ProtType->setVisible(false);
//        ui->lineEdit_PressureHex->setVisible(false);
//        ui->lineEdit_Pressure->setVisible(false);
//        ui->lineEdit_PCB_SNHex->setVisible(false);
//        ui->lineEdit_PCB_SN->setVisible(false);
//        ui->lineEdit_P2PrimAddrHex->setVisible(false);
//        ui->lineEdit_P1PrimAddrHex->setVisible(false);
//        ui->lineEdit_DNHex->setVisible(false);
//        ui->lineEdit_EPP_Trans->setVisible(false);
//        ui->lineEdit_EPP_TransHex->setVisible(false);
//        ui->lineEdit_FlowNomHex->setVisible(false);
//        ui->lineEdit_FlowRateHex->setVisible(false);
//        ui->lineEdit_Flow_max->setVisible(false);
//        ui->lineEdit_Flow_maxHex->setVisible(false);
//        ui->lineEdit_Flow_min->setVisible(false);
//        ui->lineEdit_Flow_minHex->setVisible(false);
//        ui->lineEdit_FS_DiffThrs->setVisible(false);
//        ui->lineEdit_FS_DiffThrsHex->setVisible(false);
//        ui->lineEdit_HMP_Trans->setVisible(false);
//        ui->lineEdit_HMP_TransHex->setVisible(false);
//        ui->lineEdit_Initial_value_P1->setVisible(false);
//        ui->lineEdit_Initial_value_P1Hex->setVisible(false);
//        ui->lineEdit_Initial_value_P2->setVisible(false);
//        ui->lineEdit_Initial_value_P2Hex->setVisible(false);
//        ui->lineEdit_ManCodeHex->setVisible(false);
//        ui->lineEdit_ManCode_P1->setVisible(false);
//        ui->lineEdit_ManCode_P1Hex->setVisible(false);
//        ui->lineEdit_ManCode_P2->setVisible(false);
//        ui->lineEdit_ManCode_P2Hex->setVisible(false);

    ui->lineEdit_AddrHex->setVisible(false);
    ui->lineEdit_ASW_flagHex->setVisible(false);
    ui->lineEdit_Average_THex->setVisible(false);
    ui->lineEdit_BaudRateHex->setVisible(false);
    ui->lineEdit_coef0Hex->setVisible(false);
    ui->lineEdit_coef1Hex->setVisible(false);
    ui->lineEdit_coef2Hex->setVisible(false);
    ui->lineEdit_coef3Hex->setVisible(false);
    ui->lineEdit_coef4Hex->setVisible(false);
    ui->lineEdit_coef5Hex->setVisible(false);
    ui->lineEdit_DevTypeHex->setVisible(false);
    ui->lineEdit_DevType_P1Hex->setVisible(false);
    ui->lineEdit_DevType_P2Hex->setVisible(false);
    ui->lineEdit_DNHex->setVisible(false);
    ui->lineEdit_EPP_TransHex->setVisible(false);
    ui->lineEdit_FlowNomHex->setVisible(false);
    ui->lineEdit_FlowRateHex->setVisible(false);
    ui->lineEdit_Flow_maxHex->setVisible(false);
    ui->lineEdit_Flow_minHex->setVisible(false);
    ui->lineEdit_FS_DiffThrsHex->setVisible(false);
    ui->lineEdit_HMP_TransHex->setVisible(false);
    ui->lineEdit_Initial_value_P1Hex->setVisible(false);
    ui->lineEdit_Initial_value_P2Hex->setVisible(false);
    ui->lineEdit_ManCodeHex->setVisible(false);
    ui->lineEdit_ManCode_P1Hex->setVisible(false);
    ui->lineEdit_ManCode_P2Hex->setVisible(false);
    ui->lineEdit_P1PrimAddrHex->setVisible(false);
    ui->lineEdit_P2PrimAddrHex->setVisible(false);
    ui->lineEdit_PCB_SNHex->setVisible(false);
    ui->lineEdit_PressureHex->setVisible(false);
    ui->lineEdit_ProtTypeHex->setVisible(false);
    ui->lineEdit_PulsesMode1Hex->setVisible(false);
    ui->lineEdit_PulsesMode2Hex->setVisible(false);
    ui->lineEdit_PulsesValue1Hex->setVisible(false);
    ui->lineEdit_PulsesValue2Hex->setVisible(false);
    ui->lineEdit_ReportDataHex->setVisible(false);
    ui->lineEdit_RTD_TypeHex->setVisible(false);
    ui->lineEdit_RWire1Hex->setVisible(false);
    ui->lineEdit_RWire2Hex->setVisible(false);
    ui->lineEdit_R_RTDHex->setVisible(false);
    ui->lineEdit_Scale_T1Hex->setVisible(false);
    ui->lineEdit_Scale_T2Hex->setVisible(false);
    ui->lineEdit_serialHex->setVisible(false);
    ui->lineEdit_Serial_P1Hex->setVisible(false);
    ui->lineEdit_Serial_P2Hex->setVisible(false);
    ui->lineEdit_Service_P1Hex->setVisible(false);
    ui->lineEdit_Service_P2Hex->setVisible(false);
    ui->lineEdit_Shift_T1Hex->setVisible(false);
    ui->lineEdit_Shift_T2Hex->setVisible(false);
    ui->lineEdit_Tin_maxHex->setVisible(false);
    ui->lineEdit_Tin_minHex->setVisible(false);
    ui->lineEdit_Tout_maxHex->setVisible(false);
    ui->lineEdit_Tout_minHex->setVisible(false);
    ui->lineEdit_Version_P1Hex->setVisible(false);
    ui->lineEdit_Version_P2Hex->setVisible(false);
    ui->lineEdit_serialHex2->setVisible(false);
    ui->lineEdit_serialHex3->setVisible(false);
    ui->lineEdit_serialHex4->setVisible(false);
    ui->lineEdit_PCB_SNHex2->setVisible(false);
    ui->lineEdit_PCB_SNHex3->setVisible(false);
    ui->lineEdit_PCB_SNHex4->setVisible(false);

}

FormParamsEdit::~FormParamsEdit()
{
    delete ui;
}
/***********************************************************/
void FormParamsEdit::getbufferReadHardwareConfiguration(QByteArray buffer, int workPlaceNumber)
{
    //serial
    QByteArray serialByteAray;
    serialByteAray.append(buffer[12]);
    serialByteAray.append(buffer[13]);
    serialByteAray.append(buffer[14]);
    serialByteAray.append(buffer[15]);

    switch (workPlaceNumber) {
    case 0:
       ui->lineEdit_serial->setText(serialByteAray.toHex());
       qDebug()<<"serialByteAray.toHex() "<<serialByteAray.toHex();
        break;
    case 1:
       ui->lineEdit_serial2->setText(serialByteAray.toHex());
       qDebug()<<"serialByteAray2.toHex() "<<serialByteAray.toHex();
        break;
    case 2:
       ui->lineEdit_serial3->setText(serialByteAray.toHex());
       qDebug()<<"serialByteAray3.toHex() "<<serialByteAray.toHex();
        break;
    case 3:
       ui->lineEdit_serial4->setText(serialByteAray.toHex());
       qDebug()<<"serialByteAray4.toHex() "<<serialByteAray.toHex();
        break;
    default:
        break;
    }

//    ui->lineEdit_serial->setText(serialByteAray.toHex());
//    qDebug()<<"serialByteAray.toHex() "<<serialByteAray.toHex();

    //
    //ManCode
    QByteArray manCodeByteArray;
    manCodeByteArray.append(buffer[16]);
    manCodeByteArray.append(buffer[17]);
    int manufacturerSum = static_cast<quint8>(manCodeByteArray[0]);//manufacturer.toInt();
    manufacturerSum = (manufacturerSum<<8)|(static_cast<quint8>(manCodeByteArray[1]));
    quint8 symbol1, symbol2, symbol3;
    symbol1 = ((manufacturerSum/(32*32))&0x1F) + 64;
    symbol2 = ((manufacturerSum/32)&0x1F) + 64;
    symbol3 = (manufacturerSum&0x1F) + 64;
    QByteArray manufacturerString;
    manufacturerString[0] = symbol1;
    manufacturerString[1] = symbol2;
    manufacturerString[2] = symbol3;
    if(manCodeList.contains(QString::fromLatin1(manufacturerString))) {
        ui->comboBox_ManCode->setCurrentText(QString::fromLatin1(manufacturerString));
//        ui->comboBox_ManCode->setStyleSheet("QLabel { background-color : white; color : red; }");
    }
    else {
       QMessageBox::information(this, "", tr("Недопустимое значение: Производитель"));
    }
    qDebug()<<"QString::fromLatin1(manufacturerString)"<<QString::fromLatin1(manufacturerString);
    ui->lineEdit_ManCodeHex->setText(QString::fromLatin1(manufacturerString));

    //
    //DevType
    QByteArray devType;
    devType.append(buffer[18]);

    if(devType[0] == 0x04) {
        ui->comboBox_DevType->setCurrentText("Обратный");
    }

    if(devType[0] == 0x0c) {
        ui->comboBox_DevType->setCurrentText("Подающий");
    }

    if((devType[0] != 0x04) && (devType[0] != 0x0c) ) {
        QMessageBox::information(this, "", tr("Недопустимое значение: Тип счётчика"));
    }

//    if(devTypeList.contains(devType.toHex())) {
//        ui->comboBox_DevType->setCurrentText(devType.toHex());
//    }
//    else {
//       QMessageBox::information(this, "", tr("Недопустимое значение: Тип счётчика"));
//    }

    qDebug()<<"devType.toHex() "<<devType.toHex();
    ui->lineEdit_DevTypeHex->setText(devType.toHex());

    //
    //DN
    QByteArray DN;
    DN.append(buffer[19]);
    if(DNList.contains(QString::number((quint8)buffer[19], 16))) {
        ui->comboBox_DN->setCurrentText(QString::number((quint8)buffer[19], 16));
    }
    else {
       QMessageBox::information(this, "", tr("Недопустимое значение: Диаметр условного прохода"));
    }
    qDebug()<<"DN.toHex() "<<DN.toHex();
    ui->lineEdit_DNHex->setText(DN.toHex());

    //
    //PCB_SN 8bytes (29-36)
    QByteArray PCB_SN_Array;
    PCB_SN_Array.append(buffer[29]);
    PCB_SN_Array.append(buffer[30]);
    PCB_SN_Array.append(buffer[31]);
    PCB_SN_Array.append(buffer[32]);
    PCB_SN_Array.append(buffer[33]);
    PCB_SN_Array.append(buffer[34]);
    PCB_SN_Array.append(buffer[35]);
    PCB_SN_Array.append(buffer[36]);

    switch (workPlaceNumber) {
    case 0:
       PCB_SN_ByteArray.clear();
       PCB_SN_ByteArray = PCB_SN_Array;

       ui->lineEdit_PCB_SN->setText(PCB_SN_Array.toHex());
       qDebug()<<"PCB_SN_ByteArray.toHex() "<<PCB_SN_ByteArray.toHex();
        break;
    case 1:
       PCB_SN_ByteArray2.clear();
       PCB_SN_ByteArray2 = PCB_SN_Array;

       ui->lineEdit_PCB_SN2->setText(PCB_SN_Array.toHex());
       qDebug()<<"PCB_SN_ByteArray2.toHex() "<<PCB_SN_ByteArray2.toHex();
        break;
    case 2:
       PCB_SN_ByteArray3.clear();
       PCB_SN_ByteArray3 = PCB_SN_Array;

       ui->lineEdit_PCB_SN3->setText(PCB_SN_Array.toHex());
       qDebug()<<"PCB_SN_ByteArray3.toHex() "<<PCB_SN_ByteArray3.toHex();
        break;
    case 3:
       PCB_SN_ByteArray4.clear();
       PCB_SN_ByteArray4 = PCB_SN_Array;

       ui->lineEdit_PCB_SN4->setText(PCB_SN_Array.toHex());
       qDebug()<<"PCB_SN_ByteArray4.toHex() "<<PCB_SN_ByteArray4.toHex();
        break;
    default:
        break;
    }

//    PCB_SN_ByteArray.clear();
//    PCB_SN_ByteArray = PCB_SN_Array;

//    ui->lineEdit_PCB_SN->setText(PCB_SN_Array.toHex());
//    qDebug()<<"PCB_SN_Array.toHex() "<<PCB_SN_Array.toHex();
//    qDebug()<<"PCB_SN_ByteArray.toHex() "<<PCB_SN_ByteArray.toHex();
}
/***********************************************************/
void FormParamsEdit::getbufferReadSoftWareConfiguration(QByteArray buffer)
{
    //flowMin
    QByteArray flowMinArray;
    flowMinArray.append(buffer[4]);
    flowMinArray.append(buffer[5]);
//             ui->lineEdit_Flow_min->setText(flowMinArray.toHex());

//             float flowMinFloat = buffer[4];
    quint16 flowMinQuint16 = buffer[4];
    flowMinQuint16 = flowMinQuint16<<8 | buffer[5];
    float flowMinFloat = (float)flowMinQuint16/1000;
    if(flowMinFloat>=0 && flowMinFloat<=0.2) {
        ui->lineEdit_Flow_min->setText(QString::number(flowMinFloat));
    }
    else {
 //       QMessageBox::information(this, "", tr("Недопустимое значение: Минимальный расход"));
        ui->lineEdit_Flow_min->setText(QString::number(flowMinFloat));
    }

    qDebug()<<"flowMinArray.toHex() "<<flowMinArray.toHex();
    ui->lineEdit_Flow_minHex->setText(flowMinArray.toHex());

    //
    //flowMax
    QByteArray flowMaxArray;
    flowMaxArray.append(buffer[6]);
    flowMaxArray.append(buffer[7]);

//             float flowMaxFloat = buffer[6];
    quint16 flowMaxQuint16 = buffer[6];
    flowMaxQuint16 = (flowMaxQuint16<<8) | (quint8)(buffer.at(7));
    float flowMaxFloat = flowMaxQuint16;
    flowMaxFloat = flowMaxFloat/1000;
    if(flowMaxFloat>=2 && flowMaxFloat<=5) {
        ui->lineEdit_Flow_max->setText(QString::number(flowMaxFloat));
    }
    else {
//        QMessageBox::information(this, "", tr("Недопустимое значение: Максимальный расход"));
        ui->lineEdit_Flow_max->setText(QString::number(flowMaxFloat));
    }

    qDebug()<<"flowMaxArray.toHex() "<<flowMaxArray.toHex();
    ui->lineEdit_Flow_maxHex->setText(flowMaxArray.toHex());

    //
    //tinMin
    QByteArray tinMinArray;
    tinMinArray.append(buffer[8]);


    quint8 tinMinQuint8 = buffer[8];
    if(tinMinQuint8>=10 && tinMinQuint8<=60) {
        ui->spinBox_Tin_min->setValue(tinMinQuint8);
    }
    else {
//        QMessageBox::information(this, "", tr("Недопустимое значение: Минимальная температура подачи"));
        ui->spinBox_Tin_min->setValue(tinMinQuint8);
    }

    qDebug()<<"tinMinArray.toHex() "<<tinMinArray.toHex();
    ui->lineEdit_Tin_minHex->setText(tinMinArray.toHex());

    //
    //tinMax
    QByteArray tinMaxArray;
    tinMaxArray.append(buffer[9]);


    quint8 tinMaxQuint8 = buffer[9];
    if(tinMaxQuint8>=50 && tinMaxQuint8<=150) {
        ui->spinBox_Tin_max->setValue(tinMaxQuint8);
    }
    else {
 //       QMessageBox::information(this, "", tr("Недопустимое значение: Максимальная температура подачи"));
        ui->spinBox_Tin_max->setValue(tinMaxQuint8);
    }

    qDebug()<<"tinMaxArray.toHex() "<<tinMaxArray.toHex();
    ui->lineEdit_Tin_maxHex->setText(tinMaxArray.toHex());

    //
    //toutMin
    QByteArray toutMinArray;
    toutMinArray.append(buffer[10]);

    quint8 toutMinQuint8 = buffer[10];
    if(toutMinQuint8>=10 && toutMinQuint8<=60) {
        ui->spinBox_Tout_min->setValue(toutMinQuint8);
    }
    else {
 //       QMessageBox::information(this, "", tr("Недопустимое значение: Минимальная температура обратки"));
        ui->spinBox_Tout_min->setValue(toutMinQuint8);
    }

    qDebug()<<"toutMinArray.toHex() "<<toutMinArray.toHex();
    ui->lineEdit_Tout_minHex->setText(toutMinArray.toHex());


    //
    //toutMax
    QByteArray toutMaxArray;
    toutMaxArray.append(buffer[11]);

    quint8 toutMaxQuint8 = buffer[11];
    if(toutMaxQuint8>=50 && toutMaxQuint8<=150) {
        ui->spinBox_Tout_max->setValue(toutMaxQuint8);
    }
    else {
 //       QMessageBox::information(this, "", tr("Недопустимое значение: Максимальная температура обратки"));
        ui->spinBox_Tout_max->setValue(toutMaxQuint8);
    }

    qDebug()<<"toutMaxArray.toHex() "<<toutMaxArray.toHex();
    ui->lineEdit_Tout_maxHex->setText(toutMaxArray.toHex());

    //
    //reportData
    QByteArray reportDataArray;
    reportDataArray.append(buffer[12]);

    quint8 reportDataInt = buffer[12];
    if(reportDataInt>=0 && reportDataInt<=31) {
        ui->spinBox_ReportData->setValue(reportDataInt);
    }
    else {
//        QMessageBox::information(this, "", tr("Недопустимое значение: Дата сохранения в журнал"));
        ui->spinBox_ReportData->setValue(reportDataInt);
    }

    qDebug()<<"reportDataArray.toHex() "<<reportDataArray.toHex();
    ui->lineEdit_ReportDataHex->setText(reportDataArray.toHex());

    //
    //AverageT
    QByteArray AverageTArray;
    AverageTArray.append(buffer[13]);


    quint8 AverageTInt = buffer[13];
    if(AverageTList.contains(QString::number(AverageTInt))) {
        ui->comboBox_Average_T->setCurrentText(QString::number(AverageTInt));
    }
    else {
        QMessageBox::information(this, "", tr("Недопустимое значение: Тип усреднения температуры"));

    }

    qDebug()<<"AverageTArray.toHex() "<<AverageTArray.toHex();
    ui->lineEdit_Average_THex->setText(AverageTArray.toHex());

    //
    //pulsesMode1
    QByteArray pulsesMode1Array;
    pulsesMode1Array.append(buffer[14]);


    quint8 pulsesMode1Int = (quint8)buffer[14];
    if(pulsesMode1List.contains(QString::number(pulsesMode1Int))) {
        ui->comboBox_PulsesMode1->setCurrentText(QString::number(pulsesMode1Int));
    }
    else {
        QMessageBox::information(this, "", tr("Недопустимое значение: Режим работы импульсного вывода"));
    }

    qDebug()<<"pulsesMode1Array.toHex() "<<pulsesMode1Array.toHex();
    ui->lineEdit_PulsesMode1Hex->setText(pulsesMode1Array.toHex());

    //
    //pulsesMode2
    QByteArray pulsesMode2Array;
    pulsesMode2Array.append(buffer[15]);


    quint8 pulsesMode2Int = (quint8)buffer[15];
    if(pulsesMode1List.contains(QString::number(pulsesMode2Int))) {
        ui->comboBox_PulsesMode2->setCurrentText(QString::number(pulsesMode2Int));
    }
    else {
        QMessageBox::information(this, "", tr("Недопустимое значение: Режим работы импульсного вывода"));
    }

    qDebug()<<"pulsesMode2Array.toHex() "<<pulsesMode2Array.toHex();
    ui->lineEdit_PulsesMode2Hex->setText(pulsesMode2Array.toHex());

    //
    //pulsesValue1
    QByteArray pulsesValue1Array;
    pulsesValue1Array.append(buffer[16]);
    pulsesValue1Array.append(buffer[17]);


    quint16 pulsesValue1Int = buffer[16];
    pulsesValue1Int = pulsesValue1Int<<8 | (quint8)buffer[17];
    ui->lineEdit_PulsesValue1->setText(QString::number(pulsesValue1Int));

    qDebug()<<"pulsesValue1Array.toHex() "<<pulsesValue1Array.toHex();
    ui->lineEdit_PulsesValue1Hex->setText(pulsesValue1Array.toHex());

    //
    //pulsesValue2
    QByteArray pulsesValue2Array;
    pulsesValue2Array.append(buffer[18]);
    pulsesValue2Array.append(buffer[19]);
    qDebug()<<"pulsesValue2Array.toHex() "<<pulsesValue2Array.toHex();

    quint16 pulsesValue2Int = buffer[18];
    pulsesValue2Int = pulsesValue2Int<<8 | (quint8)buffer[19];
    ui->lineEdit_PulsesValue2->setText(QString::number(pulsesValue2Int));

    qDebug()<<"pulsesValue2Array.toHex() "<<pulsesValue2Array.toHex();
    ui->lineEdit_PulsesValue2Hex->setText(pulsesValue2Array.toHex());

 //   paramsVector.append(pulsesValue2Array);
}
/***********************************************************/
void FormParamsEdit::getbufferReadSensorParameters(QByteArray buffer)
{
    //flowRate 4 bytes (4-7)
    QByteArray flowRateArray;
    flowRateArray.append(buffer[4]);
    flowRateArray.append(buffer[5]);
    flowRateArray.append(buffer[6]);
    flowRateArray.append(buffer[7]);


    quint32 flowRateQuint32 = buffer[4];
    flowRateQuint32 = flowRateQuint32<<8 | (quint8)buffer[5];
    flowRateQuint32 = flowRateQuint32<<8 | (quint8)buffer[6];
    flowRateQuint32 = flowRateQuint32<<8 | (quint8)buffer[7];
    float flowRateFloat = (float)flowRateQuint32/1000;

    QString flowRateString = QString::number(flowRateFloat);
    if((flowRateFloat - round(flowRateFloat)) == 0 ) { //проверка на отсутствие дробной части
        flowRateString = flowRateString + ".00";
    }

    qDebug()<<"flowRateString "<<flowRateString;

    if(flowRateList.contains(flowRateString)) {
        ui->comboBox_FlowRate->setCurrentText(flowRateString);
    }
    else {
        QMessageBox::information(this, "", tr("Недопустимое значение: Вес импульса вертушки"));
  //        ui->comboBox_FlowRate->setCurrentText(QString::number(flowRateFloat));
    }

    qDebug()<<"flowRateArray.toHex() "<<flowRateArray.toHex();
    ui->lineEdit_FlowRateHex->setText(flowRateArray.toHex());

    //
    //pressure 1 byte (8)
    QByteArray pressureArray;
    pressureArray.append(buffer[8]);

    quint8 pressureQuint8 = buffer[8];
    if(pressureQuint8 == 6) {
        ui->lineEdit_Pressure->setText(QString::number(pressureQuint8));
    }
    else {
//        QMessageBox::information(this, "", tr("Недопустимое значение: Условное давление"));
        ui->lineEdit_Pressure->setText(QString::number(pressureQuint8));
    }

    qDebug()<<"pressureArray.toHex() "<<pressureArray.toHex();
    ui->lineEdit_PressureHex->setText(pressureArray.toHex());

    //
    //RTD_Type 1 byte (9)
    QByteArray RTD_TypeArray;
    RTD_TypeArray.append(buffer[9]);

    quint8 RTD_TypeQuint8 = buffer[9];
    if((RTD_TypeQuint8 >= 0) && (RTD_TypeQuint8 <= (RTD_TypeList.count() - 1))) {
        ui->comboBox_RTD_Type->setCurrentIndex(RTD_TypeQuint8);
    }
    else {
        QMessageBox::information(this, "", tr("Недопустимое значение: Тип термометров сопротивления"));
//        ui->comboBox_RTD_Type->setCurrentText(QString::number(RTD_TypeQuint8));
    }

    qDebug()<<"RTD_TypeArray.toHex() "<<RTD_TypeArray.toHex();
    ui->lineEdit_RTD_TypeHex->setText(RTD_TypeArray.toHex());

    //
    //R_RTD 4 bytes (10-13)
    QByteArray R_RTDArray;
    R_RTDArray.append(buffer[10]);
    R_RTDArray.append(buffer[11]);
    R_RTDArray.append(buffer[12]);
    R_RTDArray.append(buffer[13]);


    quint32 R_RTDQuint32 = (quint8)buffer[10];
    R_RTDQuint32 = R_RTDQuint32<<8 | (quint8)buffer[11];
    R_RTDQuint32 = R_RTDQuint32<<8 | (quint8)buffer[12];
    R_RTDQuint32 = R_RTDQuint32<<8 | (quint8)buffer[13];

    if(R_RTDQuint32 == 1000) {
        ui->lineEdit_R_RTD->setText(QString::number(R_RTDQuint32));
    }
    else {
 //       QMessageBox::information(this, "", tr("Недопустимое значение: Номинальное сопротивление термометров") + QString::number(R_RTDQuint32));
    }

    qDebug()<<"R_RTDArray.toHex() "<<R_RTDArray.toHex();
    ui->lineEdit_R_RTDHex->setText(R_RTDArray.toHex());

    //
    //RWire1 4 bytes (14-17)
    QByteArray RWire1Array;
    RWire1Array.append(buffer[14]);
    RWire1Array.append(buffer[15]);
    RWire1Array.append(buffer[16]);
    RWire1Array.append(buffer[17]);


    quint32 RWire1Quint8 = buffer[14];
    RWire1Quint8 = RWire1Quint8<<8 | (quint8)buffer[15];
    RWire1Quint8 = RWire1Quint8<<8 | (quint8)buffer[16];
    RWire1Quint8 = RWire1Quint8<<8 | (quint8)buffer[17];

    if(RWire1Quint8 == 0) {
        ui->lineEdit_RWire1->setText(QString::number(RWire1Quint8));
    }
    else {
 //       QMessageBox::information(this, "", tr("Недопустимое значение: Сопротивление проводов термомнтров сопротивления"));
    }

    qDebug()<<"RWire1Array.toHex() "<<RWire1Array.toHex();
    ui->lineEdit_RWire1Hex->setText(RWire1Array.toHex());

    //
    //RWire2 4 bytes (18-21)
    QByteArray RWire2Array;
    RWire2Array.append(buffer[18]);
    RWire2Array.append(buffer[19]);
    RWire2Array.append(buffer[20]);
    RWire2Array.append(buffer[21]);


    quint32 RWire2Quint8 = buffer[18];
    RWire2Quint8 = RWire2Quint8<<8 | (quint8)buffer[19];
    RWire2Quint8 = RWire2Quint8<<8 | (quint8)buffer[20];
    RWire2Quint8 = RWire2Quint8<<8 | (quint8)buffer[21];

    if(RWire2Quint8 == 0) {
        ui->lineEdit_RWire2->setText(QString::number(RWire2Quint8));
    }
    else {
 //       QMessageBox::information(this, "", tr("Недопустимое значение: Сопротивление проводов термомнтров сопротивления"));
    }

    qDebug()<<"RWire2Array.toHex() "<<RWire2Array.toHex();
    ui->lineEdit_RWire2Hex->setText(RWire2Array.toHex());

//    paramsVector.append(RWire2Array);

    //
    //flowNom 1 byte (22)
    QByteArray flowNomArray;
    flowNomArray.append(buffer[22]);


    quint8 flowNomInt = buffer[22];
    float flowNomFloat = (float)flowNomInt/10;
    if(flowNomList.contains(QString::number(flowNomFloat))) {
        ui->comboBox_FlowNom->setCurrentText(QString::number(flowNomFloat));
    }
    else {
 //       QMessageBox::information(this, "", tr("Недопустимое значение: Номинальный расход"));
    }

    qDebug()<<"flowNomArray.toHex() "<<flowNomArray.toHex();
    ui->lineEdit_FlowNomHex->setText(flowNomArray.toHex());

//    paramsVector.append(flowNomArray);
}
/***********************************************************/
void FormParamsEdit::getbufferReadFlowCoefficient(QByteArray buffer)
{
    //flowCoef0 4 bytes (4-7)
    QByteArray flowCoef0Array;
    flowCoef0Array.append(buffer[4]);
    flowCoef0Array.append(buffer[5]);
    flowCoef0Array.append(buffer[6]);
    flowCoef0Array.append(buffer[7]);


    quint32 flowCoef0Quint32 = (quint8)buffer[4];
    flowCoef0Quint32 = flowCoef0Quint32<<8 | (quint8)buffer[5];
    flowCoef0Quint32 = flowCoef0Quint32<<8 | (quint8)buffer[6];
    flowCoef0Quint32 = flowCoef0Quint32<<8 | (quint8)buffer[7];
    float flowCoef0Float = (float)flowCoef0Quint32/1000000;
    if(flowCoef0Float == 1) {
        ui->lineEdit_coef0->setText(QString::number(flowCoef0Float, 'g', 6) + ".000000");
    }
    else {
 //       QMessageBox::information(this, "", tr("Недопустимое значение: Коэффициент коррекции расхода 0"));
        ui->lineEdit_coef0->setText(QString::number(flowCoef0Float));
    }

    qDebug()<<"flowCoef0Array.toHex() "<<flowCoef0Array.toHex();
    ui->lineEdit_coef0Hex->setText(flowCoef0Array.toHex());

    //
    //flowCoef1 4 bytes (8-11)
    QByteArray flowCoef1Array;
    flowCoef1Array.append(buffer[8]);
    flowCoef1Array.append(buffer[9]);
    flowCoef1Array.append(buffer[10]);
    flowCoef1Array.append(buffer[11]);


    quint32 flowCoef1Quint32 = (quint8)buffer[8];
    flowCoef1Quint32 = flowCoef1Quint32<<8 | (quint8)buffer[9];
    flowCoef1Quint32 = flowCoef1Quint32<<8 | (quint8)buffer[10];
    flowCoef1Quint32 = flowCoef1Quint32<<8 | (quint8)buffer[11];
    float flowCoef1Float = (float)flowCoef1Quint32/1000000;
    if(flowCoef1Float == 1) {
        ui->lineEdit_coef1->setText(QString::number(flowCoef1Float) + ".000000");
    }
    else {
 //       QMessageBox::information(this, "", tr("Недопустимое значение: Коэффициент коррекции расхода 1"));
        ui->lineEdit_coef1->setText(QString::number(flowCoef1Float));
    }

    qDebug()<<"flowCoef1Array.toHex() "<<flowCoef1Array.toHex();
    ui->lineEdit_coef1Hex->setText(flowCoef1Array.toHex());

    //
    //flowCoef2 4 bytes (12-15)
    QByteArray flowCoef2Array;
    flowCoef2Array.append(buffer[12]);
    flowCoef2Array.append(buffer[13]);
    flowCoef2Array.append(buffer[14]);
    flowCoef2Array.append(buffer[15]);


    quint32 flowCoef2Quint32 = (quint8)buffer[12];
    flowCoef2Quint32 = flowCoef2Quint32<<8 | (quint8)buffer[13];
    flowCoef2Quint32 = flowCoef2Quint32<<8 | (quint8)buffer[14];
    flowCoef2Quint32 = flowCoef2Quint32<<8 | (quint8)buffer[15];
    float flowCoef2Float = (float)flowCoef2Quint32/1000000;
    if(flowCoef2Float == 1) {
        ui->lineEdit_coef2->setText(QString::number(flowCoef2Float) + ".000000");
    }
    else {
 //       QMessageBox::information(this, "", tr("Недопустимое значение: Коэффициент коррекции расхода 2"));
        ui->lineEdit_coef2->setText(QString::number(flowCoef2Float));
    }

    qDebug()<<"flowCoef2Array.toHex() "<<flowCoef2Array.toHex();
    ui->lineEdit_coef2Hex->setText(flowCoef2Array.toHex());

    //
    //flowCoef3 4 bytes (16-19)
    QByteArray flowCoef3Array;
    flowCoef3Array.append(buffer[16]);
    flowCoef3Array.append(buffer[17]);
    flowCoef3Array.append(buffer[18]);
    flowCoef3Array.append(buffer[19]);


    quint32 flowCoef3Quint32 = (quint8)buffer[16];
    flowCoef3Quint32 = flowCoef3Quint32<<8 | (quint8)buffer[17];
    flowCoef3Quint32 = flowCoef3Quint32<<8 | (quint8)buffer[18];
    flowCoef3Quint32 = flowCoef3Quint32<<8 | (quint8)buffer[19];
    float flowCoef3Float = (float)flowCoef3Quint32/1000000;
    if(flowCoef3Float == 1) {
        ui->lineEdit_coef3->setText(QString::number(flowCoef3Float) + ".000000");
    }
    else {
  //      QMessageBox::information(this, "", tr("Недопустимое значение: Коэффициент коррекции расхода 3"));
        ui->lineEdit_coef3->setText(QString::number(flowCoef3Float));
    }

    qDebug()<<"flowCoef3Array.toHex() "<<flowCoef3Array.toHex();
    ui->lineEdit_coef3Hex->setText(flowCoef3Array.toHex());

    //
    //flowCoef4 4 bytes (20-23)
    QByteArray flowCoef4Array;
    flowCoef4Array.append(buffer[20]);
    flowCoef4Array.append(buffer[21]);
    flowCoef4Array.append(buffer[22]);
    flowCoef4Array.append(buffer[23]);


    quint32 flowCoef4Quint32 = (quint8)buffer[20];
    flowCoef4Quint32 = flowCoef4Quint32<<8 | (quint8)buffer[21];
    flowCoef4Quint32 = flowCoef4Quint32<<8 | (quint8)buffer[22];
    flowCoef4Quint32 = flowCoef4Quint32<<8 | (quint8)buffer[23];
    float flowCoef4Float = (float)flowCoef4Quint32/1000000;
    if(flowCoef4Float == 1) {
        ui->lineEdit_coef4->setText(QString::number(flowCoef4Float) + ".000000");
    }
    else {
 //       QMessageBox::information(this, "", tr("Недопустимое значение: Коэффициент коррекции расхода 4"));
        ui->lineEdit_coef4->setText(QString::number(flowCoef4Float));
    }

    qDebug()<<"flowCoef4Array.toHex() "<<flowCoef4Array.toHex();
    ui->lineEdit_coef4Hex->setText(flowCoef4Array.toHex());

    //
    //flowCoef5 4 bytes (24-27)
    QByteArray flowCoef5Array;
    flowCoef5Array.append(buffer[24]);
    flowCoef5Array.append(buffer[25]);
    flowCoef5Array.append(buffer[26]);
    flowCoef5Array.append(buffer[27]);


    quint32 flowCoef5Quint32 = (quint8)buffer[24];
    flowCoef5Quint32 = flowCoef5Quint32<<8 | (quint8)buffer[25];
    flowCoef5Quint32 = flowCoef5Quint32<<8 | (quint8)buffer[26];
    flowCoef5Quint32 = flowCoef5Quint32<<8 | (quint8)buffer[27];
    float flowCoef5Float = (float)flowCoef5Quint32/1000000;
    if(flowCoef5Float == 1) {
        ui->lineEdit_coef5->setText(QString::number(flowCoef5Float) + ".000000");
    }
    else {
 //       QMessageBox::information(this, "", tr("Недопустимое значение: Коэффициент коррекции расхода 5"));
        ui->lineEdit_coef5->setText(QString::number(flowCoef5Float));
    }

    qDebug()<<"flowCoef5Array.toHex() "<<flowCoef5Array.toHex();
    ui->lineEdit_coef5Hex->setText(flowCoef5Array.toHex());
}
/***********************************************************/
void FormParamsEdit::getbufferReadCalibrationFactor(QByteArray buffer)
{
    //FS_DiffThrs
    QByteArray FS_DiffThrsArray;
    FS_DiffThrsArray.append(buffer[8]);
    FS_DiffThrsArray.append(buffer[9]);


    quint16 FS_DiffThrsQuint16 = buffer[8];
    FS_DiffThrsQuint16 = FS_DiffThrsQuint16<<8 | buffer[9];

    if(FS_DiffThrsQuint16 == 30) {
        ui->lineEdit_FS_DiffThrs->setText(QString::number(FS_DiffThrsQuint16));
    }
    else {
 //       QMessageBox::information(this, "", tr("Недопустимое значение: Порог срабатывания датчика расхода"));
        ui->lineEdit_FS_DiffThrs->setText(QString::number(FS_DiffThrsQuint16));
    }

    qDebug()<<"FS_DiffThrsArray.toHex() "<<FS_DiffThrsArray.toHex();
    ui->lineEdit_FS_DiffThrsHex->setText(FS_DiffThrsArray.toHex());
}
/***********************************************************/
void FormParamsEdit::getbufferReadRealClock(QByteArray buffer)
{
    //Hour
    QDateTime dateTime_RealClock;
    QDate date_RealClock;
    QTime time_RealClock;

    QByteArray HourArray;
    HourArray.append(buffer[4]);


    quint8 HourQuint8 = buffer[4];

    if(HourQuint8>=0 && HourQuint8<=23) {
//                 time_RealClock.
    }
    else {
        QMessageBox::information(this, "", tr("Недопустимое значение: Часы"));
    }

    qDebug()<<"HourArray.toHex() "<<HourArray.toHex();

    //
    //Minute
    QByteArray MinuteArray;
    MinuteArray.append(buffer[5]);


    quint8 MinuteQuint8 = buffer[5];

    if(MinuteQuint8>=0 && MinuteQuint8<=59) {
//                 time_RealClock.
    }
    else {
        QMessageBox::information(this, "", tr("Недопустимое значение: Минуты"));
//                 ui->lineEdit_FS_DiffThrs->setText(QString::number(FS_DiffThrsQuint16));
    }

    qDebug()<<"MinuteArray.toHex() "<<MinuteArray.toHex();

    //
    //Second
    QByteArray SecondArray;
    SecondArray.append(buffer[6]);

    quint8 SecondQuint8 = buffer[6];

    if(SecondQuint8>=0 && SecondQuint8<=59) {
//                 time_RealClock.
    }
    else {
        QMessageBox::information(this, "", tr("Недопустимое значение: Секунды"));
//                 ui->lineEdit_FS_DiffThrs->setText(QString::number(FS_DiffThrsQuint16));
    }

    qDebug()<<"SecondArray.toHex() "<<SecondArray.toHex();

    time_RealClock.setHMS(HourQuint8, MinuteQuint8, SecondQuint8);

    //
    //Day
    QByteArray dayArray;
    dayArray.append(buffer[7]);

    quint8 dayQuint8 = buffer[7];

    if(dayQuint8>=1 && dayQuint8<=31) {

    }
    else {
        QMessageBox::information(this, "", tr("Недопустимое значение: День месяца"));
    }

    qDebug()<<"dayArray.toHex()"<<dayArray.toHex();

    //
    //Month
    QByteArray monthArray;
    monthArray.append(buffer[8]);

    quint8 monthQuint8 = buffer[8];

    if(monthQuint8>=1 && monthQuint8<=12) {

    }
    else {
        QMessageBox::information(this, "", tr("Недопустимое значение: Месяц"));
    }

    qDebug()<<"monthArray.toHex()"<<monthArray.toHex();

    //
    //Year
    QByteArray yearArray;
    yearArray.append(buffer[9]);


    quint16 yearQuint16 = buffer[9] + 2000;

    if(yearQuint16>=1990 && yearQuint16<=3000) {

    }
    else {
        QMessageBox::information(this, "", tr("Недопустимое значение: Год"));
    }

    qDebug()<<"yearArray.toHex()"<<yearArray.toHex();

    //
    //DOW
    QByteArray DOWArray;
    DOWArray.append(buffer[10]);


    quint8 DOWQuint8 = buffer[10];

    if(DOWQuint8>=1 && DOWQuint8<=7) {

    }
    else {
        QMessageBox::information(this, "", tr("Недопустимое значение: День недели"));
    }

    qDebug()<<"DOWArray.toHex()"<<DOWArray.toHex();

    date_RealClock.setDate(yearQuint16, monthQuint8, dayQuint8);

    dateTime_RealClock.setTime(time_RealClock);
    dateTime_RealClock.setDate(date_RealClock);
    ui->dateTimeEdit_realClock->setDisplayFormat("dd.MM.yyyy hh:mm:ss");
    ui->dateTimeEdit_realClock->setDateTime(dateTime_RealClock);

    //
    //ASW_flag
    QByteArray ASW_flagArray;
    ASW_flagArray.append(buffer[12]);


    quint8 ASW_flagQuint8 = buffer[12];

    if(ASW_flagList.contains(QString::number(ASW_flagQuint8))) {
        ui->comboBox_ASW_flag->setCurrentText(QString::number(ASW_flagQuint8));
    }
    else {
        QMessageBox::information(this, "", tr("Недопустимое значение: Автоматический переход зима-лето"));
    }

    qDebug()<<"ASW_flagArray.toHex()"<<ASW_flagArray.toHex();
    ui->lineEdit_ASW_flagHex->setText(ASW_flagArray.toHex());
}
/***********************************************************/
void FormParamsEdit::getbufferReadCalibrationFactor2(QByteArray buffer)
{
    //Shift_T1
    QByteArray Shift_T1Array;
    Shift_T1Array.append(buffer[18]);
    Shift_T1Array.append(buffer[19]);
    qDebug()<<"Shift_T1Array.toHex() "<<Shift_T1Array.toHex();

    quint16 Shift_T1Quint16 = buffer[18];
    Shift_T1Quint16 = Shift_T1Quint16<<8 | buffer[19];

    float Shift_T1Float = (float)Shift_T1Quint16/1000000;

    if((int)Shift_T1Float == 0) {
        ui->lineEdit_Shift_T1->setText("0.00000");
    }
    else {
 //       QMessageBox::information(this, "", tr("Недопустимое значение: Коэфффициент коррекции shift_t1 ") + QString::number(Shift_T1Float));
    }

    ui->lineEdit_Shift_T1Hex->setText(Shift_T1Array.toHex());

    //Shift_T2
    QByteArray Shift_T2Array;
    Shift_T2Array.append(buffer[20]);
    Shift_T2Array.append(buffer[21]);
    qDebug()<<"Shift_T2Array.toHex() "<<Shift_T2Array.toHex();

    quint16 Shift_T2Quint16 = buffer[20];
    Shift_T2Quint16 = Shift_T2Quint16<<8 | buffer[21];

    float Shift_T2Float = (float)Shift_T2Quint16/1000000;

    if((int)Shift_T2Float == 0) {
        ui->lineEdit_Shift_T2->setText("0.00000");
    }
    else {
 //       QMessageBox::information(this, "", tr("Недопустимое значение: Коэфффициент коррекции shift_t2 ") + QString::number(Shift_T2Float));
    }

    ui->lineEdit_Shift_T2Hex->setText(Shift_T2Array.toHex());

    //
    //Scale_T1
    QByteArray Scale_T1Array;
    Scale_T1Array.append(buffer[22]);
    Scale_T1Array.append(buffer[23]);
    Scale_T1Array.append(buffer[24]);
    Scale_T1Array.append(buffer[25]);
    qDebug()<<"Scale_T1Array.toHex() "<<Scale_T1Array.toHex();

    quint32 Scale_T1Quint32 = (quint8)buffer[22];
    Scale_T1Quint32 = Scale_T1Quint32<<8 | (quint8)buffer[23];
    Scale_T1Quint32 = Scale_T1Quint32<<8 | (quint8)buffer[24];
    Scale_T1Quint32 = Scale_T1Quint32<<8 | (quint8)buffer[25];

    float Scale_T1Float = (float)Scale_T1Quint32/1000000;

    if((int)Scale_T1Float == 1) {
        ui->lineEdit_Scale_T1->setText("1.00000");
    }
    else {
 //       QMessageBox::information(this, "", tr("Недопустимое значение: Коэфффициент коррекции scale_t1 ") + QString::number(Scale_T1Float));
    }

    ui->lineEdit_Scale_T1Hex->setText(Scale_T1Array.toHex());

    //
    //Scale_T2
    QByteArray Scale_T2Array;
    Scale_T2Array.append(buffer[26]);
    Scale_T2Array.append(buffer[27]);
    Scale_T2Array.append(buffer[28]);
    Scale_T2Array.append(buffer[29]);
    qDebug()<<"Scale_T2Array.toHex() "<<Scale_T2Array.toHex();

    quint32 Scale_T2Quint32 = (quint8)buffer[26];
    Scale_T2Quint32 = Scale_T2Quint32<<8 | (quint8)buffer[27];
    Scale_T2Quint32 = Scale_T2Quint32<<8 | (quint8)buffer[28];
    Scale_T2Quint32 = Scale_T2Quint32<<8 | (quint8)buffer[29];

    float Scale_T2Float = (float)Scale_T2Quint32/1000000;

    if((int)Scale_T2Float == 1) {
        ui->lineEdit_Scale_T2->setText("1.00000");
//           ui->lineEdit_Scale_T2->setText(QString::number((int)Scale_T2Float));
    }
    else {
//        QMessageBox::information(this, "", tr("Недопустимое значение: Коэфффициент коррекции scale_t2 ") + QString::number(Scale_T2Float));
    }

    ui->lineEdit_Scale_T2Hex->setText(Scale_T2Array.toHex());
}
/***********************************************************/
void FormParamsEdit::getbufferReadExternalInterfaceSettings(QByteArray buffer)
{
    //Addr
    QByteArray AddrArray;
    AddrArray.append(buffer[4]);
    qDebug()<<"AddrArray.toHex() "<<AddrArray.toHex();

    quint8 AddrQuint8 = (quint8)buffer[4];

    if(AddrQuint8>=0 && AddrQuint8<=252) {
        ui->spinBox_Addr->setValue(AddrQuint8);
    }
    else {
//        QMessageBox::information(this, "", tr("Недопустимое значение: Первичный адрес счётчика"));
        ui->spinBox_Addr->setValue(AddrQuint8);
    }

    ui->lineEdit_AddrHex->setText(AddrArray.toHex());

    //
    //BaudRate
    QByteArray BaudRateArray;
    BaudRateArray.append(buffer[5]);
    qDebug()<<"BaudRateArray.toHex() "<<BaudRateArray.toHex();

    quint8 BaudRateQuint8 = (quint8)buffer[5];

    for(int j=0; j< ui->comboBox_BaudRate->count(); j++) {
        ui->comboBox_BaudRate->setCurrentIndex(j);
        if(BaudRateQuint8 == (quint8)ui->comboBox_BaudRate->currentData().toInt()) {
            break;
        }

        if(j==3 && (BaudRateQuint8 != (quint8)ui->comboBox_BaudRate->currentData().toInt())) {
             QMessageBox::information(this, "", tr("Недопустимое значение: Скорость обмена ") +
                                      QString::number(BaudRateQuint8, 16));
        }
    }

//    if(BaudRateList.contains(QString::number(BaudRateQuint8, 16))) {
//        ui->comboBox_BaudRate->setCurrentText(QString::number(BaudRateQuint8, 16));
//    }
//    else {
//        QMessageBox::information(this, "", tr("Недопустимое значение: Скорость обмена ") + QString::number(BaudRateQuint8, 16));
//    }

    ui->lineEdit_BaudRateHex->setText(BaudRateArray.toHex());

    //
    //HMP_Trans 2bytes  6,7
    QByteArray HMP_TransArray;
    HMP_TransArray.append(buffer[6]);
    HMP_TransArray.append(buffer[7]);
    qDebug()<<"HMP_TransArray.toHex() "<<HMP_TransArray.toHex();

    quint16 HMP_TransInt = (quint8)buffer[6];
    HMP_TransInt = HMP_TransInt<<8 | (quint8)buffer[7];

    if(HMP_TransInt == 0x3fff) {
        ui->lineEdit_HMP_Trans->setText(QString::number(HMP_TransInt, 16));
    }
    else {
//        QMessageBox::information(this, "", tr("Недопустимое значение: Позиционный код передаваемых параметров HMP_Trans ") +
//                                QString::number(HMP_TransInt, 16));
    }

    ui->lineEdit_HMP_TransHex->setText(HMP_TransArray.toHex());

    //
    //EPP_Trans 2bytes  8,9
    QByteArray EPP_TransArray;
    EPP_TransArray.append(buffer[8]);
    EPP_TransArray.append(buffer[9]);
    qDebug()<<"EPP_TransArray.toHex() "<<EPP_TransArray.toHex();

    quint16 EPP_TransInt = (quint8)buffer[8];
    EPP_TransInt = EPP_TransInt<<8 | (quint8)buffer[9];

    if(EPP_TransInt == 0x000f) {
        ui->lineEdit_EPP_Trans->setText(QString::number(EPP_TransInt, 16));
    }
    else {
 //       QMessageBox::information(this, "", tr("Недопустимое значение: Позиционный код передаваемых параметров EPP_Trans ") +
 //                                QString::number(EPP_TransInt, 16));
    }

    ui->lineEdit_EPP_TransHex->setText(EPP_TransArray.toHex());

    //
    //P1_PrimAddr 1byte  10
    QByteArray P1_PrimAddrArray;
    P1_PrimAddrArray.append(buffer[10]);
    qDebug()<<"P1_PrimAddrArray.toHex() "<<P1_PrimAddrArray.toHex();

    quint8 P1_PrimAddrInt = (quint8)buffer[10];

    if(P1_PrimAddrInt>=0 && P1_PrimAddrInt<=252) {
        ui->spinBox_P1PrimAddr->setValue(P1_PrimAddrInt);
    }
    else {
//        QMessageBox::information(this, "", tr("Недопустимое значение: Первичный адрес импульсных входов P1_PrimAddr ") +
//                                 QString::number(P1_PrimAddrInt, 10));
    }

    ui->lineEdit_P1PrimAddrHex->setText(P1_PrimAddrArray.toHex());

    //
    //P2_PrimAddr 1byte  11
    QByteArray P2_PrimAddrArray;
    P2_PrimAddrArray.append(buffer[11]);
    qDebug()<<"P2_PrimAddrArray.toHex() "<<P2_PrimAddrArray.toHex();

    quint8 P2_PrimAddrInt = (quint8)buffer[11];

    if(P2_PrimAddrInt>=0 && P2_PrimAddrInt<=252) {
        ui->spinBox_P2PrimAddr->setValue(P2_PrimAddrInt);
    }
    else {
//        QMessageBox::information(this, "", tr("Недопустимое значение: Первичный адрес импульсных входов P2_PrimAddr ") +
//                                 QString::number(P2_PrimAddrInt, 10));
    }

    ui->lineEdit_P2PrimAddrHex->setText(P2_PrimAddrArray.toHex());

    //
    //ProtType 1byte  12
    QByteArray ProtTypeArray;
    ProtTypeArray.append(buffer[12]);
    qDebug()<<"ProtTypeArray.toHex() "<<ProtTypeArray.toHex();

    quint8 ProtTypeInt = (quint8)buffer[12];

    if(ProtTypeInt == 0xff) {
        ui->lineEdit_ProtType->setText(QString::number(ProtTypeInt, 16));
    }
    else {
//        QMessageBox::information(this, "", tr("Недопустимое значение: Первичный адрес импульсных входов P2_PrimAddr ") +
//                                 QString::number(ProtTypeInt, 16));
    }

    ui->lineEdit_ProtTypeHex->setText(ProtTypeArray.toHex());
}
/***********************************************************/
void FormParamsEdit::getbufferReadExternalDeviceSettings(QByteArray buffer)
{
    //Serial_P1
    QByteArray Serial_P1Array;
    Serial_P1Array.append(buffer[4]);
    Serial_P1Array.append(buffer[5]);
    Serial_P1Array.append(buffer[6]);
    Serial_P1Array.append(buffer[7]);
    qDebug()<<"Serial_P1Array.toHex() "<<Serial_P1Array.toHex();

    quint32 Serial_P1Quint32 = (quint8)buffer[4];
    Serial_P1Quint32 = Serial_P1Quint32<<8 | (quint8)buffer[5];
    Serial_P1Quint32 = Serial_P1Quint32<<8 | (quint8)buffer[6];
    Serial_P1Quint32 = Serial_P1Quint32<<8 | (quint8)buffer[7];

//    if(Serial_P1Quint32 == 0x00000000) {
        ui->lineEdit_Serial_P1->setText(Serial_P1Array.toHex());//(Serial_P1Quint32, 16));
//    }
//    else {
//  //        QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 1 Serial_P1 ")
//  //                                 + QString::number(Serial_P1Quint32, 16));
//    }

    ui->lineEdit_Serial_P1Hex->setText(Serial_P1Array.toHex());

    //
    //ManCode_P1
    QByteArray ManCode_P1Array;
    ManCode_P1Array.append(buffer[8]);
    ManCode_P1Array.append(buffer[9]);
    qDebug()<<"ManCode_P1Array.toHex() "<<ManCode_P1Array.toHex();

    int manufacturerP1Sum = static_cast<quint8>(ManCode_P1Array[0]);//manufacturer.toInt();
    manufacturerP1Sum = (manufacturerP1Sum<<8)|(static_cast<quint8>(ManCode_P1Array[1]));
    quint8 symbol1_1, symbol1_2, symbol1_3;
    symbol1_1 = ((manufacturerP1Sum/(32*32))&0x1F) + 64;
    symbol1_2 = ((manufacturerP1Sum/32)&0x1F) + 64;
    symbol1_3 = (manufacturerP1Sum&0x1F) + 64;
    QByteArray manufacturerP1String;
    manufacturerP1String[0] = symbol1_1;
    manufacturerP1String[1] = symbol1_2;
    manufacturerP1String[2] = symbol1_3;

//    if(manCodeList.contains(QString::fromLatin1(manufacturerString))) {

        ui->lineEdit_ManCode_P1->setText(QString::fromLatin1(manufacturerP1String));

//        ui->comboBox_ManCode->setStyleSheet("QLabel { background-color : white; color : red; }");
//    }
//    else {
//       QMessageBox::information(this, "", tr("Недопустимое значение: Производитель"));
//    }

//    quint16 ManCode_P1Quint16 = (quint8)buffer[8];
//    ManCode_P1Quint16 = ManCode_P1Quint16<<8 | (quint8)buffer[9];

//    if(ManCode_P1Quint16 == 0x0000) {
//        ui->lineEdit_ManCode_P1->setText(QString::number(ManCode_P1Quint16, 16));
//    }
//    else {
//  //        QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 1 ManCode_P1 ")
//  //                                 + QString::number(ManCode_P1Quint16, 16));
//    }

    ui->lineEdit_ManCode_P1Hex->setText(ManCode_P1Array.toHex());

    //
    //DevType_P1
    QByteArray DevType_P1Array;
    DevType_P1Array.append(buffer[10]);
    qDebug()<<"DevType_P1Array.toHex() "<<DevType_P1Array.toHex();

    quint8 DevType_P1Int = (quint8)buffer[10];

    if(DevType_P1Int == 0x00) {
        ui->lineEdit_DevType_P1->setText(QString::number(DevType_P1Int, 16));
    }
    else {
//        QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 1 DevType_P1 ") +
//                                 QString::number(DevType_P1Int, 16));
    }

    ui->lineEdit_DevType_P1Hex->setText(DevType_P1Array.toHex());

    //
    //Version_P1
    QByteArray Version_P1Array;
    Version_P1Array.append(buffer[11]);
    qDebug()<<"Version_P1Array.toHex() "<<Version_P1Array.toHex();

    quint8 Version_P1Int = (quint8)buffer[11];

    if(Version_P1Int == 0) {
        ui->lineEdit_Version_P1->setText(QString::number(Version_P1Int, 10));
    }
    else {
//        QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 1 Version_P1 ") +
//                                 QString::number(Version_P1Int, 10));
    }

    ui->lineEdit_Version_P1Hex->setText(Version_P1Array.toHex());

    //
    //Initial_value_P1   12-15
    QByteArray Initial_value_P1Array;
    Initial_value_P1Array.append(buffer[12]);
    Initial_value_P1Array.append(buffer[13]);
    Initial_value_P1Array.append(buffer[14]);
    Initial_value_P1Array.append(buffer[15]);

    qDebug()<<"Initial_value_P1Array.toHex() "<<Initial_value_P1Array.toHex();

    quint32 Initial_value_P1Int = (quint8)buffer[12];
    Initial_value_P1Int = Initial_value_P1Int<<8 | (quint8)buffer[13];
    Initial_value_P1Int = Initial_value_P1Int<<8 | (quint8)buffer[14];
    Initial_value_P1Int = Initial_value_P1Int<<8 | (quint8)buffer[15];

    if(Initial_value_P1Int == 0) {
        ui->lineEdit_Initial_value_P1->setText(QString::number(Initial_value_P1Int));
    }
    else {
//        QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 1 Initial_value_P1 ") +
//                                 QString::number(Initial_value_P1Int));
    }

    ui->lineEdit_Initial_value_P1Hex->setText(Initial_value_P1Array.toHex());

    //
    //Service_P1  16-19
    QByteArray Service_P1Array;
    Service_P1Array.append(buffer[16]);
    Service_P1Array.append(buffer[17]);
    Service_P1Array.append(buffer[18]);
    Service_P1Array.append(buffer[19]);

    qDebug()<<"Service_P1Array.toHex() "<<Service_P1Array.toHex();

    quint32 Service_P1Int = (quint8)buffer[16];
    Service_P1Int = Service_P1Int<<8 | (quint8)buffer[17];
    Service_P1Int = Service_P1Int<<8 | (quint8)buffer[18];
    Service_P1Int = Service_P1Int<<8 | (quint8)buffer[19];

    if(Service_P1Int == 0x00000000) {
        ui->lineEdit_Service_P1->setText(QString::number(Service_P1Int, 16));
    }
    else {
//        QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 1 Service_P1 ") +
//                                 QString::number(Service_P1Int, 16));
    }

    ui->lineEdit_Service_P1Hex->setText(Service_P1Array.toHex());

    //
    //Serial P2
    QByteArray Serial_P2Array;
    Serial_P2Array.append(buffer[20]);
    Serial_P2Array.append(buffer[21]);
    Serial_P2Array.append(buffer[22]);
    Serial_P2Array.append(buffer[23]);
    qDebug()<<"Serial_P2Array.toHex() "<<Serial_P2Array.toHex();

    quint32 Serial_P2Quint32 = (quint8)buffer[20];
    Serial_P2Quint32 = Serial_P2Quint32<<8 | (quint8)buffer[21];
    Serial_P2Quint32 = Serial_P2Quint32<<8 | (quint8)buffer[22];
    Serial_P2Quint32 = Serial_P2Quint32<<8 | (quint8)buffer[23];

//    if(Serial_P2Quint32 == 0x00000000) {
        ui->lineEdit_Serial_P2->setText(Serial_P2Array.toHex());//(Serial_P2Quint32, 16));
//    }
//    else {
//  //        QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 2 Serial_P2 ")
//  //                                 + QString::number(Serial_P2Quint32, 16));
//    }

    ui->lineEdit_Serial_P2Hex->setText(Serial_P2Array.toHex());

    //
    //ManCode P2
    QByteArray ManCode_P2Array;
    ManCode_P2Array.append(buffer[24]);
    ManCode_P2Array.append(buffer[25]);
    qDebug()<<"ManCode_P2Array.toHex() "<<ManCode_P2Array.toHex();

    int manufacturerP2Sum = static_cast<quint8>(ManCode_P2Array[0]);//manufacturer.toInt();
    manufacturerP2Sum = (manufacturerP2Sum<<8)|(static_cast<quint8>(ManCode_P2Array[1]));
    quint8 symbol1, symbol2, symbol3;
    symbol1 = ((manufacturerP2Sum/(32*32))&0x1F) + 64;
    symbol2 = ((manufacturerP2Sum/32)&0x1F) + 64;
    symbol3 = (manufacturerP2Sum&0x1F) + 64;
    QByteArray manufacturerP2String;
    manufacturerP2String[0] = symbol1;
    manufacturerP2String[1] = symbol2;
    manufacturerP2String[2] = symbol3;

    ui->lineEdit_ManCode_P2->setText(QString::fromLatin1(manufacturerP2String));



//    quint16 ManCode_P2Quint16 = (quint8)buffer[24];
//    ManCode_P2Quint16 = ManCode_P2Quint16<<8 | (quint8)buffer[25];

//    if(ManCode_P2Quint16 == 0x0000) {

//        ui->lineEdit_ManCode_P2->setText(QString::number(ManCode_P2Quint16, 16));

//    }
//    else {
//        QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 2 ManCode_P2 ")
//                                 + QString::number(ManCode_P2Quint16, 16));
//    }

    ui->lineEdit_ManCode_P2Hex->setText(ManCode_P2Array.toHex());

    //
    //DevType P2
    QByteArray DevType_P2Array;
    DevType_P2Array.append(buffer[26]);
    qDebug()<<"DevType_P2Array.toHex() "<<DevType_P2Array.toHex();

    quint8 DevType_P2Int = (quint8)buffer[26];

    if(DevType_P2Int == 0x00) {
        ui->lineEdit_DevType_P2->setText(QString::number(DevType_P2Int, 16));
    }
    else {
//        QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 2 DevType_P2 ") +
//                                 QString::number(DevType_P2Int, 16));
    }

    ui->lineEdit_DevType_P2Hex->setText(DevType_P2Array.toHex());

    //
    //Version P2
    QByteArray Version_P2Array;
    Version_P2Array.append(buffer[27]);
    qDebug()<<"Version_P2Array.toHex() "<<Version_P2Array.toHex();

    quint8 Version_P2Int = (quint8)buffer[27];

    if(Version_P2Int == 0) {
        ui->lineEdit_Version_P2->setText(QString::number(Version_P2Int, 10));
    }
    else {
//        QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 2 Version_P2 ") +
//                                 QString::number(Version_P2Int, 10));
    }

    ui->lineEdit_Version_P2Hex->setText(Version_P2Array.toHex());

    //
    //Initial value P2
    QByteArray Initial_value_P2Array;
    Initial_value_P2Array.append(buffer[28]);
    Initial_value_P2Array.append(buffer[29]);
    Initial_value_P2Array.append(buffer[30]);
    Initial_value_P2Array.append(buffer[31]);

    qDebug()<<"Initial_value_P2Array.toHex() "<<Initial_value_P2Array.toHex();

    quint32 Initial_value_P2Int = (quint8)buffer[28];
    Initial_value_P2Int = Initial_value_P2Int<<8 | (quint8)buffer[29];
    Initial_value_P2Int = Initial_value_P2Int<<8 | (quint8)buffer[30];
    Initial_value_P2Int = Initial_value_P2Int<<8 | (quint8)buffer[31];

    if(Initial_value_P2Int == 0) {
        ui->lineEdit_Initial_value_P2->setText(QString::number(Initial_value_P2Int));
    }
    else {
//        QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 2 Initial_value_P2 ") +
//                                 QString::number(Initial_value_P2Int));
    }

    ui->lineEdit_Initial_value_P2Hex->setText(Initial_value_P2Array.toHex());

    //
    //Service P2   32-35
    QByteArray Service_P2Array;
    Service_P2Array.append(buffer[32]);
    Service_P2Array.append(buffer[33]);
    Service_P2Array.append(buffer[34]);
    Service_P2Array.append(buffer[35]);

    qDebug()<<"Service_P2Array.toHex() "<<Service_P2Array.toHex();

    quint32 Service_P2Int = (quint8)buffer[32];
    Service_P2Int = Service_P2Int<<8 | (quint8)buffer[33];
    Service_P2Int = Service_P2Int<<8 | (quint8)buffer[34];
    Service_P2Int = Service_P2Int<<8 | (quint8)buffer[35];

    if(Service_P2Int == 0x00000000) {
        ui->lineEdit_Service_P2->setText(QString::number(Service_P2Int, 16));
    }
    else {
//        QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 2 Service_P2 ") +
//                                 QString::number(Service_P2Int, 16));
    }

    ui->lineEdit_Service_P2Hex->setText(Service_P2Array.toHex());
}
/***********************************************************/
void FormParamsEdit::on_lineEdit_serial_textChanged(const QString &arg1)
{
    QString argstring = arg1;
    QString arg1Help = arg1;

    QString argHelpPart1 = arg1Help.left(3);
    QString argHelpPart2 = arg1Help.remove(0, 3);

    //добавление нулей
    if(argHelpPart2.length() < 5) {
        int nullNumber = 5 - argHelpPart2.length();
        for(int i=0; i<nullNumber; i++) {
            argHelpPart2.insert(0, '0');
        }
    }

    argstring = argHelpPart1 + argHelpPart2;

    ui->lineEdit_serial->setText(argstring);

//    if(argstring.length() < 8) {
//        int nullNumber = 8 - argstring.length();
//        for(int i=0; i<nullNumber; i++) {
//            argstring.insert(0, '0');
//        }
//    }
    //добавление нулей/

    QString pcb_sn_String = ui->lineEdit_PCB_SN->text().left(8);
    pcb_sn_String = pcb_sn_String + argstring;
    ui->lineEdit_PCB_SN->setText(pcb_sn_String);

//    ui->textBrowser->append(argstring);

    serialNumber.clear();
    for(int i=0; i<argstring.size(); i=i+2) {
        QString str = argstring.at(i);
 //       qDebug()<<"arg1[i]"<<str;
        QString str1 = argstring.at(i+1);
        quint8 byte = static_cast<quint8>(str.toInt());
        byte = (byte<<4)|(static_cast<quint8>(str1.toInt()));
        qDebug()<<"byte"<<byte;
//        str = str + arg1[i+1];
//        qDebug()<<"arg1[i] + arg1[i+1]"<<str;
        serialNumber.append(byte);
    }
//   QByteArray deviceNumberFromLineEdit = arg1.toLocal8Bit();
//     deviceNumberFromLineEdit = arg1.toInt();
   qDebug()<<"serialNumber.toHex()"<<serialNumber.toHex();

   paramsMap["serialNumber"] = serialNumber;
   emit sendParamsMap(paramsMap);

 //  signalSendSerialFromParamsEdit(arg1);
}
/***********************************************************/
void FormParamsEdit::on_comboBox_ManCode_currentIndexChanged(const QString &arg1)
{
    manCode.clear();

    QString str = arg1.toUpper();

 //  QByteArray manufacturerFromLineEdit = arg1.toLocal8Bit();
   QByteArray manufacturerFromLineEdit = str.toLocal8Bit();
//   manufacturerFromLineEdit = manufacturerFromLineEdit.toInt();
   quint16 manufacturerSum = (static_cast<quint8>(manufacturerFromLineEdit[0])-64)*32*32 +
                              (static_cast<quint8>(manufacturerFromLineEdit[1])-64)*32 +
                               (static_cast<quint8>(manufacturerFromLineEdit[2])-64);
   quint8 byte = (quint8)(manufacturerSum>>8);
   manCode.append(byte);
   byte = (quint8)manufacturerSum;
   manCode.append(byte);
//   quint8 x = manufacturerFromLineEdit[0];//byteArray[0];
//   quint8 y = manufacturerFromLineEdit[1];//byteArray[1];
//   quint8 z = manufacturerFromLineEdit[2];//byteArray[2];
   quint8 symbol1, symbol2, symbol3;
   symbol1 = ((manufacturerSum/(32*32))&0x1F) + 64;
   symbol2 = ((manufacturerSum/32)&0x1F) + 64;
   symbol3 = (manufacturerSum&0x1F) + 64;
   QByteArray manufacturerString;
   manufacturerString[0] = symbol1;
   manufacturerString[1] = symbol2;
   manufacturerString[2] = symbol3;
   qDebug()<<"symbol1 "<<QString::number(symbol1)<<" symbol2 "<<QString::number(symbol2)<<" symbol3 "<<QString::number(symbol3);
   qDebug()<<"manufacturerFromLineEdit"<<manufacturerFromLineEdit;
   qDebug()<<"manufacturerFromLineEditHex"<<manufacturerFromLineEdit.toHex();
   qDebug()<<"manufacturerSum"<<manufacturerSum;

   paramsMap["manCode"] = manCode;
   emit sendParamsMap(paramsMap);
}
/***********************************************************/
void FormParamsEdit::on_comboBox_DevType_currentIndexChanged(const QString &arg1)
{
    devType.clear();

    if(arg1 == "Обратный") {
       devType.append((quint8)0x04);
    }
    if(arg1 == "Подающий") {
       devType.append((quint8)0x0c);
    }

//    bool ok;
//    devType.append((quint8)arg1.toInt(&ok, 16));

    paramsMap["devType"] = devType;
    emit sendParamsMap(paramsMap);
}
/***********************************************************/
void FormParamsEdit::on_comboBox_DN_currentIndexChanged(const QString &arg1)
{
    DN.clear();
    bool ok;
    DN.append((quint8)arg1.toInt(&ok, 16));

    paramsMap["DN"] = DN;
    emit sendParamsMap(paramsMap);
}
/***********************************************************/
void FormParamsEdit::on_lineEdit_PCB_SN_textChanged(const QString &arg1)
{
    if(arg1.length() != 16) {
        return;
    }

    QString argstring = arg1;

//    if(argstring.length() < 8) {
//        int nullNumber = 8 - argstring.length();
//        for(int i=0; i<nullNumber; i++) {
//            argstring.insert(0, '0');
//        }
//    }

//    ui->textBrowser->append(argstring);

    PCB_SN_ByteArray.clear();
    for(int i=0; i<argstring.length(); i=i+2) {
        QString str = argstring.at(i);
 //       qDebug()<<"arg1[i]"<<str;
        QString str1 = argstring.at(i+1);
        bool ok;
        quint8 byte = static_cast<quint8>(str.toInt(&ok, 16));
        byte = (byte<<4)|(static_cast<quint8>(str1.toInt(&ok, 16)));
        qDebug()<<"byte"<<byte;
//        str = str + arg1[i+1];
//        qDebug()<<"arg1[i] + arg1[i+1]"<<str;
        PCB_SN_ByteArray.append(byte);
    }


   qDebug()<<"PCB_SN_ByteArray.toHex()"<<PCB_SN_ByteArray.toHex();

   paramsMap["PCB_SN_ByteArray"] = PCB_SN_ByteArray;
   emit sendParamsMap(paramsMap);
}
/***********************************************************/
void FormParamsEdit::on_lineEdit_Flow_min_textChanged(const QString &arg1)
{
    flowMinByteArray.clear();
    bool ok;
    QString str = arg1;
       if(str.contains(",")) {
             str.replace(",", ".");
       }
//       qDebug()<<"str"<<str;
//       qDebug()<<"str.toFloat() "<<str.toFloat(&ok);

    float flowMinFloat = str.toFloat(&ok);
    if(ok == false) {
        QMessageBox::information(this, "", "Не удалось преобразовать flowMin в тип float");
        return;
    }

//    if(flowMinFloat<0.008) {
//        ui->lineEdit_Flow_min->setText("0.008");
//    }
//    if(flowMinFloat>0.2) {
//        ui->lineEdit_Flow_min->setText("0.2");
//    }

    flowMinFloat = flowMinFloat*1000;

    quint16 flowMinInt = (quint16)flowMinFloat;
//    qDebug()<<"flowMinInt "<<flowMinInt;

    flowMinByteArray.append((quint8)(flowMinInt>>8));
    flowMinByteArray.append((quint8)flowMinInt);

//    qDebug()<<"flowMinByteArray.toHex() "<<flowMinByteArray.toHex();

    paramsMap["flowMinByteArray"] = flowMinByteArray;
    emit sendParamsMap(paramsMap);
}
/***********************************************************/
void FormParamsEdit::on_lineEdit_Flow_max_textChanged(const QString &arg1)
{
    flowMaxByteArray.clear();

    bool ok;
    QString str = arg1;
       if(str.contains(",")) {
             str.replace(",", ".");
       }
//       qDebug()<<"str"<<str;
//       qDebug()<<"str.toFloat() "<<str.toFloat(&ok);

    float flowMaxFloat = str.toFloat(&ok);
    if(ok == false) {
        QMessageBox::information(this, "", "Не удалось преобразовать flowMax в тип float");
        return;
    }

    flowMaxFloat = flowMaxFloat*1000;

    quint16 flowMaxInt = (quint16)flowMaxFloat;
//    qDebug()<<"flowMaxInt "<<flowMaxInt;

    flowMaxByteArray.append((quint8)(flowMaxInt>>8));
    flowMaxByteArray.append((quint8)flowMaxInt);

//    qDebug()<<"flowMaxByteArray.toHex() "<<flowMaxByteArray.toHex();

    paramsMap["flowMaxByteArray"] = flowMaxByteArray;
    emit sendParamsMap(paramsMap);
}
/***********************************************************/
void FormParamsEdit::on_spinBox_Tin_min_valueChanged(int arg1)
{
    tinMinByteArray.clear();
    tinMinByteArray.append((quint8)arg1);

//    qDebug()<<"tinMinByteArray.toHex()"<<tinMinByteArray.toHex();

    paramsMap["tinMinByteArray"] = tinMinByteArray;
    emit sendParamsMap(paramsMap);
}
/***********************************************************/
void FormParamsEdit::on_spinBox_Tin_max_valueChanged(int arg1)
{
    tinMaxByteArray.clear();
    tinMaxByteArray.append((quint8)arg1);

//    qDebug()<<"tinMaxByteArray.toHex()"<<tinMaxByteArray.toHex();

    paramsMap["tinMaxByteArray"] = tinMaxByteArray;
    emit sendParamsMap(paramsMap);
}
/***********************************************************/
void FormParamsEdit::on_spinBox_Tout_min_valueChanged(int arg1)
{
    toutMinByteArray.clear();
    toutMinByteArray.append((quint8)arg1);

//    qDebug()<<"toutMinByteArray.toHex()"<<toutMinByteArray.toHex();

    paramsMap["toutMinByteArray"] = toutMinByteArray;
    emit sendParamsMap(paramsMap);
}
/***********************************************************/
void FormParamsEdit::on_spinBox_Tout_max_valueChanged(int arg1)
{
    toutMaxByteArray.clear();
    toutMaxByteArray.append((quint8)arg1);

//    qDebug()<<"toutMaxByteArray.toHex()"<<toutMaxByteArray.toHex();

    paramsMap["toutMaxByteArray"] = toutMaxByteArray;
    emit sendParamsMap(paramsMap);
}
/***********************************************************/
void FormParamsEdit::on_spinBox_ReportData_valueChanged(int arg1)
{
    reportDataByteArray.clear();
    reportDataByteArray.append((quint8)arg1);

    qDebug()<<"reportDataByteArray.toHex()"<<reportDataByteArray.toHex();

    paramsMap["reportDataByteArray"] = reportDataByteArray;
    emit sendParamsMap(paramsMap);
}
/***********************************************************/
void FormParamsEdit::on_comboBox_Average_T_currentIndexChanged(const QString &arg1)
{
    averageTByteArray.clear();
    bool ok;
    averageTByteArray.append((quint8)(arg1.toInt(&ok, 10)));

//    qDebug()<<"averageTByteArray.toHex().toHex()"<<averageTByteArray.toHex();

    paramsMap["averageTByteArray"] = averageTByteArray;
    emit sendParamsMap(paramsMap);
}
/***********************************************************/
void FormParamsEdit::on_comboBox_PulsesMode1_currentIndexChanged(const QString &arg1)
{
    pulsesMode1ByteArray.clear();
    bool ok;
    pulsesMode1ByteArray.append((quint8)(arg1.toInt(&ok, 10)));

//    qDebug()<<"pulsesMode1ByteArray.toHex().toHex()"<<pulsesMode1ByteArray.toHex();

    paramsMap["pulsesMode1ByteArray"] = pulsesMode1ByteArray;
    emit sendParamsMap(paramsMap);
}
/***********************************************************/
void FormParamsEdit::on_comboBox_PulsesMode2_currentIndexChanged(const QString &arg1)
{
    pulsesMode2ByteArray.clear();
    bool ok;
    pulsesMode2ByteArray.append((quint8)(arg1.toInt(&ok, 10)));

//    qDebug()<<"pulsesMode2ByteArray.toHex().toHex()"<<pulsesMode2ByteArray.toHex();

    paramsMap["pulsesMode2ByteArray"] = pulsesMode2ByteArray;
    emit sendParamsMap(paramsMap);
}
/***********************************************************/
void FormParamsEdit::on_lineEdit_PulsesValue1_textChanged(const QString &arg1)
{
    pulsesValue1ByteArray.clear();

    bool ok;
    quint16 pulsesValue1Int = arg1.toInt(&ok, 10);
    pulsesValue1ByteArray.append((quint8)(pulsesValue1Int>>8));
    pulsesValue1ByteArray.append((quint8)pulsesValue1Int);

    qDebug()<<"pulsesValue1ByteArray.toHex() "<<pulsesValue1ByteArray.toHex();

    paramsMap["pulsesValue1ByteArray"] = pulsesValue1ByteArray;
    emit sendParamsMap(paramsMap);

}
/***********************************************************/
void FormParamsEdit::on_lineEdit_PulsesValue2_textChanged(const QString &arg1)
{
    pulsesValue2ByteArray.clear();

    bool ok;
    quint16 pulsesValue2Int = arg1.toInt(&ok, 10);
    pulsesValue2ByteArray.append((quint8)(pulsesValue2Int>>8));
    pulsesValue2ByteArray.append((quint8)pulsesValue2Int);

    qDebug()<<"pulsesValue2ByteArray.toHex() "<<pulsesValue2ByteArray.toHex();

    paramsMap["pulsesValue2ByteArray"] = pulsesValue2ByteArray;
    emit sendParamsMap(paramsMap);
}
/***********************************************************/

void FormParamsEdit::on_comboBox_FlowRate_currentIndexChanged(const QString &arg1)
{
    flowRateByteArray.clear();
    float flowRateFloat = arg1.toFloat();
    quint32 flowRateInt = (quint32)(flowRateFloat*1000);

    flowRateByteArray.append((quint8)(flowRateInt>>24));
    flowRateByteArray.append((quint8)(flowRateInt>>16));
    flowRateByteArray.append((quint8)(flowRateInt>>8));
    flowRateByteArray.append((quint8)flowRateInt);

    qDebug()<<"flowRateByteArray.toHex() "<<flowRateByteArray.toHex();

    paramsMap["flowRateByteArray"] = flowRateByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_Pressure_textChanged(const QString &arg1)
{
    pressureByteArray.clear();
    bool ok;
    pressureByteArray.append((quint8)(arg1.toInt(&ok, 10)));

    qDebug()<<"pressureByteArray.toHex() "<<pressureByteArray.toHex();

    paramsMap["pressureByteArray"] = pressureByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_comboBox_RTD_Type_currentIndexChanged(const QString &arg1)
{
    RTD_TypeByteArray.clear();
    bool ok;
    RTD_TypeByteArray.append((quint8)(ui->comboBox_RTD_Type->currentIndex())); //(arg1.toInt(&ok, 10)));

    qDebug()<<"RTD_TypeByteArray.toHex() "<<RTD_TypeByteArray.toHex();

    paramsMap["RTD_TypeByteArray"] = RTD_TypeByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_R_RTD_textChanged(const QString &arg1)
{
    R_RTDByteArray.clear();
    bool ok;
    quint32 R_RTDInt = arg1.toInt(&ok, 10)*1000;
    R_RTDByteArray.append((quint8)(R_RTDInt>>24));
    R_RTDByteArray.append((quint8)(R_RTDInt>>16));
    R_RTDByteArray.append((quint8)(R_RTDInt>>8));
    R_RTDByteArray.append((quint8)R_RTDInt);

    quint32 r_rtd_NotMultiplicationInt = arg1.toInt(&ok, 10);
    QByteArray r_rtd_NotMultiplicationByteArray;
    r_rtd_NotMultiplicationByteArray.append((quint8)(r_rtd_NotMultiplicationInt>>24));
    r_rtd_NotMultiplicationByteArray.append((quint8)(r_rtd_NotMultiplicationInt>>16));
    r_rtd_NotMultiplicationByteArray.append((quint8)(r_rtd_NotMultiplicationInt>>8));
    r_rtd_NotMultiplicationByteArray.append((quint8)r_rtd_NotMultiplicationInt);

    qDebug()<<"R_RTDByteArray.toHex() "<<R_RTDByteArray.toHex();

    paramsMap["R_RTDByteArray"] = R_RTDByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_RWire1_textChanged(const QString &arg1)
{
    RWire1ByteArray.clear();
    bool ok;
    quint32 RWire1Int = arg1.toInt(&ok, 10);
    RWire1ByteArray.append((quint8)(RWire1Int>>24));
    RWire1ByteArray.append((quint8)(RWire1Int>>16));
    RWire1ByteArray.append((quint8)(RWire1Int>>8));
    RWire1ByteArray.append((quint8)RWire1Int);

    qDebug()<<"RWire1ByteArray.toHex() "<<RWire1ByteArray.toHex();

    paramsMap["RWire1ByteArray"] = RWire1ByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_RWire2_textChanged(const QString &arg1)
{
    RWire2ByteArray.clear();
    bool ok;
    quint32 RWire2Int = arg1.toInt(&ok, 10);
    RWire2ByteArray.append((quint8)(RWire2Int>>24));
    RWire2ByteArray.append((quint8)(RWire2Int>>16));
    RWire2ByteArray.append((quint8)(RWire2Int>>8));
    RWire2ByteArray.append((quint8)RWire2Int);

    qDebug()<<"RWire2ByteArray.toHex() "<<RWire2ByteArray.toHex();

    paramsMap["RWire2ByteArray"] = RWire2ByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_comboBox_FlowNom_currentIndexChanged(const QString &arg1)
{
    flowNomByteArray.clear();
    float flowNomFloat = arg1.toFloat();
    quint8 flowNomInt = (quint8)(flowNomFloat*10);

    flowNomByteArray.append(flowNomInt);

    qDebug()<<"flowNomByteArray.toHex() "<<flowNomByteArray.toHex();

    paramsMap["flowNomByteArray"] = flowNomByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_coef0_textChanged(const QString &arg1)
{
    flowCoef0ByteArray.clear();
    float flowCoef0Float = arg1.toFloat();
    quint32 flowCoef0Int = (quint32)(flowCoef0Float*1000000);

    flowCoef0ByteArray.append((quint8)(flowCoef0Int>>24));
    flowCoef0ByteArray.append((quint8)(flowCoef0Int>>16));
    flowCoef0ByteArray.append((quint8)(flowCoef0Int>>8));
    flowCoef0ByteArray.append((quint8)flowCoef0Int);

    paramsMap["flowCoef0ByteArray"] = flowCoef0ByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_coef1_textChanged(const QString &arg1)
{
    flowCoef1ByteArray.clear();
    float flowCoef1Float = arg1.toFloat();
    quint32 flowCoef1Int = (quint32)(flowCoef1Float*1000000);

    flowCoef1ByteArray.append((quint8)(flowCoef1Int>>24));
    flowCoef1ByteArray.append((quint8)(flowCoef1Int>>16));
    flowCoef1ByteArray.append((quint8)(flowCoef1Int>>8));
    flowCoef1ByteArray.append((quint8)flowCoef1Int);

    paramsMap["flowCoef1ByteArray"] = flowCoef1ByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_coef2_textChanged(const QString &arg1)
{
    flowCoef2ByteArray.clear();
    float flowCoef2Float = arg1.toFloat();
    quint32 flowCoef2Int = (quint32)(flowCoef2Float*1000000);

    flowCoef2ByteArray.append((quint8)(flowCoef2Int>>24));
    flowCoef2ByteArray.append((quint8)(flowCoef2Int>>16));
    flowCoef2ByteArray.append((quint8)(flowCoef2Int>>8));
    flowCoef2ByteArray.append((quint8)flowCoef2Int);

    qDebug()<<"flowCoef2ByteArray.toHex() "<<flowCoef2ByteArray.toHex();

    paramsMap["flowCoef2ByteArray"] = flowCoef2ByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_coef3_textChanged(const QString &arg1)
{
    flowCoef3ByteArray.clear();
    float flowCoef3Float = arg1.toFloat();
    quint32 flowCoef3Int = (quint32)(flowCoef3Float*1000000);

    flowCoef3ByteArray.append((quint8)(flowCoef3Int>>24));
    flowCoef3ByteArray.append((quint8)(flowCoef3Int>>16));
    flowCoef3ByteArray.append((quint8)(flowCoef3Int>>8));
    flowCoef3ByteArray.append((quint8)flowCoef3Int);

    qDebug()<<"flowCoef3ByteArray.toHex() "<<flowCoef3ByteArray.toHex();

    paramsMap["flowCoef3ByteArray"] = flowCoef3ByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_coef4_textChanged(const QString &arg1)
{
    flowCoef4ByteArray.clear();
    float flowCoef4Float = arg1.toFloat();
    quint32 flowCoef4Int = (quint32)(flowCoef4Float*1000000);

    flowCoef4ByteArray.append((quint8)(flowCoef4Int>>24));
    flowCoef4ByteArray.append((quint8)(flowCoef4Int>>16));
    flowCoef4ByteArray.append((quint8)(flowCoef4Int>>8));
    flowCoef4ByteArray.append((quint8)flowCoef4Int);

    qDebug()<<"flowCoef4ByteArray.toHex() "<<flowCoef4ByteArray.toHex();

    paramsMap["flowCoef4ByteArray"] = flowCoef4ByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_coef5_textChanged(const QString &arg1)
{
    flowCoef5ByteArray.clear();
    float flowCoef5Float = arg1.toFloat();
    quint32 flowCoef5Int = (quint32)(flowCoef5Float*1000000);

    flowCoef5ByteArray.append((quint8)(flowCoef5Int>>24));
    flowCoef5ByteArray.append((quint8)(flowCoef5Int>>16));
    flowCoef5ByteArray.append((quint8)(flowCoef5Int>>8));
    flowCoef5ByteArray.append((quint8)flowCoef5Int);

    qDebug()<<"flowCoef5ByteArray.toHex() "<<flowCoef5ByteArray.toHex();

    paramsMap["flowCoef5ByteArray"] = flowCoef5ByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_FS_DiffThrs_textChanged(const QString &arg1)
{
    FS_DiffThrsByteArray.clear();
    bool ok;
    quint16 FS_DiffThrsInt = arg1.toInt(&ok, 10);
    FS_DiffThrsByteArray.append((quint8)(FS_DiffThrsInt>>8));
    FS_DiffThrsByteArray.append((quint8)FS_DiffThrsInt);

    qDebug()<<"FS_DiffThrsByteArray.toHex() "<<FS_DiffThrsByteArray.toHex();

    paramsMap["FS_DiffThrsByteArray"] = FS_DiffThrsByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_dateTimeEdit_realClock_dateTimeChanged(const QDateTime &dateTime)
{
    hourByteArray.clear();
    minuteByteArray.clear();
    secondByteArray.clear();
    dayByteArray.clear();
    monthByteArray.clear();
    yearByteArray.clear();
    DOWByteArray.clear();
    ASW_flagByteArray.clear();

    QDate date = dateTime.date();
    QTime time = dateTime.time();

    hourByteArray.append((quint8)time.hour());
    minuteByteArray.append((quint8)time.minute());
    secondByteArray.append((quint8)time.second());
    dayByteArray.append((quint8)date.day());
    monthByteArray.append((quint8)date.month());
    int year = date.year() - 2000;
    yearByteArray.append((quint8)year);
    DOWByteArray.append((quint8)date.dayOfWeek());

    qDebug()<<"dateTime.toString() "<<dateTime.toString();

    paramsMap["hourByteArray"] = hourByteArray;
    paramsMap["minuteByteArray"] = minuteByteArray;
    paramsMap["secondByteArray"] = secondByteArray;
    paramsMap["dayByteArray"] = dayByteArray;
    paramsMap["monthByteArray"] = monthByteArray;
    paramsMap["yearByteArray"] = yearByteArray;
    paramsMap["DOWByteArray"] = DOWByteArray;

    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_comboBox_ASW_flag_currentIndexChanged(const QString &arg1)
{
    ASW_flagByteArray.clear();
    bool ok;
    ASW_flagByteArray.append((quint8)(arg1.toInt(&ok, 10)));

    qDebug()<<"ASW_flagByteArray.toHex() "<<ASW_flagByteArray.toHex();

    paramsMap["ASW_flagByteArray"] = ASW_flagByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_Shift_T1_textChanged(const QString &arg1)
{
    shift_T1ByteArray.clear();
    float shift_T1Float = arg1.toFloat();
    quint32 shift_T1Int = (quint32)(shift_T1Float*1000000);

    shift_T1ByteArray.append((quint8)(shift_T1Int>>8));
    shift_T1ByteArray.append((quint8)shift_T1Int);

    qDebug()<<"shift_T1ByteArray.toHex() "<<shift_T1ByteArray.toHex();

    paramsMap["shift_T1ByteArray"] = shift_T1ByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_Shift_T2_textChanged(const QString &arg1)
{
    shift_T2ByteArray.clear();
    float shift_T2Float = arg1.toFloat();
    quint32 shift_T2Int = (quint32)(shift_T2Float*1000000);

    shift_T2ByteArray.append((quint8)(shift_T2Int>>8));
    shift_T2ByteArray.append((quint8)shift_T2Int);

    qDebug()<<"shift_T2ByteArray.toHex() "<<shift_T2ByteArray.toHex();

    paramsMap["shift_T2ByteArray"] = shift_T2ByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_Scale_T1_textChanged(const QString &arg1)
{
    scale_T1ByteArray.clear();
    float scale_T1Float = arg1.toFloat();
    quint32 scale_T1Int = (quint32)(scale_T1Float*1000000);

    scale_T1ByteArray.append((quint8)(scale_T1Int>>24));
    scale_T1ByteArray.append((quint8)(scale_T1Int>>16));
    scale_T1ByteArray.append((quint8)(scale_T1Int>>8));
    scale_T1ByteArray.append((quint8)scale_T1Int);

    qDebug()<<"scale_T1ByteArray.toHex() "<<scale_T1ByteArray.toHex();

    paramsMap["scale_T1ByteArray"] = scale_T1ByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_Scale_T2_textChanged(const QString &arg1)
{
    scale_T2ByteArray.clear();
    float scale_T2Float = arg1.toFloat();
    quint32 scale_T2Int = (quint32)(scale_T2Float*1000000);

    scale_T2ByteArray.append((quint8)(scale_T2Int>>24));
    scale_T2ByteArray.append((quint8)(scale_T2Int>>16));
    scale_T2ByteArray.append((quint8)(scale_T2Int>>8));
    scale_T2ByteArray.append((quint8)scale_T2Int);

    qDebug()<<"scale_T2ByteArray.toHex() "<<scale_T2ByteArray.toHex();

    paramsMap["scale_T2ByteArray"] = scale_T2ByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_spinBox_Addr_valueChanged(int arg1)
{
    addrByteArray.clear();
    bool ok;
    addrByteArray.append((quint8)arg1);

    qDebug()<<"addrByteArray.toHex() "<<addrByteArray.toHex();

    paramsMap["addrByteArray"] = addrByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_comboBox_BaudRate_currentIndexChanged(const QString &arg1)
{
    baudRateByteArray.clear();
    bool ok;
    baudRateByteArray.append((quint8)ui->comboBox_BaudRate->currentData().toInt());
//    baudRateByteArray.append((quint8)(arg1.toInt(&ok, 16)));

    qDebug()<<"baudRateByteArray.toHex() "<<baudRateByteArray.toHex();

    paramsMap["baudRateByteArray"] = baudRateByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_HMP_Trans_textChanged(const QString &arg1)
{
    HMP_TransByteArray.clear();
    bool ok;
    quint16 argInt = arg1.toInt(&ok, 16);
    HMP_TransByteArray.append((quint8)(argInt>>8));
    HMP_TransByteArray.append((quint8)argInt);

    qDebug()<<"HMP_TransByteArray.toHex() "<<HMP_TransByteArray.toHex();

    paramsMap["HMP_TransByteArray"] = HMP_TransByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_EPP_Trans_textChanged(const QString &arg1)
{
    EPP_TransByteArray.clear();
    bool ok;
    quint16 argInt = arg1.toInt(&ok, 16);
    EPP_TransByteArray.append((quint8)(argInt>>8));
    EPP_TransByteArray.append((quint8)argInt);

    qDebug()<<"EPP_TransByteArray.toHex() "<<EPP_TransByteArray.toHex();

    paramsMap["EPP_TransByteArray"] = EPP_TransByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_spinBox_P1PrimAddr_valueChanged(int arg1)
{
    P1PrimAddrByteArray.clear();
    P1PrimAddrByteArray.append((quint8)arg1);

    qDebug()<<"P1PrimAddrByteArray.toHex()"<<P1PrimAddrByteArray.toHex();

    paramsMap["P1PrimAddrByteArray"] = P1PrimAddrByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_spinBox_P2PrimAddr_valueChanged(int arg1)
{
    P2PrimAddrByteArray.clear();
    P2PrimAddrByteArray.append((quint8)arg1);

    qDebug()<<"P2PrimAddrByteArray.toHex()"<<P2PrimAddrByteArray.toHex();

    paramsMap["P2PrimAddrByteArray"] = P2PrimAddrByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_ProtType_textChanged(const QString &arg1)
{
    ProtTypeByteArray.clear();
    bool ok;
    ProtTypeByteArray.append((quint8)(arg1.toInt(&ok, 16)));

    qDebug()<<"ProtTypeByteArray.toHex()"<<ProtTypeByteArray.toHex();

    paramsMap["ProtTypeByteArray"] = ProtTypeByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_Serial_P1_textChanged(const QString &arg1)
{
    QString argstring = arg1;

    if(argstring.length() < 8) {
        int nullNumber = 8 - argstring.length();
        for(int i=0; i<nullNumber; i++) {
            argstring.insert(0, '0');
        }
    }

  //    ui->textBrowser->append(argstring);

    Serial_P1ByteArray.clear();
    for(int i=0; i<argstring.size(); i=i+2) {
        QString str = argstring.at(i);
  //       qDebug()<<"arg1[i]"<<str;
        QString str1 = argstring.at(i+1);
        quint8 byte = static_cast<quint8>(str.toInt());
        byte = (byte<<4)|(static_cast<quint8>(str1.toInt()));
        qDebug()<<"byte"<<byte;
  //        str = str + arg1[i+1];
  //        qDebug()<<"arg1[i] + arg1[i+1]"<<str;
        Serial_P1ByteArray.append(byte);
    }
  //   QByteArray deviceNumberFromLineEdit = arg1.toLocal8Bit();
  //     deviceNumberFromLineEdit = arg1.toInt();
   qDebug()<<"Serial_P1ByteArray.toHex()"<<Serial_P1ByteArray.toHex();

   paramsMap["Serial_P1ByteArray"] = Serial_P1ByteArray;
   emit sendParamsMap(paramsMap);

//    Serial_P1ByteArray.clear();
//    bool ok;
//    quint32 Serial_P1Int = arg1.toInt(&ok, 16);
//    Serial_P1ByteArray.append((quint8)(Serial_P1Int>>24));
//    Serial_P1ByteArray.append((quint8)(Serial_P1Int>>16));
//    Serial_P1ByteArray.append((quint8)(Serial_P1Int>>8));
//    Serial_P1ByteArray.append((quint8)Serial_P1Int);

//    qDebug()<<"Serial_P1ByteArray.toHex() "<<Serial_P1ByteArray.toHex();

//    paramsMap["Serial_P1ByteArray"] = Serial_P1ByteArray;
//    emit sendParamsMap(paramsMap);

}

void FormParamsEdit::on_lineEdit_ManCode_P1_textChanged(const QString &arg1)
{
    ManCode_P1ByteArray.clear();

    QString str = arg1.toUpper();

 //  QByteArray manufacturerFromLineEdit = arg1.toLocal8Bit();
   QByteArray manufacturerFromLineEdit = str.toLocal8Bit();
//   manufacturerFromLineEdit = manufacturerFromLineEdit.toInt();
   quint16 manufacturerSum = (static_cast<quint8>(manufacturerFromLineEdit[0])-64)*32*32 +
                              (static_cast<quint8>(manufacturerFromLineEdit[1])-64)*32 +
                               (static_cast<quint8>(manufacturerFromLineEdit[2])-64);
   quint8 byte = (quint8)(manufacturerSum>>8);
   ManCode_P1ByteArray.append(byte);
   byte = (quint8)manufacturerSum;
   ManCode_P1ByteArray.append(byte);
//    bool ok;
//    quint16 ManCode_P1Int = arg1.toInt(&ok, 16);
//    ManCode_P1ByteArray.append((quint8)(ManCode_P1Int>>8));
//    ManCode_P1ByteArray.append((quint8)ManCode_P1Int);



    qDebug()<<"ManCode_P1ByteArray.toHex() "<<ManCode_P1ByteArray.toHex();

    paramsMap["ManCode_P1ByteArray"] = ManCode_P1ByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_DevType_P1_textChanged(const QString &arg1)
{
    DevType_P1ByteArray.clear();
    bool ok;
    quint8 DevType_P1Int = arg1.toInt(&ok, 16);
    DevType_P1ByteArray.append((quint8)DevType_P1Int);

    qDebug()<<"DevType_P1ByteArray.toHex() "<<DevType_P1ByteArray.toHex();

    paramsMap["DevType_P1ByteArray"] = DevType_P1ByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_Version_P1_textChanged(const QString &arg1)
{
    Version_P1ByteArray.clear();
    bool ok;
    quint8 Version_P1Int = arg1.toInt(&ok, 10);
    Version_P1ByteArray.append((quint8)Version_P1Int);

    qDebug()<<"Version_P1ByteArray.toHex() "<<Version_P1ByteArray.toHex();

    paramsMap["Version_P1ByteArray"] = Version_P1ByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_Initial_value_P1_textChanged(const QString &arg1)
{
    Initial_value_P1ByteArray.clear();
    bool ok;
    quint32 Initial_value_P1Int = arg1.toInt(&ok, 10);
    Initial_value_P1ByteArray.append((quint8)(Initial_value_P1Int>>24));
    Initial_value_P1ByteArray.append((quint8)(Initial_value_P1Int>>16));
    Initial_value_P1ByteArray.append((quint8)(Initial_value_P1Int>>8));
    Initial_value_P1ByteArray.append((quint8)Initial_value_P1Int);

    qDebug()<<"Initial_value_P1ByteArray.toHex() "<<Initial_value_P1ByteArray.toHex();

    paramsMap["Initial_value_P1ByteArray"] = Initial_value_P1ByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_Service_P1_textChanged(const QString &arg1)
{
    Service_P1ByteArray.clear();
    bool ok;
    quint32 Service_P1Int = arg1.toInt(&ok, 16);
    Service_P1ByteArray.append((quint8)(Service_P1Int>>24));
    Service_P1ByteArray.append((quint8)(Service_P1Int>>16));
    Service_P1ByteArray.append((quint8)(Service_P1Int>>8));
    Service_P1ByteArray.append((quint8)Service_P1Int);

    qDebug()<<"Service_P1ByteArray.toHex() "<<Service_P1ByteArray.toHex();

    paramsMap["Service_P1ByteArray"] = Service_P1ByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_Serial_P2_textChanged(const QString &arg1)
{
    QString argstring = arg1;

    if(argstring.length() < 8) {
        int nullNumber = 8 - argstring.length();
        for(int i=0; i<nullNumber; i++) {
            argstring.insert(0, '0');
        }
    }

  //    ui->textBrowser->append(argstring);

    Serial_P2ByteArray.clear();
    for(int i=0; i<argstring.size(); i=i+2) {
        QString str = argstring.at(i);
  //       qDebug()<<"arg1[i]"<<str;
        QString str1 = argstring.at(i+1);
        quint8 byte = static_cast<quint8>(str.toInt());
        byte = (byte<<4)|(static_cast<quint8>(str1.toInt()));
        qDebug()<<"byte"<<byte;
  //        str = str + arg1[i+1];
  //        qDebug()<<"arg1[i] + arg1[i+1]"<<str;
        Serial_P2ByteArray.append(byte);
    }
  //   QByteArray deviceNumberFromLineEdit = arg1.toLocal8Bit();
  //     deviceNumberFromLineEdit = arg1.toInt();
   qDebug()<<"Serial_P2ByteArray.toHex()"<<Serial_P2ByteArray.toHex();

   paramsMap["Serial_P2ByteArray"] = Serial_P2ByteArray;
   emit sendParamsMap(paramsMap);

//    Serial_P2ByteArray.clear();
//    bool ok;
//    quint32 Serial_P2Int = arg1.toInt(&ok, 16);
//    Serial_P2ByteArray.append((quint8)(Serial_P2Int>>24));
//    Serial_P2ByteArray.append((quint8)(Serial_P2Int>>16));
//    Serial_P2ByteArray.append((quint8)(Serial_P2Int>>8));
//    Serial_P2ByteArray.append((quint8)Serial_P2Int);

//    qDebug()<<"Serial_P2ByteArray.toHex() "<<Serial_P2ByteArray.toHex();

//    paramsMap["Serial_P2ByteArray"] = Serial_P2ByteArray;
//    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_ManCode_P2_textChanged(const QString &arg1)
{
    ManCode_P2ByteArray.clear();

//    bool ok;
//    quint16 ManCode_P2Int = arg1.toInt(&ok, 16);
//    ManCode_P2ByteArray.append((quint8)(ManCode_P2Int>>8));
//    ManCode_P2ByteArray.append((quint8)ManCode_P2Int);

    QString str = arg1.toUpper();

 //  QByteArray manufacturerFromLineEdit = arg1.toLocal8Bit();
   QByteArray manufacturerFromLineEdit = str.toLocal8Bit();
//   manufacturerFromLineEdit = manufacturerFromLineEdit.toInt();
   quint16 manufacturerSum = (static_cast<quint8>(manufacturerFromLineEdit[0])-64)*32*32 +
                              (static_cast<quint8>(manufacturerFromLineEdit[1])-64)*32 +
                               (static_cast<quint8>(manufacturerFromLineEdit[2])-64);
   quint8 byte = (quint8)(manufacturerSum>>8);
   ManCode_P2ByteArray.append(byte);
   byte = (quint8)manufacturerSum;
   ManCode_P2ByteArray.append(byte);

    qDebug()<<"ManCode_P2ByteArray.toHex() "<<ManCode_P2ByteArray.toHex();

    paramsMap["ManCode_P2ByteArray"] = ManCode_P2ByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_DevType_P2_textChanged(const QString &arg1)
{
    DevType_P2ByteArray.clear();
    bool ok;
    quint8 DevType_P2Int = arg1.toInt(&ok, 16);
    DevType_P2ByteArray.append((quint8)DevType_P2Int);

    qDebug()<<"DevType_P2ByteArray.toHex() "<<DevType_P2ByteArray.toHex();

    paramsMap["DevType_P2ByteArray"] = DevType_P2ByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_Version_P2_textChanged(const QString &arg1)
{
    Version_P2ByteArray.clear();
    bool ok;
    quint8 Version_P2Int = arg1.toInt(&ok, 10);
    Version_P2ByteArray.append((quint8)Version_P2Int);

    qDebug()<<"Version_P2ByteArray.toHex() "<<Version_P2ByteArray.toHex();

    paramsMap["Version_P2ByteArray"] = Version_P2ByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_Initial_value_P2_textChanged(const QString &arg1)
{
    Initial_value_P2ByteArray.clear();
    bool ok;
    quint32 Initial_value_P2Int = arg1.toInt(&ok, 10);
    Initial_value_P2ByteArray.append((quint8)(Initial_value_P2Int>>24));
    Initial_value_P2ByteArray.append((quint8)(Initial_value_P2Int>>16));
    Initial_value_P2ByteArray.append((quint8)(Initial_value_P2Int>>8));
    Initial_value_P2ByteArray.append((quint8)Initial_value_P2Int);

    qDebug()<<"Initial_value_P2ByteArray.toHex() "<<Initial_value_P2ByteArray.toHex();

    paramsMap["Initial_value_P2ByteArray"] = Initial_value_P2ByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_Service_P2_textChanged(const QString &arg1)
{
    Service_P2ByteArray.clear();
    bool ok;
    quint32 Service_P2Int = arg1.toInt(&ok, 16);
    Service_P2ByteArray.append((quint8)(Service_P2Int>>24));
    Service_P2ByteArray.append((quint8)(Service_P2Int>>16));
    Service_P2ByteArray.append((quint8)(Service_P2Int>>8));
    Service_P2ByteArray.append((quint8)Service_P2Int);

    qDebug()<<"Service_P2ByteArray.toHex() "<<Service_P2ByteArray.toHex();

    paramsMap["Service_P2ByteArray"] = Service_P2ByteArray;
    emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_toolButton_saveTemplate_clicked()
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    QString fileName = fileDialog.getSaveFileName(this, tr("Save File"),
                                                  QDir::currentPath(),
                                                  tr("Текст (*.prm)"));

//    QString channel = ui->comboBox_channelNumberWmbus->currentText();
//    QString power = ui->comboBox_power->currentText();
//    QString timeOfWaiting = QString::number(ui->spinBox_timeOfWaitingReceiver->value(), 10);
//    QString minTimeout = QString::number(ui->spinBox_minTimeoutRadio->value(), 10);
//    QString maxTimeout = QString::number(ui->spinBox_maxTimeoutRadio->value(), 10);
//    QString defaultTimeout = QString::number(ui->spinBox_timeoutRadioDefault->value(), 10);
//    QString counterNumber = ui->lineEdit_counterNumber->text();
//    QString coef = ui->lineEdit_coefScoreDecimal->text();
//    QString startValue = ui->lineEdit_startValue->text();

    QFile saveTemplate(fileName);
    if(fileName.isEmpty()) {
        return;
    }
    //открываем файл для очистки
    if(!saveTemplate.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        QMessageBox box;
        box.setText("Не удалось открыть файл");
        box.exec();
        qDebug()<<"не удалось открыть файл";
        return;
    }
    saveTemplate.close();

    //открываем файл для записи
    if(!saveTemplate.open(QIODevice::WriteOnly | QIODevice::Text)) {
//        stream << str;
        QMessageBox box;
        box.setText("Не удалось открыть файл");
        box.exec();
        qDebug()<<"не удалось открыть файл";
        return;
    }
    //создаём поток записи в файл
    QFile *filePointer = &saveTemplate;
    QTextStream stream(filePointer);
    //
    //
    stream.setCodec("UTF-8");//("Windows-1251");
    stream<<"serial " + ui->lineEdit_serial->text() + "\n";
    stream<<"serial2 " + ui->lineEdit_serial2->text() + "\n";
    stream<<"serial3 " + ui->lineEdit_serial3->text() + "\n";
    stream<<"serial4 " + ui->lineEdit_serial4->text() + "\n";
    stream<<"ManCode " + ui->comboBox_ManCode->currentText() + "\n";
    stream<<"DevType " + ui->comboBox_DevType->currentText() + "\n";
    stream<<"DN " + ui->comboBox_DN->currentText() + "\n";
    stream<<"PCB_SN " + ui->lineEdit_PCB_SN->text() + "\n";
    stream<<"PCB_SN2 " + ui->lineEdit_PCB_SN2->text() + "\n";
    stream<<"PCB_SN3 " + ui->lineEdit_PCB_SN3->text() + "\n";
    stream<<"PCB_SN4 " + ui->lineEdit_PCB_SN4->text() + "\n";
    //
    stream<<"Flow_min " + ui->lineEdit_Flow_min->text() + "\n";
    stream<<"Flow_max " + ui->lineEdit_Flow_max->text() + "\n";
    stream<<"Tin_min " + QString::number(ui->spinBox_Tin_min->value()) + "\n";
    stream<<"Tin_max " + QString::number(ui->spinBox_Tin_max->value()) + "\n";
    stream<<"Tout_min " + QString::number(ui->spinBox_Tout_min->value()) + "\n";
    stream<<"Tout_max " + QString::number(ui->spinBox_Tout_max->value()) + "\n";
    stream<<"ReportData " + QString::number(ui->spinBox_ReportData->value()) + "\n";
    stream<<"Average_T " + ui->comboBox_Average_T->currentText() + "\n";
    stream<<"PulsesMode1 " + ui->comboBox_PulsesMode1->currentText() + "\n";
    stream<<"PulsesMode2 " + ui->comboBox_PulsesMode2->currentText() + "\n";
    stream<<"PulsesValue1 " + ui->lineEdit_PulsesValue1->text() + "\n";
    stream<<"PulsesValue2 " + ui->lineEdit_PulsesValue2->text() + "\n";
    //
    stream<<"FlowRate " + ui->comboBox_FlowRate->currentText() + "\n";
    stream<<"Pressure " + ui->lineEdit_Pressure->text() + "\n";
    stream<<"RTD_Type " + ui->comboBox_RTD_Type->currentText() + "\n";
    stream<<"R_RTD " + ui->lineEdit_R_RTD->text() + "\n";
    stream<<"RWire1 " + ui->lineEdit_RWire1->text() + "\n";
    stream<<"RWire2 " + ui->lineEdit_RWire2->text() + "\n";
    stream<<"FlowNom " + ui->comboBox_FlowNom->currentText() + "\n";
    //
    stream<<"FlCoeff0 " + ui->lineEdit_coef0->text() + "\n";
    stream<<"FlCoeff1 " + ui->lineEdit_coef1->text() + "\n";
    stream<<"FlCoeff2 " + ui->lineEdit_coef2->text() + "\n";
    stream<<"FlCoeff3 " + ui->lineEdit_coef3->text() + "\n";
    stream<<"FlCoeff4 " + ui->lineEdit_coef4->text() + "\n";
    stream<<"FlCoeff5 " + ui->lineEdit_coef5->text() + "\n";
    //
    stream<<"FS_DiffThrs " + ui->lineEdit_FS_DiffThrs->text() + "\n";
    //
    QDateTime dateTimeRealClock = ui->dateTimeEdit_realClock->dateTime();
    stream<<"DateTime_RealClock " + dateTimeRealClock.toString("dd.MM.yyyy hh:mm:ss") + "\n";
    stream<<"ASW_flag " + ui->comboBox_ASW_flag->currentText() + "\n";
    //
    stream<<"Shift_T1 " + ui->lineEdit_Shift_T1->text() + "\n";
    stream<<"Shift_T2 " + ui->lineEdit_Shift_T2->text() + "\n";
    stream<<"Scale_T1 " + ui->lineEdit_Scale_T1->text() + "\n";
    stream<<"Scale_T2 " + ui->lineEdit_Scale_T2->text() + "\n";
    //
    stream<<"PrimAddr " + QString::number(ui->spinBox_Addr->value()) + "\n";
    stream<<"BaudRate " + ui->comboBox_BaudRate->currentText() + "\n";
    stream<<"HMP_Trans " + ui->lineEdit_HMP_Trans->text() + "\n";
    stream<<"EPP_Trans " + ui->lineEdit_EPP_Trans->text() + "\n";
    stream<<"P1PrimAddr " + QString::number(ui->spinBox_P1PrimAddr->value()) + "\n";
    stream<<"P2PrimAddr " + QString::number(ui->spinBox_P2PrimAddr->value()) + "\n";
    stream<<"ProtType " + ui->lineEdit_ProtType->text() + "\n";
    //
    stream<<"Serial_P1 " + ui->lineEdit_Serial_P1->text() + "\n";
    stream<<"ManCode_P1 " + ui->lineEdit_ManCode_P1->text() + "\n";
    stream<<"DevType_P1 " + ui->lineEdit_DevType_P1->text() + "\n";
    stream<<"Version_P1 " + ui->lineEdit_Version_P1->text() + "\n";
    stream<<"Initial_value_P1 " + ui->lineEdit_Initial_value_P1->text() + "\n";
    stream<<"Service_P1 " + ui->lineEdit_Service_P1->text() + "\n";
    //
    stream<<"Serial_P2 " + ui->lineEdit_Serial_P2->text() + "\n";
    stream<<"ManCode_P2 " + ui->lineEdit_ManCode_P2->text() + "\n";
    stream<<"DevType_P2 " + ui->lineEdit_DevType_P2->text() + "\n";
    stream<<"Version_P2 " + ui->lineEdit_Version_P2->text() + "\n";
    stream<<"Initial_value_P2 " + ui->lineEdit_Initial_value_P2->text() + "\n";
    stream<<"Service_P2 " + ui->lineEdit_Service_P2->text() + "\n";
    //
    //

    saveTemplate.close();
}

void FormParamsEdit::on_toolButton_loadTemplate_clicked()
{
        QMessageBox box;
        QFileDialog fileDialog;
        fileDialog.setFileMode(QFileDialog::Directory);
        QString fileName = fileDialog.getOpenFileName(this, tr("Save File"),
                                                      QDir::currentPath(),
                                                      tr("Текст (*.txt)"));
        qDebug()<<"fileName"<<fileName;

        QFile loadTemplate(fileName);
        if(!loadTemplate.open(QIODevice::ReadOnly | QIODevice::Text)) {
            box.setText("Не удалось открыть файл");
            box.exec();
            qDebug()<<"не удалось открыть файл";
            return;
        }
        //прочитываем файл в строку далее в лист
        QString fileToString = loadTemplate.readAll();
        qDebug()<<"fileToString"<<fileToString;
        QStringList fileToStringList;
        QString sym = "";
        for(int i=0; i<fileToString.size(); i++) {
            sym = sym + fileToString[i];
            if(fileToString[i+1] == "\n") {
     //           sym = sym + "\n";
                fileToStringList<<sym;
                sym = "";
     //           i=i+1;
            }
        }
        qDebug()<<"fileToStringList"<<fileToStringList;

        loadTemplate.close();

        QString serialString = fileToStringList[0];
        serialString.remove("serial ");
        ui->lineEdit_serial->setText(serialString);
        //
        QString ManCodeString = fileToStringList[1];
        ManCodeString.remove("\nManCode ");
        if(manCodeList.contains(ManCodeString)) {
           ui->comboBox_ManCode->setCurrentText(ManCodeString);
        }
        else {
            QMessageBox::information(this, "", tr("Неверное значение ManCode: ") + ManCodeString);
        }
        //
        QString DevTypeString = fileToStringList[2];
        DevTypeString.remove("\nDevType ");
        if(devTypeList.contains(DevTypeString)) {
           ui->comboBox_DevType->setCurrentText(DevTypeString);
        }
        else {
            QMessageBox::information(this, "", tr("Неверное значение DevType: ") + DevTypeString);
        }
        //
        QString DNString = fileToStringList[3];
        DNString.remove("\nDN ");
        if(DNList.contains(DNString)) {
           ui->comboBox_DN->setCurrentText(DNString);
        }
        else {
            QMessageBox::information(this, "", tr("Неверное значение DN: ") + DNString);
        }
        //
        QString PCB_SNString = fileToStringList[4];
        PCB_SNString.remove("\nPCB_SN ");
        ui->lineEdit_PCB_SN->setText(PCB_SNString);
        //
        QString Flow_minString = fileToStringList[5];
        Flow_minString.remove("\nFlow_min ");
        ui->lineEdit_Flow_min->setText(Flow_minString);
        //
        QString Flow_maxString = fileToStringList[6];
        Flow_maxString.remove("\nFlow_max ");
        ui->lineEdit_Flow_max->setText(Flow_maxString);
        //
        QString Tin_minString = fileToStringList[7];
        Tin_minString.remove("\nTin_min ");
        ui->spinBox_Tin_min->setValue(Tin_minString.toInt());
        //
        QString Tin_maxString = fileToStringList[8];
        Tin_maxString.remove("\nTin_max ");
        ui->spinBox_Tin_max->setValue(Tin_maxString.toInt());
        //
        QString Tout_minString = fileToStringList[9];
        Tout_minString.remove("\nTout_min ");
        ui->spinBox_Tout_min->setValue(Tout_minString.toInt());
        //
        QString Tout_maxString = fileToStringList[10];
        Tout_maxString.remove("\nTout_max ");
        ui->spinBox_Tout_max->setValue(Tout_maxString.toInt());
        //
        QString ReportDataString = fileToStringList[11];
        ReportDataString.remove("\nReportData ");
        ui->spinBox_ReportData->setValue(ReportDataString.toInt());
        //
        QString Average_TString = fileToStringList[12];
        Average_TString.remove("\nAverage_T ");
        if(AverageTList.contains(Average_TString)) {
           ui->comboBox_Average_T->setCurrentText(Average_TString);
        }
        else {
            QMessageBox::information(this, "", tr("Неверное значение Average_TS: ") + Average_TString);
        }
        //
        QString PulsesMode1String = fileToStringList[13];
        PulsesMode1String.remove("\nPulsesMode1 ");
        if(pulsesMode1List.contains(PulsesMode1String)) {
           ui->comboBox_PulsesMode1->setCurrentText(PulsesMode1String);
        }
        else {
            QMessageBox::information(this, "", tr("Неверное значение PulsesMode1: ") + PulsesMode1String);
        }
        //
        QString PulsesMode2String = fileToStringList[14];
        PulsesMode2String.remove("\nPulsesMode2 ");
        if(pulsesMode1List.contains(PulsesMode2String)) {
           ui->comboBox_PulsesMode2->setCurrentText(PulsesMode2String);
        }
        else {
            QMessageBox::information(this, "", tr("Неверное значение PulsesMode2: ") + PulsesMode2String);
        }
        //
        QString PulsesValue1String = fileToStringList[15];
        PulsesValue1String.remove("\nPulsesValue1 ");
        ui->lineEdit_PulsesValue1->setText(PulsesValue1String);
        //
        QString PulsesValue2String = fileToStringList[16];
        PulsesValue2String.remove("\nPulsesValue2 ");
        ui->lineEdit_PulsesValue2->setText(PulsesValue2String);
        //
        QString FlowRateString = fileToStringList[17];
        FlowRateString.remove("\nFlowRate ");
        if(flowRateList.contains(FlowRateString)) {
           ui->comboBox_FlowRate->setCurrentText(FlowRateString);
        }
        else {
            QMessageBox::information(this, "", tr("Неверное значение FlowRate: ") + FlowRateString);
        }
        //
        QString PressureString = fileToStringList[18];
        PressureString.remove("\nPressure ");
        ui->lineEdit_Pressure->setText(PressureString);
        //
        QString RTD_TypeString = fileToStringList[19];
        RTD_TypeString.remove("\nRTD_Type ");
        if(RTD_TypeList.contains(RTD_TypeString)) {
           ui->comboBox_RTD_Type->setCurrentText(RTD_TypeString);
        }
        else {
            QMessageBox::information(this, "", tr("Неверное значение RTD_Type: ") + RTD_TypeString);
        }
        //
        QString R_RTDString = fileToStringList[20];
        R_RTDString.remove("\nR_RTD ");
        ui->lineEdit_R_RTD->setText(R_RTDString);
        //
        QString RWire1String = fileToStringList[21];
        RWire1String.remove("\nRWire1 ");
        ui->lineEdit_RWire1->setText(RWire1String);
        //
        QString RWire2String = fileToStringList[22];
        RWire2String.remove("\nRWire2 ");
        ui->lineEdit_RWire2->setText(RWire2String);
        //
        QString FlowNomString = fileToStringList[23];
        FlowNomString.remove("\nFlowNom ");
        if(flowNomList.contains(FlowNomString)) {
           ui->comboBox_FlowNom->setCurrentText(FlowNomString);
        }
        else {
            QMessageBox::information(this, "", tr("Неверное значение FlowNom: ") + FlowNomString);
        }

        //
        QString FlCoeff0String = fileToStringList[24];
        FlCoeff0String.remove("\nFlCoeff0 ");
        ui->lineEdit_coef0->setText(FlCoeff0String);
        //
        QString FlCoeff1String = fileToStringList[25];
        FlCoeff1String.remove("\nFlCoeff1 ");
        ui->lineEdit_coef1->setText(FlCoeff1String);
        //
        QString FlCoeff2String = fileToStringList[26];
        FlCoeff2String.remove("\nFlCoeff2 ");
        ui->lineEdit_coef2->setText(FlCoeff2String);
        //
        QString FlCoeff3String = fileToStringList[27];
        FlCoeff3String.remove("\nFlCoeff3 ");
        ui->lineEdit_coef3->setText(FlCoeff3String);
        //
        QString FlCoeff4String = fileToStringList[28];
        FlCoeff4String.remove("\nFlCoeff4 ");
        ui->lineEdit_coef4->setText(FlCoeff4String);
        //
        QString FlCoeff5String = fileToStringList[29];
        FlCoeff5String.remove("\nFlCoeff5 ");
        ui->lineEdit_coef5->setText(FlCoeff5String);
        //
        QString FS_DiffThrsString = fileToStringList[30];
        FS_DiffThrsString.remove("\nFS_DiffThrs ");
        ui->lineEdit_FS_DiffThrs->setText(FS_DiffThrsString);
        //
        QString DateTime_RealClockString = fileToStringList[31];
        DateTime_RealClockString.remove("\nDateTime_RealClock ");
        QDateTime dateTimeFromFile = QDateTime::fromString(DateTime_RealClockString, "dd.MM.yyyy hh:mm:ss");
        ui->dateTimeEdit_realClock->setDateTime(dateTimeFromFile);
        //
        QString ASW_flagString = fileToStringList[32];
        ASW_flagString.remove("\nASW_flag ");
        if(ASW_flagList.contains(ASW_flagString)) {
           ui->comboBox_ASW_flag->setCurrentText(ASW_flagString);
        }
        else {
            QMessageBox::information(this, "", tr("Неверное значение ASW_flag: ") + ASW_flagString);
        }
        //
        QString Shift_T1String = fileToStringList[33];
        Shift_T1String.remove("\nShift_T1 ");
        ui->lineEdit_Shift_T1->setText(Shift_T1String);
        //
        QString Shift_T2String = fileToStringList[34];
        Shift_T2String.remove("\nShift_T2 ");
        ui->lineEdit_Shift_T2->setText(Shift_T2String);
        //
        QString Scale_T1String = fileToStringList[35];
        Scale_T1String.remove("\nScale_T1 ");
        ui->lineEdit_Scale_T1->setText(Scale_T1String);
        //
        QString Scale_T2String = fileToStringList[36];
        Scale_T2String.remove("\nScale_T2 ");
        ui->lineEdit_Scale_T2->setText(Scale_T2String);
        //
        QString PrimAddrString = fileToStringList[37];
        PrimAddrString.remove("\nPrimAddr ");
        ui->spinBox_Addr->setValue(PrimAddrString.toInt());
        //
        QString BaudRateString = fileToStringList[38];
        BaudRateString.remove("\nBaudRate ");
        if(BaudRateList.contains(BaudRateString)) {
           ui->comboBox_BaudRate->setCurrentText(BaudRateString);
        }
        else {
            QMessageBox::information(this, "", tr("Неверное значение BaudRate: ") + BaudRateString);
        }
        //
        QString HMP_TransString = fileToStringList[39];
        HMP_TransString.remove("\nHMP_Trans ");
        ui->lineEdit_HMP_Trans->setText(HMP_TransString);
        //
        QString EPP_TransString = fileToStringList[40];
        EPP_TransString.remove("\nEPP_Trans ");
        ui->lineEdit_EPP_Trans->setText(EPP_TransString);
        //
        QString P1PrimAddrString = fileToStringList[41];
        P1PrimAddrString.remove("\nP1PrimAddr ");
        ui->spinBox_P1PrimAddr->setValue(P1PrimAddrString.toInt());
        //
        QString P2PrimAddrString = fileToStringList[42];
        P2PrimAddrString.remove("\nP2PrimAddr ");
        ui->spinBox_P2PrimAddr->setValue(P2PrimAddrString.toInt());
        //
        QString ProtTypeString = fileToStringList[43];
        ProtTypeString.remove("\nProtType ");
        ui->lineEdit_ProtType->setText(ProtTypeString);
        //
        QString Serial_P1String = fileToStringList[44];
        Serial_P1String.remove("\nSerial_P1 ");
        ui->lineEdit_Serial_P1->setText(Serial_P1String);
        //
        QString ManCode_P1String = fileToStringList[45];
        ManCode_P1String.remove("\nManCode_P1 ");
        ui->lineEdit_ManCode_P1->setText(ManCode_P1String);
        //
        QString DevType_P1String = fileToStringList[46];
        DevType_P1String.remove("\nDevType_P1 ");
        ui->lineEdit_DevType_P1->setText(DevType_P1String);
        //
        QString Version_P1String = fileToStringList[47];
        Version_P1String.remove("\nVersion_P1 ");
        ui->lineEdit_Version_P1->setText(Version_P1String);
        //
        QString Initial_value_P1String = fileToStringList[48];
        Initial_value_P1String.remove("\nInitial_value_P1 ");
        ui->lineEdit_Initial_value_P1->setText(Initial_value_P1String);
        //
        QString Service_P1String = fileToStringList[49];
        Service_P1String.remove("\nService_P1 ");
        ui->lineEdit_Service_P1->setText(Service_P1String);
        //
        QString Serial_P2String = fileToStringList[50];
        Serial_P2String.remove("\nSerial_P2 ");
        ui->lineEdit_Serial_P2->setText(Serial_P2String);
        //
        QString ManCode_P2String = fileToStringList[51];
        ManCode_P2String.remove("\nManCode_P2 ");
        ui->lineEdit_ManCode_P2->setText(ManCode_P2String);
        //
        QString DevType_P2String = fileToStringList[52];
        DevType_P2String.remove("\nDevType_P2 ");
        ui->lineEdit_DevType_P2->setText(DevType_P2String);
        //
        QString Version_P2String = fileToStringList[53];
        Version_P2String.remove("\nVersion_P2 ");
        ui->lineEdit_Version_P2->setText(Version_P2String);
        //
        QString Initial_value_P2String = fileToStringList[54];
        Initial_value_P2String.remove("\nInitial_value_P2 ");
        ui->lineEdit_Initial_value_P2->setText(Initial_value_P2String);
        //
        QString Service_P2String = fileToStringList[55];
        Service_P2String.remove("\nService_P2 ");
        ui->lineEdit_Service_P2->setText(Service_P2String);
}

void FormParamsEdit::on_toolButton_readParams_clicked()
{
//    QObject::blockSignals(true);

//    ui->lineEdit_AddrHex->clear();
//    ui->lineEdit_ASW_flagHex->clear();
//    ui->lineEdit_Average_THex->clear();
//    ui->lineEdit_BaudRateHex->clear();
//    ui->lineEdit_coef0->clear();
//    ui->lineEdit_coef0Hex->clear();
//    ui->lineEdit_coef1->clear();
//    ui->lineEdit_coef1Hex->clear();
//    ui->lineEdit_coef2->clear();
//    ui->lineEdit_coef2Hex->clear();
//    ui->lineEdit_coef3->clear();
//    ui->lineEdit_coef3Hex->clear();
//    ui->lineEdit_coef4->clear();
//    ui->lineEdit_coef4Hex->clear();
//    ui->lineEdit_coef5->clear();
//    ui->lineEdit_coef5Hex->clear();
//    ui->lineEdit_DevTypeHex->clear();
//    ui->lineEdit_DevType_P1->clear();
//    ui->lineEdit_DevType_P1Hex->clear();
//    ui->lineEdit_DevType_P2->clear();
//    ui->lineEdit_DevType_P2Hex->clear();
//    ui->lineEdit_Version_P2Hex->clear();
//    ui->lineEdit_Version_P2->clear();
//    ui->lineEdit_Version_P1Hex->clear();
//    ui->lineEdit_Version_P1->clear();
//    ui->lineEdit_Tout_minHex->clear();
//    ui->lineEdit_Tout_maxHex->clear();
//    ui->lineEdit_Tin_minHex->clear();
//    ui->lineEdit_Tin_maxHex->clear();
//    ui->lineEdit_Shift_T2Hex->clear();
//    ui->lineEdit_Shift_T2->clear();
//    ui->lineEdit_Shift_T1Hex->clear();
//    ui->lineEdit_Shift_T1->clear();
//    ui->lineEdit_Service_P2Hex->clear();
//    ui->lineEdit_Service_P2->clear();
//    ui->lineEdit_Service_P1Hex->clear();
//    ui->lineEdit_Service_P1->clear();
//    ui->lineEdit_serialHex->clear();
//    ui->lineEdit_Serial_P2Hex->clear();
//    ui->lineEdit_Serial_P2->clear();
//    ui->lineEdit_Serial_P1Hex->clear();
//    ui->lineEdit_Serial_P1->clear();
//    ui->lineEdit_serial->clear();
//    ui->lineEdit_Scale_T2Hex->clear();
//    ui->lineEdit_Scale_T2->clear();
//    ui->lineEdit_Scale_T1Hex->clear();
//    ui->lineEdit_Scale_T1->clear();
//    ui->lineEdit_R_RTDHex->clear();
//    ui->lineEdit_R_RTD->clear();
//    ui->lineEdit_RWire2Hex->clear();
//    ui->lineEdit_RWire2->clear();
//    ui->lineEdit_RWire1Hex->clear();
//    ui->lineEdit_RWire1->clear();
//    ui->lineEdit_RTD_TypeHex->clear();
//    ui->lineEdit_ReportDataHex->clear();
//    ui->lineEdit_PulsesValue2Hex->clear();
//    ui->lineEdit_PulsesValue2->clear();
//    ui->lineEdit_PulsesValue1Hex->clear();
//    ui->lineEdit_PulsesValue1->clear();
//    ui->lineEdit_PulsesMode2Hex->clear();
//    ui->lineEdit_PulsesMode1Hex->clear();
//    ui->lineEdit_ProtTypeHex->clear();
//    ui->lineEdit_ProtType->clear();
//    ui->lineEdit_PressureHex->clear();
//    ui->lineEdit_Pressure->clear();
//    ui->lineEdit_PCB_SNHex->clear();
//    ui->lineEdit_PCB_SN->clear();
//    ui->lineEdit_P2PrimAddrHex->clear();
//    ui->lineEdit_P1PrimAddrHex->clear();
//    ui->lineEdit_DNHex->clear();
//    ui->lineEdit_EPP_Trans->clear();
//    ui->lineEdit_EPP_TransHex->clear();
//    ui->lineEdit_FlowNomHex->clear();
//    ui->lineEdit_FlowRateHex->clear();
//    ui->lineEdit_Flow_max->clear();
//    ui->lineEdit_Flow_maxHex->clear();
//    ui->lineEdit_Flow_min->clear();
//    ui->lineEdit_Flow_minHex->clear();
//    ui->lineEdit_FS_DiffThrs->clear();
//    ui->lineEdit_FS_DiffThrsHex->clear();
//    ui->lineEdit_HMP_Trans->clear();
//    ui->lineEdit_HMP_TransHex->clear();
//    ui->lineEdit_Initial_value_P1->clear();
//    ui->lineEdit_Initial_value_P1Hex->clear();
//    ui->lineEdit_Initial_value_P2->clear();
//    ui->lineEdit_Initial_value_P2Hex->clear();
//    ui->lineEdit_ManCodeHex->clear();
//    ui->lineEdit_ManCode_P1->clear();
//    ui->lineEdit_ManCode_P1Hex->clear();
//    ui->lineEdit_ManCode_P2->clear();
//    ui->lineEdit_ManCode_P2Hex->clear();

//    QObject::blockSignals(false);


    emit signalReadParams();
}

void FormParamsEdit::on_toolButton_writeParams_clicked()
{
    emit signalWriteParams();
}

void FormParamsEdit::on_lineEdit_coef0_textEdited(const QString &arg1)
{

}

void FormParamsEdit::slotGetParamsStringListFromFileFromMainWindow(QStringList fileToStringList)
{

    QString serialString = fileToStringList[0];
    serialString.remove("serial ");
    ui->lineEdit_serial->setText(serialString);

    QString serialString2 = fileToStringList[1];
    serialString2.remove("\nserial2 ");
    ui->lineEdit_serial2->setText(serialString2);

    QString serialString3 = fileToStringList[2];
    serialString3.remove("\nserial3 ");
    ui->lineEdit_serial3->setText(serialString3);

    QString serialString4 = fileToStringList[3];
    serialString4.remove("\nserial4 ");
    ui->lineEdit_serial4->setText(serialString4);

    //
    QString ManCodeString = fileToStringList[4];
    ManCodeString.remove("\nManCode ");
    if(manCodeList.contains(ManCodeString)) {
       ui->comboBox_ManCode->setCurrentText(ManCodeString);
    }
    else {
        QMessageBox::information(this, "", tr("Неверное значение ManCode: ") + ManCodeString);
    }
    //
    QString DevTypeString = fileToStringList[5];
    DevTypeString.remove("\nDevType ");
    if(devTypeList.contains(DevTypeString)) {
       ui->comboBox_DevType->setCurrentText(DevTypeString);
    }
    else {
        QMessageBox::information(this, "", tr("Неверное значение DevType: ") + DevTypeString);
    }
    //
    QString DNString = fileToStringList[6];
    DNString.remove("\nDN ");
    if(DNList.contains(DNString)) {
       ui->comboBox_DN->setCurrentText(DNString);
    }
    else {
        QMessageBox::information(this, "", tr("Неверное значение DN: ") + DNString);
    }
    //
    QString PCB_SNString = fileToStringList[7];
    PCB_SNString.remove("\nPCB_SN ");
    ui->lineEdit_PCB_SN->setText(PCB_SNString);

    QString PCB_SNString2 = fileToStringList[8];
    PCB_SNString2.remove("\nPCB_SN2 ");
    ui->lineEdit_PCB_SN2->setText(PCB_SNString2);

    QString PCB_SNString3 = fileToStringList[9];
    PCB_SNString3.remove("\nPCB_SN3 ");
    ui->lineEdit_PCB_SN3->setText(PCB_SNString3);

    QString PCB_SNString4 = fileToStringList[10];
    PCB_SNString4.remove("\nPCB_SN4 ");
    ui->lineEdit_PCB_SN4->setText(PCB_SNString4);

    //
    QString Flow_minString = fileToStringList[11];
    Flow_minString.remove("\nFlow_min ");
    ui->lineEdit_Flow_min->setText(Flow_minString);
    //
    QString Flow_maxString = fileToStringList[12];
    Flow_maxString.remove("\nFlow_max ");
    ui->lineEdit_Flow_max->setText(Flow_maxString);
    //
    QString Tin_minString = fileToStringList[13];
    Tin_minString.remove("\nTin_min ");
    ui->spinBox_Tin_min->setValue(Tin_minString.toInt());
    //
    QString Tin_maxString = fileToStringList[14];
    Tin_maxString.remove("\nTin_max ");
    ui->spinBox_Tin_max->setValue(Tin_maxString.toInt());
    //
    QString Tout_minString = fileToStringList[15];
    Tout_minString.remove("\nTout_min ");
    ui->spinBox_Tout_min->setValue(Tout_minString.toInt());
    //
    QString Tout_maxString = fileToStringList[16];
    Tout_maxString.remove("\nTout_max ");
    ui->spinBox_Tout_max->setValue(Tout_maxString.toInt());
    //
    QString ReportDataString = fileToStringList[17];
    ReportDataString.remove("\nReportData ");
    ui->spinBox_ReportData->setValue(ReportDataString.toInt());
    //
    QString Average_TString = fileToStringList[18];
    Average_TString.remove("\nAverage_T ");
    if(AverageTList.contains(Average_TString)) {
       ui->comboBox_Average_T->setCurrentText(Average_TString);
    }
    else {
        QMessageBox::information(this, "", tr("Неверное значение Average_TS: ") + Average_TString);
    }
    //
    QString PulsesMode1String = fileToStringList[19];
    PulsesMode1String.remove("\nPulsesMode1 ");
    if(pulsesMode1List.contains(PulsesMode1String)) {
       ui->comboBox_PulsesMode1->setCurrentText(PulsesMode1String);
    }
    else {
        QMessageBox::information(this, "", tr("Неверное значение PulsesMode1: ") + PulsesMode1String);
    }
    //
    QString PulsesMode2String = fileToStringList[20];
    PulsesMode2String.remove("\nPulsesMode2 ");
    if(pulsesMode1List.contains(PulsesMode2String)) {
       ui->comboBox_PulsesMode2->setCurrentText(PulsesMode2String);
    }
    else {
        QMessageBox::information(this, "", tr("Неверное значение PulsesMode2: ") + PulsesMode2String);
    }
    //
    QString PulsesValue1String = fileToStringList[21];
    PulsesValue1String.remove("\nPulsesValue1 ");
    ui->lineEdit_PulsesValue1->setText(PulsesValue1String);
    //
    QString PulsesValue2String = fileToStringList[22];
    PulsesValue2String.remove("\nPulsesValue2 ");
    ui->lineEdit_PulsesValue2->setText(PulsesValue2String);
    //
    QString FlowRateString = fileToStringList[23];
    FlowRateString.remove("\nFlowRate ");
    if(flowRateList.contains(FlowRateString)) {
       ui->comboBox_FlowRate->setCurrentText(FlowRateString);
    }
    else {
        QMessageBox::information(this, "", tr("Неверное значение FlowRate: ") + FlowRateString);
    }
    //
    QString PressureString = fileToStringList[24];
    PressureString.remove("\nPressure ");
    ui->lineEdit_Pressure->setText(PressureString);
    //
    QString RTD_TypeString = fileToStringList[25];
    RTD_TypeString.remove("\nRTD_Type ");
    if(RTD_TypeList.contains(RTD_TypeString)) {
       ui->comboBox_RTD_Type->setCurrentText(RTD_TypeString);
    }
    else {
        QMessageBox::information(this, "", tr("Неверное значение RTD_Type: ") + RTD_TypeString);
    }
    //
    QString R_RTDString = fileToStringList[26];
    R_RTDString.remove("\nR_RTD ");
    ui->lineEdit_R_RTD->setText(R_RTDString);
    //
    QString RWire1String = fileToStringList[27];
    RWire1String.remove("\nRWire1 ");
    ui->lineEdit_RWire1->setText(RWire1String);
    //
    QString RWire2String = fileToStringList[28];
    RWire2String.remove("\nRWire2 ");
    ui->lineEdit_RWire2->setText(RWire2String);
    //
    QString FlowNomString = fileToStringList[29];
    FlowNomString.remove("\nFlowNom ");
    if(flowNomList.contains(FlowNomString)) {
       ui->comboBox_FlowNom->setCurrentText(FlowNomString);
    }
    else {
        QMessageBox::information(this, "", tr("Неверное значение FlowNom: ") + FlowNomString);
    }

    //
    QString FlCoeff0String = fileToStringList[30];
    FlCoeff0String.remove("\nFlCoeff0 ");
    ui->lineEdit_coef0->setText(FlCoeff0String);
    //
    QString FlCoeff1String = fileToStringList[31];
    FlCoeff1String.remove("\nFlCoeff1 ");
    ui->lineEdit_coef1->setText(FlCoeff1String);
    //
    QString FlCoeff2String = fileToStringList[32];
    FlCoeff2String.remove("\nFlCoeff2 ");
    ui->lineEdit_coef2->setText(FlCoeff2String);
    //
    QString FlCoeff3String = fileToStringList[33];
    FlCoeff3String.remove("\nFlCoeff3 ");
    ui->lineEdit_coef3->setText(FlCoeff3String);
    //
    QString FlCoeff4String = fileToStringList[34];
    FlCoeff4String.remove("\nFlCoeff4 ");
    ui->lineEdit_coef4->setText(FlCoeff4String);
    //
    QString FlCoeff5String = fileToStringList[35];
    FlCoeff5String.remove("\nFlCoeff5 ");
    ui->lineEdit_coef5->setText(FlCoeff5String);
    //
    QString FS_DiffThrsString = fileToStringList[36];
    FS_DiffThrsString.remove("\nFS_DiffThrs ");
    ui->lineEdit_FS_DiffThrs->setText(FS_DiffThrsString);
    //
    QString DateTime_RealClockString = fileToStringList[37];
    DateTime_RealClockString.remove("\nDateTime_RealClock ");
    QDateTime dateTimeFromFile = QDateTime::fromString(DateTime_RealClockString, "dd.MM.yyyy hh:mm:ss");
    ui->dateTimeEdit_realClock->setDateTime(dateTimeFromFile);
    //
    QString ASW_flagString = fileToStringList[38];
    ASW_flagString.remove("\nASW_flag ");
    if(ASW_flagList.contains(ASW_flagString)) {
       ui->comboBox_ASW_flag->setCurrentText(ASW_flagString);
    }
    else {
        QMessageBox::information(this, "", tr("Неверное значение ASW_flag: ") + ASW_flagString);
    }
    //
    QString Shift_T1String = fileToStringList[39];
    Shift_T1String.remove("\nShift_T1 ");
    ui->lineEdit_Shift_T1->setText(Shift_T1String);
    //
    QString Shift_T2String = fileToStringList[40];
    Shift_T2String.remove("\nShift_T2 ");
    ui->lineEdit_Shift_T2->setText(Shift_T2String);
    //
    QString Scale_T1String = fileToStringList[41];
    Scale_T1String.remove("\nScale_T1 ");
    ui->lineEdit_Scale_T1->setText(Scale_T1String);
    //
    QString Scale_T2String = fileToStringList[42];
    Scale_T2String.remove("\nScale_T2 ");
    ui->lineEdit_Scale_T2->setText(Scale_T2String);
    //
    QString PrimAddrString = fileToStringList[43];
    PrimAddrString.remove("\nPrimAddr ");
    ui->spinBox_Addr->setValue(PrimAddrString.toInt());
    //
    QString BaudRateString = fileToStringList[44];
    BaudRateString.remove("\nBaudRate ");
    if(BaudRateList.contains(BaudRateString)) {
       ui->comboBox_BaudRate->setCurrentText(BaudRateString);
    }
    else {
        QMessageBox::information(this, "", tr("Неверное значение BaudRate: ") + BaudRateString);
    }
    //
    QString HMP_TransString = fileToStringList[45];
    HMP_TransString.remove("\nHMP_Trans ");
    ui->lineEdit_HMP_Trans->setText(HMP_TransString);
    //
    QString EPP_TransString = fileToStringList[46];
    EPP_TransString.remove("\nEPP_Trans ");
    ui->lineEdit_EPP_Trans->setText(EPP_TransString);
    //
    QString P1PrimAddrString = fileToStringList[47];
    P1PrimAddrString.remove("\nP1PrimAddr ");
    ui->spinBox_P1PrimAddr->setValue(P1PrimAddrString.toInt());
    //
    QString P2PrimAddrString = fileToStringList[48];
    P2PrimAddrString.remove("\nP2PrimAddr ");
    ui->spinBox_P2PrimAddr->setValue(P2PrimAddrString.toInt());
    //
    QString ProtTypeString = fileToStringList[49];
    ProtTypeString.remove("\nProtType ");
    ui->lineEdit_ProtType->setText(ProtTypeString);
    //
    QString Serial_P1String = fileToStringList[50];
    Serial_P1String.remove("\nSerial_P1 ");
    ui->lineEdit_Serial_P1->setText(Serial_P1String);
    //
    QString ManCode_P1String = fileToStringList[51];
    ManCode_P1String.remove("\nManCode_P1 ");
    ui->lineEdit_ManCode_P1->setText(ManCode_P1String);
    //
    QString DevType_P1String = fileToStringList[52];
    DevType_P1String.remove("\nDevType_P1 ");
    ui->lineEdit_DevType_P1->setText(DevType_P1String);
    //
    QString Version_P1String = fileToStringList[53];
    Version_P1String.remove("\nVersion_P1 ");
    ui->lineEdit_Version_P1->setText(Version_P1String);
    //
    QString Initial_value_P1String = fileToStringList[54];
    Initial_value_P1String.remove("\nInitial_value_P1 ");
    ui->lineEdit_Initial_value_P1->setText(Initial_value_P1String);
    //
    QString Service_P1String = fileToStringList[55];
    Service_P1String.remove("\nService_P1 ");
    ui->lineEdit_Service_P1->setText(Service_P1String);
    //
    QString Serial_P2String = fileToStringList[56];
    Serial_P2String.remove("\nSerial_P2 ");
    ui->lineEdit_Serial_P2->setText(Serial_P2String);
    //
    QString ManCode_P2String = fileToStringList[57];
    ManCode_P2String.remove("\nManCode_P2 ");
    ui->lineEdit_ManCode_P2->setText(ManCode_P2String);
    //
    QString DevType_P2String = fileToStringList[58];
    DevType_P2String.remove("\nDevType_P2 ");
    ui->lineEdit_DevType_P2->setText(DevType_P2String);
    //
    QString Version_P2String = fileToStringList[59];
    Version_P2String.remove("\nVersion_P2 ");
    ui->lineEdit_Version_P2->setText(Version_P2String);
    //
    QString Initial_value_P2String = fileToStringList[60];
    Initial_value_P2String.remove("\nInitial_value_P2 ");
    ui->lineEdit_Initial_value_P2->setText(Initial_value_P2String);
    //
    QString Service_P2String = fileToStringList[61];
    Service_P2String.remove("\nService_P2 ");
    ui->lineEdit_Service_P2->setText(Service_P2String);

    emit sendParamsMap(paramsMap);


     qDebug()<<"FormParamsEdit::slotGetParamsStringListFromFileFromMainWindow(QStringList fileToStringList)";

    //Read Software Configuration 5a000502
     qDebug()<<"serialNumber"<<serialNumber.toHex();
     qDebug()<<"serialNumber2"<<serialNumber2.toHex();
     qDebug()<<"serialNumber3"<<serialNumber3.toHex();
     qDebug()<<"serialNumber4"<<serialNumber4.toHex();
     qDebug()<<"manCode"<<manCode.toHex();
     qDebug()<<"devType"<<devType.toHex();
     qDebug()<<"DN"<<DN.toHex();
     qDebug()<<"PCB_SN_ByteArray"<<PCB_SN_ByteArray.toHex();
     qDebug()<<"PCB_SN_ByteArray2"<<PCB_SN_ByteArray2.toHex();
     qDebug()<<"PCB_SN_ByteArray3"<<PCB_SN_ByteArray3.toHex();
     qDebug()<<"PCB_SN_ByteArray4"<<PCB_SN_ByteArray4.toHex();
     qDebug()<<"flowMinByteArray"<<flowMinByteArray.toHex();
     qDebug()<<"flowMaxByteArray"<<flowMaxByteArray.toHex();
     qDebug()<<"tinMinByteArray"<<tinMinByteArray.toHex();
     qDebug()<<"tinMaxByteArray"<<tinMaxByteArray.toHex();
     qDebug()<<"toutMinByteArray"<<toutMinByteArray.toHex();
     qDebug()<<"toutMaxByteArray"<<toutMaxByteArray.toHex();
     qDebug()<<"reportDataByteArray"<<reportDataByteArray.toHex();
     qDebug()<<"averageTByteArray"<<averageTByteArray.toHex();
     qDebug()<<"pulsesMode1ByteArray"<<pulsesMode1ByteArray.toHex();
     qDebug()<<"pulsesMode2ByteArray"<<pulsesMode2ByteArray.toHex();
     qDebug()<<"pulsesValue1ByteArray"<<pulsesValue1ByteArray.toHex();
     qDebug()<<"pulsesValue2ByteArray"<<pulsesValue2ByteArray.toHex();
    //Read Sensor Parameters      5a00050b
     qDebug()<<"flowRateByteArray"<<flowRateByteArray.toHex();
     qDebug()<<"pressureByteArray"<<pressureByteArray.toHex();
     qDebug()<<"RTD_TypeByteArray"<<RTD_TypeByteArray.toHex();
     qDebug()<<"R_RTDByteArray"<<R_RTDByteArray.toHex();
     qDebug()<<"RWire1ByteArray"<<RWire1ByteArray.toHex();
     qDebug()<<"RWire2ByteArray"<<RWire2ByteArray.toHex();
     qDebug()<<"flowNomByteArray"<<flowNomByteArray.toHex();
    //Read Flow Coefficient  5a 00 05 1d
     qDebug()<<"flowCoef0ByteArray"<<flowCoef0ByteArray.toHex();
     qDebug()<<"flowCoef1ByteArray"<<flowCoef1ByteArray.toHex();
     qDebug()<<"flowCoef2ByteArray"<<flowCoef2ByteArray.toHex();
     qDebug()<<"flowCoef3ByteArray"<<flowCoef3ByteArray.toHex();
     qDebug()<<"flowCoef4ByteArray"<<flowCoef4ByteArray.toHex();
     qDebug()<<"flowCoef5ByteArray"<<flowCoef5ByteArray.toHex();
    //Read Calibration Factor 5a 00 05 06
     qDebug()<<"FS_DiffThrsByteArray"<<FS_DiffThrsByteArray.toHex();
    //Read Real Clock 5a 00 05 05
     qDebug()<<"hourByteArray"<<hourByteArray.toHex();
     qDebug()<<"minuteByteArray"<<minuteByteArray.toHex();
     qDebug()<<"secondByteArray"<<secondByteArray.toHex();
     qDebug()<<"dayByteArray"<<dayByteArray.toHex();
     qDebug()<<"monthByteArray"<<monthByteArray.toHex();
     qDebug()<<"yearByteArray"<<yearByteArray.toHex();
     qDebug()<<"DOWByteArray"<<DOWByteArray.toHex();
     qDebug()<<"ASW_flagByteArray"<<ASW_flagByteArray.toHex();
    //Read Calibration Factor
     qDebug()<<"shift_T1ByteArray"<<shift_T1ByteArray.toHex();
     qDebug()<<"shift_T2ByteArray"<<shift_T2ByteArray.toHex();
     qDebug()<<"scale_T1ByteArray"<<scale_T1ByteArray.toHex();
     qDebug()<<"scale_T2ByteArray"<<scale_T2ByteArray.toHex();
    //Read External Interface Settings
     qDebug()<<"addrByteArray"<<addrByteArray.toHex();
     qDebug()<<"baudRateByteArray"<<baudRateByteArray.toHex();
     qDebug()<<"HMP_TransByteArray"<<HMP_TransByteArray.toHex();
     qDebug()<<"EPP_TransByteArray"<<EPP_TransByteArray.toHex();
     qDebug()<<"P1PrimAddrByteArray"<<P1PrimAddrByteArray.toHex();
     qDebug()<<"P2PrimAddrByteArray"<<P2PrimAddrByteArray.toHex();
     qDebug()<<"ProtTypeByteArray"<<ProtTypeByteArray.toHex();
    //External Device Settings
     qDebug()<<"Serial_P1ByteArray"<<Serial_P1ByteArray.toHex();
     qDebug()<<"ManCode_P1ByteArray"<<ManCode_P1ByteArray.toHex();
     qDebug()<<"DevType_P1ByteArray"<<DevType_P1ByteArray.toHex();
     qDebug()<<"Version_P1ByteArray"<<Version_P1ByteArray.toHex();
     qDebug()<<"Initial_value_P1ByteArray"<<Initial_value_P1ByteArray.toHex();
     qDebug()<<"Service_P1ByteArray"<<Service_P1ByteArray.toHex();
     qDebug()<<"Serial_P2ByteArray"<<Serial_P2ByteArray.toHex();
     qDebug()<<"ManCode_P2ByteArray"<<ManCode_P2ByteArray.toHex();
     qDebug()<<"DevType_P2ByteArray"<<DevType_P2ByteArray.toHex();
     qDebug()<<"Version_P2ByteArray"<<Version_P2ByteArray.toHex();
     qDebug()<<"Initial_value_P2ByteArray"<<Initial_value_P2ByteArray.toHex();
     qDebug()<<"Service_P2ByteArray"<<Service_P2ByteArray.toHex();
}

void FormParamsEdit::slotGetSerial(QString serial)
{
    ui->lineEdit_serial->setText(serial);
}

void FormParamsEdit::slotGetSerial2(QString serial)
{
    ui->lineEdit_serial2->setText(serial);
}

void FormParamsEdit::slotGetSerial3(QString serial)
{
    ui->lineEdit_serial3->setText(serial);
}

void FormParamsEdit::slotGetSerial4(QString serial)
{
    ui->lineEdit_serial4->setText(serial);
}

void FormParamsEdit::on_lineEdit_serial2_textChanged(const QString &arg1)
{
    QString argstring = arg1;
    QString arg1Help = arg1;

    QString argHelpPart1 = arg1Help.left(3);
    QString argHelpPart2 = arg1Help.remove(0, 3);

    //добавление нулей
    if(argHelpPart2.length() < 5) {
        int nullNumber = 5 - argHelpPart2.length();
        for(int i=0; i<nullNumber; i++) {
            argHelpPart2.insert(0, '0');
        }
    }

    argstring = argHelpPart1 + argHelpPart2;

    ui->lineEdit_serial2->setText(argstring);

//    if(argstring.length() < 8) {
//        int nullNumber = 8 - argstring.length();
//        for(int i=0; i<nullNumber; i++) {
//            argstring.insert(0, '0');
//        }
//    }
    //добавление нулей/

    QString pcb_sn_String = ui->lineEdit_PCB_SN2->text().left(8);
    pcb_sn_String = pcb_sn_String + argstring;
    ui->lineEdit_PCB_SN2->setText(pcb_sn_String);

//    ui->textBrowser->append(argstring);

    serialNumber2.clear();
    for(int i=0; i<argstring.size(); i=i+2) {
        QString str = argstring.at(i);
 //       qDebug()<<"arg1[i]"<<str;
        QString str1 = argstring.at(i+1);
        quint8 byte = static_cast<quint8>(str.toInt());
        byte = (byte<<4)|(static_cast<quint8>(str1.toInt()));
        qDebug()<<"byte"<<byte;
//        str = str + arg1[i+1];
//        qDebug()<<"arg1[i] + arg1[i+1]"<<str;
        serialNumber2.append(byte);
    }
//   QByteArray deviceNumberFromLineEdit = arg1.toLocal8Bit();
//     deviceNumberFromLineEdit = arg1.toInt();
   qDebug()<<"serialNumber2.toHex()"<<serialNumber2.toHex();

   paramsMap["serialNumber2"] = serialNumber2;
   emit sendParamsMap(paramsMap);

 //  signalSendSerial2FromParamsEdit(arg1);
}

void FormParamsEdit::on_lineEdit_serial3_textChanged(const QString &arg1)
{
    QString argstring = arg1;
    QString arg1Help = arg1;

    QString argHelpPart1 = arg1Help.left(3);
    QString argHelpPart2 = arg1Help.remove(0, 3);

    //добавление нулей
    if(argHelpPart2.length() < 5) {
        int nullNumber = 5 - argHelpPart2.length();
        for(int i=0; i<nullNumber; i++) {
            argHelpPart2.insert(0, '0');
        }
    }

    argstring = argHelpPart1 + argHelpPart2;

    ui->lineEdit_serial3->setText(argstring);

//    if(argstring.length() < 8) {
//        int nullNumber = 8 - argstring.length();
//        for(int i=0; i<nullNumber; i++) {
//            argstring.insert(0, '0');
//        }
//    }
    //добавление нулей/

    QString pcb_sn_String = ui->lineEdit_PCB_SN3->text().left(8);
    pcb_sn_String = pcb_sn_String + argstring;
    ui->lineEdit_PCB_SN3->setText(pcb_sn_String);

//    ui->textBrowser->append(argstring);

    serialNumber3.clear();
    for(int i=0; i<argstring.size(); i=i+2) {
        QString str = argstring.at(i);
 //       qDebug()<<"arg1[i]"<<str;
        QString str1 = argstring.at(i+1);
        quint8 byte = static_cast<quint8>(str.toInt());
        byte = (byte<<4)|(static_cast<quint8>(str1.toInt()));
        qDebug()<<"byte"<<byte;
//        str = str + arg1[i+1];
//        qDebug()<<"arg1[i] + arg1[i+1]"<<str;
        serialNumber3.append(byte);
    }
//   QByteArray deviceNumberFromLineEdit = arg1.toLocal8Bit();
//     deviceNumberFromLineEdit = arg1.toInt();
   qDebug()<<"serialNumber3.toHex()"<<serialNumber3.toHex();

   paramsMap["serialNumber3"] = serialNumber3;
   emit sendParamsMap(paramsMap);

 //  signalSendSerial3FromParamsEdit(arg1);
}

void FormParamsEdit::on_lineEdit_serial4_textChanged(const QString &arg1)
{
    QString argstring = arg1;
    QString arg1Help = arg1;

    QString argHelpPart1 = arg1Help.left(3);
    QString argHelpPart2 = arg1Help.remove(0, 3);

    //добавление нулей
    if(argHelpPart2.length() < 5) {
        int nullNumber = 5 - argHelpPart2.length();
        for(int i=0; i<nullNumber; i++) {
            argHelpPart2.insert(0, '0');
        }
    }

    argstring = argHelpPart1 + argHelpPart2;

    ui->lineEdit_serial4->setText(argstring);

//    if(argstring.length() < 8) {
//        int nullNumber = 8 - argstring.length();
//        for(int i=0; i<nullNumber; i++) {
//            argstring.insert(0, '0');
//        }
//    }
    //добавление нулей/

    QString pcb_sn_String = ui->lineEdit_PCB_SN4->text().left(8);
    pcb_sn_String = pcb_sn_String + argstring;
    ui->lineEdit_PCB_SN4->setText(pcb_sn_String);

//    ui->textBrowser->append(argstring);

    serialNumber4.clear();
    for(int i=0; i<argstring.size(); i=i+2) {
        QString str = argstring.at(i);
 //       qDebug()<<"arg1[i]"<<str;
        QString str1 = argstring.at(i+1);
        quint8 byte = static_cast<quint8>(str.toInt());
        byte = (byte<<4)|(static_cast<quint8>(str1.toInt()));
        qDebug()<<"byte"<<byte;
//        str = str + arg1[i+1];
//        qDebug()<<"arg1[i] + arg1[i+1]"<<str;
        serialNumber4.append(byte);
    }
//   QByteArray deviceNumberFromLineEdit = arg1.toLocal8Bit();
//     deviceNumberFromLineEdit = arg1.toInt();
   qDebug()<<"serialNumber4.toHex()"<<serialNumber4.toHex();

   paramsMap["serialNumber4"] = serialNumber4;
   emit sendParamsMap(paramsMap);

 //  signalSendSerial4FromParamsEdit(arg1);
}

void FormParamsEdit::on_lineEdit_PCB_SN2_textChanged(const QString &arg1)
{
    if(arg1.length() != 16) {
        return;
    }

    QString argstring = arg1;

//    if(argstring.length() < 8) {
//        int nullNumber = 8 - argstring.length();
//        for(int i=0; i<nullNumber; i++) {
//            argstring.insert(0, '0');
//        }
//    }

//    ui->textBrowser->append(argstring);

    PCB_SN_ByteArray2.clear();
    for(int i=0; i<argstring.length(); i=i+2) {
        QString str = argstring.at(i);
 //       qDebug()<<"arg1[i]"<<str;
        QString str1 = argstring.at(i+1);
        bool ok;
        quint8 byte = static_cast<quint8>(str.toInt(&ok, 16));
        byte = (byte<<4)|(static_cast<quint8>(str1.toInt(&ok, 16)));
        qDebug()<<"byte"<<byte;
//        str = str + arg1[i+1];
//        qDebug()<<"arg1[i] + arg1[i+1]"<<str;
        PCB_SN_ByteArray2.append(byte);
    }


   qDebug()<<"PCB_SN_ByteArray2.toHex()"<<PCB_SN_ByteArray2.toHex();

   paramsMap["PCB_SN_ByteArray2"] = PCB_SN_ByteArray2;
   emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_PCB_SN3_textChanged(const QString &arg1)
{
    if(arg1.length() != 16) {
        return;
    }

    QString argstring = arg1;

//    if(argstring.length() < 8) {
//        int nullNumber = 8 - argstring.length();
//        for(int i=0; i<nullNumber; i++) {
//            argstring.insert(0, '0');
//        }
//    }

//    ui->textBrowser->append(argstring);

    PCB_SN_ByteArray3.clear();
    for(int i=0; i<argstring.length(); i=i+2) {
        QString str = argstring.at(i);
 //       qDebug()<<"arg1[i]"<<str;
        QString str1 = argstring.at(i+1);
        bool ok;
        quint8 byte = static_cast<quint8>(str.toInt(&ok, 16));
        byte = (byte<<4)|(static_cast<quint8>(str1.toInt(&ok, 16)));
        qDebug()<<"byte"<<byte;
//        str = str + arg1[i+1];
//        qDebug()<<"arg1[i] + arg1[i+1]"<<str;
        PCB_SN_ByteArray3.append(byte);
    }


   qDebug()<<"PCB_SN_ByteArray3.toHex()"<<PCB_SN_ByteArray3.toHex();

   paramsMap["PCB_SN_ByteArray3"] = PCB_SN_ByteArray3;
   emit sendParamsMap(paramsMap);
}

void FormParamsEdit::on_lineEdit_PCB_SN4_textChanged(const QString &arg1)
{
    if(arg1.length() != 16) {
        return;
    }

    QString argstring = arg1;

//    if(argstring.length() < 8) {
//        int nullNumber = 8 - argstring.length();
//        for(int i=0; i<nullNumber; i++) {
//            argstring.insert(0, '0');
//        }
//    }

//    ui->textBrowser->append(argstring);

    PCB_SN_ByteArray4.clear();
    for(int i=0; i<argstring.length(); i=i+2) {
        QString str = argstring.at(i);
 //       qDebug()<<"arg1[i]"<<str;
        QString str1 = argstring.at(i+1);
        bool ok;
        quint8 byte = static_cast<quint8>(str.toInt(&ok, 16));
        byte = (byte<<4)|(static_cast<quint8>(str1.toInt(&ok, 16)));
        qDebug()<<"byte"<<byte;
//        str = str + arg1[i+1];
//        qDebug()<<"arg1[i] + arg1[i+1]"<<str;
        PCB_SN_ByteArray4.append(byte);
    }


   qDebug()<<"PCB_SN_ByteArray4.toHex()"<<PCB_SN_ByteArray4.toHex();

   paramsMap["PCB_SN_ByteArray4"] = PCB_SN_ByteArray4;
   emit sendParamsMap(paramsMap);
}
