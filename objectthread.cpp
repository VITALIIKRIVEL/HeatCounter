#include <QCoreApplication>
#include <QApplication>

#include "objectthread.h"
#include <QDebug>
#include <QColor>
#include <QTextCodec>
#include <QTextStream>
#include "global.h"

ObjectThread::ObjectThread(QObject *parent/*, QMainWindow *mwd*/): QObject(parent)
{

    qDebug()<<"ObjectThread::ObjectThread() "<<"workPlace " + QString::number(workPlace);

    portStend = new QSerialPort(this);

 //   mainWnd = mwd;


//    Обмен с компьютером UART2
//    Скорость 115200
//    Длина слова 8 бит
//    1 стоп-бит
//    Контроля чётности нет

//    portStend->setBaudRate(QSerialPort::Baud115200);
//    portStend->setBaudRate(QSerialPort::Data8);
//    portStend->setParity(QSerialPort::NoParity);
//    portStend->setStopBits(QSerialPort::OneStop);

    allParamsNameList = QStringList()<<"serialNumber"<<
"serialNumber2"<<
"serialNumber3"<<
"serialNumber4"<<
"manCode"<<
"devType"<<
"DN"<<
"PCB_SN_ByteArray"<<
"PCB_SN_ByteArray2"<<
"PCB_SN_ByteArray3"<<
"PCB_SN_ByteArray4"<<
"flowMinByteArray"<<
"flowMaxByteArray"<<
"tinMinByteArray"<<
"tinMaxByteArray"<<
"toutMinByteArray"<<
"toutMaxByteArray"<<
"reportDataByteArray"<<
     "averageTByteArray"<<
     "pulsesMode1ByteArray"<<
     "pulsesMode2ByteArray"<<
     "pulsesValue1ByteArray"<<
     "pulsesValue2ByteArray"<<
    //Read Sensor Parameters      5a00050b
     "flowRateByteArray"<<
     "pressureByteArray"<<
     "RTD_TypeByteArray"<<
     "R_RTDByteArray"<<
     "RWire1ByteArray"<<
     "RWire2ByteArray"<<
     "flowNomByteArray"<<
    //Read Flow Coefficient  5a 00 05 1d
     "flowCoef0ByteArray"<<
     "flowCoef1ByteArray"<<
     "flowCoef2ByteArray"<<
     "flowCoef3ByteArray"<<
     "flowCoef4ByteArray"<<
     "flowCoef5ByteArray"<<
    //Read Calibration Factor 5a 00 05 06
     "FS_DiffThrsByteArray"<<
    //Read Real Clock 5a 00 05 05
     "hourByteArray"<<
     "minuteByteArray"<<
     "secondByteArray"<<
     "dayByteArray"<<
     "monthByteArray"<<
     "yearByteArray"<<
     "DOWByteArray"<<
     "ASW_flagByteArray"<<
    //Read Calibration Factor
     "shift_T1ByteArray"<<
     "shift_T2ByteArray"<<
     "scale_T1ByteArray"<<
     "scale_T2ByteArray"<<
    //Read External Interface Settings
     "addrByteArray"<<
     "baudRateByteArray"<<
     "HMP_TransByteArray"<<
     "EPP_TransByteArray"<<
     "P1PrimAddrByteArray"<<
     "P2PrimAddrByteArray"<<
     "ProtTypeByteArray"<<
    //External Device Settings
     "Serial_P1ByteArray"<<
     "ManCode_P1ByteArray"<<
     "DevType_P1ByteArray"<<
     "Version_P1ByteArray"<<
     "Initial_value_P1ByteArray"<<
     "Service_P1ByteArray"<<
     "Serial_P2ByteArray"<<
     "ManCode_P2ByteArray"<<
     "DevType_P2ByteArray"<<
     "Version_P2ByteArray"<<
     "Initial_value_P2ByteArray"<<
     "Service_P2ByteArray"<<
                                       "NB_Fi_TxCh"<<
                                       "NB_Fi_RxCh"<<
                                       "NB_Fi_RFBand"<<
                                       "NB_Fi_TxPwr"<<
//                                       "NB_Fi_RFPwr"<<  - не используется
                                       "NB_Fi_Mode"<<
                                       "NB_Fi_HdSh"<<
                                       "NB_Fi_MAC"<<
                                       "NB_Fi_RetNum"<<
                                       "NB_Fi_PldLen"<<
                                       "NB_Fi_HrbtNum"<<
                                       "NB_Fi_HrbtInt"<<
                                       "NB_Fi_InfoInt"<<
                                       "NB_Fi_BrdID"<<
                                       "NB_Fi_MdmID"<<
                                       "NB_Fi_Key"<<
                                       "W5_Trans"<<
                                       "W7_Trans"<<
                                       "NB_Fi_UsrPktInt_WAL";

    cmd = new QProcess(this);

    connect(cmd, SIGNAL(readyRead()), this, SLOT(slotProcessReadyRead()));
    connect(cmd, SIGNAL(started()), this, SLOT(slotProcessStarted()));

    vectorIndicatorBSLMatrix.resize(4);
    vectorIndicatorBSLMatrix.fill(false);

    vectorIndicatorTokPlaty.resize(4);
    vectorIndicatorTokPlaty.fill(false);

}

ObjectThread::~ObjectThread()
{

}

void ObjectThread::setWorkPlace(int workplaceNumber)
{
    workPlace = workplaceNumber;
}

void ObjectThread::setVectorIndicatorStateMatrix(QVector<QVector<bool> > vector)
{
    vectorIndicatorStateMatrix = vector;
}

void ObjectThread::setVectorTokPlaty(QVector<bool> vector)
{
    vectorIndicatorTokPlaty = vector;
}

void ObjectThread::setVectorBSLMatrix(QVector<bool> vector)
{
   vectorIndicatorBSLMatrix = vector;
}

void ObjectThread::setParamsMap(QMap<QString, QVariant> map)
{
    paramsMapToThreads = map;
}

void ObjectThread::setMapWrite(QMap<QString, QByteArray> map)
{
    mapwrite = map;
}

void ObjectThread::setMapRead(QMap<QString, QByteArray> mape)
{
   mapRead = mape;
}

void ObjectThread::setIsWorkPlaceUseVector(QVector<bool> vec)
{
   isWorkPlaceUse = vec;

   qDebug()<<"isWorkPlaceUse "<<isWorkPlaceUse;

}

QVector<bool> ObjectThread::getVectorTokPlaty()
{
    return vectorIndicatorTokPlaty;
}

QVector<bool> ObjectThread::getVectorBSL()
{
    return vectorIndicatorBSLMatrix;
}

QVector<QVector<bool>> ObjectThread::getVectorMatrix()
{
    return vectorIndicatorStateMatrix;
}

void ObjectThread::setCalibrationMap(QMap<QString, QByteArray> calibMap)
{
    calibrationMap = calibMap;

    R_Real1ByteArray = calibrationMap["R_Real1ByteArray"];
    R_Real2ByteArray = calibrationMap["R_Real2ByteArray"];

    R_Real1ByteArray2 = calibrationMap["R_Real1ByteArray2"];
    R_Real2ByteArray2 = calibrationMap["R_Real2ByteArray2"];

    R_Real1ByteArray3 = calibrationMap["R_Real1ByteArray3"];
    R_Real2ByteArray3 = calibrationMap["R_Real2ByteArray3"];

    R_Real1ByteArray4 = calibrationMap["R_Real1ByteArray4"];
    R_Real2ByteArray4 = calibrationMap["R_Real2ByteArray4"];

}

void ObjectThread::setCalibrationFloatMap(QMap<QString, float> calibFloatMap)
{
    calibrationFloatMap = calibFloatMap;

    R_Real1Float = calibrationFloatMap["R_Real1Float"];
    R_Real2Float = calibrationFloatMap["R_Real2Float"];

    R_Real1Float2 = calibrationFloatMap["R_Real1Float2"];
    R_Real2Float2 = calibrationFloatMap["R_Real2Float2"];

    R_Real1Float3 = calibrationFloatMap["R_Real1Float3"];
    R_Real2Float3 = calibrationFloatMap["R_Real2Float3"];

    R_Real1Float4 = calibrationFloatMap["R_Real1Float4"];
    R_Real2Float4 = calibrationFloatMap["R_Real2Float4"];

    temperature1Float = calibrationFloatMap["temperature1Float"];
    temperature2Float = calibrationFloatMap["temperature2Float"];

    temperature1Float2 = calibrationFloatMap["temperature1Float2"];
    temperature2Float2 = calibrationFloatMap["temperature2Float2"];

    temperature1Float3 = calibrationFloatMap["temperature1Float3"];
    temperature2Float3 = calibrationFloatMap["temperature2Float3"];

    temperature1Float4 = calibrationFloatMap["temperature1Float4"];
    temperature2Float4 = calibrationFloatMap["temperature2Float4"];

    Acceptable_R_Ref1Float = calibrationFloatMap["Acceptable_R_Ref1Float"];
    Acceptable_R_Ref2Float = calibrationFloatMap["Acceptable_R_Ref2Float"];
    Acceptable_TMeasDirFloat = calibrationFloatMap["Acceptable_TMeasDirFloat"];
    Acceptable_TMeasRevFloat = calibrationFloatMap["Acceptable_TMeasRevFloat"];
    Error_R_Reff1Float = calibrationFloatMap["Error_R_Reff1Float"];
    Error_R_Reff2Float = calibrationFloatMap["Error_R_Reff2Float"];
    Error_TMeasDirFloat = calibrationFloatMap["Error_TMeasDirFloat"];
    Error_TMeasRevFloat = calibrationFloatMap["Error_TMeasRevFloat"];

    Acceptable_R_Ref1Float2 = calibrationFloatMap["Acceptable_R_Ref1Float2"];
    Acceptable_R_Ref2Float2 = calibrationFloatMap["Acceptable_R_Ref2Float2"];
    Acceptable_TMeasDirFloat2 = calibrationFloatMap["Acceptable_TMeasDirFloat2"];
    Acceptable_TMeasRevFloat2 = calibrationFloatMap["Acceptable_TMeasRevFloat2"];
    Error_R_Reff1Float2 = calibrationFloatMap["Error_R_Reff1Float2"];
    Error_R_Reff2Float2 = calibrationFloatMap["Error_R_Reff2Float2"];
    Error_TMeasDirFloat2 = calibrationFloatMap["Error_TMeasDirFloat2"];
    Error_TMeasRevFloat2 = calibrationFloatMap["Error_TMeasRevFloat2"];

    Acceptable_R_Ref1Float3 = calibrationFloatMap["Acceptable_R_Ref1Float3"];
    Acceptable_R_Ref2Float3 = calibrationFloatMap["Acceptable_R_Ref2Float3"];
    Acceptable_TMeasDirFloat3 = calibrationFloatMap["Acceptable_TMeasDirFloat3"];
    Acceptable_TMeasRevFloat3 = calibrationFloatMap["Acceptable_TMeasRevFloat3"];
    Error_R_Reff1Float3 = calibrationFloatMap["Error_R_Reff1Float3"];
    Error_R_Reff2Float3 = calibrationFloatMap["Error_R_Reff2Float3"];
    Error_TMeasDirFloat3 = calibrationFloatMap["Error_TMeasDirFloat3"];
    Error_TMeasRevFloat3 = calibrationFloatMap["Error_TMeasRevFloat3"];

    Acceptable_R_Ref1Float4 = calibrationFloatMap["Acceptable_R_Ref1Float4"];
    Acceptable_R_Ref2Float4 = calibrationFloatMap["Acceptable_R_Ref2Float4"];
    Acceptable_TMeasDirFloat4 = calibrationFloatMap["Acceptable_TMeasDirFloat4"];
    Acceptable_TMeasRevFloat4 = calibrationFloatMap["Acceptable_TMeasRevFloat4"];
    Error_R_Reff1Float4 = calibrationFloatMap["Error_R_Reff1Float4"];
    Error_R_Reff2Float4 = calibrationFloatMap["Error_R_Reff2Float4"];
    Error_TMeasDirFloat4 = calibrationFloatMap["Error_TMeasDirFloat4"];
    Error_TMeasRevFloat4 = calibrationFloatMap["Error_TMeasRevFloat4"];

}


void ObjectThread::slotReadParams(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4)
{
    //    vectorIsErrorOccured.fill(false);


        QSerialPort *portTmp;// = new QSerialPort(this);

        int workPlaceNumber = workPlace;

        int currentIndicatorNumber = workPlaceNumber;

        QString label_StatusBar;
        QString errorString;

        if(!isWorkPlaceUse.at(workPlace)) return;

        switch (workPlaceNumber) {
        case 0:
            portTmp = port1;
            break;
        case 1:
            portTmp = port2;
            break;
        case 2:
            portTmp = port3;
            break;
        case 3:
            portTmp = port4;
            break;

        default:
            break;
        }

        portTmp->setBaudRate(QSerialPort::Baud19200);
        portTmp->setDataBits(QSerialPort::Data8);
        portTmp->setParity(QSerialPort::NoParity);
        portTmp->setStopBits(QSerialPort::OneStop);

        int currentBoxNumber = 0;

            //   readFromCounter();


        qDebug()<<"portTmp->portName() "<<portTmp->portName();

  //          paramsVector.clear();

               QByteArray buffer;

               if(!portTmp->isOpen()) {
                   if(!portTmp->open(QIODevice::ReadWrite)) {
     //                  QMessageBox::information(this, "", "Не удалось открыть порт УСО-2. Рабочее место: " + QString::number(workPlaceNumber + 1));
                       portTmp->close();
                       label_StatusBar = ("Не удалось открыть порт УСО-2. Рабочее место: " +
                                                    QString::number(workPlaceNumber + 1));
                       emit errorStringSignal(label_StatusBar + '\n');
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

       //                vectorIsErrorOccured[workPlaceNumber] = true;
                       return;
                   }
               }



               //Read Hardware Configuration 5a000501
           for(int i=0; i<20;i++) {

               portTmp->clear();
               packetToRead.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x05;
               packetToRead.append(byte);
               byte = 0x01;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
      //             QMessageBox::information(this, "", tr("Данные в порт не записаны") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

  //                 portTmp->close();
                   return;
               }

               global::pause(150);

               buffer = portTmp->readAll();

               qDebug()<<"buffer.toHex() "<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());


               if(buffer.isEmpty()) {
                   if(i==19) {
      //                 QMessageBox::information(this, "", tr("Не удалось прочитать данные") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                       portTmp->close();
                       label_StatusBar = (tr("Не удалось прочитать данные") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
     //                  ui->label_writeParams->setVisible(true);
      //                 vectorIsErrorOccured[workPlaceNumber] = true;
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x26;
                   bytesForChecking.append(byte);
                   byte = 0x81;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 19) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

   //                quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3]) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

     //                 emit sendbufferReadHardwareConfiguration(buffer, workPlaceNumber);

                      //
                      //serial
                      QByteArray serialByteAray;
                      serialByteAray.append(buffer[12]);
                      serialByteAray.append(buffer[13]);
                      serialByteAray.append(buffer[14]);
                      serialByteAray.append(buffer[15]);
    //                  ui->lineEdit_serial->setText(serialByteAray.toHex());
    //                  qDebug()<<"serialByteAray.toHex() "<<serialByteAray.toHex();
    //                  paramsVector.append(serialByteAray);

                      switch (workPlaceNumber) {
                      case 0:
                          mapRead["serialNumber"] = serialByteAray;
  //                        ui->lineEdit_serial->setText(serialByteAray.toHex());
  //                        qDebug()<<"serialByteAray.toHex() "<<serialByteAray.toHex();
                          break;
                      case 1:
                          mapRead["serialNumber2"] = serialByteAray;
   //                       ui->lineEdit_serial_2->setText(serialByteAray.toHex());
   //                       qDebug()<<"serialByteAray2.toHex() "<<serialByteAray.toHex();
                          break;
                      case 2:
                          mapRead["serialNumber3"] = serialByteAray;
//                          ui->lineEdit_serial_3->setText(serialByteAray.toHex());
//                          qDebug()<<"serialByteAray3.toHex() "<<serialByteAray.toHex();
                          break;
                      case 3:
                          mapRead["serialNumber4"] = serialByteAray;
//                          ui->lineEdit_serial_4->setText(serialByteAray.toHex());
//                          qDebug()<<"serialByteAray4.toHex() "<<serialByteAray.toHex();
                          break;
                      default:
                          break;
                      }

    //                  mapRead["serialNumber"] = serialByteAray;

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
//                      if(manCodeList.contains(QString::fromLatin1(manufacturerString))) {
//                          ui->comboBox_ManCode->setCurrentText(QString::fromLatin1(manufacturerString));
//                      }
//                      else {
//          //               QMessageBox::information(this, "", tr("Недопустимое значение: Производитель"));
//                      }
//                      qDebug()<<"QString::fromLatin1(manufacturerString)"<<QString::fromLatin1(manufacturerString);
//                      paramsVector.append(manCodeByteArray);
                      mapRead["manCode"] = manCodeByteArray;

                      //
                      //DevType
                      QByteArray devType;
                      devType.append(buffer[18]);

                      if(devType[0] == 0x04) {
       //                   ui->comboBox_DevType->setCurrentText("Обратный");
                      }

                      if(devType[0] == 0x0c) {
      //                    ui->comboBox_DevType->setCurrentText("Подающий");
                      }

                      if((devType[0] != 0x04) && (devType[0] != 0x0c) ) {
       //                   QMessageBox::information(this, "", tr("Недопустимое значение: Тип счётчика") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                      }

   //                   qDebug()<<"devType.toHex() "<<devType.toHex();
      //                paramsVector.append(devType);
                      mapRead["devType"] = devType;

                      //
                      //DN
                      QByteArray DN;
                      DN.append(buffer[19]);
//                      if(DNList.contains(QString::number((quint8)buffer[19], 16))) {
//                          ui->comboBox_DN->setCurrentText(QString::number((quint8)buffer[19], 16));
//                      }
//                      else {
//        //                 QMessageBox::information(this, "", tr("Недопустимое значение: Диаметр условного прохода"));
//                      }
  //                    qDebug()<<"DN.toHex() "<<DN.toHex();
  //                    paramsVector.append(DN);
                      mapRead["DN"] = DN;

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

//                      PCB_SN_ByteArray.clear();
//                      PCB_SN_ByteArray = PCB_SN_Array;

//                      ui->lineEdit_PCB_SN->setText(PCB_SN_Array.toHex());
//                      qDebug()<<"PCB_SN_Array.toHex() "<<PCB_SN_Array.toHex();
//                      qDebug()<<"PCB_SN_ByteArray.toHex() "<<PCB_SN_ByteArray.toHex();
//                      paramsVector.append(PCB_SN_Array);

                      switch (workPlaceNumber) {
                      case 0:
                         mapRead["PCB_SN_ByteArray"] = PCB_SN_Array;
                          break;
                      case 1:
                         mapRead["PCB_SN_ByteArray2"] = PCB_SN_Array;
                          break;
                      case 2:
                         mapRead["PCB_SN_ByteArray3"] = PCB_SN_Array;
                          break;
                      case 3:
                         mapRead["PCB_SN_ByteArray4"] = PCB_SN_Array;
                          break;
                      default:
                          break;
                      }

      //                mapRead["PCB_SN_ByteArray"] = PCB_SN_Array;


                      break;
                   }
                   else {
                       if(i==19) {
     //                     QMessageBox::information(this, "", tr("Ошибка данных") + ". Рабочее место: " + QString::number(workPlaceNumber + 1) );
                          portTmp->close();
                          label_StatusBar = (tr("Ошибка данных") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
     //                     ui->label_writeParams->setVisible(true);
     //                     vectorIsErrorOccured[workPlaceNumber] = true;
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }

               }



            }
               //
               //
               //Read Software Configuration 5a000502
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x05;
               packetToRead.append(byte);
               byte = 0x02;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
     //              QMessageBox::information(this, "", tr("Данные в порт не записаны") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
    //               ui->label_writeParams->setVisible(true);
    //               vectorIsErrorOccured[workPlaceNumber] = true;
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

               }

               global::pause(150);//(200);

               buffer = portTmp->readAll();

               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
      //                 QMessageBox::information(this, "", tr("Не удалось прочитать данные") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                       portTmp->close();
                       label_StatusBar = (tr("Не удалось прочитать данные") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
//                       ui->label_writeParams->setVisible(true);
//                       vectorIsErrorOccured[workPlaceNumber] = true;
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x16;
                   bytesForChecking.append(byte);
                   byte = 0x82;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                 //  quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3]) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);
        //              emit textBrowser("buffer.toHex()     " + buffer.toHex());

        //              emit sendbufferReadSoftWareConfiguration(buffer);

                      //
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
        //                  ui->lineEdit_Flow_min->setText(QString::number(flowMinFloat));
                      }
                      else {
         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Минимальный расход"));
         //                 ui->lineEdit_Flow_min->setText(QString::number(flowMinFloat));
                      }

//                      qDebug()<<"flowMinArray.toHex() "<<flowMinArray.toHex();
//                      paramsVector.append(flowMinArray);
                      mapRead["flowMinByteArray"] = flowMinArray;

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
        //                  ui->lineEdit_Flow_max->setText(QString::number(flowMaxFloat));
                      }
                      else {
         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Максимальный расход"));
         //                 ui->lineEdit_Flow_max->setText(QString::number(flowMaxFloat));
                      }

//                      qDebug()<<"flowMaxArray.toHex() "<<flowMaxArray.toHex();
//                      paramsVector.append(flowMaxArray);
                      mapRead["flowMaxByteArray"] = flowMaxArray;

                      //
                      //tinMin
                      QByteArray tinMinArray;
                      tinMinArray.append(buffer[8]);


                      quint8 tinMinQuint8 = buffer[8];
                      if(tinMinQuint8>=10 && tinMinQuint8<=60) {
      //                    ui->spinBox_Tin_min->setValue(tinMinQuint8);
                      }
                      else {
         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Минимальная температура подачи"));
                      }

//                      qDebug()<<"tinMinArray.toHex() "<<tinMinArray.toHex();
//                      paramsVector.append(tinMinArray);
                      mapRead["tinMinByteArray"] = tinMinArray;

                      //
                      //tinMax
                      QByteArray tinMaxArray;
                      tinMaxArray.append(buffer[9]);


                      quint8 tinMaxQuint8 = buffer[9];
                      if(tinMaxQuint8>=50 && tinMaxQuint8<=150) {
      //                    ui->spinBox_Tin_max->setValue(tinMaxQuint8);
                      }
                      else {
        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Максимальная температура подачи"));
                      }

//                      qDebug()<<"tinMaxArray.toHex() "<<tinMaxArray.toHex();
//                      paramsVector.append(tinMaxArray);
                      mapRead["tinMaxByteArray"] = tinMaxArray;

                      //
                      //toutMin
                      QByteArray toutMinArray;
                      toutMinArray.append(buffer[10]);

                      quint8 toutMinQuint8 = buffer[10];
                      if(toutMinQuint8>=10 && toutMinQuint8<=60) {
        //                  ui->spinBox_Tout_min->setValue(toutMinQuint8);
                      }
                      else {
         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Минимальная температура обратки"));
                      }

//                      qDebug()<<"toutMinArray.toHex() "<<toutMinArray.toHex();
//                      paramsVector.append(toutMinArray);
                      mapRead["toutMinByteArray"] = toutMinArray;


                      //
                      //toutMax
                      QByteArray toutMaxArray;
                      toutMaxArray.append(buffer[11]);

                      quint8 toutMaxQuint8 = buffer[11];
                      if(toutMaxQuint8>=50 && toutMaxQuint8<=150) {
         //                 ui->spinBox_Tout_max->setValue(toutMaxQuint8);
                      }
                      else {
         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Максимальная температура обратки"));
                      }

//                      qDebug()<<"toutMaxArray.toHex() "<<toutMaxArray.toHex();
//                      paramsVector.append(toutMaxArray);
                      mapRead["toutMaxByteArray"] = toutMaxArray;

                      //
                      //reportData
                      QByteArray reportDataArray;
                      reportDataArray.append(buffer[12]);

                      quint8 reportDataInt = buffer[12];
                      if(reportDataInt>=0 && reportDataInt<=31) {
         //                 ui->spinBox_ReportData->setValue(reportDataInt);
                      }
                      else {
          //                QMessageBox::information(this, "", tr("Недопустимое значение: Дата сохранения в журнал"));
                      }

//                      qDebug()<<"reportDataArray.toHex() "<<reportDataArray.toHex();
//                      paramsVector.append(reportDataArray);
                      mapRead["reportDataByteArray"] = reportDataArray;

                      //
                      //AverageT
                      QByteArray AverageTArray;
                      AverageTArray.append(buffer[13]);


                      quint8 AverageTInt = buffer[13];
//                      if(AverageTList.contains(QString::number(AverageTInt))) {
//                          ui->comboBox_Average_T->setCurrentText(QString::number(AverageTInt));
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Тип усреднения температуры"));
//                      }

//                      qDebug()<<"AverageTArray.toHex() "<<AverageTArray.toHex();
//                      paramsVector.append(AverageTArray);
                      mapRead["averageTByteArray"] = AverageTArray;

                      //
                      //pulsesMode1
                      QByteArray pulsesMode1Array;
                      pulsesMode1Array.append(buffer[14]);


                      quint8 pulsesMode1Int = (quint8)buffer[14];
//                      if(pulsesMode1List.contains(QString::number(pulsesMode1Int))) {
//                          ui->comboBox_PulsesMode1->setCurrentText(QString::number(pulsesMode1Int));
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Режим работы импульсного вывода"));
//                      }

//                      qDebug()<<"pulsesMode1Array.toHex() "<<pulsesMode1Array.toHex();
//                      paramsVector.append(pulsesMode1Array);
                      mapRead["pulsesMode1ByteArray"] = pulsesMode1Array;

                      //
                      //pulsesMode2
                      QByteArray pulsesMode2Array;
                      pulsesMode2Array.append(buffer[15]);


                      quint8 pulsesMode2Int = (quint8)buffer[15];
//                      if(pulsesMode1List.contains(QString::number(pulsesMode2Int))) {
//                          ui->comboBox_PulsesMode2->setCurrentText(QString::number(pulsesMode2Int));
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Режим работы импульсного вывода"));
//                      }

//                      qDebug()<<"pulsesMode2Array.toHex() "<<pulsesMode2Array.toHex();
//                      paramsVector.append(pulsesMode2Array);
                      mapRead["pulsesMode2ByteArray"] = pulsesMode2Array;

                      //
                      //pulsesValue1
                      QByteArray pulsesValue1Array;
                      pulsesValue1Array.append(buffer[16]);
                      pulsesValue1Array.append(buffer[17]);


                      quint16 pulsesValue1Int = buffer[16];
                      pulsesValue1Int = pulsesValue1Int<<8 | (quint8)buffer[17];
        //              if((pulsesValue1Int<=101) && (pulsesValue1Int>=99)) {

        //              }
        //              ui->lineEdit_PulsesValue1->setText(QString::number(pulsesValue1Int));

//                      qDebug()<<"pulsesValue1Array.toHex() "<<pulsesValue1Array.toHex();
//                      paramsVector.append(pulsesValue1Array);
                      mapRead["pulsesValue1ByteArray"] = pulsesValue1Array;

                      //
                      //pulsesValue2
                      QByteArray pulsesValue2Array;
                      pulsesValue2Array.append(buffer[18]);
                      pulsesValue2Array.append(buffer[19]);
                      qDebug()<<"pulsesValue2Array.toHex() "<<pulsesValue2Array.toHex();

                      quint16 pulsesValue2Int = buffer[18];
                      pulsesValue2Int = pulsesValue2Int<<8 | (quint8)buffer[19];
         //             ui->lineEdit_PulsesValue2->setText(QString::number(pulsesValue2Int));

//                      qDebug()<<"pulsesValue2Array.toHex() "<<pulsesValue2Array.toHex();
//                      paramsVector.append(pulsesValue2Array);
                      mapRead["pulsesValue2ByteArray"] = pulsesValue2Array;

                      //
                      //DevType
                      QByteArray devType;
                      devType.append(buffer[20]);

                      if(devType[0] == 0x04) {
       //                   ui->comboBox_DevType->setCurrentText("Обратный");
                      }

                      if(devType[0] == 0x0c) {
       //                   ui->comboBox_DevType->setCurrentText("Подающий");
                      }

                      if((devType[0] != 0x04) && (devType[0] != 0x0c) ) {
      //                    QMessageBox::information(this, "", tr("Недопустимое значение: Тип счётчика") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                          label_StatusBar = (tr("Недопустимое значение: Тип счётчика") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
           //               ui->label_writeParams->setVisible(true);
            //              vectorIsErrorOccured[workPlaceNumber] = true;
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                      }

//                      qDebug()<<"devType.toHex() "<<devType.toHex();
//                      paramsVector.append(devType);
                      mapRead["devType"] = devType;




                      break;
                   }
                   else {
                       if(i==2) {
     //                     QMessageBox::information(this, "", tr("Ошибка данных") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                          portTmp->close();
                          label_StatusBar = (tr("Ошибка данных") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
               //           ui->label_writeParams->setVisible(true);
                //          vectorIsErrorOccured[workPlaceNumber] = true;
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }

               }


            }

               //
               //
               //Read Sensor Parameters      5a00050b
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x05;
               packetToRead.append(byte);
               byte = 0x0b;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
     //              QMessageBox::information(this, "", tr("Данные в порт не записаны") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
          //         ui->label_writeParams->setVisible(true);
          //         vectorIsErrorOccured[workPlaceNumber] = true;
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               global::pause(150);//(200);

               buffer = portTmp->readAll();

               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
     //                  QMessageBox::information(this, "", tr("Не удалось прочитать данные") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                       portTmp->close();
                       label_StatusBar = (tr("Не удалось прочитать данные") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
//                       ui->label_writeParams->setVisible(true);
//                       vectorIsErrorOccured[workPlaceNumber] = true;
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x18;
                   bytesForChecking.append(byte);
                   byte = 0x8b;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

            //       quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3]) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);
        //              emit textBrowser("buffer.toHex()     " + buffer.toHex());

       //               emit sendbufferReadSensorParameters(buffer);

                      //
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

//                      if(flowRateList.contains(flowRateString)) {
//                          ui->comboBox_FlowRate->setCurrentText(QString::number(flowRateFloat));
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Вес импульса вертушки"));
//         //                 ui->comboBox_FlowRate->setCurrentText(QString::number(flowRateFloat));
//                      }

//                      qDebug()<<"flowRateArray.toHex() "<<flowRateArray.toHex();
//                      paramsVector.append(flowRateArray);
                      mapRead["flowRateByteArray"] = flowRateArray;

                      //
                      //pressure 1 byte (8)
                      QByteArray pressureArray;
                      pressureArray.append(buffer[8]);

                      quint8 pressureQuint8 = buffer[8];
                      if(pressureQuint8 == 6) {
           //               ui->lineEdit_Pressure->setText(QString::number(pressureQuint8));
                      }
                      else {
        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Условное давление"));
           //               ui->lineEdit_Pressure->setText(QString::number(pressureQuint8));
                      }

            //          qDebug()<<"pressureArray.toHex() "<<pressureArray.toHex();
            //          paramsVector.append(pressureArray);
                      mapRead["pressureByteArray"] = pressureArray;

                      //
                      //RTD_Type 1 byte (9)
                      QByteArray RTD_TypeArray;
                      RTD_TypeArray.append(buffer[9]);

                      quint8 RTD_TypeQuint8 = buffer[9];
//                      if((RTD_TypeQuint8 >= 0) && (RTD_TypeQuint8 <= (RTD_TypeList.count() - 1))) {
//                          ui->comboBox_RTD_Type->setCurrentIndex(RTD_TypeQuint8);
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Тип термометров сопротивления"));
//         //                 ui->comboBox_RTD_Type->setCurrentText(QString::number(RTD_TypeQuint8));
//                      }

//                      qDebug()<<"RTD_TypeArray.toHex() "<<RTD_TypeArray.toHex();
//                      paramsVector.append(RTD_TypeArray);
                      mapRead["RTD_TypeByteArray"] = RTD_TypeArray;


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

//                      if(R_RTDQuint32 == 1000) {
//                          ui->lineEdit_R_RTD->setText(QString::number(R_RTDQuint32));
//                      }
//                      else {
//          //                QMessageBox::information(this, "", tr("Недопустимое значение: Номинальное сопротивление термометров") + QString::number(R_RTDQuint32));
//                      }

//                      qDebug()<<"R_RTDArray.toHex() "<<R_RTDArray.toHex();
//                      paramsVector.append(R_RTDArray);
                      mapRead["R_RTDByteArray"] = R_RTDArray;

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

//                      if(RWire1Quint8 == 0) {
//                          ui->lineEdit_RWire1->setText(QString::number(RWire1Quint8));
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Сопротивление проводов термомнтров сопротивления"));
//                      }

//                      qDebug()<<"RWire1Array.toHex() "<<RWire1Array.toHex();
//                      paramsVector.append(RWire1Array);
                      mapRead["RWire1ByteArray"] = RWire1Array;

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

//                      if(RWire2Quint8 == 0) {
//                          ui->lineEdit_RWire2->setText(QString::number(RWire2Quint8));
//                      }
//                      else {
//          //                QMessageBox::information(this, "", tr("Недопустимое значение: Сопротивление проводов термомнтров сопротивления"));
//                      }

//                      qDebug()<<"RWire2Array.toHex() "<<RWire2Array.toHex();
//                      paramsVector.append(RWire2Array);
                      mapRead["RWire2ByteArray"] = RWire2Array;

                      //
                      //flowNom 1 byte (22)
                      QByteArray flowNomArray;
                      flowNomArray.append(buffer[22]);


                      quint8 flowNomInt = buffer[22];
                      float flowNomFloat = (float)flowNomInt/10;
//                      if(flowNomList.contains(QString::number(flowNomFloat))) {
//                          ui->comboBox_FlowNom->setCurrentText(QString::number(flowNomFloat));
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Номинальный расход"));
//                      }

//                      qDebug()<<"flowNomArray.toHex() "<<flowNomArray.toHex();
//                      paramsVector.append(flowNomArray);
                      mapRead["flowNomByteArray"] = flowNomArray;



                      break;
                   }
                   else {
                       if(i==2) {
     //                     QMessageBox::information(this, "", tr("Ошибка данных") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                          portTmp->close();
                          label_StatusBar = (tr("Ошибка данных") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
//                          ui->label_writeParams->setVisible(true);
//                          vectorIsErrorOccured[workPlaceNumber] = true;
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }

               }


            }

        //    //
        //    //
        //    //Read Flow Coefficient  5a 00 05 1d

            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x05;
               packetToRead.append(byte);
               byte = 0x1d;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
      //             QMessageBox::information(this, "", tr("Данные в порт не записаны") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
//                   ui->label_writeParams->setVisible(true);
//                   vectorIsErrorOccured[workPlaceNumber] = true;
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               global::pause(150);//(200);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
      //                 QMessageBox::information(this, "", tr("Не удалось прочитать данные") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                       portTmp->close();
                       label_StatusBar = (tr("Не удалось прочитать данные") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
//                       ui->label_writeParams->setVisible(true);
//                       vectorIsErrorOccured[workPlaceNumber] = true;
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x1d;
                   bytesForChecking.append(byte);
                   byte = 0x9d;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

       //            quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3]) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);
         //             emit textBrowser("buffer.toHex()     " + buffer.toHex());

        //              emit sendbufferReadFlowCoefficient(buffer);

                      //
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
//                      if(flowCoef0Float == 1) {
//                          ui->lineEdit_coef0->setText(QString::number(flowCoef0Float, 'g', 6) + ".000000");
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Коэффициент коррекции расхода 0"));
//                          ui->lineEdit_coef0->setText(QString::number(flowCoef0Float));
//                      }

//                      qDebug()<<"flowCoef0Array.toHex() "<<flowCoef0Array.toHex();
//                      paramsVector.append(flowCoef0Array);
                      mapRead["flowCoef0ByteArray"] = flowCoef0Array;

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
//                      if(flowCoef1Float == 1) {
//                          ui->lineEdit_coef1->setText(QString::number(flowCoef1Float) + ".000000");
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Коэффициент коррекции расхода 1"));
//                          ui->lineEdit_coef1->setText(QString::number(flowCoef1Float));
//                      }

//                      qDebug()<<"flowCoef1Array.toHex() "<<flowCoef1Array.toHex();
//                      paramsVector.append(flowCoef1Array);
                      mapRead["flowCoef1ByteArray"] = flowCoef1Array;


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
//                      if(flowCoef2Float == 1) {
//                          ui->lineEdit_coef2->setText(QString::number(flowCoef2Float) + ".000000");
//                      }
//                      else {
//          //                QMessageBox::information(this, "", tr("Недопустимое значение: Коэффициент коррекции расхода 2"));
//                          ui->lineEdit_coef2->setText(QString::number(flowCoef2Float));
//                      }

//                      qDebug()<<"flowCoef2Array.toHex() "<<flowCoef2Array.toHex();
//                      paramsVector.append(flowCoef2Array);
                      mapRead["flowCoef2ByteArray"] = flowCoef2Array;


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
//                      if(flowCoef3Float == 1) {
//                          ui->lineEdit_coef3->setText(QString::number(flowCoef3Float) + ".000000");
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Коэффициент коррекции расхода 3"));
//                          ui->lineEdit_coef3->setText(QString::number(flowCoef3Float));
//                      }

//                      qDebug()<<"flowCoef3Array.toHex() "<<flowCoef3Array.toHex();
//                      paramsVector.append(flowCoef3Array);
                      mapRead["flowCoef3ByteArray"] = flowCoef3Array;


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
//                      if(flowCoef4Float == 1) {
//                          ui->lineEdit_coef4->setText(QString::number(flowCoef4Float) + ".000000");
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Коэффициент коррекции расхода 4"));
//                          ui->lineEdit_coef4->setText(QString::number(flowCoef4Float));
//                      }

//                      qDebug()<<"flowCoef4Array.toHex() "<<flowCoef4Array.toHex();
//                      paramsVector.append(flowCoef4Array);
                      mapRead["flowCoef4ByteArray"] = flowCoef4Array;


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
//                      if(flowCoef5Float == 1) {
//                          ui->lineEdit_coef5->setText(QString::number(flowCoef5Float) + ".000000");
//                      }
//                      else {
//          //                QMessageBox::information(this, "", tr("Недопустимое значение: Коэффициент коррекции расхода 5"));
//                          ui->lineEdit_coef5->setText(QString::number(flowCoef5Float));
//                      }

//                      qDebug()<<"flowCoef5Array.toHex() "<<flowCoef5Array.toHex();
//                      paramsVector.append(flowCoef5Array);
                      mapRead["flowCoef5ByteArray"] = flowCoef5Array;





                      break;
                   }
                   else {
                       if(i==2) {
     //                     QMessageBox::information(this, "", tr("Ошибка данных") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                          portTmp->close();
                          label_StatusBar = (tr("Ошибка данных") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
//                          ui->label_writeParams->setVisible(true);
//                          vectorIsErrorOccured[workPlaceNumber] = true;
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }

               }


            }


        //    //
        //    //Read Calibration Factor 5a 00 05 06
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x05;
               packetToRead.append(byte);
               byte = 0x06;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
      //             QMessageBox::information(this, "", tr("Данные в порт не записаны") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
//                   ui->label_writeParams->setVisible(true);
//                   vectorIsErrorOccured[workPlaceNumber] = true;
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               global::pause(150);//(200);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
     //                  QMessageBox::information(this, "", tr("Не удалось прочитать данные") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                       portTmp->close();
                       label_StatusBar = (tr("Не удалось прочитать данные") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
//                       ui->label_writeParams->setVisible(true);
//                       vectorIsErrorOccured[workPlaceNumber] = true;
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x23;
                   bytesForChecking.append(byte);
                   byte = 0x86;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

       //            quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3]) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);
        //              emit textBrowser("buffer.toHex()      " + buffer.toHex());

        //              emit sendbufferReadCalibrationFactor(buffer);

                      //
                      //FS_DiffThrs
                      QByteArray FS_DiffThrsArray;
                      FS_DiffThrsArray.append(buffer[8]);
                      FS_DiffThrsArray.append(buffer[9]);


                      quint16 FS_DiffThrsQuint16 = buffer[8];
                      FS_DiffThrsQuint16 = FS_DiffThrsQuint16<<8 | buffer[9];

//                      if(FS_DiffThrsQuint16 == 30) {
//                          ui->lineEdit_FS_DiffThrs->setText(QString::number(FS_DiffThrsQuint16));
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Порог срабатывания датчика расхода "));
//                          ui->lineEdit_FS_DiffThrs->setText(QString::number(FS_DiffThrsQuint16));
//                      }

//                      qDebug()<<"FS_DiffThrsArray.toHex() "<<FS_DiffThrsArray.toHex();
//                      paramsVector.append(FS_DiffThrsArray);
                      mapRead["FS_DiffThrsByteArray"] = FS_DiffThrsArray;



                      break;
                   }
                   else {
                       if(i==2) {
     //                     QMessageBox::information(this, "", tr("Ошибка данных") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                          portTmp->close();
                          label_StatusBar = (tr("Ошибка данных") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
//                          ui->label_writeParams->setVisible(true);
//                          vectorIsErrorOccured[workPlaceNumber] = true;
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }

               }


            }

            //
            //Read Real Clock 5a 00 05 05
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x05;
               packetToRead.append(byte);
               byte = 0x05;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
     //              QMessageBox::information(this, "", tr("Данные в порт не записаны") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
//                   ui->label_writeParams->setVisible(true);
//                   vectorIsErrorOccured[workPlaceNumber] = true;
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               global::pause(150);//(200);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось прочитать данные") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
    //                   portTmp->close();
                       label_StatusBar = (tr("Не удалось прочитать данные") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
//                       ui->label_writeParams->setVisible(true);
//                       vectorIsErrorOccured[workPlaceNumber] = true;
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x0e;
                   bytesForChecking.append(byte);
                   byte = 0x85;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

             //      quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3]) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);
        //              emit textBrowser("buffer.toHex()     " + buffer.toHex());

        //              emit sendbufferReadRealClock(buffer);

                      //
                      //Hour
                      QDateTime dateTime_RealClock;
                      QDate date_RealClock;
                      QTime time_RealClock;

                      QByteArray HourArray;
                      HourArray.append(buffer[4]);


                      quint8 HourQuint8 = buffer[4];

//                      if(HourQuint8>=0 && HourQuint8<=23) {
//         //                 time_RealClock.
//                      }
//                      else {
//      //                    QMessageBox::information(this, "", tr("Недопустимое значение: Часы") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
//                          label_StatusBar = (tr("Недопустимое значение: Часы") +
//                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
//                          emit errorStringSignal(label_StatusBar + '\n');
//                          ui->label_writeParams->setVisible(true);
//                          vectorIsErrorOccured[workPlaceNumber] = true;
//                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
//                          checkWritingError(currentIndicatorNumber);
//                          return;
//                      }

//                      qDebug()<<"HourArray.toHex() "<<HourArray.toHex();
//                      paramsVector.append(HourArray);
                      mapRead["hourByteArray"] = HourArray;

                      //
                      //Minute
                      QByteArray MinuteArray;
                      MinuteArray.append(buffer[5]);


                      quint8 MinuteQuint8 = buffer[5];

//                      if(MinuteQuint8>=0 && MinuteQuint8<=59) {
//         //                 time_RealClock.
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Минуты"));
//         //                 ui->lineEdit_FS_DiffThrs->setText(QString::number(FS_DiffThrsQuint16));
//                      }

//                      qDebug()<<"MinuteArray.toHex() "<<MinuteArray.toHex();
//                      paramsVector.append(MinuteArray);
                      mapRead["minuteByteArray"] = MinuteArray;

                      //
                      //Second
                      QByteArray SecondArray;
                      SecondArray.append(buffer[6]);

                      quint8 SecondQuint8 = buffer[6];

                      if(SecondQuint8>=0 && SecondQuint8<=59) {
         //                 time_RealClock.
                      }
                      else {
         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Секунды"));
         //                 ui->lineEdit_FS_DiffThrs->setText(QString::number(FS_DiffThrsQuint16));
                      }

//                      qDebug()<<"SecondArray.toHex() "<<SecondArray.toHex();
//                      paramsVector.append(SecondArray);
                      mapRead["secondByteArray"] = SecondArray;

                      time_RealClock.setHMS(HourQuint8, MinuteQuint8, SecondQuint8);

                      //
                      //Day
                      QByteArray dayArray;
                      dayArray.append(buffer[7]);

                      quint8 dayQuint8 = buffer[7];

                      if(dayQuint8>=1 && dayQuint8<=31) {

                      }
                      else {
         //                 QMessageBox::information(this, "", tr("Недопустимое значение: День месяца"));
                      }

//                      qDebug()<<"dayArray.toHex()"<<dayArray.toHex();
//                      paramsVector.append(dayArray);
                      mapRead["dayByteArray"] = dayArray;

                      //
                      //Month
                      QByteArray monthArray;
                      monthArray.append(buffer[8]);

                      quint8 monthQuint8 = buffer[8];

                      if(monthQuint8>=1 && monthQuint8<=12) {

                      }
                      else {
         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Месяц"));
                      }

//                      qDebug()<<"monthArray.toHex()"<<monthArray.toHex();
//                      paramsVector.append(monthArray);
                      mapRead["monthByteArray"] = monthArray;

                      //
                      //Year
                      QByteArray yearArray;
                      yearArray.append(buffer[9]);


                      quint16 yearQuint16 = buffer[9] + 2000;

                      if(yearQuint16>=1990 && yearQuint16<=3000) {

                      }
                      else {
         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Год"));
                      }

//                      qDebug()<<"yearArray.toHex()"<<yearArray.toHex();
//                      paramsVector.append(yearArray);
                      mapRead["yearByteArray"] = yearArray;

                      //
                      //DOW
                      QByteArray DOWArray;
                      DOWArray.append(buffer[10]);


                      quint8 DOWQuint8 = buffer[10];

                      if(DOWQuint8>=1 && DOWQuint8<=7) {

                      }
                      else {
          //                QMessageBox::information(this, "", tr("Недопустимое значение: День недели"));
                      }

//                      qDebug()<<"DOWArray.toHex()"<<DOWArray.toHex();
//                      paramsVector.append(DOWArray);
                      mapRead["DOWByteArray"] = DOWArray;

                      date_RealClock.setDate(yearQuint16, monthQuint8, dayQuint8);

                      dateTime_RealClock.setTime(time_RealClock);
                      dateTime_RealClock.setDate(date_RealClock);
//                      ui->dateTimeEdit_realClock->setDisplayFormat("dd.MM.yyyy hh:mm:ss");
//                      ui->dateTimeEdit_realClock->setDateTime(dateTime_RealClock);

                      //
                      //ASW_flag
                      QByteArray ASW_flagArray;
                      ASW_flagArray.append(buffer[12]);


                      quint8 ASW_flagQuint8 = buffer[12];

//                      if(ASW_flagList.contains(QString::number(ASW_flagQuint8))) {
//                          ui->comboBox_ASW_flag->setCurrentText(QString::number(ASW_flagQuint8));
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Автоматический переход зима-лето"));
//                      }

//                      qDebug()<<"ASW_flagArray.toHex()"<<ASW_flagArray.toHex();
//                      paramsVector.append(ASW_flagArray);
                      mapRead["ASW_flagByteArray"] = ASW_flagArray;
                      //
                      //



                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка данных"));
    //                      portTmp->close();
                          label_StatusBar = (tr("Ошибка данных") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
//                          ui->label_writeParams->setVisible(true);
//                          vectorIsErrorOccured[workPlaceNumber] = true;
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }



               }


            }


            //
            //Read Calibration Factor 2
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x05;
               packetToRead.append(byte);
               byte = 0x06;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
       //            QMessageBox::information(this, "", tr("Данные в порт не записаны") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
//                   ui->label_writeParams->setVisible(true);
//                   vectorIsErrorOccured[workPlaceNumber] = true;
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               global::pause(150);//(200);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось прочитать данные") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
    //                   portTmp->close();
                       label_StatusBar = (tr("Не удалось прочитать данные") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
//                       ui->label_writeParams->setVisible(true);
//                       vectorIsErrorOccured[workPlaceNumber] = true;
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x23;
                   bytesForChecking.append(byte);
                   byte = 0x86;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

       //            quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3]) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);
         //             emit textBrowser("buffer.toHex()     " + buffer.toHex());

         //             emit sendbufferReadCalibrationFactor2(buffer);

                      //
                      //Shift_T1

                      QByteArray Shift_T1Array;
                      Shift_T1Array.append(buffer[18]);
                      Shift_T1Array.append(buffer[19]);
                      qDebug()<<"Shift_T1Array.toHex() "<<Shift_T1Array.toHex();

                      quint16 Shift_T1Quint16 = buffer[18];
                      Shift_T1Quint16 = Shift_T1Quint16<<8 | buffer[19];

                      float Shift_T1Float = (float)Shift_T1Quint16/1000000;

                      if((int)Shift_T1Float == 0) {
          //                ui->lineEdit_Shift_T1->setText("0.000000");
                      }
                      else {
          //                QMessageBox::information(this, "", tr("Недопустимое значение: Коэфффициент коррекции shift_t1 ")+ QString::number(Shift_T1Float)) ;
                      }

          //            paramsVector.append(Shift_T1Array);
                      mapRead["shift_T1ByteArray"] = Shift_T1Array;

                      //
                      //Shift_T2
                      QByteArray Shift_T2Array;
                      Shift_T2Array.append(buffer[20]);
                      Shift_T2Array.append(buffer[21]);
                      qDebug()<<"Shift_T2Array.toHex() "<<Shift_T2Array.toHex();

                      quint16 Shift_T2Quint16 = buffer[20];
                      Shift_T2Quint16 = Shift_T2Quint16<<8 | buffer[21];

                      float Shift_T2Float = (float)Shift_T2Quint16/1000000;

                      if((int)Shift_T2Float == 0) {
          //                ui->lineEdit_Shift_T2->setText("0.000000");
                      }
                      else {
         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Коэфффициент коррекции shift_t2 ") + QString::number(Shift_T2Float));
                      }

          //            paramsVector.append(Shift_T2Array);
                      mapRead["shift_T2ByteArray"] = Shift_T2Array;

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
         //                 ui->lineEdit_Scale_T1->setText("1.000000");
                      }
                      else {
        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Коэфффициент коррекции scale_t1 ") + QString::number(Scale_T1Float));
                      }

        //              paramsVector.append(Scale_T1Array);
                      mapRead["scale_T1ByteArray"] = Scale_T1Array;

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
             //             ui->lineEdit_Scale_T2->setText("1.000000");
               //           ui->lineEdit_Scale_T2->setText(QString::number((int)Scale_T2Float));
                      }
                      else {
         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Коэфффициент коррекции scale_t2 ") + QString::number(Scale_T2Float));
                      }

          //            paramsVector.append(Scale_T2Array);
                      mapRead["scale_T2ByteArray"] = Scale_T2Array;

                      //
                      //



                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка данных") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
    //                      portTmp->close();
                          label_StatusBar = (tr("Ошибка данных") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
//                          ui->label_writeParams->setVisible(true);
//                          vectorIsErrorOccured[workPlaceNumber] = true;
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }



               }


            }


            //
            //Read External Interface Settings 5a 00 05 1b
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x05;
               packetToRead.append(byte);
               byte = 0x1b;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
       //            QMessageBox::information(this, "", tr("Данные в порт не записаны") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
//                   ui->label_writeParams->setVisible(true);
//                   vectorIsErrorOccured[workPlaceNumber] = true;
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;;
               }

               global::pause(150);//(200);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось прочитать данные") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
    //                   portTmp->close();
                       label_StatusBar = (tr("Не удалось прочитать данные") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
//                       ui->label_writeParams->setVisible(true);
//                       vectorIsErrorOccured[workPlaceNumber] = true;
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x0e;
                   bytesForChecking.append(byte);
                   byte = 0x9b;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

            //       quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3]) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);
         //             emit textBrowser("buffer.toHex()     " + buffer.toHex());

         //             emit sendbufferReadExternalInterfaceSettings(buffer);

                      //
                      //Addr
                      QByteArray AddrArray;
                      AddrArray.append(buffer[4]);
                      qDebug()<<"AddrArray.toHex() "<<AddrArray.toHex();

                      quint8 AddrQuint8 = (quint8)buffer[4];

//                      if(AddrQuint8>=0 && AddrQuint8<=252) {
//                          ui->spinBox_Addr->setValue(AddrQuint8);
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Первичный адрес счётчика "));
//                          ui->spinBox_Addr->setValue(AddrQuint8);
//                      }

//                      paramsVector.append(AddrArray);
                      mapRead["addrByteArray"] = AddrArray;

                      //
                      //BaudRate
                      QByteArray BaudRateArray;
                      BaudRateArray.append(buffer[5]);
                      qDebug()<<"BaudRateArray.toHex() "<<BaudRateArray.toHex();

                      quint8 BaudRateQuint8 = (quint8)buffer[5];

//                      for(int j=0; j< ui->comboBox_BaudRate->count(); j++) {
//                          ui->comboBox_BaudRate->setCurrentIndex(j);
//                          if(BaudRateQuint8 == (quint8)ui->comboBox_BaudRate->currentData().toInt()) {
//                              break;
//                          }

//                          if(j==3 && (BaudRateQuint8 != (quint8)ui->comboBox_BaudRate->currentData().toInt())) {
//                               //                 QMessageBox::information(this, "", tr("Недопустимое значение: Скорость обмена ") + QString::number(BaudRateQuint8, 16));
//                          }
//                      }


//                      paramsVector.append(BaudRateArray);
                      mapRead["baudRateByteArray"] = BaudRateArray;

                      //
                      //HMP_Trans 2bytes  6,7
                      QByteArray HMP_TransArray;
                      HMP_TransArray.append(buffer[6]);
                      HMP_TransArray.append(buffer[7]);
                      qDebug()<<"HMP_TransArray.toHex() "<<HMP_TransArray.toHex();

                      quint16 HMP_TransInt = (quint8)buffer[6];
                      HMP_TransInt = HMP_TransInt<<8 | (quint8)buffer[7];

//                      if(HMP_TransInt == 0x3fff) {
//                          ui->lineEdit_HMP_Trans->setText(QString::number(HMP_TransInt, 16));
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Позиционный код передаваемых параметров HMP_Trans ") +
//        //                                           QString::number(HMP_TransInt, 16));
//                      }

//                      paramsVector.append(HMP_TransArray);
                      mapRead["HMP_TransByteArray"] = HMP_TransArray;

                      //
                      //EPP_Trans 2bytes  8,9
                      QByteArray EPP_TransArray;
                      EPP_TransArray.append(buffer[8]);
                      EPP_TransArray.append(buffer[9]);
                      qDebug()<<"EPP_TransArray.toHex() "<<EPP_TransArray.toHex();

                      quint16 EPP_TransInt = (quint8)buffer[8];
                      EPP_TransInt = EPP_TransInt<<8 | (quint8)buffer[9];

//                      if(EPP_TransInt == 0x000f) {
//                          ui->lineEdit_EPP_Trans->setText(QString::number(EPP_TransInt, 16));
//                      }
//                      else {
//          //                QMessageBox::information(this, "", tr("Недопустимое значение: Позиционный код передаваемых параметров EPP_Trans ") +
//          //                                         QString::number(EPP_TransInt, 16));
//                      }

//                      paramsVector.append(EPP_TransArray);
                      mapRead["EPP_TransByteArray"] = EPP_TransArray;

                      //
                      //P1_PrimAddr 1byte  10
                      QByteArray P1_PrimAddrArray;
                      P1_PrimAddrArray.append(buffer[10]);
                      qDebug()<<"P1_PrimAddrArray.toHex() "<<P1_PrimAddrArray.toHex();

                      quint8 P1_PrimAddrInt = (quint8)buffer[10];

//                      if(P1_PrimAddrInt>=0 && P1_PrimAddrInt<=252) {
//                          ui->spinBox_P1PrimAddr->setValue(P1_PrimAddrInt);
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Первичный адрес импульсных входов P1_PrimAddr ") +
//        //                                           QString::number(P1_PrimAddrInt, 10));
//                      }

//                      paramsVector.append(P1_PrimAddrArray);
                      mapRead["P1PrimAddrByteArray"] = P1_PrimAddrArray;

                      //
                      //P2_PrimAddr 1byte  11
                      QByteArray P2_PrimAddrArray;
                      P2_PrimAddrArray.append(buffer[11]);
                      qDebug()<<"P2_PrimAddrArray.toHex() "<<P2_PrimAddrArray.toHex();

                      quint8 P2_PrimAddrInt = (quint8)buffer[11];

//                      if(P2_PrimAddrInt>=0 && P2_PrimAddrInt<=252) {
//                          ui->spinBox_P2PrimAddr->setValue(P2_PrimAddrInt);
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Первичный адрес импульсных входов P2_PrimAddr ") +
//        //                                           QString::number(P2_PrimAddrInt, 10));
//                      }

//                      paramsVector.append(P2_PrimAddrArray);
                      mapRead["P2PrimAddrByteArray"] = P2_PrimAddrArray;

                      //
                      //ProtType 1byte  12
                      QByteArray ProtTypeArray;
                      ProtTypeArray.append(buffer[12]);
                      qDebug()<<"ProtTypeArray.toHex() "<<ProtTypeArray.toHex();

                      quint8 ProtTypeInt = (quint8)buffer[12];

//                      if(ProtTypeInt == 0xff) {
//                          ui->lineEdit_ProtType->setText(QString::number(ProtTypeInt, 16));
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Первичный адрес импульсных входов P2_PrimAddr ") +
//        //                                           QString::number(ProtTypeInt, 16));
//                      }

//                      paramsVector.append(ProtTypeArray);
                      mapRead["ProtTypeByteArray"] = ProtTypeArray;




                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка данных") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
    //                      portTmp->close();
                          label_StatusBar = (tr("Ошибка данных") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
//                          ui->label_writeParams->setVisible(true);
//                          vectorIsErrorOccured[workPlaceNumber] = true;
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }



               }


            }

            //
            //
            //Read External Device Settings 5a 00 05 1c
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x05;
               packetToRead.append(byte);
               byte = 0x1c;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
     //              QMessageBox::information(this, "", tr("Данные в порт не записаны") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
//                   ui->label_writeParams->setVisible(true);
//                   vectorIsErrorOccured[workPlaceNumber] = true;
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               global::pause(150);//(200);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось прочитать данные") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
    //                   portTmp->close();
                       label_StatusBar = (tr("Не удалось прочитать данные") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
//                       ui->label_writeParams->setVisible(true);
//                       vectorIsErrorOccured[workPlaceNumber] = true;
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x25;
                   bytesForChecking.append(byte);
                   byte = 0x9c;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

         //          quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3]) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);
        //              emit textBrowser("buffer.toHex()     " + buffer.toHex());

        //              emit sendbufferReadExternalDeviceSettings(buffer);

                      //
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

//                      if(Serial_P1Quint32 == 0x00000000) {
//                          ui->lineEdit_Serial_P1->setText(QString::number(Serial_P1Quint32, 16));
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 1 Serial_P1 ")
//        //                                           + QString::number(Serial_P1Quint32, 16));
//                      }

//                      paramsVector.append(Serial_P1Array);
                      mapRead["Serial_P1ByteArray"] = Serial_P1Array;

                      //
                      //ManCode_P1
                      QByteArray ManCode_P1Array;
                      ManCode_P1Array.append(buffer[8]);
                      ManCode_P1Array.append(buffer[9]);
                      qDebug()<<"ManCode_P1Array.toHex() "<<ManCode_P1Array.toHex();

                      quint16 ManCode_P1Quint16 = (quint8)buffer[8];
                      ManCode_P1Quint16 = ManCode_P1Quint16<<8 | (quint8)buffer[9];

//                      if(ManCode_P1Quint16 == 0x0000) {
//                          ui->lineEdit_ManCode_P1->setText(QString::number(ManCode_P1Quint16, 16));
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 1 ManCode_P1 ")
//        //                                           + QString::number(ManCode_P1Quint16, 16));
//                      }

//                      paramsVector.append(ManCode_P1Array);
                      mapRead["ManCode_P1ByteArray"] = ManCode_P1Array;

                      //
                      //DevType_P1
                      QByteArray DevType_P1Array;
                      DevType_P1Array.append(buffer[10]);
                      qDebug()<<"DevType_P1Array.toHex() "<<DevType_P1Array.toHex();

                      quint8 DevType_P1Int = (quint8)buffer[10];

//                      if(DevType_P1Int == 0x00) {
//                          ui->lineEdit_DevType_P1->setText(QString::number(DevType_P1Int, 16));
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 1 DevType_P1 ") +
//        //                                           QString::number(DevType_P1Int, 16));
//                      }

//                      paramsVector.append(DevType_P1Array);
                      mapRead["DevType_P1ByteArray"] = DevType_P1Array;

                      //
                      //Version_P1
                      QByteArray Version_P1Array;
                      Version_P1Array.append(buffer[11]);
                      qDebug()<<"Version_P1Array.toHex() "<<Version_P1Array.toHex();

                      quint8 Version_P1Int = (quint8)buffer[11];

//                      if(Version_P1Int == 0) {
//                          ui->lineEdit_Version_P1->setText(QString::number(Version_P1Int, 10));
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 1 Version_P1 ") +
//        //                                           QString::number(Version_P1Int, 10));
//                      }

//                      paramsVector.append(Version_P1Array);
                      mapRead["Version_P1ByteArray"] = Version_P1Array;

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

//                      if(Initial_value_P1Int == 0) {
//                          ui->lineEdit_Initial_value_P1->setText(QString::number(Initial_value_P1Int));
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 1 Initial_value_P1 ") +
//        //                                           QString::number(Initial_value_P1Int));
//                      }

//                      paramsVector.append(Initial_value_P1Array);
                      mapRead["Initial_value_P1ByteArray"] = Initial_value_P1Array;

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

//                      if(Service_P1Int == 0x00000000) {
//                          ui->lineEdit_Service_P1->setText(QString::number(Service_P1Int, 16));
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 1 Service_P1 ") +
//        //                                           QString::number(Service_P1Int, 16));
//                      }

//                      paramsVector.append(Service_P1Array);
                      mapRead["Service_P1ByteArray"] = Service_P1Array;

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

//                      if(Serial_P2Quint32 == 0x00000000) {
//                          ui->lineEdit_Serial_P2->setText(QString::number(Serial_P2Quint32, 16));
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 2 Serial_P2 ")
//        //                                           + QString::number(Serial_P2Quint32, 16));
//                      }

//                      paramsVector.append(Serial_P2Array);
                      mapRead["Serial_P2ByteArray"] = Serial_P2Array;

                      //
                      //ManCode P2
                      QByteArray ManCode_P2Array;
                      ManCode_P2Array.append(buffer[24]);
                      ManCode_P2Array.append(buffer[25]);
                      qDebug()<<"ManCode_P2Array.toHex() "<<ManCode_P2Array.toHex();

                      quint16 ManCode_P2Quint16 = (quint8)buffer[24];
                      ManCode_P2Quint16 = ManCode_P2Quint16<<8 | (quint8)buffer[25];

//                      if(ManCode_P2Quint16 == 0x0000) {
//                          ui->lineEdit_ManCode_P2->setText(QString::number(ManCode_P2Quint16, 16));
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 2 ManCode_P2 ")
//        //                                           + QString::number(ManCode_P2Quint16, 16));
//                      }

//                      paramsVector.append(ManCode_P2Array);
                      mapRead["ManCode_P2ByteArray"] = ManCode_P2Array;

                      //
                      //DevType P2
                      QByteArray DevType_P2Array;
                      DevType_P2Array.append(buffer[26]);
                      qDebug()<<"DevType_P2Array.toHex() "<<DevType_P2Array.toHex();

                      quint8 DevType_P2Int = (quint8)buffer[26];

//                      if(DevType_P2Int == 0x00) {
//                          ui->lineEdit_DevType_P2->setText(QString::number(DevType_P2Int, 16));
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 2 DevType_P2 ") +
//        //                                           QString::number(DevType_P2Int, 16));
//                      }

//                      paramsVector.append(DevType_P2Array);
                      mapRead["DevType_P2ByteArray"] = DevType_P2Array;

                      //
                      //Version P2
                      QByteArray Version_P2Array;
                      Version_P2Array.append(buffer[27]);
                      qDebug()<<"Version_P2Array.toHex() "<<Version_P2Array.toHex();

                      quint8 Version_P2Int = (quint8)buffer[27];

//                      if(Version_P2Int == 0) {
//                          ui->lineEdit_Version_P2->setText(QString::number(Version_P2Int, 10));
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 2 Version_P2 ") +
//        //                                           QString::number(Version_P2Int, 10));
//                      }

//                      paramsVector.append(Version_P2Array);
                      mapRead["Version_P2ByteArray"] = Version_P2Array;

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

//                      if(Initial_value_P2Int == 0) {
//                          ui->lineEdit_Initial_value_P2->setText(QString::number(Initial_value_P2Int));
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 2 Initial_value_P2 ") +
//        //                                           QString::number(Initial_value_P2Int));
//                      }

//                      paramsVector.append(Initial_value_P2Array);
                      mapRead["Initial_value_P2ByteArray"] = Initial_value_P2Array;

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

//                      if(Service_P2Int == 0x00000000) {
//                          ui->lineEdit_Service_P2->setText(QString::number(Service_P2Int, 16));
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Параметр счётчика, подключенного к счётному входу 2 Service_P2 ") +
//        //                                           QString::number(Service_P2Int, 16));
//                      }

//                      paramsVector.append(Service_P2Array);
                      mapRead["Service_P2ByteArray"] = Service_P2Array;

                      //
                      //





                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка данных") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
    //                      portTmp->close();
                          label_StatusBar = (tr("Не удалось прочитать данные") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
//                          ui->label_writeParams->setVisible(true);
//                          vectorIsErrorOccured[workPlaceNumber] = true;
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }



               }


            }

            //
            //Read External Device Settings/


            //
            //Read RF Settings Request	5A	00	05	18	CRC

                       //            Answer	5A	00	0A	98	NB_Fi_TxCh	NB_Fi_RxCh	NB_Fi_RFBand	NB_Fi_TxPwr	NB_Fi_RFPwr	CRC
                       //                    0	1	2	3	4	        5	        6	            7	        8	        9
                       //      Sync	Empty	Length	Cmd	    1	        1	        1	            1	        1

            emit textBrowser("Read RF Settings");

            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x05;
               packetToRead.append(byte);
               byte = 0x18;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
     //              QMessageBox::information(this, "", tr("Данные в порт не записаны") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
//                   ui->label_writeParams->setVisible(true);
//                   vectorIsErrorOccured[workPlaceNumber] = true;
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               global::pause(150);//(200);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось прочитать данные") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
    //                   portTmp->close();
                       label_StatusBar = (tr("Не удалось прочитать данные") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
//                       ui->label_writeParams->setVisible(true);
//                       vectorIsErrorOccured[workPlaceNumber] = true;
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x0a;
                   bytesForChecking.append(byte);
                   byte = 0x98;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

             //      quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3]) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);
        //              emit textBrowser("buffer.toHex()     " + buffer.toHex());

        //              emit sendbufferReadRealClock(buffer);

                      //

                      //NB_Fi_TxCh
                      QByteArray NB_Fi_TxChArray;
                      NB_Fi_TxChArray.append(buffer[4]);

                      mapRead["NB_Fi_TxCh"] = NB_Fi_TxChArray;

                      //
                      //NB_Fi_RxCh
                      QByteArray NB_Fi_RxChArray;
                      NB_Fi_RxChArray.append(buffer[5]);

                      mapRead["NB_Fi_RxCh"] = NB_Fi_RxChArray;

                      //
                      //NB_Fi_RFBand
                      QByteArray NB_Fi_RFBandArray;
                      NB_Fi_RFBandArray.append(buffer[6]);

                      mapRead["NB_Fi_RFBand"] = NB_Fi_RFBandArray;

                      //
                      //NB_Fi_TxPwr
                      QByteArray NB_Fi_TxPwrArray;
                      NB_Fi_TxPwrArray.append(buffer[7]);

                      mapRead["NB_Fi_TxPwr"] = NB_Fi_TxPwrArray;

                      //
                      //NB_Fi_RFPwr
                      QByteArray NB_Fi_RFPwrArray;
                      NB_Fi_RFPwrArray.append(buffer[8]);

                      mapRead["NB_Fi_RFPwr"] = NB_Fi_RFPwrArray;

                      //
                      //



                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка данных"));
    //                      portTmp->close();
                          label_StatusBar = (tr("Ошибка данных") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
//                          ui->label_writeParams->setVisible(true);
//                          vectorIsErrorOccured[workPlaceNumber] = true;
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }



               }


            }



            //Read RF Settings/
            //


            //
            //Read NB-FI Settings   5A	00	05	19	CRC

//                                  5A	00	13	99	NB_Fi_Mode	NB_Fi_HdSh	NB_Fi_MAC	NB_Fi_RetNum	NB_Fi_PldLen	NB_Fi_HrbtNum	NB_Fi_HrbtInt	NB_Fi_InfoInt	CRC
//                                  0	 1	2	3	4	         5	        6	            7	            8	            9	        10…13	        14…17	         18
//            Sync	Empty	Length	Cmd	            1	         1	        1	            1	            1	            1	        4	            4




            emit textBrowser("Read NB-FI Settings");

            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x05;
               packetToRead.append(byte);
               byte = 0x19;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
     //              QMessageBox::information(this, "", tr("Данные в порт не записаны") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
//                   ui->label_writeParams->setVisible(true);
//                   vectorIsErrorOccured[workPlaceNumber] = true;
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               global::pause(150);//(200);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось прочитать данные") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
    //                   portTmp->close();
                       label_StatusBar = (tr("Не удалось прочитать данные") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
//                       ui->label_writeParams->setVisible(true);
//                       vectorIsErrorOccured[workPlaceNumber] = true;
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x13;
                   bytesForChecking.append(byte);
                   byte = 0x99;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

             //      quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3]) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);
        //              emit textBrowser("buffer.toHex()     " + buffer.toHex());

        //              emit sendbufferReadRealClock(buffer);

                      //

                      //NB_Fi_Mode
                      QByteArray NB_Fi_ModeArray;
                      NB_Fi_ModeArray.append(buffer[4]);

                      mapRead["NB_Fi_Mode"] = NB_Fi_ModeArray;

                      //
                      //NB_Fi_HdSh
                      QByteArray NB_Fi_HdShArray;
                      NB_Fi_HdShArray.append(buffer[5]);

                      mapRead["NB_Fi_HdSh"] = NB_Fi_HdShArray;

                      //
                      //NB_Fi_MAC
                      QByteArray NB_Fi_MACArray;
                      NB_Fi_MACArray.append(buffer[6]);

                      mapRead["NB_Fi_MAC"] = NB_Fi_MACArray;

                      //
                      //NB_Fi_RetNum
                      QByteArray NB_Fi_RetNumArray;
                      NB_Fi_RetNumArray.append(buffer[7]);

                      mapRead["NB_Fi_RetNum"] = NB_Fi_RetNumArray;

                      //
                      //NB_Fi_PldLen
                      QByteArray NB_Fi_PldLenArray;
                      NB_Fi_PldLenArray.append(buffer[8]);

                      mapRead["NB_Fi_PldLen"] = NB_Fi_PldLenArray;

                      //
                      //NB_Fi_HrbtNum
                      QByteArray NB_Fi_HrbtNumArray;
                      NB_Fi_HrbtNumArray.append(buffer[9]);

                      mapRead["NB_Fi_HrbtNum"] = NB_Fi_HrbtNumArray;

                      //
                      //NB_Fi_HrbtInt 4 bytes
                      QByteArray NB_Fi_HrbtIntArray;
                      NB_Fi_HrbtIntArray.append(buffer[10]);
                      NB_Fi_HrbtIntArray.append(buffer[11]);
                      NB_Fi_HrbtIntArray.append(buffer[12]);
                      NB_Fi_HrbtIntArray.append(buffer[13]);

                      mapRead["NB_Fi_HrbtInt"] = NB_Fi_HrbtIntArray;

                      //
                      //NB_Fi_InfoInt 4 bytes
                      QByteArray NB_Fi_InfoIntArray;
                      NB_Fi_InfoIntArray.append(buffer[14]);
                      NB_Fi_InfoIntArray.append(buffer[15]);
                      NB_Fi_InfoIntArray.append(buffer[16]);
                      NB_Fi_InfoIntArray.append(buffer[17]);

                      mapRead["NB_Fi_InfoInt"] = NB_Fi_InfoIntArray;


                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка данных"));
    //                      portTmp->close();
                          label_StatusBar = (tr("Ошибка данных") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
//                          ui->label_writeParams->setVisible(true);
//                          vectorIsErrorOccured[workPlaceNumber] = true;
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }



               }


            }


            //Read NB-FI Settings/
            //


            //
            //Read Identificators   5A	00	05	15	CRC

//                         Answer	5A	00	2D	95	NB_Fi_BrdID	NB_Fi_MdmID	NB_Fi_Key	CRC
//                                   0	1	2	3	4…7	        8…11	    12…43	     44
//                Sync	Empty	Length	Cmd	        4	        4	        32




            emit textBrowser("Read Identificators");

            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x05;
               packetToRead.append(byte);
               byte = 0x15;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
     //              QMessageBox::information(this, "", tr("Данные в порт не записаны") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
//                   ui->label_writeParams->setVisible(true);
//                   vectorIsErrorOccured[workPlaceNumber] = true;
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               global::pause(150);//(200);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось прочитать данные") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
    //                   portTmp->close();
                       label_StatusBar = (tr("Не удалось прочитать данные") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
//                       ui->label_writeParams->setVisible(true);
//                       vectorIsErrorOccured[workPlaceNumber] = true;
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x2d;
                   bytesForChecking.append(byte);
                   byte = 0x95;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

             //      quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3]) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);
        //              emit textBrowser("buffer.toHex()     " + buffer.toHex());

        //              emit sendbufferReadRealClock(buffer);

                      //

                      //NB_Fi_BrdID 4 bytes
                      QByteArray NB_Fi_BrdIDArray;
                      NB_Fi_BrdIDArray.append(buffer[4]);
                      NB_Fi_BrdIDArray.append(buffer[5]);
                      NB_Fi_BrdIDArray.append(buffer[6]);
                      NB_Fi_BrdIDArray.append(buffer[7]);

                      mapRead["NB_Fi_BrdID"] = NB_Fi_BrdIDArray;

                      //
                      //NB_Fi_MdmID 4 bytes
                      QByteArray NB_Fi_MdmIDArray;
                      NB_Fi_MdmIDArray.append(buffer[8]);
                      NB_Fi_MdmIDArray.append(buffer[9]);
                      NB_Fi_MdmIDArray.append(buffer[10]);
                      NB_Fi_MdmIDArray.append(buffer[11]);

                      mapRead["NB_Fi_MdmID"] = NB_Fi_MdmIDArray;

                      //
                      //NB_Fi_Key 32 bytes
                      QByteArray NB_Fi_KeyArray;
                      NB_Fi_KeyArray.append(buffer[12]);
                      NB_Fi_KeyArray.append(buffer[13]);
                      NB_Fi_KeyArray.append(buffer[14]);
                      NB_Fi_KeyArray.append(buffer[15]);
                      NB_Fi_KeyArray.append(buffer[16]);
                      NB_Fi_KeyArray.append(buffer[17]);
                      NB_Fi_KeyArray.append(buffer[18]);
                      NB_Fi_KeyArray.append(buffer[19]);
                      NB_Fi_KeyArray.append(buffer[20]);
                      NB_Fi_KeyArray.append(buffer[21]);
                      NB_Fi_KeyArray.append(buffer[22]);
                      NB_Fi_KeyArray.append(buffer[23]);
                      NB_Fi_KeyArray.append(buffer[24]);
                      NB_Fi_KeyArray.append(buffer[25]);
                      NB_Fi_KeyArray.append(buffer[26]);
                      NB_Fi_KeyArray.append(buffer[27]);
                      NB_Fi_KeyArray.append(buffer[28]);
                      NB_Fi_KeyArray.append(buffer[29]);
                      NB_Fi_KeyArray.append(buffer[30]);
                      NB_Fi_KeyArray.append(buffer[31]);
                      NB_Fi_KeyArray.append(buffer[32]);
                      NB_Fi_KeyArray.append(buffer[33]);
                      NB_Fi_KeyArray.append(buffer[34]);
                      NB_Fi_KeyArray.append(buffer[35]);
                      NB_Fi_KeyArray.append(buffer[36]);
                      NB_Fi_KeyArray.append(buffer[37]);
                      NB_Fi_KeyArray.append(buffer[38]);
                      NB_Fi_KeyArray.append(buffer[39]);
                      NB_Fi_KeyArray.append(buffer[40]);
                      NB_Fi_KeyArray.append(buffer[41]);
                      NB_Fi_KeyArray.append(buffer[42]);
                      NB_Fi_KeyArray.append(buffer[43]);

                      mapRead["NB_Fi_Key"] = NB_Fi_KeyArray;

                      //


                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка данных"));
    //                      portTmp->close();
                          label_StatusBar = (tr("Ошибка данных") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
//                          ui->label_writeParams->setVisible(true);
//                          vectorIsErrorOccured[workPlaceNumber] = true;
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }



               }


            }


            //Read Identificators/
            //


            //
            //Read Applycation Layer    5A  00  05	14	CRC

//                              Answer	5A	00	09	94	W5_Trans	W7_Trans	NB_Fi_UsrPktInt	CRC
//                                      0	1	2	3	4	        5	        6, 7	        8
//                Sync	Empty	Length	Cmd	            1	        1	        2



            emit textBrowser("Read Identificators");

            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x05;
               packetToRead.append(byte);
               byte = 0x14;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
     //              QMessageBox::information(this, "", tr("Данные в порт не записаны") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
//                   ui->label_writeParams->setVisible(true);
//                   vectorIsErrorOccured[workPlaceNumber] = true;
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               global::pause(150);//(200);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось прочитать данные") + ". Рабочее место: " + QString::number(workPlaceNumber + 1));
    //                   portTmp->close();
                       label_StatusBar = (tr("Не удалось прочитать данные") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
//                       ui->label_writeParams->setVisible(true);
//                       vectorIsErrorOccured[workPlaceNumber] = true;
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x09;
                   bytesForChecking.append(byte);
                   byte = 0x94;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

             //      quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3]) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);
        //              emit textBrowser("buffer.toHex()     " + buffer.toHex());

        //              emit sendbufferReadRealClock(buffer);

                      //

                      //W5_Trans 4 bytes
                      QByteArray W5_TransArray;
                      W5_TransArray.append(buffer[4]);

                      mapRead["W5_Trans"] = W5_TransArray;

                      //
                      //W7_Trans 4 bytes
                      QByteArray W7_TransArray;
                      W7_TransArray.append(buffer[5]);

                      mapRead["W7_Trans"] = W7_TransArray;

                      //
                      //NB_Fi_UsrPktInt_WAL 32 bytes
                      QByteArray NB_Fi_UsrPktInt_WALArray;
                      NB_Fi_UsrPktInt_WALArray.append(buffer[6]);
                      NB_Fi_UsrPktInt_WALArray.append(buffer[7]);

                      mapRead["NB_Fi_UsrPktInt_WAL"] = NB_Fi_UsrPktInt_WALArray;

                      //


                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка данных"));
    //                      portTmp->close();
                          label_StatusBar = (tr("Ошибка данных") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
//                          ui->label_writeParams->setVisible(true);
//                          vectorIsErrorOccured[workPlaceNumber] = true;
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }



               }


            }

            //Read Applycation Layer/
            //


            emit checkWritingError(currentIndicatorNumber);


}

void ObjectThread::slotWriteParams(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4)
{
    qDebug()<<"ObjectThread::slotWriteParams() "<<"workPlace "<<QString::number(workPlace);

        QSerialPort *portTmp;

        bool isNeedPaintEvent = true;//false;

        int workPlaceNumber = workPlace;

        if(!isWorkPlaceUse.at(workPlace)) return;

        int currentIndicatorNumber = workPlaceNumber;

        QString label_StatusBar;
        QString errorString;

        switch (workPlaceNumber) {
        case 0:
            portTmp = port1;
            break;
        case 1:
            portTmp = port2;
            break;
        case 2:
            portTmp = port3;
            break;
        case 3:
            portTmp = port4;
            break;

        default:
            break;
        }

        portTmp->setBaudRate(QSerialPort::Baud19200);
        portTmp->setDataBits(QSerialPort::Data8);
        portTmp->setParity(QSerialPort::NoParity);
        portTmp->setStopBits(QSerialPort::OneStop);


        int currentBoxNumber = 0;
  //      vectorIsErrorOccured[workPlaceNumber] = false;

        //


            bool ok;
            QByteArray ProtTypeByteArray;
            ProtTypeByteArray.clear();
            quint8 byteProtType = 0xff;
            ProtTypeByteArray.append(byteProtType);

//            ui->label_writeParams->setVisible(false);
//            ui->label_Calibration->setVisible(false);
//            ui->label_pulsesOutputHeat->setVisible(false);
//            ui->label_pulsesInputVolume->setVisible(false);
//            ui->label_pulsesOutputOff->setVisible(false);
//            ui->label_MBusOn->setVisible(false);
//            ui->label_MBusOff->setVisible(false);
//            ui->label_CheckMbus->setVisible(false);
//            ui->label_digitalWireInterfaceChecking->setVisible(false);
//            ui->label_RMV_ErrorFlags_tick->setVisible(false);
//            ui->label_calibrationModeOff->setVisible(false);
//            ui->label_realClockCalibration->setVisible(false);

//            ui->lineEdit_RMV_ErrorFlags->setText("");

            //
            //индикатор
 //           QColor color(255, 127, 50);
//            ui->label_writeParams->setStyleSheet(QString("color: red").arg(color.name()));
//            ui->label_writeParams->setText("X");
//            ui->label_writeParams->setVisible(false);
            //
            //

  //          label_StatusBar = ("");

            //
            QByteArray pulsesMode1ByteArray;
            QByteArray pulsesMode2ByteArray;

            pulsesMode1ByteArray.clear();
            pulsesMode2ByteArray.clear();

            pulsesMode1ByteArray.append(paramsMapToThreads["pulsesMode1PrimalInt"].toInt()); //append(byte);
            pulsesMode2ByteArray.append(paramsMapToThreads["pulsesMode2PrimalInt"].toInt()); //append(byte);
            //
            //
            if(paramsMapToThreads["pulsesValue1PrimeInt == 0"].toInt()) {
      //          QMessageBox::information(this, "", tr("Недопустимое значение pulsesvalue1 ") + QString::number(pulsesValue1PrimeInt));
                label_StatusBar = (tr("Недопустимое значение pulsesvalue1 ") + QString::number(paramsMapToThreads["pulsesValue1PrimeInt"].toInt()) +
                                             ". Рабочее место: " + QString::number(workPlaceNumber+1));

                emit errorStringSignal(label_StatusBar + '\n');
                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                emit workPlaceOff(currentIndicatorNumber);
                emit checkWritingError(currentIndicatorNumber);

                return;
            }
            if(paramsMapToThreads["pulsesValue2PrimeInt == 0"].toInt()) {
     //           QMessageBox::information(this, "", tr("Недопустимое значение pulsesvalue2 ") + QString::number(pulsesValue2PrimeInt));
                label_StatusBar = tr("Недопустимое значение pulsesvalue2 ") + QString::number(paramsMapToThreads["pulsesValue2PrimeInt"].toInt()) +
                                             ". Рабочее место: " + QString::number(workPlaceNumber+1);
                emit errorStringSignal(label_StatusBar + '\n');
                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                emit workPlaceOff(currentIndicatorNumber);
                emit checkWritingError(currentIndicatorNumber);

                return;
            }

            QByteArray pulsesValue1ByteArray;
            QByteArray pulsesValue2ByteArray;

            pulsesValue1ByteArray.clear();
            pulsesValue2ByteArray.clear();
            pulsesValue1ByteArray.append((quint8)(paramsMapToThreads["pulsesValue1PrimeInt"].toInt()     >>8));
            pulsesValue1ByteArray.append((quint8)(paramsMapToThreads["pulsesValue1PrimeInt"].toInt()));

            pulsesValue2ByteArray.append((quint8)(paramsMapToThreads["pulsesValue2PrimeInt"].toInt()     >>8));
            pulsesValue2ByteArray.append((quint8)(paramsMapToThreads["pulsesValue2PrimeInt"].toInt()));
            //

            bool isWritingCorrect = false;

  //          paramsVectorToWrite.clear();

            QByteArray buffer;

            if(!portTmp->isOpen()) {
                if(!portTmp->open(QIODevice::ReadWrite)) {
    //                QMessageBox::information(this, "", "Не удалось открыть порт УСО-2. Рабочее место: " +
    //                                         QString::number(workPlaceNumber + 1) );
                    label_StatusBar = ("Не удалось открыть порт УСО-2. Рабочее место: " +
                                                 QString::number(workPlaceNumber + 1));
                    emit errorStringSignal(label_StatusBar + '\n');
                    vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                    emit workPlaceOff(currentIndicatorNumber);
                    emit checkWritingError(currentIndicatorNumber);
//                    checkWritingError(currentIndicatorNumber);
//                    emit workPlaceOff(currentIndicatorNumber);
                    return;
                }
            }


            //
            //Set Password Default 5a 00 07 42 23 17
            for(int i=0; i<13;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x07;
               packetToRead.append(byte);
               byte = 0x42;
               packetToRead.append(byte);
               byte = 0x23;
               packetToRead.append(byte);
               byte = 0x17;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
    //                                                 ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);
//                   checkWritingError(currentIndicatorNumber);
//                   emit workPlaceOff(currentIndicatorNumber);

                   return;
               }

               QApplication::processEvents();
               global::pause(300);


               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==12) {
    //                   QMessageBox::information(this, "", tr("Ошибка установки пароля по умолчанию") +
    //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = tr("Ошибка установки пароля по умолчанию") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1);
                       emit errorStringSignal(label_StatusBar + '\n');
  //                     ui->label_writeParams->setVisible(true);
  //                     vectorIsErrorOccured[workPlaceNumber] = true;
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);
//                       checkWritingError(currentIndicatorNumber);
//                       emit workPlaceOff(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xc2;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 12) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);



                      //
                      //



                      break;
                   }
                   else {
                            if(i==12) {
    //                            QMessageBox::information(this, "", tr("Ошибка установки пароля по умолчанию") +
    //                                                     "Рабочее место: " + QString::number(workPlaceNumber+1));
                                label_StatusBar = (tr("Ошибка установки пароля по умолчанию") +
                                                             ". Рабочее место: " + QString::number(workPlaceNumber+1));
                                emit errorStringSignal(label_StatusBar + '\n');
   //                             ui->label_writeParams->setVisible(true);
   //                             vectorIsErrorOccured[workPlaceNumber] = true;
                                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
//                                checkWritingError(currentIndicatorNumber);
//                                emit workPlaceOff(currentIndicatorNumber);

                                emit workPlaceOff(currentIndicatorNumber);
                                emit checkWritingError(currentIndicatorNumber);

                                return;
                            }

                   }



               }


            }

            //
            //Set Password Default 5a 00 07 42 23 17/


            //
            //Открыть парольную сессию 5a 00 08 40 01 04 57
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x08;
               packetToRead.append(byte);
               byte = 0x40;
               packetToRead.append(byte);
               byte = 0x01;
               packetToRead.append(byte);
               byte = 0x04;
               packetToRead.append(byte);
               byte = 0x57;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
    //                                                 "Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
//                   checkWritingError(currentIndicatorNumber);
//                   emit workPlaceOff(currentIndicatorNumber);
                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               QApplication::processEvents();
               global::pause(150);//(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") +
    //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Ошибка открытия парольной сессии") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
 //                      ui->label_writeParams->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
//                       checkWritingError(currentIndicatorNumber);
//                       emit workPlaceOff(currentIndicatorNumber);
                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xc0;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);


                      //
                      //



                      break;
                   }
                   else {
                            if(i==2) {
    //                            QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") +
    //                                                     "Рабочее место: " + QString::number(workPlaceNumber+1));
                                label_StatusBar = (tr("Ошибка открытия парольной сессии") +
                                                             ". Рабочее место: " + QString::number(workPlaceNumber+1));
                                emit errorStringSignal(label_StatusBar + '\n');
    //                            ui->label_writeParams->setVisible(true);
                                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
//                                checkWritingError(currentIndicatorNumber);
                                emit workPlaceOff(currentIndicatorNumber);
                                emit checkWritingError(currentIndicatorNumber);

                                return;
                            }

                   }



               }


            }

            //
            //Открыть парольную сессию 5a 00 08 40 01 04 57/


            //
            //Write Hardware Configuration 5a 00 15 29 serial(4) manCode(2) devType(1) DN(1)
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x15;
               packetToRead.append(byte);
               byte = 0x29;
               packetToRead.append(byte);

               //
               switch (workPlaceNumber) {
               case 0:
                    byte = paramsMapToThreads["serialNumber"].toByteArray()[0];
                    packetToRead.append(byte);
                    byte = paramsMapToThreads["serialNumber"].toByteArray()[1];
                    packetToRead.append(byte);
                    byte = paramsMapToThreads["serialNumber"].toByteArray()[2];
                    packetToRead.append(byte);
                    byte = paramsMapToThreads["serialNumber"].toByteArray()[3];
                    packetToRead.append(byte);
           //         paramsVectorToWrite.append(serialNumber);
                    mapwrite["serialNumber"] = paramsMapToThreads["serialNumber"].toByteArray();
                   break;
               case 1:
                    byte = paramsMapToThreads["serialNumber2"].toByteArray()[0];
                    packetToRead.append(byte);
                    byte = paramsMapToThreads["serialNumber2"].toByteArray()[1];
                    packetToRead.append(byte);
                    byte = paramsMapToThreads["serialNumber2"].toByteArray()[2];
                    packetToRead.append(byte);
                    byte = paramsMapToThreads["serialNumber2"].toByteArray()[3];
                    packetToRead.append(byte);
            //        paramsVectorToWrite.append(serialNumber2);
                    mapwrite["serialNumber2"] = paramsMapToThreads["serialNumber2"].toByteArray();
                   break;
               case 2:
                    byte = paramsMapToThreads["serialNumber3"].toByteArray()[0];
                    packetToRead.append(byte);
                    byte = paramsMapToThreads["serialNumber3"].toByteArray()[1];
                    packetToRead.append(byte);
                    byte = paramsMapToThreads["serialNumber3"].toByteArray()[2];
                    packetToRead.append(byte);
                    byte = paramsMapToThreads["serialNumber3"].toByteArray()[3];
                    packetToRead.append(byte);
  //                  paramsVectorToWrite.append(serialNumber3);
                    mapwrite["serialNumber3"] = paramsMapToThreads["serialNumber3"].toByteArray();
                   break;
               case 3:
                    byte = paramsMapToThreads["serialNumber4"].toByteArray()[0];
                    packetToRead.append(byte);
                    byte = paramsMapToThreads["serialNumber4"].toByteArray()[1];
                    packetToRead.append(byte);
                    byte = paramsMapToThreads["serialNumber4"].toByteArray()[2];
                    packetToRead.append(byte);
                    byte = paramsMapToThreads["serialNumber4"].toByteArray()[3];
                    packetToRead.append(byte);
  //                  paramsVectorToWrite.append(serialNumber4);
                    mapwrite["serialNumber4"] = paramsMapToThreads["serialNumber4"].toByteArray();
                   break;
               default:
                   break;
               }
               //


               byte = paramsMapToThreads["manCode"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["manCode"].toByteArray()[1];
               packetToRead.append(byte);
  //             paramsVectorToWrite.append(manCode);
               mapwrite["manCode"] = paramsMapToThreads["manCode"].toByteArray();


               byte = paramsMapToThreads["devType"].toByteArray()[0];
               packetToRead.append(byte);
//               paramsVectorToWrite.append(devType);
               mapwrite["devType"] = paramsMapToThreads["devType"].toByteArray();


               byte = paramsMapToThreads["DN"].toByteArray()[0];
               packetToRead.append(byte);
 //              paramsVectorToWrite.append(DN);
               mapwrite["DN"] = paramsMapToThreads["DN"].toByteArray();


               //
               switch (workPlaceNumber) {
               case 0:
                   byte = paramsMapToThreads["PCB_SN_ByteArray"].toByteArray()[0];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray"].toByteArray()[1];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray"].toByteArray()[2];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray"].toByteArray()[3];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray"].toByteArray()[4];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray"].toByteArray()[5];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray"].toByteArray()[6];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray"].toByteArray()[7];
                   packetToRead.append(byte);
  //                 paramsVectorToWrite.append(PCB_SN_ByteArray);
                   mapwrite["PCB_SN_ByteArray"] = paramsMapToThreads["PCB_SN_ByteArray"].toByteArray();
                  break;
               case 1:
                   byte = paramsMapToThreads["PCB_SN_ByteArray2"].toByteArray()[0];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray2"].toByteArray()[1];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray2"].toByteArray()[2];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray2"].toByteArray()[3];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray2"].toByteArray()[4];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray2"].toByteArray()[5];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray2"].toByteArray()[6];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray2"].toByteArray()[7];
                   packetToRead.append(byte);
     //              paramsVectorToWrite.append(PCB_SN_ByteArray2);
                   mapwrite["PCB_SN_ByteArray2"] = paramsMapToThreads["PCB_SN_ByteArray2"].toByteArray();
                  break;
               case 2:
                   byte = paramsMapToThreads["PCB_SN_ByteArray3"].toByteArray()[0];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray3"].toByteArray()[1];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray3"].toByteArray()[2];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray3"].toByteArray()[3];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray3"].toByteArray()[4];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray3"].toByteArray()[5];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray3"].toByteArray()[6];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray3"].toByteArray()[7];
                   packetToRead.append(byte);
   //                paramsVectorToWrite.append(PCB_SN_ByteArray3);
                   mapwrite["PCB_SN_ByteArray3"] = paramsMapToThreads["PCB_SN_ByteArray3"].toByteArray();
                  break;
               case 3:
                   byte = paramsMapToThreads["PCB_SN_ByteArray4"].toByteArray()[0];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray4"].toByteArray()[1];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray4"].toByteArray()[2];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray4"].toByteArray()[3];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray4"].toByteArray()[4];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray4"].toByteArray()[5];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray4"].toByteArray()[6];
                   packetToRead.append(byte);
                   byte = paramsMapToThreads["PCB_SN_ByteArray4"].toByteArray()[7];
                   packetToRead.append(byte);
    //               paramsVectorToWrite.append(PCB_SN_ByteArray4);
                   mapwrite["PCB_SN_ByteArray4"] = paramsMapToThreads["PCB_SN_ByteArray4"].toByteArray();
                  break;
               default:
                  break;
               }


               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
    //                                                 ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
//                   checkWritingError(currentIndicatorNumber);
                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               QApplication::processEvents();
               global::pause(250);//(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось записать данные : Hardware Configuration") +
    //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Не удалось записать данные : Hardware Configuration ") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
  //                     ui->label_writeParams->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
//                       checkWritingError(currentIndicatorNumber);
                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xa9;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer[4];

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                      //


                      //
                      //



                      break;
                   }
                   else {
                             if(i==2) {
    //                             QMessageBox::information(this, "", tr("Ошибка записи данных : Hardware Configuration ") +
    //                                                      "Рабочее место: " + QString::number(workPlaceNumber+1));
                                 label_StatusBar = (tr("Ошибка записи данных : Hardware Configuration ") +
                                                              ". Рабочее место: " + QString::number(workPlaceNumber+1));
                                 emit errorStringSignal(label_StatusBar + '\n');
   //                              ui->label_writeParams->setVisible(true);
                                 vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
//                                 checkWritingError(currentIndicatorNumber);
                                 emit workPlaceOff(currentIndicatorNumber);
                                 emit checkWritingError(currentIndicatorNumber);

                                 return;
                             }

                   }



               }


            }

            //
            //Write Software Configuration 5a 00 16 20
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x16;
               packetToRead.append(byte);
               byte = 0x20;
               packetToRead.append(byte);

               byte = paramsMapToThreads["flowMinByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowMinByteArray"].toByteArray()[1];
               packetToRead.append(byte);
  //             paramsVectorToWrite.append(flowMinByteArray);
               mapwrite["flowMinByteArray"] = paramsMapToThreads["flowMinByteArray"].toByteArray();


               byte = paramsMapToThreads["flowMaxByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowMaxByteArray"].toByteArray()[1];
               packetToRead.append(byte);
 //              paramsVectorToWrite.append(flowMaxByteArray);
               mapwrite["flowMaxByteArray"] = paramsMapToThreads["flowMaxByteArray"].toByteArray();


               byte = paramsMapToThreads["tinMinByteArray"].toByteArray()[0];
               packetToRead.append(byte);
//               paramsVectorToWrite.append(tinMinByteArray);
               mapwrite["tinMinByteArray"] = paramsMapToThreads["tinMinByteArray"].toByteArray();


               byte = paramsMapToThreads["tinMaxByteArray"].toByteArray()[0];
               packetToRead.append(byte);
//               paramsVectorToWrite.append(tinMaxByteArray);
               mapwrite["tinMaxByteArray"] = paramsMapToThreads["tinMaxByteArray"].toByteArray();


               byte = paramsMapToThreads["toutMinByteArray"].toByteArray()[0];
               packetToRead.append(byte);
  //             paramsVectorToWrite.append(toutMinByteArray);
               mapwrite["toutMinByteArray"] = paramsMapToThreads["toutMinByteArray"].toByteArray();


               byte = paramsMapToThreads["toutMaxByteArray"].toByteArray()[0];
               packetToRead.append(byte);
 //              paramsVectorToWrite.append(toutMaxByteArray);
               mapwrite["toutMaxByteArray"] = paramsMapToThreads["toutMaxByteArray"].toByteArray();


               byte = paramsMapToThreads["reportDataByteArray"].toByteArray()[0];
               packetToRead.append(byte);
  //             paramsVectorToWrite.append(reportDataByteArray);
               mapwrite["reportDataByteArray"] = paramsMapToThreads["reportDataByteArray"].toByteArray();


               byte = paramsMapToThreads["averageTByteArray"].toByteArray()[0];
               packetToRead.append(byte);
  //             paramsVectorToWrite.append(averageTByteArray);
               mapwrite["averageTByteArray"] = paramsMapToThreads["averageTByteArray"].toByteArray();


               byte = paramsMapToThreads["pulsesMode1ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
  //             paramsVectorToWrite.append(pulsesMode1ByteArray);
               mapwrite["pulsesMode1ByteArray"] = paramsMapToThreads["pulsesMode1ByteArray"].toByteArray();


               byte = paramsMapToThreads["pulsesMode2ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
 //              paramsVectorToWrite.append(pulsesMode2ByteArray);
               mapwrite["pulsesMode2ByteArray"] = paramsMapToThreads["pulsesMode2ByteArray"].toByteArray();


        //       quint16 pulseV1Int = (quint8)pulsesValue1ByteArray[0];
        //       pulseV1Int = pulseV1Int | (quint8)pulsesValue1ByteArray[1];
        //       if(pulseV1Int == 0) {
        //           QMessageBox::information(this, "", tr("Недопустимое для записи значение: pulsesValue1 ") + QString::number(pulseV1Int));
        //           return;
        //       }
               byte = paramsMapToThreads["pulsesValue1ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["pulsesValue1ByteArray"].toByteArray()[1];
               packetToRead.append(byte);
 //              paramsVectorToWrite.append(pulsesValue1ByteArray);
               mapwrite["pulsesValue1ByteArray"] = paramsMapToThreads["pulsesValue1ByteArray"].toByteArray();


        //       quint16 pulseV2Int = (quint8)pulsesValue2ByteArray[0];
        //       pulseV2Int = pulseV2Int | (quint8)pulsesValue2ByteArray[1];
        //       if(pulseV2Int == 0) {
        //           QMessageBox::information(this, "", tr("Недопустимое для записи значение: pulsesValue2 ") + QString::number(pulseV2Int));
        //           return;
        //       }
               byte = paramsMapToThreads["pulsesValue2ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["pulsesValue2ByteArray"].toByteArray()[1];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(pulsesValue2ByteArray);
               mapwrite["pulsesValue2ByteArray"] = paramsMapToThreads["pulsesValue2ByteArray"].toByteArray();

               byte = paramsMapToThreads["devType"].toByteArray()[0];
               packetToRead.append(byte);
   //            paramsVectorToWrite.append(devType);
               mapwrite["devType"] = paramsMapToThreads["devType"].toByteArray();


               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"porTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
    //                                                 "Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны ") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               QApplication::processEvents();
               global::pause(250);;//(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось записать данные : Software Configuration ") +
    //                                            ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Не удалось записать данные : Software Configuration ") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
    //                   ui->label_writeParams->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xa0;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer[4];

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                      //


                      //
                      //



                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка записи данных : Software Configuration ") +
    //                                               "Рабочее место: " + QString::number(workPlaceNumber+1));
                          label_StatusBar = (tr("Ошибка записи данных : Software Configuration ") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
       //                   ui->label_writeParams->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }



               }


            }

            //
            //
            //Write Sensor Parameters 5a 00 18 2d
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x18;
               packetToRead.append(byte);
               byte = 0x2d;
               packetToRead.append(byte);

               byte = paramsMapToThreads["flowRateByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowRateByteArray"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowRateByteArray"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowRateByteArray"].toByteArray()[3];
               packetToRead.append(byte);
        //       paramsVectorToWrite.append(flowRateByteArray);
               mapwrite["flowRateByteArray"] = paramsMapToThreads["flowRateByteArray"].toByteArray();


               byte = paramsMapToThreads["pressureByteArray"].toByteArray()[0];
               packetToRead.append(byte);
        //       paramsVectorToWrite.append(pressureByteArray);
               mapwrite["pressureByteArray"] = paramsMapToThreads["pressureByteArray"].toByteArray();


               byte = paramsMapToThreads["RTD_TypeByteArray"].toByteArray()[0];
               packetToRead.append(byte);
       //        paramsVectorToWrite.append(RTD_TypeByteArray);
               mapwrite["RTD_TypeByteArray"] = paramsMapToThreads["RTD_TypeByteArray"].toByteArray();


               byte = paramsMapToThreads["R_RTDByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["R_RTDByteArray"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["R_RTDByteArray"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["R_RTDByteArray"].toByteArray()[3];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(R_RTDByteArray);

               quint32 r_rtdInt = (quint8)paramsMapToThreads["R_RTDByteArray"].toByteArray()[0];
               r_rtdInt = r_rtdInt<<8 | (quint8)paramsMapToThreads["R_RTDByteArray"].toByteArray()[1];
               r_rtdInt = r_rtdInt<<8 | (quint8)paramsMapToThreads["R_RTDByteArray"].toByteArray()[2];
               r_rtdInt = r_rtdInt<<8 | (quint8)paramsMapToThreads["R_RTDByteArray"].toByteArray()[3];

               r_rtdInt = r_rtdInt/1000;

               QByteArray r_rtd_NotMultiplicationByteArray;
               r_rtd_NotMultiplicationByteArray.append((quint8)(r_rtdInt>>24));
               r_rtd_NotMultiplicationByteArray.append((quint8)(r_rtdInt>>16));
               r_rtd_NotMultiplicationByteArray.append((quint8)(r_rtdInt>>8));
               r_rtd_NotMultiplicationByteArray.append((quint8)r_rtdInt);
               mapwrite["R_RTDByteArray"] = r_rtd_NotMultiplicationByteArray; //проверять нужно неумноженное значение


               byte = paramsMapToThreads["RWire1ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["RWire1ByteArray"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["RWire1ByteArray"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["RWire1ByteArray"].toByteArray()[3];
               packetToRead.append(byte);
          //     paramsVectorToWrite.append(RWire1ByteArray);
               mapwrite["RWire1ByteArray"] = paramsMapToThreads["RWire1ByteArray"].toByteArray();


               byte = paramsMapToThreads["RWire2ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["RWire2ByteArray"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["RWire2ByteArray"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["RWire2ByteArray"].toByteArray()[3];
               packetToRead.append(byte);
       //        paramsVectorToWrite.append(RWire2ByteArray);
               mapwrite["RWire2ByteArray"] = paramsMapToThreads["RWire2ByteArray"].toByteArray();


               byte = paramsMapToThreads["flowNomByteArray"].toByteArray()[0];
               packetToRead.append(byte);
        //       paramsVectorToWrite.append(flowNomByteArray);
               mapwrite["flowNomByteArray"] = paramsMapToThreads["flowNomByteArray"].toByteArray();


               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны ") +
    //                                                 "Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны ") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               QApplication::processEvents();
               global::pause(250);//(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось записать данные : Sensor Parameters ") +
    //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Не удалось записать данные : Sensor Parameters ") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
  //                     ui->label_writeParams->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xad;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer[4];

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                      //


                      //
                      //



                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка записи данных : Sensor Parameters ") +
    //                                               "Рабочее место: " + QString::number(workPlaceNumber+1));
                          label_StatusBar = (tr("Ошибка записи данных : Sensor Parameters ") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
//                          ui->label_writeParams->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }



               }


            }

            //
            //
            //Write Flow Coefficient 5a 00 1d 3d
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x1d;
               packetToRead.append(byte);
               byte = 0x3d;
               packetToRead.append(byte);

               byte = paramsMapToThreads["flowCoef0ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowCoef0ByteArray"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowCoef0ByteArray"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowCoef0ByteArray"].toByteArray()[3];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(flowCoef0ByteArray);
               mapwrite["flowCoef0ByteArray"] = paramsMapToThreads["flowCoef0ByteArray"].toByteArray();


               byte = paramsMapToThreads["flowCoef1ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowCoef1ByteArray"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowCoef1ByteArray"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowCoef1ByteArray"].toByteArray()[3];
               packetToRead.append(byte);
        //       paramsVectorToWrite.append(flowCoef1ByteArray);
               mapwrite["flowCoef1ByteArray"] = paramsMapToThreads["flowCoef1ByteArray"].toByteArray();


               byte = paramsMapToThreads["flowCoef2ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowCoef2ByteArray"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowCoef2ByteArray"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowCoef2ByteArray"].toByteArray()[3];
               packetToRead.append(byte);
        //       paramsVectorToWrite.append(flowCoef2ByteArray);
               mapwrite["flowCoef2ByteArray"] = paramsMapToThreads["flowCoef2ByteArray"].toByteArray();


               byte = paramsMapToThreads["flowCoef3ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowCoef3ByteArray"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowCoef3ByteArray"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowCoef3ByteArray"].toByteArray()[3];
               packetToRead.append(byte);
      //         paramsVectorToWrite.append(flowCoef3ByteArray);
               mapwrite["flowCoef3ByteArray"] = paramsMapToThreads["flowCoef3ByteArray"].toByteArray();


               byte = paramsMapToThreads["flowCoef4ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowCoef4ByteArray"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowCoef4ByteArray"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowCoef4ByteArray"].toByteArray()[3];
               packetToRead.append(byte);
       //        paramsVectorToWrite.append(flowCoef4ByteArray);
               mapwrite["flowCoef4ByteArray"] = paramsMapToThreads["flowCoef4ByteArray"].toByteArray();


               byte = paramsMapToThreads["flowCoef5ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowCoef5ByteArray"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowCoef5ByteArray"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowCoef5ByteArray"].toByteArray()[3];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(flowCoef5ByteArray);
               mapwrite["flowCoef5ByteArray"] = paramsMapToThreads["flowCoef5ByteArray"].toByteArray();


               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
    //                                                 "Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны ") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                   checkWritingError(currentIndicatorNumber);
                   emit workPlaceOff(currentIndicatorNumber);
                   return;
               }

               QApplication::processEvents();
               global::pause(250);//(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось записать данные : Flow Coefficient") +
    //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Не удалось записать данные : Flow Coefficient ") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
 //                      ui->label_writeParams->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xbd;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer[4];

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                      //


                      //
                      //



                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка записи данных : Flow Coefficient ") +
    //                                               "Рабочее место: " + QString::number(workPlaceNumber+1));
                          label_StatusBar = (tr("Ошибка записи данных : Flow Coefficient ") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
          //                ui->label_writeParams->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }



               }


            }

            //
            //
            //Write Differential Threshold 5a 00 07 3e
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x07;
               packetToRead.append(byte);
               byte = 0x3e;
               packetToRead.append(byte);

               byte = paramsMapToThreads["FS_DiffThrsByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["FS_DiffThrsByteArray"].toByteArray()[1];
               packetToRead.append(byte);
       //        paramsVectorToWrite.append(FS_DiffThrsByteArray);
               mapwrite["FS_DiffThrsByteArray"] = paramsMapToThreads["FS_DiffThrsByteArray"].toByteArray();


               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
    //                                                 "Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны ") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               QApplication::processEvents();
               global::pause(250);//(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось записать данные : Differential Threshold ") +
    //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Не удалось записать данные : Differential Threshold ") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
 //                      ui->label_writeParams->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xbe;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer[4];

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                      //


                      //
                      //



                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка записи данных : Differential Threshold ") +
    //                                               "Рабочее место: " + QString::number(workPlaceNumber+1));
                          label_StatusBar = (tr("Ошибка записи данных : Differential Threshold ") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
  //                        ui->label_writeParams->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }



               }


            }

            //
            //
            //Write Real Clock 5a 00 0d 22
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x0d;
               packetToRead.append(byte);
               byte = 0x22;
               packetToRead.append(byte);

               byte = paramsMapToThreads["hourByteArray"].toByteArray()[0];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(hourByteArray);
               mapwrite["hourByteArray"] = paramsMapToThreads["hourByteArray"].toByteArray();


               byte = paramsMapToThreads["minuteByteArray"].toByteArray()[0];
               packetToRead.append(byte);
      //         paramsVectorToWrite.append(minuteByteArray);
               mapwrite["minuteByteArray"] = paramsMapToThreads["minuteByteArray"].toByteArray();


               byte = paramsMapToThreads["secondByteArray"].toByteArray()[0];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(secondByteArray);
               mapwrite["secondByteArray"] = paramsMapToThreads["secondByteArray"].toByteArray();

               byte = paramsMapToThreads["dayByteArray"].toByteArray()[0];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(dayByteArray);
               mapwrite["dayByteArray"] = paramsMapToThreads["dayByteArray"].toByteArray();


               byte = paramsMapToThreads["monthByteArray"].toByteArray()[0];
               packetToRead.append(byte);
      //         paramsVectorToWrite.append(monthByteArray);
               mapwrite["monthByteArray"] = paramsMapToThreads["monthByteArray"].toByteArray();


               byte = paramsMapToThreads["yearByteArray"].toByteArray()[0];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(yearByteArray);
               mapwrite["yearByteArray"] = paramsMapToThreads["yearByteArray"].toByteArray();


               byte = paramsMapToThreads["DOWByteArray"].toByteArray()[0];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(DOWByteArray);
               mapwrite["DOWByteArray"] = paramsMapToThreads["DOWByteArray"].toByteArray();


               byte = paramsMapToThreads["ASW_flagByteArray"].toByteArray()[0];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(ASW_flagByteArray);
               mapwrite["ASW_flagByteArray"] = paramsMapToThreads["ASW_flagByteArray"].toByteArray();


               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны ") +
    //                                                 "Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны ") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               QApplication::processEvents();
               global::pause(250);//(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось записать данные : Real Clock ") +
    //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Не удалось записать данные : Real Clock ") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
   //                    ui->label_writeParams->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xa2;
                   bytesForChecking.append(byte);

                   quint8 error = buffer[4];

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                      //


                      //
                      //



                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка записи данных : Real Clock ") +
    //                                               "Рабочее место: " + QString::number(workPlaceNumber+1));
                          label_StatusBar = (tr("Ошибка записи данных : Real Clock ") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
   //                       ui->label_writeParams->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }



               }


            }

            //
            //
            //Write Temperature Calibration Coefficient 5a 00 11 36
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x11;
               packetToRead.append(byte);
               byte = 0x36;
               packetToRead.append(byte);

               byte = paramsMapToThreads["shift_T1ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["shift_T1ByteArray"].toByteArray()[1];
               packetToRead.append(byte);
   //            paramsVectorToWrite.append(shift_T1ByteArray);
               mapwrite["shift_T1ByteArray"] = paramsMapToThreads["shift_T1ByteArray"].toByteArray();


               byte = paramsMapToThreads["shift_T2ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["shift_T2ByteArray"].toByteArray()[1];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(shift_T2ByteArray);
               mapwrite["shift_T2ByteArray"] = paramsMapToThreads["shift_T2ByteArray"].toByteArray();


               byte = paramsMapToThreads["scale_T1ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["scale_T1ByteArray"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["scale_T1ByteArray"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["scale_T1ByteArray"].toByteArray()[3];
               packetToRead.append(byte);
   //            paramsVectorToWrite.append(scale_T1ByteArray);
               mapwrite["scale_T1ByteArray"] = paramsMapToThreads["scale_T1ByteArray"].toByteArray();


               byte = paramsMapToThreads["scale_T2ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["scale_T2ByteArray"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["scale_T2ByteArray"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["scale_T2ByteArray"].toByteArray()[3];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(scale_T2ByteArray);
               mapwrite["scale_T2ByteArray"] = paramsMapToThreads["scale_T2ByteArray"].toByteArray();


               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны ") +
    //                                                 "Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны ") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               QApplication::processEvents();
               global::pause(250);//(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось записать данные : Temperature Calibration Coefficient ") +
    //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Не удалось записать данные : Temperature Calibration Coefficient ") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
  //                     ui->label_writeParams->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xb6;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer[4];

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                      //


                      //
                      //



                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка записи данных: Temperature Calibration Coefficient") +
    //                                               "Рабочее место: " + QString::number(workPlaceNumber+1));
                          label_StatusBar = (tr("Ошибка записи данных: Temperature Calibration Coefficient ") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
    //                      ui->label_writeParams->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }



               }


            }

            //
            //
            //Write External Interface Settings 5a 00 0e 2b
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x0e;
               packetToRead.append(byte);
               byte = 0x2b;
               packetToRead.append(byte);

               byte = paramsMapToThreads["addrByteArray"].toByteArray()[0];
               packetToRead.append(byte);
       //        paramsVectorToWrite.append(addrByteArray);
               mapwrite["addrByteArray"] = paramsMapToThreads["addrByteArray"].toByteArray();


               byte = paramsMapToThreads["baudRateByteArray"].toByteArray()[0];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(baudRateByteArray);
               mapwrite["baudRateByteArray"] = paramsMapToThreads["baudRateByteArray"].toByteArray();


               byte = paramsMapToThreads["HMP_TransByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["HMP_TransByteArray"].toByteArray()[1];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(HMP_TransByteArray);
               mapwrite["HMP_TransByteArray"] = paramsMapToThreads["HMP_TransByteArray"].toByteArray();


               byte = paramsMapToThreads["EPP_TransByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["EPP_TransByteArray"].toByteArray()[1];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(EPP_TransByteArray);
               mapwrite["EPP_TransByteArray"] = paramsMapToThreads["EPP_TransByteArray"].toByteArray();


               byte = paramsMapToThreads["P1PrimAddrByteArray"].toByteArray()[0];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(P1PrimAddrByteArray);
               mapwrite["P1PrimAddrByteArray"] = paramsMapToThreads["P1PrimAddrByteArray"].toByteArray();


               byte = paramsMapToThreads["P2PrimAddrByteArray"].toByteArray()[0];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(P2PrimAddrByteArray);
               mapwrite["P2PrimAddrByteArray"] = paramsMapToThreads["P2PrimAddrByteArray"].toByteArray();


               byte = paramsMapToThreads["ProtTypeByteArray"].toByteArray()[0];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(ProtTypeByteArray);
               mapwrite["ProtTypeByteArray"] = paramsMapToThreads["ProtTypeByteArray"].toByteArray();


               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны ") +
    //                                                 "Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны ") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               QApplication::processEvents();
               global::pause(250);//(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось записать данные : External Interface Settings ") +
    //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Не удалось записать данные : External Interface Settings ") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
       //                ui->label_writeParams->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xab;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer[4];

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                      //


                      //
                      //



                      break;
                   }
                   else {
                       if(i==2) {
    //                       QMessageBox::information(this, "", tr("Ошибка записи данных : External Interface Settings ") +
    //                                                "Рабочее место: " + QString::number(workPlaceNumber+1));
                          label_StatusBar = (tr("Ошибка записи данных : External Interface Settings ") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
  //                        ui->label_writeParams->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }



               }


            }

            //
            //
            //Write External Device Settings  5a 00 25 3c
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x25;
               packetToRead.append(byte);
               byte = 0x3c;
               packetToRead.append(byte);

               byte = paramsMapToThreads["Serial_P1ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["Serial_P1ByteArray"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["Serial_P1ByteArray"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["Serial_P1ByteArray"].toByteArray()[3];
               packetToRead.append(byte);
   //            paramsVectorToWrite.append(Serial_P1ByteArray);
               mapwrite["Serial_P1ByteArray"] = paramsMapToThreads["Serial_P1ByteArray"].toByteArray();


               byte = paramsMapToThreads["ManCode_P1ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["ManCode_P1ByteArray"].toByteArray()[1];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(ManCode_P1ByteArray);
               mapwrite["ManCode_P1ByteArray"] = paramsMapToThreads["ManCode_P1ByteArray"].toByteArray();


               byte = paramsMapToThreads["DevType_P1ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
   //            paramsVectorToWrite.append(DevType_P1ByteArray);
               mapwrite["DevType_P1ByteArray"] = paramsMapToThreads["DevType_P1ByteArray"].toByteArray();


               byte = paramsMapToThreads["Version_P1ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
      //         paramsVectorToWrite.append(Version_P1ByteArray);
               mapwrite["Version_P1ByteArray"] = paramsMapToThreads["Version_P1ByteArray"].toByteArray();


               byte = paramsMapToThreads["Initial_value_P1ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["Initial_value_P1ByteArray"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["Initial_value_P1ByteArray"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["Initial_value_P1ByteArray"].toByteArray()[3];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(Initial_value_P1ByteArray);
               mapwrite["Initial_value_P1ByteArray"] = paramsMapToThreads["Initial_value_P1ByteArray"].toByteArray();


               byte = paramsMapToThreads["Service_P1ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["Service_P1ByteArray"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["Service_P1ByteArray"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["Service_P1ByteArray"].toByteArray()[3];
               packetToRead.append(byte);
      //         paramsVectorToWrite.append(Service_P1ByteArray);
               mapwrite["Service_P1ByteArray"] = paramsMapToThreads["Service_P1ByteArray"].toByteArray();


               byte = paramsMapToThreads["Serial_P2ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["Serial_P2ByteArray"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["Serial_P2ByteArray"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["Serial_P2ByteArray"].toByteArray()[3];
               packetToRead.append(byte);
   //            paramsVectorToWrite.append(Serial_P2ByteArray);
               mapwrite["Serial_P2ByteArray"] = paramsMapToThreads["Serial_P2ByteArray"].toByteArray();


               byte = paramsMapToThreads["ManCode_P2ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["ManCode_P2ByteArray"].toByteArray()[1];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(ManCode_P2ByteArray);
               mapwrite["ManCode_P2ByteArray"] = paramsMapToThreads["ManCode_P2ByteArray"].toByteArray();


               byte = paramsMapToThreads["DevType_P2ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
      //         paramsVectorToWrite.append(DevType_P2ByteArray);
               mapwrite["DevType_P2ByteArray"] = paramsMapToThreads["DevType_P2ByteArray"].toByteArray();


               byte = paramsMapToThreads["Version_P2ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
      //         paramsVectorToWrite.append(Version_P2ByteArray);
               mapwrite["Version_P2ByteArray"] = paramsMapToThreads["Version_P2ByteArray"].toByteArray();


               byte = paramsMapToThreads["Initial_value_P2ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["Initial_value_P2ByteArray"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["Initial_value_P2ByteArray"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["Initial_value_P2ByteArray"].toByteArray()[3];
               packetToRead.append(byte);
      //         paramsVectorToWrite.append(Initial_value_P2ByteArray);
               mapwrite["Initial_value_P2ByteArray"] = paramsMapToThreads["Initial_value_P2ByteArray"].toByteArray();


               byte = paramsMapToThreads["Service_P2ByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["Service_P2ByteArray"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["Service_P2ByteArray"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["Service_P2ByteArray"].toByteArray()[3];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(Service_P2ByteArray);
               mapwrite["Service_P2ByteArray"] = paramsMapToThreads["Service_P2ByteArray"].toByteArray();


               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны ") +
    //                                                 "Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны ") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               QApplication::processEvents();
               global::pause(250);//(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось записать данные : External Device Settings ") +
    //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Не удалось записать данные : External Device Settings ") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
 //                      ui->label_writeParams->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xbc;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer[4];

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                      //


                      //
                      //



                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка записи данных : External Device Settings") +
    //                                               "Рабочее место: " + QString::number(workPlaceNumber+1));
                          label_StatusBar = (tr("Ошибка записи данных : External Device Settings ") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
    //                      ui->label_writeParams->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }



               }


            }


//            //Открыть парольную сессию 5a 00 08 40 01 04 57
//            for(int i=0; i<3;i++) {

//               portTmp->clear();
//               packetToRead.clear();
//               buffer.clear();
//               quint8 byte = 0x5a;
//               packetToRead.append(byte);
//               byte = 0x00;
//               packetToRead.append(byte);
//               byte = 0x08;
//               packetToRead.append(byte);
//               byte = 0x40;
//               packetToRead.append(byte);
//               byte = 0x01;
//               packetToRead.append(byte);
//               byte = 0x04;
//               packetToRead.append(byte);
//               byte = 0x57;
//               packetToRead.append(byte);
//               quint8 crc = makeCRC(packetToRead);
//               packetToRead.append(crc);

//               quint64 cnt = portTmp->write(packetToRead);
//               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

//               qDebug()<<"cnt "<<cnt
//                       <<"packetToRead.toHex() "<<packetToRead.toHex()
//                       <<"portTmp->portName() "<<portTmp->portName();

//               if(cnt == 0) {
//    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
//    //                                                 "Рабочее место: " + QString::number(workPlaceNumber+1));
//                   label_StatusBar = (tr("Данные в порт не записаны") +
//                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
//                   emit errorStringSignal(label_StatusBar + '\n');
//                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
// //                   checkWritingError(currentIndicatorNumber);
// //                   emit workPlaceOff(currentIndicatorNumber);
//                   emit workPlaceOff(currentIndicatorNumber);
//                   emit checkWritingError(currentIndicatorNumber);

//                   return;
//               }

//               QApplication::processEvents();
//               global::pause(150);//(300);

//               buffer = portTmp->readAll();
//               qDebug()<<"buffer.toHex()"<<buffer.toHex();
//               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

//               if(buffer.isEmpty()) {
//                   if(i==2) {
//    //                   QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") +
//    //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
//                       label_StatusBar = (tr("Ошибка открытия парольной сессии") +
//                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
//                       emit errorStringSignal(label_StatusBar + '\n');
// //                      ui->label_writeParams->setVisible(true);
//                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
// //                       checkWritingError(currentIndicatorNumber);
// //                       emit workPlaceOff(currentIndicatorNumber);
//                       emit workPlaceOff(currentIndicatorNumber);
//                       emit checkWritingError(currentIndicatorNumber);

//                       return;
//                   } else{}

//               }
//               else {
//                   //проверяем crc и первые четыре байта ответного пакета
//                   QByteArray bytesForChecking;
//                   quint8 byte = 0x5a;
//                   bytesForChecking.append(byte);
//                   byte = 0x00;
//                   bytesForChecking.append(byte);
//                   byte = 0x06;
//                   bytesForChecking.append(byte);
//                   byte = 0xc0;
//                   bytesForChecking.append(byte);

//                   if(!packetSETProcessing(buffer)) {
//                       if(i == 2) {
//                           label_StatusBar = (tr("Ошибка данных") +
//                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
//                           emit errorStringSignal(label_StatusBar + '\n');
//                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
//                           emit workPlaceOff(currentIndicatorNumber);
//                           emit checkWritingError(currentIndicatorNumber);
//                           return;
//                       }
//                       else {
//                           continue;
//                       }
//                   }

//                   quint8 error = buffer.at(4);

//                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

//                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
//                           <<"checkCRC(buffer) "<<checkCRC(buffer);


//                      //
//                      //



//                      break;
//                   }
//                   else {
//                            if(i==2) {
//    //                            QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") +
//    //                                                     "Рабочее место: " + QString::number(workPlaceNumber+1));
//                                label_StatusBar = (tr("Ошибка открытия парольной сессии") +
//                                                             ". Рабочее место: " + QString::number(workPlaceNumber+1));
//                                emit errorStringSignal(label_StatusBar + '\n');
//    //                            ui->label_writeParams->setVisible(true);
//                                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
// //                                checkWritingError(currentIndicatorNumber);
//                                emit workPlaceOff(currentIndicatorNumber);
//                                emit checkWritingError(currentIndicatorNumber);

//                                return;
//                            }

//                   }



//               }


//            }

//            //
//            //Открыть парольную сессию 5a 00 08 40 01 04 57/

            //
            //Write Application Layer

//            Write Application Layer	WAL	34h	Request	5A	00	09	34	W5_Trans	W7_Trans	NB_Fi_UsrPktInt	CRC
//                                                      0	1	2	3	4	        5	        6, 7	        8
//                                                      Syn	Emp	Len	Cmd	1	        1	        2
//                                              Answer	5A	00	05	B4	Error	CRC
//                                                                      0 - daily; 1 - weekly; 2 - month; 7 - W5
//                                                                                  0 - Q; 1 - V; 2 - M; 3, 4 - T; 5 - Flags; 7 - W7

            emit textBrowser(">> Write Application Layer");

            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x09;
               packetToRead.append(byte);
               byte = 0x34;
               packetToRead.append(byte);

               byte = paramsMapToThreads["W5_Trans"].toByteArray()[0];
               packetToRead.append(byte);
               mapwrite["W5_Trans"] = paramsMapToThreads["W5_Trans"].toByteArray();

               byte = paramsMapToThreads["W7_Trans"].toByteArray()[0];
               packetToRead.append(byte);
               mapwrite["W7_Trans"] = paramsMapToThreads["W7_Trans"].toByteArray();

               byte = paramsMapToThreads["NB_Fi_UsrPktInt_WAL"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_UsrPktInt_WAL"].toByteArray()[1];
               packetToRead.append(byte);
               mapwrite["NB_Fi_UsrPktInt_WAL"] = paramsMapToThreads["NB_Fi_UsrPktInt_WAL"].toByteArray();

               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны ") +
    //                                                 "Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны ") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               QApplication::processEvents();
               global::pause(250);//(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось записать данные : Write Application Layer ") +
    //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Не удалось записать данные : Write Application Layer ") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
 //                      ui->label_writeParams->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета  Answer	5A	00	05	B4
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x05;
                   bytesForChecking.append(byte);
                   byte = 0xb4;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer[4];

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                      //


                      //
                      //



                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка записи данных : Write Application Layer ") +
    //                                               "Рабочее место: " + QString::number(workPlaceNumber+1));
                          label_StatusBar = (tr("Ошибка записи данных : Write Application Layer ") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
    //                      ui->label_writeParams->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }



               }


            }


            //Write Application Layer/
            //

            global::pause(timeIntervalWAL_WRS/*15000*/);

            //
            //Write RF Settings

            emit textBrowser(">> Write RF Settings");

            //Открыть парольную сессию 5a 00 08 40 01 04 57
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x08;
               packetToRead.append(byte);
               byte = 0x40;
               packetToRead.append(byte);
               byte = 0x01;
               packetToRead.append(byte);
               byte = 0x04;
               packetToRead.append(byte);
               byte = 0x57;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
    //                                                 "Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
 //                   checkWritingError(currentIndicatorNumber);
 //                   emit workPlaceOff(currentIndicatorNumber);
                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               QApplication::processEvents();
               global::pause(150);//(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") +
    //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Ошибка открытия парольной сессии") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
 //                      ui->label_writeParams->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
 //                       checkWritingError(currentIndicatorNumber);
 //                       emit workPlaceOff(currentIndicatorNumber);
                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xc0;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);


                      //
                      //



                      break;
                   }
                   else {
                            if(i==2) {
    //                            QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") +
    //                                                     "Рабочее место: " + QString::number(workPlaceNumber+1));
                                label_StatusBar = (tr("Ошибка открытия парольной сессии") +
                                                             ". Рабочее место: " + QString::number(workPlaceNumber+1));
                                emit errorStringSignal(label_StatusBar + '\n');
    //                            ui->label_writeParams->setVisible(true);
                                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
 //                                checkWritingError(currentIndicatorNumber);
                                emit workPlaceOff(currentIndicatorNumber);
                                emit checkWritingError(currentIndicatorNumber);

                                return;
                            }

                   }



               }


            }

            //
            //Открыть парольную сессию 5a 00 08 40 01 04 57/

//            Write RF Settings	WRS	38h	Request	5A	00	0A	38	NB_Fi_TxCh	NB_Fi_RxCh	NB_Fi_RFBand	NB_Fi_TxPwr	NB_Fi_RFPwr	CRC
//                                              0	1	2	3	4	        5	        6	            7	        8	        9
//                                              Syn Emp Len Cmd	1	        1	        1	            1	        1
//                                       Answer	5A	00	06	B8	Error	CRC
//                                                              Type - nbfi_phy_channel_t, see file nbfi.h
//                                                                          0…8, see file nbfi_config.h
//                                                                                      Output power in dBm


            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x0A;
               packetToRead.append(byte);
               byte = 0x38;
               packetToRead.append(byte);

               byte = paramsMapToThreads["NB_Fi_TxCh"].toByteArray()[0];
               packetToRead.append(byte);
               mapwrite["NB_Fi_TxCh"] = paramsMapToThreads["NB_Fi_TxCh"].toByteArray();

               byte = paramsMapToThreads["NB_Fi_RxCh"].toByteArray()[0];
               packetToRead.append(byte);
               mapwrite["NB_Fi_RxCh"] = paramsMapToThreads["NB_Fi_RxCh"].toByteArray();

               byte = paramsMapToThreads["NB_Fi_RFBand"].toByteArray()[0];
               packetToRead.append(byte);
               mapwrite["NB_Fi_RFBand"] = paramsMapToThreads["NB_Fi_RFBand"].toByteArray();

               byte = paramsMapToThreads["NB_Fi_TxPwr"].toByteArray()[0];
               packetToRead.append(byte);
               mapwrite["NB_Fi_TxPwr"] = paramsMapToThreads["NB_Fi_TxPwr"].toByteArray();

               byte = paramsMapToThreads["NB_Fi_RFPwr"].toByteArray()[0];
               packetToRead.append(byte);
               mapwrite["NB_Fi_RFPwr"] = paramsMapToThreads["NB_Fi_RFPwr"].toByteArray();

               //
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны ") +
    //                                                 "Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны ") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               QApplication::processEvents();
               global::pause(250);//(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось записать данные : External Device Settings ") +
    //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Не удалось записать данные : Write RF Settings ") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
 //                      ui->label_writeParams->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета Answer	5A	00	06	B8
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xb8;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer[4];

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                      //


                      //
                      //



                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка записи данных : External Device Settings") +
    //                                               "Рабочее место: " + QString::number(workPlaceNumber+1));
                          label_StatusBar = (tr("Ошибка записи данных : Write RF Settings ") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
    //                      ui->label_writeParams->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }



               }


            }



            //Write RF Settings/
            //


            global::pause(timeIntervalWRF_WNS/*30000*/);

            emit textBrowser(">> Write NB-FI Settings");

            //Открыть парольную сессию 5a 00 08 40 01 04 57
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x08;
               packetToRead.append(byte);
               byte = 0x40;
               packetToRead.append(byte);
               byte = 0x01;
               packetToRead.append(byte);
               byte = 0x04;
               packetToRead.append(byte);
               byte = 0x57;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
    //                                                 "Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
 //                   checkWritingError(currentIndicatorNumber);
 //                   emit workPlaceOff(currentIndicatorNumber);
                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               QApplication::processEvents();
               global::pause(150);//(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") +
    //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Ошибка открытия парольной сессии") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
 //                      ui->label_writeParams->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
 //                       checkWritingError(currentIndicatorNumber);
 //                       emit workPlaceOff(currentIndicatorNumber);
                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xc0;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);


                      //
                      //



                      break;
                   }
                   else {
                            if(i==2) {
    //                            QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") +
    //                                                     "Рабочее место: " + QString::number(workPlaceNumber+1));
                                label_StatusBar = (tr("Ошибка открытия парольной сессии") +
                                                             ". Рабочее место: " + QString::number(workPlaceNumber+1));
                                emit errorStringSignal(label_StatusBar + '\n');
    //                            ui->label_writeParams->setVisible(true);
                                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
 //                                checkWritingError(currentIndicatorNumber);
                                emit workPlaceOff(currentIndicatorNumber);
                                emit checkWritingError(currentIndicatorNumber);

                                return;
                            }

                   }



               }


            }

            //
            //Открыть парольную сессию 5a 00 08 40 01 04 57/


            //
            //Write NB-FI Settings

//            Write NB-Fi Setting	WNS	39h	Request	5A	00	15	39	NB_Fi_Mode	NB_Fi_HdSh	NB_Fi_MAC	NB_Fi_RetNum	NB_Fi_PldLen	NB_Fi_HrbtNum	NB_Fi_HrbtInt	NB_Fi_InfoInt	NB_Fi_UsrPktInt	CRC
//                                                  0	1	2	3	4	        5	        6	        7	            8	            9	            10…13	        14…17	         18, 19	        20
//                                                Syn	Emp	Len	Cmd	1	        1	        1	        1	            1	            1	            4	            4	             2
//                                        Answer   5A	00	06	B9	Error	CRC
//                                                                  0 - NRX, 1 - DRX, 2 - CRX, 3 - TRANSPARENT, 4 - OFF; type - nbfi_mode_t, see file nbfi.h
//                                                                              0 - HANDSHAKE_NONE, 1 - HANDSHAKE_SIMPLE; type - nbfi_handshake_t, see file nbfi.h
//                                                                                          Type - nbfi_mack_mode_t, see file nbfi.h
//                                                                                                      Retries number
//                                                                                                                      Payload length in bytes
//                                                                                                                                      Heartbeats number
//                                                                                                                                                      Time interval in minute
//                                                                                                                                                                      Time interval in second
//                                                                                                                                                                                       Time interval in minute

            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x13;//не нужен последний параметр, длина изменилась
               packetToRead.append(byte);
               byte = 0x39;
               packetToRead.append(byte);

               byte = paramsMapToThreads["NB_Fi_Mode"].toByteArray()[0];
               packetToRead.append(byte);
               mapwrite["NB_Fi_Mode"] = paramsMapToThreads["NB_Fi_Mode"].toByteArray();

               byte = paramsMapToThreads["NB_Fi_HdSh"].toByteArray()[0];
               packetToRead.append(byte);
               mapwrite["NB_Fi_HdSh"] = paramsMapToThreads["NB_Fi_HdSh"].toByteArray();

               byte = paramsMapToThreads["NB_Fi_MAC"].toByteArray()[0];
               packetToRead.append(byte);
               mapwrite["NB_Fi_MAC"] = paramsMapToThreads["NB_Fi_MAC"].toByteArray();

               byte = paramsMapToThreads["NB_Fi_RetNum"].toByteArray()[0];
               packetToRead.append(byte);
               mapwrite["NB_Fi_RetNum"] = paramsMapToThreads["NB_Fi_RetNum"].toByteArray();

               byte = paramsMapToThreads["NB_Fi_PldLen"].toByteArray()[0];
               packetToRead.append(byte);
               mapwrite["NB_Fi_PldLen"] = paramsMapToThreads["NB_Fi_PldLen"].toByteArray();

               byte = paramsMapToThreads["NB_Fi_HrbtNum"].toByteArray()[0];
               packetToRead.append(byte);
               mapwrite["NB_Fi_HrbtNum"] = paramsMapToThreads["NB_Fi_HrbtNum"].toByteArray();

               byte = paramsMapToThreads["NB_Fi_HrbtInt"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_HrbtInt"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_HrbtInt"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_HrbtInt"].toByteArray()[3];
               packetToRead.append(byte);
               mapwrite["NB_Fi_HrbtInt"] = paramsMapToThreads["NB_Fi_HrbtInt"].toByteArray();

               byte = paramsMapToThreads["NB_Fi_InfoInt"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_InfoInt"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_InfoInt"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_InfoInt"].toByteArray()[3];
               packetToRead.append(byte);
               mapwrite["NB_Fi_InfoInt"] = paramsMapToThreads["NB_Fi_InfoInt"].toByteArray();

               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны ") +
    //                                                 "Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны ") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               QApplication::processEvents();
               global::pause(250);//(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось записать данные : External Device Settings ") +
    //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Не удалось записать данные : Write NB-FI Settings ") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
 //                      ui->label_writeParams->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета Answer   5A	00	06	B9
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xb9;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer[4];

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                      //


                      //
                      //



                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка записи данных : External Device Settings") +
    //                                               "Рабочее место: " + QString::number(workPlaceNumber+1));
                          label_StatusBar = (tr("Ошибка записи данных : Write NB-FI Settings ") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
    //                      ui->label_writeParams->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }



               }


            }

            //Write NB-FI Settings/
            //

            global::pause(timeIntervalWNB_WID/*19000*/);

            emit textBrowser(">> Write Identificators");

            //Открыть парольную сессию 5a 00 08 40 01 04 57
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x08;
               packetToRead.append(byte);
               byte = 0x40;
               packetToRead.append(byte);
               byte = 0x01;
               packetToRead.append(byte);
               byte = 0x04;
               packetToRead.append(byte);
               byte = 0x57;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
    //                                                 "Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
 //                   checkWritingError(currentIndicatorNumber);
 //                   emit workPlaceOff(currentIndicatorNumber);
                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               QApplication::processEvents();
               global::pause(150);//(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") +
    //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Ошибка открытия парольной сессии") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
 //                      ui->label_writeParams->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
 //                       checkWritingError(currentIndicatorNumber);
 //                       emit workPlaceOff(currentIndicatorNumber);
                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xc0;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);


                      //
                      //



                      break;
                   }
                   else {
                            if(i==2) {
    //                            QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") +
    //                                                     "Рабочее место: " + QString::number(workPlaceNumber+1));
                                label_StatusBar = (tr("Ошибка открытия парольной сессии") +
                                                             ". Рабочее место: " + QString::number(workPlaceNumber+1));
                                emit errorStringSignal(label_StatusBar + '\n');
    //                            ui->label_writeParams->setVisible(true);
                                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
 //                                checkWritingError(currentIndicatorNumber);
                                emit workPlaceOff(currentIndicatorNumber);
                                emit checkWritingError(currentIndicatorNumber);

                                return;
                            }

                   }



               }


            }

            //
            //Открыть парольную сессию 5a 00 08 40 01 04 57/


            //
            //Write Identificators

//            Write IDentificators	WID	35h	Request	5A	00	2D	35	NB_Fi_BrdID	NB_Fi_MdmID	NB_Fi_Key	CRC
//                                                  0	1	2	3	4…7	        8…11	    12…43	    44
//                                                  Syn	Emp	Len	Cmd	4	        4	        32
//                                          Answer	5A	00	05	B5	Error	CRC


            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x2d;
               packetToRead.append(byte);
               byte = 0x35;
               packetToRead.append(byte);

               byte = paramsMapToThreads["NB_Fi_BrdID"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_BrdID"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_BrdID"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_BrdID"].toByteArray()[3];
               packetToRead.append(byte);
               mapwrite["NB_Fi_BrdID"] = paramsMapToThreads["NB_Fi_BrdID"].toByteArray();

               byte = paramsMapToThreads["NB_Fi_MdmID"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_MdmID"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_MdmID"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_MdmID"].toByteArray()[3];
               packetToRead.append(byte);
               mapwrite["NB_Fi_MdmID"] = paramsMapToThreads["NB_Fi_MdmID"].toByteArray();

               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[1];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[2];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[3];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[4];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[5];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[6];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[7];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[8];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[9];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[10];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[11];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[12];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[13];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[14];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[15];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[16];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[17];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[18];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[19];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[20];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[21];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[22];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[23];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[24];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[25];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[26];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[27];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[28];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[29];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[30];
               packetToRead.append(byte);
               byte = paramsMapToThreads["NB_Fi_Key"].toByteArray()[31];
               packetToRead.append(byte);
               mapwrite["NB_Fi_Key"] = paramsMapToThreads["NB_Fi_Key"].toByteArray();

               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны ") +
    //                                                 "Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны ") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWritingError(currentIndicatorNumber);

                   return;
               }

               QApplication::processEvents();
               global::pause(250);//(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось записать данные : Write Identificators ") +
    //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Не удалось записать данные : Write Identificators ") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
 //                      ui->label_writeParams->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета Answer	5A	00	05	B5
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x05;
                   bytesForChecking.append(byte);
                   byte = 0xb5;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWritingError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer[4];

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                      //


                      //
                      //



                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка записи данных : Write Identificators ") +
    //                                               "Рабочее место: " + QString::number(workPlaceNumber+1));
                          label_StatusBar = (tr("Ошибка записи данных : Write Identificators ") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
    //                      ui->label_writeParams->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWritingError(currentIndicatorNumber);

                          return;
                       }
                   }



               }


            }


            //Write Identificators/
            //


            global::pause(timeIntervalWID_Read/*12000*/);


            //
            //
            //проверка записанного. из списка проверяемых параметров удаляем те серийный номера и PCB_SN,
            //которые не являются текущими для рабочего места



//            readParams(workPlaceNumber);

            slotReadParams(port1, port2, port3, port4);



            allParamsNameList.removeOne("minuteByteArray");
            allParamsNameList.removeOne("secondByteArray");
            allParamsNameList.removeOne("hourByteArray");

            switch (workPlaceNumber) {
            case 0:
               mapwrite["serialNumber2"] = "";
               mapwrite["serialNumber3"] = "";
               mapwrite["serialNumber4"] = "";
               mapRead["serialNumber2"] = "";
               mapRead["serialNumber3"] = "";
               mapRead["serialNumber4"] = "";

               mapwrite["PCB_SN_ByteArray2"] = "";
               mapwrite["PCB_SN_ByteArray3"] = "";
               mapwrite["PCB_SN_ByteArray4"] = "";
               mapRead["PCB_SN_ByteArray2"] = "";
               mapRead["PCB_SN_ByteArray3"] = "";
               mapRead["PCB_SN_ByteArray4"] = "";
                break;
            case 1:
               mapwrite["serialNumber"] = "";
               mapwrite["serialNumber3"] = "";
               mapwrite["serialNumber4"] = "";
               mapRead["serialNumber"] = "";
               mapRead["serialNumber3"] = "";
               mapRead["serialNumber4"] = "";

               mapwrite["PCB_SN_ByteArray"] = "";
               mapwrite["PCB_SN_ByteArray3"] = "";
               mapwrite["PCB_SN_ByteArray4"] = "";
               mapRead["PCB_SN_ByteArray"] = "";
               mapRead["PCB_SN_ByteArray3"] = "";
               mapRead["PCB_SN_ByteArray4"] = "";
                break;
            case 2:
               mapwrite["serialNumber2"] = "";
               mapwrite["serialNumber"] = "";
               mapwrite["serialNumber4"] = "";
               mapRead["serialNumber2"] = "";
               mapRead["serialNumber"] = "";
               mapRead["serialNumber4"] = "";

               mapwrite["PCB_SN_ByteArray2"] = "";
               mapwrite["PCB_SN_ByteArray"] = "";
               mapwrite["PCB_SN_ByteArray4"] = "";
               mapRead["PCB_SN_ByteArray2"] = "";
               mapRead["PCB_SN_ByteArray"] = "";
               mapRead["PCB_SN_ByteArray4"] = "";
                break;
            case 3:
               mapwrite["serialNumber2"] = "";
               mapwrite["serialNumber3"] = "";
               mapwrite["serialNumber"] = "";
               mapRead["serialNumber2"] = "";
               mapRead["serialNumber3"] = "";
               mapRead["serialNumber"] = "";

               mapwrite["PCB_SN_ByteArray2"] = "";
               mapwrite["PCB_SN_ByteArray3"] = "";
               mapwrite["PCB_SN_ByteArray"] = "";
               mapRead["PCB_SN_ByteArray2"] = "";
               mapRead["PCB_SN_ByteArray3"] = "";
               mapRead["PCB_SN_ByteArray"] = "";
                break;
            default:
                break;
            }



            for(int i=0; i<allParamsNameList.size(); i++) {
                qDebug()<<"Write "<<allParamsNameList[i] + " "<<mapwrite[allParamsNameList[i]].toHex();
                qDebug()<<"Read "<<allParamsNameList[i] + " "<<mapRead[allParamsNameList[i]].toHex();

                if((allParamsNameList[i] != "pulsesValue1ByteArray") && (allParamsNameList[i] != "pulsesValue2ByteArray")) {
                   if(mapwrite[allParamsNameList[i]] != mapRead[allParamsNameList[i]]) {
    //                   QMessageBox::information(this, "", "Параметр записан некорректно : " +
    //                                            allParamsNameList[i] + " " + mapRead[allParamsNameList[i]].toHex());
                       label_StatusBar = (tr("Параметр записан некорректно : ") + allParamsNameList[i] + " " +
                                                    mapRead[allParamsNameList[i]].toHex() +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWritingError(currentIndicatorNumber);

    //                   label_StatusBar = ("Запись : Неудачно");
    //                   ui->label_writeParams->setVisible(true);
                       return;
                   }
                }

                //pulsesValue1
                if(allParamsNameList[i] == "pulsesValue1ByteArray") {
                    quint16 pulsesValue1IntRead = mapRead["pulsesValue1ByteArray"].at(0);
                    pulsesValue1IntRead = pulsesValue1IntRead<<8 | mapRead["pulsesValue1ByteArray"].at(1);

                    quint16 pulsesValue1IntWrite = mapwrite["pulsesValue1ByteArray"].at(0);
                    pulsesValue1IntWrite = pulsesValue1IntWrite<<8 | mapwrite["pulsesValue1ByteArray"].at(1);

                    if((pulsesValue1IntRead <= (pulsesValue1IntWrite + 1)) && (pulsesValue1IntRead >= (pulsesValue1IntWrite - 1))) {

                    }
                    else {
    //                    QMessageBox::information(this, "", "Параметр записан некорректно : pulsesValue1 " +
    //                                             QString::number(pulsesValue1IntRead));
                        label_StatusBar = (tr("Параметр записан некорректно : pulsesValue1 ") +
                                                     QString::number(pulsesValue1IntRead) +
                                                     ". Рабочее место: " + QString::number(workPlaceNumber+1));
                        emit errorStringSignal(label_StatusBar + '\n');
                        vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                        emit workPlaceOff(currentIndicatorNumber);
                        emit checkWritingError(currentIndicatorNumber);

    //                    label_StatusBar = ("Запись : Неудачно");
    //                    ui->label_writeParams->setVisible(true);
                        return;
                    }
                }

                //pulsesValue2
                if(allParamsNameList[i] == "pulsesValue2ByteArray") {
                    quint16 pulsesValue2IntRead = mapRead["pulsesValue2ByteArray"].at(0);
                    pulsesValue2IntRead = pulsesValue2IntRead<<8 | mapRead["pulsesValue2ByteArray"].at(1);

                    quint16 pulsesValue2IntWrite = mapwrite["pulsesValue2ByteArray"].at(0);
                    pulsesValue2IntWrite = pulsesValue2IntWrite<<8 | mapwrite["pulsesValue2ByteArray"].at(1);

                    if((pulsesValue2IntRead <= (pulsesValue2IntWrite + 1)) && (pulsesValue2IntRead >= (pulsesValue2IntWrite - 1))) {

                    }
                    else {
    //                    QMessageBox::information(this, "", "Параметр записан некорректно : pulsesValue2 " +
    //                                             QString::number(pulsesValue2IntRead));
                        label_StatusBar = (tr("Параметр записан некорректно : pulsesValue2 ") +
                                                     QString::number(pulsesValue2IntRead) +
                                                     ". Рабочее место: " + QString::number(workPlaceNumber+1));
                        emit errorStringSignal(label_StatusBar + '\n');
                        vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                        emit workPlaceOff(currentIndicatorNumber);
                        emit checkWritingError(currentIndicatorNumber);

    //                    label_StatusBar = ("Запись : Неудачно");
    //                    ui->label_writeParams->setVisible(true);
                        return;
                    }
                }

            }

//      //      label_StatusBar = ("Запись : Успешно ");

//            ui->label_writeParams->setStyleSheet(QString("color: green").arg(color.name()));
//            ui->label_writeParams->setText("V");
//      //      ui->label_writeParams->setVisible(true);



           emit checkWritingError(currentIndicatorNumber);


}

void ObjectThread::slotCalibration(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4)
{

    qDebug()<<"ObjectThread::slotCalibration() "<<"workPlace "<<QString::number(workPlace);

            QSerialPort *portTmp;

            bool isNeedPaintEvent = true;//false;
            int workPlaceNumber = workPlace;
            int currentIndicatorNumber = workPlaceNumber;

            if(!isWorkPlaceUse.at(workPlace)) return;

            QString label_StatusBar;
            QString errorString;

            switch (workPlaceNumber) {
            case 0:
                portTmp = port1;
                r_ref1Result.clear();
                r_ref2Result.clear();
                break;
            case 1:
                portTmp = port2;
                r_ref1Result2.clear();
                r_ref2Result2.clear();
                break;
            case 2:
                portTmp = port3;
                r_ref1Result3.clear();
                r_ref2Result3.clear();
                break;
            case 3:
                portTmp = port4;
                r_ref1Result4.clear();
                r_ref2Result4.clear();
                break;

            default:
                break;
            }


            int currentBoxNumber = 1;


//            QColor color(255, 127, 50);
//            ui->label_Calibration->setStyleSheet(QString("color: red").arg(color.name()));
//            ui->label_Calibration->setText("X");
//            ui->label_Calibration->setVisible(false);

//            label_StatusBar = ("");

//            ui->lineEdit_RMV_ErrorFlags->setText("");
//            ui->label_RMV_ErrorFlags_tick->setStyleSheet(QString("color: red").arg(color.name()));
//            ui->label_RMV_ErrorFlags_tick->setText("X");
//            ui->label_RMV_ErrorFlags_tick->setVisible(false);


            bool isCalibrationCorrect = false;
            bool isR_Ref1_Correct = false;
            bool isR_Ref2_Correct = false;
            bool isTMeasDirCorrect  = false;
            bool isTMeasRevCorrect = false;

            bool isCalibrationCorrect2 = false;
            bool isR_Ref1_Correct2 = false;
            bool isR_Ref2_Correct2 = false;
            bool isTMeasDirCorrect2  = false;
            bool isTMeasRevCorrect2 = false;

            bool isCalibrationCorrect3 = false;
            bool isR_Ref1_Correct3 = false;
            bool isR_Ref2_Correct3 = false;
            bool isTMeasDirCorrect3  = false;
            bool isTMeasRevCorrect3 = false;

            bool isCalibrationCorrect4 = false;
            bool isR_Ref1_Correct4 = false;
            bool isR_Ref2_Correct4 = false;
            bool isTMeasDirCorrect4  = false;
            bool isTMeasRevCorrect4 = false;

            qDebug()<<"MainWindow::slotStartCalibration(QMap<QString, QByteArray> calibrationMap)";

            QByteArray buffer;

            if(!portTmp->isOpen()) {
                if(!portTmp->open(QIODevice::ReadWrite)) {
    //                QMessageBox::information(this, "", "Не удалось открыть порт УСО-2. Рабочее место: " +
    //                                         QString::number(workPlaceNumber + 1));
                    label_StatusBar = ("Не удалось открыть порт УСО-2. Рабочее место: " +
                                                 QString::number(workPlaceNumber + 1));
                    emit errorStringSignal(label_StatusBar + '\n');
                    vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                    emit workPlaceOff(currentIndicatorNumber);
                    emit checkCalibrationError(currentIndicatorNumber);

                    return;
                }
            }


        //    R_Real1ByteArray = calibrationMap["R_Real1ByteArray"];
        //    R_Real2ByteArray = calibrationMap["R_Real2ByteArray"];
        //    qDebug()<<"R_Real1ByteArray.toHex()"<<R_Real1ByteArray.toHex()
        //            <<"R_Real2ByteArray.toHex()"<<R_Real2ByteArray.toHex();
        //    temperature1ByteArray = calibrationMap["temperature1ByteArray"];
        //    temperature2ByteArray = calibrationMap["temperature2ByteArray"];


            //
            //Открыть парольную сессию 5a 00 08 40 01 04 57
            for(int i=0; i<20;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x08;
               packetToRead.append(byte);
               byte = 0x40;
               packetToRead.append(byte);
               byte = 0x01;
               packetToRead.append(byte);
               byte = 0x04;
               packetToRead.append(byte);
               byte = 0x57;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
    //                                                 "Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
//                   checkWritingError(currentIndicatorNumber);
//                   emit workPlaceOff(currentIndicatorNumber);
                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkCalibrationError(currentIndicatorNumber);

                   return;
               }

               QApplication::processEvents();
               global::pause(150);//(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==19) {
    //                   QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") +
    //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Ошибка открытия парольной сессии") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
 //                      ui->label_writeParams->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
//                       checkWritingError(currentIndicatorNumber);
//                       emit workPlaceOff(currentIndicatorNumber);
                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkCalibrationError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xc0;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 19) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkCalibrationError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);


                      //
                      //



                      break;
                   }
                   else {
                            if(i==19) {
    //                            QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") +
    //                                                     "Рабочее место: " + QString::number(workPlaceNumber+1));
                                label_StatusBar = (tr("Ошибка открытия парольной сессии") +
                                                             ". Рабочее место: " + QString::number(workPlaceNumber+1));
                                emit errorStringSignal(label_StatusBar + '\n');
    //                            ui->label_writeParams->setVisible(true);
                                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
//                                checkWritingError(currentIndicatorNumber);
                                emit workPlaceOff(currentIndicatorNumber);
                                emit checkCalibrationError(currentIndicatorNumber);

                                return;
                            }

                   }



               }


            }

            //
            //Открыть парольную сессию 5a 00 08 40 01 04 57/



            //
            //Temperature Channel Calibration 5a 00 0d 37 R_Real1(4bytes) R_Real2(4bytes)
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x0d;
               packetToRead.append(byte);
               byte = 0x37;
               packetToRead.append(byte);

              if(workPlaceNumber == 0) {
               byte = R_Real1ByteArray[0];
               packetToRead.append(byte);
               byte = R_Real1ByteArray[1];
               packetToRead.append(byte);
               byte = R_Real1ByteArray[2];
               packetToRead.append(byte);
               byte = R_Real1ByteArray[3];
               packetToRead.append(byte);

               byte = R_Real2ByteArray[0];
               packetToRead.append(byte);
               byte = R_Real2ByteArray[1];
               packetToRead.append(byte);
               byte = R_Real2ByteArray[2];
               packetToRead.append(byte);
               byte = R_Real2ByteArray[3];
               packetToRead.append(byte);
              }

              if(workPlaceNumber == 1) {
               byte = R_Real1ByteArray2[0];
               packetToRead.append(byte);
               byte = R_Real1ByteArray2[1];
               packetToRead.append(byte);
               byte = R_Real1ByteArray2[2];
               packetToRead.append(byte);
               byte = R_Real1ByteArray2[3];
               packetToRead.append(byte);

               byte = R_Real2ByteArray2[0];
               packetToRead.append(byte);
               byte = R_Real2ByteArray2[1];
               packetToRead.append(byte);
               byte = R_Real2ByteArray2[2];
               packetToRead.append(byte);
               byte = R_Real2ByteArray2[3];
               packetToRead.append(byte);
              }

              if(workPlaceNumber == 2) {
               byte = R_Real1ByteArray3[0];
               packetToRead.append(byte);
               byte = R_Real1ByteArray3[1];
               packetToRead.append(byte);
               byte = R_Real1ByteArray3[2];
               packetToRead.append(byte);
               byte = R_Real1ByteArray3[3];
               packetToRead.append(byte);

               byte = R_Real2ByteArray3[0];
               packetToRead.append(byte);
               byte = R_Real2ByteArray3[1];
               packetToRead.append(byte);
               byte = R_Real2ByteArray3[2];
               packetToRead.append(byte);
               byte = R_Real2ByteArray3[3];
               packetToRead.append(byte);
              }

              if(workPlaceNumber == 3) {
               byte = R_Real1ByteArray4[0];
               packetToRead.append(byte);
               byte = R_Real1ByteArray4[1];
               packetToRead.append(byte);
               byte = R_Real1ByteArray4[2];
               packetToRead.append(byte);
               byte = R_Real1ByteArray4[3];
               packetToRead.append(byte);

               byte = R_Real2ByteArray4[0];
               packetToRead.append(byte);
               byte = R_Real2ByteArray4[1];
               packetToRead.append(byte);
               byte = R_Real2ByteArray4[2];
               packetToRead.append(byte);
               byte = R_Real2ByteArray4[3];
               packetToRead.append(byte);
              }

               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
    //                                                 " Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                " Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkCalibrationError(currentIndicatorNumber);

                   return;
               }

               global::pause(4500);//(5000);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось записать данные") +
    //                                            " Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Не удалось записать данные") +
                                                    " Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
   //                    ui->label_Calibration->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkCalibrationError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xb7;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkCalibrationError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }


                   quint8 error = buffer.at(4);


                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0]
                           && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                      //


                      //
                      //



                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка данных") +
    //                                               " Рабочее место: " + QString::number(workPlaceNumber+1));
                          label_StatusBar = (tr("Ошибка данных") +
                                                       " Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
       //                   ui->label_Calibration->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkCalibrationError(currentIndicatorNumber);

                          return;
                       } else{}
                   }



               }


            }

            //
            //
            //RCF   5a 00 05 06

            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x05;
               packetToRead.append(byte);
               byte = 0x06;
               packetToRead.append(byte);

               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
    //                                                 " Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                " Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkCalibrationError(currentIndicatorNumber);

                   return;
               }

               global::pause(200);//(200);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());;

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось прочитать данные") +
    //                                            " Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Не удалось прочитать данные") +
                                                    " Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
         //              ui->label_Calibration->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkCalibrationError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x23;
                   bytesForChecking.append(byte);
                   byte = 0x86;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Не удалось прочитать данные") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkCalibrationError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

    //               quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0]
                           && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3]) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                     //------------------------------------------

                     if(workPlaceNumber == 0) {

                      //
                      //
                      //R_Ref1 (10-13)
                      quint32 R_Ref1Int = (quint8)buffer[10];
                      R_Ref1Int = R_Ref1Int<<8 | (quint8)buffer[11];
                      R_Ref1Int = R_Ref1Int<<8 | (quint8)buffer[12];
                      R_Ref1Int = R_Ref1Int<<8 | (quint8)buffer[13];

                      float R_Ref1Float = (float)R_Ref1Int/1000000;

                      qDebug()<<"R_Ref1Float "<<R_Ref1Float;

                      r_ref1Result = QString::number(R_Ref1Float, 'f', 6);
                      //

                      //
                      //R_Ref2 (14-17)
                      quint32 R_Ref2Int = (quint8)buffer[14];
                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[15];
                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[16];
                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[17];

                      float R_Ref2Float = (float)R_Ref2Int/1000000;

                      qDebug()<<"R_Ref2Float "<<R_Ref2Float;

                      r_ref2Result = QString::number(R_Ref2Float, 'f', 6);
                      //


                      if( R_Ref1Float>=(Acceptable_R_Ref1Float - Acceptable_R_Ref1Float/100) &&
                              R_Ref1Float<=(Acceptable_R_Ref1Float + Acceptable_R_Ref1Float/100) ) { //если выходит за допустимые пределы

                          isR_Ref1_Correct = true;
                      }
                      else {
    //                         QMessageBox::information(this, "", "Недопустимое значение R_Ref1. Рабочее место: "
    //                                                  + QString::number(workPlaceNumber+1));
                             label_StatusBar = ("Недопустимое значение R_Ref1. Рабочее место: "
                                                          + QString::number(workPlaceNumber+1) + ".R_Ref1 " + QString::number(R_Ref1Float));
                             emit errorStringSignal(label_StatusBar + '\n');
        //                     ui->label_Calibration->setVisible(true);
                             vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                             emit workPlaceOff(currentIndicatorNumber);
                             emit checkCalibrationError(currentIndicatorNumber);

                             return;
                      }


                      //
                      //R_Ref2 (14-17)
//                      quint32 R_Ref2Int = (quint8)buffer[14];
//                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[15];
//                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[16];
//                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[17];

//                      float R_Ref2Float = (float)R_Ref2Int/1000000;

//                      qDebug()<<"R_Ref2Float "<<R_Ref2Float;

//                      r_ref2Result = QString::number(R_Ref2Float);

                      if( R_Ref2Float>=(Acceptable_R_Ref2Float - Acceptable_R_Ref2Float/100) &&
                              R_Ref2Float<=(Acceptable_R_Ref2Float + Acceptable_R_Ref2Float/100) ) { //если выходит за допустимые пределы

                          isR_Ref2_Correct = true;

                      }
                      else {

    //                      QMessageBox::information(this, "", "Недопустимое значение R_Ref2. Рабочее место: "
    //                                               + QString::number(workPlaceNumber+1));
                          label_StatusBar = ("Недопустимое значение R_Ref2. Рабочее место: "
                                                       + QString::number(workPlaceNumber+1) + ".R_Ref2 " + QString::number(R_Ref2Float));
                          emit errorStringSignal(label_StatusBar + '\n');
     //                     ui->label_Calibration->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkCalibrationError(currentIndicatorNumber);

                          return;
                      }

                      //
                      //
                      //R_Ref2 (14-17) /

                     }

                     //----------------------------------------------

                     if(workPlaceNumber == 1) {

                      //
                      //
                      //R_Ref1 (10-13)
                      quint32 R_Ref1Int = (quint8)buffer[10];
                      R_Ref1Int = R_Ref1Int<<8 | (quint8)buffer[11];
                      R_Ref1Int = R_Ref1Int<<8 | (quint8)buffer[12];
                      R_Ref1Int = R_Ref1Int<<8 | (quint8)buffer[13];

                      float R_Ref1Float = (float)R_Ref1Int/1000000;

                      qDebug()<<"R_Ref1Float "<<R_Ref1Float;

                      r_ref1Result2 = QString::number(R_Ref1Float, 'f', 6);
                      //

                      //
                      //R_Ref2 (14-17)
                      quint32 R_Ref2Int = (quint8)buffer[14];
                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[15];
                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[16];
                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[17];

                      float R_Ref2Float = (float)R_Ref2Int/1000000;

                      qDebug()<<"R_Ref2Float "<<R_Ref2Float;

                      r_ref2Result2 = QString::number(R_Ref2Float, 'f', 6);
                      //

                      if( R_Ref1Float>=(Acceptable_R_Ref1Float2 - Acceptable_R_Ref1Float2/100) &&
                              R_Ref1Float<=(Acceptable_R_Ref1Float2 + Acceptable_R_Ref1Float2/100) ) { //если выходит за допустимые пределы

                          isR_Ref1_Correct2 = true;
                      }
                      else {
    //                         QMessageBox::information(this, "", "Недопустимое значение R_Ref1. Рабочее место: "
    //                                                  + QString::number(workPlaceNumber+1));
                             label_StatusBar = ("Недопустимое значение R_Ref1. Рабочее место: "
                                                          + QString::number(workPlaceNumber+1) + ".R_Ref1 " + QString::number(R_Ref1Float));
                             emit errorStringSignal(label_StatusBar + '\n');
        //                     ui->label_Calibration->setVisible(true);
                             vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                             emit workPlaceOff(currentIndicatorNumber);
                             emit checkCalibrationError(currentIndicatorNumber);

                             return;
                      }


                      //
                      //R_Ref2 (14-17)
//                      quint32 R_Ref2Int = (quint8)buffer[14];
//                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[15];
//                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[16];
//                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[17];

//                      float R_Ref2Float = (float)R_Ref2Int/1000000;

//                      qDebug()<<"R_Ref2Float "<<R_Ref2Float;

//                      r_ref2Result2 = QString::number(R_Ref2Float);
                      //



                      if( R_Ref2Float>=(Acceptable_R_Ref2Float2 - Acceptable_R_Ref2Float2/100) &&
                              R_Ref2Float<=(Acceptable_R_Ref2Float2 + Acceptable_R_Ref2Float2/100) ) { //если выходит за допустимые пределы

                          isR_Ref2_Correct2 = true;

                      }
                      else {

    //                      QMessageBox::information(this, "", "Недопустимое значение R_Ref2. Рабочее место: "
    //                                               + QString::number(workPlaceNumber+1));
                          label_StatusBar = ("Недопустимое значение R_Ref2. Рабочее место: "
                                                       + QString::number(workPlaceNumber+1) + ".R_Ref2 " + QString::number(R_Ref2Float));
                          emit errorStringSignal(label_StatusBar + '\n');
      //                    ui->label_Calibration->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkCalibrationError(currentIndicatorNumber);

                          return;
                      }

                      //
                      //
                      //R_Ref2 (14-17) /

                     }

                     //---------------------------------------------------------

                     if(workPlaceNumber == 2) {

                      //
                      //
                      //R_Ref1 (10-13)
                      quint32 R_Ref1Int = (quint8)buffer[10];
                      R_Ref1Int = R_Ref1Int<<8 | (quint8)buffer[11];
                      R_Ref1Int = R_Ref1Int<<8 | (quint8)buffer[12];
                      R_Ref1Int = R_Ref1Int<<8 | (quint8)buffer[13];

                      float R_Ref1Float = (float)R_Ref1Int/1000000;

                      qDebug()<<"R_Ref1Float "<<R_Ref1Float;

                      r_ref1Result3 = QString::number(R_Ref1Float, 'f', 6);
                      //

                      //
                      //R_Ref2 (14-17)
                      quint32 R_Ref2Int = (quint8)buffer[14];
                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[15];
                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[16];
                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[17];

                      float R_Ref2Float = (float)R_Ref2Int/1000000;

                      qDebug()<<"R_Ref2Float "<<R_Ref2Float;

                      r_ref2Result3 = QString::number(R_Ref2Float, 'f', 6);
                      //
                      //


                      if( R_Ref1Float>=(Acceptable_R_Ref1Float3 - Acceptable_R_Ref1Float3/100) &&
                              R_Ref1Float<=(Acceptable_R_Ref1Float3 + Acceptable_R_Ref1Float3/100) ) { //если выходит за допустимые пределы

                          isR_Ref1_Correct3 = true;
                      }
                      else {
    //                         QMessageBox::information(this, "", "Недопустимое значение R_Ref1. Рабочее место: "
    //                                                  + QString::number(workPlaceNumber+1));
                             label_StatusBar = ("Недопустимое значение R_Ref1. Рабочее место: "
                                                          + QString::number(workPlaceNumber+1) + ".R_Ref1 " + QString::number(R_Ref1Float));
                             emit errorStringSignal(label_StatusBar + '\n');
       //                      ui->label_Calibration->setVisible(true);
                             vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                             emit workPlaceOff(currentIndicatorNumber);
                             emit checkCalibrationError(currentIndicatorNumber);

                             return;
                      }


                      //
                      //R_Ref2 (14-17)
//                      quint32 R_Ref2Int = (quint8)buffer[14];
//                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[15];
//                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[16];
//                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[17];

//                      float R_Ref2Float = (float)R_Ref2Int/1000000;

//                      qDebug()<<"R_Ref2Float "<<R_Ref2Float;

//                      r_ref2Result3 = QString::number(R_Ref2Float);
                      //

                      if( R_Ref2Float>=(Acceptable_R_Ref2Float3 - Acceptable_R_Ref2Float3/100) &&
                              R_Ref2Float<=(Acceptable_R_Ref2Float3 + Acceptable_R_Ref2Float3/100) ) { //если выходит за допустимые пределы

                          isR_Ref2_Correct3 = true;

                      }
                      else {

    //                      QMessageBox::information(this, "", "Недопустимое значение R_Ref2. Рабочее место: "
    //                                               + QString::number(workPlaceNumber+1));
                          label_StatusBar = ("Недопустимое значение R_Ref2. Рабочее место: "
                                                       + QString::number(workPlaceNumber+1) + ".R_Ref2 " + QString::number(R_Ref2Float));
                          emit errorStringSignal(label_StatusBar + '\n');
      //                    ui->label_Calibration->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkCalibrationError(currentIndicatorNumber);

                          return;
                      }

                      //
                      //
                      //R_Ref2 (14-17) /

                     }

                     //---------------------------------------------------------

                     if(workPlaceNumber == 3) {

                      //
                      //
                      //R_Ref1 (10-13)
                      quint32 R_Ref1Int = (quint8)buffer[10];
                      R_Ref1Int = R_Ref1Int<<8 | (quint8)buffer[11];
                      R_Ref1Int = R_Ref1Int<<8 | (quint8)buffer[12];
                      R_Ref1Int = R_Ref1Int<<8 | (quint8)buffer[13];

                      float R_Ref1Float = (float)R_Ref1Int/1000000;

                      qDebug()<<"R_Ref1Float "<<R_Ref1Float;

                      r_ref1Result4 = QString::number(R_Ref1Float, 'f', 6);
                      //

                      //
                      //R_Ref2 (14-17)
                      quint32 R_Ref2Int = (quint8)buffer[14];
                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[15];
                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[16];
                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[17];

                      float R_Ref2Float = (float)R_Ref2Int/1000000;

                      qDebug()<<"R_Ref2Float "<<R_Ref2Float;

                      r_ref2Result4 = QString::number(R_Ref2Float, 'f', 6);
                      //


                      if( R_Ref1Float>=(Acceptable_R_Ref1Float4 - Acceptable_R_Ref1Float4/100) &&
                              R_Ref1Float<=(Acceptable_R_Ref1Float4 + Acceptable_R_Ref1Float4/100) ) { //если выходит за допустимые пределы

                          isR_Ref1_Correct4 = true;
                      }
                      else {
    //                         QMessageBox::information(this, "", "Недопустимое значение R_Ref1. Рабочее место: "
    //                                                  + QString::number(workPlaceNumber+1));
                             label_StatusBar = ("Недопустимое значение R_Ref1. Рабочее место: "
                                                          + QString::number(workPlaceNumber+1) + ".R_Ref1 " + QString::number(R_Ref1Float));
                             emit errorStringSignal(label_StatusBar + '\n');
       //                      ui->label_Calibration->setVisible(true);
                             vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                             emit workPlaceOff(currentIndicatorNumber);
                             emit checkCalibrationError(currentIndicatorNumber);

                             return;
                      }


                      //
                      //R_Ref2 (14-17)
//                      quint32 R_Ref2Int = (quint8)buffer[14];
//                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[15];
//                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[16];
//                      R_Ref2Int = R_Ref2Int<<8 | (quint8)buffer[17];

//                      float R_Ref2Float = (float)R_Ref2Int/1000000;

//                      qDebug()<<"R_Ref2Float "<<R_Ref2Float;

//                      r_ref2Result4 = QString::number(R_Ref2Float);
                      //

                      if( R_Ref2Float>=(Acceptable_R_Ref2Float4 - Acceptable_R_Ref2Float4/100) &&
                              R_Ref2Float<=(Acceptable_R_Ref2Float4 + Acceptable_R_Ref2Float4/100) ) { //если выходит за допустимые пределы

                          isR_Ref2_Correct4 = true;

                      }
                      else {

    //                      QMessageBox::information(this, "", "Недопустимое значение R_Ref2. Рабочее место: "
    //                                               + QString::number(workPlaceNumber+1));
                          label_StatusBar = ("Недопустимое значение R_Ref2. Рабочее место: "
                                                       + QString::number(workPlaceNumber+1) + ".R_Ref2 " + QString::number(R_Ref2Float));
                          emit errorStringSignal(label_StatusBar + '\n');
       //                   ui->label_Calibration->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkCalibrationError(currentIndicatorNumber);

                          return;
                      }

                      //
                      //
                      //R_Ref2 (14-17) /

                     }

                     //---------------------------------------------------------


                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка данных") +
    //                                               " Рабочее место: " + QString::number(workPlaceNumber+1));
                          label_StatusBar = (tr("Ошибка данных") +
                                                       " Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
      //                    ui->label_Calibration->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkCalibrationError(currentIndicatorNumber);

                          return;
                       } else{}
                   }



               }


            }

            //
            //
            //RMV 5a 00 05 04
            int j=0; //счётчик удачных попыток прочтения
            for(int i=0; i<10;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x05;
               packetToRead.append(byte);
               byte = 0x04;
               packetToRead.append(byte);

               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
    //                                                 " Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                " Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkCalibrationError(currentIndicatorNumber);

                   return;
               }

               global::pause(600);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==9) {
    //                   QMessageBox::information(this, "", tr("Не удалось прочитать данные") +
    //                                            " Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Не удалось прочитать данные") +
                                                    " Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkCalibrationError(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x20;
                   bytesForChecking.append(byte);
                   byte = 0x84;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 9) {
                           label_StatusBar = (tr("Не удалось прочитать данные") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkCalibrationError(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

          //         quint8 error = buffer.at(4);



                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0]
                           && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3]) {

                      j=j+1;
                      emit textBrowser("j = " + QString::number(j));

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                     if(j==3) {

                      QByteArray errorFlags;
                      errorFlags.clear();
                      errorFlags.append((quint8)buffer[22]);
                      errorFlags.append((quint8)buffer[23]);
                      errorFlags.append((quint8)buffer[24]);
                      emit textBrowser("RMV_ErrorFlags " + errorFlags.toHex());

                      quint8 errorFlagsQuint8 = (quint8)buffer[24];
                      errorFlagsQuint8 = errorFlagsQuint8 & 0x0f;
                      qDebug()<<"QString::number(errorFlagsQuint8) "<<QString::number(errorFlagsQuint8);

                     //--------------------------------------------------------
                     if(workPlaceNumber == 0) {

                      if((errorFlagsQuint8 == 1) || (errorFlagsQuint8 == 3)) {
                          emit signalRMV(0, "Магнит сейчас", currentIndicatorNumber);
//                          ui->lineEdit_RMV_ErrorFlags->setText("Магнит сейчас");
//                          ui->label_RMV_ErrorFlags_tick->setStyleSheet(QString("color: green").arg(color.name()));
//                          ui->label_RMV_ErrorFlags_tick->setText("V");
//                          ui->label_RMV_ErrorFlags_tick->setVisible(true);
                      }
                      if((errorFlagsQuint8 == 2)/*|| (errorFlagsQuint8 == 0)*/) {
                          emit signalRMV(1, "Магнит был", currentIndicatorNumber);
//                          ui->lineEdit_RMV_ErrorFlags->setText("Магнит был");
//                          ui->label_RMV_ErrorFlags_tick->setVisible(true);

                      }

         //             ui->lineEdit_RMV_ErrorFlags->setText(errorFlags.toHex());

                      //
                      //TMeasDir (18, 19)
                      quint16 TMeasDirInt = (quint8)buffer[18];
                      TMeasDirInt = TMeasDirInt<<8 | (quint8)buffer[19];

                      float TMeasDirFloat = (float)TMeasDirInt/100;

                      qDebug()<<"TMeasDirFloat "<<TMeasDirFloat;

                      if(TMeasDirFloat>=(temperature1Float - 0.1/*(temperature1Float/1000)*2*/) &&  //если выходит за допустимые пределы
                           TMeasDirFloat<=(temperature1Float + 0.1/*(temperature1Float/1000)*2*/)) {

                          isTMeasDirCorrect = true;

                      }
                      else {
    //                      QMessageBox::information(this, "", "Недопустимое значение TMeasDir. Рабочее место: "
    //                                               + QString::number(workPlaceNumber+1));
                          label_StatusBar = ("Недопустимое значение TMeasDir. Рабочее место: "
                                                       + QString::number(workPlaceNumber+1) + ". TMeasDirFloat " + QString::number(TMeasDirFloat));
                          emit errorStringSignal(label_StatusBar + '\n');
          //                ui->label_Calibration->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkCalibrationError(currentIndicatorNumber);

                          return;

                      }


                      //
                      //TMeasRev (20, 21)
                      quint16 TMeasRevInt = (quint8)buffer[20];
                      TMeasRevInt = TMeasRevInt<<8 | (quint8)buffer[21];

                      float TMeasRevFloat = (float)TMeasRevInt/100;

                      qDebug()<<"TMeasRevFloat "<<TMeasRevFloat;

                      if(TMeasRevFloat>=(temperature2Float - 0.1/*(temperature2Float/1000)*2*/) &&  //если выходит за допустимые пределы
                           TMeasRevFloat<=(temperature2Float + 0.1/*(temperature2Float/1000)*2*/)) {

                          isTMeasRevCorrect = true;

                      }
                      else {
    //                         QMessageBox::information(this, "", "Недопустимое значение TMeasRev. Рабочее место: "
    //                                                  + QString::number(workPlaceNumber+1));
                             label_StatusBar = ("Недопустимое значение TMeasRev. Рабочее место: "
                                                          + QString::number(workPlaceNumber+1) + ". TMeasRevFloat " + QString::number(TMeasRevFloat));
                             emit errorStringSignal(label_StatusBar + '\n');
          //                   ui->label_Calibration->setVisible(true);
                             vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                             emit workPlaceOff(currentIndicatorNumber);
                             emit checkCalibrationError(currentIndicatorNumber);

                             return;

                      }

                     }
                     //----------------------------------------------------
                     if(workPlaceNumber == 1) {

                      if((errorFlagsQuint8 == 1) || (errorFlagsQuint8 == 3)) {
                          emit signalRMV(0, "Магнит сейчас", currentIndicatorNumber);
//                          ui->lineEdit_RMV_ErrorFlags_2->setText("Магнит сейчас");
//                          ui->label_RMV_ErrorFlags_tick_2->setStyleSheet(QString("color: green").arg(color.name()));
//                          ui->label_RMV_ErrorFlags_tick_2->setText("V");
//                          ui->label_RMV_ErrorFlags_tick_2->setVisible(true);
                      }
                      if((errorFlagsQuint8 == 2) /*|| (errorFlagsQuint8 == 0)*/) {
                          emit signalRMV(1, "Магнит был", currentIndicatorNumber);
//                          ui->lineEdit_RMV_ErrorFlags_2->setText("Магнит был");
//                          ui->label_RMV_ErrorFlags_tick_2->setVisible(true);

                      }

         //             ui->lineEdit_RMV_ErrorFlags->setText(errorFlags.toHex());

                      //
                      //TMeasDir (18, 19)
                      quint16 TMeasDirInt = (quint8)buffer[18];
                      TMeasDirInt = TMeasDirInt<<8 | (quint8)buffer[19];

                      float TMeasDirFloat = (float)TMeasDirInt/100;

                      qDebug()<<"TMeasDirFloat "<<TMeasDirFloat;

                      if(TMeasDirFloat>=(temperature1Float2 - 0.1/*(temperature1Float2/1000)*2*/) &&  //если выходит за допустимые пределы
                           TMeasDirFloat<=(temperature1Float2 + 0.1/*(temperature1Float2/1000)*2*/)) {

                          isTMeasDirCorrect2 = true;

                      }
                      else {
    //                      QMessageBox::information(this, "", "Недопустимое значение TMeasDir. Рабочее место: "
    //                                               + QString::number(workPlaceNumber+1));
                          label_StatusBar = ("Недопустимое значение TMeasDir. Рабочее место: "
                                                       + QString::number(workPlaceNumber+1) + ". TMeasDirFloat " + QString::number(TMeasDirFloat));
                          emit errorStringSignal(label_StatusBar + '\n');
        //                  ui->label_Calibration->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkCalibrationError(currentIndicatorNumber);

                          return;

                      }


                      //
                      //TMeasRev (20, 21)
                      quint16 TMeasRevInt = (quint8)buffer[20];
                      TMeasRevInt = TMeasRevInt<<8 | (quint8)buffer[21];

                      float TMeasRevFloat = (float)TMeasRevInt/100;

                      qDebug()<<"TMeasRevFloat "<<TMeasRevFloat;

                      if(TMeasRevFloat>=(temperature2Float2 - 0.1/*(temperature2Float2/1000)*2*/) &&  //если выходит за допустимые пределы
                           TMeasRevFloat<=(temperature2Float2 + 0.1/*(temperature2Float2/1000)*2*/)) {

                          isTMeasRevCorrect2 = true;

                      }
                      else {
    //                         QMessageBox::information(this, "", "Недопустимое значение TMeasRev. Рабочее место: "
    //                                                  + QString::number(workPlaceNumber+1));
                             label_StatusBar = ("Недопустимое значение TMeasRev. Рабочее место: "
                                                          + QString::number(workPlaceNumber+1) + ". TMeasRevFloat " + QString::number(TMeasRevFloat));
                             emit errorStringSignal(label_StatusBar + '\n');
       //                      ui->label_Calibration->setVisible(true);
                             vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                             emit workPlaceOff(currentIndicatorNumber);
                             emit checkCalibrationError(currentIndicatorNumber);

                             return;

                      }

                     }


                     //----------------------------------------------------
                     if(workPlaceNumber == 2) {

                      if((errorFlagsQuint8 == 1) || (errorFlagsQuint8 == 3)) {
                          emit signalRMV(0, "Магнит сейчас", currentIndicatorNumber);
//                          ui->lineEdit_RMV_ErrorFlags_3->setText("Магнит сейчас");
//                          ui->label_RMV_ErrorFlags_tick_3->setStyleSheet(QString("color: green").arg(color.name()));
//                          ui->label_RMV_ErrorFlags_tick_3->setText("V");
//                          ui->label_RMV_ErrorFlags_tick_3->setVisible(true);
                      }
                      if((errorFlagsQuint8 == 2) /*|| (errorFlagsQuint8 == 0)*/) {
                          emit signalRMV(1, "Магнит был", currentIndicatorNumber);
//                          ui->lineEdit_RMV_ErrorFlags_3->setText("Магнит был");
//                          ui->label_RMV_ErrorFlags_tick_3->setVisible(true);

                      }

         //             ui->lineEdit_RMV_ErrorFlags->setText(errorFlags.toHex());

                      //
                      //TMeasDir (18, 19)
                      quint16 TMeasDirInt = (quint8)buffer[18];
                      TMeasDirInt = TMeasDirInt<<8 | (quint8)buffer[19];

                      float TMeasDirFloat = (float)TMeasDirInt/100;

                      qDebug()<<"TMeasDirFloat "<<TMeasDirFloat;

                      if(TMeasDirFloat>=(temperature1Float3 - 0.1/*(temperature1Float3/1000)*2*/) &&  //если выходит за допустимые пределы
                           TMeasDirFloat<=(temperature1Float3 + 0.1/*(temperature1Float3/1000)*2*/)) {

                          isTMeasDirCorrect3 = true;

                      }
                      else {
    //                      QMessageBox::information(this, "", "Недопустимое значение TMeasDir. Рабочее место: "
    //                                               + QString::number(workPlaceNumber+1));
                          label_StatusBar = ("Недопустимое значение TMeasDir. Рабочее место: "
                                                       + QString::number(workPlaceNumber+1) + ". TMeasDirFloat " + QString::number(TMeasDirFloat));
                          emit errorStringSignal(label_StatusBar + '\n');
         //                 ui->label_Calibration->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkCalibrationError(currentIndicatorNumber);

                          return;

                      }


                      //
                      //TMeasRev (20, 21)
                      quint16 TMeasRevInt = (quint8)buffer[20];
                      TMeasRevInt = TMeasRevInt<<8 | (quint8)buffer[21];

                      float TMeasRevFloat = (float)TMeasRevInt/100;

                      qDebug()<<"TMeasRevFloat "<<TMeasRevFloat;

                      if(TMeasRevFloat>=(temperature2Float3 - 0.1/*(temperature2Float3/1000)*2*/) &&  //если выходит за допустимые пределы
                           TMeasRevFloat<=(temperature2Float3 + 0.1/*(temperature2Float3/1000)*2*/)) {

                          isTMeasRevCorrect3 = true;

                      }
                      else {
    //                         QMessageBox::information(this, "", "Недопустимое значение TMeasRev. Рабочее место: "
    //                                                  + QString::number(workPlaceNumber+1));
                             label_StatusBar = ("Недопустимое значение TMeasRev. Рабочее место: "
                                                          + QString::number(workPlaceNumber+1) + ". TMeasRevFloat " + QString::number(TMeasRevFloat));
                             emit errorStringSignal(label_StatusBar + '\n');
       //                      ui->label_Calibration->setVisible(true);
                             vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                             emit workPlaceOff(currentIndicatorNumber);
                             emit checkCalibrationError(currentIndicatorNumber);

                             return;

                      }

                     }


                     //----------------------------------------------------
                     if(workPlaceNumber == 3) {

                      if((errorFlagsQuint8 == 1) || (errorFlagsQuint8 == 3)) {
                          emit signalRMV(0, "Магнит сейчас", currentIndicatorNumber);
//                          ui->lineEdit_RMV_ErrorFlags_4->setText("Магнит сейчас");
//                          ui->label_RMV_ErrorFlags_tick_4->setStyleSheet(QString("color: green").arg(color.name()));
//                          ui->label_RMV_ErrorFlags_tick_4->setText("V");
//                          ui->label_RMV_ErrorFlags_tick_4->setVisible(true);
                      }
                      if((errorFlagsQuint8 == 2) /*|| (errorFlagsQuint8 == 0)*/) {
                          emit signalRMV(1, "Магнит был", currentIndicatorNumber);
//                          ui->lineEdit_RMV_ErrorFlags_4->setText("Магнит был");
//                          ui->label_RMV_ErrorFlags_tick_4->setVisible(true);

                      }

         //             ui->lineEdit_RMV_ErrorFlags->setText(errorFlags.toHex());

                      //
                      //TMeasDir (18, 19)
                      quint16 TMeasDirInt = (quint8)buffer[18];
                      TMeasDirInt = TMeasDirInt<<8 | (quint8)buffer[19];

                      float TMeasDirFloat = (float)TMeasDirInt/100;

                      qDebug()<<"TMeasDirFloat "<<TMeasDirFloat;

                      if(TMeasDirFloat>=(temperature1Float4 - 0.1/*(temperature1Float4/1000)*2*/) &&  //если выходит за допустимые пределы
                           TMeasDirFloat<=(temperature1Float4 + 0.1/*(temperature1Float4/1000)*2*/)) {

                          isTMeasDirCorrect4 = true;

                      }
                      else {
    //                      QMessageBox::information(this, "", "Недопустимое значение TMeasDir. Рабочее место: "
    //                                               + QString::number(workPlaceNumber+1));
                          label_StatusBar = ("Недопустимое значение TMeasDir. Рабочее место: "
                                                       + QString::number(workPlaceNumber+1) + ". TMeasDirFloat " + QString::number(TMeasDirFloat));
                          emit errorStringSignal(label_StatusBar + '\n');
       //                   ui->label_Calibration->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkCalibrationError(currentIndicatorNumber);

                          return;

                      }


                      //
                      //TMeasRev (20, 21)
                      quint16 TMeasRevInt = (quint8)buffer[20];
                      TMeasRevInt = TMeasRevInt<<8 | (quint8)buffer[21];

                      float TMeasRevFloat = (float)TMeasRevInt/100;

                      qDebug()<<"TMeasRevFloat "<<TMeasRevFloat;

                      if(TMeasRevFloat>=(temperature2Float4 - 0.1/*(temperature2Float4/1000)*2*/) &&  //если выходит за допустимые пределы
                           TMeasRevFloat<=(temperature2Float4 + 0.1/*(temperature2Float4/1000)*2*/)) {

                          isTMeasRevCorrect4 = true;

                      }
                      else {
    //                         QMessageBox::information(this, "", "Недопустимое значение TMeasRev. Рабочее место: "
    //                                                  + QString::number(workPlaceNumber+1));
                             label_StatusBar = ("Недопустимое значение TMeasRev. Рабочее место: "
                                                          + QString::number(workPlaceNumber+1) + ". TMeasRevFloat " + QString::number(TMeasRevFloat));
                             emit errorStringSignal(label_StatusBar + '\n');
          //                   ui->label_Calibration->setVisible(true);
                             vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                             emit workPlaceOff(currentIndicatorNumber);
                             emit checkCalibrationError(currentIndicatorNumber);

                             return;

                      }

                     }


                      //----------------------------------------------------






                      break;


                    }//ifend


         //             break;
                   }
                   else {
                       if(i==9) {
    //                      QMessageBox::information(this, "", tr("Ошибка данных") +
    //                                               " Рабочее место: " + QString::number(workPlaceNumber+1));
                          label_StatusBar = (tr("Ошибка данных") +
                                                       " Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
         //                 ui->label_Calibration->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkCalibrationError(currentIndicatorNumber);

                          return;
                       }
                   }



               }


            }


            //-----------------------Проверка результата калибровки-----------------------
          if(workPlaceNumber == 0) {

            if((isR_Ref1_Correct == true) && (isR_Ref2_Correct == true) && (isTMeasDirCorrect == true) && (isTMeasRevCorrect == true)) {
                isCalibrationCorrect = true;
    //            label_StatusBar = ("Калибровка : успешно. Рабочее место: "
    //                                         + QString::number(workPlaceNumber+1));
    //            emit errorStringSignal(label_StatusBar);
       //         ui->label_Calibration->setStyleSheet(QString("color: green").arg(color.name()));
       //         ui->label_Calibration->setText("V");
      //          ui->label_Calibration->setVisible(true);
                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = false;
                emit checkCalibrationError(currentIndicatorNumber);
            }
            else {
                label_StatusBar = ("Калибровка : неудачно. Рабочее место: "
                                             + QString::number(workPlaceNumber+1));
                emit errorStringSignal(label_StatusBar + '\n');
       //         ui->label_Calibration->setVisible(true);
                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                emit workPlaceOff(currentIndicatorNumber);
                emit checkCalibrationError(currentIndicatorNumber);

            }

          }

            //
            //

          if(workPlaceNumber == 1) {

            if((isR_Ref1_Correct2 == true) && (isR_Ref2_Correct2 == true) && (isTMeasDirCorrect2 == true) && (isTMeasRevCorrect2 == true)) {
                isCalibrationCorrect2 = true;
    //            label_StatusBar = ("Калибровка : успешно. Рабочее место: "
    //                                         + QString::number(workPlaceNumber+1));
    //            emit errorStringSignal(label_StatusBar);
//                ui->label_Calibration->setStyleSheet(QString("color: green").arg(color.name()));
//                ui->label_Calibration->setText("V");
     //           ui->label_Calibration->setVisible(true);
                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = false;
                emit checkCalibrationError(currentIndicatorNumber);
            }
            else {
                label_StatusBar = ("Калибровка : неудачно. Рабочее место: "
                                             + QString::number(workPlaceNumber+1));
                emit errorStringSignal(label_StatusBar + '\n');
       //         ui->label_Calibration->setVisible(true);
                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                emit workPlaceOff(currentIndicatorNumber);
                emit checkCalibrationError(currentIndicatorNumber);

            }

          }

            //
            //

          if(workPlaceNumber == 2) {

            if((isR_Ref1_Correct3 == true) && (isR_Ref2_Correct3 == true) && (isTMeasDirCorrect3 == true) && (isTMeasRevCorrect3 == true)) {
                isCalibrationCorrect3 = true;
    //            label_StatusBar = ("Калибровка : успешно. Рабочее место: "
    //                                         + QString::number(workPlaceNumber+1));
    //            emit errorStringSignal(label_StatusBar);
//                ui->label_Calibration->setStyleSheet(QString("color: green").arg(color.name()));
//                ui->label_Calibration->setText("V");
    //            ui->label_Calibration->setVisible(true);
                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = false;
                emit checkCalibrationError(currentIndicatorNumber);
            }
            else {
                label_StatusBar = ("Калибровка : неудачно. Рабочее место: "
                                             + QString::number(workPlaceNumber+1));
                emit errorStringSignal(label_StatusBar + '\n');
     //           ui->label_Calibration->setVisible(true);
                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                emit workPlaceOff(currentIndicatorNumber);
                emit checkCalibrationError(currentIndicatorNumber);

            }

          }

            //
            //

          if(workPlaceNumber == 3) {

            if((isR_Ref1_Correct4 == true) && (isR_Ref2_Correct4 == true) && (isTMeasDirCorrect4 == true) && (isTMeasRevCorrect4 == true)) {
                isCalibrationCorrect4 = true;
    //            label_StatusBar = ("Калибровка : успешно. Рабочее место: "
    //                                         + QString::number(workPlaceNumber+1));
    //            emit errorStringSignal(label_StatusBar);
//                ui->label_Calibration->setStyleSheet(QString("color: green").arg(color.name()));
//                ui->label_Calibration->setText("V");
       //         ui->label_Calibration->setVisible(true);
                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = false;
                emit checkCalibrationError(currentIndicatorNumber);
            }
            else {
                label_StatusBar = ("Калибровка : неудачно. Рабочее место: "
                                             + QString::number(workPlaceNumber+1));
                emit errorStringSignal(label_StatusBar + '\n');
       //         ui->label_Calibration->setVisible(true);
                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                emit workPlaceOff(currentIndicatorNumber);
                emit checkCalibrationError(currentIndicatorNumber);

            }

          }


}

void ObjectThread::slotPulsesOutputHeat(QSerialPort *port1, QSerialPort *port2,
                                        QSerialPort *port3, QSerialPort *port4)
{

    if(!isWorkPlaceUse.at(workPlace)) return;

    int currentIndicatorNumber = workPlace;
    int currentBoxNumber = 2;

    //сбросить счётчик импульсов стенда
    if(!pulsesReset()) {
        vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
        emit workPlaceOff(currentIndicatorNumber);
        emit checkPulsesOutputHeat(currentIndicatorNumber);

        return;
    }

    qDebug()<<"ObjectThread::slotWriteParams() "<<"workPlace "<<QString::number(workPlace);

        QSerialPort *portTmp;

        bool isNeedPaintEvent = true;//false;

        int workPlaceNumber = workPlace;

        QString label_StatusBar;
        QString errorString;

        switch (workPlaceNumber) {
        case 0:
            portTmp = port1;
            break;
        case 1:
            portTmp = port2;
            break;
        case 2:
            portTmp = port3;
            break;
        case 3:
            portTmp = port4;
            break;

        default:
            break;
        }

        portTmp->setBaudRate(QSerialPort::Baud19200);
        portTmp->setDataBits(QSerialPort::Data8);
        portTmp->setParity(QSerialPort::NoParity);
        portTmp->setStopBits(QSerialPort::OneStop);



   //     vectorIsErrorOccured[workPlaceNumber] = false;


            QColor color(255, 127, 50);
//            ui->label_pulsesOutputHeat->setStyleSheet(QString("color: red").arg(color.name()));
//            ui->label_pulsesOutputHeat->setText("X");
//            ui->label_pulsesOutputHeat->setVisible(false);

   //         label_StatusBar = ("");


            QByteArray pulsesMode1ByteArray;
            QByteArray pulsesMode2ByteArray;

            QByteArray pulsesValue1ByteArray;
            QByteArray pulsesValue2ByteArray;

            pulsesMode1ByteArray.clear();
            pulsesMode2ByteArray.clear();
            quint8 byte = 1;
            pulsesMode1ByteArray.append(byte);
            pulsesMode2ByteArray.append(byte);
            //
            pulsesValue1ByteArray.clear();
            pulsesValue2ByteArray.clear();
            quint16 twoBytes = 100;
            pulsesValue1ByteArray.append((quint8)(twoBytes>>8));
            pulsesValue1ByteArray.append((quint8)(twoBytes));

            pulsesValue2ByteArray.append((quint8)(twoBytes>>8));
            pulsesValue2ByteArray.append((quint8)(twoBytes));
            //
            QByteArray buffer;

            if(!portTmp->isOpen()) {
                if(!portTmp->open(QIODevice::ReadWrite)) {
    //                QMessageBox::information(this, "", "Не удалось открыть порт УСО-2. Рабочее место: " +
    //                                         QString::number(workPlaceNumber + 1));
                    label_StatusBar = ("Не удалось открыть порт УСО-2. Рабочее место: " +
                                                 QString::number(workPlaceNumber + 1));
                    emit errorStringSignal(label_StatusBar + '\n');
                    vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                    emit workPlaceOff(currentIndicatorNumber);
                    emit checkPulsesOutputHeat(currentIndicatorNumber);

                    return;
                }
            }

            //одна команда Write Software Configuration с открытием парольной сессии

            //Открыть парольную сессию 5a 00 08 40 01 04 57
            for(int i=0; i<10;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x08;
               packetToRead.append(byte);
               byte = 0x40;
               packetToRead.append(byte);
               byte = 0x01;
               packetToRead.append(byte);
               byte = 0x04;
               packetToRead.append(byte);
               byte = 0x57;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
    //                                                 " Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkPulsesOutputHeat(currentIndicatorNumber);

                   return;
               }

               global::pause(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==9) {
    //                   QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") +
    //                                                         " Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Ошибка открытия парольной сессии") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkPulsesOutputHeat(currentIndicatorNumber);

       //                ui->label_pulsesOutputHeat->setVisible(true);
                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xc0;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 9) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkPulsesOutputHeat(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);


                      //
                      //



                      break;
                   }
                   else {
                            if(i==9) {
    //                            QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") +
    //                                                     " Рабочее место: " + QString::number(workPlaceNumber+1));
                                label_StatusBar = (tr("Ошибка открытия парольной сессии") +
                                                             ". Рабочее место: " + QString::number(workPlaceNumber+1));
                                emit errorStringSignal(label_StatusBar + '\n');
                                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                                emit workPlaceOff(currentIndicatorNumber);
                                emit checkPulsesOutputHeat(currentIndicatorNumber);

         //                       ui->label_pulsesOutputHeat->setVisible(true);
                                return;
                            }

                   }



               }


            }

            //Открыть парольную сессию 5a 00 08 40 01 04 57 /

            //
            //Write Software Configuration 5a 00 16 20
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x16;
               packetToRead.append(byte);
               byte = 0x20;
               packetToRead.append(byte);

               byte = paramsMapToThreads["flowMinByteArray"].toByteArray()[0]; //flowMinByteArray[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowMinByteArray"].toByteArray()[1];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(flowMinByteArray);
               mapwrite["flowMinByteArray"] = paramsMapToThreads["flowMinByteArray"].toByteArray();


               byte = paramsMapToThreads["flowMaxByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowMaxByteArray"].toByteArray()[1];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(flowMaxByteArray);
               mapwrite["flowMaxByteArray"] = paramsMapToThreads["flowMaxByteArray"].toByteArray();


               byte = paramsMapToThreads["tinMinByteArray"].toByteArray()[0];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(tinMinByteArray);
               mapwrite["tinMinByteArray"] = paramsMapToThreads["tinMinByteArray"].toByteArray();


               byte = paramsMapToThreads["tinMaxByteArray"].toByteArray()[0];
               packetToRead.append(byte);
      //         paramsVectorToWrite.append(tinMaxByteArray);
               mapwrite["tinMaxByteArray"] = paramsMapToThreads["tinMaxByteArray"].toByteArray();


               byte = paramsMapToThreads["toutMinByteArray"].toByteArray()[0];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(toutMinByteArray);
               mapwrite["toutMinByteArray"] = paramsMapToThreads["toutMinByteArray"].toByteArray();


               byte = paramsMapToThreads["toutMaxByteArray"].toByteArray()[0];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(toutMaxByteArray);
               mapwrite["toutMaxByteArray"] = paramsMapToThreads["toutMaxByteArray"].toByteArray();


               byte = paramsMapToThreads["reportDataByteArray"].toByteArray()[0];
               packetToRead.append(byte);
 //              paramsVectorToWrite.append(reportDataByteArray);
               mapwrite["reportDataByteArray"] = paramsMapToThreads["reportDataByteArray"].toByteArray();


               byte = paramsMapToThreads["averageTByteArray"].toByteArray()[0];
               packetToRead.append(byte);
      //         paramsVectorToWrite.append(averageTByteArray);
               mapwrite["averageTByteArray"] = paramsMapToThreads["averageTByteArray"].toByteArray();


               byte = pulsesMode1ByteArray[0];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(pulsesMode1ByteArray);
               mapwrite["pulsesMode1ByteArray"] = pulsesMode1ByteArray;


               byte = pulsesMode2ByteArray[0];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(pulsesMode2ByteArray);
               mapwrite["pulsesMode2ByteArray"] = pulsesMode2ByteArray;


               byte = pulsesValue1ByteArray[0];
               packetToRead.append(byte);
               byte = pulsesValue1ByteArray[1];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(pulsesValue1ByteArray);
               mapwrite["pulsesValue1ByteArray"] = pulsesValue1ByteArray;


               byte = pulsesValue2ByteArray[0];
               packetToRead.append(byte);
               byte = pulsesValue2ByteArray[1];
               packetToRead.append(byte);
   //            paramsVectorToWrite.append(pulsesValue2ByteArray);
               mapwrite["pulsesValue2ByteArray"] = pulsesValue2ByteArray;

               byte = paramsMapToThreads["devType"].toByteArray()[0];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(devType);
               mapwrite["devType"] = paramsMapToThreads["devType"].toByteArray();

               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
    //                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkPulsesOutputHeat(currentIndicatorNumber);

                   return;
               }

               global::pause(250);//(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось записать данные : Software Configuration") +
    //                                            " Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Не удалось записать данные : Software Configuration") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkPulsesOutputHeat(currentIndicatorNumber);

        //               ui->label_pulsesOutputHeat->setVisible(true);
                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xa0;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkPulsesOutputHeat(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer[4];

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                      //


                      //
                      //



                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка записи данных : Software Configuration") +
    //                                               " Рабочее место: " + QString::number(workPlaceNumber+1));
                          label_StatusBar = (tr("Ошибка записи данных : Software Configuration") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkPulsesOutputHeat(currentIndicatorNumber);

        //                  ui->label_pulsesOutputHeat->setVisible(true);
                          return;
                       }
                   }



               }


            }


            //
            //Read Software Configuration 5a000502
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x05;
               packetToRead.append(byte);
               byte = 0x02;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
    //                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkPulsesOutputHeat(currentIndicatorNumber);

                   return;
               }

               global::pause(150);//(200);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось прочитать данные") +
    //                                            " Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Не удалось прочитать данные") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkPulsesOutputHeat(currentIndicatorNumber);

       //                ui->label_pulsesOutputHeat->setVisible(true);
                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x16;
                   bytesForChecking.append(byte);
                   byte = 0x82;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkPulsesOutputHeat(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

         //          quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3]) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);
        //              ui->textBrowser->append("buffer.toHex()     " + buffer.toHex());

        //              emit sendbufferReadSoftWareConfiguration(buffer);

                      //
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
    //                      ui->lineEdit_Flow_min->setText(QString::number(flowMinFloat));
                      }
                      else {
         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Минимальный расход"));
    //                      ui->lineEdit_Flow_min->setText(QString::number(flowMinFloat));
                      }

//                      qDebug()<<"flowMinArray.toHex() "<<flowMinArray.toHex();
//                      paramsVector.append(flowMinArray);
                      mapRead["flowMinByteArray"] = flowMinArray;

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
//                      if(flowMaxFloat>=2 && flowMaxFloat<=5) {
//                          ui->lineEdit_Flow_max->setText(QString::number(flowMaxFloat));
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Максимальный расход"));
//                          ui->lineEdit_Flow_max->setText(QString::number(flowMaxFloat));
//                      }

//                      qDebug()<<"flowMaxArray.toHex() "<<flowMaxArray.toHex();
//                      paramsVector.append(flowMaxArray);
                      mapRead["flowMaxByteArray"] = flowMaxArray;

                      //
                      //tinMin
                      QByteArray tinMinArray;
                      tinMinArray.append(buffer[8]);


                      quint8 tinMinQuint8 = buffer[8];
//                      if(tinMinQuint8>=10 && tinMinQuint8<=60) {
//                          ui->spinBox_Tin_min->setValue(tinMinQuint8);
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Минимальная температура подачи"));
//                      }

//                      qDebug()<<"tinMinArray.toHex() "<<tinMinArray.toHex();
//                      paramsVector.append(tinMinArray);
                      mapRead["tinMinByteArray"] = tinMinArray;

                      //
                      //tinMax
                      QByteArray tinMaxArray;
                      tinMaxArray.append(buffer[9]);


                      quint8 tinMaxQuint8 = buffer[9];
//                      if(tinMaxQuint8>=50 && tinMaxQuint8<=150) {
//                          ui->spinBox_Tin_max->setValue(tinMaxQuint8);
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Максимальная температура подачи"));
//                      }

//                      qDebug()<<"tinMaxArray.toHex() "<<tinMaxArray.toHex();
//                      paramsVector.append(tinMaxArray);
                      mapRead["tinMaxByteArray"] = tinMaxArray;

                      //
                      //toutMin
                      QByteArray toutMinArray;
                      toutMinArray.append(buffer[10]);

                      quint8 toutMinQuint8 = buffer[10];
//                      if(toutMinQuint8>=10 && toutMinQuint8<=60) {
//                          ui->spinBox_Tout_min->setValue(toutMinQuint8);
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Минимальная температура обратки"));
//                      }

//                      qDebug()<<"toutMinArray.toHex() "<<toutMinArray.toHex();
//                      paramsVector.append(toutMinArray);
                      mapRead["toutMinByteArray"] = toutMinArray;


                      //
                      //toutMax
                      QByteArray toutMaxArray;
                      toutMaxArray.append(buffer[11]);

                      quint8 toutMaxQuint8 = buffer[11];
//                      if(toutMaxQuint8>=50 && toutMaxQuint8<=150) {
//                          ui->spinBox_Tout_max->setValue(toutMaxQuint8);
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Максимальная температура обратки"));
//                      }

//                      qDebug()<<"toutMaxArray.toHex() "<<toutMaxArray.toHex();
//                      paramsVector.append(toutMaxArray);
                      mapRead["toutMaxByteArray"] = toutMaxArray;

                      //
                      //reportData
                      QByteArray reportDataArray;
                      reportDataArray.append(buffer[12]);

                      quint8 reportDataInt = buffer[12];
//                      if(reportDataInt>=0 && reportDataInt<=31) {
//                          ui->spinBox_ReportData->setValue(reportDataInt);
//                      }
//                      else {
//          //                QMessageBox::information(this, "", tr("Недопустимое значение: Дата сохранения в журнал"));
//                      }

//                      qDebug()<<"reportDataArray.toHex() "<<reportDataArray.toHex();
//                      paramsVector.append(reportDataArray);
                      mapRead["reportDataByteArray"] = reportDataArray;

                      //
                      //AverageT
                      QByteArray AverageTArray;
                      AverageTArray.append(buffer[13]);


                      quint8 AverageTInt = buffer[13];
//                      if(AverageTList.contains(QString::number(AverageTInt))) {
//                          ui->comboBox_Average_T->setCurrentText(QString::number(AverageTInt));
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Тип усреднения температуры"));
//                      }

//                      qDebug()<<"AverageTArray.toHex() "<<AverageTArray.toHex();
//                      paramsVector.append(AverageTArray);
                      mapRead["averageTByteArray"] = AverageTArray;

                      //
                      //pulsesMode1
                      QByteArray pulsesMode1Array;
                      pulsesMode1Array.append(buffer[14]);


                      quint8 pulsesMode1Int = (quint8)buffer[14];
//                      if(pulsesMode1List.contains(QString::number(pulsesMode1Int))) {
//                          ui->comboBox_PulsesMode1->setCurrentText(QString::number(pulsesMode1Int));
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Режим работы импульсного вывода"));
//                      }

//                      qDebug()<<"pulsesMode1Array.toHex() "<<pulsesMode1Array.toHex();
//                      paramsVector.append(pulsesMode1Array);
                      mapRead["pulsesMode1ByteArray"] = pulsesMode1Array;

                      //
                      //pulsesMode2
                      QByteArray pulsesMode2Array;
                      pulsesMode2Array.append(buffer[15]);


                      quint8 pulsesMode2Int = (quint8)buffer[15];
//                      if(pulsesMode1List.contains(QString::number(pulsesMode2Int))) {
//                          ui->comboBox_PulsesMode2->setCurrentText(QString::number(pulsesMode2Int));
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Режим работы импульсного вывода"));
//                      }

//                      qDebug()<<"pulsesMode2Array.toHex() "<<pulsesMode2Array.toHex();
//                      paramsVector.append(pulsesMode2Array);
                      mapRead["pulsesMode2ByteArray"] = pulsesMode2Array;

                      //
                      //pulsesValue1
                      QByteArray pulsesValue1Array;
                      pulsesValue1Array.append(buffer[16]);
                      pulsesValue1Array.append(buffer[17]);


                      quint16 pulsesValue1Int = buffer[16];
                      pulsesValue1Int = pulsesValue1Int<<8 | (quint8)buffer[17];
        //              if((pulsesValue1Int<=101) && (pulsesValue1Int>=99)) {

        //              }
//                      ui->lineEdit_PulsesValue1->setText(QString::number(pulsesValue1Int));

//                      qDebug()<<"pulsesValue1Array.toHex() "<<pulsesValue1Array.toHex();
//                      paramsVector.append(pulsesValue1Array);
                      mapRead["pulsesValue1ByteArray"] = pulsesValue1Array;

                      //
                      //pulsesValue2
                      QByteArray pulsesValue2Array;
                      pulsesValue2Array.append(buffer[18]);
                      pulsesValue2Array.append(buffer[19]);
                      qDebug()<<"pulsesValue2Array.toHex() "<<pulsesValue2Array.toHex();

                      quint16 pulsesValue2Int = buffer[18];
                      pulsesValue2Int = pulsesValue2Int<<8 | (quint8)buffer[19];
//                      ui->lineEdit_PulsesValue2->setText(QString::number(pulsesValue2Int));

//                      qDebug()<<"pulsesValue2Array.toHex() "<<pulsesValue2Array.toHex();
//                      paramsVector.append(pulsesValue2Array);
                      mapRead["pulsesValue2ByteArray"] = pulsesValue2Array;

                      //
                      //DevType
                      QByteArray devType;
                      devType.append(buffer[20]);

//                      if(devType[0] == 0x04) {
//                          ui->comboBox_DevType->setCurrentText("Обратный");
//                      }

//                      if(devType[0] == 0x0c) {
//                          ui->comboBox_DevType->setCurrentText("Подающий");
//                      }

//                      if((devType[0] != 0x04) && (devType[0] != 0x0c) ) {
//                          QMessageBox::information(this, "", tr("Недопустимое значение: Тип счётчика"));
//                      }

//                      qDebug()<<"devType.toHex() "<<devType.toHex();
//                      paramsVector.append(devType);
                      mapRead["devType"] = devType;


                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка данных") +
    //                                               " Рабочее место: " + QString::number(workPlaceNumber+1));
                          label_StatusBar = (tr("Ошибка данных") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkPulsesOutputHeat(currentIndicatorNumber);

       //                   ui->label_pulsesOutputHeat->setVisible(true);
                          return;
                       }
                   }

               }


            }



            QVector<QByteArray> writeVector;
            writeVector.append(mapwrite["flowMinByteArray"]);
            writeVector.append(mapwrite["flowMaxByteArray"]);
            writeVector.append(mapwrite["tinMinByteArray"]);
            writeVector.append(mapwrite["tinMaxByteArray"]);
            writeVector.append(mapwrite["toutMinByteArray"]);
            writeVector.append(mapwrite["toutMaxByteArray"]);
            writeVector.append(mapwrite["reportDataByteArray"]);
            writeVector.append(mapwrite["averageTByteArray"]);
            writeVector.append(mapwrite["pulsesMode1ByteArray"]);
            writeVector.append(mapwrite["pulsesMode2ByteArray"]);
            writeVector.append(mapwrite["pulsesValue1ByteArray"]);
            writeVector.append(mapwrite["pulsesValue2ByteArray"]);

            QVector<QByteArray> readVector;
            readVector.append(mapRead["flowMinByteArray"]);
            readVector.append(mapRead["flowMaxByteArray"]);
            readVector.append(mapRead["tinMinByteArray"]);
            readVector.append(mapRead["tinMaxByteArray"]);
            readVector.append(mapRead["toutMinByteArray"]);
            readVector.append(mapRead["toutMaxByteArray"]);
            readVector.append(mapRead["reportDataByteArray"]);
            readVector.append(mapRead["averageTByteArray"]);
            readVector.append(mapRead["pulsesMode1ByteArray"]);
            readVector.append(mapRead["pulsesMode2ByteArray"]);
            readVector.append(mapRead["pulsesValue1ByteArray"]);
            readVector.append(mapRead["pulsesValue2ByteArray"]);

            for(int i=0; i<allParamsNameList.size(); i++) {
                qDebug()<<"Write "<<allParamsNameList[i] + " "<<mapwrite[allParamsNameList[i]].toHex();
                qDebug()<<"Read "<<allParamsNameList[i] + " "<<mapRead[allParamsNameList[i]].toHex();
            }

            for(int i=0; i<writeVector.size(); i++) {


                if((i == (writeVector.size() - 2)) || (i == (writeVector.size() - 1))) {
                    int paramRead = (quint8)(readVector[i][0]);
                    paramRead = paramRead<<8 | (quint8)(readVector[i][1]);

                    int paramWrite = (quint8)(writeVector[i][0]);
                    paramWrite = paramWrite<<8 | (quint8)(writeVector[i][1]);

                    if((paramRead >= (paramWrite - 1)) && (paramRead <= (paramWrite + 1))) {

                    }
                    else {
    //                    QMessageBox::information(this, "", "Параметр записан некорректно: запись " + writeVector[i].toHex() +
    //                                             " " + "чтение " + readVector[i].toHex());
                        label_StatusBar = ("Параметр записан некорректно: запись " + writeVector[i].toHex() +
                                                     " " + "чтение " + readVector[i].toHex() +
                                                     ". Рабочее место: " + QString::number(workPlaceNumber+1));
                        emit errorStringSignal(label_StatusBar + '\n');
                        vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                        emit workPlaceOff(currentIndicatorNumber);
                        emit checkPulsesOutputHeat(currentIndicatorNumber);

       //                 ui->label_pulsesOutputHeat->setVisible(true);
                        return;
                    }
                }
                else {
                        if((readVector[i] != writeVector[i]) && (i != (writeVector.size() - 2)) && (i != (writeVector.size() - 1)) ) {

                           label_StatusBar = ("Параметр записан некорректно: запись " + writeVector[i].toHex() +
                                                        " " + "чтение " + readVector[i].toHex() +
                                                        ". Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkPulsesOutputHeat(currentIndicatorNumber);

                           return;
                        }
                }
            }



          //через 3 сек прочитываем кол-во импульсов из стенда
            global::pause(4000);

            readPulsesChannel1();
            readPulsesChannel2();

            emit textBrowser("Импульсы. канал 1 - " + QString::number(pulsesValueCh1) +
                              ",канал 2 - " + QString::number(pulsesValueCh2));

            if( (pulsesValueCh1 < 2) || (pulsesValueCh2 < 2) || (pulsesValueCh1!=pulsesValueCh2) ) {

                //импульсы не работают

                label_StatusBar = ("Импульсные выходы по теплу не работают. Рабочее место: " + QString::number(workPlaceNumber+1));
                emit errorStringSignal(label_StatusBar + '\n');
                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                emit workPlaceOff(currentIndicatorNumber);
                emit checkPulsesOutputHeat(currentIndicatorNumber);

                return;

            }


          emit checkPulsesOutputHeat(currentIndicatorNumber);




}

void ObjectThread::slotPulsesInputVolume(QSerialPort *port1, QSerialPort *port2,
                                         QSerialPort *port3, QSerialPort *port4)
{

    if(!isWorkPlaceUse.at(workPlace)) return;

//    8)	Программа даёт стенду команду выдать количество импульсов на счётные входы платы.
//            Программа считывает из платы по УСО количество импульсов.
//            Если входы работают, то идти дальше,
//            иначе – включить аварийную индикацию на стенде. Остановка.

    int currentIndicatorNumber = workPlace;
    int currentBoxNumber = 3;

//    if(!writePulsesToGenChannel1()) {
//        vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
//        emit workPlaceOff(currentIndicatorNumber);
//        emit checkPulsesInputVolume(currentIndicatorNumber);

//        return;
//    }
//    if(!writePulsesToGenChannel1()) {
//        vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
//        emit workPlaceOff(currentIndicatorNumber);
//        emit checkPulsesInputVolume(currentIndicatorNumber);

//        return;
//    }

//    global::pause(3000);

    qDebug()<<"ObjectThread::slotPulsesInputVolume() "<<"workPlace "<<QString::number(workPlace);

        QSerialPort *portTmp;

        bool isNeedPaintEvent = true;//false;

        int workPlaceNumber = workPlace;



        QString label_StatusBar;
        QString errorString;

        switch (workPlaceNumber) {
        case 0:
            portTmp = port1;
            break;
        case 1:
            portTmp = port2;
            break;
        case 2:
            portTmp = port3;
            break;
        case 3:
            portTmp = port4;
            break;

        default:
            break;
        }

        portTmp->setBaudRate(QSerialPort::Baud19200);
        portTmp->setDataBits(QSerialPort::Data8);
        portTmp->setParity(QSerialPort::NoParity);
        portTmp->setStopBits(QSerialPort::OneStop);

            QColor color(255, 127, 50);

            QByteArray pulsesMode1ByteArray;
            QByteArray pulsesMode2ByteArray;

            QByteArray pulsesValue1ByteArray;
            QByteArray pulsesValue2ByteArray;

            pulsesMode1ByteArray.clear();
            pulsesMode2ByteArray.clear();
            quint8 byte = 2;
            pulsesMode1ByteArray.append(byte);
            pulsesMode2ByteArray.append(byte);
            //
            pulsesValue1ByteArray.clear();
            pulsesValue2ByteArray.clear();
            quint16 twoBytes = 100;
            pulsesValue1ByteArray.append((quint8)(twoBytes>>8));
            pulsesValue1ByteArray.append((quint8)(twoBytes));

            pulsesValue2ByteArray.append((quint8)(twoBytes>>8));
            pulsesValue2ByteArray.append((quint8)(twoBytes));

            QByteArray buffer;

            if(!portTmp->isOpen()) {
                if(!portTmp->open(QIODevice::ReadWrite)) {

                    label_StatusBar = ("Не удалось открыть порт УСО-2. Рабочее место: " +
                                                 QString::number(workPlaceNumber + 1));
                    emit errorStringSignal(label_StatusBar + '\n');
                    vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                    emit workPlaceOff(currentIndicatorNumber);
                    emit checkPulsesInputVolume(currentIndicatorNumber);

                    return;
                }
            }

            //одна команда Write Software Configuration с открытием парольной сессии

            //Открыть парольную сессию 5a 00 08 40 01 04 57
            for(int i=0; i<10;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x08;
               packetToRead.append(byte);
               byte = 0x40;
               packetToRead.append(byte);
               byte = 0x01;
               packetToRead.append(byte);
               byte = 0x04;
               packetToRead.append(byte);
               byte = 0x57;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                " Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkPulsesInputVolume(currentIndicatorNumber);

                   return;
               }

               global::pause(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==9) {
                       label_StatusBar = (tr("Ошибка открытия парольной сессии") +
                                                    " Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkPulsesInputVolume(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xc0;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 9) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkPulsesInputVolume(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);


                      //
                      //



                      break;
                   }
                   else {
                            if(i==9) {
    //                            QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") +
    //                                                     " Рабочее место: " + QString::number(workPlaceNumber+1));
                                label_StatusBar = (tr("Ошибка открытия парольной сессии") +
                                                             " Рабочее место: " + QString::number(workPlaceNumber+1));
                                emit errorStringSignal(label_StatusBar + '\n');
                                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                                emit workPlaceOff(currentIndicatorNumber);
                                emit checkPulsesInputVolume(currentIndicatorNumber);

      //                          ui->label_pulsesInputVolume->setVisible(true);
                                return;
                            }

                   }



               }


            }

            //
            //Write Software Configuration 5a 00 16 20
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x16;
               packetToRead.append(byte);
               byte = 0x20;
               packetToRead.append(byte);

               byte = paramsMapToThreads["flowMinByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowMinByteArray"].toByteArray()[1];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(flowMinByteArray);
               mapwrite["flowMinByteArray"] = paramsMapToThreads["flowMinByteArray"].toByteArray();


               byte = paramsMapToThreads["flowMaxByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowMaxByteArray"].toByteArray()[1];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(flowMaxByteArray);
               mapwrite["flowMaxByteArray"] = paramsMapToThreads["flowMaxByteArray"].toByteArray();


               byte = paramsMapToThreads["tinMinByteArray"].toByteArray()[0];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(tinMinByteArray);
               mapwrite["tinMinByteArray"] = paramsMapToThreads["tinMinByteArray"].toByteArray();


               byte = paramsMapToThreads["tinMaxByteArray"].toByteArray()[0];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(tinMaxByteArray);
               mapwrite["tinMaxByteArray"] = paramsMapToThreads["tinMaxByteArray"].toByteArray();


               byte = paramsMapToThreads["toutMinByteArray"].toByteArray()[0];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(toutMinByteArray);
               mapwrite["toutMinByteArray"] = paramsMapToThreads["toutMinByteArray"].toByteArray();


               byte = paramsMapToThreads["toutMaxByteArray"].toByteArray()[0];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(toutMaxByteArray);
               mapwrite["toutMaxByteArray"] = paramsMapToThreads["toutMaxByteArray"].toByteArray();


               byte = paramsMapToThreads["reportDataByteArray"].toByteArray()[0];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(reportDataByteArray);
               mapwrite["reportDataByteArray"] = paramsMapToThreads["reportDataByteArray"].toByteArray();


               byte = paramsMapToThreads["averageTByteArray"].toByteArray()[0];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(averageTByteArray);
               mapwrite["averageTByteArray"] = paramsMapToThreads["averageTByteArray"].toByteArray();


               byte = pulsesMode1ByteArray[0];
               packetToRead.append(byte);
       //        paramsVectorToWrite.append(pulsesMode1ByteArray);
               mapwrite["pulsesMode1ByteArray"] = pulsesMode1ByteArray;


               byte = pulsesMode2ByteArray[0];
               packetToRead.append(byte);
        //       paramsVectorToWrite.append(pulsesMode2ByteArray);
               mapwrite["pulsesMode2ByteArray"] = pulsesMode2ByteArray;


               byte = pulsesValue1ByteArray[0];
               packetToRead.append(byte);
               byte = pulsesValue1ByteArray[1];
               packetToRead.append(byte);
      //         paramsVectorToWrite.append(pulsesValue1ByteArray);
               mapwrite["pulsesValue1ByteArray"] = pulsesValue1ByteArray;


               byte = pulsesValue2ByteArray[0];
               packetToRead.append(byte);
               byte = pulsesValue2ByteArray[1];
               packetToRead.append(byte);
       //        paramsVectorToWrite.append(pulsesValue2ByteArray);
               mapwrite["pulsesValue2ByteArray"] = pulsesValue2ByteArray;

               byte = paramsMapToThreads["devType"].toByteArray()[0];
               packetToRead.append(byte);
       //        paramsVectorToWrite.append(devType);
               mapwrite["devType"] = paramsMapToThreads["devType"].toByteArray();

               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
    //                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                " Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkPulsesInputVolume(currentIndicatorNumber);

                   return;
               }

               global::pause(250);//(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось записать данные : Software Configuration") +
    //                                            " Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Не удалось записать данные : Software Configuration") +
                                                    " Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkPulsesInputVolume(currentIndicatorNumber);

     //                  ui->label_pulsesInputVolume->setVisible(true);
                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xa0;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkPulsesInputVolume(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer[4];

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                      //


                      //
                      //



                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка записи данных : Software Configuration") +
    //                                               " Рабочее место: " + QString::number(workPlaceNumber+1));
                          label_StatusBar = (tr("Ошибка записи данных : Software Configuration") +
                                                       " Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkPulsesInputVolume(currentIndicatorNumber);

         //                 ui->label_pulsesInputVolume->setVisible(true);
                          return;
                       }
                   }



               }


            }


            //
            //Read Software Configuration 5a000502
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x05;
               packetToRead.append(byte);
               byte = 0x02;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
    //                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                " Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkPulsesInputVolume(currentIndicatorNumber);

                   return;
               }

               global::pause(150);//(200);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось прочитать данные") +
    //                                            " Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Не удалось прочитать данные") +
                                                    " Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkPulsesInputVolume(currentIndicatorNumber);

      //                 ui->label_pulsesInputVolume->setVisible(true);
                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x16;
                   bytesForChecking.append(byte);
                   byte = 0x82;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkPulsesInputVolume(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

       //            quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3]) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);
        //              ui->textBrowser->append("buffer.toHex()     " + buffer.toHex());

        //              emit sendbufferReadSoftWareConfiguration(buffer);

                      //
                      //flowMin
                      QByteArray flowMinArray;
                      flowMinArray.append(buffer[4]);
                      flowMinArray.append(buffer[5]);
         //             ui->lineEdit_Flow_min->setText(flowMinArray.toHex());

         //             float flowMinFloat = buffer[4];
                      quint16 flowMinQuint16 = buffer[4];
                      flowMinQuint16 = flowMinQuint16<<8 | buffer[5];
                      float flowMinFloat = (float)flowMinQuint16/1000;
//                      if(flowMinFloat>=0 && flowMinFloat<=0.2) {
//                          ui->lineEdit_Flow_min->setText(QString::number(flowMinFloat));
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Минимальный расход"));
//                          ui->lineEdit_Flow_min->setText(QString::number(flowMinFloat));
//                      }

//                      qDebug()<<"flowMinArray.toHex() "<<flowMinArray.toHex();
//                      paramsVector.append(flowMinArray);
                      mapRead["flowMinByteArray"] = flowMinArray;

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
//                      if(flowMaxFloat>=2 && flowMaxFloat<=5) {
//                          ui->lineEdit_Flow_max->setText(QString::number(flowMaxFloat));
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Максимальный расход"));
//                          ui->lineEdit_Flow_max->setText(QString::number(flowMaxFloat));
//                      }

//                      qDebug()<<"flowMaxArray.toHex() "<<flowMaxArray.toHex();
//                      paramsVector.append(flowMaxArray);
                      mapRead["flowMaxByteArray"] = flowMaxArray;

                      //
                      //tinMin
                      QByteArray tinMinArray;
                      tinMinArray.append(buffer[8]);


                      quint8 tinMinQuint8 = buffer[8];
//                      if(tinMinQuint8>=10 && tinMinQuint8<=60) {
//                          ui->spinBox_Tin_min->setValue(tinMinQuint8);
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Минимальная температура подачи"));
//                      }

//                      qDebug()<<"tinMinArray.toHex() "<<tinMinArray.toHex();
//                      paramsVector.append(tinMinArray);
                      mapRead["tinMinByteArray"] = tinMinArray;

                      //
                      //tinMax
                      QByteArray tinMaxArray;
                      tinMaxArray.append(buffer[9]);


                      quint8 tinMaxQuint8 = buffer[9];
//                      if(tinMaxQuint8>=50 && tinMaxQuint8<=150) {
//                          ui->spinBox_Tin_max->setValue(tinMaxQuint8);
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Максимальная температура подачи"));
//                      }

//                      qDebug()<<"tinMaxArray.toHex() "<<tinMaxArray.toHex();
//                      paramsVector.append(tinMaxArray);
                      mapRead["tinMaxByteArray"] = tinMaxArray;

                      //
                      //toutMin
                      QByteArray toutMinArray;
                      toutMinArray.append(buffer[10]);

                      quint8 toutMinQuint8 = buffer[10];
//                      if(toutMinQuint8>=10 && toutMinQuint8<=60) {
//                          ui->spinBox_Tout_min->setValue(toutMinQuint8);
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Минимальная температура обратки"));
//                      }

//                      qDebug()<<"toutMinArray.toHex() "<<toutMinArray.toHex();
//                      paramsVector.append(toutMinArray);
                      mapRead["toutMinByteArray"] = toutMinArray;


                      //
                      //toutMax
                      QByteArray toutMaxArray;
                      toutMaxArray.append(buffer[11]);

                      quint8 toutMaxQuint8 = buffer[11];
//                      if(toutMaxQuint8>=50 && toutMaxQuint8<=150) {
//                          ui->spinBox_Tout_max->setValue(toutMaxQuint8);
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Максимальная температура обратки"));
//                      }

//                      qDebug()<<"toutMaxArray.toHex() "<<toutMaxArray.toHex();
//                      paramsVector.append(toutMaxArray);
                      mapRead["toutMaxByteArray"] = toutMaxArray;

                      //
                      //reportData
                      QByteArray reportDataArray;
                      reportDataArray.append(buffer[12]);

                      quint8 reportDataInt = buffer[12];
//                      if(reportDataInt>=0 && reportDataInt<=31) {
//                          ui->spinBox_ReportData->setValue(reportDataInt);
//                      }
//                      else {
//          //                QMessageBox::information(this, "", tr("Недопустимое значение: Дата сохранения в журнал"));
//                      }

//                      qDebug()<<"reportDataArray.toHex() "<<reportDataArray.toHex();
//                      paramsVector.append(reportDataArray);
                      mapRead["reportDataByteArray"] = reportDataArray;

                      //
                      //AverageT
                      QByteArray AverageTArray;
                      AverageTArray.append(buffer[13]);


                      quint8 AverageTInt = buffer[13];
//                      if(AverageTList.contains(QString::number(AverageTInt))) {
//                          ui->comboBox_Average_T->setCurrentText(QString::number(AverageTInt));
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Тип усреднения температуры"));
//                      }

//                      qDebug()<<"AverageTArray.toHex() "<<AverageTArray.toHex();
//                      paramsVector.append(AverageTArray);
                      mapRead["averageTByteArray"] = AverageTArray;

                      //
                      //pulsesMode1
                      QByteArray pulsesMode1Array;
                      pulsesMode1Array.append(buffer[14]);


                      quint8 pulsesMode1Int = (quint8)buffer[14];
//                      if(pulsesMode1List.contains(QString::number(pulsesMode1Int))) {
//                          ui->comboBox_PulsesMode1->setCurrentText(QString::number(pulsesMode1Int));
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Режим работы импульсного вывода"));
//                      }

//                      qDebug()<<"pulsesMode1Array.toHex() "<<pulsesMode1Array.toHex();
//                      paramsVector.append(pulsesMode1Array);
                      mapRead["pulsesMode1ByteArray"] = pulsesMode1Array;

                      //
                      //pulsesMode2
                      QByteArray pulsesMode2Array;
                      pulsesMode2Array.append(buffer[15]);


                      quint8 pulsesMode2Int = (quint8)buffer[15];
//                      if(pulsesMode1List.contains(QString::number(pulsesMode2Int))) {
//                          ui->comboBox_PulsesMode2->setCurrentText(QString::number(pulsesMode2Int));
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Режим работы импульсного вывода"));
//                      }

//                      qDebug()<<"pulsesMode2Array.toHex() "<<pulsesMode2Array.toHex();
//                      paramsVector.append(pulsesMode2Array);
                      mapRead["pulsesMode2ByteArray"] = pulsesMode2Array;

                      //
                      //pulsesValue1
                      QByteArray pulsesValue1Array;
                      pulsesValue1Array.append(buffer[16]);
                      pulsesValue1Array.append(buffer[17]);


                      quint16 pulsesValue1Int = buffer[16];
                      pulsesValue1Int = pulsesValue1Int<<8 | (quint8)buffer[17];
        //              if((pulsesValue1Int<=101) && (pulsesValue1Int>=99)) {

        //              }
//                      ui->lineEdit_PulsesValue1->setText(QString::number(pulsesValue1Int));

//                      qDebug()<<"pulsesValue1Array.toHex() "<<pulsesValue1Array.toHex();
//                      paramsVector.append(pulsesValue1Array);
                      mapRead["pulsesValue1ByteArray"] = pulsesValue1Array;

                      //
                      //pulsesValue2
                      QByteArray pulsesValue2Array;
                      pulsesValue2Array.append(buffer[18]);
                      pulsesValue2Array.append(buffer[19]);
                      qDebug()<<"pulsesValue2Array.toHex() "<<pulsesValue2Array.toHex();

                      quint16 pulsesValue2Int = buffer[18];
                      pulsesValue2Int = pulsesValue2Int<<8 | (quint8)buffer[19];
      //                ui->lineEdit_PulsesValue2->setText(QString::number(pulsesValue2Int));

//                      qDebug()<<"pulsesValue2Array.toHex() "<<pulsesValue2Array.toHex();
//                      paramsVector.append(pulsesValue2Array);
                      mapRead["pulsesValue2ByteArray"] = pulsesValue2Array;

                      //
                      //DevType
                      QByteArray devType;
                      devType.append(buffer[20]);

//                      if(devType[0] == 0x04) {
//                          ui->comboBox_DevType->setCurrentText("Обратный");
//                      }

//                      if(devType[0] == 0x0c) {
//                          ui->comboBox_DevType->setCurrentText("Подающий");
//                      }

                      if((devType[0] != 0x04) && (devType[0] != 0x0c) ) {
    //                      QMessageBox::information(this, "", tr("Недопустимое значение: Тип счётчика") +
    //                                               " Рабочее место: " + QString::number(workPlaceNumber+1));
                          label_StatusBar = (tr("Недопустимое значение: Тип счётчика") +
                                                       " Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkPulsesInputVolume(currentIndicatorNumber);

          //                ui->label_pulsesInputVolume->setVisible(true);
                          return;
                      }

//                      qDebug()<<"devType.toHex() "<<devType.toHex();
//                      paramsVector.append(devType);
                      mapRead["devType"] = devType;



                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка данных") +
    //                                               " Рабочее место: " + QString::number(workPlaceNumber+1));
                          label_StatusBar = (tr("Ошибка данных") +
                                                       " Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkPulsesInputVolume(currentIndicatorNumber);

           //               ui->label_pulsesInputVolume->setVisible(true);
                          return;
                       }
                   }

               }


            }



            QVector<QByteArray> writeVector;
            writeVector.append(mapwrite["flowMinByteArray"]);
            writeVector.append(mapwrite["flowMaxByteArray"]);
            writeVector.append(mapwrite["tinMinByteArray"]);
            writeVector.append(mapwrite["tinMaxByteArray"]);
            writeVector.append(mapwrite["toutMinByteArray"]);
            writeVector.append(mapwrite["toutMaxByteArray"]);
            writeVector.append(mapwrite["reportDataByteArray"]);
            writeVector.append(mapwrite["averageTByteArray"]);
            writeVector.append(mapwrite["pulsesMode1ByteArray"]);
            writeVector.append(mapwrite["pulsesMode2ByteArray"]);
            writeVector.append(mapwrite["pulsesValue1ByteArray"]);
            writeVector.append(mapwrite["pulsesValue2ByteArray"]);

            QVector<QByteArray> readVector;
            readVector.append(mapRead["flowMinByteArray"]);
            readVector.append(mapRead["flowMaxByteArray"]);
            readVector.append(mapRead["tinMinByteArray"]);
            readVector.append(mapRead["tinMaxByteArray"]);
            readVector.append(mapRead["toutMinByteArray"]);
            readVector.append(mapRead["toutMaxByteArray"]);
            readVector.append(mapRead["reportDataByteArray"]);
            readVector.append(mapRead["averageTByteArray"]);
            readVector.append(mapRead["pulsesMode1ByteArray"]);
            readVector.append(mapRead["pulsesMode2ByteArray"]);
            readVector.append(mapRead["pulsesValue1ByteArray"]);
            readVector.append(mapRead["pulsesValue2ByteArray"]);

            for(int i=0; i<allParamsNameList.size(); i++) {
                qDebug()<<"Write "<<allParamsNameList[i] + " "<<mapwrite[allParamsNameList[i]].toHex();
                qDebug()<<"Read "<<allParamsNameList[i] + " "<<mapRead[allParamsNameList[i]].toHex();
            }

            for(int i=0; i<writeVector.size(); i++) {


                if((i == (writeVector.size() - 2)) || (i == (writeVector.size() - 1))) {
                    int paramRead = (quint8)(readVector[i][0]);
                    paramRead = paramRead<<8 | (quint8)(readVector[i][1]);

                    int paramWrite = (quint8)(writeVector[i][0]);
                    paramWrite = paramWrite<<8 | (quint8)(writeVector[i][1]);

                    if((paramRead >= (paramWrite - 1)) && (paramRead <= (paramWrite + 1))) {

                    }
                    else {
                        label_StatusBar = ("Параметр записан некорректно: запись " + writeVector[i].toHex() +
                                                     " " + "чтение " + readVector[i].toHex() +
                                                     " Рабочее место: " + QString::number(workPlaceNumber+1));
                        emit errorStringSignal(label_StatusBar + '\n');
                        vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                        emit workPlaceOff(currentIndicatorNumber);
                        emit checkPulsesInputVolume(currentIndicatorNumber);

                        return;
                    }
                }
                else {
                        if((readVector[i] != writeVector[i]) && (i != (writeVector.size() - 2)) && (i != (writeVector.size() - 1)) ) {

                           label_StatusBar = ("Параметр записан некорректно: запись " + writeVector[i].toHex() +
                                                        " " + "чтение " + readVector[i].toHex() +
                                                           " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkPulsesInputVolume(currentIndicatorNumber);

                           return;
                        }
                }
            }




            emit checkPulsesInputVolume(currentIndicatorNumber);


}

void ObjectThread::slotPulsesOutputDefault(QSerialPort *port1, QSerialPort *port2,
                                           QSerialPort *port3, QSerialPort *port4)
{

    if(!isWorkPlaceUse.at(workPlace)) return;

    int currentIndicatorNumber = workPlace;
    int currentBoxNumber = 4;

    //сбросить счётчик импульсов стенда
    if(!pulsesReset()) {
        vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
        emit workPlaceOff(currentIndicatorNumber);
        emit checkPulsesOutputDefault(currentIndicatorNumber);

        return;
    }

    qDebug()<<"ObjectThread::slotPulsesOutputDefault() "<<"workPlace "<<QString::number(workPlace);

        QSerialPort *portTmp;

        bool isNeedPaintEvent = true;//false;

        int workPlaceNumber = workPlace;              

        QString label_StatusBar;
        QString errorString;

        switch (workPlaceNumber) {
        case 0:
            portTmp = port1;
            break;
        case 1:
            portTmp = port2;
            break;
        case 2:
            portTmp = port3;
            break;
        case 3:
            portTmp = port4;
            break;

        default:
            break;
        }

        portTmp->setBaudRate(QSerialPort::Baud19200);
        portTmp->setDataBits(QSerialPort::Data8);
        portTmp->setParity(QSerialPort::NoParity);
        portTmp->setStopBits(QSerialPort::OneStop);

   //     vectorIsErrorOccured[workPlaceNumber] = false;

   //         label_StatusBar =("");

            QColor color(255, 127, 50);
//            ui->label_pulsesOutputOff->setStyleSheet(QString("color: red").arg(color.name()));
//            ui->label_pulsesOutputOff->setText("X");
//            ui->label_pulsesOutputOff->setVisible(false);

            QByteArray pulsesMode1ByteArray;
            QByteArray pulsesMode2ByteArray;

            QByteArray pulsesValue1ByteArray;
            QByteArray pulsesValue2ByteArray;

            pulsesMode1ByteArray.clear();
            pulsesMode2ByteArray.clear();

            pulsesMode1ByteArray.append((quint8)paramsMapToThreads["pulsesMode1PrimalInt"].toInt()); //append(byte);
            pulsesMode2ByteArray.append((quint8)paramsMapToThreads["pulsesMode2PrimalInt"].toInt()); //append(byte);


            if((quint8)paramsMapToThreads["pulsesValue1PrimeInt"].toInt() == 0) {
    //            QMessageBox::information(this, "", tr("Недоустимое значение pulsesvalue1 ") + QString::number(pulsesValue1PrimeInt) +
    //                                     ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                label_StatusBar =(tr("Недоустимое значение pulsesvalue1 ") +
                                  QString::number((quint8)paramsMapToThreads["pulsesValue1PrimeInt"].toInt()) +
                                             ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                emit errorStringSignal(label_StatusBar + '\n');
                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                emit workPlaceOff(currentIndicatorNumber);
                emit checkPulsesOutputDefault(currentIndicatorNumber);

                return;
            }
            if((quint8)paramsMapToThreads["pulsesValue2PrimeInt"].toInt() == 0) {
    //            QMessageBox::information(this, "", tr("Недоустимое значение pulsesvalue2 ") + QString::number(pulsesValue2PrimeInt) +
    //                                     ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                label_StatusBar =(tr("Недоустимое значение pulsesvalue2 ") +
                                  QString::number((quint8)paramsMapToThreads["pulsesValue2PrimeInt"].toInt()) +
                                             ". Рабочее место: " + QString::number(workPlaceNumber + 1));
                emit errorStringSignal(label_StatusBar + '\n');
                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                emit workPlaceOff(currentIndicatorNumber);
                emit checkPulsesOutputDefault(currentIndicatorNumber);

                return;
            }
            pulsesValue1ByteArray.clear();
            pulsesValue2ByteArray.clear();
            pulsesValue1ByteArray.append((quint8)((quint8)paramsMapToThreads["pulsesValue1PrimeInt"].toInt()>>8));
            pulsesValue1ByteArray.append((quint8)((quint8)paramsMapToThreads["pulsesValue1PrimeInt"].toInt()));

            pulsesValue2ByteArray.append((quint8)((quint8)paramsMapToThreads["pulsesValue2PrimeInt"].toInt()>>8));
            pulsesValue2ByteArray.append((quint8)((quint8)paramsMapToThreads["pulsesValue2PrimeInt"].toInt()));

            QByteArray buffer;

            if(!portTmp->isOpen()) {
                if(!portTmp->open(QIODevice::ReadWrite)) {
    //                QMessageBox::information(this, "", "Не удалось открыть порт УСО-2. Рабочее место: "
    //                                         + QString::number(workPlaceNumber + 1));
                    label_StatusBar =("Не удалось открыть порт УСО-2. Рабочее место: "
                                                 + QString::number(workPlaceNumber + 1));
                    emit errorStringSignal(label_StatusBar + '\n');
                    vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                    emit workPlaceOff(currentIndicatorNumber);
                    emit checkPulsesOutputDefault(currentIndicatorNumber);

                    return;
                }
            }

            //одна команда Write Software Configuration с открытием парольной сессии

            //Открыть парольную сессию 5a 00 08 40 01 04 57
            for(int i=0; i<10;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x08;
               packetToRead.append(byte);
               byte = 0x40;
               packetToRead.append(byte);
               byte = 0x01;
               packetToRead.append(byte);
               byte = 0x04;
               packetToRead.append(byte);
               byte = 0x57;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") + ". Рабочее место: "
    //                                                 + QString::number(workPlaceNumber + 1));
                   label_StatusBar =(tr("Данные в порт не записаны") + ". Рабочее место: "
                                                + QString::number(workPlaceNumber + 1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkPulsesOutputDefault(currentIndicatorNumber);

                   return;
               }

               global::pause(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==9) {
    //                   QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") + ". Рабочее место: "
    //                                            + QString::number(workPlaceNumber + 1));
                       label_StatusBar =(tr("Ошибка открытия парольной сессии") + ". Рабочее место: "
                                                    + QString::number(workPlaceNumber + 1));
                       emit errorStringSignal(label_StatusBar + '\n');
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkPulsesOutputDefault(currentIndicatorNumber);

         //              ui->label_pulsesOutputOff->setVisible(true);
                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xc0;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 9) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkPulsesOutputDefault(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);


                      //
                      //



                      break;
                   }
                   else {
                            if(i==9) {
    //                            QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") + ". Рабочее место: "
    //                                                     + QString::number(workPlaceNumber + 1));
                                label_StatusBar =(tr("Ошибка открытия парольной сессии") + ". Рабочее место: "
                                                             + QString::number(workPlaceNumber + 1));
                                emit errorStringSignal(label_StatusBar + '\n');
                                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                                emit workPlaceOff(currentIndicatorNumber);
                                emit checkPulsesOutputDefault(currentIndicatorNumber);

         //                       ui->label_pulsesOutputOff->setVisible(true);
                                return;
                            }

                   }



               }


            }

            //
            //Write Software Configuration 5a 00 16 20
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x16;
               packetToRead.append(byte);
               byte = 0x20;
               packetToRead.append(byte);

               byte = paramsMapToThreads["flowMinByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowMinByteArray"].toByteArray()[1];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(flowMinByteArray);
               mapwrite["flowMinByteArray"] = paramsMapToThreads["flowMinByteArray"].toByteArray();


               byte = paramsMapToThreads["flowMaxByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["flowMaxByteArray"].toByteArray()[1];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(flowMaxByteArray);
               mapwrite["flowMaxByteArray"] = paramsMapToThreads["flowMaxByteArray"].toByteArray();


               byte = paramsMapToThreads["tinMinByteArray"].toByteArray()[0];
               packetToRead.append(byte);
  //             paramsVectorToWrite.append(tinMinByteArray);
               mapwrite["tinMinByteArray"] = paramsMapToThreads["tinMinByteArray"].toByteArray();


               byte = paramsMapToThreads["tinMaxByteArray"].toByteArray()[0];
               packetToRead.append(byte);
   //            paramsVectorToWrite.append(tinMaxByteArray);
               mapwrite["tinMaxByteArray"] = paramsMapToThreads["tinMaxByteArray"].toByteArray();


               byte = paramsMapToThreads["toutMinByteArray"].toByteArray()[0];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(toutMinByteArray);
               mapwrite["toutMinByteArray"] = paramsMapToThreads["toutMinByteArray"].toByteArray();


               byte = paramsMapToThreads["toutMaxByteArray"].toByteArray()[0];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(toutMaxByteArray);
               mapwrite["toutMaxByteArray"] = paramsMapToThreads["toutMaxByteArray"].toByteArray();


               byte = paramsMapToThreads["reportDataByteArray"].toByteArray()[0];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(reportDataByteArray);
               mapwrite["reportDataByteArray"] = paramsMapToThreads["reportDataByteArray"].toByteArray();


               byte = paramsMapToThreads["averageTByteArray"].toByteArray()[0];
               packetToRead.append(byte);
   //            paramsVectorToWrite.append(averageTByteArray);
               mapwrite["averageTByteArray"] = paramsMapToThreads["averageTByteArray"].toByteArray();


               byte = pulsesMode1ByteArray[0];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(pulsesMode1ByteArray);
               mapwrite["pulsesMode1ByteArray"] = pulsesMode1ByteArray;


               byte = pulsesMode2ByteArray[0];
               packetToRead.append(byte);
      //         paramsVectorToWrite.append(pulsesMode2ByteArray);
               mapwrite["pulsesMode2ByteArray"] = pulsesMode2ByteArray;


               byte = pulsesValue1ByteArray[0];
               packetToRead.append(byte);
               byte = pulsesValue1ByteArray[1];
               packetToRead.append(byte);
      //         paramsVectorToWrite.append(pulsesValue1ByteArray);
               mapwrite["pulsesValue1ByteArray"] = pulsesValue1ByteArray;


               byte = pulsesValue2ByteArray[0];
               packetToRead.append(byte);
               byte = pulsesValue2ByteArray[1];
               packetToRead.append(byte);
     //          paramsVectorToWrite.append(pulsesValue2ByteArray);
               mapwrite["pulsesValue2ByteArray"] = pulsesValue2ByteArray;

               byte = paramsMapToThreads["devType"].toByteArray()[0];
               packetToRead.append(byte);
      //         paramsVectorToWrite.append(devType);
               mapwrite["devType"] = paramsMapToThreads["devType"].toByteArray();

               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") + ". Рабочее место: "
    //                                        + QString::number(workPlaceNumber + 1));
                   label_StatusBar =(tr("Данные в порт не записаны") + ". Рабочее место: "
                                                + QString::number(workPlaceNumber + 1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkPulsesOutputDefault(currentIndicatorNumber);

          //         ui->label_pulsesOutputOff->setVisible(true);
                   return;
               }

               global::pause(250);//(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось записать данные : Software Configuration") + ". Рабочее место: "
    //                                            + QString::number(workPlaceNumber + 1));
                       label_StatusBar =(tr("Не удалось записать данные : Software Configuration") + ". Рабочее место: "
                                                    + QString::number(workPlaceNumber + 1));
                       emit errorStringSignal(label_StatusBar + '\n');
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkPulsesOutputDefault(currentIndicatorNumber);

          //             ui->label_pulsesOutputOff->setVisible(true);
                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xa0;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkPulsesOutputDefault(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer[4];

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                      //


                      //
                      //



                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка записи данных : Software Configuration") + ". Рабочее место: "
    //                                               + QString::number(workPlaceNumber + 1));
                          label_StatusBar =(tr("Ошибка записи данных : Software Configuration") + ". Рабочее место: "
                                                       + QString::number(workPlaceNumber + 1));
                          emit errorStringSignal(label_StatusBar + '\n');
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkPulsesOutputDefault(currentIndicatorNumber);

        //                  ui->label_pulsesOutputOff->setVisible(true);
                          return;
                       }
                   }



               }


            }


            //
            //Read Software Configuration 5a000502
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x05;
               packetToRead.append(byte);
               byte = 0x02;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") + ". Рабочее место: "
    //                                        + QString::number(workPlaceNumber + 1));
                   label_StatusBar =(tr("Данные в порт не записаны") + ". Рабочее место: "
                                                + QString::number(workPlaceNumber + 1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkPulsesOutputDefault(currentIndicatorNumber);

       //            ui->label_pulsesOutputOff->setVisible(true);
                   return;
               }

               global::pause(150);//(200);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось прочитать данные") + ". Рабочее место: "
    //                                            + QString::number(workPlaceNumber + 1));
                       label_StatusBar =(tr("Не удалось прочитать данные") + ". Рабочее место: "
                                                    + QString::number(workPlaceNumber + 1));
                       emit errorStringSignal(label_StatusBar + '\n');
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkPulsesOutputDefault(currentIndicatorNumber);

         //              ui->label_pulsesOutputOff->setVisible(true);
                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x16;
                   bytesForChecking.append(byte);
                   byte = 0x82;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkPulsesOutputDefault(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

       //            quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3]) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);
        //              ui->textBrowser->append("buffer.toHex()     " + buffer.toHex());

         //             emit sendbufferReadSoftWareConfiguration(buffer);

                      //
                      //flowMin
                      QByteArray flowMinArray;
                      flowMinArray.append(buffer[4]);
                      flowMinArray.append(buffer[5]);
         //             ui->lineEdit_Flow_min->setText(flowMinArray.toHex());

         //             float flowMinFloat = buffer[4];
                      quint16 flowMinQuint16 = buffer[4];
                      flowMinQuint16 = flowMinQuint16<<8 | buffer[5];
                      float flowMinFloat = (float)flowMinQuint16/1000;
//                      if(flowMinFloat>=0 && flowMinFloat<=0.2) {
//                          ui->lineEdit_Flow_min->setText(QString::number(flowMinFloat));
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Минимальный расход"));
//                          ui->lineEdit_Flow_min->setText(QString::number(flowMinFloat));
//                      }

//                      qDebug()<<"flowMinArray.toHex() "<<flowMinArray.toHex();
//                      paramsVector.append(flowMinArray);
                      mapRead["flowMinByteArray"] = flowMinArray;

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
//                      if(flowMaxFloat>=2 && flowMaxFloat<=5) {
//                          ui->lineEdit_Flow_max->setText(QString::number(flowMaxFloat));
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Максимальный расход"));
//                          ui->lineEdit_Flow_max->setText(QString::number(flowMaxFloat));
//                      }

//                      qDebug()<<"flowMaxArray.toHex() "<<flowMaxArray.toHex();
//                      paramsVector.append(flowMaxArray);
                      mapRead["flowMaxByteArray"] = flowMaxArray;

                      //
                      //tinMin
                      QByteArray tinMinArray;
                      tinMinArray.append(buffer[8]);


                      quint8 tinMinQuint8 = buffer[8];
//                      if(tinMinQuint8>=10 && tinMinQuint8<=60) {
//                          ui->spinBox_Tin_min->setValue(tinMinQuint8);
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Минимальная температура подачи"));
//                      }

//                      qDebug()<<"tinMinArray.toHex() "<<tinMinArray.toHex();
//                      paramsVector.append(tinMinArray);
                      mapRead["tinMinByteArray"] = tinMinArray;

                      //
                      //tinMax
                      QByteArray tinMaxArray;
                      tinMaxArray.append(buffer[9]);


                      quint8 tinMaxQuint8 = buffer[9];
//                      if(tinMaxQuint8>=50 && tinMaxQuint8<=150) {
//                          ui->spinBox_Tin_max->setValue(tinMaxQuint8);
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Максимальная температура подачи"));
//                      }

//                      qDebug()<<"tinMaxArray.toHex() "<<tinMaxArray.toHex();
//                      paramsVector.append(tinMaxArray);
                      mapRead["tinMaxByteArray"] = tinMaxArray;

                      //
                      //toutMin
                      QByteArray toutMinArray;
                      toutMinArray.append(buffer[10]);

                      quint8 toutMinQuint8 = buffer[10];
//                      if(toutMinQuint8>=10 && toutMinQuint8<=60) {
//                          ui->spinBox_Tout_min->setValue(toutMinQuint8);
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Минимальная температура обратки"));
//                      }

//                      qDebug()<<"toutMinArray.toHex() "<<toutMinArray.toHex();
//                      paramsVector.append(toutMinArray);
                      mapRead["toutMinByteArray"] = toutMinArray;


                      //
                      //toutMax
                      QByteArray toutMaxArray;
                      toutMaxArray.append(buffer[11]);

                      quint8 toutMaxQuint8 = buffer[11];
//                      if(toutMaxQuint8>=50 && toutMaxQuint8<=150) {
//                          ui->spinBox_Tout_max->setValue(toutMaxQuint8);
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Максимальная температура обратки"));
//                      }

//                      qDebug()<<"toutMaxArray.toHex() "<<toutMaxArray.toHex();
//                      paramsVector.append(toutMaxArray);
                      mapRead["toutMaxByteArray"] = toutMaxArray;

                      //
                      //reportData
                      QByteArray reportDataArray;
                      reportDataArray.append(buffer[12]);

                      quint8 reportDataInt = buffer[12];
//                      if(reportDataInt>=0 && reportDataInt<=31) {
//                          ui->spinBox_ReportData->setValue(reportDataInt);
//                      }
//                      else {
//          //                QMessageBox::information(this, "", tr("Недопустимое значение: Дата сохранения в журнал"));
//                      }

//                      qDebug()<<"reportDataArray.toHex() "<<reportDataArray.toHex();
//                      paramsVector.append(reportDataArray);
                      mapRead["reportDataByteArray"] = reportDataArray;

                      //
                      //AverageT
                      QByteArray AverageTArray;
                      AverageTArray.append(buffer[13]);


                      quint8 AverageTInt = buffer[13];
//                      if(AverageTList.contains(QString::number(AverageTInt))) {
//                          ui->comboBox_Average_T->setCurrentText(QString::number(AverageTInt));
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Тип усреднения температуры"));
//                      }

//                      qDebug()<<"AverageTArray.toHex() "<<AverageTArray.toHex();
//                      paramsVector.append(AverageTArray);
                      mapRead["averageTByteArray"] = AverageTArray;

                      //
                      //pulsesMode1
                      QByteArray pulsesMode1Array;
                      pulsesMode1Array.append(buffer[14]);


                      quint8 pulsesMode1Int = (quint8)buffer[14];
//                      if(pulsesMode1List.contains(QString::number(pulsesMode1Int))) {
//                          ui->comboBox_PulsesMode1->setCurrentText(QString::number(pulsesMode1Int));
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Режим работы импульсного вывода"));
//                      }

//                      qDebug()<<"pulsesMode1Array.toHex() "<<pulsesMode1Array.toHex();
//                      paramsVector.append(pulsesMode1Array);
                      mapRead["pulsesMode1ByteArray"] = pulsesMode1Array;

                      //
                      //pulsesMode2
                      QByteArray pulsesMode2Array;
                      pulsesMode2Array.append(buffer[15]);


                      quint8 pulsesMode2Int = (quint8)buffer[15];
//                      if(pulsesMode1List.contains(QString::number(pulsesMode2Int))) {
//                          ui->comboBox_PulsesMode2->setCurrentText(QString::number(pulsesMode2Int));
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Режим работы импульсного вывода"));
//                      }

//                      qDebug()<<"pulsesMode2Array.toHex() "<<pulsesMode2Array.toHex();
//                      paramsVector.append(pulsesMode2Array);
                      mapRead["pulsesMode2ByteArray"] = pulsesMode2Array;

                      //
                      //pulsesValue1
                      QByteArray pulsesValue1Array;
                      pulsesValue1Array.append(buffer[16]);
                      pulsesValue1Array.append(buffer[17]);


                      quint16 pulsesValue1Int = buffer[16];
                      pulsesValue1Int = pulsesValue1Int<<8 | (quint8)buffer[17];
        //              if((pulsesValue1Int<=101) && (pulsesValue1Int>=99)) {

        //              }
//                      ui->lineEdit_PulsesValue1->setText(QString::number(pulsesValue1Int));

//                      qDebug()<<"pulsesValue1Array.toHex() "<<pulsesValue1Array.toHex();
//                      paramsVector.append(pulsesValue1Array);
                      mapRead["pulsesValue1ByteArray"] = pulsesValue1Array;

                      //
                      //pulsesValue2
                      QByteArray pulsesValue2Array;
                      pulsesValue2Array.append(buffer[18]);
                      pulsesValue2Array.append(buffer[19]);
                      qDebug()<<"pulsesValue2Array.toHex() "<<pulsesValue2Array.toHex();

                      quint16 pulsesValue2Int = buffer[18];
                      pulsesValue2Int = pulsesValue2Int<<8 | (quint8)buffer[19];
//                      ui->lineEdit_PulsesValue2->setText(QString::number(pulsesValue2Int));

//                      qDebug()<<"pulsesValue2Array.toHex() "<<pulsesValue2Array.toHex();
//                      paramsVector.append(pulsesValue2Array);
                      mapRead["pulsesValue2ByteArray"] = pulsesValue2Array;

                      //
                      //DevType
                      QByteArray devType;
                      devType.append(buffer[20]);

//                      if(devType[0] == 0x04) {
//                          ui->comboBox_DevType->setCurrentText("Обратный");
//                      }

//                      if(devType[0] == 0x0c) {
//                          ui->comboBox_DevType->setCurrentText("Подающий");
//                      }

                      if((devType[0] != 0x04) && (devType[0] != 0x0c) ) {
    //                      QMessageBox::information(this, "", tr("Недопустимое значение: Тип счётчика") + ". Рабочее место: "
    //                                               + QString::number(workPlaceNumber + 1));
                          label_StatusBar =(tr("Недопустимое значение: Тип счётчика") + ". Рабочее место: "
                                                       + QString::number(workPlaceNumber + 1));
                          emit errorStringSignal(label_StatusBar + '\n');
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkPulsesOutputDefault(currentIndicatorNumber);

                          return;
                      }

//                      qDebug()<<"devType.toHex() "<<devType.toHex();
//                      paramsVector.append(devType);
                      mapRead["devType"] = devType;



                      break;
                   }
                   else {
                       if(i==2) {
    //                      QMessageBox::information(this, "", tr("Ошибка данных") + ". Рабочее место: "
    //                                               + QString::number(workPlaceNumber + 1));
                          label_StatusBar =(tr("Ошибка данных") + ". Рабочее место: "
                                                       + QString::number(workPlaceNumber + 1));
                          emit errorStringSignal(label_StatusBar + '\n');
             //             ui->label_pulsesOutputOff->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkPulsesOutputDefault(currentIndicatorNumber);

                          return;
                       }
                   }

               }


            }



            QVector<QByteArray> writeVector;
            writeVector.append(mapwrite["flowMinByteArray"]);
            writeVector.append(mapwrite["flowMaxByteArray"]);
            writeVector.append(mapwrite["tinMinByteArray"]);
            writeVector.append(mapwrite["tinMaxByteArray"]);
            writeVector.append(mapwrite["toutMinByteArray"]);
            writeVector.append(mapwrite["toutMaxByteArray"]);
            writeVector.append(mapwrite["reportDataByteArray"]);
            writeVector.append(mapwrite["averageTByteArray"]);
            writeVector.append(mapwrite["pulsesMode1ByteArray"]);
            writeVector.append(mapwrite["pulsesMode2ByteArray"]);
            writeVector.append(mapwrite["pulsesValue1ByteArray"]);
            writeVector.append(mapwrite["pulsesValue2ByteArray"]);

            QVector<QByteArray> readVector;
            readVector.append(mapRead["flowMinByteArray"]);
            readVector.append(mapRead["flowMaxByteArray"]);
            readVector.append(mapRead["tinMinByteArray"]);
            readVector.append(mapRead["tinMaxByteArray"]);
            readVector.append(mapRead["toutMinByteArray"]);
            readVector.append(mapRead["toutMaxByteArray"]);
            readVector.append(mapRead["reportDataByteArray"]);
            readVector.append(mapRead["averageTByteArray"]);
            readVector.append(mapRead["pulsesMode1ByteArray"]);
            readVector.append(mapRead["pulsesMode2ByteArray"]);
            readVector.append(mapRead["pulsesValue1ByteArray"]);
            readVector.append(mapRead["pulsesValue2ByteArray"]);

            for(int i=0; i<allParamsNameList.size(); i++) {
                qDebug()<<"Write "<<allParamsNameList[i] + " "<<mapwrite[allParamsNameList[i]].toHex();
                qDebug()<<"Read "<<allParamsNameList[i] + " "<<mapRead[allParamsNameList[i]].toHex();
            }

            for(int i=0; i<writeVector.size(); i++) {


                if((i == (writeVector.size() - 2)) || (i == (writeVector.size() - 1))) {
                    int paramRead = (quint8)(readVector[i][0]);
                    paramRead = paramRead<<8 | (quint8)(readVector[i][1]);

                    int paramWrite = (quint8)(writeVector[i][0]);
                    paramWrite = paramWrite<<8 | (quint8)(writeVector[i][1]);

                    if((paramRead >= (paramWrite - 1)) && (paramRead <= (paramWrite + 1))) {

                    }
                    else {
    //                    QMessageBox::information(this, "", "Параметр записан некорректно: запись " + writeVector[i].toHex() +
    //                                             " " + "чтение " + readVector[i].toHex() + ". Рабочее место: "
    //                                             + QString::number(workPlaceNumber + 1));
                        label_StatusBar =("Параметр записан некорректно: запись " + writeVector[i].toHex() +
                                                     " " + "чтение " + readVector[i].toHex() + ". Рабочее место: "
                                                     + QString::number(workPlaceNumber + 1));
                        emit errorStringSignal(label_StatusBar + '\n');
          //              ui->label_pulsesOutputOff->setVisible(true);
                        vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                        emit workPlaceOff(currentIndicatorNumber);
                        emit checkPulsesOutputDefault(currentIndicatorNumber);

                        return;
                    }
                }
                else {
                        if((readVector[i] != writeVector[i]) && (i != (writeVector.size() - 2)) && (i != (writeVector.size() - 1)) ) {
    //                       QMessageBox::information(this, "", "Параметр записан некорректно: запись " + writeVector[i].toHex() +
    //                                             " " + "чтение " + readVector[i].toHex() + ". Рабочее место: "
    //                                                + QString::number(workPlaceNumber + 1));
                           label_StatusBar =("Параметр записан некорректно: запись " + writeVector[i].toHex() +
                                                        " " + "чтение " + readVector[i].toHex() + ". Рабочее место: "
                                                           + QString::number(workPlaceNumber + 1));
                           emit errorStringSignal(label_StatusBar + '\n');
              //             ui->label_pulsesOutputOff->setVisible(true);
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkPulsesOutputDefault(currentIndicatorNumber);

                           return;
                        }
                }
            }


            //через 3 сек прочитываем кол-во импульсов из стенда
              global::pause(4000);

              readPulsesChannel1();
              readPulsesChannel2();

              emit textBrowser("Импульсы. канал 1 - " + QString::number(pulsesValueCh1) +
                                ",канал 2 - " + QString::number(pulsesValueCh2));


               if( (pulsesValueCh1 < 2) || (pulsesValueCh2 < 2) || (pulsesValueCh1!=pulsesValueCh2) ) {

                  //импульсы не работают

                  label_StatusBar = ("Импульсные выходы - по умолчанию не работают. Рабочее место: " + QString::number(workPlaceNumber+1));
                  emit errorStringSignal(label_StatusBar + '\n');
                  vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                  emit workPlaceOff(currentIndicatorNumber);
                  emit checkPulsesOutputDefault(currentIndicatorNumber);

                  return;

              }


            emit checkPulsesOutputDefault(currentIndicatorNumber);


}

void ObjectThread::slotMBusOn(QSerialPort *port1, QSerialPort *port2,
                              QSerialPort *port3, QSerialPort *port4)
{

    qDebug()<<"ObjectThread::slotMBusOn() "<<"workPlace "<<QString::number(workPlace);

        QSerialPort *portTmp;

        bool isNeedPaintEvent = true;//false;

        int workPlaceNumber = workPlace;

        if(!isWorkPlaceUse.at(workPlace)) return;

        int currentIndicatorNumber = workPlaceNumber;

        QString label_StatusBar;
        QString errorString;

        switch (workPlaceNumber) {
        case 0:
            portTmp = port1;
            break;
        case 1:
            portTmp = port2;
            break;
        case 2:
            portTmp = port3;
            break;
        case 3:
            portTmp = port4;
            break;

        default:
            break;
        }


        int currentBoxNumber = 5;
  //      vectorIsErrorOccured[workPlaceNumber] = false;


//            QColor color(255, 127, 50);
//            ui->label_MBusOn->setStyleSheet(QString("color: red").arg(color.name()));
//            ui->label_MBusOn->setText("X");
//            ui->label_MBusOn->setVisible(false);

//            label_StatusBar =("");

        //    ProtTypeByteArray.clear();
        //    quint8 byte = 0;
        //    ProtTypeByteArray.append(byte);

            QByteArray buffer;

            if(!portTmp->isOpen()) {
                if(!portTmp->open(QIODevice::ReadWrite)) {
    //                QMessageBox::information(this, "", "Не удалось открыть порт УСО-2. Рабочее место: " +
    //                                         QString::number(workPlaceNumber + 1));
                    label_StatusBar =("Не удалось открыть порт УСО-2. Рабочее место: " +
                                                 QString::number(workPlaceNumber + 1));
                    emit errorStringSignal(label_StatusBar + '\n');
                    vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();


                    emit checkMBusOn(currentIndicatorNumber);

                    return;
                }
            }

            //Открыть парольную сессию, Write External interface Settings

            //
            //Открыть парольную сессию 5a 00 08 40 01 04 57
            for(int i=0; i<10;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x08;
               packetToRead.append(byte);
               byte = 0x40;
               packetToRead.append(byte);
               byte = 0x01;
               packetToRead.append(byte);
               byte = 0x04;
               packetToRead.append(byte);
               byte = 0x57;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") + ". Рабочее место: " +
    //                                        QString::number(workPlaceNumber + 1));
                   label_StatusBar =(tr("Данные в порт не записаны") + ". Рабочее место: " +
                                                QString::number(workPlaceNumber + 1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                   emit checkMBusOn(currentIndicatorNumber);

                   return;
               }

               global::pause(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==9) {
    //                   QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") + ". Рабочее место: " +
    //                                            QString::number(workPlaceNumber + 1));
                       label_StatusBar =(tr("Ошибка открытия парольной сессии") + ". Рабочее место: " +
                                                    QString::number(workPlaceNumber + 1));
                       emit errorStringSignal(label_StatusBar + '\n');
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                       emit checkMBusOn(currentIndicatorNumber);
                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xc0;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 9) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkMBusOn(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);


                      //
                      //



                      break;
                   }
                   else {
                            if(i==9) {
    //                            QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") + ". Рабочее место: " +
    //                                                     QString::number(workPlaceNumber + 1));
                                label_StatusBar =(tr("Ошибка открытия парольной сессии") + ". Рабочее место: " +
                                                             QString::number(workPlaceNumber + 1));
                                emit errorStringSignal(label_StatusBar + '\n');
                                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                                emit checkMBusOn(currentIndicatorNumber);
                                return;
                            }

                   }



               }


            }

            //
            //
            //Write External Interface Settings 5a 00 0e 2b
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x0e;
               packetToRead.append(byte);
               byte = 0x2b;
               packetToRead.append(byte);

               byte = paramsMapToThreads["addrByteArray"].toByteArray()[0];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(addrByteArray);
               mapwrite["addrByteArray"] = paramsMapToThreads["addrByteArray"].toByteArray();


               byte = paramsMapToThreads["baudRateByteArray"].toByteArray()[0];
               packetToRead.append(byte);
      //         paramsVectorToWrite.append(baudRateByteArray);
               mapwrite["baudRateByteArray"] = paramsMapToThreads["baudRateByteArray"].toByteArray();


               byte = paramsMapToThreads["HMP_TransByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["HMP_TransByteArray"].toByteArray()[1];
               packetToRead.append(byte);
   //            paramsVectorToWrite.append(HMP_TransByteArray);
               mapwrite["HMP_TransByteArray"] = paramsMapToThreads["HMP_TransByteArray"].toByteArray();


               byte = paramsMapToThreads["EPP_TransByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["EPP_TransByteArray"].toByteArray()[1];
               packetToRead.append(byte);
   //            paramsVectorToWrite.append(EPP_TransByteArray);
               mapwrite["EPP_TransByteArray"] = paramsMapToThreads["EPP_TransByteArray"].toByteArray();


               byte = paramsMapToThreads["P1PrimAddrByteArray"].toByteArray()[0];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(P1PrimAddrByteArray);
               mapwrite["P1PrimAddrByteArray"] = paramsMapToThreads["P1PrimAddrByteArray"].toByteArray();


               byte = paramsMapToThreads["P2PrimAddrByteArray"].toByteArray()[0];
               packetToRead.append(byte);
   //            paramsVectorToWrite.append(P2PrimAddrByteArray);
               mapwrite["P2PrimAddrByteArray"] = paramsMapToThreads["P2PrimAddrByteArray"].toByteArray();

                   //

                   QByteArray ProtTypeByteArray;

                   ProtTypeByteArray.clear();
                   byte = 0;
                   ProtTypeByteArray.append(byte);
                   //
               byte = ProtTypeByteArray[0];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(ProtTypeByteArray);
               mapwrite["ProtTypeByteArray"] = ProtTypeByteArray;

//               ProtTypeByteArray.clear();
//               byte = 0xff;
//               ProtTypeByteArray.append(byte);


               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") + ". Рабочее место: " +
    //                                                 QString::number(workPlaceNumber + 1));
                   label_StatusBar =(tr("Данные в порт не записаны") + ". Рабочее место: " +
                                                QString::number(workPlaceNumber + 1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                   emit checkMBusOn(currentIndicatorNumber);
                   return;
               }

               global::pause(250);//(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
         //              QMessageBox::information(this, "", tr("Не удалось записать данные : External Interface Settings"));
         //              ui->label_MBusOn->setVisible(true);
         //              return;
                   } else{}

               }
               else {
                   //если ответ не пустой, тогда считаем ошибка
                   label_StatusBar =(tr("Ошибка ответа") + ". Рабочее место: " +
                                                QString::number(workPlaceNumber + 1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                   emit checkMBusOn(currentIndicatorNumber);
                   return;

                   //проверяем crc и первые четыре байта ответного пакета
//                   QByteArray bytesForChecking;
//                   quint8 byte = 0x5a;
//                   bytesForChecking.append(byte);
//                   byte = 0x00;
//                   bytesForChecking.append(byte);
//                   byte = 0x06;
//                   bytesForChecking.append(byte);
//                   byte = 0xab;
//                   bytesForChecking.append(byte);

//                   quint8 error = buffer[4];

//                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] &&
//                                               bytesForChecking[2] == buffer[2] && bytesForChecking[3] == buffer[3] && error == 0) {

//                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
//                           <<"checkCRC(buffer) "<<checkCRC(buffer);

//                      //


//                      //
//                      //



//                      break;
//                   }
//                   else {
//                       if(i==2) {
//    //                      QMessageBox::information(this, "", tr("Ошибка записи данных : External Interface Settings") + ". Рабочее место: " +
//    //                                               QString::number(workPlaceNumber + 1));

//                          label_StatusBar =(tr("Ошибка записи данных : External Interface Settings") + ". Рабочее место: " +
//                                                       QString::number(workPlaceNumber + 1));
//                          emit errorStringSignal(label_StatusBar + '\n');
//                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
//                          emit checkMBusOn(currentIndicatorNumber);
//                          return;
//                       }
//                   }



               }


            }


            //
            //Read External Interface Settings 5a 00 05 1b
//            for(int i=0; i<3;i++) {

//               portTmp->clear();
//               packetToRead.clear();
//               buffer.clear();
//               quint8 byte = 0x5a;
//               packetToRead.append(byte);
//               byte = 0x00;
//               packetToRead.append(byte);
//               byte = 0x05;
//               packetToRead.append(byte);
//               byte = 0x1b;
//               packetToRead.append(byte);
//               quint8 crc = makeCRC(packetToRead);
//               packetToRead.append(crc);

//               quint64 cnt = portTmp->write(packetToRead);
//               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

//               qDebug()<<"cnt "<<cnt
//                       <<"packetToRead.toHex() "<<packetToRead.toHex()
//                       <<"portTmp->portName() "<<portTmp->portName();

//               if(cnt == 0) {
//    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") + ". Рабочее место: " +
//    //                                        QString::number(workPlaceNumber + 1));
//                   label_StatusBar =(tr("Данные в порт не записаны") + ". Рабочее место: " +
//                                                QString::number(workPlaceNumber + 1));
//                   emit errorStringSignal(label_StatusBar + '\n');
//                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
//                   emit checkMBusOn(currentIndicatorNumber);
//                   return;
//               }

//               global::pause(150);//(200);

//               buffer = portTmp->readAll();
//               qDebug()<<"buffer.toHex()"<<buffer.toHex();
//               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

//               if(buffer.isEmpty()) {
//                   if(i==2) {
//    //                   QMessageBox::information(this, "", tr("Не удалось прочитать данные") + ". Рабочее место: " +
//    //                                            QString::number(workPlaceNumber + 1));
//                       label_StatusBar =(tr("Не удалось прочитать данные") + ". Рабочее место: " +
//                                                    QString::number(workPlaceNumber + 1));
//                       emit errorStringSignal(label_StatusBar + '\n');
//                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
//                       emit checkMBusOn(currentIndicatorNumber);
//                       return;
//                   } else{}

//               }
//               else {
//                   //проверяем crc и первые четыре байта ответного пакета
//                   QByteArray bytesForChecking;
//                   quint8 byte = 0x5a;
//                   bytesForChecking.append(byte);
//                   byte = 0x00;
//                   bytesForChecking.append(byte);
//                   byte = 0x0e;
//                   bytesForChecking.append(byte);
//                   byte = 0x9b;
//                   bytesForChecking.append(byte);

//                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] &&
//                                               bytesForChecking[2] == buffer[2] && bytesForChecking[3] == buffer[3]) {

//                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
//                           <<"checkCRC(buffer) "<<checkCRC(buffer);
//         //             ui->textBrowser->append("buffer.toHex()     " + buffer.toHex());

//                      emit sendbufferReadExternalInterfaceSettings(buffer);

//                      //
//                      //Addr
//                      QByteArray AddrArray;
//                      AddrArray.append(buffer[4]);
//                      qDebug()<<"AddrArray.toHex() "<<AddrArray.toHex();

//                      quint8 AddrQuint8 = (quint8)buffer[4];

//                      if(AddrQuint8>=0 && AddrQuint8<=252) {
//                          ui->spinBox_Addr->setValue(AddrQuint8);
//                      }
//                      else {
//         //                 QMessageBox::information(this, "", tr("Недопустимое значение: Первичный адрес счётчика "));
//                          ui->spinBox_Addr->setValue(AddrQuint8);
//                      }

//                      paramsVector.append(AddrArray);
//                      mapRead["addrByteArray"] = AddrArray;

//                      //
//                      //BaudRate
//                      QByteArray BaudRateArray;
//                      BaudRateArray.append(buffer[5]);
//                      qDebug()<<"BaudRateArray.toHex() "<<BaudRateArray.toHex();

//                      quint8 BaudRateQuint8 = (quint8)buffer[5];

//                      for(int j=0; j< ui->comboBox_BaudRate->count(); j++) {
//                          ui->comboBox_BaudRate->setCurrentIndex(j);
//                          if(BaudRateQuint8 == (quint8)ui->comboBox_BaudRate->currentData().toInt()) {
//                              break;
//                          }

//                          if(j==3 && (BaudRateQuint8 != (quint8)ui->comboBox_BaudRate->currentData().toInt())) {
//                               //                 QMessageBox::information(this, "", tr("Недопустимое значение: Скорость обмена ") + QString::number(BaudRateQuint8, 16));
//                          }
//                      }

//        //              if(BaudRateList.contains(QString::number(BaudRateQuint8, 16))) {
//        //                  ui->comboBox_BaudRate->setCurrentText(QString::number(BaudRateQuint8, 16));
//        //              }
//        //              else {
//        //   //                 QMessageBox::information(this, "", tr("Недопустимое значение: Скорость обмена ") + QString::number(BaudRateQuint8, 16));
//        //              }

//                      paramsVector.append(BaudRateArray);
//                      mapRead["baudRateByteArray"] = BaudRateArray;

//                      //
//                      //HMP_Trans 2bytes  6,7
//                      QByteArray HMP_TransArray;
//                      HMP_TransArray.append(buffer[6]);
//                      HMP_TransArray.append(buffer[7]);
//                      qDebug()<<"HMP_TransArray.toHex() "<<HMP_TransArray.toHex();

//                      quint16 HMP_TransInt = (quint8)buffer[6];
//                      HMP_TransInt = HMP_TransInt<<8 | (quint8)buffer[7];

//                      if(HMP_TransInt == 0x3fff) {
//                          ui->lineEdit_HMP_Trans->setText(QString::number(HMP_TransInt, 16));
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Позиционный код передаваемых параметров HMP_Trans ") +
//        //                                           QString::number(HMP_TransInt, 16));
//                      }

//                      paramsVector.append(HMP_TransArray);
//                      mapRead["HMP_TransByteArray"] = HMP_TransArray;

//                      //
//                      //EPP_Trans 2bytes  8,9
//                      QByteArray EPP_TransArray;
//                      EPP_TransArray.append(buffer[8]);
//                      EPP_TransArray.append(buffer[9]);
//                      qDebug()<<"EPP_TransArray.toHex() "<<EPP_TransArray.toHex();

//                      quint16 EPP_TransInt = (quint8)buffer[8];
//                      EPP_TransInt = EPP_TransInt<<8 | (quint8)buffer[9];

//                      if(EPP_TransInt == 0x000f) {
//                          ui->lineEdit_EPP_Trans->setText(QString::number(EPP_TransInt, 16));
//                      }
//                      else {
//          //                QMessageBox::information(this, "", tr("Недопустимое значение: Позиционный код передаваемых параметров EPP_Trans ") +
//          //                                         QString::number(EPP_TransInt, 16));
//                      }

//                      paramsVector.append(EPP_TransArray);
//                      mapRead["EPP_TransByteArray"] = EPP_TransArray;

//                      //
//                      //P1_PrimAddr 1byte  10
//                      QByteArray P1_PrimAddrArray;
//                      P1_PrimAddrArray.append(buffer[10]);
//                      qDebug()<<"P1_PrimAddrArray.toHex() "<<P1_PrimAddrArray.toHex();

//                      quint8 P1_PrimAddrInt = (quint8)buffer[10];

//                      if(P1_PrimAddrInt>=0 && P1_PrimAddrInt<=252) {
//                          ui->spinBox_P1PrimAddr->setValue(P1_PrimAddrInt);
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Первичный адрес импульсных входов P1_PrimAddr ") +
//        //                                           QString::number(P1_PrimAddrInt, 10));
//                      }

//                      paramsVector.append(P1_PrimAddrArray);
//                      mapRead["P1PrimAddrByteArray"] = P1_PrimAddrArray;

//                      //
//                      //P2_PrimAddr 1byte  11
//                      QByteArray P2_PrimAddrArray;
//                      P2_PrimAddrArray.append(buffer[11]);
//                      qDebug()<<"P2_PrimAddrArray.toHex() "<<P2_PrimAddrArray.toHex();

//                      quint8 P2_PrimAddrInt = (quint8)buffer[11];

//                      if(P2_PrimAddrInt>=0 && P2_PrimAddrInt<=252) {
//                          ui->spinBox_P2PrimAddr->setValue(P2_PrimAddrInt);
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Первичный адрес импульсных входов P2_PrimAddr ") +
//        //                                           QString::number(P2_PrimAddrInt, 10));
//                      }

//                      paramsVector.append(P2_PrimAddrArray);
//                      mapRead["P2PrimAddrByteArray"] = P2_PrimAddrArray;

//                      //
//                      //ProtType 1byte  12
//                      QByteArray ProtTypeArray;
//                      ProtTypeArray.append(buffer[12]);
//                      qDebug()<<"ProtTypeArray.toHex() "<<ProtTypeArray.toHex();

//                      quint8 ProtTypeInt = (quint8)buffer[12];

//                      if(ProtTypeInt == 0xff) {
//                          ui->lineEdit_ProtType->setText(QString::number(ProtTypeInt, 16));
//                      }
//                      else {
//        //                  QMessageBox::information(this, "", tr("Недопустимое значение: Первичный адрес импульсных входов P2_PrimAddr ") +
//        //                                           QString::number(ProtTypeInt, 16));
//                      }

//                      paramsVector.append(ProtTypeArray);
//                      mapRead["ProtTypeByteArray"] = ProtTypeArray;




//                      break;
//                   }
//                   else {
//                       if(i==2) {
//    //                      QMessageBox::information(this, "", tr("Ошибка данных") + ". Рабочее место: " +
//    //                                               QString::number(workPlaceNumber + 1));
//                          label_StatusBar =(tr("Ошибка данных") + ". Рабочее место: " +
//                                                       QString::number(workPlaceNumber + 1));
//                          emit errorStringSignal(label_StatusBar + '\n');
//                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
//                          emit checkMBusOn(currentIndicatorNumber);
//                          return;
//                       }
//                   }



//               }


//            }




    //        QVector<QByteArray> writeVector;
    //        writeVector.append(mapwrite["addrByteArray"]);
    //        writeVector.append(mapwrite["baudRateByteArray"]);
    //        writeVector.append(mapwrite["HMP_TransByteArray"]);
    //        writeVector.append(mapwrite["EPP_TransByteArray"]);
    //        writeVector.append(mapwrite["P1PrimAddrByteArray"]);
    //        writeVector.append(mapwrite["P2PrimAddrByteArray"]);
    //        writeVector.append(mapwrite["ProtTypeByteArray"]);

    //        QVector<QByteArray> readVector;
    //        readVector.append(mapRead["addrByteArray"]);
    //        readVector.append(mapRead["baudRateByteArray"]);
    //        readVector.append(mapRead["HMP_TransByteArray"]);
    //        readVector.append(mapRead["EPP_TransByteArray"]);
    //        readVector.append(mapRead["P1PrimAddrByteArray"]);
    //        readVector.append(mapRead["P2PrimAddrByteArray"]);
    //        readVector.append(mapRead["ProtTypeByteArray"]);


    //        for(int i=0; i<allParamsNameList.size(); i++) {
    //            qDebug()<<"Write "<<allParamsNameList[i] + " "<<mapwrite[allParamsNameList[i]].toHex();
    //            qDebug()<<"Read "<<allParamsNameList[i] + " "<<mapRead[allParamsNameList[i]].toHex();
    //        }

    //        for(int i=0; i<writeVector.size(); i++) {

    //                    if((readVector[i] != writeVector[i]) && (i != (writeVector.size() - 2)) && (i != (writeVector.size() - 1)) ) {
    //                       QMessageBox::information(this, "", "Параметр записан некорректно: запись " + writeVector[i].toHex() +
    //                                             " " + "чтение " + readVector[i].toHex() + ". Рабочее место: " +
    //                                                QString::number(workPlaceNumber + 1));
    //                       label_StatusBar =();
    //                       emit errorStringSignal(label_StatusBar + '\n');
    //                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
    //                       emit checkMBusOn(currentIndicatorNumber);
    //                       return;
    //                    }

    //        }


//            ui->label_MBusOn->setStyleSheet(QString("color: green").arg(color.name()));
//            ui->label_MBusOn->setText("V");
//            ui->label_MBusOn->setVisible(true);


            emit checkMBusOn(currentIndicatorNumber);


}

void ObjectThread::slotMBusCheck(QSerialPort *port1, QSerialPort *port2,
                                 QSerialPort *port3, QSerialPort *port4)
{

    qDebug()<<"ObjectThread::slotMBusCheck() "<<"workPlace "<<QString::number(workPlace);

        QSerialPort *portTmp;

        bool isNeedPaintEvent = true;//false;

        int workPlaceNumber = workPlace;

        if(!isWorkPlaceUse.at(workPlace)) return;

        int currentIndicatorNumber = workPlaceNumber;

        QString label_StatusBar;
        QString errorString;

        switch (workPlaceNumber) {
        case 0:
            portTmp = port1;
            break;
        case 1:
            portTmp = port2;
            break;
        case 2:
            portTmp = port3;
            break;
        case 3:
            portTmp = port4;
            break;

        default:
            break;
        }


        int currentBoxNumber = 6;
  //      vectorIsErrorOccured[workPlaceNumber] = false;

//            QColor color(255, 127, 50);
//            ui->label_CheckMbus->setStyleSheet(QString("color: red").arg(color.name()));
//            ui->label_CheckMbus->setText("X");
//            ui->label_CheckMbus->setVisible(false);

//            label_StatusBar =("");

            QByteArray buffer;

            if(!portTmp->isOpen()) {
                if(!portTmp->open(QIODevice::ReadWrite)) {
    //                QMessageBox::information(this, "", "Не удалось открыть порт" +
    //                                         ". Рабочее место: " + QString::number(workPlaceNumber+1));
                    label_StatusBar =("Не удалось открыть порт. Рабочее место: " + QString::number(workPlaceNumber+1));
                    emit errorStringSignal(label_StatusBar + '\n');
                    vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                    emit workPlaceOff(currentIndicatorNumber);
                    emit checkMBusCheck(currentIndicatorNumber);

                    return;
                }
            }



            portTmp->setBaudRate(QSerialPort::Baud2400);
            portTmp->setDataBits(QSerialPort::Data8);
            portTmp->setParity(QSerialPort::EvenParity);
            portTmp->setStopBits(QSerialPort::OneStop);


            //
            //Проверка протокола M-Bus //Проверка проводных интерфейсов 10 5B FE 59 16
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x10;
               packetToRead.append(byte);
               byte = 0x5b;
               packetToRead.append(byte);
               byte = 0xfe;
               packetToRead.append(byte);
               byte = 0x59;
               packetToRead.append(byte);
               byte = 0x16;
               packetToRead.append(byte);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portDigitalInterfaceChecking->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
    //                                        ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar =(tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkMBusCheck(currentIndicatorNumber);

                   return;
               }

               global::pause(2000);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
                       portTmp->setBaudRate(QSerialPort::Baud19200);
                       portTmp->setDataBits(QSerialPort::Data8);
                       portTmp->setParity(QSerialPort::NoParity);
                       portTmp->setStopBits(QSerialPort::OneStop);

    //                   QMessageBox::information(this, "", tr("Ошибка проверки M-Bus") +
    //                                            ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar =(tr("Ошибка проверки M-Bus") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkMBusCheck(currentIndicatorNumber);

    //                   ui->label_CheckMbus->setVisible(true);
                       return;
                   } else{}

               }
               else {

                   int len =  (quint8)buffer[1];

                   quint64 sumBytes = 0;
                   for(int i=4; i<(buffer.size() - 2); i++) {
                       sumBytes = sumBytes + (quint8)buffer[i];
                   }

                   quint8 crcMod256 = (quint8)sumBytes;

                   qDebug()<<"crcMod256 = "<<QString::number(crcMod256);

        //           Answer
        //           68	Begin Sync
        //           52	Len
        //           52	Len
        //           68	Begin Sync
        //           08	C-field
        //           Adr	Primary address
        //           72	CI-field
        //           xx	Serial
        //           xx
        //           xx
        //           xx
        //           B4	Man ID - SET
        //           4C
        //           01	Ver
        //           04 / 0C	Type
        //           xx	Num
        //           xx	Stat
        //           00	Sign
        //           00
        //           04	DIF
        //           FB	VIF
        //           0D
        //           xx	Heat energy, 1 Mcal
        //           xx
        //           xx
        //           xx
        //           04	DIF
        //           13	VIF
        //           xx	Volume, 1 l
        //           xx
        //           xx
        //           xx
        //           04	DIF
        //           1B	VIF
        //           xx	Mass, 1 kg
        //           xx
        //           xx
        //           xx
        //           02	DIF
        //           59	VIF
        //           xx	Inlet temperature,      0,01 °CInlet temperature,      0,01 °C
        //           xx
        //           02	DIF
        //           5D	VIF
        //           xx	Oulet temperature,      0,01 °COulet temperature,      0,01 °C
        //           xx
        //           02	DIF
        //           61	VIF
        //           xx	temperature differential, 10 mK
        //           xx
        //           04	DIF
        //           FD	VIF
        //           17
        //           xx	Tin_Er && Tout_Er
        //           xx	dT_Er && Flow_Er
        //           xx	Magnet state
        //           xx	CRC
        //           16	End Sync


                   if(crcMod256 = buffer[buffer.size()-2]) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                      portTmp->setBaudRate(QSerialPort::Baud19200);
                      portTmp->setDataBits(QSerialPort::Data8);
                      portTmp->setParity(QSerialPort::NoParity);
                      portTmp->setStopBits(QSerialPort::OneStop);

       //               label_StatusBar =("Проверка протокола M-Bus : Успешно");
//                      ui->label_CheckMbus->setStyleSheet(QString("color: green").arg(color.name()));
//                      ui->label_CheckMbus->setText("V");
        //              ui->label_CheckMbus->setVisible(true);




                      break;
                   }
                   else {
                       if (i==2) {
                          portTmp->setBaudRate(QSerialPort::Baud19200);
                          portTmp->setDataBits(QSerialPort::Data8);
                          portTmp->setParity(QSerialPort::NoParity);
                          portTmp->setStopBits(QSerialPort::OneStop);
    //                      QMessageBox::information(this, "", tr("Ошибка проверки M-Bus") +
    //                                               ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          label_StatusBar =(tr("Ошибка проверки M-Bus") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkMBusCheck(currentIndicatorNumber);

          //                ui->label_CheckMbus->setVisible(true);
    //                      label_StatusBar =("Проверка протокола M-Bus : Неудачно. Рабочее место: " + QString::number(workPlaceNumber+1));
    //                      emit errorStringSignal(label_StatusBar + '\n');
         //                 ui->label_CheckMbus->setVisible(true);
                          return;
                       }
                   }



               }


            }


            emit checkMBusCheck(currentIndicatorNumber);


}

void ObjectThread::slotCalibrationModeOff(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4)
{

    qDebug()<<"ObjectThread::slotCalibrationModeOff() "<<"workPlace "<<QString::number(workPlace);

        QSerialPort *portTmp;

        bool isNeedPaintEvent = true;//false;

        int workPlaceNumber = workPlace;

        if(!isWorkPlaceUse.at(workPlace)) return;

        int currentIndicatorNumber = workPlaceNumber;

        QString label_StatusBar;
        QString errorString;

        switch (workPlaceNumber) {
        case 0:
            portTmp = port1;
            break;
        case 1:
            portTmp = port2;
            break;
        case 2:
            portTmp = port3;
            break;
        case 3:
            portTmp = port4;
            break;

        default:
            break;
        }


        int currentBoxNumber = 9;
  //      vectorIsErrorOccured[workPlaceNumber] = false;



//            QColor color(255, 127, 50);
//            ui->label_calibrationModeOff->setStyleSheet(QString("color: red").arg(color.name()));
//            ui->label_calibrationModeOff->setText("X");
//            ui->label_calibrationModeOff->setVisible(false);

//            label_StatusBar = ("");

            QByteArray buffer;

            if(!portTmp->isOpen()) {
                if(!portTmp->open(QIODevice::ReadWrite)) {
      //              QMessageBox::information(this, "", "Не удалось открыть порт");
                    label_StatusBar = ("Не удалось открыть порт. Рабочее место: " + QString::number(workPlaceNumber+1));
                    emit errorStringSignal(label_StatusBar + '\n');
                    vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                    emit workPlaceOff(currentIndicatorNumber);
                    emit checkCalibrationModeOff(currentIndicatorNumber);

                    return;
                }
            }


            //--------------------- Выключение режима калибровки по СЭТ --------------------
            // 5A	00	06	4C	Mode=1	CRC
            // 5A	00	06	CC	Error	CRC



            portTmp->setBaudRate(QSerialPort::Baud19200);
            portTmp->setDataBits(QSerialPort::Data8);
            portTmp->setParity(QSerialPort::NoParity);
            portTmp->setStopBits(QSerialPort::OneStop);


            //
            //Disable Calibr Mode 5a 00 06 4c 01 crc
            for(int i=0; i<10;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x06;
               packetToRead.append(byte);
               byte = 0x4c;
               packetToRead.append(byte);
               byte = 0x01;
               packetToRead.append(byte);
    //           byte = 0x17;
    //           packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
    //                                                 ". Рабочее место: " + QString::number(workPlaceNumber+1));
    //               label_StatusBar = (tr("Данные в порт не записаны") +
    //                                            ". Рабочее место: " + QString::number(workPlaceNumber+1));
    //               emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
     //              emit checkCalibrationModeOff(currentIndicatorNumber);
     //              return;
               }

               global::pause(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();

               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==9) {
    //                   QMessageBox::information(this, "", tr("Ошибка установки пароля по умолчанию") +
    //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
    //                   label_StatusBar = (tr("Ошибка выключения режима калибровки") +
    //                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
    //                   emit errorStringSignal(label_StatusBar + '\n');
    //                   ui->label_writeParams->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
     //                  emit checkCalibrationModeOff(currentIndicatorNumber);
     //                  return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета  5A  00  06	CC	Error	CRC
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xcc;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 9) {
//                           label_StatusBar = (tr("Ошибка данных") +
//                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
//                           emit errorStringSignal(label_StatusBar + '\n');
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          if(buffer.size()<5)
                              buffer.resize(5);

//                           emit workPlaceOff(currentIndicatorNumber);
//                           emit checkWritingError(currentIndicatorNumber);
//                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);



                      //
                      //



                      break;
                   }
                   else {
                            if(i==9) {
    //                            QMessageBox::information(this, "", tr("Ошибка установки пароля по умолчанию") +
    //                                                     "Рабочее место: " + QString::number(workPlaceNumber+1));
    //                            label_StatusBar = (tr("Ошибка установки пароля по умолчанию") +
    //                                                         ". Рабочее место: " + QString::number(workPlaceNumber+1));
    //                            emit errorStringSignal(label_StatusBar + '\n');
    //                            ui->label_writeParams->setVisible(true);
                                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
     //                           emit checkCalibrationModeOff(currentIndicatorNumber);
     //                           return;
                            }

                   }



               }


            }


            if(vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] == false) {
                //ошибка не произошла

                emit checkCalibrationModeOff(currentIndicatorNumber);
                return;
            }

            //--------------------- Выключение режима калибровки по СЭТ --------------------/



            vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = false;

            //    Выключение режима "Калибровка" по M-Bus	CalibrEnable	SND-UD	Request	68	06	06	68	53 / 73	Adr		51	00	FD 15	(By4 + ... + By9) mod 256		16
            //        CalibrAcc			                                      Begin Sync Length	Length	Begin Sync	C-field	Primary address		CI-field	DIF	 VIF	           CRC		            End Sync
            //            Answer		E5


            portTmp->setBaudRate(QSerialPort::Baud2400);
            portTmp->setDataBits(QSerialPort::Data8);
            portTmp->setParity(QSerialPort::EvenParity);
            portTmp->setStopBits(QSerialPort::OneStop);


            for(int i=0; i<1/*3*/;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();

               quint8 byte = 0x68;
               packetToRead.append(byte);
               byte = 0x06;
               packetToRead.append(byte);
               byte = 0x06;
               packetToRead.append(byte);
               byte = 0x68;
               packetToRead.append(byte);
               byte = 0x53;
               packetToRead.append(byte);
               byte = 0xfe;
               packetToRead.append(byte);
               byte = 0x51;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0xfd;
               packetToRead.append(byte);
               byte = 0x15;
               packetToRead.append(byte);

               quint32 sum = 0;
               for(int j=4; j<packetToRead.size(); j++) {
                   sum = sum + (quint8)packetToRead[j];
               }

               quint8 crcMod256 = (quint8)sum;
               packetToRead.append(crcMod256);
               byte = 0x16;
               packetToRead.append(byte);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны"));
                   label_StatusBar = ("Данные в порт не записаны. Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkCalibrationModeOff(currentIndicatorNumber);

                   return;
               }

               global::pause(2000);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==0/*2*/) {
                       portTmp->setBaudRate(QSerialPort::Baud19200);
                       portTmp->setDataBits(QSerialPort::Data8);
                       portTmp->setParity(QSerialPort::NoParity);
                       portTmp->setStopBits(QSerialPort::OneStop);

     //                  QMessageBox::information(this, "", tr("Ошибка выключения режима калибровки. Рабочее место: "));
                       label_StatusBar = (tr("Ошибка выключения режима калибровки. Рабочее место: ")
                                                    + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkCalibrationModeOff(currentIndicatorNumber);

     //                  ui->label_calibrationModeOff->setVisible(true);
                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета


                   if((quint8)buffer[0] == 0xe5) {

                       portTmp->setBaudRate(QSerialPort::Baud19200);
                       portTmp->setDataBits(QSerialPort::Data8);
                       portTmp->setParity(QSerialPort::NoParity);
                       portTmp->setStopBits(QSerialPort::OneStop);

          //             label_StatusBar = ("Выключение режима калибровки : Успешно");
//                       ui->label_calibrationModeOff->setStyleSheet(QString("color: green").arg(color.name()));
//                       ui->label_calibrationModeOff->setText("V");
          //             ui->label_calibrationModeOff->setVisible(true);

                       qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                      //


                      //
                      //



                      break;
                   }
                   else {
                       if(i==0/*2*/) {
                           portTmp->setBaudRate(QSerialPort::Baud19200);
                           portTmp->setDataBits(QSerialPort::Data8);
                           portTmp->setParity(QSerialPort::NoParity);
                           portTmp->setStopBits(QSerialPort::OneStop);
     //                      QMessageBox::information(this, "", tr("Ошибка выключения режима калибровки"));
                           label_StatusBar = (tr("Ошибка выключения режима калибровки. Рабочее место: ")
                                                        + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkCalibrationModeOff(currentIndicatorNumber);

    //                       ui->label_calibrationModeOff->setVisible(true);
    //                       label_StatusBar = ("Выключение режима калибровки : Неудачно");
    //                       ui->label_calibrationModeOff->setVisible(true);
                           return;
                       }
                   }



               }


            }


            emit checkCalibrationModeOff(currentIndicatorNumber);


}

void ObjectThread::slotWireInterfaceChecking(QSerialPort *port1, QSerialPort *port2,
                                             QSerialPort *port3, QSerialPort *port4, QSerialPort *portDigitalInterface)
{

    qDebug()<<"ObjectThread::slotWireInterfaceChecking() "<<"workPlace "<<QString::number(workPlace);

    //подключить интерфейс
    //тип интерфейса определяется исполнением теплосчётчика, указанным в файле исполнений

    if(selectMbus()) return;
    if(selectRS()) return;
    if(externalInterfaceOn()) return;

        QSerialPort *portTmp;
        QSerialPort *portDigitalInterfaceChecking = portDigitalInterface;

        bool isNeedPaintEvent = true;//false;

        int workPlaceNumber = workPlace;

        if(!isWorkPlaceUse.at(workPlace)) return;

        int currentIndicatorNumber = workPlaceNumber;

        QString label_StatusBar;
        QString errorString;

        switch (workPlaceNumber) {
        case 0:
            portTmp = port1;
            break;
        case 1:
            portTmp = port2;
            break;
        case 2:
            portTmp = port3;
            break;
        case 3:
            portTmp = port4;
            break;

        default:
            break;
        }

        portTmp->setBaudRate(QSerialPort::Baud19200);
        portTmp->setDataBits(QSerialPort::Data8);
        portTmp->setParity(QSerialPort::NoParity);
        portTmp->setStopBits(QSerialPort::OneStop);


        int currentBoxNumber = 8;
  //      vectorIsErrorOccured[workPlaceNumber] = false;

//            label_StatusBar = ("");

//            QColor color(255, 127, 50);
//            ui->label_digitalWireInterfaceChecking->setStyleSheet(QString("color: red").arg(color.name()));
//            ui->label_digitalWireInterfaceChecking->setText("X");
//            ui->label_digitalWireInterfaceChecking->setVisible(false);

            QByteArray buffer;

            if(!portTmp->isOpen()) {
                if(!portTmp->open(QIODevice::ReadWrite)) {
      //              QMessageBox::information(this, "", tr("Не удалось открыть порт: ИК") +
       //                                      ". Рабочее место: " + QString::number(workPlaceNumber+1));
                    label_StatusBar = (tr("Не удалось открыть порт: ИК") +
                                                 ". Рабочее место: " + QString::number(workPlaceNumber+1));
                    emit errorStringSignal(label_StatusBar + '\n');
                    vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                    emit workPlaceOff(currentIndicatorNumber);
                    emit checkWireInterfaceChecking(currentIndicatorNumber);
                    return;
                }
            }


            //Открыть парольную сессию 5a 00 08 40 01 04 57
            for(int i=0; i<10;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x08;
               packetToRead.append(byte);
               byte = 0x40;
               packetToRead.append(byte);
               byte = 0x01;
               packetToRead.append(byte);
               byte = 0x04;
               packetToRead.append(byte);
               byte = 0x57;
               packetToRead.append(byte);
               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
    //                                                 " Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWireInterfaceChecking(currentIndicatorNumber);
                   return;
               }

               global::pause(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==9) {
    //                   QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") +
    //                                                         " Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Ошибка открытия парольной сессии") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWireInterfaceChecking(currentIndicatorNumber);
  //                     ui->label_pulsesOutputHeat->setVisible(true);
                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xc0;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 9) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWireInterfaceChecking(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer.at(4);

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);


                      //
                      //



                      break;
                   }
                   else {
                            if(i==9) {
    //                            QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") +
    //                                                     " Рабочее место: " + QString::number(workPlaceNumber+1));
                                label_StatusBar = (tr("Ошибка открытия парольной сессии") +
                                                             ". Рабочее место: " + QString::number(workPlaceNumber+1));
                                emit errorStringSignal(label_StatusBar + '\n');
                                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                                emit workPlaceOff(currentIndicatorNumber);
                                emit checkWireInterfaceChecking(currentIndicatorNumber);
     //                           ui->label_pulsesOutputHeat->setVisible(true);
                                return;
                            }

                   }



               }


            }

            //Открыть парольную сессию 5a 00 08 40 01 04 57 /


            //
            //----------------- Установка адресов от 1 до 4 -----------------

            //
            //
            //Write External Interface Settings 5a 00 0e 2b
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x5a;
               packetToRead.append(byte);
               byte = 0x00;
               packetToRead.append(byte);
               byte = 0x0e;
               packetToRead.append(byte);
               byte = 0x2b;
               packetToRead.append(byte);

               byte = (quint8)(workPlaceNumber + 1); //addrByteArray[0];
               packetToRead.append(byte);
               QByteArray addrByteArray;
               addrByteArray.append(byte);
  //             paramsVectorToWrite.append(addrByteArray);
               mapwrite["addrByteArray"] = addrByteArray;


               byte = paramsMapToThreads["baudRateByteArray"].toByteArray()[0];
               packetToRead.append(byte);
  //             paramsVectorToWrite.append(baudRateByteArray);
               mapwrite["baudRateByteArray"] = paramsMapToThreads["baudRateByteArray"].toByteArray();


               byte = paramsMapToThreads["HMP_TransByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["HMP_TransByteArray"].toByteArray()[1];
               packetToRead.append(byte);
   //            paramsVectorToWrite.append(HMP_TransByteArray);
               mapwrite["HMP_TransByteArray"] = paramsMapToThreads["HMP_TransByteArray"].toByteArray();


               byte = paramsMapToThreads["EPP_TransByteArray"].toByteArray()[0];
               packetToRead.append(byte);
               byte = paramsMapToThreads["EPP_TransByteArray"].toByteArray()[1];
               packetToRead.append(byte);
   //            paramsVectorToWrite.append(EPP_TransByteArray);
               mapwrite["EPP_TransByteArray"] = paramsMapToThreads["EPP_TransByteArray"].toByteArray();


               byte = paramsMapToThreads["P1PrimAddrByteArray"].toByteArray()[0];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(P1PrimAddrByteArray);
               mapwrite["P1PrimAddrByteArray"] = paramsMapToThreads["P1PrimAddrByteArray"].toByteArray();


               byte = paramsMapToThreads["P2PrimAddrByteArray"].toByteArray()[0];
               packetToRead.append(byte);
    //           paramsVectorToWrite.append(P2PrimAddrByteArray);
               mapwrite["P2PrimAddrByteArray"] = paramsMapToThreads["P2PrimAddrByteArray"].toByteArray();


               byte = paramsMapToThreads["ProtTypeByteArray"].toByteArray()[0];
               packetToRead.append(byte);
   //            paramsVectorToWrite.append(ProtTypeByteArray);
               mapwrite["ProtTypeByteArray"] = paramsMapToThreads["ProtTypeByteArray"].toByteArray();


               quint8 crc = makeCRC(packetToRead);
               packetToRead.append(crc);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
        //               QMessageBox::information(this, "", tr("Данные в порт не записаны ") +
        //                                                 "Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Данные в порт не записаны ") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWireInterfaceChecking(currentIndicatorNumber);

                   return;
               }

               global::pause(250);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
        //                   QMessageBox::information(this, "", tr("Не удалось записать данные : External Interface Settings ") +
        //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
                       label_StatusBar = (tr("Не удалось установить адрес : External Interface Settings ") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
  //                     ui->label_writeParams->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWireInterfaceChecking(currentIndicatorNumber);

                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета
                   QByteArray bytesForChecking;
                   quint8 byte = 0x5a;
                   bytesForChecking.append(byte);
                   byte = 0x00;
                   bytesForChecking.append(byte);
                   byte = 0x06;
                   bytesForChecking.append(byte);
                   byte = 0xab;
                   bytesForChecking.append(byte);

                   if(!packetSETProcessing(buffer)) {
                       if(i == 2) {
                           label_StatusBar = (tr("Ошибка данных") +
                                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWireInterfaceChecking(currentIndicatorNumber);
                           return;
                       }
                       else {
                           continue;
                       }
                   }

                   quint8 error = buffer[4];

                   if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                      //


                      //
                      //



                      break;
                   }
                   else {
                       if(i==2) {
        //                       QMessageBox::information(this, "", tr("Ошибка записи данных : External Interface Settings ") +
        //                                                "Рабочее место: " + QString::number(workPlaceNumber+1));
                          label_StatusBar = (tr("Не удалось установить адрес : External Interface Settings ") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
      //                    ui->label_writeParams->setVisible(true);
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                          emit workPlaceOff(currentIndicatorNumber);
                          emit checkWireInterfaceChecking(currentIndicatorNumber);

                          return;
                       }
                   }



               }


            }


            //
            //----------------- Установка адресов от 1 до 4 -----------------/


            //
            //
            //Проверка проводных интерфейсов 10 5B FE 59 16

            for(int r=0; r<100; r++) {

                if(!portDigitalInterfaceChecking->isOpen()) break;

                global::pause(100);

            }

            portDigitalInterfaceChecking->close();

            if(!portDigitalInterfaceChecking->isOpen()) {
                if(!portDigitalInterfaceChecking->open(QIODevice::ReadWrite)) {
     //               QMessageBox::information(this, "", "Не удалось открыть порт: Проводные интерфейсы");
                    label_StatusBar = tr("Не удалось открыть порт: Проводные интерфейсы");
 //                                                 ". Рабочее место: " + QString::number(workPlaceNumber+1));
                    emit errorStringSignal(label_StatusBar + '\n');
                    vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                    emit checkWireInterfaceChecking(currentIndicatorNumber);
                    portDigitalInterfaceChecking->close();
                    return;
                }
            }

            for(int i=0; i<3;i++) {

               portDigitalInterfaceChecking->clear();
               packetToRead.clear();
               buffer.clear();
               quint8 byte = 0x10;
               packetToRead.append(byte);
               byte = 0x5b;
               packetToRead.append(byte);
               byte = (quint8)(workPlaceNumber + 1);//0xfe;
               packetToRead.append(byte);
               byte = (quint8)(0x5b + workPlaceNumber + 1);//0x59;
               packetToRead.append(byte);
               byte = 0x16;
               packetToRead.append(byte);

               quint64 cnt = portDigitalInterfaceChecking->write(packetToRead);
               emit textBrowser(">> " + portDigitalInterfaceChecking->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portDigitalInterfaceChecking->portName() "<<portDigitalInterfaceChecking->portName();

               if(cnt == 0) {
      //             QMessageBox::information(this, "", tr("Данные в порт не записаны"));
                   label_StatusBar = (tr("Данные в порт не записаны. Ошибка проверки проводных интерфейсов") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
    //               ui->label_writeParams->setVisible(true);
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWireInterfaceChecking(currentIndicatorNumber);
                   portDigitalInterfaceChecking->close();

                   setPrimeAddressDefault(workPlaceNumber, port1, port2, port3, port4);
                   return;
               }

               global::pause(100);

               buffer = portDigitalInterfaceChecking->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portDigitalInterfaceChecking->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
       //                QMessageBox::information(this, "", tr("Ошибка проверки проводных интерфейсов"));
                       label_StatusBar = (tr("Ошибка проверки проводных интерфейсов") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
       //                ui->label_writeParams->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWireInterfaceChecking(currentIndicatorNumber);
                       portDigitalInterfaceChecking->close();

                       setPrimeAddressDefault(workPlaceNumber, port1, port2, port3, port4);
        //               ui->label_digitalWireInterfaceChecking->setVisible(true);

        //               usableResult = "Не годен";
        //               on_toolButton_result_clicked();

                       return;
                   } else{}

               }
               else {

                   int len =  (quint8)buffer[1];
                   quint8 crcBeyondEndOfLen = (quint8)buffer.at(4 + len);

                   quint64 sumBytes = 0;
                   for(int u=4; u<(4 + len)/*(buffer.size() - 2)*/; u++) {
                       sumBytes = sumBytes + (quint8)buffer[u];
                   }

                   quint8 crcMod256 = (quint8)sumBytes;

                   qDebug()<<"crcMod256 = "<<QString::number(crcMod256);

                   QByteArray firstFourBytesArray;
                   firstFourBytesArray.append((quint8)buffer.at(0));
                   firstFourBytesArray.append((quint8)buffer.at(1));
                   firstFourBytesArray.append((quint8)buffer.at(2));
                   firstFourBytesArray.append((quint8)buffer.at(3));

                   //
                   //проверяем первые 4 байта
                   if( (firstFourBytesArray.at(0) != 0x68) || (firstFourBytesArray.at(3) != 0x68) ||
                           (firstFourBytesArray.at(1) != firstFourBytesArray.at(2)) ||
                           (buffer.at(4) != 0x08) ||
                           (buffer.at(6) != 0x72) ||
                           (buffer.at(4 + len + 1) != 0x16) ) {

                       if(i==2) {
     //                      QMessageBox::information(this, "", tr("Ошибка проверки проводных интерфейсов"));
                           label_StatusBar = (tr("Ошибка проверки проводных интерфейсов") +
                                                        ". Рабочее место: " + QString::number(workPlaceNumber+1));
                           emit errorStringSignal(label_StatusBar + '\n');
      //                     ui->label_writeParams->setVisible(true);
                           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                           emit workPlaceOff(currentIndicatorNumber);
                           emit checkWireInterfaceChecking(currentIndicatorNumber);
                           portDigitalInterfaceChecking->close();

                           setPrimeAddressDefault(workPlaceNumber, port1, port2, port3, port4);
     //                      ui->label_digitalWireInterfaceChecking->setVisible(true);

     //                      usableResult = "Не годен";
            //               on_toolButton_result_clicked();

                           return;
                       } else{}

                   }
                   //
                   //проверяем первые 4 байта/




        //           Answer
        //           68	Begin Sync
        //           52	Len
        //           52	Len
        //           68	Begin Sync
        //           08	C-field
        //           Adr	Primary address
        //           72	CI-field
        //           xx	Serial
        //           xx
        //           xx
        //           xx
        //           B4	Man ID - SET
        //           4C
        //           01	Ver
        //           04 / 0C	Type
        //           xx	Num
        //           xx	Stat
        //           00	Sign
        //           00
        //           04	DIF
        //           FB	VIF
        //           0D
        //           xx	Heat energy, 1 Mcal
        //           xx
        //           xx
        //           xx
        //           04	DIF
        //           13	VIF
        //           xx	Volume, 1 l
        //           xx
        //           xx
        //           xx
        //           04	DIF
        //           1B	VIF
        //           xx	Mass, 1 kg
        //           xx
        //           xx
        //           xx
        //           02	DIF
        //           59	VIF
        //           xx	Inlet temperature,      0,01 °CInlet temperature,      0,01 °C
        //           xx
        //           02	DIF
        //           5D	VIF
        //           xx	Oulet temperature,      0,01 °COulet temperature,      0,01 °C
        //           xx
        //           02	DIF
        //           61	VIF
        //           xx	temperature differential, 10 mK
        //           xx
        //           04	DIF
        //           FD	VIF
        //           17
        //           xx	Tin_Er && Tout_Er
        //           xx	dT_Er && Flow_Er
        //           xx	Magnet state
        //           xx	CRC
        //           16	End Sync


                   if(crcMod256 = crcBeyondEndOfLen/*buffer[buffer.size()-2]*/) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

       //            label_StatusBar = ("Проверка проводных интерфейсов : Успешно");
//                   ui->label_digitalWireInterfaceChecking->setStyleSheet(QString("color: green").arg(color.name()));
//                   ui->label_digitalWireInterfaceChecking->setText("V");
       //            ui->label_digitalWireInterfaceChecking->setVisible(true);


        //              if(((ui->label_writeParams->text() == "V") && (ui->label_writeParams->isVisible() == true)) &&
        //                 ((ui->label_Calibration->text() == "V") && (ui->label_Calibration->isVisible() == true)) &&
        //                 ((ui->label_pulsesOutputHeat->text() == "V") && (ui->label_pulsesOutputHeat->isVisible() == true)) &&
        //                 ((ui->label_pulsesInputVolume->text() == "V") && (ui->label_pulsesInputVolume->isVisible() == true)) &&
        //                 ((ui->label_pulsesOutputOff->text() == "V") && (ui->label_pulsesOutputOff->isVisible() == true)) &&
        //                 ((ui->label_CheckMbus->text() == "V") && (ui->label_CheckMbus->isVisible() == true)) &&
        //                 ((ui->label_calibrationModeOff->text() == "V") && (ui->label_calibrationModeOff->isVisible() == true)) &&
        //                 ((ui->label_digitalWireInterfaceChecking->text() == "V") && (ui->label_digitalWireInterfaceChecking->isVisible() == true)) &&
        //                 ((ui->label_RMV_ErrorFlags_tick->text() == "V") && (ui->label_RMV_ErrorFlags_tick->isVisible() == true))     )   {


        //                  usableResult = "Годен";
        //  //                  on_toolButton_result_clicked();

        //              }
        //              else {
        //                  usableResult = "Не годен";
        //  //                  on_toolButton_result_clicked();
        //              }





                      break;
                   }
                   else {
                       if (i==2) {
       //                QMessageBox::information(this, "", tr("Ошибка проверки проводных интерфейсов"));
                       label_StatusBar = (tr("Ошибка проверки проводных интерфейсов") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
      //                 ui->label_writeParams->setVisible(true);
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWireInterfaceChecking(currentIndicatorNumber);
                       portDigitalInterfaceChecking->close();

                       setPrimeAddressDefault(workPlaceNumber, port1, port2, port3, port4);
      //                 label_StatusBar = ("Проверка проводных интерфейсов : Неудачно");
      //                 ui->label_digitalWireInterfaceChecking->setVisible(true);

        //               usableResult = "Не годен";
        //               on_toolButton_result_clicked();

                       return;
                       }
                   }



               }


            }


            //
            //
            //Проверка проводных интерфейсов 10 5B FE 59 16 /


            //установка исходного адреса

            setPrimeAddressDefault(workPlaceNumber, port1, port2, port3, port4);

            emit checkWireInterfaceChecking(currentIndicatorNumber);

            externalInterfaceOff();

            portDigitalInterfaceChecking->close();


}

void ObjectThread::setPrimeAddressDefault(int workPlaceNumber, QSerialPort *port1, QSerialPort *port2,
                                          QSerialPort *port3, QSerialPort *port4)
{

        QSerialPort *portTmp;

        bool isNeedPaintEvent = true;//false;

        workPlaceNumber = workPlace;

        if(!isWorkPlaceUse.at(workPlace)) return;

        int currentIndicatorNumber = workPlaceNumber;

        QString label_StatusBar;
        QString errorString;

        switch (workPlaceNumber) {
        case 0:
            portTmp = port1;
            break;
        case 1:
            portTmp = port2;
            break;
        case 2:
            portTmp = port3;
            break;
        case 3:
            portTmp = port4;
            break;

        default:
            break;
        }


        int currentBoxNumber = 8;
 //       vectorIsErrorOccured[workPlaceNumber] = false;

  //      ui->label_StatusBar->setText("");

        QByteArray buffer;

        //
        //----------------- Установка адресов все 1 -----------------

        //
        //
        //Write External Interface Settings 5a 00 0e 2b
        for(int i=0; i<3;i++) {

           portTmp->clear();
           packetToRead.clear();
           buffer.clear();
           quint8 byte = 0x5a;
           packetToRead.append(byte);
           byte = 0x00;
           packetToRead.append(byte);
           byte = 0x0e;
           packetToRead.append(byte);
           byte = 0x2b;
           packetToRead.append(byte);

           byte = paramsMapToThreads["addrByteArray"].toByteArray()[0];//0x01;//(quint8)(workPlaceNumber + 1); //addrByteArray[0];
           packetToRead.append(byte);
   //        paramsVectorToWrite.append(addrByteArray);
           mapwrite["addrByteArray"] = paramsMapToThreads["addrByteArray"].toByteArray();


           byte = paramsMapToThreads["baudRateByteArray"].toByteArray()[0];
           packetToRead.append(byte);
   //        paramsVectorToWrite.append(baudRateByteArray);
           mapwrite["baudRateByteArray"] = paramsMapToThreads["baudRateByteArray"].toByteArray();


           byte = paramsMapToThreads["HMP_TransByteArray"].toByteArray()[0];
           packetToRead.append(byte);
           byte = paramsMapToThreads["HMP_TransByteArray"].toByteArray()[1];
           packetToRead.append(byte);
  //         paramsVectorToWrite.append(HMP_TransByteArray);
           mapwrite["HMP_TransByteArray"] = paramsMapToThreads["HMP_TransByteArray"].toByteArray();


           byte = paramsMapToThreads["EPP_TransByteArray"].toByteArray()[0];
           packetToRead.append(byte);
           byte = paramsMapToThreads["EPP_TransByteArray"].toByteArray()[1];
           packetToRead.append(byte);
 //          paramsVectorToWrite.append(EPP_TransByteArray);
           mapwrite["EPP_TransByteArray"] = paramsMapToThreads["EPP_TransByteArray"].toByteArray();


           byte = paramsMapToThreads["P1PrimAddrByteArray"].toByteArray()[0];
           packetToRead.append(byte);
   //        paramsVectorToWrite.append(P1PrimAddrByteArray);
           mapwrite["P1PrimAddrByteArray"] = paramsMapToThreads["P1PrimAddrByteArray"].toByteArray();


           byte = paramsMapToThreads["P2PrimAddrByteArray"].toByteArray()[0];
           packetToRead.append(byte);
  //         paramsVectorToWrite.append(P2PrimAddrByteArray);
           mapwrite["P2PrimAddrByteArray"] = paramsMapToThreads["P2PrimAddrByteArray"].toByteArray();


           byte = paramsMapToThreads["ProtTypeByteArray"].toByteArray()[0];
           packetToRead.append(byte);
  //         paramsVectorToWrite.append(ProtTypeByteArray);
           mapwrite["ProtTypeByteArray"] = paramsMapToThreads["ProtTypeByteArray"].toByteArray();


           quint8 crc = makeCRC(packetToRead);
           packetToRead.append(crc);

           quint64 cnt = portTmp->write(packetToRead);
           emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

           qDebug()<<"cnt "<<cnt
                   <<"packetToRead.toHex() "<<packetToRead.toHex()
                   <<"portTmp->portName() "<<portTmp->portName();

           if(cnt == 0) {
    //               QMessageBox::information(this, "", tr("Данные в порт не записаны ") +
    //                                                 "Рабочее место: " + QString::number(workPlaceNumber+1));
               label_StatusBar = (tr("Данные в порт не записаны ") +
                                            ". Рабочее место: " + QString::number(workPlaceNumber+1));
               emit errorStringSignal(label_StatusBar + '\n');
               vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

               emit workPlaceOff(currentIndicatorNumber);
               emit checkWireInterfaceChecking(currentIndicatorNumber);

               return;
           }

           global::pause(250);

           buffer = portTmp->readAll();
           qDebug()<<"buffer.toHex()"<<buffer.toHex();
           if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

           if(buffer.isEmpty()) {
               if(i==2) {
    //                   QMessageBox::information(this, "", tr("Не удалось записать данные : External Interface Settings ") +
    //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
                   label_StatusBar = (tr("Не удалось установить исходный адрес : External Interface Settings ") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
   //                ui->label_writeParams->setVisible(true);
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkWireInterfaceChecking(currentIndicatorNumber);

                   return;
               } else{}

           }
           else {
               //проверяем crc и первые четыре байта ответного пакета
               QByteArray bytesForChecking;
               quint8 byte = 0x5a;
               bytesForChecking.append(byte);
               byte = 0x00;
               bytesForChecking.append(byte);
               byte = 0x06;
               bytesForChecking.append(byte);
               byte = 0xab;
               bytesForChecking.append(byte);

               if(!packetSETProcessing(buffer)) {
                   if(i == 2) {
                       label_StatusBar = (tr("Ошибка данных") +
                                                    " Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkWireInterfaceChecking(currentIndicatorNumber);
                       return;
                   }
                   else {
                       continue;
                   }
               }

               quint8 error = buffer[4];

               if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] &&
                                           bytesForChecking[2] == buffer[2] && bytesForChecking[3] == buffer[3] && error == 0) {

                  qDebug()<<"buffer.toHex() "<<buffer.toHex()
                       <<"checkCRC(buffer) "<<checkCRC(buffer);

                  //


                  //
                  //



                  break;
               }
               else {
                   if(i==2) {
    //                       QMessageBox::information(this, "", tr("Ошибка записи данных : External Interface Settings ") +
    //                                                "Рабочее место: " + QString::number(workPlaceNumber+1));
                      label_StatusBar = (tr("Не удалось установить исходный адрес : External Interface Settings ") +
                                                   ". Рабочее место: " + QString::number(workPlaceNumber+1));
                      emit errorStringSignal(label_StatusBar + '\n');
   //                   ui->label_writeParams->setVisible(true);
                      vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                      emit workPlaceOff(currentIndicatorNumber);
                      emit checkWireInterfaceChecking(currentIndicatorNumber);
                      return;
                   }
               }



           }


        }


        //
        //----------------- Установка адресов от 1 до 4 -----------------/


}

void ObjectThread::slotMBusOff(QSerialPort *port1, QSerialPort *port2,
                               QSerialPort *port3, QSerialPort *port4)
{

    qDebug()<<"ObjectThread::slotMBusOff() "<<"workPlace "<<QString::number(workPlace);

        QSerialPort *portTmp;

        bool isNeedPaintEvent = true;//false;

        int workPlaceNumber = workPlace;

        if(!isWorkPlaceUse.at(workPlace)) return;

        int currentIndicatorNumber = workPlaceNumber;

        QString label_StatusBar;
        QString errorString;

        switch (workPlaceNumber) {
        case 0:
            portTmp = port1;
            break;
        case 1:
            portTmp = port2;
            break;
        case 2:
            portTmp = port3;
            break;
        case 3:
            portTmp = port4;
            break;

        default:
            break;
        }


        int currentBoxNumber = 8;
   //     vectorIsErrorOccured[workPlaceNumber] = false;

   //         ui->label_StatusBar->setText("");

         QByteArray ProtTypeByteArray;

            ProtTypeByteArray.clear();
            quint8 byte = 0;
            ProtTypeByteArray.append(byte);

            QByteArray buffer;

            if(!portTmp->isOpen()) {
                if(!portTmp->open(QIODevice::ReadWrite)) {
      //              QMessageBox::information(this, "", "Не удалось открыть порт УСО-2");
                    label_StatusBar = (tr("Не удалось открыть порт УСО-2") +
                                                 ". Рабочее место: " + QString::number(workPlaceNumber+1));
                    emit errorStringSignal(label_StatusBar + '\n');
                    vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                    emit checkMbusOff(currentIndicatorNumber);
                    emit workPlaceOff(currentIndicatorNumber);
                    return;
                }
            }



            //выключение протокола M-Bus

            portTmp->setBaudRate(QSerialPort::Baud2400);
            portTmp->setDataBits(QSerialPort::Data8);
            portTmp->setParity(QSerialPort::EvenParity);
            portTmp->setStopBits(QSerialPort::OneStop);

            //
            //
            //команда открытия парольной сессии 68 09 09 68 53 FE 51 03 FD 13 aa=0x04 aa=0x57 01

            for(int i=0; i<10;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();

               quint8 byte = 0x68;
               packetToRead.append(byte);
               byte = 0x09;
               packetToRead.append(byte);
               byte = 0x09;
               packetToRead.append(byte);
               byte = 0x68;
               packetToRead.append(byte);
               byte = 0x53;
               packetToRead.append(byte);
               byte = 0xfe;
               packetToRead.append(byte);
               byte = 0x51;
               packetToRead.append(byte);
               byte = 0x03;
               packetToRead.append(byte);
               byte = 0xfd;
               packetToRead.append(byte);
               byte = 0x13;
               packetToRead.append(byte);
               byte = 0x57; //aa;
               packetToRead.append(byte);
               byte = 0x04; //aa;
               packetToRead.append(byte);
               byte = 0x01;
               packetToRead.append(byte);

               quint32 sum = 0;
               for(int j=4; j<packetToRead.size(); j++) {
                   sum = sum + (quint8)packetToRead[j];
               }

               quint8 crcMod256 = (quint8)sum;
               packetToRead.append(crcMod256);
               byte = 0x16;
               packetToRead.append(byte);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
     //              QMessageBox::information(this, "", tr("Данные в порт не записаны"));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                   emit checkMbusOff(currentIndicatorNumber);
                   emit workPlaceOff(currentIndicatorNumber);
                   return;
               }

               global::pause(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==9) {
                       portTmp->setBaudRate(QSerialPort::Baud19200);
                       portTmp->setDataBits(QSerialPort::Data8);
                       portTmp->setParity(QSerialPort::NoParity);
                       portTmp->setStopBits(QSerialPort::OneStop);
      //                 QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии по M-Bus"));
                       label_StatusBar = (tr("Ошибка открытия парольной сессии по M-Bus") +
                                                    ". Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                       emit checkMbusOff(currentIndicatorNumber);
                       emit workPlaceOff(currentIndicatorNumber);
                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета


                   if((quint8)buffer[0] == 0xe5) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                      portTmp->setBaudRate(QSerialPort::Baud19200);
                      portTmp->setDataBits(QSerialPort::Data8);
                      portTmp->setParity(QSerialPort::NoParity);
                      portTmp->setStopBits(QSerialPort::OneStop);

                      //


                      //
                      //



                      break;
                   }
                   else {
                       if(i==9) {
                          portTmp->setBaudRate(QSerialPort::Baud19200);
                          portTmp->setDataBits(QSerialPort::Data8);
                          portTmp->setParity(QSerialPort::NoParity);
                          portTmp->setStopBits(QSerialPort::OneStop);
     //                     QMessageBox::information(this, "", tr("Ошибка выключения режима M-Bus"));
                          label_StatusBar = (tr("Ошибка открытия парольной сессии по M-Bus") +
                                                       ". Рабочее место: " + QString::number(workPlaceNumber+1));
                          emit errorStringSignal(label_StatusBar + '\n');
                          vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                          emit checkMbusOff(currentIndicatorNumber);
                          emit workPlaceOff(currentIndicatorNumber);
                          return;
                       }
                   }



               }


            }

            //
            //
            //


            portTmp->setBaudRate(QSerialPort::Baud2400);
            portTmp->setDataBits(QSerialPort::Data8);
            portTmp->setParity(QSerialPort::EvenParity);
            portTmp->setStopBits(QSerialPort::OneStop);

            //команда изменения протокола 68 11 11 68 53 fe 51 0d 7C 04 55 4E 45 4D 06 54 45 53 74 72 50
            for(int i=0; i<3;i++) {

               portTmp->clear();
               packetToRead.clear();
               buffer.clear();

               quint8 byte = 0x68;
               packetToRead.append(byte);
               byte = 0x11;
               packetToRead.append(byte);
               byte = 0x11;
               packetToRead.append(byte);
               byte = 0x68;
               packetToRead.append(byte);
               byte = 0x53;
               packetToRead.append(byte);
               byte = 0xfe;
               packetToRead.append(byte);
               byte = 0x51;
               packetToRead.append(byte);
               byte = 0x0d;
               packetToRead.append(byte);
               byte = 0x7c;
               packetToRead.append(byte);
               byte = 0x04;
               packetToRead.append(byte);
               byte = 0x55;
               packetToRead.append(byte);
               byte = 0x4e;
               packetToRead.append(byte);
               byte = 0x45;
               packetToRead.append(byte);
               byte = 0x4d;
               packetToRead.append(byte);
               byte = 0x06;
               packetToRead.append(byte);
               byte = 0x54;
               packetToRead.append(byte);
               byte = 0x45;
               packetToRead.append(byte);
               byte = 0x53;
               packetToRead.append(byte);
               byte = 0x74;
               packetToRead.append(byte);
               byte = 0x72;
               packetToRead.append(byte);
               byte = 0x50;
               packetToRead.append(byte);

               quint32 sum = 0;
               for(int j=4; j<packetToRead.size(); j++) {
                   sum = sum + (quint8)packetToRead[j];
               }

               quint8 crcMod256 = (quint8)sum;
               packetToRead.append(crcMod256);
               byte = 0x16;
               packetToRead.append(byte);

               quint64 cnt = portTmp->write(packetToRead);
               emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

               qDebug()<<"cnt "<<cnt
                       <<"packetToRead.toHex() "<<packetToRead.toHex()
                       <<"portTmp->portName() "<<portTmp->portName();

               if(cnt == 0) {
      //             QMessageBox::information(this, "", tr("Данные в порт не записаны"));
                   label_StatusBar = (tr("Данные в порт не записаны") +
                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                   emit checkMbusOff(currentIndicatorNumber);
                   emit workPlaceOff(currentIndicatorNumber);
                   return;
               }

               global::pause(300);

               buffer = portTmp->readAll();
               qDebug()<<"buffer.toHex()"<<buffer.toHex();
               if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

               if(buffer.isEmpty()) {
                   if(i==2) {
                       portTmp->setBaudRate(QSerialPort::Baud19200);
                       portTmp->setDataBits(QSerialPort::Data8);
                       portTmp->setParity(QSerialPort::NoParity);
                       portTmp->setStopBits(QSerialPort::OneStop);
        //               QMessageBox::information(this, "", tr("Ошибка выключения режима M-Bus"));
    //                   ui->label_StatusBar->setText(tr("Ошибка выключения режима M-Bus") +
    //                                                ". Рабочее место: " + QString::number(workPlaceNumber+1));
    //                   errorString.append(ui->label_StatusBar->text() + '\n');
    //                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
    //                   checkMbusOff(currentIndicatorNumber);
    //                   workPlaceOff(currentIndicatorNumber);
                       return;
                   } else{}

               }
               else {
                   //проверяем crc и первые четыре байта ответного пакета


                   if((quint8)buffer[0] == 0xe5) {

                      qDebug()<<"buffer.toHex() "<<buffer.toHex()
                           <<"checkCRC(buffer) "<<checkCRC(buffer);

                      portTmp->setBaudRate(QSerialPort::Baud19200);
                      portTmp->setDataBits(QSerialPort::Data8);
                      portTmp->setParity(QSerialPort::NoParity);
                      portTmp->setStopBits(QSerialPort::OneStop);

                      //


                      //
                      //



                      break;
                   }
                   else {
                       if(i==2) {
                          portTmp->setBaudRate(QSerialPort::Baud19200);
                          portTmp->setDataBits(QSerialPort::Data8);
                          portTmp->setParity(QSerialPort::NoParity);
                          portTmp->setStopBits(QSerialPort::OneStop);
       //                   QMessageBox::information(this, "", tr("Ошибка выключения режима M-Bus"));
    //                      ui->label_StatusBar->setText(tr("Ошибка выключения режима M-Bus") +
    //                                                   ". Рабочее место: " + QString::number(workPlaceNumber+1));
    //                      errorString.append(ui->label_StatusBar->text() + '\n');
    //                      vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
    //                      checkMbusOff(currentIndicatorNumber);
    //                      workPlaceOff(currentIndicatorNumber);
                          return;
                       }
                   }



               }


            }


            checkMbusOff(currentBoxNumber);


            portTmp->setBaudRate(QSerialPort::Baud19200);
            portTmp->setDataBits(QSerialPort::Data8);
            portTmp->setParity(QSerialPort::NoParity);
            portTmp->setStopBits(QSerialPort::OneStop);


}

void ObjectThread::slotOnlyReadParams(QSerialPort *port1, QSerialPort *port2,
                                      QSerialPort *port3, QSerialPort *port4)
{

}

quint8 ObjectThread::makeCRC(QByteArray buffer)
{
    quint8 crc = 0x00;

    for(int i=1; i<buffer.size(); i++) {
        //reverse
        //
        quint8 byte = buffer.at(i);
        quint8 temp = reverse8(byte);

        crc ^= temp;

        for(int j=0; j<8; j++) {
            temp = crc & 0x80;
            crc = crc<<1;
            if(temp) {
                crc ^= 0x31;
            }
        }
    }

    crc = reverse8(crc);

    return crc;
}

quint8 ObjectThread::checkCRC(QByteArray buffer)
{
    quint8 crc = 0x00;

    for(int i=1; i<buffer.size(); i++) {
        //reverse
        //
        quint8 byte = buffer.at(i);
        quint8 temp = reverse8(byte);

        crc ^= temp;

        for(int j=0; j<8; j++) {
            temp = crc & 0x80;
            crc = crc<<1;
            if(temp) {
                crc ^= 0x31;
            }
        }
    }

    crc = reverse8(crc);

    return crc;
}

quint8 ObjectThread::reverse8(quint8 temp)
{
    quint8 result=0,
    i;
    for(i=0;i<8;i++) {
       if (temp & (1 << i)) {
          result |= 1 << (7-i);
       }
    }

//    qDebug()<<"QString::number(result) "<<QString::number(result);

    return result;
}

bool ObjectThread::packetSETProcessing(QByteArray pack)
{
    //проверяем первые 2 байта

    if( ((quint8)pack.at(0) != 0x5a) || ((quint8)pack.at(1) != 0x00) || pack.size()<4) {
        emit errorStringSignal(tr("Некорректный пакет") + "\n");
        return false;
    }

    int len = (quint8)pack.at(2);

    if( pack.size() != len ) {
        emit errorStringSignal(tr("Hеверная длина пакета") + "\n");
        //неверная длина пакета
        return false;
    }


    return true;

}

QString ObjectThread::getR_Ref1_1()
{
    return r_ref1Result;
}
QString ObjectThread::getR_Ref2_1()
{
    return r_ref2Result;
}
QString ObjectThread::getR_Ref1_2()
{
    return r_ref1Result2;
}
QString ObjectThread::getR_Ref2_2()
{
    return r_ref2Result2;
}
QString ObjectThread::getR_Ref1_3()
{
    return r_ref1Result3;
}
QString ObjectThread::getR_Ref2_3()
{
    return r_ref2Result3;
}
QString ObjectThread::getR_Ref1_4()
{
    return r_ref1Result4;
}
QString ObjectThread::getR_Ref2_4()
{
    return r_ref2Result4;
}

//------------------------------------------------------------------
//---------------------SmartStend-----------------------------------
//------------------------------------------------------------------

void ObjectThread::getPortStendName(QSerialPort *port)
{
    portStend = port;
}

void ObjectThread::sendCommandToStend(QString cmd, int workplace)
{

}

void ObjectThread::getPortBSL(QSerialPort *port, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4)
{
    if(workPlace == 0) portBSL = port;
    if(workPlace == 1) portBSL = port2;
    if(workPlace == 2) portBSL = port3;
    if(workPlace == 3) portBSL = port4;
}

void ObjectThread::bslProgramming()//аргумент не используется
{
    if(!isWorkPlaceUse.at(workPlace)) return;

    if(!errorIndicatorOff()) return;//отключить индикатор "авария"

    if(!programmatorOn()) return;
    if(!plataOn()) return;

    processData.clear();
 //   ui->textBrowser->clear();

    //специальные настройки для windows
    if(QSysInfo::productType()=="windows")
    {
        QTextCodec *codec = QTextCodec::codecForName("IBM 866");
    }

   //формирование команды
    QString strCommand;

    if(workPlace == 0)
        strCommand = "cmd /C BSL_Scripter scriptProgramming.txt";//"cmd.exe ";
    if(workPlace == 1)
        strCommand = "cmd /C BSL_Scripter scriptProgramming_2.txt";//"cmd.exe ";
    if(workPlace == 2)
        strCommand = "cmd /C BSL_Scripter scriptProgramming_3.txt";//"cmd.exe ";
    if(workPlace == 3)
        strCommand = "cmd /C BSL_Scripter scriptProgramming_4.txt";//"cmd.exe ";

    cmd->start(strCommand/*, listParam*/);
}

void ObjectThread::slotTokPlatyRequest()
{
    if(!isWorkPlaceUse.at(workPlace)) return;

    bool a = readTok();

}

void ObjectThread::slotProcessReadyRead()
{
    bool isBslOk = false;

    processData = "";
    qDebug()<<"MainWindow::slotReadyRead()";
    QTextCodec *codec = QTextCodec::codecForName("IBM 866");
    processData = codec->toUnicode(cmd->readAll());//readAll();

    emit textBrowser("<< " + portBSL->portName() + " " + processData);

 //   ui->textEdit->append(processData);//setText(processData);

    qDebug()<<"processData"<<processData;

    if(processData.isEmpty()) {
        QString label_StatusBar = tr("Программирование BSL неудачно . Рабочее место: ") + QString::number(workPlace+1);
        emit errorStringSignal(label_StatusBar + '\n');
        vectorIndicatorBSLMatrix[workPlace] = true;

        emit workPlaceOff(workPlace);
        emit checkBslError(workPlace);

        programmatorOff();
        plataOff();
        errorIndicatorOn();//включить индикатор АВАРИЯ

        return;
    }

    qDebug()<<"MainWindow::slotTimerStageTwo()";
 //   timerStageTwo->stop();
    QStringList list;
    QString sym = "";
    for(int i=0; i<processData.size(); i++) {
        sym = sym + processData[i];
        if(processData[i+1]=='\n') {
            list<<sym;
            sym = "";
            i=i+1;
        }

    }

    qDebug()<<"list"<<list;
    //находим строку со словом Writing
    int rowIndex;
    for(int i=0; i<list.size(); i++) {
        QString row = list.at(i);
        if(row.contains("Writing")) {
            rowIndex = i;
        }
        else {}
    }

    //
    QString rowWithWord = list.at(rowIndex);

    //проверяем, содержиться ли в строке со словом Writing слово DONE
    if(rowWithWord.contains("DONE")) {

        isBslOk = true;//программирование удачно

        programmatorOff();
        plataOff();

        processData.clear();

    }
    else { //программирование не удачно

        QString label_StatusBar = tr("Программирование BSL неудачно . Рабочее место: ") + QString::number(workPlace+1);
        emit errorStringSignal(label_StatusBar + '\n');
        vectorIndicatorBSLMatrix[workPlace] = true;

        emit workPlaceOff(workPlace);
        emit checkBslError(workPlace);

        programmatorOff();
        plataOff();
        errorIndicatorOn();//включить индикатор АВАРИЯ

        return;
    }


    emit checkBslError(workPlace);

}

void ObjectThread::slotGetAnsFromStend(QString answer)
{

//                                      Прерывание	Ответ РС
//Нажата кнопка ПРОШИТЬ	                BUTPRG=1	OK
//Нажата кнопка КАЛИБРОВКА	            BUTCAL=1	OK
//Ток потребления платы превышен	    OVRLD	    OK
//Ток потребления интерфейса превышен	INVALID	    OK



   if(answer == "BUTPRG=1") {//начать программирование bsl
       sendCommandToStend("OK", workPlace);
       //start bsl
   }

   if(answer == "BUTCAL=1") {//начать процесс выполнения команд
       sendCommandToStend("OK", workPlace);
       //start write & calibration
   }

   if(answer == "OVRLD") {//?
       sendCommandToStend("OK", workPlace);
   }

   if(answer == "INVALID") {//?
       sendCommandToStend("OK", workPlace);
   }

}

//------------------Проверка импульсных выходов-------------------

bool ObjectThread::readPulsesChannel1()
{
   //Чтение значения счётчика импульсов, канал 1	CNT1?	CNT1=xxx
    pulsesValueCh1 = 0;

    QByteArray buffer;

//    кто и кому:
//    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    кто	    кому	команда					                    Конец сообщения
//    BIN	    BIN	    STRING					                    0x00

    if(!isWorkPlaceUse.at(workPlace)) return false;

    QString command = "CNT1?";

    QByteArray parcel;

    quint8 sender = 0x01;
    quint8 receiver;
    quint8 stopByte = 0x00;

    if(workPlace == 0) receiver = 0x11;
    if(workPlace == 1) receiver = 0x22;
    if(workPlace == 2) receiver = 0x33;
    if(workPlace == 3) receiver = 0x44;

    QByteArray stringToByteArray;
    stringToByteArray = command.toLocal8Bit();

    //формирование посылки
    parcel.append(sender);
    parcel.append(receiver);
    for(int m=0; m<stringToByteArray.size(); m++) {
        parcel.append(stringToByteArray.at(m));
    }
    parcel.append(stopByte);

    qDebug()<<"parcel"<<parcel.toHex();

    if(!portStend->isOpen()) {

        if(!portStend->open(QIODevice::ReadWrite)) {

            QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
                                         ". Рабочее место: " + QString::number(workPlace+1));
            emit errorStringSignal(label_StatusBar + '\n');
            return false;
        }
    }

    portStend->clear();

    quint64 count = portStend->write(parcel);
    emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

    //ждать ответа от стенда 10 мсек

    global::pause(50);

    buffer = portStend->readAll();
//    portStend->close();

    if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

    if(buffer.isEmpty()) {
        QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Чтение значения счётчика импульсов, канал 1\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
  //      portStend->close();
        return false;
    }

    QString answerStr;
    QByteArray bufTmp = buffer;

    bufTmp.remove(0, 2);
    bufTmp.remove(bufTmp.size()-1, 1);

    answerStr = QString::fromLocal8Bit(bufTmp);

    pulsesValueCh1 = answerStr.remove("CNT1=").toInt();

    if(buffer.at(0)!=receiver && buffer.at(1)!=sender /*&& answerStr!="OK"*/) {
        QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Чтение значения счётчика импульсов, канал 1\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
 //       portStend->close();
        return false;
    }

 //  portStend->close();

   return true;

}

bool ObjectThread::readPulsesChannel2()
{
   //Чтение значения счётчика импульсов, канал 2	CNT2?	CNT2=xxx

    pulsesValueCh2 = 0;

    QByteArray buffer;

//    кто и кому:
//    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    кто	    кому	команда					                    Конец сообщения
//    BIN	    BIN	    STRING					                    0x00

    if(!isWorkPlaceUse.at(workPlace)) return false;

    QString command = "CNT2?";

    QByteArray parcel;

    quint8 sender = 0x01;
    quint8 receiver;
    quint8 stopByte = 0x00;

    if(workPlace == 0) receiver = 0x11;
    if(workPlace == 1) receiver = 0x22;
    if(workPlace == 2) receiver = 0x33;
    if(workPlace == 3) receiver = 0x44;

    QByteArray stringToByteArray;
    stringToByteArray = command.toLocal8Bit();

    //формирование посылки
    parcel.append(sender);
    parcel.append(receiver);
    for(int m=0; m<stringToByteArray.size(); m++) {
        parcel.append(stringToByteArray.at(m));
    }
    parcel.append(stopByte);

    qDebug()<<"parcel"<<parcel.toHex();

    if(!portStend->isOpen()) {

        if(!portStend->open(QIODevice::ReadWrite)) {

            QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
                                         ". Рабочее место: " + QString::number(workPlace+1));
            emit errorStringSignal(label_StatusBar + '\n');
            return false;
        }
    }

    portStend->clear();

    quint64 count = portStend->write(parcel);
    emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

    //ждать ответа от стенда 10 мсек

    global::pause(50);

    buffer = portStend->readAll();
  //  portStend->close();

    if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

    if(buffer.isEmpty()) {
        QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Чтение значения счётчика импульсов, канал 2\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
  //      portStend->close();
        return false;
    }

    QString answerStr;
    QByteArray bufTmp = buffer;

    bufTmp.remove(0, 2);
    bufTmp.remove(bufTmp.size()-1, 1);

    answerStr = QString::fromLocal8Bit(bufTmp);

    pulsesValueCh2 = answerStr.remove("CNT2=").toInt();

    if(buffer.at(0)!=receiver && buffer.at(1)!=sender /*&& answerStr!="OK"*/) {
        QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Чтение значения счётчика импульсов, канал 2\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
 //       portStend->close();
        return false;
    }

 //  portStend->close();

   return true;
}

bool ObjectThread::pulsesReset()
{
   //Сбросить счётчик импульсов, оба канала	CNTCLR	ОК

    pulsesValueCh1 = 0;
    pulsesValueCh2 = 0;

    QByteArray buffer;

//    кто и кому:
//    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    кто	    кому	команда					                    Конец сообщения
//    BIN	    BIN	    STRING					                    0x00

    if(!isWorkPlaceUse.at(workPlace)) return false;

    QString command = "CNTCLR";

    QByteArray parcel;

    quint8 sender = 0x01;
    quint8 receiver;
    quint8 stopByte = 0x00;

    if(workPlace == 0) receiver = 0x11;
    if(workPlace == 1) receiver = 0x22;
    if(workPlace == 2) receiver = 0x33;
    if(workPlace == 3) receiver = 0x44;

    QByteArray stringToByteArray;
    stringToByteArray = command.toLocal8Bit();

    //формирование посылки
    parcel.append(sender);
    parcel.append(receiver);
    for(int m=0; m<stringToByteArray.size(); m++) {
        parcel.append(stringToByteArray.at(m));
    }
    parcel.append(stopByte);

    qDebug()<<"parcel"<<parcel.toHex();

    if(!portStend->isOpen()) {

        if(!portStend->open(QIODevice::ReadWrite)) {

            QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
                                         ". Рабочее место: " + QString::number(workPlace+1));
            emit errorStringSignal(label_StatusBar + '\n');
            return false;
        }
    }

    portStend->clear();

    quint64 count = portStend->write(parcel);
    emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

    //ждать ответа от стенда 10 мсек

    global::pause(100);

    buffer = portStend->readAll();
  //  portStend->close();

    if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

    if(buffer.isEmpty()) {
        QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Сбросить счётчик импульсов\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
  //      portStend->close();
        return false;
    }

    QString answerStr;
    QByteArray bufTmp = buffer;

    bufTmp.remove(0, 2);
    bufTmp.remove(bufTmp.size()-1, 1);

    answerStr = QString::fromLocal8Bit(bufTmp);

    if(buffer.at(0)!=receiver && buffer.at(1)!=sender && answerStr!="OK") {
        QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Сбросить счётчик импульсов\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
 //       portStend->close();
        return false;
    }

 //  portStend->close();

   return true;
}

//------------------Проверка импульсных входов--------------------

bool ObjectThread::writePulsesToGenChannel1()
{
   //Запись в генератор импульсов, канал 1	GEN1=xxx	OK

    QByteArray buffer;

//    кто и кому:
//    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    кто	    кому	команда					                    Конец сообщения
//    BIN	    BIN	    STRING					                    0x00

    if(!isWorkPlaceUse.at(workPlace)) return false;

    QString command = "GEN1=3";

    QByteArray parcel;

    quint8 sender = 0x01;
    quint8 receiver;
    quint8 stopByte = 0x00;

    if(workPlace == 0) receiver = 0x11;
    if(workPlace == 1) receiver = 0x22;
    if(workPlace == 2) receiver = 0x33;
    if(workPlace == 3) receiver = 0x44;

    QByteArray stringToByteArray;
    stringToByteArray = command.toLocal8Bit();

    //формирование посылки
    parcel.append(sender);
    parcel.append(receiver);
    for(int m=0; m<stringToByteArray.size(); m++) {
        parcel.append(stringToByteArray.at(m));
    }
    parcel.append(stopByte);

    qDebug()<<"parcel"<<parcel.toHex();

    if(!portStend->isOpen()) {

        if(!portStend->open(QIODevice::ReadWrite)) {

            QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
                                         ". Рабочее место: " + QString::number(workPlace+1));
            emit errorStringSignal(label_StatusBar + '\n');
            return false;
        }
    }

    portStend->clear();

    quint64 count = portStend->write(parcel);
    emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

    //ждать ответа от стенда 10 мсек

    global::pause(100);

    buffer = portStend->readAll();
 //   portStend->close();

    if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

    if(buffer.isEmpty()) {
        QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Запись в генератор импульсов, канал 1\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
  //      portStend->close();
        return false;
    }

    QString answerStr;
    QByteArray bufTmp = buffer;

    bufTmp.remove(0, 2);
    bufTmp.remove(bufTmp.size()-1, 1);

    answerStr = QString::fromLocal8Bit(bufTmp);

    if(buffer.at(0)!=receiver && buffer.at(1)!=sender && answerStr!="OK") {
        QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Запись в генератор импульсов, канал 1\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
 //       portStend->close();
        return false;
    }

 //  portStend->close();

   return true;
}

bool ObjectThread::writePulsesToGenChannel2()
{
   //Запись числа импульсов в генератор импульсов, канал 2	GEN2=xxx	OK

    QByteArray buffer;

//    кто и кому:
//    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    кто	    кому	команда					                    Конец сообщения
//    BIN	    BIN	    STRING					                    0x00

    if(!isWorkPlaceUse.at(workPlace)) return false;

    QString command = "GEN2=3";

    QByteArray parcel;

    quint8 sender = 0x01;
    quint8 receiver;
    quint8 stopByte = 0x00;

    if(workPlace == 0) receiver = 0x11;
    if(workPlace == 1) receiver = 0x22;
    if(workPlace == 2) receiver = 0x33;
    if(workPlace == 3) receiver = 0x44;

    QByteArray stringToByteArray;
    stringToByteArray = command.toLocal8Bit();

    //формирование посылки
    parcel.append(sender);
    parcel.append(receiver);
    for(int m=0; m<stringToByteArray.size(); m++) {
        parcel.append(stringToByteArray.at(m));
    }
    parcel.append(stopByte);

    qDebug()<<"parcel"<<parcel.toHex();

    if(!portStend->isOpen()) {

        if(!portStend->open(QIODevice::ReadWrite)) {

            QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
                                         ". Рабочее место: " + QString::number(workPlace+1));
            emit errorStringSignal(label_StatusBar + '\n');
            return false;
        }
    }

    portStend->clear();

    quint64 count = portStend->write(parcel);
    emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

    //ждать ответа от стенда 10 мсек

    global::pause(100);

    buffer = portStend->readAll();
 //   portStend->close();

    if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

    if(buffer.isEmpty()) {
        QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Запись в генератор импульсов, канал 2\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
  //      portStend->close();
        return false;
    }

    QString answerStr;
    QByteArray bufTmp = buffer;

    bufTmp.remove(0, 2);
    bufTmp.remove(bufTmp.size()-1, 1);

    answerStr = QString::fromLocal8Bit(bufTmp);

    if(buffer.at(0)!=receiver && buffer.at(1)!=sender && answerStr!="OK") {
        QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Запись в генератор импульсов, канал 2\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
 //       portStend->close();
        return false;
    }

 //  portStend->close();

   return true;
}

//-----------------Внешний интерфейс обмена-----------------------

bool ObjectThread::externalInterfaceOn()
{
   //Подключить внешний интерфейс	EXT=1	OK

    QByteArray buffer;

//    кто и кому:
//    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    кто	    кому	команда					                    Конец сообщения
//    BIN	    BIN	    STRING					                    0x00

    if(!isWorkPlaceUse.at(workPlace)) return false;

    QString command = "EXT=1";

    QByteArray parcel;

    quint8 sender = 0x01;
    quint8 receiver;
    quint8 stopByte = 0x00;

    if(workPlace == 0) receiver = 0x11;
    if(workPlace == 1) receiver = 0x22;
    if(workPlace == 2) receiver = 0x33;
    if(workPlace == 3) receiver = 0x44;

    QByteArray stringToByteArray;
    stringToByteArray = command.toLocal8Bit();

    //формирование посылки
    parcel.append(sender);
    parcel.append(receiver);
    for(int m=0; m<stringToByteArray.size(); m++) {
        parcel.append(stringToByteArray.at(m));
    }
    parcel.append(stopByte);

    qDebug()<<"parcel"<<parcel.toHex();

    if(!portStend->isOpen()) {

        if(!portStend->open(QIODevice::ReadWrite)) {

            QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
                                         ". Рабочее место: " + QString::number(workPlace+1));
            emit errorStringSignal(label_StatusBar + '\n');
            return false;
        }
    }

    portStend->clear();

    quint64 count = portStend->write(parcel);
    emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

    //ждать ответа от стенда 10 мсек

    global::pause(100);

    buffer = portStend->readAll();
 //   portStend->close();

    if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

    if(buffer.isEmpty()) {
        QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Подключить внешний интерфейс\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
  //      portStend->close();
        return false;
    }

    QString answerStr;
    QByteArray bufTmp = buffer;

    bufTmp.remove(0, 2);
    bufTmp.remove(bufTmp.size()-1, 1);

    answerStr = QString::fromLocal8Bit(bufTmp);

    if(buffer.at(0)!=receiver && buffer.at(1)!=sender && answerStr!="OK") {
        QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Подключить внешний интерфейс\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
 //       portStend->close();
        return false;
    }

 //  portStend->close();

   return true;
}

bool ObjectThread::externalInterfaceOff()
{
   //Отключить внешний интерфейс	 EXT=0	OK

    QByteArray buffer;

//    кто и кому:
//    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    кто	    кому	команда					                    Конец сообщения
//    BIN	    BIN	    STRING					                    0x00

    if(!isWorkPlaceUse.at(workPlace)) return false;

    QString command = "EXT=0";

    QByteArray parcel;

    quint8 sender = 0x01;
    quint8 receiver;
    quint8 stopByte = 0x00;

    if(workPlace == 0) receiver = 0x11;
    if(workPlace == 1) receiver = 0x22;
    if(workPlace == 2) receiver = 0x33;
    if(workPlace == 3) receiver = 0x44;

    QByteArray stringToByteArray;
    stringToByteArray = command.toLocal8Bit();

    //формирование посылки
    parcel.append(sender);
    parcel.append(receiver);
    for(int m=0; m<stringToByteArray.size(); m++) {
        parcel.append(stringToByteArray.at(m));
    }
    parcel.append(stopByte);

    qDebug()<<"parcel"<<parcel.toHex();

    if(!portStend->isOpen()) {

        if(!portStend->open(QIODevice::ReadWrite)) {

            QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
                                         ". Рабочее место: " + QString::number(workPlace+1));
            emit errorStringSignal(label_StatusBar + '\n');
            return false;
        }
    }

    portStend->clear();

    quint64 count = portStend->write(parcel);
    emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

    //ждать ответа от стенда 10 мсек

    global::pause(100);

    buffer = portStend->readAll();
  //  portStend->close();

    if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

    if(buffer.isEmpty()) {
        QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Отключить внешний интерфейс\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
  //      portStend->close();
        return false;
    }

    QString answerStr;
    QByteArray bufTmp = buffer;

    bufTmp.remove(0, 2);
    bufTmp.remove(bufTmp.size()-1, 1);

    answerStr = QString::fromLocal8Bit(bufTmp);

    if(buffer.at(0)!=receiver && buffer.at(1)!=sender && answerStr!="OK") {
        QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Отключить внешний интерфейс\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
 //       portStend->close();
        return false;
    }

 //  portStend->close();

   return true;
}

bool ObjectThread::setTokInterfaceOff()
{
   //Установить ток отключения (в мА)	EXTSET=xxx	OK
}

//-----------------Калибровка частоты 512------------------------

bool ObjectThread::freqMeterOn()
{
   //Включить частотомер	FREQ=1	OK
}

bool ObjectThread::freqMeterOff()
{
   //Выключить частотомер	FREQ=0	OK
}

bool ObjectThread::readFreq()
{
   //Прочитать значение частоты	FREQ?	FREQ=xxx..xxx(в Гц)

    QByteArray buffer;

//    кто и кому:
//    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    кто	    кому	команда					                    Конец сообщения
//    BIN	    BIN	    STRING					                    0x00

    if(!isWorkPlaceUse.at(workPlace)) return false;

    periodClock = 0;

    QString command = "FREQ?";

    QByteArray parcel;

    quint8 sender = 0x01;
    quint8 receiver;
    quint8 stopByte = 0x00;

    if(workPlace == 0) receiver = 0x11;
    if(workPlace == 1) receiver = 0x22;
    if(workPlace == 2) receiver = 0x33;
    if(workPlace == 3) receiver = 0x44;

    QByteArray stringToByteArray;
    stringToByteArray = command.toLocal8Bit();

    //формирование посылки
    parcel.append(sender);
    parcel.append(receiver);
    for(int m=0; m<stringToByteArray.size(); m++) {
        parcel.append(stringToByteArray.at(m));
    }
    parcel.append(stopByte);

    qDebug()<<"parcel"<<parcel.toHex();

    if(!portStend->isOpen()) {

        if(!portStend->open(QIODevice::ReadWrite)) {

            QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
                                         ". Рабочее место: " + QString::number(workPlace+1));
            emit errorStringSignal(label_StatusBar + '\n');
            return false;
        }
    }

    portStend->clear();

    quint64 count = portStend->write(parcel);
    emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

    //ждать ответа от стенда 10 мсек

    global::pause(100);

    buffer = portStend->readAll();
 //   portStend->close();

    if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

    if(buffer.isEmpty()) {
        QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Прочитать значение периода\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
  //      portStend->close();
        return false;
    }

    QString answerStr;
    QByteArray bufTmp = buffer;

    bufTmp.remove(0, 2);
    bufTmp.remove(bufTmp.size()-1, 1);

    answerStr = QString::fromLocal8Bit(bufTmp);

    if(buffer.at(0)!=receiver && buffer.at(1)!=sender && answerStr.left(5)!="FREQ=") {
        QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Прочитать значение периода\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
 //       portStend->close();
        return false;
    }

    periodClock = answerStr.remove(0, 5).toFloat();//прочитываеттся в микросекундах, надо делить на 1000, чтобы получить мс
    periodClock = periodClock/1000;

 //  portStend->close();

   return true;
}

//-----------------Ток потребления платы--------------------------

bool ObjectThread::plataOn()
{
   //Подать питание на плату	POW=1	OK

    QByteArray buffer;

//    кто и кому:
//    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    кто	    кому	команда					                    Конец сообщения
//    BIN	    BIN	    STRING					                    0x00

    if(!isWorkPlaceUse.at(workPlace)) return false;

   //Подать питание на плату	POW=1	OK

    QString command = "POW=1";

    QByteArray parcel;

    quint8 sender = 0x01;
    quint8 receiver;
    quint8 stopByte = 0x00;

    if(workPlace == 0) receiver = 0x11;
    if(workPlace == 1) receiver = 0x22;
    if(workPlace == 2) receiver = 0x33;
    if(workPlace == 3) receiver = 0x44;

    QByteArray stringToByteArray;
    stringToByteArray = command.toLocal8Bit();

    //формирование посылки
    parcel.append(sender);
    parcel.append(receiver);
    for(int m=0; m<stringToByteArray.size(); m++) {
        parcel.append(stringToByteArray.at(m));
    }
    parcel.append(stopByte);

    qDebug()<<"parcel"<<parcel.toHex();

    if(!portStend->isOpen()) {

        if(!portStend->open(QIODevice::ReadWrite)) {

            QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
                                         ". Рабочее место: " + QString::number(workPlace+1));
            emit errorStringSignal(label_StatusBar + '\n');
            vectorIndicatorBSLMatrix[workPlace] = true;

            emit workPlaceOff(workPlace);
            emit checkBslError(workPlace);
            return false;
        }
    }

    portStend->clear();

    quint64 count = portStend->write(parcel);
    emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

    //ждать ответа от стенда 10 мсек

    global::pause(100);

    buffer = portStend->readAll();
 //   portStend->close();

    if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

    if(buffer.isEmpty()) {
        QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Подать питание на плату\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
        vectorIndicatorBSLMatrix[workPlace] = true;
        emit workPlaceOff(workPlace);
        emit checkBslError(workPlace);

  //      portStend->close();
        return false;
    }

    QString answerStr;
    QByteArray bufTmp = buffer;

    bufTmp.remove(0, 2);
    bufTmp.remove(bufTmp.size()-1, 1);

    answerStr = QString::fromLocal8Bit(bufTmp);

    if(buffer.at(0)!=receiver && buffer.at(1)!=sender && answerStr!="OK") {
        QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Подать питание на плату\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
        vectorIndicatorBSLMatrix[workPlace] = true;
        emit workPlaceOff(workPlace);
        emit checkBslError(workPlace);
 //       portStend->close();
        return false;
    }

 //  portStend->close();

   return true;
}

bool ObjectThread::plataOff()
{
   //Отключить питание от платы	POW=0	OK

    QByteArray buffer;

//    кто и кому:
//    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    кто	    кому	команда					                    Конец сообщения
//    BIN	    BIN	    STRING					                    0x00

    if(!isWorkPlaceUse.at(workPlace)) return false;

   //Отключить питание от платы	POW=0	OK

    QString command = "POW=0";

    QByteArray parcel;

    quint8 sender = 0x01;
    quint8 receiver;
    quint8 stopByte = 0x00;

    if(workPlace == 0) receiver = 0x11;
    if(workPlace == 1) receiver = 0x22;
    if(workPlace == 2) receiver = 0x33;
    if(workPlace == 3) receiver = 0x44;

    QByteArray stringToByteArray;
    stringToByteArray = command.toLocal8Bit();

    //формирование посылки
    parcel.append(sender);
    parcel.append(receiver);
    for(int m=0; m<stringToByteArray.size(); m++) {
        parcel.append(stringToByteArray.at(m));
    }
    parcel.append(stopByte);

    qDebug()<<"parcel"<<parcel.toHex();

    if(!portStend->isOpen()) {

        if(!portStend->open(QIODevice::ReadWrite)) {

            QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
                                         ". Рабочее место: " + QString::number(workPlace+1));
            emit errorStringSignal(label_StatusBar + '\n');
            vectorIndicatorBSLMatrix[workPlace] = true;
            emit workPlaceOff(workPlace);
            emit checkBslError(workPlace);
            return false;
        }
    }

    portStend->clear();

    quint64 count = portStend->write(parcel);
    emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

    //ждать ответа от стенда 10 мсек

    global::pause(100);

    buffer = portStend->readAll();
 //   portStend->close();

    if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

    if(buffer.isEmpty()) {
        QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Отключить питание платы\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
        vectorIndicatorBSLMatrix[workPlace] = true;
        emit workPlaceOff(workPlace);
        emit checkBslError(workPlace);
 //       portStend->close();
        return false;
    }

    QString answerStr;
    QByteArray bufTmp = buffer;

    bufTmp.remove(0, 2);
    bufTmp.remove(bufTmp.size()-1, 1);

    answerStr = QString::fromLocal8Bit(bufTmp);

    if(buffer.at(0)!=receiver && buffer.at(1)!=sender && answerStr!="OK") {
        QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Отключить питание платы\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
        vectorIndicatorBSLMatrix[workPlace] = true;
        emit workPlaceOff(workPlace);
        emit checkBslError(workPlace);
  //      portStend->close();
        return false;
    }

 //   portStend->close();

   return true;
}

bool ObjectThread::readTok()
{
    //подключить плату
    if(!plataOn()) {
        QString label_StatusBar = (tr("Не удалось включить питание платы") +
                                  ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
        vectorIndicatorTokPlaty[workPlace] = true;
        errorIndicatorOn();
        emit workPlaceOff(workPlace);
        emit checkTokPlaty(workPlace);

        errorIndicatorOn();
        return false;
    }

//    QByteArray buffer;

//   //Прочитать ток потребления	POW?	POW=xxxxxx(в мкА)

//    //    кто и кому:
//    //    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    //    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    //    кто	    кому	команда					                    Конец сообщения
//    //    BIN	    BIN	    STRING					                    0x00

//        if(!isWorkPlaceUse.at(workPlace)) return false;

//        QString command = "POW?";

//        QByteArray parcel;

//        quint8 sender = 0x01;
//        quint8 receiver;
//        quint8 stopByte = 0x00;

//        if(workPlace == 0) receiver = 0x11;
//        if(workPlace == 1) receiver = 0x22;
//        if(workPlace == 2) receiver = 0x33;
//        if(workPlace == 3) receiver = 0x44;

//        QByteArray stringToByteArray;
//        stringToByteArray = command.toLocal8Bit();

//        //формирование посылки
//        parcel.append(sender);
//        parcel.append(receiver);
//        for(int m=0; m<stringToByteArray.size(); m++) {
//            parcel.append(stringToByteArray.at(m));
//        }
//        parcel.append(stopByte);

//        qDebug()<<"parcel"<<parcel.toHex();

//        if(!portStend->isOpen()) {

//            if(!portStend->open(QIODevice::ReadWrite)) {

//                QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
//                                             ". Рабочее место: " + QString::number(workPlace+1));
//                emit errorStringSignal(label_StatusBar + '\n');
//                vectorIndicatorTokPlaty[workPlace] = true;

//                emit workPlaceOff(workPlace);
//                emit checkTokPlaty(workPlace);
//                return false;
//            }
//        }

//        portStend->clear();

//        quint64 count = portStend->write(parcel);
//        emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

//        //ждать ответа от стенда 10 мсек

//        global::pause(100);

//        buffer = portStend->readAll();
//        portStend->close();

//        if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

//        if(buffer.isEmpty()) {
//            QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Прочитать ток платы\" ") +
//                                         ". Рабочее место: " + QString::number(workPlace+1));
//            emit errorStringSignal(label_StatusBar + '\n');
//            vectorIndicatorTokPlaty[workPlace] = true;
//            emit workPlaceOff(workPlace);
//            emit checkTokPlaty(workPlace);

//            return false;
//        }

//        QString answerStr;
//        QByteArray bufTmp = buffer;

//        bufTmp.remove(0, 2);
//        bufTmp.remove(bufTmp.size()-1, 1);

//        answerStr = QString::fromLocal8Bit(bufTmp);

//        if(buffer.at(0)!=receiver && buffer.at(1)!=sender && answerStr.left(4) != "POW=") {
//            QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Прочитать ток платы\" ") +
//                                         ". Рабочее место: " + QString::number(workPlace+1));
//            emit errorStringSignal(label_StatusBar + '\n');
//            vectorIndicatorTokPlaty[workPlace] = true;
//            emit workPlaceOff(workPlace);
//            emit checkTokPlaty(workPlace);
//  //          portStend->close();
//            return false;
//        }

//       QString answerStrValue = answerStr.remove(0,4);

//       int tokPlaty = answerStrValue.toInt();

//       if(workPlace == 0) emit tok1(answerStrValue);
//       if(workPlace == 1) emit tok2(answerStrValue);
//       if(workPlace == 2) emit tok3(answerStrValue);
//       if(workPlace == 3) emit tok4(answerStrValue);

//       if(1) {
//           //проверяем ток на допустимое значение

//  //           if(workPlace == 0) mainWnd->setLabelTok1(tokPlaty);
//  //           if(workPlace == 1) mainWnd->setLabelTok2(tokPlaty);
//  //           if(workPlace == 2) mainWnd->setLabelTok3(tokPlaty);
//  //           if(workPlace == 3) mainWnd->setLabelTok4(tokPlaty);

//       }

//  //     portStend->close();

       emit checkTokPlaty(workPlace);

       return true;
}

bool ObjectThread::setTokPlataOff()
{
   //Установить ток отключения (в мА)	POWSET=xxx	OK
}

//--------------------Программирование-----------------------------

bool ObjectThread::programmatorOn()
{
    QByteArray buffer;

//    кто и кому:
//    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    кто	    кому	команда					                    Конец сообщения
//    BIN	    BIN	    STRING					                    0x00

    if(!isWorkPlaceUse.at(workPlace)) return false;

   //Подключить программатор к плате	PRG=1	OK

    QString command = "PRG=1";

    QByteArray parcel;

    quint8 sender = 0x01;
    quint8 receiver;
    quint8 stopByte = 0x00;

    if(workPlace == 0) receiver = 0x11;
    if(workPlace == 1) receiver = 0x22;
    if(workPlace == 2) receiver = 0x33;
    if(workPlace == 3) receiver = 0x44;

    QByteArray stringToByteArray;
    stringToByteArray = command.toLocal8Bit();

    //формирование посылки
    parcel.append(sender);
    parcel.append(receiver);
    for(int m=0; m<stringToByteArray.size(); m++) {
        parcel.append(stringToByteArray.at(m));
    }
    parcel.append(stopByte);

    qDebug()<<"parcel"<<parcel.toHex();

    if(!portStend->isOpen()) {

        if(!portStend->open(QIODevice::ReadWrite)) {

            QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
                                         ". Рабочее место: " + QString::number(workPlace+1));
            emit errorStringSignal(label_StatusBar + '\n');
            vectorIndicatorBSLMatrix[workPlace] = true;
            emit workPlaceOff(workPlace);
            emit checkBslError(workPlace);
            return false;
        }
    }

    portStend->clear();

    quint64 count = portStend->write(parcel);
    emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

    //ждать ответа от стенда 10 мсек

    global::pause(100);

    buffer = portStend->readAll();
  //  portStend->close();

    if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

    if(buffer.isEmpty()) {
        QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Подключить программатор\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
        vectorIndicatorBSLMatrix[workPlace] = true;
        emit workPlaceOff(workPlace);
        emit checkBslError(workPlace);
  //      portStend->close();
        return false;
    }

    QString answerStr;
    QByteArray bufTmp = buffer;

    bufTmp.remove(0, 2);
    bufTmp.remove(bufTmp.size()-1, 1);

    answerStr = QString::fromLocal8Bit(bufTmp);

    if(buffer.at(0)!=receiver && buffer.at(1)!=sender && answerStr!="OK") {
        QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Подключить программатор\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
        vectorIndicatorBSLMatrix[workPlace] = true;
        emit workPlaceOff(workPlace);
        emit checkBslError(workPlace);
 //       portStend->close();
        return false;
    }

 //   portStend->close();

   return true;

}

bool ObjectThread::programmatorOff()
{
   //Отключить программатор от платы	PRG=0	OK

    QByteArray buffer;

//    кто и кому:
//    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    кто	    кому	команда					                    Конец сообщения
//    BIN	    BIN	    STRING					                    0x00

    if(!isWorkPlaceUse.at(workPlace)) return false;

   //Отключить программатор от платы	PRG=0	OK

    QString command = "PRG=0";

    QByteArray parcel;

    quint8 sender = 0x01;
    quint8 receiver;
    quint8 stopByte = 0x00;

    if(workPlace == 0) receiver = 0x11;
    if(workPlace == 1) receiver = 0x22;
    if(workPlace == 2) receiver = 0x33;
    if(workPlace == 3) receiver = 0x44;

    QByteArray stringToByteArray;
    stringToByteArray = command.toLocal8Bit();

    //формирование посылки
    parcel.append(sender);
    parcel.append(receiver);
    for(int m=0; m<stringToByteArray.size(); m++) {
        parcel.append(stringToByteArray.at(m));
    }
    parcel.append(stopByte);

    qDebug()<<"parcel"<<parcel.toHex();

    if(!portStend->isOpen()) {

        if(!portStend->open(QIODevice::ReadWrite)) {

            QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
                                         ". Рабочее место: " + QString::number(workPlace+1));
            emit errorStringSignal(label_StatusBar + '\n');
            vectorIndicatorBSLMatrix[workPlace] = true;
            emit workPlaceOff(workPlace);
            emit checkBslError(workPlace);
            return false;
        }
    }

    portStend->clear();

    quint64 count = portStend->write(parcel);
    emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

    //ждать ответа от стенда 10 мсек

    global::pause(100);

    buffer = portStend->readAll();
 //   portStend->close();

    if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

    if(buffer.isEmpty()) {
        QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Отключить программатор\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
        vectorIndicatorBSLMatrix[workPlace] = true;
        emit workPlaceOff(workPlace);
        emit checkBslError(workPlace);
 //       portStend->close();
        return false;
    }

    QString answerStr;
    QByteArray bufTmp = buffer;

    bufTmp.remove(0, 2);
    bufTmp.remove(bufTmp.size()-1, 1);

    answerStr = QString::fromLocal8Bit(bufTmp);

    if(buffer.at(0)!=receiver && buffer.at(1)!=sender && answerStr!="OK") {
        QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Отключить программатор\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
        vectorIndicatorBSLMatrix[workPlace] = true;
        emit workPlaceOff(workPlace);
        emit checkBslError(workPlace);
  //      portStend->close();
        return false;
    }

 //   portStend->close();

   return true;
}

//--------------------Индикация-----------------------------------

bool ObjectThread::finishIndicatorOn()
{
   //Включить индикатор ЗАВЕРШЕНО	FIN=1	OK

    QByteArray buffer;

//    кто и кому:
//    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    кто	    кому	команда					                    Конец сообщения
//    BIN	    BIN	    STRING					                    0x00

    if(!isWorkPlaceUse.at(workPlace)) return false;

    QString command = "FIN=1";

    QByteArray parcel;

    quint8 sender = 0x01;
    quint8 receiver;
    quint8 stopByte = 0x00;

    if(workPlace == 0) receiver = 0x11;
    if(workPlace == 1) receiver = 0x22;
    if(workPlace == 2) receiver = 0x33;
    if(workPlace == 3) receiver = 0x44;

    QByteArray stringToByteArray;
    stringToByteArray = command.toLocal8Bit();

    //формирование посылки
    parcel.append(sender);
    parcel.append(receiver);
    for(int m=0; m<stringToByteArray.size(); m++) {
        parcel.append(stringToByteArray.at(m));
    }
    parcel.append(stopByte);

    qDebug()<<"parcel"<<parcel.toHex();

    if(!portStend->isOpen()) {

        if(!portStend->open(QIODevice::ReadWrite)) {

            QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
                                         ". Рабочее место: " + QString::number(workPlace+1));
            emit errorStringSignal(label_StatusBar + '\n');

            return false;
        }
    }

    portStend->clear();

    quint64 count = portStend->write(parcel);
    emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

    //ждать ответа от стенда 10 мсек

    global::pause(100);

    buffer = portStend->readAll();
 //   portStend->close();

    if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

    if(buffer.isEmpty()) {
        QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Включить индикацию Завершено\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');

        return false;
    }

    QString answerStr;
    QByteArray bufTmp = buffer;

    bufTmp.remove(0, 2);
    bufTmp.remove(bufTmp.size()-1, 1);

    answerStr = QString::fromLocal8Bit(bufTmp);

    if(buffer.at(0)!=receiver && buffer.at(1)!=sender && answerStr!="OK") {
        QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Включить индикацию Завершено\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');

        return false;
    }

 //   portStend->close();

   return true;
}

bool ObjectThread::finishIndicatorOff()
{
   //Отключить индикатор ЗАВЕРШЕНО	FIN=0	OK

    QByteArray buffer;

//    кто и кому:
//    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    кто	    кому	команда					                    Конец сообщения
//    BIN	    BIN	    STRING					                    0x00

    if(!isWorkPlaceUse.at(workPlace)) return false;

    QString command = "FIN=0";

    QByteArray parcel;

    quint8 sender = 0x01;
    quint8 receiver;
    quint8 stopByte = 0x00;

    if(workPlace == 0) receiver = 0x11;
    if(workPlace == 1) receiver = 0x22;
    if(workPlace == 2) receiver = 0x33;
    if(workPlace == 3) receiver = 0x44;

    QByteArray stringToByteArray;
    stringToByteArray = command.toLocal8Bit();

    //формирование посылки
    parcel.append(sender);
    parcel.append(receiver);
    for(int m=0; m<stringToByteArray.size(); m++) {
        parcel.append(stringToByteArray.at(m));
    }
    parcel.append(stopByte);

    qDebug()<<"parcel"<<parcel.toHex();

    if(!portStend->isOpen()) {

        if(!portStend->open(QIODevice::ReadWrite)) {

            QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
                                         ". Рабочее место: " + QString::number(workPlace+1));
            emit errorStringSignal(label_StatusBar + '\n');

            return false;
        }
    }

    portStend->clear();

    quint64 count = portStend->write(parcel);
    emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

    //ждать ответа от стенда 10 мсек

    global::pause(100);

    buffer = portStend->readAll();
 //   portStend->close();

    if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

    if(buffer.isEmpty()) {
        QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Выключить индикацию Завершено\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');

        return false;
    }

    QString answerStr;
    QByteArray bufTmp = buffer;

    bufTmp.remove(0, 2);
    bufTmp.remove(bufTmp.size()-1, 1);

    answerStr = QString::fromLocal8Bit(bufTmp);

    if(buffer.at(0)!=receiver && buffer.at(1)!=sender && answerStr!="OK") {
        QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Выключить индикацию Завершено\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');

        return false;
    }

 //   portStend->close();

   return true;

}

bool ObjectThread::errorIndicatorOn()
{
   //Включить индикатор АВАРИЯ	ALR=1	OK

    QByteArray buffer;

//    кто и кому:
//    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    кто	    кому	команда					                    Конец сообщения
//    BIN	    BIN	    STRING					                    0x00

    if(!isWorkPlaceUse.at(workPlace)) return false;

    QString command = "ALR=1";

    QByteArray parcel;

    quint8 sender = 0x01;
    quint8 receiver;
    quint8 stopByte = 0x00;

    if(workPlace == 0) receiver = 0x11;
    if(workPlace == 1) receiver = 0x22;
    if(workPlace == 2) receiver = 0x33;
    if(workPlace == 3) receiver = 0x44;

    QByteArray stringToByteArray;
    stringToByteArray = command.toLocal8Bit();

    //формирование посылки
    parcel.append(sender);
    parcel.append(receiver);
    for(int m=0; m<stringToByteArray.size(); m++) {
        parcel.append(stringToByteArray.at(m));
    }
    parcel.append(stopByte);

    qDebug()<<"parcel"<<parcel.toHex();

    if(!portStend->isOpen()) {

        if(!portStend->open(QIODevice::ReadWrite)) {

            QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
                                         ". Рабочее место: " + QString::number(workPlace+1));
            emit errorStringSignal(label_StatusBar + '\n');
//            vectorIndicatorBSLMatrix[workPlace] = true;

//            emit workPlaceOff(workPlace);
//            emit checkRashodomerError(workPlace);
            return false;
        }
    }

    portStend->clear();

    quint64 count = portStend->write(parcel);
    emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

    //ждать ответа от стенда 10 мсек

    global::pause(100);

    buffer = portStend->readAll();
 //   portStend->close();

    if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

    if(buffer.isEmpty()) {
        QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Включить индикатор АВАРИЯ\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
//        vectorIndicatorBSLMatrix[workPlace] = true;
//        emit workPlaceOff(workPlace);
//        emit checkRashodomerError(workPlace);

  //      portStend->close();
        return false;
    }

    QString answerStr;
    QByteArray bufTmp = buffer;

    bufTmp.remove(0, 2);
    bufTmp.remove(bufTmp.size()-1, 1);

    answerStr = QString::fromLocal8Bit(bufTmp);

    if(buffer.at(0)!=receiver && buffer.at(1)!=sender && answerStr!="OK") {
        QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Включить индикатор АВАРИЯ\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
//        vectorIndicatorBSLMatrix[workPlace] = true;
//        emit workPlaceOff(workPlace);
//        emit checkRashodomerError(workPlace);
 //       portStend->close();
        return false;
    }

 //  portStend->close();

   return true;
}

bool ObjectThread::errorIndicatorOff()
{
   //Отключить индикатор АВАРИЯ	ALR=0	OK

    QByteArray buffer;

//    кто и кому:
//    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    кто	    кому	команда					                    Конец сообщения
//    BIN	    BIN	    STRING					                    0x00

    if(!isWorkPlaceUse.at(workPlace)) return false;

    QString command = "ALR=0";

    QByteArray parcel;

    quint8 sender = 0x01;
    quint8 receiver;
    quint8 stopByte = 0x00;

    if(workPlace == 0) receiver = 0x11;
    if(workPlace == 1) receiver = 0x22;
    if(workPlace == 2) receiver = 0x33;
    if(workPlace == 3) receiver = 0x44;

    QByteArray stringToByteArray;
    stringToByteArray = command.toLocal8Bit();

    //формирование посылки
    parcel.append(sender);
    parcel.append(receiver);
    for(int m=0; m<stringToByteArray.size(); m++) {
        parcel.append(stringToByteArray.at(m));
    }
    parcel.append(stopByte);

    qDebug()<<"parcel"<<parcel.toHex();

    if(!portStend->isOpen()) {

        if(!portStend->open(QIODevice::ReadWrite)) {

            QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
                                         ". Рабочее место: " + QString::number(workPlace+1));
            emit errorStringSignal(label_StatusBar + '\n');
//            vectorIndicatorBSLMatrix[workPlace] = true;

//            emit workPlaceOff(workPlace);
//            emit checkRashodomerError(workPlace);
            return false;
        }
    }

    portStend->clear();

    quint64 count = portStend->write(parcel);
    emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

    //ждать ответа от стенда 10 мсек

    global::pause(100);

    buffer = portStend->readAll();
 //   portStend->close();

    if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

    if(buffer.isEmpty()) {
        QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Отключить индикатор АВАРИЯ\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
//        vectorIndicatorBSLMatrix[workPlace] = true;
//        emit workPlaceOff(workPlace);
//        emit checkRashodomerError(workPlace);

  //      portStend->close();
        return false;
    }

    QString answerStr;
    QByteArray bufTmp = buffer;

    bufTmp.remove(0, 2);
    bufTmp.remove(bufTmp.size()-1, 1);

    answerStr = QString::fromLocal8Bit(bufTmp);

    if(buffer.at(0)!=receiver && buffer.at(1)!=sender && answerStr!="OK") {
        QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Отключить индикатор АВАРИЯ\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
//        vectorIndicatorBSLMatrix[workPlace] = true;
//        emit workPlaceOff(workPlace);
//        emit checkRashodomerError(workPlace);
 //       portStend->close();
        return false;
    }

 //  portStend->close();

   return true;
}

//------------------Вход датчика вращения--------------------------

bool ObjectThread::setFreqRotation(QString freq)
{
    //Установить частоту имитации вращения (в Гц) FIM=xxx	OK

    QByteArray buffer;

//    кто и кому:
//    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    кто	    кому	команда					                    Конец сообщения
//    BIN	    BIN	    STRING					                    0x00

    if(!isWorkPlaceUse.at(workPlace)) return false;

    QString command = "FIM=" + freq;

    QByteArray parcel;

    quint8 sender = 0x01;
    quint8 receiver;
    quint8 stopByte = 0x00;

    if(workPlace == 0) receiver = 0x11;
    if(workPlace == 1) receiver = 0x22;
    if(workPlace == 2) receiver = 0x33;
    if(workPlace == 3) receiver = 0x44;

    QByteArray stringToByteArray;
    stringToByteArray = command.toLocal8Bit();

    //формирование посылки
    parcel.append(sender);
    parcel.append(receiver);
    for(int m=0; m<stringToByteArray.size(); m++) {
        parcel.append(stringToByteArray.at(m));
    }
    parcel.append(stopByte);

    qDebug()<<"parcel"<<parcel.toHex();

    if(!portStend->isOpen()) {

        if(!portStend->open(QIODevice::ReadWrite)) {

            QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
                                         ". Рабочее место: " + QString::number(workPlace+1));
            emit errorStringSignal(label_StatusBar + '\n');
            vectorIndicatorBSLMatrix[workPlace] = true;

            emit workPlaceOff(workPlace);
            emit checkRashodomerError(workPlace);
            return false;
        }
    }

    portStend->clear();

    quint64 count = portStend->write(parcel);
    emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

    //ждать ответа от стенда 10 мсек

    global::pause(100);

    buffer = portStend->readAll();
  //  portStend->close();

    if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

    if(buffer.isEmpty()) {
        QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Установить частоту имитации вращения\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
        vectorIndicatorBSLMatrix[workPlace] = true;
        emit workPlaceOff(workPlace);
        emit checkRashodomerError(workPlace);

  //      portStend->close();
        return false;
    }

    QString answerStr;
    QByteArray bufTmp = buffer;

    bufTmp.remove(0, 2);
    bufTmp.remove(bufTmp.size()-1, 1);

    answerStr = QString::fromLocal8Bit(bufTmp);

    if(buffer.at(0)!=receiver && buffer.at(1)!=sender && answerStr!="OK") {
        QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Установить частоту имитации вращения\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
        vectorIndicatorBSLMatrix[workPlace] = true;
        emit workPlaceOff(workPlace);
        emit checkRashodomerError(workPlace);
 //       portStend->close();
        return false;
    }

 //  portStend->close();

   return true;
}

bool ObjectThread::imitRotationOn()
{
    //Включить  имитацию вращения IM=1	OK

    QByteArray buffer;

//    кто и кому:
//    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    кто	    кому	команда					                    Конец сообщения
//    BIN	    BIN	    STRING					                    0x00

    if(!isWorkPlaceUse.at(workPlace)) return false;

    QString command = "IM=1";

    QByteArray parcel;

    quint8 sender = 0x01;
    quint8 receiver;
    quint8 stopByte = 0x00;

    if(workPlace == 0) receiver = 0x11;
    if(workPlace == 1) receiver = 0x22;
    if(workPlace == 2) receiver = 0x33;
    if(workPlace == 3) receiver = 0x44;

    QByteArray stringToByteArray;
    stringToByteArray = command.toLocal8Bit();

    //формирование посылки
    parcel.append(sender);
    parcel.append(receiver);
    for(int m=0; m<stringToByteArray.size(); m++) {
        parcel.append(stringToByteArray.at(m));
    }
    parcel.append(stopByte);

    qDebug()<<"parcel"<<parcel.toHex();

    if(!portStend->isOpen()) {

        if(!portStend->open(QIODevice::ReadWrite)) {

            QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
                                         ". Рабочее место: " + QString::number(workPlace+1));
            emit errorStringSignal(label_StatusBar + '\n');
            vectorIndicatorBSLMatrix[workPlace] = true;

            emit workPlaceOff(workPlace);
            emit checkRashodomerError(workPlace);
            return false;
        }
    }

    portStend->clear();

    quint64 count = portStend->write(parcel);
    emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

    //ждать ответа от стенда 10 мсек

    global::pause(100);

    buffer = portStend->readAll();
 //   portStend->close();

    if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

    if(buffer.isEmpty()) {
        QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Включить  имитацию вращения\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
        vectorIndicatorBSLMatrix[workPlace] = true;
        emit workPlaceOff(workPlace);
        emit checkRashodomerError(workPlace);

  //      portStend->close();
        return false;
    }

    QString answerStr;
    QByteArray bufTmp = buffer;

    bufTmp.remove(0, 2);
    bufTmp.remove(bufTmp.size()-1, 1);

    answerStr = QString::fromLocal8Bit(bufTmp);

    if(buffer.at(0)!=receiver && buffer.at(1)!=sender && answerStr!="OK") {
        QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Включить  имитацию вращения\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
        vectorIndicatorBSLMatrix[workPlace] = true;
        emit workPlaceOff(workPlace);
        emit checkRashodomerError(workPlace);
 //       portStend->close();
        return false;
    }

 //  portStend->close();

   return true;
}

bool ObjectThread::imitRotationOff()
{
    //Выключить имитацию вращения IM=0	OK

    QByteArray buffer;

//    кто и кому:
//    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    кто	    кому	команда					                    Конец сообщения
//    BIN	    BIN	    STRING					                    0x00

    if(!isWorkPlaceUse.at(workPlace)) return false;

    QString command = "IM=0";

    QByteArray parcel;

    quint8 sender = 0x01;
    quint8 receiver;
    quint8 stopByte = 0x00;

    if(workPlace == 0) receiver = 0x11;
    if(workPlace == 1) receiver = 0x22;
    if(workPlace == 2) receiver = 0x33;
    if(workPlace == 3) receiver = 0x44;

    QByteArray stringToByteArray;
    stringToByteArray = command.toLocal8Bit();

    //формирование посылки
    parcel.append(sender);
    parcel.append(receiver);
    for(int m=0; m<stringToByteArray.size(); m++) {
        parcel.append(stringToByteArray.at(m));
    }
    parcel.append(stopByte);

    qDebug()<<"parcel"<<parcel.toHex();

    if(!portStend->isOpen()) {

        if(!portStend->open(QIODevice::ReadWrite)) {

            QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
                                         ". Рабочее место: " + QString::number(workPlace+1));
            emit errorStringSignal(label_StatusBar + '\n');
            vectorIndicatorBSLMatrix[workPlace] = true;

            emit workPlaceOff(workPlace);
            emit checkRashodomerError(workPlace);
            return false;
        }
    }

    portStend->clear();

    quint64 count = portStend->write(parcel);
    emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

    //ждать ответа от стенда 10 мсек

    global::pause(100);

    buffer = portStend->readAll();
 //   portStend->close();

    if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

    if(buffer.isEmpty()) {
        QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Выключить имитацию вращения\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
        vectorIndicatorBSLMatrix[workPlace] = true;
        emit workPlaceOff(workPlace);
        emit checkRashodomerError(workPlace);

  //      portStend->close();
        return false;
    }

    QString answerStr;
    QByteArray bufTmp = buffer;

    bufTmp.remove(0, 2);
    bufTmp.remove(bufTmp.size()-1, 1);

    answerStr = QString::fromLocal8Bit(bufTmp);

    if(buffer.at(0)!=receiver && buffer.at(1)!=sender && answerStr!="OK") {
        QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Выключить имитацию вращения\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
        vectorIndicatorBSLMatrix[workPlace] = true;
        emit workPlaceOff(workPlace);
        emit checkRashodomerError(workPlace);
 //       portStend->close();
        return false;
    }

 //  portStend->close();

   return true;
}

bool ObjectThread::readTimeoutMagnSensor()
{
  //  Прочитать таймаут импульса питания на резистивный датчик	TIM?	TIM=xxxx

}

bool ObjectThread::selectMbus()
{
    //    Выбрать MBUS  EXT=MBUS  OK

    QByteArray buffer;

//    кто и кому:
//    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    кто	    кому	команда					                    Конец сообщения
//    BIN	    BIN	    STRING					                    0x00

    if(!isWorkPlaceUse.at(workPlace)) return false;

    QString command = "EXT=MBUS";

    QByteArray parcel;

    quint8 sender = 0x01;
    quint8 receiver;
    quint8 stopByte = 0x00;

    if(workPlace == 0) receiver = 0x11;
    if(workPlace == 1) receiver = 0x22;
    if(workPlace == 2) receiver = 0x33;
    if(workPlace == 3) receiver = 0x44;

    QByteArray stringToByteArray;
    stringToByteArray = command.toLocal8Bit();

    //формирование посылки
    parcel.append(sender);
    parcel.append(receiver);
    for(int m=0; m<stringToByteArray.size(); m++) {
        parcel.append(stringToByteArray.at(m));
    }
    parcel.append(stopByte);

    qDebug()<<"parcel"<<parcel.toHex();

    if(!portStend->isOpen()) {

        if(!portStend->open(QIODevice::ReadWrite)) {

            QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
                                         ". Рабочее место: " + QString::number(workPlace+1));
            emit errorStringSignal(label_StatusBar + '\n');
//            vectorIndicatorBSLMatrix[workPlace] = true;

//            emit workPlaceOff(workPlace);
//            emit checkRashodomerError(workPlace);
            return false;
        }
    }

    portStend->clear();

    quint64 count = portStend->write(parcel);
    emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

    //ждать ответа от стенда 10 мсек

    global::pause(100);

    buffer = portStend->readAll();
 //   portStend->close();

    if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

    if(buffer.isEmpty()) {
        QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Выбрать MBUS\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
//        vectorIndicatorBSLMatrix[workPlace] = true;
//        emit workPlaceOff(workPlace);
//        emit checkRashodomerError(workPlace);

  //      portStend->close();
        return false;
    }

    QString answerStr;
    QByteArray bufTmp = buffer;

    bufTmp.remove(0, 2);
    bufTmp.remove(bufTmp.size()-1, 1);

    answerStr = QString::fromLocal8Bit(bufTmp);

    if(buffer.at(0)!=receiver && buffer.at(1)!=sender && answerStr!="OK") {
        QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Выбрать MBUS\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
//        vectorIndicatorBSLMatrix[workPlace] = true;
//        emit workPlaceOff(workPlace);
//        emit checkRashodomerError(workPlace);
 //       portStend->close();
        return false;
    }

 //  portStend->close();

   return true;
}

bool ObjectThread::selectRS()
{
    //    Выбрать RS485  EXT=485  OK

    QByteArray buffer;

//    кто и кому:
//    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    кто	    кому	команда					                    Конец сообщения
//    BIN	    BIN	    STRING					                    0x00

    if(!isWorkPlaceUse.at(workPlace)) return false;

    QString command = "EXT=485";

    QByteArray parcel;

    quint8 sender = 0x01;
    quint8 receiver;
    quint8 stopByte = 0x00;

    if(workPlace == 0) receiver = 0x11;
    if(workPlace == 1) receiver = 0x22;
    if(workPlace == 2) receiver = 0x33;
    if(workPlace == 3) receiver = 0x44;

    QByteArray stringToByteArray;
    stringToByteArray = command.toLocal8Bit();

    //формирование посылки
    parcel.append(sender);
    parcel.append(receiver);
    for(int m=0; m<stringToByteArray.size(); m++) {
        parcel.append(stringToByteArray.at(m));
    }
    parcel.append(stopByte);

    qDebug()<<"parcel"<<parcel.toHex();

    if(!portStend->isOpen()) {

        if(!portStend->open(QIODevice::ReadWrite)) {

            QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
                                         ". Рабочее место: " + QString::number(workPlace+1));
            emit errorStringSignal(label_StatusBar + '\n');
//            vectorIndicatorBSLMatrix[workPlace] = true;

//            emit workPlaceOff(workPlace);
//            emit checkRashodomerError(workPlace);
            return false;
        }
    }

    portStend->clear();

    quint64 count = portStend->write(parcel);
    emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

    //ждать ответа от стенда 10 мсек

    global::pause(100);

    buffer = portStend->readAll();
 //   portStend->close();

    if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

    if(buffer.isEmpty()) {
        QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Выбрать RS485\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
//        vectorIndicatorBSLMatrix[workPlace] = true;
//        emit workPlaceOff(workPlace);
//        emit checkRashodomerError(workPlace);

  //      portStend->close();
        return false;
    }

    QString answerStr;
    QByteArray bufTmp = buffer;

    bufTmp.remove(0, 2);
    bufTmp.remove(bufTmp.size()-1, 1);

    answerStr = QString::fromLocal8Bit(bufTmp);

    if(buffer.at(0)!=receiver && buffer.at(1)!=sender && answerStr!="OK") {
        QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Выбрать RS485\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
//        vectorIndicatorBSLMatrix[workPlace] = true;
//        emit workPlaceOff(workPlace);
//        emit checkRashodomerError(workPlace);
 //       portStend->close();
        return false;
    }

 //  portStend->close();

   return true;
}

//--------------------Служебные------------------------------------

bool ObjectThread::testCommand()
{
   //Тестовая команда	STENDx	OK
}

void ObjectThread::getMainWndPointer(QMainWindow * mainwnd)
{
  // mainWnd = mainwnd;
}

void ObjectThread::slotRealClockCalibration(QSerialPort *port1, QSerialPort *port2,
                              QSerialPort *port3, QSerialPort *port4
                              /*float period1, float period2, float period3, float period4*/)
{

    if(!isWorkPlaceUse.at(workPlace)) return;

    //сначала прочитать значения периодов из каждого стенда

    int currentIndicatorNumber = workPlace;
    int currentBoxNumber = 5;//вместо "Включить протокол Mbus"

    if(!readFreq()) {
        vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
        emit workPlaceOff(currentIndicatorNumber);
        emit checkTimeCalError(currentIndicatorNumber);

        return;
    }

    //сначала прочитать значения периодов из каждого стенда/

    QSerialPort *portTmp;// = new QSerialPort(this);

    int workPlaceNumber = workPlace;    

    QString label_StatusBar;
    QString errorString;    

    switch (workPlaceNumber) {
    case 0:
        portTmp = port1;
        break;
    case 1:
        portTmp = port2;
        break;
    case 2:
        portTmp = port3;
        break;
    case 3:
        portTmp = port4;
        break;

    default:
        break;
    }

    portTmp->setBaudRate(QSerialPort::Baud19200);
    portTmp->setDataBits(QSerialPort::Data8);
    portTmp->setParity(QSerialPort::NoParity);
    portTmp->setStopBits(QSerialPort::OneStop);

    //    Значение коэффициента коррекции должно рассчитываться по формуле:
    //    K = |Round(983040 * (1 - 1953,125 / Tи))|,
    //    где Tи – измеренное значение периода в мс.

        float periodBetweenPulses;

        if(workPlace == 0) periodBetweenPulses = periodBetweenPulses1;
        if(workPlace == 1) periodBetweenPulses = periodBetweenPulses2;
        if(workPlace == 2) periodBetweenPulses = periodBetweenPulses3;
        if(workPlace == 3) periodBetweenPulses = periodBetweenPulses4;

        float periodError = (1953.125/10000)*3;

        periodBetweenPulses = periodClock;//1953.125;//заглушка. должно стоять periodClock, выраженный в мс

        if( !((periodBetweenPulses >= 1953.125 - periodError) && (periodBetweenPulses <= 1953.125 + periodError)) ) {
  //          QMessageBox::information(this, "" , tr("Недопустимое значение периода следования импульсов "));
            label_StatusBar =("Недопустимое значение периода следования импульсов. Рабочее место: " + QString::number(workPlaceNumber + 1));
            emit errorStringSignal(label_StatusBar + '\n');
            vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
            emit workPlaceOff(currentIndicatorNumber);
            emit checkTimeCalError(currentIndicatorNumber);

            return;
        }

        quint16 correctionCoeff = (quint16)round(abs(983040*(1 - 1953.125/periodBetweenPulses)));
        qDebug()<<"correctionCoeff "<<correctionCoeff;

    //    correctionCoeff = 240;//заглушка

        if(correctionCoeff > 240) {
  //          QMessageBox::information(this, "", tr("Недопустимое значение коэффициента коррекции ") + QString::number(correctionCoeff));

            label_StatusBar =("Недопустимое значение коэффициента коррекции " + QString::number(correctionCoeff) +
                                        " Рабочее место: " + QString::number(workPlaceNumber + 1));
            emit errorStringSignal(label_StatusBar + '\n');
            vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
            emit workPlaceOff(currentIndicatorNumber);
            emit checkTimeCalError(currentIndicatorNumber);

            return;
        }

        // установка/сброс старшего бита
        quint16 correctionCoeffResult = correctionCoeff;

        if(periodBetweenPulses>1953.125) {
            quint16 bitField = (1<<15);
            correctionCoeffResult = correctionCoeff | bitField;
            qDebug()<<"correctionCoeffResult case1 "<<correctionCoeffResult;
        }
        if(periodBetweenPulses<=1953.125) {
            quint16 bitField = ~(1<<15);
            correctionCoeffResult = correctionCoeff & bitField;
            qDebug()<<"correctionCoeffResult case2 "<<correctionCoeffResult;

        }


      //расчета коэффициента коррекции и записи его в энергонезависимую память
      //командой WCC, параметр RTC_Offset (значение параметра RTC_Tcomp всегда 0)

//        ui->label_StatusBar->setText("");

//        QColor color(255, 127, 50);
//        ui->label_realClockCalibration->setStyleSheet(QString("color: red").arg(color.name()));
//        ui->label_realClockCalibration->setText("X");
//        ui->label_realClockCalibration->setVisible(false);

        QByteArray buffer;

        if(!portTmp->isOpen()) {
            if(!portTmp->open(QIODevice::ReadWrite)) {
  //              QMessageBox::information(this, "", "Не удалось открыть порт УСО-2");
                label_StatusBar =("Не удалось открыть порт УСО-2. Рабочее место: "
                                             + QString::number(workPlaceNumber + 1));
                emit errorStringSignal(label_StatusBar + '\n');
                vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                emit workPlaceOff(currentIndicatorNumber);
                emit checkTimeCalError(currentIndicatorNumber);

                return;
            }
        }

        //
        //WCC  5a 00 09 26

      bool isWritingCorrect = false;

      for(int j=0; j<3; j++) {


        QByteArray buffer;

        for(int i=0; i<10;i++) {

           portTmp->clear();
           packetToRead.clear();
           buffer.clear();
           quint8 byte = 0x5a;
           packetToRead.append(byte);
           byte = 0x00;
           packetToRead.append(byte);
           byte = 0x09;
           packetToRead.append(byte);
           byte = 0x26;
           packetToRead.append(byte);

           byte = (quint8)(correctionCoeffResult>>8);//старший байт
           packetToRead.append(byte);
           byte = (quint8)correctionCoeffResult;//младший байт
           packetToRead.append(byte);

           byte = 0;
           packetToRead.append(byte);
           byte = 0;
           packetToRead.append(byte);

           quint8 crc = makeCRC(packetToRead);
           packetToRead.append(crc);

           quint64 cnt = portTmp->write(packetToRead);
           emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

           qDebug()<<"cnt "<<cnt
                   <<"packetToRead.toHex() "<<packetToRead.toHex()
                   <<"portTmp->portName() "<<portTmp->portName();

           if(cnt == 0) {
               //QMessageBox::information(this, "", tr("Данные в порт не записаны"));
               label_StatusBar =("Данные в порт не записаны. Рабочее место: "
                                            + QString::number(workPlaceNumber + 1));
               emit errorStringSignal(label_StatusBar + '\n');
               vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

               emit workPlaceOff(currentIndicatorNumber);
               emit checkTimeCalError(currentIndicatorNumber);

               return;
           }

           global::pause(300);

           buffer = portTmp->readAll();
           qDebug()<<"buffer.toHex()"<<buffer.toHex();
           if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

  //         ui->textBrowser->append("buffer.toHex() " + buffer.toHex());

           if(buffer.isEmpty()) {
               if(i==9) {
//                   QMessageBox::information(this, "", tr("Ошибка калибровки генератора часов реального времени"));
//                   ui->label_realClockCalibration->setVisible(true);
                   label_StatusBar =("Ошибка калибровки часов. Рабочее место: "
                                                + QString::number(workPlaceNumber + 1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkTimeCalError(currentIndicatorNumber);

                   return;
               } else{}

           }
           else {
               //проверяем crc и первые четыре байта ответного пакета
               QByteArray bytesForChecking;
               quint8 byte = 0x5a;
               bytesForChecking.append(byte);
               byte = 0x00;
               bytesForChecking.append(byte);
               byte = 0x06;
               bytesForChecking.append(byte);
               byte = 0xa6;
               bytesForChecking.append(byte);

               if(!packetSETProcessing(buffer)) {
                   if(i == 9) {
                       label_StatusBar = (tr("Ошибка данных") +
                                                    " Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkTimeCalError(currentIndicatorNumber);
                       return;
                   }
                   else {
                       continue;
                   }
               }

               quint8 error = buffer.at(4);

               if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1]
                   && bytesForChecking[3] == buffer[3] && error == 0) {

                  qDebug()<<"buffer.toHex() "<<buffer.toHex()
                       <<"checkCRC(buffer) "<<checkCRC(buffer);






                  break;
               }
               else {
                   if(i==9) {
//                   QMessageBox::information(this, "", tr("Ошибка калибровки генератора часов реального времени"));
//                   ui->label_realClockCalibration->setVisible(true);
                     label_StatusBar = (tr("Ошибка калибровки часов") +
                                                " Рабочее место: " + QString::number(workPlaceNumber+1));
                      emit errorStringSignal(label_StatusBar + '\n');
                      vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                      emit workPlaceOff(currentIndicatorNumber);
                      emit checkTimeCalError(currentIndicatorNumber);

                      return;
                   }
               }



           }


        }



        //
        //RCC  5a 00 05 06

        for(int i=0; i<3;i++) {

           portTmp->clear();
           packetToRead.clear();
           buffer.clear();
           quint8 byte = 0x5a;
           packetToRead.append(byte);
           byte = 0x00;
           packetToRead.append(byte);
           byte = 0x05;
           packetToRead.append(byte);
           byte = 0x06;
           packetToRead.append(byte);
           quint8 crc = makeCRC(packetToRead);
           packetToRead.append(crc);

           quint64 cnt = portTmp->write(packetToRead);
           emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

           qDebug()<<"cnt "<<cnt
                   <<"packetToRead.toHex() "<<packetToRead.toHex()
                   <<"portTmp->portName() "<<portTmp->portName();

           if(cnt == 0) {
               //QMessageBox::information(this, "", tr("Данные в порт не записаны"));
               label_StatusBar =("Данные в порт не записаны. Рабочее место: "
                                            + QString::number(workPlaceNumber + 1));
               emit errorStringSignal(label_StatusBar + '\n');
               vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

               emit workPlaceOff(currentIndicatorNumber);
               emit checkTimeCalError(currentIndicatorNumber);

               return;

           }

           global::pause(200);

           buffer = portTmp->readAll();
           qDebug()<<"buffer.toHex()"<<buffer.toHex();
           if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

           if(buffer.isEmpty()) {
               if(i==2) {
//                   QMessageBox::information(this, "", tr("Ошибка калибровки генератора часов реального времени"));
//                   ui->label_realClockCalibration->setVisible(true);
                   label_StatusBar =("Ошибка калибровки часов. Рабочее место: "
                                                + QString::number(workPlaceNumber + 1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkTimeCalError(currentIndicatorNumber);

                   return;
               } else{}

           }
           else {
               //проверяем crc и первые четыре байта ответного пакета
               QByteArray bytesForChecking;
               quint8 byte = 0x5a;
               bytesForChecking.append(byte);
               byte = 0x00;
               bytesForChecking.append(byte);
               byte = 0x23;
               bytesForChecking.append(byte);
               byte = 0x86;
               bytesForChecking.append(byte);

               if(!packetSETProcessing(buffer)) {
                   if(i == 2) {
                       label_StatusBar = (tr("Ошибка данных") +
                                                    " Рабочее место: " + QString::number(workPlaceNumber+1));
                       emit errorStringSignal(label_StatusBar + '\n');
                       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                       emit workPlaceOff(currentIndicatorNumber);
                       emit checkTimeCalError(currentIndicatorNumber);
                       return;
                   }
                   else {
                       continue;
                   }
               }

               if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] &&
                       bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3]) {

                  qDebug()<<"buffer.toHex() "<<buffer.toHex()
                       <<"checkCRC(buffer) "<<checkCRC(buffer);


                 quint16  RTC_Offset = (quint8)buffer[4];
                 RTC_Offset = RTC_Offset<<8 | (quint8)buffer[5];

                 qDebug()<<"RTC_Offset "<<QString::number(RTC_Offset);

                 quint16 RTC_Tcomp = (quint8)buffer[6];
                 RTC_Tcomp = RTC_Tcomp<<8 | (quint8)buffer[7];

                 qDebug()<<""<<QString::number(RTC_Tcomp);

                 if(RTC_Offset == correctionCoeffResult && RTC_Tcomp == 0) {
                     j = 3; // прекращаем самый внешний цикл 3-х кратной записи

                     qDebug()<<"Коэффициенты коррекции записаны успешно";

                     isWritingCorrect = true;

                     rtc_OffsetResult = QString::number(RTC_Offset);
                     rtc_TcompResult = QString::number(RTC_Tcomp);
                 }






                  break;
               }
               else {
                   if(i==2) {
//                   QMessageBox::information(this, "", tr("Ошибка калибровки генератора часов реального времени"));
//                   ui->label_realClockCalibration->setVisible(true);
                       label_StatusBar = (tr("Ошибка калибровки часов") +
                                                  " Рабочее место: " + QString::number(workPlaceNumber+1));
                        emit errorStringSignal(label_StatusBar + '\n');
                        vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                        emit workPlaceOff(currentIndicatorNumber);
                        emit checkTimeCalError(currentIndicatorNumber);

                       return;
                   }
               }



           }


        }


      }


      if(isWritingCorrect == false) { // проверка корректности записи калибровочных коээффициентов
//          QMessageBox::information(this, "", "Не удалось корректно записать калибровочные коэффициенты");
//          ui->label_realClockCalibration->setVisible(true);
          label_StatusBar = (tr("Не удалось корректно записать калибровочные коэффициенты.") +
                                     " Рабочее место: " + QString::number(workPlaceNumber+1));
           emit errorStringSignal(label_StatusBar + '\n');
           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
           emit workPlaceOff(currentIndicatorNumber);
           emit checkTimeCalError(currentIndicatorNumber);

           return;
      }
      else {
//          ui->label_StatusBar->setText("Калибровка генератора часов: успешно");
//          ui->label_realClockCalibration->setStyleSheet(QString("color: green").arg(color.name()));
//          ui->label_realClockCalibration->setText("V");
//          ui->label_realClockCalibration->setVisible(true);
      }

      emit checkTimeCalError(currentIndicatorNumber);


}


void ObjectThread::slotRashodomer(QSerialPort *port1, QSerialPort *port2,
                                  QSerialPort *port3, QSerialPort *port4)
{

    QByteArray buffer;

    QSerialPort *portTmp;

    bool isNeedPaintEvent = true;//false;

    int workPlaceNumber = workPlace;

    if(!isWorkPlaceUse.at(workPlace)) return;

    int currentIndicatorNumber = workPlaceNumber;

    QString label_StatusBar;
    QString errorString;

    switch (workPlaceNumber) {
    case 0:
        portTmp = port1;
        break;
    case 1:
        portTmp = port2;
        break;
    case 2:
        portTmp = port3;
        break;
    case 3:
        portTmp = port4;
        break;

    default:
        break;
    }

    portTmp->setBaudRate(QSerialPort::Baud19200);
    portTmp->setDataBits(QSerialPort::Data8);
    portTmp->setParity(QSerialPort::NoParity);
    portTmp->setStopBits(QSerialPort::OneStop);


    int currentBoxNumber = 6;

//    1)	Установить частоту (FIM=xxx)
//    2)	Включить имитацию вращения (IM=1)
//    3)	Прочитать из Гефеста значение потока
//    4)	Выключить имитацию вращения (IM=0)


    if(!setFreqRotation("100")) {
        label_StatusBar =("Не удалось установить частоту вращения. Рабочее место: "
                                     + QString::number(workPlaceNumber + 1));
        emit errorStringSignal(label_StatusBar + '\n');
        vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

        emit workPlaceOff(currentIndicatorNumber);
        emit checkRashodomerError(currentIndicatorNumber);

        return;
    }

    if(!imitRotationOn()) {
        label_StatusBar =("Не удалось включить имитацию вращения. Рабочее место: "
                                     + QString::number(workPlaceNumber + 1));
        emit errorStringSignal(label_StatusBar + '\n');
        vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

        emit workPlaceOff(currentIndicatorNumber);
        emit checkRashodomerError(currentIndicatorNumber);

       return;
    }

    global::pause(5000);

    if(!portTmp->isOpen()) {
        if(!portTmp->open(QIODevice::ReadWrite)) {
//              QMessageBox::information(this, "", "Не удалось открыть порт УСО-2");
            label_StatusBar =("Не удалось открыть порт УСО-2. Рабочее место: "
                                         + QString::number(workPlaceNumber + 1));
            emit errorStringSignal(label_StatusBar + '\n');
            vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

            imitRotationOff();
            emit workPlaceOff(currentIndicatorNumber);
            emit checkRashodomerError(currentIndicatorNumber);

            return;
        }
    }

//прочитать значение потока

    //
    //Открыть парольную сессию 5a 00 08 40 01 04 57
//    for(int i=0; i<3;i++) {

//       portTmp->clear();
//       packetToRead.clear();
//       buffer.clear();
//       quint8 byte = 0x5a;
//       packetToRead.append(byte);
//       byte = 0x00;
//       packetToRead.append(byte);
//       byte = 0x08;
//       packetToRead.append(byte);
//       byte = 0x40;
//       packetToRead.append(byte);
//       byte = 0x01;
//       packetToRead.append(byte);
//       byte = 0x04;
//       packetToRead.append(byte);
//       byte = 0x57;
//       packetToRead.append(byte);
//       quint8 crc = makeCRC(packetToRead);
//       packetToRead.append(crc);

//       quint64 cnt = portTmp->write(packetToRead);
//       emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

//       qDebug()<<"cnt "<<cnt
//               <<"packetToRead.toHex() "<<packetToRead.toHex()
//               <<"portTmp->portName() "<<portTmp->portName();

//       if(cnt == 0) {
// //               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
// //                                                 "Рабочее место: " + QString::number(workPlaceNumber+1));
//           label_StatusBar = (tr("Данные в порт не записаны") +
//                                        ". Рабочее место: " + QString::number(workPlaceNumber+1));
//           emit errorStringSignal(label_StatusBar + '\n');
//           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
// //                   checkWritingError(currentIndicatorNumber);
// //                   emit workPlaceOff(currentIndicatorNumber);
//           emit workPlaceOff(currentIndicatorNumber);
//           emit checkRashodomerError(currentIndicatorNumber);

//           return;
//       }

//       QApplication::processEvents();
//       global::pause(150);//(300);

//       buffer = portTmp->readAll();
//       qDebug()<<"buffer.toHex()"<<buffer.toHex();
//       if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

//       if(buffer.isEmpty()) {
//           if(i==2) {
// //                   QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") +
// //                                            "Рабочее место: " + QString::number(workPlaceNumber+1));
//               label_StatusBar = (tr("Ошибка открытия парольной сессии") +
//                                            ". Рабочее место: " + QString::number(workPlaceNumber+1));
//               emit errorStringSignal(label_StatusBar + '\n');
// //                      ui->label_writeParams->setVisible(true);
//               vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
// //                       checkWritingError(currentIndicatorNumber);
// //                       emit workPlaceOff(currentIndicatorNumber);
//               emit workPlaceOff(currentIndicatorNumber);
//               emit checkRashodomerError(currentIndicatorNumber);

//               return;
//           } else{}

//       }
//       else {
//           //проверяем crc и первые четыре байта ответного пакета
//           QByteArray bytesForChecking;
//           quint8 byte = 0x5a;
//           bytesForChecking.append(byte);
//           byte = 0x00;
//           bytesForChecking.append(byte);
//           byte = 0x06;
//           bytesForChecking.append(byte);
//           byte = 0xc0;
//           bytesForChecking.append(byte);

//           if(!packetSETProcessing(buffer)) {
//               if(i == 2) {
//                   label_StatusBar = (tr("Ошибка данных") +
//                                                " Рабочее место: " + QString::number(workPlaceNumber+1));
//                   emit errorStringSignal(label_StatusBar + '\n');
//                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
//                   emit workPlaceOff(currentIndicatorNumber);
//                   emit checkRashodomerError(currentIndicatorNumber);
//                   return;
//               }
//               else {
//                   continue;
//               }
//           }

//           quint8 error = buffer.at(4);

//           if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0] && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3] && error == 0) {

//              qDebug()<<"buffer.toHex() "<<buffer.toHex()
//                   <<"checkCRC(buffer) "<<checkCRC(buffer);


//              //
//              //



//              break;
//           }
//           else {
//                    if(i==2) {
// //                            QMessageBox::information(this, "", tr("Ошибка открытия парольной сессии") +
// //                                                     "Рабочее место: " + QString::number(workPlaceNumber+1));
//                        label_StatusBar = (tr("Ошибка открытия парольной сессии") +
//                                                     ". Рабочее место: " + QString::number(workPlaceNumber+1));
//                        emit errorStringSignal(label_StatusBar + '\n');
// //                            ui->label_writeParams->setVisible(true);
//                        vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
// //                                checkWritingError(currentIndicatorNumber);
//                        emit workPlaceOff(currentIndicatorNumber);
//                        emit checkRashodomerError(currentIndicatorNumber);

//                        return;
//                    }

//           }



//       }


//    }

    //
    //Открыть парольную сессию 5a 00 08 40 01 04 57/


    //
    //Read Measured Value      5a000504

    for(int i=0; i<10;i++) {

       portTmp->clear();
       packetToRead.clear();
       buffer.clear();
       quint8 byte = 0x5a;
       packetToRead.append(byte);
       byte = 0x00;
       packetToRead.append(byte);
       byte = 0x05;
       packetToRead.append(byte);
       byte = 0x04;
       packetToRead.append(byte);

       quint8 crc = makeCRC(packetToRead);
       packetToRead.append(crc);

       quint64 cnt = portTmp->write(packetToRead);
       emit textBrowser(">> " + portTmp->portName() + " " + packetToRead.toHex());

       qDebug()<<"cnt "<<cnt
               <<"packetToRead.toHex() "<<packetToRead.toHex()
               <<"portTmp->portName() "<<portTmp->portName();

       if(cnt == 0) {
//               QMessageBox::information(this, "", tr("Данные в порт не записаны") +
//                                                 " Рабочее место: " + QString::number(workPlaceNumber+1));
           label_StatusBar = (tr("Данные в порт не записаны") +
                                        " Рабочее место: " + QString::number(workPlaceNumber+1));
           emit errorStringSignal(label_StatusBar + '\n');
           vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

           imitRotationOff();
           emit workPlaceOff(currentIndicatorNumber);
           emit checkRashodomerError(currentIndicatorNumber);

           return;
       }

       global::pause(600);

       buffer = portTmp->readAll();
       qDebug()<<"buffer.toHex()"<<buffer.toHex();
       if(!buffer.isEmpty()) emit textBrowser("<< " + portTmp->portName() + " " + buffer.toHex());

       if(buffer.isEmpty()) {
           if(i==9) {
//                   QMessageBox::information(this, "", tr("Не удалось прочитать данные") +
//                                            " Рабочее место: " + QString::number(workPlaceNumber+1));
               label_StatusBar = (tr("Не удалось прочитать данные") +
                                            " Рабочее место: " + QString::number(workPlaceNumber+1));
               emit errorStringSignal(label_StatusBar + '\n');
               vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

               imitRotationOff();
               emit workPlaceOff(currentIndicatorNumber);
               emit checkRashodomerError(currentIndicatorNumber);

               return;
           } else{}

       }
       else {
           //проверяем crc и первые четыре байта ответного пакета
           QByteArray bytesForChecking;
           quint8 byte = 0x5a;
           bytesForChecking.append(byte);
           byte = 0x00;
           bytesForChecking.append(byte);
           byte = 0x20;
           bytesForChecking.append(byte);
           byte = 0x84;
           bytesForChecking.append(byte);

           if(!packetSETProcessing(buffer)) {
               if(i == 9) {
                   label_StatusBar = (tr("Ошибка данных") +
                                                " Рабочее место: " + QString::number(workPlaceNumber+1));
                   emit errorStringSignal(label_StatusBar + '\n');
                   vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
                   imitRotationOff();
                   emit workPlaceOff(currentIndicatorNumber);
                   emit checkRashodomerError(currentIndicatorNumber);
                   return;
               }
               else {
                   continue;
               }
           }

  //         quint8 error = buffer.at(4);



           if(checkCRC(buffer) == 0 && bytesForChecking[0] == buffer[0]
                   && bytesForChecking[1] == buffer[1] && bytesForChecking[3] == buffer[3]) {


              qDebug()<<"buffer.toHex() "<<buffer.toHex()
                   <<"checkCRC(buffer) "<<checkCRC(buffer);


              QByteArray flowValue;
              flowValue.append(buffer.at(12));
              flowValue.append(buffer.at(13));

              quint16 flowInt = (quint8)buffer.at(12);
              flowInt = flowInt<<8 | (quint8)buffer.at(13);

              float flowFloat = flowInt;

               emit textBrowser("RMV VMeasHour array = " + flowValue.toHex());
              emit textBrowser("RMV VMeasHour array = " + QString::number(flowFloat/10000) + " m3/h" /*flowValue.toHex()*/);

              if( !( (flowFloat/10000)>=(0.612-0.612/10) && (flowFloat/10000)<=(0.612 + 0.612/10) ) ) {

                  label_StatusBar = (tr("Недопустимое значение потока") +
                                               " Рабочее место: " + QString::number(workPlaceNumber+1));
                  emit errorStringSignal(label_StatusBar + '\n');
 //                 ui->label_Calibration->setVisible(true);
                  vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                  imitRotationOff();
                  emit workPlaceOff(currentIndicatorNumber);
                  emit checkRashodomerError(currentIndicatorNumber);

                  return;

              }



              break;


            }//ifend


           else {
               if(i==9) {
//                      QMessageBox::information(this, "", tr("Ошибка данных") +
//                                               " Рабочее место: " + QString::number(workPlaceNumber+1));
                  label_StatusBar = (tr("Ошибка данных") +
                                               " Рабочее место: " + QString::number(workPlaceNumber+1));
                  emit errorStringSignal(label_StatusBar + '\n');
 //                 ui->label_Calibration->setVisible(true);
                  vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

                  imitRotationOff();
                  emit workPlaceOff(currentIndicatorNumber);
                  emit checkRashodomerError(currentIndicatorNumber);

                  return;
               }
           }



       }

    }


    if(!imitRotationOff()) {
        label_StatusBar = (tr("Не удалось выключить имитацию вращения") +
                                     " Рабочее место: " + QString::number(workPlaceNumber+1));
        emit errorStringSignal(label_StatusBar + '\n');
        vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

        emit workPlaceOff(currentIndicatorNumber);
        emit checkRashodomerError(currentIndicatorNumber);

        return;
    }

    emit checkRashodomerError(currentIndicatorNumber);

}

void ObjectThread::slotMagnSensor()
{
    //Прочитать таймаут импульса питания на датчик резистивный датчик TIM?	TIM=xxxx

    QByteArray buffer;

//    кто и кому:
//    ПК=0x01	стенд1=0x11	стенд2=0x22	стенд3=0x33	стенд4=0x44

//    1 байт	2 байт	3 байт	4 байт	… байт	… байт	(N-1) байт	N байт
//    кто	    кому	команда					                    Конец сообщения
//    BIN	    BIN	    STRING					                    0x00

    if(!isWorkPlaceUse.at(workPlace)) return;

    int currentIndicatorNumber = workPlace;
    int currentBoxNumber = 8;

    QString command = "TIM?";

    QByteArray parcel;

    quint8 sender = 0x01;
    quint8 receiver;
    quint8 stopByte = 0x00;

    if(workPlace == 0) receiver = 0x11;
    if(workPlace == 1) receiver = 0x22;
    if(workPlace == 2) receiver = 0x33;
    if(workPlace == 3) receiver = 0x44;

    QByteArray stringToByteArray;
    stringToByteArray = command.toLocal8Bit();

    //формирование посылки
    parcel.append(sender);
    parcel.append(receiver);
    for(int m=0; m<stringToByteArray.size(); m++) {
        parcel.append(stringToByteArray.at(m));
    }
    parcel.append(stopByte);

    qDebug()<<"parcel"<<parcel.toHex();

    if(!portStend->isOpen()) {

        if(!portStend->open(QIODevice::ReadWrite)) {

            QString label_StatusBar = (tr("Не удалось открыть порт стенда") +
                                         ". Рабочее место: " + QString::number(workPlace+1));
            emit errorStringSignal(label_StatusBar + '\n');
            vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();

            emit workPlaceOff(workPlace);
            emit checkMagnSensorError(workPlace);
            return;
        }
    }

    //прочитываем пять раз TIM?. если на каком-то шаге произошло уменьшение, то считаем, что импульс сбрасывается

    QVector<int> vectorTIM;

   for (int k = 0; k < 7; ++k) {

    portStend->clear();

    quint64 count = portStend->write(parcel);
    emit textBrowser(">> " + portStend->portName() + " " + parcel.toHex());

    //ждать ответа от стенда 10 мсек

    global::pause(150);

    buffer = portStend->readAll();
 //   portStend->close();

    if(!buffer.isEmpty()) emit textBrowser("<< " + portStend->portName() + " " + buffer.toHex());

    if(buffer.isEmpty()) {
        QString label_StatusBar = (tr("Нет ответа стенда. Команда \"Прочитать таймаут импульса питания на датчик резистивный датчик\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
        vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
        emit workPlaceOff(workPlace);
        emit checkMagnSensorError(workPlace);

  //      portStend->close();
        return;
    }

    QString answerStr;
    QByteArray bufTmp = buffer;

    bufTmp.remove(0, 2);
    bufTmp.remove(bufTmp.size()-1, 1);

    answerStr = QString::fromLocal8Bit(bufTmp);

    QString TIMStr = answerStr;
    TIMStr.remove(0, 4);

    int TIMInt = TIMStr.toInt();
    vectorTIM.append(TIMInt);
    emit textBrowser("TIM=" + TIMStr);

    if(buffer.at(0)!=receiver && buffer.at(1)!=sender && answerStr.left(4)!="TIM=") {
        QString label_StatusBar = (tr("Неверный ответ стенда. Команда \"Прочитать таймаут импульса питания на датчик резистивный датчик\" ") +
                                     ". Рабочее место: " + QString::number(workPlace+1));
        emit errorStringSignal(label_StatusBar + '\n');
        vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
        emit workPlaceOff(workPlace);
        emit checkMagnSensorError(workPlace);
        return;
    }

   }

   //проходим вектор, находим шаг, на котой произошло уменьшение значения
   bool isOk = false;
   for(int m=0; m<(vectorTIM.size()-1); m++) {
       if(vectorTIM.at(m) > vectorTIM.at(m+1)) isOk = true;
   }

   if(isOk == false) {
       QString label_StatusBar = (tr("Таймаут импульса питания на резистивный датчик не сбрасывается\" ") +
                                    ". Рабочее место: " + QString::number(workPlace+1));
       emit errorStringSignal(label_StatusBar + '\n');
       vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true; errorIndicatorOn();
       emit workPlaceOff(workPlace);
       emit checkMagnSensorError(workPlace);
       return;
   }


    emit checkMagnSensorError(workPlace);

   return;

}


void ObjectThread::setTimeIntervalWAL_WRS(int time)
{
    timeIntervalWAL_WRS = time;
}
void ObjectThread::setTimeIntervalWRF_WNS(int time)
{
    timeIntervalWRF_WNS = time;
}
void ObjectThread::setTimeIntervalWNB_WID(int time)
{
    timeIntervalWNB_WID = time;
}

void ObjectThread::setTimeIntervalWID_Read(int time)
{
    timeIntervalWID_Read = time;
}


void ObjectThread::slotGetFinishOff()
{
    finishIndicatorOn();
}



























