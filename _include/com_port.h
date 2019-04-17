#ifndef COM_PORT_H
#define COM_PORT_H

#include <QObject>
#include <QSerialPort>

#ifdef WIDGETS_SOFT
#include <QComboBox>
#endif

//
#include "log.h"
//======================================================================================================
namespace nayk {
//======================================================================================================

typedef struct PortSettingsStruct {
    QString portName;
    qint32 baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::StopBits stopBits;
    QSerialPort::Parity parity;
    bool dtr {false};
    bool rts {false};
    qint32 bufSize;
} PortSettingsStruct;

//======================================================================================================
class ComPort : public QObject
{
    Q_OBJECT

public:
    explicit ComPort(QObject *parent = nullptr, bool autoRead = false );
    ~ComPort();
    QString lastError() const { return _lastError; }
    bool autoRead() { return _autoRead; }
    void setAutoRead( bool on ) { _autoRead = on; }
    bool open(QIODevice::OpenMode openMode = QIODevice::ReadWrite);
    bool close();
    bool isOpen() const { return port.isOpen(); }
    QString portName() const { return port.portName(); }
    qint32 baudRate() const { return port.baudRate(); }
    QSerialPort::DataBits dataBits() const { return port.dataBits(); }
    QSerialPort::StopBits stopBits() const { return port.stopBits(); }
    QSerialPort::Parity parity() const { return port.parity(); }
    PortSettingsStruct portSettings();
    bool isDtr() { return port.isDataTerminalReady(); }
    bool isRts() { return port.isRequestToSend(); }

    bool setPortSettings(const QString &portName, qint32 baudRate = 9600, QSerialPort::DataBits dataBits = QSerialPort::Data8,
                         QSerialPort::StopBits stopBits = QSerialPort::OneStop, QSerialPort::Parity parity = QSerialPort::NoParity,
                         qint64 bufSize = 1024);
    bool setPortSettings( const PortSettingsStruct &portSettings );
    bool setPortName(const QString &portName);
    bool setBaudRate(qint32 baudRate);
    bool setDataBits(QSerialPort::DataBits dataBits);
    bool setStopBits(QSerialPort::StopBits stopBits);
    bool setParity(QSerialPort::Parity parity);
    bool setBufferSize(qint64 bufSize);
    bool setDtr(bool on = true);
    bool setRts(bool on = true);
    bool clear(QSerialPort::Directions directions = QSerialPort::AllDirections);
    QByteArray readAll();
    qint64 write(const QByteArray &data);

    static QString dataBitsToStr(QSerialPort::DataBits dataBits);
    static QString stopBitsToStr(QSerialPort::StopBits stopBits);
    static QString parityToStr(QSerialPort::Parity parity);
    static QSerialPort::DataBits strToDataBits(const QString &dataBitsStr);
    static QSerialPort::StopBits strToStopBits(const QString &stopBitsStr);
    static QSerialPort::Parity strToParity(const QString &parityStr);
    static PortSettingsStruct readSettingsFromFile(const QString &fileName, const QString &sectionName, const PortSettingsStruct &defaultSettings);
    static void writeSettingsToFile(const QString &fileName, const QString &sectionName, const PortSettingsStruct &portSettings);
    static PortSettingsStruct parseSettingsFromString(const QString &settingsString);
    static QString settingsString(const PortSettingsStruct &portSettings, bool withPortName = true);

#ifdef WIDGETS_SOFT
    static void fillPortNameBox(QComboBox *box, const QString &defaultPortName = QString());
    static void fillBaudRateBox(QComboBox *box, qint32 defaultBaudRate = 9600);
    static void fillDataBitsBox(QComboBox *box, QSerialPort::DataBits defaultDataBits = QSerialPort::Data8);
    static void fillStopBitsBox(QComboBox *box, QSerialPort::StopBits defaultStopBits = QSerialPort::OneStop);
    static void fillParityBox(QComboBox *box, QSerialPort::Parity defaultParity = QSerialPort::NoParity);
#endif

private:
    QSerialPort port;
    QString _lastError {""};
    bool _autoRead {false};

signals:
    void toLog(LogType logType, QString text);
    void rxBytes(QByteArray rxBuf);
    void readyRead();
    void beforeOpen();
    void afterOpen();
    void beforeClose();
    void afterClose();
    void errorOccurred(QSerialPort::SerialPortError error);

private slots:
    void on_ReadyRead();
    void on_Error(QSerialPort::SerialPortError error);
};
//======================================================================================================
} // namespace nayk
#endif // COM_PORT_H
