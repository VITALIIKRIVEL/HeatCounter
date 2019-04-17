#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>
#include <QDateTime>

/*******************************************************************************************/
namespace global {

/*******************************************************************************************/
const QString month_Names[12] = { QObject::tr("Январь"), QObject::tr("Февраль"), QObject::tr("Март"),
                                  QObject::tr("Апрель"), QObject::tr("Май"), QObject::tr("Июнь"),
                                  QObject::tr("Июль"), QObject::tr("Август"), QObject::tr("Сентябрь"),
                                  QObject::tr("Октябрь"), QObject::tr("Ноябрь"), QObject::tr("Декабрь") };

/*******************************************************************************************/
typedef union Value8bits {
    qint8 val_int;
    quint8 val_uint;
} ByteUnion;

/*******************************************************************************************/
typedef union Value16bits {
    qint16 val_int;
    quint16 val_uint;
    quint8 val_byte[2];
} ShortUnion;

/*******************************************************************************************/
typedef union Value32bits {
    qint32 val_int;
    quint32 val_uint;
    float val_float;
    quint8 val_byte[4];
} SingleUnion;

/*******************************************************************************************/
typedef union Value64bits {
    qint64 val_int;
    quint64 val_uint;
    double val_float;
    quint8 val_byte[8];
} DoubleUnion;

/*******************************************************************************************/
bool directoryExists(const QString &dirName);
bool fileExists(const QString &fileName);
bool makePath(const QString &dirPath);
QString changeFileExt(const QString &fileName, const QString &newExt);
QString extractFileExt(const QString &fileName, bool withDot = true);
QString extractFileName(const QString &fileName);
QString extractFilePath(const QString &fileName);
//
QString intToHex(qint8  val, int len = 2, bool withPrefix = false);
QString intToHex(quint8  val, int len = 2, bool withPrefix = false);
QString intToHex(qint16 val, int len = 4, bool withPrefix = false);
QString intToHex(quint16 val, int len = 4, bool withPrefix = false);
QString intToHex(qint32 val, int len = 8, bool withPrefix = false);
QString intToHex(quint32 val, int len = 8, bool withPrefix = false);
QString intToHex(qint64 val, int len = 16, bool withPrefix = false);
QString intToHex(quint64 val, int len = 16, bool withPrefix = false);
//
qint8 strToIntDef(const QString &str, qint8 def);
quint8 strToIntDef(const QString &str, quint8 def);
qint16 strToIntDef(const QString &str, qint16 def);
quint16 strToIntDef(const QString &str, quint16 def);
qint32 strToIntDef(const QString &str, qint32 def);
quint32 strToIntDef(const QString &str, quint32 def);
qint64 strToIntDef(const QString &str, qint64 def);
quint64 strToIntDef(const QString &str, quint64 def);
//
QString doubleToStr(double val, int precise = 6);
double  strToDouble(const QString &str, bool *ok = Q_NULLPTR );
int getMaxPrecise(const QString &val1, const QString &val2);
//
quint8  lo(quint16 val);
quint16 lo(quint32 val);
quint32 lo(quint64 val);
quint8  hi(quint16 val);
quint16 hi(quint32 val);
quint32 hi(quint64 val);
//
quint16 reverse(quint16 val);
quint32 reverse(quint32 val);
quint64 reverse(quint64 val);
//
bool isASCII(quint8 val);
bool isASCII(qint8 val);
bool isASCII(char val);
//
bool isSummerTime(QDateTime dateTime);
QString strToHTML(const QString &str);
bool isValidLogin(const QString &login);
bool isValidEMail(const QString &email);
//

void pause(int ms = 1000);

} // namespace global

/*******************************************************************************************/
#endif // GLOBAL_H
