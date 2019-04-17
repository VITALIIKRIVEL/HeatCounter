#include <QLocale>
#include "convert.h"

namespace nayk {
//=======================================================================================================
const QChar charXML[]     = {'"',      '&',     '<',    '>',    '\n',    '\r'};
const QString stringXML[] = {"&quot;", "&amp;", "&lt;", "&gt;", "&#10;", "&#13;"};

//=======================================================================================================
bool Convert::isASCII(quint8 val)
{
    return ((val>=32) && (val<=126));
}
//=======================================================================================================
bool Convert::isASCII(qint8 val)
{
    return ((val>=32) && (val<=126));
}
//=======================================================================================================
bool Convert::isASCII(char val)
{
    return ((val>=32) && (val<=126));
}
//=======================================================================================================
quint8 Convert::bcdEncode(qint8 value)
{
    QString s = "";
    if(value < 0) {
        s += "F";
        value = 0-value;
    }
    s += QString::number(value);
    return static_cast<quint8>( s.toUInt(nullptr, 16) );
}
//=======================================================================================================
quint16 Convert::bcdEncode(qint16 value)
{
    QString s = "";
    if(value < 0) {
        s += "F";
        value = 0-value;
    }
    s += QString::number(value);
    return static_cast<quint16>( s.toUInt(nullptr, 16) );
}
//=======================================================================================================
quint32 Convert::bcdEncode(qint32 value)
{
    QString s = "";
    if(value < 0) {
        s += "F";
        value = 0-value;
    }
    s += QString::number(value);
    return static_cast<quint32>( s.toULong(nullptr, 16) );
}
//=======================================================================================================
quint64 Convert::bcdEncode(qint64 value)
{
    QString s = "";
    if(value < 0) {
        s += "F";
        value = 0-value;
    }
    s += QString::number(value);
    return static_cast<quint64>( s.toULongLong(nullptr, 16) );
}
//=======================================================================================================
quint8 Convert::bcdEncode(quint8 value)
{
    QString s = QString::number( value );
    return static_cast<quint8>( s.toUInt(nullptr, 16) );
}
//=======================================================================================================
quint16 Convert::bcdEncode(quint16 value)
{
    QString s = QString::number( value );
    return static_cast<quint16>( s.toUInt(nullptr, 16) );
}
//=======================================================================================================
quint32 Convert::bcdEncode(quint32 value)
{
    QString s = QString::number( value );
    return static_cast<quint32>( s.toULong(nullptr, 16) );
}
//=======================================================================================================
quint64 Convert::bcdEncode(quint64 value)
{
    QString s = QString::number( value );
    return static_cast<quint64>( s.toULongLong(nullptr, 16) );
}
//=======================================================================================================
qint8 Convert::bcdDecode(quint8 bcdValue, bool *ok)
{
    QString s = intToHex( bcdValue, 2, false );
    if(s.left(1).toUpper() == "F") s[0] = QChar('-');
    return static_cast<qint8>(s.toInt(ok, 10));
}
//=======================================================================================================
qint16 Convert::bcdDecode(quint16 bcdValue, bool *ok)
{
    QString s = intToHex( bcdValue, 4, false );
    if(s.left(1).toUpper() == "F") s[0] = QChar('-');
    return static_cast<qint16>(s.toInt(ok, 10));
}
//=======================================================================================================
qint32 Convert::bcdDecode(quint32 bcdValue, bool *ok)
{
    QString s = intToHex( bcdValue, 8, false );
    if(s.left(1).toUpper() == "F") s[0] = QChar('-');
    return static_cast<qint32>(s.toLong(ok, 10));
}
//=======================================================================================================
qint64 Convert::bcdDecode(quint64 bcdValue, bool *ok)
{
    QString s = intToHex( bcdValue, 16, false );
    if(s.left(1).toUpper() == "F") s[0] = QChar('-');
    return static_cast<qint64>(s.toLongLong(ok, 10));
}
//=======================================================================================================
quint8 Convert::bcdDecodeUnsigned(quint8 bcdValue, bool *ok)
{
    QString s = intToHex( bcdValue, 2, false );
    return static_cast<quint8>(s.toUInt(ok, 10));
}
//=======================================================================================================
quint16 Convert::bcdDecodeUnsigned(quint16 bcdValue, bool *ok)
{
    QString s = intToHex( bcdValue, 4, false );
    return static_cast<quint16>(s.toUInt(ok, 10));
}
//=======================================================================================================
quint32 Convert::bcdDecodeUnsigned(quint32 bcdValue, bool *ok)
{
    QString s = intToHex( bcdValue, 8, false );
    return static_cast<quint32>(s.toULong(ok, 10));
}
//=======================================================================================================
quint64 Convert::bcdDecodeUnsigned(quint64 bcdValue, bool *ok)
{
    QString s = intToHex( bcdValue, 16, false );
    return static_cast<quint64>(s.toULongLong(ok, 10));
}
//=======================================================================================================
QString Convert::intToHex(quint8  val, int len /*= 2*/, bool withPrefix /*= false*/)
{
    QString prefix = withPrefix ? "0x" : "";
    QString res = QString::number(val, 16);
    while(res.length() < len) res = "0" + res;
    return prefix + res.toUpper().right(len);
}
//=======================================================================================================
QString Convert::intToHex(qint8  val, int len /*= 2*/, bool withPrefix /*= false*/)
{
    return intToHex( static_cast<quint8>(val), len, withPrefix );
}
//=======================================================================================================
QString Convert::intToHex(quint16 val, int len /*= 4*/, bool withPrefix /*= false*/)
{
    QString prefix = withPrefix ? "0x" : "";
    QString res = QString::number(val, 16);
    while(res.length() < len) res = "0" + res;
    return prefix + res.toUpper().right(len);
}
//=======================================================================================================
QString Convert::intToHex(qint16  val, int len /*= 4*/, bool withPrefix /*= false*/)
{
    return intToHex( static_cast<quint16>(val), len, withPrefix );
}
//=======================================================================================================
QString Convert::intToHex(quint32 val, int len /*= 8*/, bool withPrefix /*= false*/)
{
    QString prefix = withPrefix ? "0x" : "";
    QString res = QString::number(val, 16);
    while(res.length() < len) res = "0" + res;
    return prefix + res.toUpper().right(len);
}
//=======================================================================================================
QString Convert::intToHex(qint32  val, int len /*= 8*/, bool withPrefix /*= false*/)
{
    return intToHex( static_cast<quint32>(val), len, withPrefix );
}
//=======================================================================================================
QString Convert::intToHex(quint64 val, int len /*= 16*/, bool withPrefix /*= false*/)
{
    QString prefix = withPrefix ? "0x" : "";
    QString res = QString::number(val, 16);
    while(res.length() < len) res = "0" + res;
    return prefix + res.toUpper().right(len);
}
//=======================================================================================================
QString Convert::intToHex(qint64  val, int len /*= 16*/, bool withPrefix /*= false*/)
{
    return intToHex( static_cast<quint64>(val), len, withPrefix );
}
//=======================================================================================================
quint8  Convert::lo(quint16 val)
{
    return static_cast<quint8>( val & 0x00FF );
}
//=======================================================================================================
quint16 Convert::lo(quint32 val)
{
    return static_cast<quint16>( val & 0x0000FFFF );
}
//=======================================================================================================
quint32 Convert::lo(quint64 val)
{
    return static_cast<quint32>( val & 0x00000000FFFFFFFF );
}
//=======================================================================================================
quint8  Convert::hi(quint16 val)
{
    return static_cast<quint8>( (val & 0xFF00) >> 8 );
}
//=======================================================================================================
quint16 Convert::hi(quint32 val)
{
    return static_cast<quint16>( (val & 0xFFFF0000) >> 16 );
}
//=======================================================================================================
quint32 Convert::hi(quint64 val)
{
    return static_cast<quint32>( (val & 0xFFFFFFFF00000000) >> 32 );
}
//=======================================================================================================
quint16 Convert::reverse(quint16 val)
{
    return ((val & 0x00FF) << 8) | ((val & 0xFF00) >> 8);
}
//=======================================================================================================
quint32 Convert::reverse(quint32 val)
{
    return ((val & 0x000000FF) << 24) | ((val & 0x0000FF00) << 8) |
            ((val & 0xFF000000) >> 24) | ((val & 0x00FF0000) >> 8);
}
//=======================================================================================================
quint64 Convert::reverse(quint64 val)
{
    return (( val & 0x00000000000000FF)  << 56) | ((val & 0x000000000000FF00) << 40) |
            ((val & 0x0000000000FF0000) << 24)  | ((val & 0x00000000FF000000) << 8)  |
            ((val & 0x000000FF00000000) >> 8)   | ((val & 0x0000FF0000000000) >> 24) |
            ((val & 0x00FF000000000000) >> 40)  | ((val & 0xFF00000000000000) >> 56);
}
//=======================================================================================================
qint8 Convert::strToIntDef(const QString &str, qint8 def)
{
    bool ok=false;
    int base = 10;
    QString val = str.trimmed();
    if ((val.length()>2) && (val.left(2).toLower()=="0x")) {
        base = 16;
        val.remove(0,2);
    }
    int n = val.toInt(&ok, base);
    if (ok) return static_cast<qint8>(n);
    else return def;
}
//=======================================================================================================
quint8 Convert::strToIntDef(const QString &str, quint8 def)
{
    return static_cast<quint8>( strToIntDef(str, (qint8)def) );
}
//=======================================================================================================
qint16 Convert::strToIntDef(const QString &str, qint16 def)
{
    bool ok=false;
    int base = 10;
    QString val = str.trimmed();
    if ((val.length()>2) && (val.left(2).toLower()=="0x")) {
        base = 16;
        val.remove(0,2);
    }
    qint16 n = val.toShort(&ok, base);
    if (ok) return n;
    else return def;
}
//=======================================================================================================
quint16 Convert::strToIntDef(const QString &str, quint16 def)
{
    bool ok=false;
    int base = 10;
    QString val = str.trimmed();
    if ((val.length()>2) && (val.left(2).toLower()=="0x")) {
        base = 16;
        val.remove(0,2);
    }
    quint16 n = val.toUShort(&ok, base);
    if (ok) return n;
    else return def;
}
//=======================================================================================================
qint32 Convert::strToIntDef(const QString &str, qint32 def)
{
    bool ok=false;
    int base = 10;
    QString val = str.trimmed();
    if ((val.length()>2) && (val.left(2).toLower()=="0x")) {
        base = 16;
        val.remove(0,2);
    }
    qint32 n = val.toLong(&ok, base);
    if (ok) return n;
    else return def;
}
//=======================================================================================================
quint32 Convert::strToIntDef(const QString &str, quint32 def)
{
    bool ok=false;
    int base = 10;
    QString val = str.trimmed();
    if ((val.length()>2) && (val.left(2).toLower()=="0x")) {
        base = 16;
        val.remove(0,2);
    }
    quint32 n = val.toULong(&ok, base);
    if (ok) return n;
    else return def;
}
//=======================================================================================================
qint64 Convert::strToIntDef(const QString &str, qint64 def)
{
    bool ok=false;
    int base = 10;
    QString val = str.trimmed();
    if ((val.length()>2) && (val.left(2).toLower()=="0x")) {
        base = 16;
        val.remove(0,2);
    }
    qint64 n = val.toLongLong(&ok, base);
    if (ok) return n;
    else return def;
}
//=======================================================================================================
quint64 Convert::strToIntDef(const QString &str, quint64 def)
{
    bool ok=false;
    int base = 10;
    QString val = str.trimmed();
    if ((val.length()>2) && (val.left(2).toLower()=="0x")) {
        base = 16;
        val.remove(0,2);
    }
    quint64 n = val.toULongLong(&ok, base);
    if (ok) return n;
    else return def;
}
//=======================================================================================================
QString Convert::endLineToHTML(const QString &str) {
    QString res = str;

    int i = res.indexOf("\n");
    while (i>=0) {
        res = res.left(i) + "<br>" + res.right( res.length()-i-1 );
        i = res.indexOf("\n");
    }
    return res;
}
//=======================================================================================================
QString Convert::doubleToStr(double val, int precise)
{
    if(qIsNaN(val)) return "NaN";
    if(qIsInf(val)) return "Inf";
    QLocale c(QLocale::C);
    c.setNumberOptions( QLocale::OmitGroupSeparator );
    QString valStr = qIsFinite(val) ? c.toString( val, 'f', precise) : "";
    if(valStr.isNull()) return "";
    bool ok;
    double d=strToDouble(valStr, &ok);
    Q_UNUSED(d);
    return  ok ? valStr : "";
}
//=======================================================================================================
double Convert::strToDouble(const QString &str, bool *ok /*= Q_NULLPTR*/ )
{
    QLocale c(QLocale::C);
    bool err;
    double val = c.toDouble( str, &err );
    if(!err) val = 0;
    if(ok) *ok = err;
    return val;
}
//=======================================================================================================
QDateTime Convert::strToDateTime(const QString &str)
{
    QString dtStr = str.trimmed();
    QStringList lst = dtStr.split(' ', QString::SkipEmptyParts);
    if(lst.size() != 2) return QDateTime::fromMSecsSinceEpoch(-1);

    int y=0, m=0, d=0, h=0, n=0, s=0;
    if(dtStr.contains('-')) {
        QStringList dtLst = QString( lst.first() ).split('-', QString::SkipEmptyParts);
        if(dtLst.size() != 3) return QDateTime::fromMSecsSinceEpoch(-1);
        y = strToIntDef(dtLst.at(0), -1);
        m = strToIntDef(dtLst.at(1), -1);
        d = strToIntDef(dtLst.at(2), -1);
    }
    else if(dtStr.contains('.')) {
        QStringList dtLst = QString( lst.first() ).split('.', QString::SkipEmptyParts);
        if(dtLst.size() != 3) return QDateTime::fromMSecsSinceEpoch(-1);
        y = strToIntDef(dtLst.at(2), -1);
        m = strToIntDef(dtLst.at(1), -1);
        d = strToIntDef(dtLst.at(0), -1);
    }

    if(y>0 && y<1000) y += 2000;

    QStringList tmLst = QString( lst.last() ).split(':', QString::SkipEmptyParts);
    if(tmLst.size() < 2) return QDateTime::fromMSecsSinceEpoch(-1);

    h = strToIntDef(tmLst.at(0), -1);
    n = strToIntDef(tmLst.at(1), -1);
    if(tmLst.size() > 2) s = strToIntDef(tmLst.at(2), -1);

    return QDateTime( QDate(y, m, d), QTime(h, n, s) );
}
//=======================================================================================================
QString Convert::updatePrecise(const QString &val, int precise)
{
    QString str = val;
    QString znak = "";
    if(str[0] == '-') {
        znak = "-";
        str.remove(0,1);
    }

    if(precise < 0) {

        precise = 0 - precise;
        int n = str.indexOf(',');
        if(n >= 0) str[n] = '.';
        n = str.indexOf('.');
        QString s1 = "";
        if(n >= 0) {
            s1 = str.right( str.length() - n - 1 );
            str = str.left(n);
        }

        while(s1.length() < precise) s1 += "0";
        s1 = s1.left(precise);
        str += s1;
        while( !str.isEmpty() && (str.left(1) == "0") ) str.remove(0,1);
        if(str.isEmpty()) str = "0";
    }
    else if(precise > 0) {

        while (str.length() < precise) str = "0" + str;

        QString s1 = str.right(precise);
        str.remove(str.length()-precise, precise );

        while( !str.isEmpty() && (str.left(1) == "0") ) str.remove(0,1);
        if(str.isEmpty()) str = "0";

        while( !s1.isEmpty() && (s1.right(1) == "0") ) s1.remove( s1.length()-1, 1 );

        if(!s1.isEmpty()) str += "." + s1;

    }
    return znak + str;
}
//====================================================================================================
QString Convert::encodeXMLText(const QString &text)
{
    QString res = "";
    bool ok;

    for(int i=0; i<text.length(); i++) {
        for(int j=0; j<6; j++) {
            ok=false;
            if( text.at(i) == charXML[j] ) {
                ok = true;
                res += stringXML[j];
                break;
            }
        }
        if (!ok) res += text.mid(i,1);
    }

    return res;
}
//====================================================================================================
QString Convert::decodeXMLText(const QString &text)
{
    QString res = text;

    for(int j=0; j<6; j++) {
        int n = res.indexOf( stringXML[j] );
        while(n >= 0) {
            res.remove( n, stringXML[j].length() );
            res.insert(n, charXML[j]);
            n = res.indexOf( stringXML[j] );
        }
    }

    return res;
}
//====================================================================================================
QString Convert::changeDecimalSeparator(const QString &str)
{
    bool ok;
    QString res = str;
    double d = strToDouble(res.trimmed(), &ok);
    Q_UNUSED(d);
    if(ok) {
        QLocale c;
        QChar separator = c.toString( 0.1 ).at(1);
        QChar repSep = (separator == '.') ? ',' : '.';
        res.replace(repSep, separator);
    }
    return res;
}
//=====================================================================================================
QString Convert::changeLineFit(const QString &str, const QString &lineFit)
{
    QStringList sList1 = str.split("\r\n");
    QStringList sList2;
    for(int i=0; i<sList1.size(); i++) sList2.append( QString(sList1.at(i)).split("\r") );
    QStringList sList3;
    for(int i=0; i<sList2.size(); i++) sList3.append( QString(sList2.at(i)).split("\n") );
    QString res="";
    for(int i=0; i<sList3.size(); i++) {
        res += sList3.at(i);
        if(i<(sList3.size()-1)) res += lineFit;
    }
    return res;
}
//==========================================================================================
} // namespace nayk

