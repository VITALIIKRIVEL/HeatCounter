#include <QCoreApplication>
//
#include "Crypto"
#include "Convert"
#include "SysUtils"
//
#include "gefest.h"
//===================================================================================================
namespace gefest {
//===================================================================================================
Gefest::Gefest(QObject *parent, const PortSettingsStruct &settings): QObject(parent)
{
    port = new ComPort(this, false);
    port->setPortSettings(settings);
    connect(port, &ComPort::toLog, this, &Gefest::toLog);
    connect(port, &ComPort::readyRead, this, &Gefest::on_readyRead);
    connect(&timer, &QTimer::timeout, this, &Gefest::on_Timer);
    connect(&timerDPS, &QTimer::timeout, this, &Gefest::on_TimerDPS);
    connect(&timerPPS, &QTimer::timeout, this, &Gefest::on_TimerPPS);
    connect(this, &Gefest::selfRepeatMessage, this, &Gefest::on_repeatMessage);
    connect(this, &Gefest::selfSendMessage, this, &Gefest::on_txMessage);
    connect(this, &Gefest::selfSendWHC, this, &Gefest::on_sendWHC);
    connect(this, &Gefest::selfSendWSC, this, &Gefest::on_sendWSC);
    connect(this, &Gefest::selfSendWSP, this, &Gefest::on_sendWSP);
    connect(this, &Gefest::selfSendWRC, this, &Gefest::on_sendWRC);
    connect(this, &Gefest::selfSendTCC, this, &Gefest::on_sendTCC);
    connect(this, &Gefest::selfSendTSC, this, &Gefest::on_sendTSC);
    connect(this, &Gefest::selfSendETS, this, &Gefest::on_sendETS);
    connect(this, &Gefest::selfSendPSS, this, &Gefest::on_sendPSS);

    timerDPS.setInterval( 9500 );
    timerPPS.setInterval( 4500 );

    resetData();
}
//===================================================================================================
Gefest::~Gefest()
{
    timer.stop();
    if(port) {
        port->close();
        delete port;
    }
}
//===================================================================================================
void Gefest::resetData()
{
    _hcStruct.ok  = false;
    _scStruct.ok  = false;
    _mvStruct.ok  = false;
    _rcStruct.ok  = false;
    _cfStruct.ok  = false;
    _spStruct.ok  = false;
    _etStruct.ok  = false;
    _tscStruct.ok = false;
}
//===================================================================================================
bool Gefest::openPort(bool workEnable)
{
    if(!port) return false;
    if(port->isOpen()) {
        if(workEnable) isWorkMode = true;
        return true;
    }

    timer.stop();
    waitAnswer = false;
    if(port->open()) {
        port->clear();
        if(workEnable) isWorkMode = true;
    }
    return port->isOpen();
}
//===================================================================================================
bool Gefest::closePort(bool onlyWorkDisable)
{
    if(!port) return false;

    stopWork();
    timerDPS.stop();
    timerPPS.stop();
    isOpenDPS = false;
    isOpenPPS = false;

    if(onlyWorkDisable) return !isWorkMode;
    return port->close();
}
//===================================================================================================
bool Gefest::startWork()
{
    if(!port || !port->isOpen()) return false;
    isWorkMode = true;
    return isWorkMode;
}
//===================================================================================================
bool Gefest::stopWork()
{
    timer.stop();
    waitAnswer = false;
    isWorkMode = false;
    return !isWorkMode;
}
//===================================================================================================
QString Gefest::errorText(quint16 errorCode)
{
    switch(errorCode) {
    case Error_No:	              return tr("Нет ошибки.");
    case Error_Command:	          return tr("Неверная команда сообщения.");
    case Error_TimeOut:	          return tr("Сообщение не принято в течении 2 сек.");
    case Error_FlashAccess:	      return tr("Ошибка записи во внутреннюю EEPROM.");
    case Error_RealClock:	      return tr("Ошибка часов реального времени.");
    case Error_CalibrKey:	      return tr("Не включен режим \"Калибровка\".");
    case Error_OutOffRange:	      return tr("Выход параметров за допустимые границы.");
    case Error_PswTO:	          return tr("Парольная сессия закрыта теплосчетчиком.");
    case Error_PswEq:	          return tr("В команде PSS или PSC передан неверный пароль.");
    case Error_PswNotOpen:	      return tr("Парольная сессия не открыта.");

    case Error_TimeOutWaitAnswer: return tr("Таймаут ожидания ответа.");
    case Error_TxPacket:          return tr("Ошибка при отправке.");
    default:                      return tr("Неизвестная ошибка.");
    }
    return tr("Неизвестная ошибка.");
}
//===================================================================================================
#ifdef WIDGETS_SOFT
void Gefest::fillComboBoxCommands(QComboBox *box, quint8 minCmd, quint8 maxCmd, quint8 defaultCmd)
{
    if(!box) return;
    box->clear();

    if(minCmd < 1) minCmd = 1;
    if(maxCmd > 127) maxCmd = 127;

    for(auto cmd = minCmd; cmd <= maxCmd; ++cmd) {
        QString str = cmdText[cmd].trimmed();
        if(!str.isEmpty()) box->addItem( str, cmd );
    }

    for(auto i=0; i<box->count(); ++i) {
        if(box->itemData(i).toInt() == defaultCmd) {
            box->setCurrentIndex(i);
            break;
        }
    }
}
#endif
//===================================================================================================
int Gefest::timeOutForCmd(quint8 cmd)
{
    if((cmd == Cmd_TCC) or (cmd == Cmd_RMV)) return 8000;
    else if(cmd == Cmd_ETS) return 3000;
    else if(cmd == Cmd_TSC) return 25000;
    else if(cmd < Cmd_WSC) return 1000;
    else if(cmd < Cmd_PSS) return 1000;
    else if(cmd < 0x50) return 1000;
    return 2000;
}
//===================================================================================================
void Gefest::on_readyRead()
{
    if(!port || !isWorkMode || !waitAnswer || !port->isOpen()) return;
    emit rxBytes();

    QByteArray buf = port->readAll();
    for(auto i=0; i<buf.size(); ++i) {
        if(newByte(buf.at(i))) {

            timer.stop();
            waitAnswer = false;
            checkRxPacket();
            rxData.clear();
            return;
        }
    }
}
//===================================================================================================
void Gefest::checkRxPacket()
{
    // проверка полученного пакета
    quint8 cmd = rxData.at(3) - 0x80;
    QByteArray data = rxData.mid(4, rxData.size()-5 );
    quint8 errCode = static_cast<quint8>( data.at(0) );

    if((data.size() == 1) && ( errCode != Error_No ) &&
          ((currentCmd != Cmd_TSC) || (firstTSC) || ( (errCode != 0x40) && (errCode != 0x80) ))
            ) { // - код ошибки

        emit toLog(LogDbg, Str_Port_Send_Signal.arg(port->portName()).arg("error"));
        emit error( static_cast<quint16>( data.at(0) & 0xFF ) );
        return;
    }
    _data = data;
    QString dbgStr = "";

    switch (cmd) {
    case Cmd_RHC: {
        if(data.size() < 33) break;

        Value16bits v16;

        v16.val_byte[1] = data.at(0);
        v16.val_byte[0] = data.at(1);
        _hcStruct.device = Convert::bcdDecodeUnsigned( v16.val_uint );
        dbgStr += QString("Device=%1; ").arg( _hcStruct.device );

        v16.val_byte[1] = data.at(2);
        v16.val_byte[0] = data.at(3);
        _hcStruct.version = v16.val_uint;
        dbgStr += QString("Version=%1; ").arg( Convert::intToHex(_hcStruct.version,4,false).insert(2,'.') );

        v16.val_byte[1] = data.at(4);
        v16.val_byte[0] = data.at(5);
        _hcStruct.buildNum = v16.val_uint;
        dbgStr += QString("BuildNum=%1; ").arg( Convert::intToHex(_hcStruct.buildNum,4,false).insert(2,'.') );

        _hcStruct.lcdType = QString::fromLocal8Bit( data.mid(6,2) );
        dbgStr += QString("LCD_Type=%1; ").arg( _hcStruct.lcdType );

        Value32bits v32;

        v32.val_byte[3] = data.at(8);
        v32.val_byte[2] = data.at(9);
        v32.val_byte[1] = data.at(10);
        v32.val_byte[0] = data.at(11);
        _hcStruct.serial = v32.val_uint;
        dbgStr += QString("Serial=%1; ").arg( Convert::intToHex(_hcStruct.serial, 8, false) );

        v16.val_byte[1] = data.at(12);
        v16.val_byte[0] = data.at(13);
        _hcStruct.manCode = v16.val_uint;
        dbgStr += QString("ManCode=%1; ").arg( Convert::intToHex(_hcStruct.manCode, 4, true) );

        _hcStruct.devType = data.at(14);
        dbgStr += QString("DevType=%1; ").arg( Convert::intToHex( _hcStruct.devType, 2, true) );

        _hcStruct.dn = Convert::bcdDecodeUnsigned( static_cast<quint8>(data.at(15)) );
        dbgStr += QString("DN=%1; ").arg( _hcStruct.dn );

        v16.val_byte[1] = data.at(16);
        v16.val_byte[0] = data.at(17);
        _hcStruct.compCnt = v16.val_uint;
        dbgStr += QString("CompCnt=%1; ").arg( Convert::intToHex(_hcStruct.compCnt,3,false) );

        v16.val_byte[1] = data.at(20);
        v16.val_byte[0] = data.at(21);
        _hcStruct.compDate = QDate( Convert::bcdDecodeUnsigned(v16.val_uint),
                             Convert::bcdDecodeUnsigned(static_cast<quint8>(data.at(19))),
                             Convert::bcdDecodeUnsigned(static_cast<quint8>(data.at(18))) );
        dbgStr += QString("CompDate=%1; ").arg( _hcStruct.compDate.toString("dd.MM.yyyy") );

        _hcStruct.compTime = QTime( Convert::bcdDecodeUnsigned(static_cast<quint8>(data.at(22))),
                             Convert::bcdDecodeUnsigned(static_cast<quint8>(data.at(23))),
                             Convert::bcdDecodeUnsigned(static_cast<quint8>(data.at(24))) );
        dbgStr += QString("CompTime=%1; ").arg( _hcStruct.compTime.toString("HH:mm:ss") );

        Value64bits v64;

        v64.val_byte[7] = data.at(25);
        v64.val_byte[6] = data.at(26);
        v64.val_byte[5] = data.at(27);
        v64.val_byte[4] = data.at(28);
        v64.val_byte[3] = data.at(29);
        v64.val_byte[2] = data.at(30);
        v64.val_byte[1] = data.at(31);
        v64.val_byte[0] = data.at(32);
        _hcStruct.pcbSN = v64.val_uint;
        dbgStr += QString("PCB_SN=%1; ").arg( Convert::intToHex(_hcStruct.pcbSN, 16, false) );

        _hcStruct.ok = true;

        emit toLog(LogDbg, Str_Port_DbgData.arg(port->portName()).arg(dbgStr));
        emit normalAnswer(cmd);
    }
        break;
    case Cmd_RSC: {
        if(data.size() < 17) break;

        Value16bits v16;

        v16.val_byte[1] = data.at(0);
        v16.val_byte[0] = data.at(1);
        qreal d = v16.val_uint;
        _scStruct.flowMin = d * 0.001;
        dbgStr += QString("Flow_Min=%1; ").arg( Convert::doubleToStr(_scStruct.flowMin, 3) );

        v16.val_byte[1] = data.at(2);
        v16.val_byte[0] = data.at(3);
        d = v16.val_uint;
        _scStruct.flowMax = d * 0.001;
        dbgStr += QString("Flow_Max=%1; ").arg( Convert::doubleToStr(_scStruct.flowMax, 3) );

        _scStruct.tInMin = static_cast<quint8>(data.at(4));
        dbgStr += QString("Tin_Min=%1; ").arg( _scStruct.tInMin );

        _scStruct.tInMax = static_cast<quint8>(data.at(5));
        dbgStr += QString("Tin_Max=%1; ").arg( _scStruct.tInMax );

        _scStruct.tOutMin = static_cast<quint8>(data.at(6));
        dbgStr += QString("Tout_Min=%1; ").arg( _scStruct.tOutMin );

        _scStruct.tOutMax = static_cast<quint8>(data.at(7));
        dbgStr += QString("Tout_Max=%1; ").arg( _scStruct.tOutMax );

        _scStruct.reportData = static_cast<quint8>(data.at(8));
        dbgStr += QString("ReportData=%1; ").arg( _scStruct.reportData );

        _scStruct.averageT = static_cast<quint8>(data.at(9));
        dbgStr += QString("Average_T=%1; ").arg( _scStruct.averageT );

        _scStruct.pulsesMode1 = static_cast<quint8>(data.at(10));
        dbgStr += QString("PulsesMode1=%1; ").arg( _scStruct.pulsesMode1 );

        _scStruct.pulsesMode2 = static_cast<quint8>(data.at(11));
        dbgStr += QString("PulsesMode2=%1; ").arg( _scStruct.pulsesMode2 );

        v16.val_byte[1] = data.at(12);
        v16.val_byte[0] = data.at(13);
        d = v16.val_uint;
        _scStruct.pulsesValue1 = d * 0.0001;
        dbgStr += QString("PulsesValue1=%1; ").arg( Convert::doubleToStr(_scStruct.pulsesValue1, 4) );

        v16.val_byte[1] = data.at(14);
        v16.val_byte[0] = data.at(15);
        d = v16.val_uint;
        _scStruct.pulsesValue2 = d * 0.0001;
        dbgStr += QString("PulsesValue2=%1; ").arg( Convert::doubleToStr(_scStruct.pulsesValue2, 4) );

        _scStruct.devType = data.at(16);
        dbgStr += QString("DevType=%1; ").arg( Convert::intToHex( _scStruct.devType, 2, true) );

        _scStruct.ok = true;

        emit toLog(LogDbg, Str_Port_DbgData.arg(port->portName()).arg(dbgStr));
        emit normalAnswer(cmd);
    }
        break;
    case Cmd_RMV: {
        if(data.size() < 27) break;

        Value32bits v32;
        v32.val_byte[3] = data.at(0);
        v32.val_byte[2] = data.at(1);
        v32.val_byte[1] = data.at(2);
        v32.val_byte[0] = data.at(3);
        qreal d = v32.val_uint;
        _mvStruct.qMeasHour = d * 0.00001;
        dbgStr += QString("QMeasHous=%1; ").arg( Convert::doubleToStr(_mvStruct.qMeasHour, 5) );

        v32.val_byte[3] = data.at(4);
        v32.val_byte[2] = data.at(5);
        v32.val_byte[1] = data.at(6);
        v32.val_byte[0] = data.at(7);
        d = v32.val_uint;
        _mvStruct.qSumm = d * 0.00001;
        dbgStr += QString("QSumm=%1; ").arg( Convert::doubleToStr(_mvStruct.qSumm, 5) );

        Value16bits v16;
        v16.val_byte[1] = data.at(8);
        v16.val_byte[0] = data.at(9);
        d = v16.val_uint;
        _mvStruct.vMeasHour = d * 0.00001;
        dbgStr += QString("VMeasHour=%1; ").arg( Convert::doubleToStr(_mvStruct.vMeasHour, 5) );

        v32.val_byte[3] = data.at(10);
        v32.val_byte[2] = data.at(11);
        v32.val_byte[1] = data.at(12);
        v32.val_byte[0] = data.at(13);
        d = v32.val_uint;
        _mvStruct.vSumm = d * 0.00001;
        dbgStr += QString("VSumm=%1; ").arg( Convert::doubleToStr(_mvStruct.vSumm, 5) );

        v16.val_byte[1] = data.at(14);
        v16.val_byte[0] = data.at(15);
        d = v16.val_int;
        _mvStruct.tMeasDir = d * 0.01;
        dbgStr += QString("TMeasDir=%1; ").arg( Convert::doubleToStr(_mvStruct.tMeasDir, 2) );

        v16.val_byte[1] = data.at(16);
        v16.val_byte[0] = data.at(17);
        d = v16.val_int;
        _mvStruct.tMeasRev = d * 0.01;
        dbgStr += QString("TMeasRev=%1; ").arg( Convert::doubleToStr(_mvStruct.tMeasRev, 2) );

        v32.val_uint = 0;
        v32.val_byte[0] = data.at(18);
        v32.val_byte[1] = data.at(19);
        v32.val_byte[2] = data.at(20);
        _mvStruct.errorFlags = v32.val_uint;
        dbgStr += QString("ErrorFlags=%1; ").arg( Convert::intToHex(_mvStruct.errorFlags, 6, true) );

        Value32bits v64;
        v64.val_uint = 0;
        v64.val_byte[0] = data.at(21);
        v64.val_byte[1] = data.at(22);
        v64.val_byte[2] = data.at(23);
        v64.val_byte[3] = data.at(24);
        v64.val_byte[4] = data.at(25);
        v64.val_byte[5] = data.at(26);
        _mvStruct.specUse = v64.val_uint;
        dbgStr += QString("FSU_Only=%1; ").arg( Convert::intToHex(_mvStruct.specUse, 12, true) );

        _mvStruct.ok = true;

        emit toLog(LogDbg, Str_Port_DbgData.arg(port->portName()).arg(dbgStr));
        emit normalAnswer(cmd);
    }
        break;
    case Cmd_RRC: {
        if(data.size() < 9) break;

        _rcStruct.dateTime = QDateTime( QDate( static_cast<quint8>(data.at(5)) + 2000, data.at(4), data.at(3) ),
                                 QTime( data.at(0), data.at(1), data.at(2)) );
        _rcStruct.flagSW = data.at(7);
        _rcStruct.flagASW = data.at(8);

        dbgStr += QString("Hour=%1; ").arg( static_cast<quint8>(data.at(0)) );
        dbgStr += QString("Minute=%1; ").arg( static_cast<quint8>(data.at(1)) );
        dbgStr += QString("Second=%1; ").arg( static_cast<quint8>(data.at(2)) );
        dbgStr += QString("Day=%1; ").arg( static_cast<quint8>(data.at(3)) );
        dbgStr += QString("Month=%1; ").arg( static_cast<quint8>(data.at(4)) );
        dbgStr += QString("Year=%1; ").arg( 2000 + static_cast<quint8>(data.at(5)) );
        dbgStr += QString("DOW=%1; ").arg( static_cast<quint8>(data.at(6)) );
        dbgStr += QString("SW_flag=%1; ").arg( static_cast<quint8>(data.at(7)) );
        dbgStr += QString("ASW_flag=%1; ").arg( static_cast<quint8>(data.at(8)) );

        _rcStruct.ok = true;

        emit toLog(LogDbg, Str_Port_DbgData.arg(port->portName()).arg(dbgStr));
        emit normalAnswer(cmd);
    }
        break;
    case Cmd_RCF: {

        if(data.size() < 30) break;

        Value16bits v16;
        v16.val_byte[1] = data.at(0);
        v16.val_byte[0] = data.at(1);
        _cfStruct.rtcOffset = v16.val_int;
        dbgStr += QString("RTC_Offset=%1; ").arg( _cfStruct.rtcOffset );

        v16.val_byte[1] = data.at(2);
        v16.val_byte[0] = data.at(3);
        _cfStruct.rtcTComp = v16.val_int;
        dbgStr += QString("RTC_Tcomp=%1; ").arg( _cfStruct.rtcTComp );

        v16.val_byte[1] = data.at(4);
        v16.val_byte[0] = data.at(5);
        _cfStruct.fsDiffThrs = v16.val_uint;
        dbgStr += QString("FS_DiffThrs=%1; ").arg( _cfStruct.fsDiffThrs );

        Value32bits v32;
        v32.val_byte[3] = data.at(6);
        v32.val_byte[2] = data.at(7);
        v32.val_byte[1] = data.at(8);
        v32.val_byte[0] = data.at(9);
        qreal d = v32.val_uint;
        _cfStruct.rRef1 = d * 0.000001;
        dbgStr += QString("R_Ref1=%1; ").arg( Convert::doubleToStr(_cfStruct.rRef1, 6) );

        v32.val_byte[3] = data.at(10);
        v32.val_byte[2] = data.at(11);
        v32.val_byte[1] = data.at(12);
        v32.val_byte[0] = data.at(13);
        d = v32.val_uint;
        _cfStruct.rRef2 = d * 0.000001;
        dbgStr += QString("R_Ref2=%1; ").arg( Convert::doubleToStr(_cfStruct.rRef2, 6) );

        v16.val_byte[1] = data.at(14);
        v16.val_byte[0] = data.at(15);
        d = v16.val_int;
        _cfStruct.shiftT1 = d * 0.01;
        dbgStr += QString("Shift_T1=%1; ").arg( Convert::doubleToStr(_cfStruct.shiftT1, 2) );

        v16.val_byte[1] = data.at(16);
        v16.val_byte[0] = data.at(17);
        d = v16.val_int;
        _cfStruct.shiftT2 = d * 0.01;
        dbgStr += QString("Shift_T2=%1; ").arg( Convert::doubleToStr(_cfStruct.shiftT2, 2) );

        v32.val_byte[3] = data.at(18);
        v32.val_byte[2] = data.at(19);
        v32.val_byte[1] = data.at(20);
        v32.val_byte[0] = data.at(21);
        d = v32.val_uint;
        _cfStruct.scaleT1 = d * 0.000001;
        dbgStr += QString("Scale_T1=%1; ").arg( Convert::doubleToStr(_cfStruct.scaleT1, 6) );

        v32.val_byte[3] = data.at(22);
        v32.val_byte[2] = data.at(23);
        v32.val_byte[1] = data.at(24);
        v32.val_byte[0] = data.at(25);
        d = v32.val_uint;
        _cfStruct.scaleT2 = d * 0.000001;
        dbgStr += QString("Scale_T2=%1; ").arg( Convert::doubleToStr(_cfStruct.scaleT2, 6) );

        v32.val_byte[3] = data.at(26);
        v32.val_byte[2] = data.at(27);
        v32.val_byte[1] = data.at(28);
        v32.val_byte[0] = data.at(29);
        _cfStruct.fsuOnly = v32.val_uint;
        dbgStr += QString("FSU_Only=%1; ").arg( _cfStruct.fsuOnly );

        _cfStruct.ok = true;

        emit toLog(LogDbg, Str_Port_DbgData.arg(port->portName()).arg(dbgStr));
        emit normalAnswer(cmd);
    }
        break;
    case Cmd_RSP: {
        if(data.size() < 19) break;

        Value32bits v32;
        v32.val_byte[3] = data.at(0);
        v32.val_byte[2] = data.at(1);
        v32.val_byte[1] = data.at(2);
        v32.val_byte[0] = data.at(3);

        qreal d = v32.val_uint;
        _spStruct.flowRate = d * 0.001;
        dbgStr += QString("FlowRate=%1; ").arg( Convert::doubleToStr(_spStruct.flowRate, 2) );

        _spStruct.pressure = static_cast<quint8>(data.at(4));
        dbgStr += QString("Pressure=%1; ").arg( _spStruct.pressure );

        _spStruct.rtdType = static_cast<quint8>(data.at(5));
        dbgStr += QString("RTD_Type=%1; ").arg( _spStruct.rtdType );

        v32.val_byte[3] = data.at(6);
        v32.val_byte[2] = data.at(7);
        v32.val_byte[1] = data.at(8);
        v32.val_byte[0] = data.at(9);
        _spStruct.rRTD = v32.val_uint;
        dbgStr += QString("R_RTD=%1; ").arg( _spStruct.rRTD ); // - при записи * 1000, при чтении - нет

        v32.val_byte[3] = data.at(10);
        v32.val_byte[2] = data.at(11);
        v32.val_byte[1] = data.at(12);
        v32.val_byte[0] = data.at(13);
        d = v32.val_uint ;
        _spStruct.rWare1 = d * 0.01;
        dbgStr += QString("RWare1=%1; ").arg( Convert::doubleToStr(_spStruct.rWare1, 2) );

        v32.val_byte[3] = data.at(14);
        v32.val_byte[2] = data.at(15);
        v32.val_byte[1] = data.at(16);
        v32.val_byte[0] = data.at(17);
        d = v32.val_uint ;
        _spStruct.rWare2 = d * 0.01;
        dbgStr += QString("RWare2=%1; ").arg( Convert::doubleToStr(_spStruct.rWare2, 2) );

        _spStruct.flowNomInt = qBound((quint8)6, static_cast<quint8>(data.at(18)), (quint8)50 );
        dbgStr += QString("FlowNom=%1; ").arg( Convert::doubleToStr((double)_spStruct.flowNomInt * 0.1, 1) );

        _spStruct.ok = true;

        emit toLog(LogDbg, Str_Port_DbgData.arg(port->portName()).arg(dbgStr));
        emit normalAnswer(cmd);
    }
        break;
    case Cmd_RCC: { emit normalAnswer(cmd); } break;
    case Cmd_RML: { emit normalAnswer(cmd); } break;
    case Cmd_REI: { emit normalAnswer(cmd); } break;
    case Cmd_RED: { emit normalAnswer(cmd); } break;
    case Cmd_RFC: { emit normalAnswer(cmd); } break;
    case Cmd_WSC: { emit normalAnswer(cmd); } break;
    case Cmd_WRC: { emit normalAnswer(cmd); } break;
    case Cmd_WCC: { emit normalAnswer(cmd); } break;
    case Cmd_WHC: { emit normalAnswer(cmd); } break;
    case Cmd_WEI: { emit normalAnswer(cmd); } break;
    case Cmd_WSP: { emit normalAnswer(cmd); } break;
    case Cmd_WCL: { emit normalAnswer(cmd); } break;
    case Cmd_WTC: { emit normalAnswer(cmd); } break;
    case Cmd_TCC: { emit normalAnswer(cmd); } break;
    case Cmd_ETS: {
        if(data.size() < 10) break;

        Value16bits v16;
        v16.val_byte[1] = data.at(0);
        v16.val_byte[0] = data.at(1);
        qreal d = v16.val_uint;
        _etStruct.testVolume = d * 0.001;
        dbgStr += QString("TestVolume=%1; ").arg( Convert::doubleToStr(_etStruct.testVolume, 3) );

        Value32bits v32;
        v32.val_byte[3] = data.at(2);
        v32.val_byte[2] = data.at(3);
        v32.val_byte[1] = data.at(4);
        v32.val_byte[0] = data.at(5);
        d = v32.val_uint;
        _etStruct.testEnergy = d * 0.001;
        dbgStr += QString("TestEnergy=%1; ").arg( Convert::doubleToStr(_etStruct.testEnergy, 3) );

        v16.val_byte[1] = data.at(6);
        v16.val_byte[0] = data.at(7);
        d = v16.val_int;
        _etStruct.tMeasDir = d * 0.01;
        dbgStr += QString("TMeasDir=%1; ").arg( Convert::doubleToStr(_etStruct.tMeasDir, 2) );

        v16.val_byte[1] = data.at(8);
        v16.val_byte[0] = data.at(9);
        d = v16.val_int;
        _etStruct.tMeasRev = d * 0.01;
        dbgStr += QString("TMeasRev=%1; ").arg( Convert::doubleToStr(_etStruct.tMeasRev, 2) );

        _etStruct.ok = true;

        emit toLog(LogDbg, Str_Port_DbgData.arg(port->portName()).arg(dbgStr));
        emit normalAnswer(cmd);
    }
        break;
    case Cmd_WED: { emit normalAnswer(cmd); } break;
    case Cmd_WFC: { emit normalAnswer(cmd); } break;
    case Cmd_WDT: { emit normalAnswer(cmd); } break;
    case Cmd_TSC: {
        if(data.size() < 1) break;

        if(firstTSC) {
            _tscStruct.ok = false;
            _tscStruct.firstRes = data.at(0);
        }
        else {
           _tscStruct.secondRes = data.at(0);
           _tscStruct.ok = true;
        }

        dbgStr += QString("CalibRes=%1; ").arg( Convert::intToHex(data.at(0), 2, true) );
        emit toLog(LogDbg, Str_Port_DbgData.arg(port->portName()).arg(dbgStr));        
        emit normalAnswer(cmd);

        if(firstTSC) {
            firstTSC = false;
            if(port && port->isOpen()) {
                waitAnswer = true;
                timer.start( timeOutForCmd(Cmd_TSC) );
            }
        }

    }
        break;
    case Cmd_PSS: { emit normalAnswer(cmd); } break;
    case Cmd_PSC: { emit normalAnswer(cmd); } break;
    case Cmd_SDP: { emit normalAnswer(cmd); } break;
    case Cmd_DCM: { emit normalAnswer(cmd); } break;
    case Cmd_RPS: { emit normalAnswer(cmd); } break;
    default:
        emit normalAnswer(cmd);
        break;
    }
}
//===================================================================================================
void Gefest::on_Timer()
{
    timer.stop();
    if(!port || !port->isOpen() || (currentCmd == 0)) return;

    emit toLog(LogError, tr("Порт %1: Тайм-аут ожидания ответа на команду %2.").arg(port->portName()).arg(cmdText[currentCmd]));
    if( ((currentCmd == Cmd_TSC) && !firstTSC) || (++sendCount > 2)) {
        waitAnswer = false;
        emit error(Error_TimeOutWaitAnswer);
        return;
    }
    emit toLog(LogInfo, tr("Порт %1: Повторная отправка команды %2.").arg(port->portName()).arg(cmdText[currentCmd]));
    if(!txPacket()) {
        waitAnswer = false;
        emit error(Error_TxPacket);
        return;
    }
}
//===================================================================================================
bool Gefest::txPacket()
{
    if(!port || !port->isOpen() || !isWorkMode || (currentCmd == 0)) return false;

    rxData.clear();
    port->clear();
    qint64 len = port->write(txData);
    emit txBytes();
    if(len == txData.size()) {        
        waitAnswer = true;
        timer.start();
        return true;
    }
    return false;
}
//===================================================================================================
bool Gefest::txMessage(quint8 cmd, const QByteArray &data)
{
    if(!isWorkMode || (cmd == 0)) return false;
    timer.stop();
    waitAnswer = false;
    sendCount = 0;
    currentCmd = cmd;

    txData.clear();
    txData.append( Sync_Byte );
    txData.append( Empty_Byte );
    txData.append( static_cast<quint8>(data.size() + 5) );
    txData.append( currentCmd );
    if(!data.isNull() && !data.isEmpty()) txData.append( data );
    txData.append( Crypto::crc8_Dallas(txData.mid(1)) );

    timer.setInterval( timeOutForCmd(currentCmd) );

    if(port) {
        emit toLog(LogInfo, tr("Порт %1: Отправка команды %2.").arg(port->portName()).arg(cmdText[currentCmd]));
    }
    return txPacket();
}
//===================================================================================================
bool Gefest::isOpen(bool really)
{
    if(!port) return false;
    return really ? port->isOpen() : isWorkMode;
}
//===================================================================================================
bool Gefest::repeatMessage()
{
    if(!isWorkMode || (currentCmd == 0)) return false;
    timer.stop();
    waitAnswer = false;

    if(port) {
        emit toLog(LogInfo, tr("Порт %1: Отправка команды %2.").arg(port->portName()).arg(cmdText[currentCmd]));
    }
    return txPacket();
}
//===================================================================================================
bool Gefest::newByte(quint8 byte)
{
    if(rxData.isEmpty() ) { // - ожидание синхробайта

        if(byte == Sync_Byte) rxData.append(byte);
        return false;
    }
    else if(rxData.size() == 1 ) { // - ожидается пустой байт

        if(byte == Empty_Byte) rxData.append(byte);
        else rxData.clear();
        return false;
    }
    else if(txData.size() == 2 ) { // - ожидается байт длины

        if(byte < 6) {
            rxData.clear();
            emit toLog(LogError, tr("Порт %1: Получен неверный байт длины: %2.")
                       .arg( port->portName() )
                       .arg( Convert::intToHex( byte, 2, true ) )
                       );
        }
        else rxData.append(byte);
        return false;
    }
    else if(rxData.size() == 3 ) { // - ожидается байт команды

        if((byte < 0x80) || ((byte - 0x80) != currentCmd)) {
            rxData.clear();
            emit toLog(LogError, tr("Порт %1: Получен неверный байт команды: %2. Ожидается: %3.")
                       .arg( port->portName() )
                       .arg( Convert::intToHex( byte, 2, true ) )
                       .arg( Convert::intToHex( currentCmd + 0x80, 2, true ) )
                       );
        }
        else rxData.append(byte);
        return false;
    }
    else if(rxData.size() == rxData.at(2)-1) { // crc

        quint8 crc = Crypto::crc8_Dallas(rxData.mid(1));
        if(crc == byte) { // полный пакет
            rxData.append(byte);
            return true;
        }
        else { // crc error!
            rxData.clear();
            emit toLog(LogError, tr("Порт %1: Получен неверный байт CRC: %2. Ожидается: %3.")
                       .arg( port->portName() )
                       .arg( Convert::intToHex( byte, 2, true ) )
                       .arg( Convert::intToHex( crc, 2, true ) )
                       );
        }
        return false;
    }
    else {
        rxData.append(byte);
    }
    return false;
}
//===================================================================================================
bool Gefest::correctMessage(QByteArray &data)
{
    while((data.size() > 5) && (static_cast<quint8>(data.at(0)) != Sync_Byte) &&
          (static_cast<quint8>(data.at(1)) != Empty_Byte)) {
        data.remove(0,1);
    }

    if(data.size() < 6) {
        emit toLog(LogError, tr("Порт %1: Неверный размер пакета данных: %2.")
                   .arg( port->portName() )
                   .arg( data.size() )
                   );
        return false;
    }

    if(static_cast<quint8>(data.at(0)) != Sync_Byte) {
        emit toLog(LogError, tr("Порт %1: Не найден синхробайт.")
                   .arg( port->portName() )
                   );
        return false;
    }
    else if(static_cast<quint8>(data.at(1)) != Empty_Byte) {
        emit toLog(LogError, tr("Порт %1: Неверный формат пакета данных.")
                   .arg( port->portName() )
                   );
        return false;
    }
    else if(static_cast<quint8>(data.at(2)) < 6) {
        emit toLog(LogError, tr("Порт %1: Неверный байт длины: %2.")
                   .arg( port->portName() )
                   .arg( Convert::intToHex( static_cast<quint8>(data.at(2)), 2, true ) )
                   );
        return false;
    }
    else if( static_cast<quint8>(data.at(2)) > data.size() ) {
        emit toLog(LogError, tr("Порт %1: Неполный пакет данных.")
                   .arg( port->portName() )
                   );
        return false;
    }

    int len = static_cast<quint8>(data.at(2));
    if(data.size() > len) data.remove( len, data.size()-len );

    quint8 crc = Crypto::crc8_Dallas(data.mid(1, data.size()-2));
    if(crc != static_cast<quint8>(data.at( data.size()-1 ))) {
        emit toLog(LogError, tr("Порт %1: Неверный байт CRC: %2. Ожидается: %3.")
                   .arg( port->portName() )
                   .arg( Convert::intToHex( static_cast<quint8>(data.at( data.size()-1 )), 2, true ) )
                   .arg( Convert::intToHex( crc, 2, true ) )
                   );
        return false;
    }
    return true;
}
//===================================================================================================
bool Gefest::openDPS(quint16 password)
{
    return sendPSS(opDPS, password);
}
//===================================================================================================
bool Gefest::closeDPS(quint16 password)
{
    return sendPSS(clDPS, password);
}
//===================================================================================================
bool Gefest::openPPS(quint16 password)
{
    return sendPSS(opPPS, password);
}
//===================================================================================================
bool Gefest::closePPS(quint16 password)
{
    return sendPSS(clPPS, password);
}
//===================================================================================================
bool Gefest::send_PSS(quint8 sessionType, quint16 password)
{
    if(!port || !isWorkMode) return false;
    if(!port->isOpen()) return false;

    QByteArray data;
    QString logText = "";
    switch (sessionType) {
    case opDPS:
        currentPasswordDPS = password;
        logText = tr("Открытие парольной сессии");
        break;
    case clDPS:
        currentPasswordDPS = password;
        logText = tr("Закрытие парольной сессии");
        break;
    case opPPS:
        currentPasswordPPS = password;
        logText = tr("Открытие парольной сессии");
        break;
    case clPPS:
        currentPasswordPPS = password;
        logText = tr("Закрытие парольной сессии");
        break;
    default:
        return false;
        break;
    }
    currentSessionType = sessionType;

    data.append(sessionType);
    data.append(Convert::hi(password));
    data.append(Convert::lo(password));

    bool res = txMessage(Cmd_PSS, data);
    emit toLog(LogDbg, Str_Port_DbgData.arg(port->portName()).arg(logText));

    if(res) {
        switch (sessionType) {
        case opDPS:
            timerDPS.stop();
            isOpenDPS = true;
            timerDPS.start();
            break;
        case clDPS:
            timerDPS.stop();
            isOpenDPS = false;
            break;
        case opPPS:
            timerPPS.stop();
            isOpenPPS = true;
            timerPPS.start();
            break;
        case clPPS:
            timerPPS.stop();
            isOpenPPS = false;
            break;
        default:
            return false;
            break;
        }
    }

    return res;
}
//===================================================================================================
bool Gefest::sendPSS(quint8 sessionType, quint16 password)
{
    if(!port || !isWorkMode) return false;
    if(!port->isOpen()) return false;

    QString logText = "";
    switch (sessionType) {
    case opDPS:
        if(isOpenDPS && (currentPasswordDPS == password)) return true;
        currentPasswordDPS = password;
        logText = tr("Открытие");
        break;
    case clDPS:
        if(!isOpenDPS && (currentPasswordDPS == password)) return true;
        currentPasswordDPS = password;
        logText = tr("Закрытие");
        break;
    case opPPS:
        if(isOpenPPS && (currentPasswordPPS == password)) return true;
        currentPasswordPPS = password;
        logText = tr("Открытие");
        break;
    case clPPS:
        if(!isOpenDPS && (currentPasswordPPS == password)) return true;
        currentPasswordPPS = password;
        logText = tr("Закрытие");
        break;
    default:
        return false;
        break;
    }

    currentSessionType = sessionType;

    timer.stop();
    waitAnswer = false;
    sendCount = 0;
    currentCmd = Cmd_PSS;
    txData.clear();
    txData.append( Sync_Byte);
    txData.append( Empty_Byte);
    txData.append( 8 );
    txData.append( currentCmd);
    txData.append(sessionType);
    txData.append(Convert::hi(password));
    txData.append(Convert::lo(password));
    txData.append(Crypto::crc8_Dallas(txData.mid(1)));

    int cnt = 3;
    while(cnt > 0) {
        cnt--;
        if(cnt < 2) System::pause(10);
        rxData.clear();
        port->clear();
        emit toLog(LogInfo, tr("Порт %1: %2 парольной сессии.").arg(port->portName()).arg(logText));
        qint64 len = port->write(txData);
        emit txBytes();
        if(len != txData.size()) continue;

        bool ok = false;
        int n = 0;
        while(!ok && (n<10)) {
            System::pauseMS( (n==0) ? 600 : 300 );
            rxData.append( port->readAll() );
            emit rxBytes();
            ok = correctMessage(rxData);
            n++;
        }        

        if(!ok) continue;
        if(( static_cast<quint8>(rxData.at(3)) - 0x80) != Cmd_PSS) continue;

        _data = rxData.mid(4, rxData.size()-5 );

        if((_data.size() == 1) && ( static_cast<quint8>( _data.at(0) ) != Error_No )) { // - код ошибки

            emit toLog(LogDbg, Str_Port_Send_Signal.arg(port->portName()).arg("error"));
            if(cnt == 0) emit error( static_cast<quint16>( _data.at(0) & 0xFF ) );
            continue;
        }

        if((_data.size() == 1) && ( static_cast<quint8>( _data.at(0) ) == Error_No )) {

            switch (sessionType) {
            case opDPS:
                timerDPS.stop();
                isOpenDPS = true;
                timerDPS.start();
                break;
            case clDPS:
                timerDPS.stop();
                isOpenDPS = false;
                break;
            case opPPS:
                timerPPS.stop();
                isOpenPPS = true;
                timerPPS.start();
                break;
            case clPPS:
                timerPPS.stop();
                isOpenPPS = false;
                break;
            default:
                return false;
                break;
            }
            return true;
        }
    }
    return false;
}
//===================================================================================================
bool Gefest::sendWHC(const HardwareConfigStruct &hardwareConfig)
{
    if(!port || !port->isOpen() || !isWorkMode) return false;

    QByteArray data;
    QString dbgStr = "";

    Value32bits v32;
    v32.val_uint = hardwareConfig.serial;
    data.append( v32.val_byte[3] );
    data.append( v32.val_byte[2] );
    data.append( v32.val_byte[1] );
    data.append( v32.val_byte[0] );
    dbgStr += QString("Serial=%1; ").arg( Convert::intToHex(hardwareConfig.serial, 8, false) );

    data.append( Convert::hi(hardwareConfig.manCode) );
    data.append( Convert::lo(hardwareConfig.manCode) );
    dbgStr += QString("ManCode=%1; ").arg( Convert::intToHex( hardwareConfig.manCode, 4, true) );

    data.append(hardwareConfig.devType);
    dbgStr += QString("DevType=%1; ").arg( Convert::intToHex( hardwareConfig.devType, 2, true) );

    data.append(Convert::bcdEncode(hardwareConfig.dn));
    dbgStr += QString("DN=%1; ").arg( hardwareConfig.dn );

    Value64bits v64;
    v64.val_uint = hardwareConfig.pcbSN;

    data.append( v64.val_byte[7] );
    data.append( v64.val_byte[6] );
    data.append( v64.val_byte[5] );
    data.append( v64.val_byte[4] );
    data.append( v64.val_byte[3] );
    data.append( v64.val_byte[2] );
    data.append( v64.val_byte[1] );
    data.append( v64.val_byte[0] );
    dbgStr += QString("PCB_SN=%1; ").arg( Convert::intToHex( hardwareConfig.pcbSN, 16, false) );

    bool res = txMessage(Cmd_WHC, data);
    emit toLog(LogDbg, Str_Port_DbgData.arg(port->portName()).arg(dbgStr));
    return res;
}
//===================================================================================================
bool Gefest::sendWSC( const SoftwareConfigStruct &softwareConfig, quint16 password )
{
    if(!port || !port->isOpen() || !isWorkMode) return false;
    if(!sendPSS(opDPS, password)) return false;

    QByteArray data;
    QString dbgStr = "";

    quint16 val = softwareConfig.flowMin * 1000;
    data.append( Convert::hi(val) );
    data.append( Convert::lo(val) );
    dbgStr += QString("FlowMin=%1; ").arg( Convert::doubleToStr(softwareConfig.flowMin, 3) );

    val = softwareConfig.flowMax * 1000;
    data.append( Convert::hi(val) );
    data.append( Convert::lo(val) );
    dbgStr += QString("FlowMax=%1; ").arg( Convert::doubleToStr(softwareConfig.flowMax, 3) );

    data.append( softwareConfig.tInMin );
    dbgStr += QString("Tin_Min=%1; ").arg( softwareConfig.tInMin );

    data.append( softwareConfig.tInMax );
    dbgStr += QString("Tin_Max=%1; ").arg( softwareConfig.tInMax );

    data.append( softwareConfig.tOutMin );
    dbgStr += QString("Tout_Min=%1; ").arg( softwareConfig.tOutMin );

    data.append( softwareConfig.tOutMax );
    dbgStr += QString("Tout_Max=%1; ").arg( softwareConfig.tOutMax );

    data.append( softwareConfig.reportData );
    dbgStr += QString("ReportData=%1; ").arg( softwareConfig.reportData );

    data.append( softwareConfig.averageT );
    dbgStr += QString("AverageT=%1; ").arg( softwareConfig.averageT );

    data.append( softwareConfig.pulsesMode1 );
    dbgStr += QString("PulsesMode1=%1; ").arg( softwareConfig.pulsesMode1 );

    data.append( softwareConfig.pulsesMode2 );
    dbgStr += QString("PulsesMode2=%1; ").arg( softwareConfig.pulsesMode2 );

    val = softwareConfig.pulsesValue1 * 10000;
    data.append( Convert::hi(val) );
    data.append( Convert::lo(val) );
    dbgStr += QString("PulsesValue1=%1; ").arg( Convert::doubleToStr(softwareConfig.pulsesValue1, 4) );

    val = softwareConfig.pulsesValue2 * 10000;
    data.append( Convert::hi(val) );
    data.append( Convert::lo(val) );
    dbgStr += QString("PulsesValue2=%1; ").arg( Convert::doubleToStr(softwareConfig.pulsesValue2, 4) );

    data.append( softwareConfig.devType );
    dbgStr += QString("DevType=%1; ").arg( Convert::intToHex( softwareConfig.devType, 2, true) );

    bool res = txMessage(Cmd_WSC, data);
    emit toLog(LogDbg, Str_Port_DbgData.arg(port->portName()).arg(dbgStr));
    return res;
}
//===================================================================================================
bool Gefest::sendWSP( const SensorParametersStruct &sensorParameters )
{
    if(!port || !port->isOpen() || !isWorkMode) return false;

    QByteArray data;
    QString dbgStr = "";    

    Value32bits v32;
    v32.val_uint = sensorParameters.flowRate * 1000;
    data.append( v32.val_byte[3] );
    data.append( v32.val_byte[2] );
    data.append( v32.val_byte[1] );
    data.append( v32.val_byte[0] );
    dbgStr += QString("FlowRate=%1; ").arg( Convert::doubleToStr( sensorParameters.flowRate, 2 ) );

    data.append( sensorParameters.pressure );
    dbgStr += QString("Pressure=%1; ").arg( sensorParameters.pressure );

    data.append( sensorParameters.rtdType );
    dbgStr += QString("RTD_Type=%1; ").arg( sensorParameters.rtdType );

    v32.val_uint = sensorParameters.rRTD * 1000; // - при записи * 1000, при чтении - нет
    data.append( v32.val_byte[3] );
    data.append( v32.val_byte[2] );
    data.append( v32.val_byte[1] );
    data.append( v32.val_byte[0] );
    dbgStr += QString("R_RTD=%1; ").arg( sensorParameters.rRTD );

    v32.val_uint = sensorParameters.rWare1 * 100;
    data.append( v32.val_byte[3] );
    data.append( v32.val_byte[2] );
    data.append( v32.val_byte[1] );
    data.append( v32.val_byte[0] );
    dbgStr += QString("RWire1=%1; ").arg( Convert::doubleToStr(sensorParameters.rWare1,2) );

    v32.val_uint = sensorParameters.rWare2 * 100;
    data.append( v32.val_byte[3] );
    data.append( v32.val_byte[2] );
    data.append( v32.val_byte[1] );
    data.append( v32.val_byte[0] );
    dbgStr += QString("RWire2=%1; ").arg( Convert::doubleToStr(sensorParameters.rWare2,2) );

    dbgStr += QString("FlowNom=%1; ").arg( Convert::doubleToStr( (double)sensorParameters.flowNomInt * 0.1, 1 ) );
    data.append( sensorParameters.flowNomInt );

    bool res = txMessage(Cmd_WSP, data);
    emit toLog(LogDbg, Str_Port_DbgData.arg(port->portName()).arg(dbgStr));
    return res;
}
//===================================================================================================
bool Gefest::sendWRC( const RealClockStruct &realClockStruct, quint16 password )
{
    if(!port || !port->isOpen() || !isWorkMode) return false;
    if(!sendPSS(opDPS, password)) return false;

    QByteArray data;
    QString dbgStr = "";

    data.append( realClockStruct.dateTime.time().hour() );
    dbgStr += QString("Hour=%1; ").arg( static_cast<quint8>(data.at(0)) );

    data.append( static_cast<quint8>(realClockStruct.dateTime.time().minute()) );
    dbgStr += QString("Minute=%1; ").arg( static_cast<quint8>(data.at(1)) );

    data.append( static_cast<quint8>(realClockStruct.dateTime.time().second()) );
    dbgStr += QString("Second=%1; ").arg( static_cast<quint8>(data.at(2)) );

    data.append( static_cast<quint8>(realClockStruct.dateTime.date().day()) );
    dbgStr += QString("Day=%1; ").arg( static_cast<quint8>(data.at(3)) );

    data.append( static_cast<quint8>(realClockStruct.dateTime.date().month()) );
    dbgStr += QString("Month=%1; ").arg( static_cast<quint8>(data.at(4)) );

    data.append( static_cast<quint8>( realClockStruct.dateTime.date().year() - 2000 ) );
    dbgStr += QString("Year=%1; ").arg( static_cast<quint8>(data.at(5)) + 2000 );

    data.append( static_cast<quint8>( realClockStruct.dateTime.date().dayOfWeek() ) );
    dbgStr += QString("DOW=%1; ").arg( static_cast<quint8>(data.at(6)) );

    data.append( realClockStruct.flagASW );
    dbgStr += QString("ASW_flag=%1; ").arg( static_cast<quint8>(data.at(7)) );

    bool res = txMessage(Cmd_WRC, data);
    emit toLog(LogDbg, Str_Port_DbgData.arg(port->portName()).arg(dbgStr));
    return res;
}
//===================================================================================================
bool Gefest::sendTCC(qreal rReal1, qreal rReal2)
{
    if(!port || !port->isOpen() || !isWorkMode) return false;

    QByteArray data;
    QString dbgStr = "";

    Value32bits v32;
    v32.val_uint = rReal1 * 1000;
    data.append( v32.val_byte[3] );
    data.append( v32.val_byte[2] );
    data.append( v32.val_byte[1] );
    data.append( v32.val_byte[0] );
    dbgStr += QString("R_Real1=%1; ").arg( Convert::doubleToStr( rReal1, 3 ) );

    v32.val_uint = rReal2 * 1000;
    data.append( v32.val_byte[3] );
    data.append( v32.val_byte[2] );
    data.append( v32.val_byte[1] );
    data.append( v32.val_byte[0] );
    dbgStr += QString("R_Real2=%1; ").arg( Convert::doubleToStr( rReal2, 3 ) );

    bool res = txMessage(Cmd_TCC, data);
    emit toLog(LogDbg, Str_Port_DbgData.arg(port->portName()).arg(dbgStr));
    return res;
}
//===================================================================================================
bool Gefest::sendTSC( quint8 calibType, qreal tReal )
{
    if(!port || !port->isOpen() || !isWorkMode) return false;

    QByteArray data;
    QString dbgStr = "";

    data.append( calibType );
    dbgStr += QString("CalibType=%1; ").arg( calibType );

    Value32bits v16;
    v16.val_uint = tReal * 100;
    data.append( v16.val_byte[1] );
    data.append( v16.val_byte[0] );
    dbgStr += QString("T_Real=%1; ").arg( Convert::doubleToStr( tReal, 2 ) );

    firstTSC = true;
    bool res = txMessage(Cmd_TSC, data);
    emit toLog(LogDbg, Str_Port_DbgData.arg(port->portName()).arg(dbgStr));
    return res;
}
//===================================================================================================
bool Gefest::sendETS( qreal testVolume )
{
    if(!port || !port->isOpen() || !isWorkMode) return false;

    QByteArray data;
    QString dbgStr = "";

    Value32bits v16;
    v16.val_uint = testVolume * 1000;
    data.append( v16.val_byte[1] );
    data.append( v16.val_byte[0] );
    dbgStr += QString("TestVolume=%1; ").arg( Convert::doubleToStr( testVolume, 3 ) );

    bool res = txMessage(Cmd_ETS, data);
    emit toLog(LogDbg, Str_Port_DbgData.arg(port->portName()).arg(dbgStr));
    return res;
}
//===================================================================================================
void Gefest::on_TimerDPS()
{
    timerDPS.stop();
    isOpenDPS = false;
}
//===================================================================================================
void Gefest::on_TimerPPS()
{
    timerPPS.stop();
    isOpenPPS = false;
}
//===================================================================================================
} // namespace
