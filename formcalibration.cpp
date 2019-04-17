#include "formcalibration.h"
#include "ui_formcalibration.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QFileDialog>

FormCalibration::FormCalibration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCalibration)
{
    ui->setupUi(this);

    QStringList RTD_TypeList = QStringList()<<"0"<<"1"<<"2"<<"3";
    ui->comboBox_RTD_Type->addItems(RTD_TypeList);

    QStringList configFileFormat = QStringList()<<"R_Real1"<<"R_Real2"<<"Temperature1_type0"<<"Temperature2_type0"<<
                                                  "Temperature1_type1"<<"Temperature2_type1"<<
                                                  "Temperature1_type2"<<"Temperature2_type2"<<
                                                  "Temperature1_type3"<<"Temperature2_type3"<<
                                                  "Acceptable_R_Ref1"<<"Acceptable_R_Ref2"<<
                                                  "Acceptable_TMeasDir"<<"Acceptable_TMeasRev"<<
                                                  "Error_R_Reff1"<<"Error_R_Reff2"<<
                                                  "Error_TMeasDir"<<"Error_TMeasRev";
}
/******************************************************************/
FormCalibration::~FormCalibration()
{
    delete ui;
}
/******************************************************************/
void FormCalibration::on_toolButton_selectConfigFile_clicked()
{
    QMessageBox box;
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    QString fileName = fileDialog.getOpenFileName(this, tr("Open File"),
                                                  QDir::currentPath(),
                                                  tr("Текст (*.txt)"));
    qDebug()<<"fileName"<<fileName;
    ui->lineEdit_selectConfigFile->setText(fileName);

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

    if( !fileToStringList[0].contains("R_Real1 ") || !fileToStringList[1].contains("R_Real2 ") ||
            !fileToStringList[2].contains("Temperature1_type0 ") || !fileToStringList[3].contains("Temperature2_type0 ") ||
            !fileToStringList[4].contains("Temperature1_type1 ") || !fileToStringList[5].contains("Temperature2_type1 ") ||
            !fileToStringList[6].contains("Temperature1_type2 ") || !fileToStringList[7].contains("Temperature2_type2 ") ||
            !fileToStringList[8].contains("Temperature1_type3 ") || !fileToStringList[9].contains("Temperature2_type3 ") ||
            !fileToStringList[10].contains("Acceptable_R_Ref1 ") || !fileToStringList[11].contains("Acceptable_R_Ref2 ") ||
            !fileToStringList[12].contains("Acceptable_TMeasDir ") || !fileToStringList[13].contains("Acceptable_TMeasRev ") ||
            !fileToStringList[14].contains("Error_R_Reff1 ") || !fileToStringList[15].contains("Error_R_Reff2 ") ||
            !fileToStringList[16].contains("Error_TMeasDir ") || !fileToStringList[17].contains("Error_TMeasRev ") ) {

        QMessageBox::information(this, "", "Неверный формат файла");
        return;

    }


    QString R_Real1String = fileToStringList[0];
    R_Real1String.remove("R_Real1 ");
    ui->lineEdit_R_Real1->setText(R_Real1String);
    QString R_Real2String = fileToStringList[1];
    R_Real1String.remove("R_Real2 ");
    ui->lineEdit_R_Real2->setText(R_Real1String);

    if(RTD_TypeInt == 0) {
        QString temperature1String = fileToStringList[2];
        temperature1String.remove("Temperature1_type0 ");
        ui->lineEdit_Temperature1->setText(temperature1String);

        QString temperature2String = fileToStringList[3];
        temperature2String.remove("Temperature2_type0 ");
        ui->lineEdit_Temperature2->setText(temperature2String);
    }
    if(RTD_TypeInt == 1) {
        QString temperature1String = fileToStringList[4];
        temperature1String.remove("Temperature1_type1 ");
        ui->lineEdit_Temperature1->setText(temperature1String);

        QString temperature2String = fileToStringList[5];
        temperature2String.remove("Temperature2_type1 ");
        ui->lineEdit_Temperature2->setText(temperature2String);
    }
    if(RTD_TypeInt == 2) {
        QString temperature1String = fileToStringList[6];
        temperature1String.remove("Temperature1_type2 ");
        ui->lineEdit_Temperature1->setText(temperature1String);

        QString temperature2String = fileToStringList[7];
        temperature2String.remove("Temperature2_type2 ");
        ui->lineEdit_Temperature2->setText(temperature2String);
    }
    if(RTD_TypeInt == 3) {
        QString temperature1String = fileToStringList[8];
        temperature1String.remove("Temperature1_type3 ");
        ui->lineEdit_Temperature1->setText(temperature1String);

        QString temperature2String = fileToStringList[9];
        temperature2String.remove("Temperature2_type3 ");
        ui->lineEdit_Temperature2->setText(temperature2String);
    }

    QStringList Acceptable_R_Ref1List = fileToStringList[10].split(' ');
    ui->lineEdit_Acceptable_R_Ref1->setText(Acceptable_R_Ref1List[1]);
    Acceptable_R_Ref1Float = Acceptable_R_Ref1List[1].toFloat();
    calibrationFloatMap["Acceptable_R_Ref1Float"] = Acceptable_R_Ref1Float;

    QStringList Acceptable_R_Ref2List = fileToStringList[11].split(' ');
    ui->lineEdit_Acceptable_R_Ref2->setText(Acceptable_R_Ref2List[1]);
    Acceptable_R_Ref2Float = Acceptable_R_Ref2List[1].toFloat();
    calibrationFloatMap["Acceptable_R_Ref2Float"] = Acceptable_R_Ref2Float;

    QStringList Acceptable_TMeasDirList = fileToStringList[12].split(' ');
    ui->lineEdit_Acceptable_TMeasDir->setText(Acceptable_TMeasDirList[1]);
    Acceptable_TMeasDirFloat = Acceptable_TMeasDirList[1].toFloat();
    calibrationFloatMap["Acceptable_TMeasDirFloat"] = Acceptable_TMeasDirFloat;

    QStringList Acceptable_TMeasRevList = fileToStringList[13].split(' ');
    ui->lineEdit_Acceptable_TMeasRev->setText(Acceptable_TMeasRevList[1]);
    Acceptable_TMeasRevFloat = Acceptable_TMeasRevList[1].toFloat();
    calibrationFloatMap["Acceptable_TMeasRevFloat"] = Acceptable_TMeasRevFloat;

    QStringList Error_R_Reff1List = fileToStringList[14].split(' ');
    ui->lineEdit_Error_R_Reff1->setText(Error_R_Reff1List[1]);
    Error_R_Reff1Float = Error_R_Reff1List[1].toFloat();
    calibrationFloatMap["Error_R_Reff1Float"] = Error_R_Reff1Float;

    QStringList Error_R_Reff2List = fileToStringList[15].split(' ');
    ui->lineEdit_Error_R_Reff2->setText(Error_R_Reff2List[1]);
    Error_R_Reff2Float = Error_R_Reff2List[1].toFloat();
    calibrationFloatMap["Error_R_Reff2Float"] = Error_R_Reff2Float;

    QStringList Error_TMeasDirList = fileToStringList[16].split(' ');
    ui->lineEdit_Error_TMeasDir->setText(Error_TMeasDirList[1]);
    Error_TMeasDirFloat = Error_TMeasDirList[1].toFloat();
    calibrationFloatMap["Error_TMeasDirFloat"] = Error_TMeasDirFloat;

    QStringList Error_TMeasRevList = fileToStringList[17].split(' ');
    ui->lineEdit_Error_TMeasRev->setText(Error_TMeasRevList[1]);
    Error_TMeasRevFloat = Error_TMeasRevList[1].toFloat();
    calibrationFloatMap["Error_TMeasRevFloat"] = Error_TMeasRevFloat;



    loadTemplate.close();
}
/******************************************************************/
void FormCalibration::on_toolButton_startCalibration_clicked()
{
    emit startCalibration(calibrationMap, calibrationFloatMap);

 //   emit signalCalibrationFloatMap(calibrationFloatMap);
}
/******************************************************************/
void FormCalibration::on_lineEdit_R_Real1_textChanged(const QString &arg1)
{
    R_Real1ByteArray.clear();
    bool ok;
    float rreal1Float = arg1.toFloat()*1000;

    quint32 rreal1Int = (quint32)rreal1Float;
    R_Real1ByteArray.append((quint8)(rreal1Int>>24));
    R_Real1ByteArray.append((quint8)(rreal1Int>>16));
    R_Real1ByteArray.append((quint8)(rreal1Int>>8));
    R_Real1ByteArray.append((quint8)(rreal1Int));

    qDebug()<<"R_Real1ByteArray.toHex() "<<R_Real1ByteArray.toHex();

    calibrationMap["R_Real1ByteArray"] = R_Real1ByteArray;
}
/******************************************************************/
void FormCalibration::on_lineEdit_R_Real2_textChanged(const QString &arg1)
{
    R_Real2ByteArray.clear();
    bool ok;
    float rreal2Float = arg1.toFloat()*1000;

    quint32 rreal2Int = (quint32)rreal2Float;
    R_Real2ByteArray.append((quint8)(rreal2Int>>24));
    R_Real2ByteArray.append((quint8)(rreal2Int>>16));
    R_Real2ByteArray.append((quint8)(rreal2Int>>8));
    R_Real2ByteArray.append((quint8)(rreal2Int));

    qDebug()<<"R_Real2ByteArray.toHex() "<<R_Real2ByteArray.toHex();

    calibrationMap["R_Real2ByteArray"] = R_Real2ByteArray;
}
/******************************************************************/
void FormCalibration::on_lineEdit_Temperature1_textChanged(const QString &arg1)
{
    temperature1ByteArray.clear();
    bool ok;
    quint16 temp1Int = arg1.toInt(&ok, 10);
    temperature1ByteArray.append((quint8)temp1Int>>8);
    temperature1ByteArray.append((quint8)temp1Int);

    calibrationMap["temperature1ByteArray"] = temperature1ByteArray;
}
/******************************************************************/
void FormCalibration::on_lineEdit_Temperature2_textChanged(const QString &arg1)
{
    temperature2ByteArray.clear();
    bool ok;
    quint16 temp2Int = arg1.toInt(&ok, 10);
    temperature2ByteArray.append((quint8)temp2Int>>8);
    temperature2ByteArray.append((quint8)temp2Int);

    calibrationMap["temperature2ByteArray"] = temperature2ByteArray;
}
/******************************************************************/

void FormCalibration::on_comboBox_RTD_Type_currentIndexChanged(const QString &arg1)
{
    RTD_TypeInt = arg1.toInt();
}

void FormCalibration::on_toolButton_realClockCalibration_clicked()
{
//    Значение коэффициента коррекции должно рассчитываться по формуле:
//    K = |Round(983040 * (1 - 1953,125 / Tи))|,
//    где Tи – измеренное значение периода в мс.

    float periodError = (1953.125/10000)*3;

    if( !((periodBetweenPulses >= 1953.125 - periodError) && (periodBetweenPulses <= 1953.125 + periodError)) ) {
        QMessageBox::information(this, "" , tr("Недопустимое значение периода следования импульсов "));
        return;
    }

    quint16 correctionCoeff = (quint16)round(abs(983040*(1 - 1953.125/periodBetweenPulses)));
    qDebug()<<"correctionCoeff "<<correctionCoeff;

    if(correctionCoeff > 240) {
        QMessageBox::information(this, "", tr("Недопустимое значение коэффициента коррекции ") + QString::number(correctionCoeff));
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

    emit signalStartRealClockCalibration(correctionCoeffResult);


}

void FormCalibration::on_lineEdit_periodBetweenPulses_textChanged(const QString &arg1)
{
    bool ok;
    periodBetweenPulses = arg1.toFloat(&ok);
}
