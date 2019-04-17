#ifndef GEFEST_H
#define GEFEST_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>
#include <QString>

#ifdef WIDGETS_SOFT
#include <QComboBox>
#endif

//
#include "ComPort"
#include "Log"
//===================================================================================================
namespace gefest {
//===========================================================================================
using namespace nayk;
//===========================================================================================

// Коды ошибок Gefest:
const quint8 Error_No	              = 0x00; // Нет ошибки
const quint8 Error_Command	          = 0x03; // Неверная команда сообщения
const quint8 Error_TimeOut	          = 0x04; // Сообщение не принято в течении 2 с
const quint8 Error_FlashAccess	      = 0x05; // Ошибка записи во внутреннюю EEPROM
const quint8 Error_RealClock	      = 0x07; // Ошибка часов реального времени
const quint8 Error_CalibrKey	      = 0x09; // Не включен режим "Калибровка"
const quint8 Error_OutOffRange	      = 0x0B; // Выход параметров за допустимые границы
const quint8 Error_PswTO	          = 0x0C; // Парольная сессия закрыта теплосчетчиком
const quint8 Error_PswEq	          = 0x0D; // В команде PSS или PSC передан неверный пароль
const quint8 Error_PswNotOpen	      = 0x0E; // Парольная сессия не открыта

// Коды ошибок обмена:
const quint16 Error_TimeOutWaitAnswer = 0x0100; // Тайм-аут ожидания ответа
const quint16 Error_TxPacket          = 0x0101; // Ошибка при оправке

// Байты пакета:
const quint8 Sync_Byte                = 0x5A; // Синхробайт
const quint8 Empty_Byte               = 0x00; // Empty

// Команды:
const quint8 Cmd_RHC                  = 0x01; // Read Hardware Configuration
const quint8 Cmd_RSC                  = 0x02; // Read Software Configuration
const quint8 Cmd_RMV                  = 0x04; // Read Measured Value
const quint8 Cmd_RRC                  = 0x05; // Read Real Clock
const quint8 Cmd_RCF                  = 0x06; // Read Calibration Factor
const quint8 Cmd_RSP                  = 0x0B; // Read Sensor Parameters
const quint8 Cmd_RCC                  = 0x0C; // Read CaLlendar coefficient
const quint8 Cmd_RML                  = 0x10; // Read Measured Log
const quint8 Cmd_REI                  = 0x1B; // Read External Interface Settings
const quint8 Cmd_RED                  = 0x1C; // Read External Device Settings
const quint8 Cmd_RFC                  = 0x1D; // Read Flow Coefficient
const quint8 Cmd_WSC                  = 0x20; // Write Software Configuration
const quint8 Cmd_WRC                  = 0x22; // Write Real Clock
const quint8 Cmd_WCC                  = 0x26; // Write Clock Calibration
const quint8 Cmd_WHC                  = 0x29; // Write Hardware Configuration
const quint8 Cmd_WEI                  = 0x2B; // Write External Interface Settings
const quint8 Cmd_WSP                  = 0x2D; // Write Sensor Parameters
const quint8 Cmd_WCL                  = 0x2E; // Write CaLlendar Coefficient
const quint8 Cmd_WTC                  = 0x36; // Write Temperature Calibration coefficient
const quint8 Cmd_TCC                  = 0x37; // Temperature Channel Calibration
const quint8 Cmd_ETS                  = 0x3B; // Energy Test
const quint8 Cmd_WED                  = 0x3C; // Write External Device Settings
const quint8 Cmd_WFC                  = 0x3D; // Write Flow Coefficient
const quint8 Cmd_WDT                  = 0x3E; // Write Differential Threshold
const quint8 Cmd_TSC                  = 0x3F; // RTD Sensor Calibration
const quint8 Cmd_PSS                  = 0x40; // Open/Close Password Session
const quint8 Cmd_PSC                  = 0x41; // Password Change
const quint8 Cmd_SDP                  = 0x42; // Set Default Password
const quint8 Cmd_DCM                  = 0x4C; // Disable Calibration Mode
const quint8 Cmd_RPS                  = 0x4F; // Read Password


// Текстовое представление команд:
const QString cmdText[127] = {
    "0x00", // 00
    "RHC",  // 01
    "RSC",  // 02
    "0x03", // 03
    "RMV",  // 04
    "RRC",  // 05
    "RCF",  // 06
    "0x07", "0x08", "0x09", "0x0A", // 07-0A
    "RSP",  // 0B
    "RCC",  // 0C
    "0x0D", "0x0E", "0x0F", // 0D-0F
    "RML",  // 10
    "0x11", "0x12", "0x13", "0x14", "0x15", "0x16", "0x17", "0x18", "0x19", "0x1A", // 11-1A
    "REI",  // 1B
    "RED",  // 1C
    "RFC",  // 1D
    "0x1E", "0x1F", // 1E-1F
    "WSC",  // 20
    "0x21", // 21
    "WRC",  // 22
    "0x23", "0x24", "0x25", // 23-25
    "WCC",  // 26
    "0x27", "0x28", // 27-28
    "WHC",  // 29
    "0x2A", // 2A
    "WEI",  // 2B
    "0x2C", // 2C
    "WSP",  // 2D
    "WCL",  // 2E
    "0x2F", "0x30",  "0x31", "0x32", "0x33", "0x34", "0x35", // 2F - 35
    "WTC",  // 36
    "TCC",  // 37
    "0x38", "0x39", "0x3A",  // 38 - 3A
    "ETS",  // 3B
    "WED",  // 3C
    "WFC",  // 3D
    "WDT",  // 3E
    "TSC",  // 3F
    "PSS",  // 40
    "PSC",  // 41
    "SDP",  // 42
    "0x43", "0x44", "0x45", "0x46", "0x47", "0x48", "0x49", "0x4A", "0x4B", // 43-4B
    "DCM",  // 4C
    "0x4D", "0x4E", // 4D-4E
    "RPS",  // 4F
    "0x50", "0x51", "0x52", "0x53", "0x54", "0x55", "0x56", "0x57", "0x58",
    "0x59", "0x5A", "0x5B", "0x5C", "0x5D", "0x5E", "0x5F", "0x60", "0x61", "0x62",
    "0x63", "0x64", "0x65", "0x66", "0x67", "0x68", "0x69", "0x6A", "0x6B", "0x6C",
    "0x6D", "0x6E", "0x6F", "0x70", "0x71", "0x72", "0x73", "0x74", "0x75", "0x76",
    "0x77", "0x78", "0x79", "0x7A", "0x7B", "0x7C", "0x7D", "0x7E"
};
//===================================================================================================

const quint16 Default_Password     = 1111;
const quint8 opDPS = 1;
const quint8 clDPS = 2;
const quint8 opPPS = 3;
const quint8 clPPS = 4;
const QString Str_Port_Send_Signal = QObject::tr("Порт %1: Отправка сигнала \"%2\".");
const QString Str_Port_DbgData     = QObject::tr("Порт %1: Данные пакета: %2");

//===================================================================================================

typedef struct HardwareConfigStruct {
    bool ok {false};
    quint16 device;
    quint16 version;
    quint16 buildNum;
    QString lcdType;
    quint32 serial;
    quint16 manCode;
    quint8 devType;
    quint8 dn;
    quint16 compCnt;
    QDate compDate;
    QTime compTime;
    quint64 pcbSN;
} HardwareConfigStruct;

//===================================================================================================

typedef struct SoftwareConfigStruct {
    bool ok {false};
    double flowMin;
    double flowMax;
    quint8 tInMin;
    quint8 tInMax;
    quint8 tOutMin;
    quint8 tOutMax;
    quint8 reportData;
    quint8 averageT;
    quint8 pulsesMode1;
    quint8 pulsesMode2;
    double pulsesValue1;
    double pulsesValue2;
    quint8 devType;
} SoftwareConfigStruct;

//===================================================================================================

typedef struct SensorParametersStruct {
    bool ok {false};
    double flowRate;
    quint8 pressure;
    quint8 rtdType;
    quint32 rRTD;
    double rWare1;
    double rWare2;
    //double flowNom;
    quint8 flowNomInt;
} SensorParametersStruct;

//===================================================================================================

typedef struct RealClockStruct {
    bool ok {false};
    QDateTime dateTime;
    quint8 flagSW;
    quint8 flagASW;
} RealClockStruct;

//===================================================================================================

typedef struct CalibrationFactorStruct {
    bool ok {false};
    qint16 rtcOffset;
    qint16 rtcTComp;
    quint16 fsDiffThrs;
    double rRef1;
    double rRef2;
    double shiftT1;
    double shiftT2;
    double scaleT1;
    double scaleT2;
    quint32 fsuOnly;
} CalibrationFactorStruct;

//===================================================================================================

typedef struct MeasureValueStruct {
    bool ok {false};
    double qMeasHour;
    double qSumm;
    double vMeasHour;
    double vSumm;
    double tMeasDir;
    double tMeasRev;
    quint32 errorFlags;
    quint64 specUse;
} MeasureValueStruct;

//===================================================================================================

typedef struct EnergyTestStruct {
    bool ok {false};
    double testVolume;
    double testEnergy;
    double tMeasDir;
    double tMeasRev;
} EnergyTestStruct;

//===================================================================================================

typedef struct TSCStruct {
    bool ok {false};
    quint8 firstRes;
    quint8 secondRes;
} TSCStruct;

//===================================================================================================

class Gefest: public QObject
{
    Q_OBJECT
public:
    explicit Gefest(QObject *parent, const PortSettingsStruct &settings);
    ~Gefest();
    void resetData();
    bool openPort(bool workEnable = true);
    bool closePort(bool onlyWorkDisable = true);
    bool startWork();
    bool stopWork();
    QString portName() { return port ? port->portName() : ""; }
    bool txMessage(quint8 cmd, const QByteArray &data = QByteArray());
    bool repeatMessage();
    bool isOpen(bool really = false);
    bool isWorkEnabled() { return isWorkMode; }
    quint8 cmd() { return currentCmd; }
    bool isDPS() { return isOpenDPS; }
    bool isPPS() { return isOpenPPS; }
    quint8 sessionType() { return currentSessionType; }
    bool correctMessage(QByteArray &data);
    bool openDPS(quint16 password = Default_Password);
    bool closeDPS(quint16 password = Default_Password);
    bool openPPS(quint16 password = Default_Password);
    bool closePPS(quint16 password = Default_Password);
    bool sendWHC( const HardwareConfigStruct &hardwareConfig );
    bool sendWSC( const SoftwareConfigStruct &softwareConfig, quint16 password = Default_Password );
    bool sendWSP(const SensorParametersStruct &sensorParameters);    
    bool send_PSS( quint8 sessionType, quint16 password = Default_Password );
    bool sendWRC( const RealClockStruct &realClockStruct, quint16 password = Default_Password );
    bool sendTCC( qreal rReal1, qreal rReal2 );
    bool sendTSC( quint8 calibType, qreal tReal );
    bool sendETS( qreal testVolume );
    bool setSettings(const PortSettingsStruct &settings) { return port ? port->setPortSettings(settings) : false; }

    void setTXData(const QByteArray &data = QByteArray()) { _dataTX=data; }
    void setTXHardwareConfigStruct(const HardwareConfigStruct &dataStruct) { _hcStructTX = dataStruct; }
    void setTXSoftwareConfigStruct(const SoftwareConfigStruct &dataStruct) { _scStructTX = dataStruct; }
    void setTXSensorParametersStruct(const SensorParametersStruct &dataStruct) { _spStructTX = dataStruct; }
    void setTXRealClockStruct(const RealClockStruct &dataStruct) { _rcStructTX = dataStruct; }

    //
    QByteArray dataResult() { return _data; }
    HardwareConfigStruct hardwareConfigResult() { return _hcStruct; }
    SoftwareConfigStruct softwareConfigResult() { return _scStruct; }
    MeasureValueStruct measureValueResult() { return _mvStruct; }
    RealClockStruct realClockResult() { return _rcStruct; }
    CalibrationFactorStruct calibrationFactorResult() { return _cfStruct; }
    SensorParametersStruct sensorParametersResult() { return _spStruct; }
    EnergyTestStruct energyTestResult() { return _etStruct; }
    TSCStruct tscResult() { return _tscStruct; }
    //
    static QString errorText(quint16 errorCode);
    static int timeOutForCmd(quint8 cmd);

#ifdef WIDGETS_SOFT
    static void fillComboBoxCommands(QComboBox *box, quint8 minCmd = 1, quint8 maxCmd = 127, quint8 defaultCmd = 0);

#endif

signals:
    void toLog(LogType logType, QString text);
    void selfRepeatMessage();
    void selfSendMessage(quint8 cmd);
    void selfSendWHC();
    void selfSendWSC();
    void selfSendWSP();
    void selfSendWRC();
    void selfSendTCC( qreal rReal1, qreal rReal2 );
    void selfSendTSC( quint8 calibType, qreal tReal );
    void selfSendETS( qreal testVolume );
    void selfSendPSS( quint8 sessionType );
    //
    void txBytes();
    void rxBytes();
    void error(quint16 errorCode);    
    void normalAnswer(quint8 cmd);

private:
    ComPort *port {nullptr};
    bool isWorkMode {false};
    bool waitAnswer {false};
    int sendCount {0};
    QTimer timer;
    QTimer timerDPS;
    QTimer timerPPS;

    QByteArray txData;
    QByteArray rxData;
    quint8 currentCmd {0};
    bool isOpenDPS {false};
    bool isOpenPPS {false};
    quint16 currentPasswordDPS {Default_Password};
    quint16 currentPasswordPPS {Default_Password};
    quint8 currentSessionType;
    bool firstTSC {true};    
    //
    QByteArray _data;
    HardwareConfigStruct _hcStruct;
    SoftwareConfigStruct _scStruct;
    MeasureValueStruct _mvStruct;
    RealClockStruct _rcStruct;
    CalibrationFactorStruct _cfStruct;
    SensorParametersStruct _spStruct;
    EnergyTestStruct _etStruct;
    TSCStruct _tscStruct;
    //
    bool txPacket();
    bool newByte(quint8 byte);
    void checkRxPacket();
    bool sendPSS( quint8 sessionType, quint16 password = Default_Password );
    //
    QByteArray _dataTX;
    HardwareConfigStruct _hcStructTX;
    SoftwareConfigStruct _scStructTX;
    SensorParametersStruct _spStructTX;
    RealClockStruct _rcStructTX;

public slots:
    void on_txMessage(quint8 cmd) { txMessage(cmd, _dataTX); }
    void on_openDPS() { openDPS(); }
    void on_closeDPS() { closeDPS(); }
    void on_openPPS() { openPPS(); }
    void on_closePPS() { closePPS(); }
    void on_sendWHC() { sendWHC(_hcStructTX); }
    void on_sendWSC() { sendWSC(_scStructTX); }
    void on_sendWSP() { sendWSP(_spStructTX); }
    void on_sendPSS( quint8 sessionType ) { send_PSS(sessionType); }
    void on_sendWRC() { sendWRC(_rcStructTX); }
    void on_sendTCC( qreal rReal1, qreal rReal2 ) { sendTCC(rReal1, rReal2); }
    void on_sendTSC( quint8 calibType, qreal tReal ) { sendTSC(calibType, tReal); }
    void on_sendETS( qreal testVolume ) { sendETS(testVolume); }
    void on_repeatMessage() { repeatMessage(); }

private slots:
    void on_readyRead();
    void on_Timer();
    void on_TimerDPS();
    void on_TimerPPS();
};

//===================================================================================================
} // namespace
#endif // GEFEST_H
