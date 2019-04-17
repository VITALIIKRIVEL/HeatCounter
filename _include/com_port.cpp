#include <QCoreApplication>
#include <QSerialPortInfo>
#include <QSettings>
//
#include "convert.h"
#include "system_utils.h"
//
#include "com_port.h"

namespace nayk {
//=======================================================================================================

//=======================================================================================================
ComPort::ComPort(QObject *parent, bool autoRead) : QObject(parent)
{
    _autoRead = autoRead;
    connect(&port, &QSerialPort::readyRead, this, &ComPort::on_ReadyRead);
    connect(&port, &QSerialPort::readyRead, this, &ComPort::readyRead);
    connect(&port, &QSerialPort::errorOccurred, this, &ComPort::on_Error);
    connect(&port, &QSerialPort::errorOccurred, this, &ComPort::errorOccurred);
}
//=======================================================================================================
ComPort::~ComPort()
{
    close();
}
//=======================================================================================================
void ComPort::on_Error(QSerialPort::SerialPortError error)
{
    switch (error) {
    case QSerialPort::DeviceNotFoundError:
        emit toLog(LogError, tr("Порт %1: Устройство не найдено.").arg(port.portName()));
        break;
    case QSerialPort::PermissionError:
        emit toLog(LogError, tr("Порт %1: Ошибка доступа.").arg(port.portName()));
        break;
    case QSerialPort::OpenError:
        emit toLog(LogError, tr("Порт %1: Ошибка при открытии.").arg(port.portName()));
        break;
    case QSerialPort::ParityError:
        emit toLog(LogError, tr("Порт %1: Ошибка четности.").arg(port.portName()));
        break;
    case QSerialPort::FramingError:
        emit toLog(LogError, tr("Порт %1: Ошибка кадрирования.").arg(port.portName()));
        break;
    case QSerialPort::BreakConditionError:
        emit toLog(LogError, tr("Порт %1: Ошибка условия прерывания.").arg(port.portName()));
        break;
    case QSerialPort::WriteError:
        emit toLog(LogError, tr("Порт %1: Ошибка записи.").arg(port.portName()));
        break;
    case QSerialPort::ReadError:
        emit toLog(LogError, tr("Порт %1: Ошибка чтения.").arg(port.portName()));
        break;
    case QSerialPort::ResourceError:
        emit toLog(LogError, tr("Порт %1: Ошибка ресурса.").arg(port.portName()));
        break;
    case QSerialPort::UnsupportedOperationError:
        emit toLog(LogError, tr("Порт %1: Неподдерживаемая операция.").arg(port.portName()));
        break;
    case QSerialPort::UnknownError:
        emit toLog(LogError, tr("Порт %1: Неизвестная ошибка.").arg(port.portName()));
        break;
    case QSerialPort::TimeoutError:
        emit toLog(LogError, tr("Порт %1: Таймаут.").arg(port.portName()));
        break;
    case QSerialPort::NotOpenError:
        emit toLog(LogError, tr("Порт %1: Не открыт.").arg(port.portName()));
        break;
    default:
        break;
    }
}
//=======================================================================================================
void ComPort::on_ReadyRead()
{
    if(!_autoRead) return;
    QByteArray buf = readAll();
    if(!buf.isEmpty()) emit rxBytes(buf);
}
//=======================================================================================================
QByteArray ComPort::readAll()
{
    QByteArray buf;
    if(!port.isOpen()) {
        _lastError = tr("Порт %1: Ошибка чтения - порт закрыт.").arg(port.portName());
        emit toLog(LogError, _lastError);
        return buf;
    }
    buf = port.readAll();
    if(!buf.isEmpty()) {
        QString str = "";
        for(auto i=0; i<buf.size(); ++i) str += Convert::intToHex( static_cast<quint8>( buf.at(i) ), 2, false ) + " ";
        emit toLog(LogIn, tr("Порт %1: %2").arg(port.portName()).arg(str));
    }
    return buf;
}
//=======================================================================================================
qint64 ComPort::write(const QByteArray &data)
{
    if(!port.isOpen()) {
        _lastError = tr("Порт %1: Ошибка записи - порт закрыт.").arg(port.portName());
        emit toLog(LogError, _lastError);
        return 0;
    }

    qint64 startIndx = 0;
    int n = 0;
    qint64 dataLen = 0;

    while( (dataLen < data.size()) && (n<4)) {
        qint64 len = port.write(data.right( data.size() - startIndx ));
        if(len < 0) {
            _lastError = tr("Порт %1: Ошибка записи. %2.").arg(port.portName().arg(port.errorString()));
            emit toLog(LogError, _lastError);
            return dataLen;
        }
        else if(len > 0) {
            QString str = "";
            for(auto i=0; i<len; ++i) str += Convert::intToHex( static_cast<quint8>( data.at(startIndx+i) ), 2, false ) + " ";
            emit toLog(LogOut, tr("Порт %1: %2").arg(port.portName()).arg(str));
        }
        dataLen += len;
        startIndx = startIndx + len;
    }

    return dataLen;
}
//=======================================================================================================
bool ComPort::open(QIODevice::OpenMode openMode)
{
    if(port.isOpen() && (port.openMode() == openMode)) return true;
    if(!close()) return false;
    emit beforeOpen();
    if(!port.open( openMode )) {
        _lastError = tr("Порт %1: Ошибка при открытии. %2.").arg( port.portName() ).arg( port.errorString());
        emit toLog(LogError, _lastError);
        return false;
    }
    emit afterOpen();
    emit toLog(LogInfo, tr("Порт %1: Открыт.").arg(port.portName()) );
    return true;
}
//=======================================================================================================
bool ComPort::close()
{
    if(!port.isOpen()) return true;
    emit beforeClose();
    port.close();
    if(port.isOpen()) {
        _lastError = tr("Порт %1: Ошибка при закрытии.").arg( port.portName() );
        emit toLog(LogError, _lastError);
        return false;
    }
    emit afterClose();
    emit toLog(LogInfo, tr("Порт %1: Закрыт.").arg(port.portName()) );
    return true;
}
//=======================================================================================================
bool ComPort::setPortSettings( const PortSettingsStruct &portSettings )
{
    return (setPortName(portSettings.portName) && setBaudRate(portSettings.baudRate) &&
            setDataBits(portSettings.dataBits) && setStopBits(portSettings.stopBits) &&
            setParity(portSettings.parity) && setBufferSize(portSettings.bufSize) );
}
//=======================================================================================================
bool ComPort::setPortSettings(const QString &portName, qint32 baudRate, QSerialPort::DataBits dataBits,
                     QSerialPort::StopBits stopBits, QSerialPort::Parity parity, qint64 bufSize)
{
    return (setPortName(portName) && setBaudRate(baudRate) && setDataBits(dataBits) &&
            setStopBits(stopBits) && setParity(parity) && setBufferSize(bufSize) );
}
//=======================================================================================================
bool ComPort::setBufferSize(qint64 bufSize)
{
    port.setReadBufferSize( bufSize );
    toLog(LogInfo, tr("Порт %1: Установлен размер буфера %2.").arg(port.portName()).arg(bufSize));
    return true;
}
//=======================================================================================================
bool ComPort::setPortName(const QString &portName)
{
    QString oldName = port.portName();
    port.setPortName(portName);
    toLog(LogInfo, tr("Порт %1: Изменение имени на %2.").arg(oldName).arg(port.portName()));
    return true;
}
//=======================================================================================================
bool ComPort::setBaudRate(qint32 baudRate)
{
    if(!port.setBaudRate(baudRate)) {
        _lastError = tr("Порт %1: Ошибка при установке скорости %2.").arg( port.portName() ).arg(baudRate);
        emit toLog(LogError, _lastError);
        return false;
    }
    toLog(LogInfo, tr("Порт %1: Установлена скорость %2.").arg(port.portName()).arg(baudRate));
    return true;
}
//=======================================================================================================
bool ComPort::setDataBits(QSerialPort::DataBits dataBits)
{
    QString str = dataBitsToStr(dataBits);
    if(!port.setDataBits(dataBits)) {
        _lastError = tr("Порт %1: Ошибка при установке бит данных %2.").arg( port.portName() ).arg(str);
        emit toLog(LogError, _lastError);
        return false;
    }
    toLog(LogInfo, tr("Порт %1: Установлены биты данных %2.").arg(port.portName()).arg(str));
    return true;
}
//=======================================================================================================
bool ComPort::setStopBits(QSerialPort::StopBits stopBits)
{
    QString str = stopBitsToStr(stopBits);
    if(!port.setStopBits(stopBits)) {
        _lastError = tr("Порт %1: Ошибка при установке стоповых бит %2.").arg( port.portName() ).arg(str);
        emit toLog(LogError, _lastError);
        return false;
    }
    toLog(LogInfo, tr("Порт %1: Установлены стоповые биты %2.").arg(port.portName()).arg(str));
    return true;
}
//=======================================================================================================
bool ComPort::setParity(QSerialPort::Parity parity)
{
    QString str = parityToStr(parity);
    if(!port.setParity(parity)) {
        _lastError = tr("Порт %1: Ошибка при установке четности %2.").arg( port.portName() ).arg(str);
        emit toLog(LogError, _lastError);
        return false;
    }
    toLog(LogInfo, tr("Порт %1: Установлена четность %2.").arg(port.portName()).arg(str));
    return true;
}
//=======================================================================================================
bool ComPort::setDtr(bool on)
{
    QString str = on ? "вкл" : "выкл";
    if(!port.setDataTerminalReady(on)) {
        _lastError = tr("Порт %1: Ошибка при установке DTR = %2.").arg( port.portName() ).arg(str);
        emit toLog(LogError, _lastError);
        return false;
    }
    toLog(LogInfo, tr("Порт %1: Установлен DTR = %2.").arg(port.portName()).arg(str));
    return true;
}
//=======================================================================================================
bool ComPort::setRts(bool on)
{
    QString str = on ? "вкл" : "выкл";
    if(!port.setRequestToSend(on)) {
        _lastError = tr("Порт %1: Ошибка при установке RTS = %2.").arg( port.portName() ).arg(str);
        emit toLog(LogError, _lastError);
        return false;
    }
    toLog(LogInfo, tr("Порт %1: Установлен RTS = %2.").arg(port.portName()).arg(str));
    return true;
}
//=======================================================================================================
bool ComPort::clear(QSerialPort::Directions directions)
{
    QString str = ((directions & QSerialPort::Input) == QSerialPort::Input) ? "входящий" : "";
    if((directions & QSerialPort::Output) == QSerialPort::Output) {
        if(!str.isEmpty()) str += " и ";
        str += "исходящий";
    }

    if(!port.clear(directions)) {
        _lastError = tr("Порт %1: Ошибка при очистке буфера %2.").arg( port.portName() ).arg(str);
        emit toLog(LogError, _lastError);
        return false;
    }
    toLog(LogInfo, tr("Порт %1: Очищен буфер %2.").arg(port.portName()).arg(str));
    return true;
}
//=======================================================================================================
QString ComPort::dataBitsToStr(QSerialPort::DataBits dataBits)
{
    return (dataBits == QSerialPort::UnknownDataBits) ? "Unknown" : QString("%1").arg(dataBits);
}
//=======================================================================================================
QString ComPort::stopBitsToStr(QSerialPort::StopBits stopBits)
{
    switch (stopBits) {
    case QSerialPort::OneStop:         return "1";
    case QSerialPort::OneAndHalfStop:  return "1.5";
    case QSerialPort::TwoStop:         return "2";
    default:                           return "Unknown";
    }
    return "Unknown";
}
//=======================================================================================================
QString ComPort::parityToStr(QSerialPort::Parity parity)
{
    switch (parity) {
    case QSerialPort::NoParity:       return "No";
    case QSerialPort::EvenParity:     return "Even";
    case QSerialPort::OddParity:      return "Odd";
    case QSerialPort::SpaceParity:    return "Space";
    case QSerialPort::MarkParity:     return "Mark";
    default:                          return "Unknown";
    }
    return "Unknown";
}
//=======================================================================================================
QSerialPort::DataBits ComPort::strToDataBits(const QString &dataBitsStr)
{
    int n = Convert::strToIntDef(dataBitsStr, -1);
    if((n<5) || (n>8)) return QSerialPort::UnknownDataBits;
    return static_cast<QSerialPort::DataBits>(n);
}
//=======================================================================================================
QSerialPort::StopBits ComPort::strToStopBits(const QString &stopBitsStr)
{
    if(stopBitsStr == "1") return QSerialPort::OneStop;
    if(stopBitsStr == "1.5") return QSerialPort::OneAndHalfStop;
    if(stopBitsStr == "2") return QSerialPort::TwoStop;
    return QSerialPort::UnknownStopBits;
}
//=======================================================================================================
QSerialPort::Parity ComPort::strToParity(const QString &parityStr)
{
    if(parityStr.toLower() == "no") return QSerialPort::NoParity;
    if(parityStr.toLower() == "even") return QSerialPort::EvenParity;
    if(parityStr.toLower() == "odd") return QSerialPort::OddParity;
    if(parityStr.toLower() == "space") return QSerialPort::SpaceParity;
    if(parityStr.toLower() == "mark") return QSerialPort::MarkParity;
    return QSerialPort::UnknownParity;
}
//=======================================================================================================
PortSettingsStruct ComPort::readSettingsFromFile(const QString &fileName, const QString &sectionName, const PortSettingsStruct &defaultSettings)
{
    PortSettingsStruct res;
    QSettings *ini = new QSettings(fileName, QSettings::IniFormat);
    ini->setIniCodec("UTF-8");

    res.portName = ini->value( QString("%1/portname").arg(sectionName), defaultSettings.portName).toString();
    res.baudRate = ini->value( QString("%1/baudrate").arg(sectionName), defaultSettings.baudRate).toInt();
    res.bufSize = ini->value( QString("%1/bufsize").arg(sectionName), defaultSettings.bufSize).toLongLong();
    res.dataBits = strToDataBits( ini->value( QString("%1/databits").arg(sectionName), dataBitsToStr(defaultSettings.dataBits)).toString() );
    res.stopBits = strToStopBits( ini->value( QString("%1/stopbits").arg(sectionName), stopBitsToStr(defaultSettings.stopBits)).toString() );
    res.parity = strToParity( ini->value( QString("%1/parity").arg(sectionName), parityToStr(defaultSettings.parity)).toString() );
    res.dtr = ini->value( QString("%1/dtr").arg(sectionName), defaultSettings.dtr).toBool();
    res.rts = ini->value( QString("%1/rts").arg(sectionName), defaultSettings.rts).toBool();

    delete ini;
    return res;
}
//========================================================================================================
void ComPort::writeSettingsToFile(const QString &fileName, const QString &sectionName, const PortSettingsStruct &portSettings)
{
    QSettings *ini = new QSettings(fileName, QSettings::IniFormat);
    ini->setIniCodec("UTF-8");

    ini->setValue( QString("%1/portname").arg(sectionName), portSettings.portName);
    ini->setValue( QString("%1/baudrate").arg(sectionName), portSettings.baudRate);
    ini->setValue( QString("%1/bufsize").arg(sectionName), portSettings.bufSize);
    ini->setValue( QString("%1/databits").arg(sectionName), dataBitsToStr(portSettings.dataBits));
    ini->setValue( QString("%1/stopbits").arg(sectionName), stopBitsToStr(portSettings.stopBits));
    ini->setValue( QString("%1/parity").arg(sectionName), parityToStr(portSettings.parity));
    ini->setValue( QString("%1/dtr").arg(sectionName), portSettings.dtr);
    ini->setValue( QString("%1/rts").arg(sectionName), portSettings.rts);

    ini->sync();
    delete ini;
}
//========================================================================================================
PortSettingsStruct ComPort::parseSettingsFromString(const QString &settingsString)
{
    PortSettingsStruct s;
    s.portName = "COM1";
    s.baudRate = 9600;
    s.bufSize = 1024;
    s.dataBits = QSerialPort::Data8;
    s.stopBits = QSerialPort::OneStop;
    s.parity = QSerialPort::NoParity;
    s.dtr = false;
    s.rts = false;

    QStringList lst = settingsString.split(":");
    int indx = 0;
    if(lst.size() > 0) {
       QString str = lst.at(indx++);
       bool ok;
       qint32 n = str.toUInt(&ok);
       if(ok) {
           s.baudRate = n;
       }
       else {
           s.portName = str;
           if(indx < lst.size()) {
               str = lst.at(indx++);
               n = str.toUInt(&ok);
               if(ok) {
                   s.baudRate = n;
               }
           }
       }

       if(indx < lst.size()) {
           s.dataBits = strToDataBits( lst.at(indx++) );
       }

       if(indx < lst.size()) {
           s.stopBits = strToStopBits( lst.at(indx++) );
       }

       if(indx < lst.size()) {
           s.parity = strToParity( lst.at(indx++) );
       }

    }
    return s;
}
//========================================================================================================
QString ComPort::settingsString(const PortSettingsStruct &portSettings, bool withPortName)
{
    return QString(
                (withPortName ? portSettings.portName + ":" : "") +
                QString::number(portSettings.baudRate) + ":" +
                dataBitsToStr(portSettings.dataBits) + ":" +
                stopBitsToStr(portSettings.stopBits) + ":" +
                parityToStr(portSettings.parity)
                );
}
//========================================================================================================
PortSettingsStruct ComPort::portSettings()
{
    PortSettingsStruct res;
    res.portName = port.portName();
    res.baudRate = port.baudRate();
    res.bufSize = port.readBufferSize();
    res.dataBits = port.dataBits();
    res.stopBits = port.stopBits();
    res.parity = port.parity();
    res.dtr = port.isDataTerminalReady();
    res.rts = port.isRequestToSend();
    return res;
}
//========================================================================================================
#ifdef WIDGETS_SOFT
//========================================================================================================
void ComPort::fillPortNameBox(QComboBox *box, const QString &defaultPortName)
{
    if(!box) return;
    box->clear();
    int indx = 0;
    QSerialPortInfo info;
    for(auto i=0; i<info.availablePorts().size(); ++i) {
        QString portName = info.availablePorts().at(i).portName();
        box->addItem( portName );
        if(portName == defaultPortName) indx = i;
    }
    if(indx < box->count()) box->setCurrentIndex(indx);
    if(box->isEditable() && !defaultPortName.isEmpty() && (box->currentText() != defaultPortName))
        box->setCurrentText(defaultPortName);
}
//========================================================================================================
void ComPort::fillBaudRateBox(QComboBox *box, qint32 defaultBaudRate)
{
    if(!box) return;
    box->clear();
    box->addItem( "300", 300);
    box->addItem( "600", 600);
    box->addItem( "1200", 1200);
    box->addItem( "2400", 2400);
    box->addItem( "4800", 4800);
    box->addItem( "9600", 9600);
    box->addItem( "19200", 19200);
    box->addItem( "38400", 38400);
    box->addItem( "57600", 57600);
    box->addItem( "115200", 115200);

    for(auto i=0; i<box->count(); ++i) {
        if(box->itemData(i).toInt() == defaultBaudRate) {
            box->setCurrentIndex(i);
            break;
        }
    }
}
//========================================================================================================
void ComPort::fillDataBitsBox(QComboBox *box, QSerialPort::DataBits defaultDataBits)
{
    if(!box) return;
    box->clear();
    int indx = 0;
    for(auto i=5; i<=8; ++i) {
        QSerialPort::DataBits d = static_cast<QSerialPort::DataBits>(i);
        box->addItem( dataBitsToStr(d), d );
        if(d == defaultDataBits) indx = box->count()-1;
    }
    if(indx < box->count()) box->setCurrentIndex(indx);
}
//========================================================================================================
void ComPort::fillStopBitsBox(QComboBox *box, QSerialPort::StopBits defaultStopBits)
{
    if(!box) return;
    box->clear();
    box->addItem( stopBitsToStr(QSerialPort::OneStop), QSerialPort::OneStop);
    box->addItem( stopBitsToStr(QSerialPort::OneAndHalfStop), QSerialPort::OneAndHalfStop);
    box->addItem( stopBitsToStr(QSerialPort::TwoStop), QSerialPort::TwoStop);
    for(auto i=0; i<box->count(); ++i) {
        if(box->itemData(i).toInt() == static_cast<int>(defaultStopBits)) {
            box->setCurrentIndex(i);
            break;
        }
    }
}
//========================================================================================================
void ComPort::fillParityBox(QComboBox *box, QSerialPort::Parity defaultParity)
{
    if(!box) return;
    box->clear();
    box->addItem( parityToStr(QSerialPort::NoParity), QSerialPort::NoParity);
    box->addItem( parityToStr(QSerialPort::EvenParity), QSerialPort::EvenParity);
    box->addItem( parityToStr(QSerialPort::OddParity), QSerialPort::OddParity);
    box->addItem( parityToStr(QSerialPort::SpaceParity), QSerialPort::SpaceParity);
    box->addItem( parityToStr(QSerialPort::MarkParity), QSerialPort::MarkParity);
    for(auto i=0; i<box->count(); ++i) {
        if(box->itemData(i).toInt() == static_cast<int>(defaultParity)) {
            box->setCurrentIndex(i);
            break;
        }
    }
}
//========================================================================================================
#endif
//========================================================================================================
} // namespace nayk
