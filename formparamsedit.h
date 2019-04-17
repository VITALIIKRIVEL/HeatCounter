#ifndef FORMPARAMSEDIT_H
#define FORMPARAMSEDIT_H

#include <QWidget>
#include <QMap>

namespace Ui {
class FormParamsEdit;
}

class FormParamsEdit : public QWidget
{
    Q_OBJECT

public:
    explicit FormParamsEdit(QWidget *parent = 0);
    ~FormParamsEdit();

signals:
    sendParamsMap(QMap<QString, QByteArray>);
    signalReadParams();
    signalWriteParams();
    signalSendSerialFromParamsEdit(QString);
    signalSendSerial2FromParamsEdit(QString);
    signalSendSerial3FromParamsEdit(QString);
    signalSendSerial4FromParamsEdit(QString);

private slots:
    void slotGetSerial(QString);
    void slotGetSerial2(QString);
    void slotGetSerial3(QString);
    void slotGetSerial4(QString);
    void slotGetParamsStringListFromFileFromMainWindow(QStringList);
    void getbufferReadHardwareConfiguration(QByteArray, int);
    void getbufferReadSoftWareConfiguration(QByteArray);
    void getbufferReadSensorParameters(QByteArray);
    void getbufferReadFlowCoefficient(QByteArray);
    void getbufferReadCalibrationFactor(QByteArray);
    void getbufferReadRealClock(QByteArray);
    void getbufferReadCalibrationFactor2(QByteArray);
    void getbufferReadExternalInterfaceSettings(QByteArray);
    void getbufferReadExternalDeviceSettings(QByteArray);

    void on_lineEdit_serial_textChanged(const QString &arg1);
    void on_lineEdit_serial2_textChanged(const QString &arg1);
    void on_lineEdit_serial3_textChanged(const QString &arg1);
    void on_lineEdit_serial4_textChanged(const QString &arg1);

    void on_comboBox_ManCode_currentIndexChanged(const QString &arg1);

    void on_comboBox_DevType_currentIndexChanged(const QString &arg1);

    void on_comboBox_DN_currentIndexChanged(const QString &arg1);

    void on_lineEdit_PCB_SN_textChanged(const QString &arg1);

    void on_lineEdit_Flow_min_textChanged(const QString &arg1);

    void on_lineEdit_Flow_max_textChanged(const QString &arg1);

    void on_spinBox_Tin_min_valueChanged(int arg1);

    void on_spinBox_Tin_max_valueChanged(int arg1);

    void on_spinBox_Tout_min_valueChanged(int arg1);

    void on_spinBox_Tout_max_valueChanged(int arg1);

    void on_spinBox_ReportData_valueChanged(int arg1);

    void on_comboBox_Average_T_currentIndexChanged(const QString &arg1);

    void on_comboBox_PulsesMode1_currentIndexChanged(const QString &arg1);

    void on_comboBox_PulsesMode2_currentIndexChanged(const QString &arg1);

    void on_lineEdit_PulsesValue1_textChanged(const QString &arg1);

    void on_lineEdit_PulsesValue2_textChanged(const QString &arg1);

    void on_comboBox_FlowRate_currentIndexChanged(const QString &arg1);

    void on_lineEdit_Pressure_textChanged(const QString &arg1);

    void on_comboBox_RTD_Type_currentIndexChanged(const QString &arg1);

    void on_lineEdit_R_RTD_textChanged(const QString &arg1);

    void on_lineEdit_RWire1_textChanged(const QString &arg1);

    void on_lineEdit_RWire2_textChanged(const QString &arg1);

    void on_comboBox_FlowNom_currentIndexChanged(const QString &arg1);

    void on_lineEdit_coef0_textChanged(const QString &arg1);

    void on_lineEdit_coef1_textChanged(const QString &arg1);

    void on_lineEdit_coef2_textChanged(const QString &arg1);

    void on_lineEdit_coef3_textChanged(const QString &arg1);

    void on_lineEdit_coef4_textChanged(const QString &arg1);

    void on_lineEdit_coef5_textChanged(const QString &arg1);

    void on_lineEdit_FS_DiffThrs_textChanged(const QString &arg1);

    void on_dateTimeEdit_realClock_dateTimeChanged(const QDateTime &dateTime);

    void on_comboBox_ASW_flag_currentIndexChanged(const QString &arg1);

    void on_lineEdit_Shift_T1_textChanged(const QString &arg1);

    void on_lineEdit_Shift_T2_textChanged(const QString &arg1);

    void on_lineEdit_Scale_T1_textChanged(const QString &arg1);

    void on_lineEdit_Scale_T2_textChanged(const QString &arg1);

    void on_spinBox_Addr_valueChanged(int arg1);

    void on_comboBox_BaudRate_currentIndexChanged(const QString &arg1);

    void on_lineEdit_HMP_Trans_textChanged(const QString &arg1);

    void on_lineEdit_EPP_Trans_textChanged(const QString &arg1);

    void on_spinBox_P1PrimAddr_valueChanged(int arg1);

    void on_spinBox_P2PrimAddr_valueChanged(int arg1);

    void on_lineEdit_ProtType_textChanged(const QString &arg1);

    void on_lineEdit_Serial_P1_textChanged(const QString &arg1);

    void on_lineEdit_ManCode_P1_textChanged(const QString &arg1);

    void on_lineEdit_DevType_P1_textChanged(const QString &arg1);

    void on_lineEdit_Version_P1_textChanged(const QString &arg1);

    void on_lineEdit_Initial_value_P1_textChanged(const QString &arg1);

    void on_lineEdit_Service_P1_textChanged(const QString &arg1);

    void on_lineEdit_Serial_P2_textChanged(const QString &arg1);

    void on_lineEdit_ManCode_P2_textChanged(const QString &arg1);

    void on_lineEdit_DevType_P2_textChanged(const QString &arg1);

    void on_lineEdit_Version_P2_textChanged(const QString &arg1);

    void on_lineEdit_Initial_value_P2_textChanged(const QString &arg1);

    void on_lineEdit_Service_P2_textChanged(const QString &arg1);

    void on_toolButton_saveTemplate_clicked();

    void on_toolButton_loadTemplate_clicked();

    void on_toolButton_readParams_clicked();

    void on_toolButton_writeParams_clicked();

    void on_lineEdit_coef0_textEdited(const QString &arg1);

    void on_lineEdit_PCB_SN2_textChanged(const QString &arg1);

    void on_lineEdit_PCB_SN3_textChanged(const QString &arg1);

    void on_lineEdit_PCB_SN4_textChanged(const QString &arg1);

private:
    Ui::FormParamsEdit *ui;

    QMap<QString, QByteArray> paramsMap;


//    QSerialPort *port;

    QStringList manCodeList;
    QStringList devTypeList;
    QStringList DNList;
    QStringList AverageTList;
    QStringList pulsesMode1List;
    QStringList pulsesMode2List;
    QStringList flowRateList;
    QStringList flowNomList;
    QStringList RTD_TypeList;
    QStringList ASW_flagList;
    QStringList BaudRateList;

    //Read Software Configuration 5a000502
    QByteArray serialNumber, serialNumber2, serialNumber3, serialNumber4;
    QByteArray manCode;
    QByteArray devType;
    QByteArray DN;
    QByteArray PCB_SN_ByteArray, PCB_SN_ByteArray2, PCB_SN_ByteArray3, PCB_SN_ByteArray4;
    QByteArray flowMinByteArray;
    QByteArray flowMaxByteArray;
    QByteArray tinMinByteArray;
    QByteArray tinMaxByteArray;
    QByteArray toutMinByteArray;
    QByteArray toutMaxByteArray;
    QByteArray reportDataByteArray;
    QByteArray averageTByteArray;
    QByteArray pulsesMode1ByteArray;
    QByteArray pulsesMode2ByteArray;
    QByteArray pulsesValue1ByteArray;
    QByteArray pulsesValue2ByteArray;
    //Read Sensor Parameters      5a00050b
    QByteArray flowRateByteArray;
    QByteArray pressureByteArray;
    QByteArray RTD_TypeByteArray;
    QByteArray R_RTDByteArray;
    QByteArray RWire1ByteArray;
    QByteArray RWire2ByteArray;
    QByteArray flowNomByteArray;
    //Read Flow Coefficient  5a 00 05 1d
    QByteArray flowCoef0ByteArray;
    QByteArray flowCoef1ByteArray;
    QByteArray flowCoef2ByteArray;
    QByteArray flowCoef3ByteArray;
    QByteArray flowCoef4ByteArray;
    QByteArray flowCoef5ByteArray;
    //Read Calibration Factor 5a 00 05 06
    QByteArray FS_DiffThrsByteArray;
    //Read Real Clock 5a 00 05 05
    QByteArray hourByteArray;
    QByteArray minuteByteArray;
    QByteArray secondByteArray;
    QByteArray dayByteArray;
    QByteArray monthByteArray;
    QByteArray yearByteArray;
    QByteArray DOWByteArray;
    QByteArray ASW_flagByteArray;
    //Read Calibration Factor
    QByteArray shift_T1ByteArray;
    QByteArray shift_T2ByteArray;
    QByteArray scale_T1ByteArray;
    QByteArray scale_T2ByteArray;
    //Read External Interface Settings
    QByteArray addrByteArray;
    QByteArray baudRateByteArray;
    QByteArray HMP_TransByteArray;
    QByteArray EPP_TransByteArray;
    QByteArray P1PrimAddrByteArray;
    QByteArray P2PrimAddrByteArray;
    QByteArray ProtTypeByteArray;
    //External Device Settings
    QByteArray Serial_P1ByteArray;
    QByteArray ManCode_P1ByteArray;
    QByteArray DevType_P1ByteArray;
    QByteArray Version_P1ByteArray;
    QByteArray Initial_value_P1ByteArray;
    QByteArray Service_P1ByteArray;
    QByteArray Serial_P2ByteArray;
    QByteArray ManCode_P2ByteArray;
    QByteArray DevType_P2ByteArray;
    QByteArray Version_P2ByteArray;
    QByteArray Initial_value_P2ByteArray;
    QByteArray Service_P2ByteArray;
};

#endif // FORMPARAMSEDIT_H
