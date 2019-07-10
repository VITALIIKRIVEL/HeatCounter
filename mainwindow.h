#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QSerialPort>
#include <QSerialPortinfo>
#include <QTimer>
#include <QSettings>
#include <QDebug>
#include <QWidget>
#include <QThread>
#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QSqlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "formcalibration.h"
#include "formparamsedit.h"
#include "formlog.h"
#include "dialogupdatenote.h"
#include "dialoglogindatabase.h"
#include "dialogwritingdb.h"
#include "dialogisrunwithoutconnect.h"
#include "formviewtable.h"
#include "formconnectionparams.h"
#include "formusercontrol.h"
#include "objectthread.h"
#include "gefest.h"
#include "smartstend.h"

using namespace gefest;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Ui::MainWindow *ui;

    void primeStart();
    void startCommandLine();
    void makeKey();
    void readSettings();
    void writeSettings();
    void loadSettings();
    void makePacketToRead(quint8, quint8);
    quint8 makeCRC(QByteArray);
    quint8 checkCRC(QByteArray);
    void readFromCounter();
    void writeToCounter();
    quint8 reverse8(quint8);
    void readPassword();
    void writeTCC();
    void realClockCalibration();//командой WCC, параметр RTC_Offset (значение параметра RTC_Tcomp всегда 0)
    void saveCounterCheckingResult(int workplace, QString marker);

    void indicatorReset();
    void paintIndicator();

    void checkBslError(int currentIndicator);
    void checkTimeCalibrationError(int currentIndicator);
    void checkRashodomerError(int currentIndicator);
    void checkMagnSensorError(int currentIndicator);
    void checkTokPlatyError(int currentIndicator);
    void checkWritingError(int currentIndicator);
    void checkCalibrationError(int currentIndicator);
    void checkPulsesOutputHeat(int currentIndicator);
    void checkPulsesInputVolume(int currentIndicator);
    void checkPulsesOutputDefault(int currentIndicator);
    void checkMbusOn(int currentIndicator);
    void checkMbusCheck(int currentIndicator);
    void checkCalibrationModeOff(int currentIndicator);
    void checkWireInterfaceChecking(int currentIndicator);
    void checkMbusOff(int currentIndicatorNumber);

    void setPrimeAddress(int workPlaceNumber);
    void setPrimeAddressDefault(int workPlaceNumber);

    void currentWorkPlaceCheckBoxOff(int workPlaceNumber);
    void workPlaceOff(int currentIndicatorNumber);
    //

    void updateVectorStateMatrix(int currentIndicator);

    void wireInterfaceCheckModified(int workPlaceNumber);

    //
    void setCurrentDateToSerial();

    //соединение с сервером
    bool serverConnection();
    bool serverConnectionWithPassword(QString, QString);
    bool serverConnectWithPasswordExtServ(QString username, QString password);

    //выполнение запросов
    bool execQuery(QString queryStr);
    bool SelectQuery(QString str);
    bool InsertQuery(QString);

    //Json
    void makeJsonDoc(QString PCB_SN, QString Rref1, QString Rref2, QString usable);

    //проверка на существование одинаковых номеров
    bool dublicatNumberCheck(QString number);
    bool dublicatNumberCheckExtServ();

    //вставка новой записи в таблицу
    void insertNewNote();

    //обновление записи
    void updateDataBaseNote(QString number, int workPlace);
    void updateDataBaseNoteExtServ(QString number, int workPlace);

    //проверка пароля пользателя по таблице user
    bool userPasswordChecking(QString log, QString pas);

    //прочитать список пользователей
    QStringList readUserTable();

    //стенд

protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
//    bool event(QEvent *event) Q_DECL_OVERRIDE;

    /* Определяем виртуальный метод родительского класса
     * для отрисовки содержимого виджета
     * */
    void paintEvent(QPaintEvent *event);
//    virtual bool eventFilter(QObject *watched, QEvent *event);

signals:

    sendFinishOff1();
    sendFinishOff2();
    sendFinishOff3();
    sendFinishOff4();

    sendCurrentUser(QString user);

    sendUserTable(QStringList list);
    sendParamsConnectFromSettings(QStringList listParamsConnection);
    sendUserList(QStringList list);
    sendTableListToTableForm(QStringList, QString userName, QMap<int, QString> userMap, QStringList, QList<int>);

    sendNoteDataToDialofForm(QString serial, QString jsonString, int workplace, QString dateTime, int usable);
    sendLogPasFromSettings(QString log, QString pas);

    sendParamsStringListFromFileToFormParamsedit(QStringList);
    sendbufferReadHardwareConfiguration(QByteArray, int);
    sendbufferReadSoftWareConfiguration(QByteArray);
    sendbufferReadSensorParameters(QByteArray);
    sendbufferReadFlowCoefficient(QByteArray);
    sendbufferReadCalibrationFactor(QByteArray);
    sendbufferReadRealClock(QByteArray);
    sendbufferReadCalibrationFactor2(QByteArray);
    sendbufferReadExternalInterfaceSettings(QByteArray);
    sendbufferReadExternalDeviceSettings(QByteArray);
    signalSendSerial(QString);
    signalSendSerial2(QString);
    signalSendSerial3(QString);
    signalSendSerial4(QString);

    signalReadParams(int workPlaceNumber);
    signalReadParamsToThread(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);
    signalWriteParamsToThread(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);
    signalCalibrationToThread(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);
    signalPulsesOutputHeatToThread(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);
    signalPulsesInputVolToThread(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);
    signalPulsesOutputDefToThread(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);
    signalMBusOnToThread(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);
    signalCheckMBusToThread(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);
    signalCalibModeOffToThread(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);
    signalDigitalInterfaceToThread(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4,
                                   QSerialPort *portDigitalInterface);
    signalMBusOffToThread(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);

    signalWriteParams(int workPlaceNumber);
    signalWriteParams2(int workPlaceNumber);
    signalWriteParams3(int workPlaceNumber);
    signalWriteParams4(int workPlaceNumber);
    signalCalibration(int workPlaceNumber);
    signalPulsesOutputHeat(int workPlaceNumber);
    signalPulsesInputVolume(int workPlaceNumber);
    signalPulsesOutputDefault(int workPlaceNumber);
    signalMBusOn(int workPlaceNumber);
    signalMBusCheck(int workPlaceNumber);
    signalCalibrationModeOff(int workPlaceNumber);
    signalWireInterfaceChecking(int workPlaceNumber);
    signalMBusOff(int workPlaceNmber);

    signalLog(QString str);

    //Стенд

    startBSLProgramming();
    startTokPlatyRequest();
    startTimeCalibration(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);
    startRashodomer(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);
    startMagnSensor();

    //Стенд/

public slots:

    void setLabelTok1(QString label);
    void setLabelTok2(QString label);
    void setLabelTok3(QString label);
    void setLabelTok4(QString label);
    
    void slotGetAnswerFromStend(QString answer);

    void slotAddUser(QVector<QString> vector);
    void slotChangeUser(QVector<QString> vector);
    void slotDeleteUser(QVector<QString> vector);

    void slotWorkPlaceOff(int);
    void slotCheckBslError(int currentIndicator);
    void slotCheckTimeCalError(int currentIndicator);
    void slotCheckTokPlatyError(int currentIndicator);
    void slotCheckRashodomerError(int currentIndicator);
    void slotCheckMagnSensorError(int currentIndicator);
    void slotCheckWritingError(int);
    void slotCheckCalibrationError(int currentIndicator);
    void slotGetRMV(int color, QString str, int currentIndicator);
    void slotCheckPulsesOutputHeatError(int currentIndicator);
    void slotCheckPulsesInputVolumeError(int currentIndicator);
    void slotCheckPulsesOutputDefError(int currentIndicator);
    void slotCheckMBusOnError(int currentIndicator);
    void slotCheckMBusCheckError(int currentIndicator);
    void slotCheckCalibModeOffError(int currentIndicator);
    void slotCheckWireInterfaceChecking(int currentIndicator);
    void slotCheckMBusOffError(int currentIndicator);
    void slotError(QString);

    void slotPort1AnyError(QSerialPort::SerialPortError error);
    void slotPort2AnyError(QSerialPort::SerialPortError error);
    void slotPort3AnyError(QSerialPort::SerialPortError error);
    void slotPort4AnyError(QSerialPort::SerialPortError error);
    void slotPortDigatalInterAnyError(QSerialPort::SerialPortError error);

private slots:
//    void slotNewThreadOne();
//    void slotNewThreadTwo();
//    void slotNewThreadThree();
//    void slotNewThreadFour();
    void slotGetSignalShowConnPar();
    void slotGetParamsConnection(QStringList);
    void slotGetLoginPassword(QString login, QString password, bool isRememberLogin);
    void slotGetSignalCloseLogin();

    void slotGetLog(QString);
    void slotGetLogFromGefest(nayk::LogType, QString);

    void slotGetSerialFromParamsEdit(QString);
    void slotGetSerial2FromParamsEdit(QString);
    void slotGetSerial3FromParamsEdit(QString);
    void slotGetSerial4FromParamsEdit(QString);

    void slotGetParamsMap(QMap<QString, QByteArray>);

    void slotStartCalibration(QMap<QString, QByteArray>, QMap<QString, float>);
    void slotStartRealClockCalibation(quint16);

    void slotReadParams();
    void slotWriteParams();

//    void slotSignalCalibrationFloatMap(QMap<QString, float>);

    void on_toolButton_programmingBSL_clicked();
    void on_toolButton_openFile_clicked();
    void slotReadyRead();
    void on_toolButton_clearTextBrowser_clicked();
    void on_toolButton_selectProgrammingFile_clicked();
    void on_comboBox_portList_currentIndexChanged(const QString &arg1);
    void on_toolButton_sendCommandOptical_clicked();
    void slotReadyReadOptical();
    void on_comboBox_portListOptical_currentIndexChanged(const QString &arg1);
    void on_lineEdit_reverse_textChanged(const QString &arg1);
    void on_toolButton_saveTemplate_clicked();
    void on_toolButton_loadTemplate_clicked();
    void on_lineEdit_sendCommandOptical_textChanged(const QString &arg1);

    void on_toolButton_readParams_clicked();
    void readParams(int workPlaceNumber);
    //
    void on_toolButton_writeParams_clicked();
    void writeParams(int workPlaceNumber);
    void writeParams2(int workPlaceNumber);
    void writeParams3(int workPlaceNumber);
    void writeParams4(int workPlaceNumber);
    //

    void on_toolButton_readPassword_clicked();

    void on_lineEdit_serial_textChanged(const QString &arg1);

    void on_comboBox_ManCode_currentIndexChanged(const QString &arg1);

    void on_comboBox_DevType_currentIndexChanged(const QString &arg1);

    void on_comboBox_DN_currentIndexChanged(const QString &arg1);

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

    void on_action_triggered();

    void on_action_2_triggered();

    void on_lineEdit_PCB_SN_textChanged(const QString &arg1);

    void on_toolButton_digitalWireInterfaceChecking_clicked();
    void wireInterfaceChecking(int workPlaceNumber);


    void on_comboBox_RTD_Type_2_currentIndexChanged(const QString &arg1);

    void on_toolButton_selectConfigFile_clicked();

    void on_toolButton_startCalibration_clicked();
    void calibration(int workPlaceNumber);


    void on_lineEdit_periodBetweenPulses_textChanged(const QString &arg1);

    void on_toolButton_realClockCalibration_clicked();

    QByteArray on_lineEdit_R_Real1_2_textChanged(const QString &arg1);

    QByteArray on_lineEdit_R_Real2_2_textChanged(const QString &arg1);

    QByteArray on_lineEdit_Temperature1_2_textChanged(const QString &arg1);

    void on_lineEdit_Temperature2_2_textChanged(const QString &arg1);

    void on_comboBox_portListInterfaceChecking_currentIndexChanged(const QString &arg1);

    void on_lineEdit_Error_TMeasRev_2_textChanged(const QString &arg1);

    void on_lineEdit_Acceptable_TMeasDir_2_textChanged(const QString &arg1);

    void on_lineEdit_PulsesValue1_textChanged(const QString &arg1);

    void on_toolButton_pulsesOutputHeat_clicked();
    void pulsesOutputHeat(int workPlaceNumber);


    void on_toolButton_pulsesInputVolume_clicked();
    void pulsesInputVolume(int workPlaceNumber);

    void on_toolButton_pulsesOutputOff_clicked();
    void pulsesOutputDefault(int workPlaceNumber);

    void on_lineEdit_Error_TMeasDir_2_textChanged(const QString &arg1);

    void on_lineEdit_Error_R_Reff1_2_textChanged(const QString &arg1);

    void on_toolButton_MBusOn_clicked();
    void MBusOn(int workPlaceNumber);

    void on_toolButton_MBusOff_clicked();
    void MBusOff(int workPlaceNumber);

    void on_toolButton_CheckMbus_clicked();
    void MBusCheck(int workPlaceNumber);

    void on_toolButton_calibrationModeOff_clicked();
    void calibrationModeOff(int workPlaceNumber);

    void on_toolButton_result_clicked();

    void on_lineEdit_humanName_textEdited(const QString &arg1);

    void on_toolButton_resultFileChecking_clicked();

    void slotTimerWriteParams();
    void slotTimerCalibration();
    void slotTimerPulsesOutputHeat();
    void slotTimerPulsesInputVolume();
    void slotTimerPulsesOutputOff();
    void slotTimerCheckMbus();
    void slotTimerCalibrationModeOff();
    void slotTimerDigitalWireInterfaceChecking();

    void slotReadyReadStend();

    void on_spinBox_serial_valueChanged(const QString &arg1);

    void on_lineEdit_serial_textEdited(const QString &arg1);

    void on_label_writeParams_windowTitleChanged(const QString &title);

    void on_lineEdit_loadParamsFile_textChanged(const QString &arg1);

    void on_toolButton_selectConfigFile_2_clicked();

    void on_toolButton_selectConfigFile_3_clicked();

    void on_toolButton_selectConfigFile_4_clicked();

    void on_comboBox_portListOptical_2_currentIndexChanged(const QString &arg1);

    void on_comboBox_portListOptical_3_currentIndexChanged(const QString &arg1);

    void on_comboBox_portListOptical_4_currentIndexChanged(const QString &arg1);

    void on_lineEdit_serial_2_textChanged(const QString &arg1);

    void on_lineEdit_serial_3_textChanged(const QString &arg1);

    void on_lineEdit_serial_4_textChanged(const QString &arg1);

    void on_lineEdit_selectConfigFile_textChanged(const QString &arg1);

    void on_checkBox_workPlace1_clicked(bool checked);

    void on_checkBox_workPlace2_clicked(bool checked);

    void on_checkBox_workPlace3_clicked(bool checked);

    void on_checkBox_workPlace4_clicked(bool checked);

    void on_checkBox_writeParamsUse_clicked(bool checked);

    void on_checkBox_startCalibrationUse_clicked(bool checked);

    void on_checkBox_pulsesOutputHeatUse_clicked(bool checked);

    void on_checkBox_pulsesInputVolumeUse_clicked(bool checked);

    void on_checkBox_pulsesOutputOffUse_clicked(bool checked);

    void on_checkBox_MBusOnUse_clicked(bool checked);

    void on_checkBox_CheckMbusUse_clicked(bool checked);

    void on_checkBox_calibrationModeOffUse_clicked(bool checked);

    void on_checkBox_digitalWireInterfaceCheckingUse_clicked(bool checked);

    void on_checkBox_MBusOffUse_clicked(bool checked);

    void on_toolButton_executeCommands_clicked();

    void on_checkBox_workPlace1_clicked();

    void on_action_3_triggered();

    void on_textBrowser_textChanged();

    void on_toolButton_setAddressDefault_clicked();

    void on_checkBox_OneWorkPlace_clicked(bool checked);

    void on_checkBox_OneWorkPlace_clicked();

    void on_toolButton_stopThreads_clicked();

    void on_spinBox_serial__valueChanged(const QString &arg1);

    void on_spinBox_serial_2_valueChanged(const QString &arg1);

    void on_spinBox_serial_3_valueChanged(const QString &arg1);

    void on_spinBox_serial_4_valueChanged(const QString &arg1);

    void on_checkBox_MBusOffUse_clicked();

    void on_toolButton_wireInterfaceCheckRepeat_clicked();

    void on_toolButton_serverConnect_clicked();

    void on_toolButton_createJson_clicked();

    void on_toolButton_execSql_clicked();

    void on_toolButton_addNoteToTable_clicked();

    void on_toolButton_uniqNumChecking_clicked();

    void on_toolButton_externalServerConnect_clicked();

    void on_lineEdit_humanName_textChanged(const QString &arg1);

    void on_toolButton_resetAllCommands_clicked();

    void on_toolButton_calibrationRepeat_clicked();

    void on_action_4_triggered();

    void on_ResultTable_triggered();

    void on_connectionParams_triggered();

    void on_action_users_triggered();

    void on_toolButton_calibrationModeOffRepeat_clicked();

    void on_comboBox_portList_2_currentIndexChanged(const QString &arg1);

    void on_comboBox_portList_3_currentIndexChanged(const QString &arg1);

    void on_comboBox_portList_4_currentIndexChanged(const QString &arg1);

    void on_comboBox_portStend_currentIndexChanged(const QString &arg1);

    void on_pushButton_plataOn_clicked();

    void on_pushButton_plataOff_clicked();

    void on_pushButton_progrOn_clicked();

    void on_pushButton_progrOff_clicked();

    void on_pushButton_tokRequest_clicked();

    void on_pushButton_openPortStend_clicked();

    void on_pushButton_closePortStend_clicked();

    void on_pushButton_portStendSettings_clicked();

    void on_radioButton_plataOn1_clicked();

    void on_radioButton_plataOff1_clicked();

    void on_radioButton_plataOn1_clicked(bool checked);

    void on_radioButton_plataOn2_clicked(bool checked);

    void on_radioButton_15_clicked(bool checked);

    void on_radioButton_plataOn3_clicked(bool checked);

    void on_radioButton_plataOn4_clicked(bool checked);

    void on_radioButton_progrOn1_clicked(bool checked);

    void on_radioButton_progrOn2_clicked(bool checked);

    void on_radioButton_progrOn3_clicked(bool checked);

    void on_radioButton_progrOn4_clicked(bool checked);

    void on_radioButton_plataOff1_clicked(bool checked);

    void on_radioButton_plataOff2_clicked(bool checked);

    void on_radioButton_plataOff3_clicked(bool checked);

    void on_radioButton_plataOff4_clicked(bool checked);

    void on_radioButton_progrOff1_clicked(bool checked);

    void on_radioButton_progrOff2_clicked(bool checked);

    void on_radioButton_progrOff3_clicked(bool checked);

    void on_radioButton_progrOff4_clicked(bool checked);

    void on_radioButton_Mbus1_clicked(bool checked);

    void on_radioButton_RS1_clicked(bool checked);

    void on_radioButton_InterfaceOff1_clicked(bool checked);

    void on_radioButton_Mbus2_clicked(bool checked);

    void on_radioButton_RS2_clicked(bool checked);

    void on_radioButton_InterfaceOff2_clicked(bool checked);

    void on_radioButton_Mbus3_clicked(bool checked);

    void on_radioButton_RS3_clicked(bool checked);

    void on_radioButton_InterfaceOff3_clicked(bool checked);

    void on_radioButton_Mbus4_clicked(bool checked);

    void on_radioButton_RS4_clicked(bool checked);

    void on_radioButton_InterfaceOff4_clicked(bool checked);

    void on_toolButton_pulsesReset_clicked();

    void on_comboBox_portStend_currentTextChanged(const QString &arg1);

    void on_comboBox_portStend_2_currentIndexChanged(const QString &arg1);

    void on_comboBox_portStend_3_currentIndexChanged(const QString &arg1);

    void on_comboBox_portStend_4_currentIndexChanged(const QString &arg1);

private:
    
 //   Ui::MainWindow *ui;
    FormCalibration *formCalibration;
    FormParamsEdit *formParamsEdit;
    FormLog *formLog;
    DialogUpdateNote *dialogUpdateNote;
    DialogLoginDataBase *dialogLoginDataBase;
    DialogWritingDB *dialogWritingDB;
    DialogIsRunWithoutConnect * dialogIsRunWithoutConnect;
    FormViewTable * formViewTable;
    FormConnectionParams * formConnectionParams;
    Formusercontrol * formUserControl;

    SmartStend * smartStend;
    SmartStend * smartStend2;
    SmartStend * smartStend3;
    SmartStend * smartStend4;

    ObjectThread *ObjectThread1;
    ObjectThread *ObjectThread2;
    ObjectThread *ObjectThread3;
    ObjectThread *ObjectThread4;

    QThread Thread1;
    QThread Thread2;
    QThread Thread3;
    QThread Thread4;

    QSqlDatabase dataBase;
    QSqlQuery currentQueryResult;

    QJsonObject currentJsonObject1;
    QJsonObject currentJsonObject2;
    QJsonObject currentJsonObject3;
    QJsonObject currentJsonObject4;

    QString R_Ref1String1, R_Ref2String1, R_Ref1String2, R_Ref2String2,
            R_Ref1String3, R_Ref2String3, R_Ref1String4, R_Ref2String4;

    QString PCB_SN_String1, PCB_SN_String2, PCB_SN_String3, PCB_SN_String4;

    QString usableResultString1, usableResultString2,
            usableResultString3, usableResultString4;

    QString serialNumberString1, serialNumberString2,
            serialNumberString3, serialNumberString4;
    //
//    Gefest *gef1;
//    Gefest *gef2;
//    Gefest *gef3;
//    Gefest *gef4;

//    typedef struct PortSettingsStruct {
//        QString portName;
//        qint32 baudRate;
//        QSerialPort::DataBits dataBits;
//        QSerialPort::StopBits stopBits;
//        QSerialPort::Parity parity;
//        bool dtr;
//        bool rts;
//        qint32 bufSize;
//    } PortSettingsStruct;
    //
    QProcess *cmd;
    QSerialPort *port, *port2, *port3, *port4;
    QSerialPort *portOptical, *portOptical2, *portOptical3, *portOptical4;
    QSerialPort *portCurrentOptical;
    QSerialPort *portDigitalInterfaceChecking;
    QSerialPort *portStend, *portStend2, *portStend3, *portStend4;
    QString portName, portName2, portName3, portName4;
    QString portNameOptical, portNameOptical2, portNameOptical3, portNameOptical4;
    QString portNameInterfaceChecking;
    QString fileName;
    QString processData;
    QSettings settings;
    QString programmingFileToString;
    QStringList programmingFileToStringList;
    QByteArray packetToRead;
    QByteArray packetToWrite;
    QByteArray packetFromLineEdit;
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
    quint16 password;

    //Read Software Configuration 5a000502
    QByteArray serialNumber, serialNumber2, serialNumber3, serialNumber4;
    QByteArray manCode, manCode2, manCode3, manCode4;
    QByteArray devType, devType2, devType3, devType4;
    QByteArray DN, DN2, DN3, DN4;
    QByteArray PCB_SN_ByteArray, PCB_SN_ByteArray2, PCB_SN_ByteArray3, PCB_SN_ByteArray4;
    QByteArray flowMinByteArray, flowMinByteArray2, flowMinByteArray3, flowMinByteArray4;
    QByteArray flowMaxByteArray, flowMaxByteArray2, flowMaxByteArray3, flowMaxByteArray4;
    QByteArray tinMinByteArray, tinMinByteArray2, tinMinByteArray3, tinMinByteArray4;
    QByteArray tinMaxByteArray, tinMaxByteArray2, tinMaxByteArray3, tinMaxByteArray4;
    QByteArray toutMinByteArray, toutMinByteArray2, toutMinByteArray3, toutMinByteArray4;
    QByteArray toutMaxByteArray, toutMaxByteArray2, toutMaxByteArray3, toutMaxByteArray4;
    QByteArray reportDataByteArray, reportDataByteArray2, reportDataByteArray3, reportDataByteArray4;
    QByteArray averageTByteArray, averageTByteArray2, averageTByteArray3, averageTByteArray4;
    QByteArray pulsesMode1ByteArray, pulsesMode1ByteArray2, pulsesMode1ByteArray3, pulsesMode1ByteArray4;
    QByteArray pulsesMode2ByteArray, pulsesMode2ByteArray2, pulsesMode2ByteArray3,pulsesMode2ByteArray4;
    quint8 pulsesMode1PrimalInt, pulsesMode1PrimalInt2, pulsesMode1PrimalInt3, pulsesMode1PrimalInt4;
    quint8 pulsesMode2PrimalInt, pulsesMode2PrimalInt2, pulsesMode2PrimalInt3, pulsesMode2PrimalInt4;
    QByteArray pulsesValue1ByteArray, pulsesValue1ByteArray2, pulsesValue1ByteArray3, pulsesValue1ByteArray4;
    QByteArray pulsesValue2ByteArray, pulsesValue2ByteArray2, pulsesValue2ByteArray3, pulsesValue2ByteArray4;
    quint16 pulsesValue1PrimeInt, pulsesValue1PrimeInt2, pulsesValue1PrimeInt3, pulsesValue1PrimeInt4;
    quint16 pulsesValue2PrimeInt, pulsesValue2PrimeInt2, pulsesValue2PrimeInt3, pulsesValue2PrimeInt4;

    //Read Sensor Parameters      5a00050b
    QByteArray flowRateByteArray, flowRateByteArray2, flowRateByteArray3, flowRateByteArray4;
    QByteArray pressureByteArray, pressureByteArray2, pressureByteArray3, pressureByteArray4;
    QByteArray RTD_TypeByteArray, RTD_TypeByteArray2, RTD_TypeByteArray3, RTD_TypeByteArray4;
    QByteArray R_RTDByteArray, R_RTDByteArray2, R_RTDByteArray3, R_RTDByteArray4;
    QByteArray RWire1ByteArray, RWire1ByteArray2, RWire1ByteArray3, RWire1ByteArray4;
    QByteArray RWire2ByteArray, RWire2ByteArray2, RWire2ByteArray3, RWire2ByteArray4;
    QByteArray flowNomByteArray, flowNomByteArray2, flowNomByteArray3, flowNomByteArray4;

    //Read Flow Coefficient  5a 00 05 1d
    QByteArray flowCoef0ByteArray, flowCoef0ByteArray2, flowCoef0ByteArray3, flowCoef0ByteArray4;
    QByteArray flowCoef1ByteArray, flowCoef1ByteArray2, flowCoef1ByteArray3, flowCoef1ByteArray4;
    QByteArray flowCoef2ByteArray, flowCoef2ByteArray2, flowCoef2ByteArray3, flowCoef2ByteArray4;
    QByteArray flowCoef3ByteArray, flowCoef3ByteArray2, flowCoef3ByteArray3, flowCoef3ByteArray4;
    QByteArray flowCoef4ByteArray, flowCoef4ByteArray2, flowCoef4ByteArray3, flowCoef4ByteArray4;
    QByteArray flowCoef5ByteArray, flowCoef5ByteArray2, flowCoef5ByteArray3, flowCoef5ByteArray4;

    //Read Calibration Factor 5a 00 05 06
    QByteArray FS_DiffThrsByteArray, FS_DiffThrsByteArray2, FS_DiffThrsByteArray3, FS_DiffThrsByteArray4;

    //Read Real Clock 5a 00 05 05
    QByteArray hourByteArray, hourByteArray2, hourByteArray3, hourByteArray4;
    QByteArray minuteByteArray, minuteByteArray2, minuteByteArray3, minuteByteArray4;
    QByteArray secondByteArray, secondByteArray2, secondByteArray3, secondByteArray4;
    QByteArray dayByteArray, dayByteArray2, dayByteArray3, dayByteArray4;
    QByteArray monthByteArray, monthByteArray2, monthByteArray3, monthByteArray4;
    QByteArray yearByteArray, yearByteArray2, yearByteArray3, yearByteArray4;
    QByteArray DOWByteArray, DOWByteArray2, DOWByteArray3, DOWByteArray4;
    QByteArray ASW_flagByteArray, ASW_flagByteArray2, ASW_flagByteArray3, ASW_flagByteArray4;

    //Read Calibration Factor
    QByteArray shift_T1ByteArray, shift_T1ByteArray2, shift_T1ByteArray3, shift_T1ByteArray4;
    QByteArray shift_T2ByteArray, shift_T2ByteArray2, shift_T2ByteArray3, shift_T2ByteArray4;
    QByteArray scale_T1ByteArray, scale_T1ByteArray2, scale_T1ByteArray3, scale_T1ByteArray4;
    QByteArray scale_T2ByteArray, scale_T2ByteArray2, scale_T2ByteArray3, scale_T2ByteArray4;

    //Read External Interface Settings
    QByteArray addrByteArray, addrByteArray2, addrByteArray3, addrByteArray4;
    QByteArray baudRateByteArray, baudRateByteArray2, baudRateByteArray3, baudRateByteArray4;
    QByteArray HMP_TransByteArray, HMP_TransByteArray2, HMP_TransByteArray3, HMP_TransByteArray4;
    QByteArray EPP_TransByteArray, EPP_TransByteArray2, EPP_TransByteArray3, EPP_TransByteArray4;
    QByteArray P1PrimAddrByteArray, P1PrimAddrByteArray2, P1PrimAddrByteArray3, P1PrimAddrByteArray4;
    QByteArray P2PrimAddrByteArray, P2PrimAddrByteArray2, P2PrimAddrByteArray3, P2PrimAddrByteArray4;
    QByteArray ProtTypeByteArray, ProtTypeByteArray2, ProtTypeByteArray3, ProtTypeByteArray4;

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

    //Калибровка TCC
    QMap<QString, QByteArray> calibrationMap;
    QMap<QString, float> calibrationFloatMap;

    QByteArray R_Real1ByteArray, R_Real1ByteArray2, R_Real1ByteArray3, R_Real1ByteArray4;
    QByteArray R_Real2ByteArray, R_Real2ByteArray2, R_Real2ByteArray3, R_Real2ByteArray4;
    QByteArray Temperature1ByteArray, Temperature1ByteArray2, Temperature1ByteArray3, Temperature1ByteArray4;
    QByteArray Temperature2ByteArray, Temperature2ByteArray2, Temperature2ByteArray3, Temperature2ByteArray4;
    QByteArray temperature1ByteArray, temperature1ByteArray2, temperature1ByteArray3, temperature1ByteArray4;
    QByteArray temperature2ByteArray, temperature2ByteArray2, temperature2ByteArray3, temperature2ByteArray4;


    float R_Ref1Float, R_Ref1Float2, R_Ref1Float3, R_Ref1Float4;
    float R_Ref2Float, R_Ref2Float2, R_Ref2Float3, R_Ref2Float4;
    float R_Real1Float, R_Real1Float2, R_Real1Float3, R_Real1Float4;
    float R_Real2Float, R_Real2Float2, R_Real2Float3, R_Real2Float4;
    float temperature1Float, temperature1Float2, temperature1Float3, temperature1Float4;
    float temperature2Float, temperature2Float2, temperature2Float3, temperature2Float4;
    float Acceptable_R_Ref1Float, Acceptable_R_Ref1Float2, Acceptable_R_Ref1Float3, Acceptable_R_Ref1Float4;
    float Acceptable_R_Ref2Float, Acceptable_R_Ref2Float2, Acceptable_R_Ref2Float3, Acceptable_R_Ref2Float4;
    float Acceptable_TMeasDirFloat, Acceptable_TMeasDirFloat2, Acceptable_TMeasDirFloat3, Acceptable_TMeasDirFloat4;
    float Acceptable_TMeasRevFloat, Acceptable_TMeasRevFloat2, Acceptable_TMeasRevFloat3, Acceptable_TMeasRevFloat4;
    float Error_R_Reff1Float, Error_R_Reff1Float2, Error_R_Reff1Float3, Error_R_Reff1Float4;
    float Error_R_Reff2Float, Error_R_Reff2Float2, Error_R_Reff2Float3, Error_R_Reff2Float4;
    float Error_TMeasDirFloat, Error_TMeasDirFloat2, Error_TMeasDirFloat3, Error_TMeasDirFloat4;
    float Error_TMeasRevFloat, Error_TMeasRevFloat2, Error_TMeasRevFloat3, Error_TMeasRevFloat4;
    int RTD_TypeInt, RTD_TypeInt2, RTD_TypeInt3, RTD_TypeInt4;

    //калибровка генератора часов
    float periodBetweenPulses;

    QVector<QByteArray> paramsVector;
    QVector<QByteArray> paramsVectorToWrite;

    QByteArray bufferReadHardwareConfiguration;
    QByteArray bufferReadSoftWareConfiguration;
    QByteArray bufferReadSensorParameters;
    QByteArray bufferReadFlowCoefficient;
    QByteArray bufferReadCalibrationFactor;
    QByteArray bufferReadRealClock;
    QByteArray bufferReadCalibrationFactor2;
    QByteArray ReadExternalInterfaceSettings;
    QByteArray ReadExternalDeviceSettings;

    //проверка записанного и прочитанного
    QMap<QString, QByteArray> mapRead;
    QMap<QString, QByteArray> mapwrite;
    QStringList allParamsNameList;

    //result
    QString initFileNameResult;
    QString manufacturerNumberResult, manufacturerNumberResult2, manufacturerNumberResult3, manufacturerNumberResult4;

    QString r_ref1Result, r_ref1Result2, r_ref1Result3, r_ref1Result4,
            r_ref2Result, r_ref2Result2, r_ref2Result3, r_ref2Result4,
            rtc_OffsetResult, rtc_OffsetResult2, rtc_OffsetResult3, rtc_OffsetResult4,
            rtc_TcompResult, rtc_TcompResult2, rtc_TcompResult3, rtc_TcompResult4;

    QString humanNameResult;
    QString usableResult, usableResult2, usableResult3, usableResult4;

    //timers
    QTimer timerWriteParams, timerWriteParams2, timerWriteParams3, timerWriteParams4,
    timerCalibration, timerPulsesOutputHeat,
    timerPulsesInputVolume, timerPulsesOutputOff, timerCheckMbus,
    timerCalibrationModeOff, timerDigitalWireInterfaceChecking;

    //
    QVector<bool> vectorIsWorkPlaceUse;

    QVector<bool> vectorIsCommandUse;

    QVector<bool> vectorIsErrorOccured;

    QVector<QVector<bool>> vectorIndicatorStateMatrix;
    QVector<bool> vectorBSL;
    QVector<bool> vectorTokPlaty;

    int currentBoxNumber;
    int currentIndicatorNumber;

    bool isNeedPaintEvent;

    bool isBslFinished1, isBslFinished2, isBslFinished3, isBslFinished4;
    bool isTimeCalFinished1, isTimeCalFinished2, isTimeCalFinished3, isTimeCalFinished4;
    bool isTokPlatyFinished1, isTokPlatyFinished2, isTokPlatyFinished3, isTokPlatyFinished4;
    bool isRashodomerFinished1, isRashodomerFinished2, isRashodomerFinished3, isRashodomerFinished4;
    bool isMagnSensorFinished1, isMagnSensorFinished2, isMagnSensorFinished3, isMagnSensorFinished4;
    bool isWritingFinished1, isWritingFinished2, isWritingFinished3, isWritingFinished4;
    bool isCalibrationFinished1, isCalibrationFinished2, isCalibrationFinished3, isCalibrationFinished4;
    bool isPulsesOutputHeat1, isPulsesOutputHeat2, isPulsesOutputHeat3, isPulsesOutputHeat4;
    bool isPulsesInputVolume1, isPulsesInputVolume2, isPulsesInputVolume3, isPulsesInputVolume4;
    bool isPulsesOutputDefault1, isPulsesOutputDefault2, isPulsesOutputDefault3, isPulsesOutputDefault4;
    bool isMBusOn1, isMBusOn2, isMBusOn3, isMBusOn4;
    bool isMBusCheck1, isMBusCheck2, isMBusCheck3, isMBusCheck4;
    bool isCalibrationModeOff1, isCalibrationModeOff2, isCalibrationModeOff3, isCalibrationModeOff4;
    bool isWireInterfaceChecking1, isWireInterfaceChecking2, isWireInterfaceChecking3, isWireInterfaceChecking4;
    bool isMBusOff1, isMBusOff2, isMBusOff3, isMBusOff4;

    QString errorString;
    
    QMap<QString, QVariant> paramsMapToThreads;

    bool KeyEventEnterBool;

    bool isPulsOutputHeatCmdStarted;
    bool isPulsInputVolumeCmdStarted;
    bool isPulsesOutputDefCmdStarted;

    bool isCommandsEnded;
    bool isBslEnded;

    //БД
    QString workPlace1ResultString, workPlace2ResultString, workPlace3ResultString, workPlace4ResultString;

    bool isDublExist1, isDublExist2, isDublExist3, isDublExist4;

    bool isLastNumberWritedToDataBase1, isLastNumberWritedToDataBase2,
         isLastNumberWritedToDataBase3, isLastNumberWritedToDataBase4;

    QString loginDB;
    QString passwordDB;
    bool serverConnect;

    QByteArray jsonByteArray1, jsonByteArray2, jsonByteArray3, jsonByteArray4;

    QString currentDublicatTime1, currentDublicatTime2, currentDublicatTime3, currentDublicatTime4;

    int currentDublicatUsable1, currentDublicatUsable2, currentDublicatUsable3, currentDublicatUsable4;

    bool isRememberLogPas;

    bool isLoginPasswordOk;

    bool isNoteWriting, isNoteWriting2, isNoteWriting3, isNoteWriting4;

    int userId;

    QString hostName, dataBaseName, dataBaseUserName, dataBasePassword;

    QStringList userTable;

    int repeatParameter;

    bool connectCondition;
    bool isRunWithoutConnect;

    //стенд
    QString extInterface1, extInterface2, extInterface3, extInterface4;

};

#endif // MAINWINDOW_H
