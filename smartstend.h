#ifndef SMARTSTEND_H
#define SMARTSTEND_H

#include <QObject>
#include <QtSerialPort>

class SmartStend : public QObject
{
    Q_OBJECT
public:
    explicit SmartStend(QObject *parent = nullptr);
    ~SmartStend();

    void getPortPointer(QSerialPort * port);
    void sendCommandToStend(QString cmd);

    void readPulsesChannel1();
    void readPulsesChannel2();
    void pulsesReset();
    void writePulsesToGenChannel1();
    void writePulsesToGenChannel2();
    void externalInterfaceOn();
    void externalInterfaceOff();
    void setTokInterfaceOff();
    void freqMeterOn();
    void freqMeterOff();
    void readFreq();
    void plataOn();
    void plataOff();
    void readTok();
    void setTokPlataOff();


    void programmatorOn();
    void programmatorOff();

    void finishIndicatorOn();
    void finishIndicatorOff();
    void errorIndicatoOn();
    void errorIndicatoOff();

    void testCommand();

signals:
    answerFromStend(QString answer);

public slots:
    void slotGetAnsFromStend(QString answer);

private:
    QSerialPort * portStend;

};

#endif // SMARTSTEND_H
