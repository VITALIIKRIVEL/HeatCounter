#ifndef CONVERT_H
#define CONVERT_H

#include <QtCore>
#include <QString>
#include <QDateTime>
//=========================================================================================================
namespace nayk {
//=========================================================================================================
typedef union Value8bits {
    qint8 val_int;
    quint8 val_uint;
} ByteUnion;

//=========================================================================================================
typedef union Value16bits {
    qint16 val_int;
    quint16 val_uint;
    quint8 val_byte[2];
    qint8 val_sbyte[2];
} ShortUnion;

//=========================================================================================================
typedef union Value32bits {
    qint32 val_int;
    quint32 val_uint;
    float val_float;
    quint8 val_byte[4];
    qint8 val_sbyte[4];
} SingleUnion;

//=========================================================================================================
typedef union Value64bits {
    qint64 val_int;
    quint64 val_uint;
    double val_float;
    quint8 val_byte[8];
    qint8 val_sbyte[8];
} DoubleUnion;

//=========================================================================================================
class Convert
{
public:
    static quint8 bcdEncode(qint8 value);
    static quint16 bcdEncode(qint16 value);
    static quint32 bcdEncode(qint32 value);
    static quint64 bcdEncode(qint64 value);
    static quint8 bcdEncode(quint8 value);
    static quint16 bcdEncode(quint16 value);
    static quint32 bcdEncode(quint32 value);
    static quint64 bcdEncode(quint64 value);
    static qint8 bcdDecode(quint8 bcdValue, bool *ok = Q_NULLPTR);
    static qint16 bcdDecode(quint16 bcdValue, bool *ok = Q_NULLPTR);
    static qint32 bcdDecode(quint32 bcdValue, bool *ok = Q_NULLPTR);
    static qint64 bcdDecode(quint64 bcdValue, bool *ok = Q_NULLPTR);
    static quint8 bcdDecodeUnsigned(quint8 bcdValue, bool *ok = Q_NULLPTR);
    static quint16 bcdDecodeUnsigned(quint16 bcdValue, bool *ok = Q_NULLPTR);
    static quint32 bcdDecodeUnsigned(quint32 bcdValue, bool *ok = Q_NULLPTR);
    static quint64 bcdDecodeUnsigned(quint64 bcdValue, bool *ok = Q_NULLPTR);
    static QString intToHex(qint8  val, int len = 2, bool withPrefix = false);
    static QString intToHex(quint8  val, int len = 2, bool withPrefix = false);
    static QString intToHex(qint16 val, int len = 4, bool withPrefix = false);
    static QString intToHex(quint16 val, int len = 4, bool withPrefix = false);
    static QString intToHex(qint32 val, int len = 8, bool withPrefix = false);
    static QString intToHex(quint32 val, int len = 8, bool withPrefix = false);
    static QString intToHex(qint64 val, int len = 16, bool withPrefix = false);
    static QString intToHex(quint64 val, int len = 16, bool withPrefix = false);
    static qint8 strToIntDef(const QString &str, qint8 def);
    static quint8 strToIntDef(const QString &str, quint8 def);
    static qint16 strToIntDef(const QString &str, qint16 def);
    static quint16 strToIntDef(const QString &str, quint16 def);
    static qint32 strToIntDef(const QString &str, qint32 def);
    static quint32 strToIntDef(const QString &str, quint32 def);
    static qint64 strToIntDef(const QString &str, qint64 def);
    static quint64 strToIntDef(const QString &str, quint64 def);
    static QString doubleToStr(double val, int precise = 2);
    static double strToDouble(const QString &str, bool *ok = Q_NULLPTR );
    static QString updatePrecise(const QString &val, int precise = 2);
    static QDateTime strToDateTime(const QString &str);
    static quint8  lo(quint16 val);
    static quint16 lo(quint32 val);
    static quint32 lo(quint64 val);
    static quint8  hi(quint16 val);
    static quint16 hi(quint32 val);
    static quint32 hi(quint64 val);
    static quint16 reverse(quint16 val);
    static quint32 reverse(quint32 val);
    static quint64 reverse(quint64 val);
    static bool isASCII(quint8 val);
    static bool isASCII(qint8 val);
    static bool isASCII(char val);
    static QString endLineToHTML(const QString &str);
    static QString encodeXMLText(const QString &text);
    static QString decodeXMLText(const QString &text);
    static QString changeDecimalSeparator(const QString &str);
    static QString changeLineFit(const QString &str, const QString &lineFit);
    //
    Convert() = delete; // < std=c++11, обеспечивает запрет на создание любого экземпляра Convert
};

//=========================================================================================================
} // namespace nayk

//=========================================================================================================
#endif // CONVERT_H
