#ifndef OBJECTTHREAD_H
#define OBJECTTHREAD_H
#include <QObject>
#include <QSerialPort>
#include <QMap>
#include <QVariant>
#include <QVector>


class ObjectThread: public QObject
{
   Q_OBJECT

public:
    int workPlace;

    explicit ObjectThread(QObject *parent);
    ~ObjectThread();

    quint8 makeCRC(QByteArray);
    quint8 checkCRC(QByteArray);
    quint8 reverse8(quint8 temp);

    QVector<QVector<bool>> getVectorMatrix();

    void setWorkPlace(int workplaceNumber);
    void setVectorIndicatorStateMatrix(QVector<QVector<bool>> vectorIndicatorStateMatrix);
    void setParamsMap(QMap<QString, QVariant> map);
    void setMapWrite(QMap<QString, QByteArray> map);
    void setMapRead(QMap<QString, QByteArray> mape);
    void setIsWorkPlaceUseVector(QVector<bool> vec);

    void setCalibrationMap(QMap<QString, QByteArray> calibrationMap);
    void setCalibrationFloatMap(QMap<QString, float> calibrationFloatMap);

    //установить порт проверки проводных интерфейсов
    void setPortDigitalInterface(QSerialPort *portDigitalInterface);

    //правильный анализатор пакета
    bool packetSETProcessing(QByteArray pack);

    //установка исходных адресов после проверки проводных интерфейсов
    void setPrimeAddressDefault(int workPlaceNumber, QSerialPort *port1, QSerialPort *port2,
                                QSerialPort *port3, QSerialPort *port4);

    //передача в главное окно результатов калибровки
    QString getR_Ref1_1();
    QString getR_Ref2_1();
    QString getR_Ref1_2();
    QString getR_Ref2_2();
    QString getR_Ref1_3();
    QString getR_Ref2_3();
    QString getR_Ref1_4();
    QString getR_Ref2_4();


signals:
    checkWritingError(int currentIndicatorNumber);
    checkCalibrationError(int currentIndicatorNumber);
    signalRMV(int color, QString str, int currentIndicatorNumber);
    checkPulsesOutputHeat(int currentIndicatorNumber);
    checkPulsesInputVolume(int currentIndicatorNumber);
    checkPulsesOutputDefault(int currentIndicatorNumber);
    checkMBusOn(int currentIndicatorNumber);
    checkMBusCheck(int currentIndicatorNumber);
    checkCalibrationModeOff(int currentIndicatorNumber);
    checkWireInterfaceChecking(int currentIndicatorNumber);
    checkMbusOff(int currentIndicatorNumber);

    workPlaceOff(int currentIndicatorNumber);
    textBrowser(QString message);
    errorStringSignal(QString str);

public slots:
    void slotWriteParams(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);
    void slotReadParams(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);
    void slotCalibration(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);
    void slotPulsesOutputHeat(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);
    void slotPulsesInputVolume(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);
    void slotPulsesOutputDefault(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);
    void slotMBusOn(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);
    void slotMBusCheck(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);
    void slotCalibrationModeOff(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);
    void slotWireInterfaceChecking(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4,
                                   QSerialPort *portDigitalInterfaceChecking);
    void slotMBusOff(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);


    void slotOnlyReadParams(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4);

private slots:
//    void slotWriteParams(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4,
//                         int workPlaceNumber, QVector<QVector<bool>> vectorIndicatorStateMatrix,
//                         QMap<QString, QVariant> paramsMapToThreads, QMap<QString, QByteArray> mapwrite, QMap<QString, QByteArray> mapRead,
//                         QVector<bool> isWorkPlaceUse);
//    void slotReadParams(QSerialPort *port1, QSerialPort *port2, QSerialPort *port3, QSerialPort *port4,
//                        int workPlaceNumber, QVector<QVector<bool>> vectorIndicatorStateMatrix,
//                        QMap<QString, QVariant> paramsMapToThreads, QMap<QString, QByteArray> mapwrite,
//                        QMap<QString, QByteArray> mapRead,
//                        QVector<bool> isWorkPlaceUse);

private:

 //   QSerialPort *portDigitalInterface;

    QByteArray packetToRead;
    QVector<QVector<bool>> vectorIndicatorStateMatrix;
    QMap<QString, QVariant> paramsMapToThreads;
    QMap<QString, QByteArray> mapwrite;
    QMap<QString, QByteArray> mapRead;
    QVector<bool> isWorkPlaceUse;

    QStringList allParamsNameList;

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

    QString r_ref1Result, r_ref1Result2, r_ref1Result3, r_ref1Result4,
            r_ref2Result, r_ref2Result2, r_ref2Result3, r_ref2Result4,
            rtc_OffsetResult, rtc_OffsetResult2, rtc_OffsetResult3, rtc_OffsetResult4,
            rtc_TcompResult, rtc_TcompResult2, rtc_TcompResult3, rtc_TcompResult4;


    //параметры для записи в БД
    QString R_Ref1String1, R_Ref2String1,
            R_Ref1String2, R_Ref2String2,
            R_Ref1String3, R_Ref2String3,
            R_Ref1String4, R_Ref2String4;




};

#endif // OBJECTTHREAD_H
