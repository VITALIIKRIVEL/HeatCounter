#include <QDir>
#include <QLocale>
#include <QTimer>
#include <QEventLoop>
#include "global.h"
#include "wmbus.h"

namespace global {
/***************************************************************************************************/
void pause(int ms)
{
    QTimer timer;
    timer.setInterval(ms);
    timer.setSingleShot(true);
    QEventLoop loop;
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start();
    loop.exec();
}
/*==========================================================================*/
bool directoryExists(const QString &dirName)
{
    QDir dir( QDir::homePath() );
    return dir.exists(dirName);
}
/*==========================================================================*/
bool fileExists(const QString &fileName)
{
    return QFile::exists(fileName);
}
/*==========================================================================*/
bool makePath(const QString &dirPath)
{
    QDir dir;
    return dir.mkpath(dirPath);
}
/*==========================================================================*/
QString changeFileExt(const QString &fileName, const QString &newExt)
{
    int i = fileName.lastIndexOf('.');
    if( (i>=0) && (newExt.length() > 0) ) {
        if(newExt[0] == '.') return fileName.left( i ) + newExt;
        else return fileName.left( i ) +"."+ newExt;
    }
    else return fileName;
}
/*==========================================================================*/
QString extractFileExt(const QString &fileName, bool withDot)
{
    QString f = extractFileName(fileName);
    int i = f.lastIndexOf('.');
    if(i>=0) {
        if(withDot) return f.right( f.length() - i );
        else return f.right( f.length() - i - 1 );
    }
    else return "";
}
/*==========================================================================*/
QString extractFileName(const QString &fileName)
{
    QString f = QDir::toNativeSeparators(fileName);
    int i;
#ifdef Q_OS_WIN
    i = f.indexOf(':');
    if(i >= 0) f.remove(0,i+1);
#endif
    while((f.length()>0) && (f[f.length()-1] == QDir::separator()))
        f.remove(f.length()-1,1);

    i = f.lastIndexOf( QDir::separator() );
    if(i>=0) f.remove(0, i+1);
    return f;
}
/*==========================================================================*/
QString extractFilePath(const QString &fileName)
{
   QString f = QDir::toNativeSeparators(fileName);
   int i = f.lastIndexOf( QDir::separator() );
   if(i>=0) return f.left(i+1);
   else return "";
}
/*==========================================================================*/
bool isASCII(quint8 val)
{
    return ((val>=32) && (val<=126));
}
/*==========================================================================*/
bool isASCII(qint8 val)
{
    return ((val>=32) && (val<=126));
}
/*==========================================================================*/
bool isASCII(char val)
{
    return ((val>=32) && (val<=126));
}
/*==========================================================================*/
QString intToHex(quint8  val, int len /*= 2*/, bool withPrefix /*= false*/)
{
    QString prefix = withPrefix ? "0x" : "";
    QString res = QString::number(val, 16);
    while(res.length() < len) res = "0" + res;
    return prefix + res.toUpper().right(len);
}
/*==========================================================================*/
QString intToHex(qint8  val, int len /*= 2*/, bool withPrefix /*= false*/)
{
    return intToHex( static_cast<quint8>(val), len, withPrefix );
}
/*==========================================================================*/
QString intToHex(quint16 val, int len /*= 4*/, bool withPrefix /*= false*/)
{
    QString prefix = withPrefix ? "0x" : "";
    QString res = QString::number(val, 16);
    while(res.length() < len) res = "0" + res;
    return prefix + res.toUpper().right(len);
}
/*==========================================================================*/
QString intToHex(qint16  val, int len /*= 4*/, bool withPrefix /*= false*/)
{
    return intToHex( static_cast<quint16>(val), len, withPrefix );
}
/*==========================================================================*/
QString intToHex(quint32 val, int len /*= 8*/, bool withPrefix /*= false*/)
{
    QString prefix = withPrefix ? "0x" : "";
    QString res = QString::number(val, 16);
    while(res.length() < len) res = "0" + res;
    return prefix + res.toUpper().right(len);
}
/*==========================================================================*/
QString intToHex(qint32  val, int len /*= 8*/, bool withPrefix /*= false*/)
{
    return intToHex( static_cast<quint32>(val), len, withPrefix );
}
/*==========================================================================*/
QString intToHex(quint64 val, int len /*= 16*/, bool withPrefix /*= false*/)
{
    QString prefix = withPrefix ? "0x" : "";
    QString res = QString::number(val, 16);
    while(res.length() < len) res = "0" + res;
    return prefix + res.toUpper().right(len);
}
/*==========================================================================*/
QString intToHex(qint64  val, int len /*= 16*/, bool withPrefix /*= false*/)
{
    return intToHex( static_cast<quint64>(val), len, withPrefix );
}
/*==========================================================================*/
quint8  lo(quint16 val)
{
    return static_cast<quint8>( val & 0x00FF );
}
/*==========================================================================*/
quint16 lo(quint32 val)
{
    return static_cast<quint16>( val & 0x0000FFFF );
}
/*==========================================================================*/
quint32 lo(quint64 val)
{
    return static_cast<quint32>( val & 0x00000000FFFFFFFF );
}
/*==========================================================================*/
quint8  hi(quint16 val)
{
    return static_cast<quint8>( (val & 0xFF00) >> 8 );
}
/*==========================================================================*/
quint16 hi(quint32 val)
{
    return static_cast<quint16>( (val & 0xFFFF0000) >> 16 );
}
/*==========================================================================*/
quint32 hi(quint64 val)
{
    return static_cast<quint32>( (val & 0xFFFFFFFF00000000) >> 32 );
}
/*==========================================================================*/
quint16 reverse(quint16 val)
{
    return ((val & 0x00FF) << 8) | ((val & 0xFF00) >> 8);
}
/*==========================================================================*/
quint32 reverse(quint32 val)
{
    return ((val & 0x000000FF) << 24) | ((val & 0x0000FF00) << 8) |
            ((val & 0xFF000000) >> 24) | ((val & 0x00FF0000) >> 8);
}
/*==========================================================================*/
quint64 reverse(quint64 val)
{
    return (( val & 0x00000000000000FF)  << 56) | ((val & 0x000000000000FF00) << 40) |
            ((val & 0x0000000000FF0000) << 24)  | ((val & 0x00000000FF000000) << 8)  |
            ((val & 0x000000FF00000000) >> 8)   | ((val & 0x0000FF0000000000) >> 24) |
            ((val & 0x00FF000000000000) >> 40)  | ((val & 0xFF00000000000000) >> 56);
}
/*==========================================================================*/
qint8 strToIntDef(const QString &str, qint8 def)
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
/***************************************************************************************************/
quint8 strToIntDef(const QString &str, quint8 def)
{
    return static_cast<quint8>( strToIntDef(str, (qint8)def) );
}
/***************************************************************************************************/
qint16 strToIntDef(const QString &str, qint16 def)
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
/***************************************************************************************************/
quint16 strToIntDef(const QString &str, quint16 def)
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
/***************************************************************************************************/
qint32 strToIntDef(const QString &str, qint32 def)
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
/***************************************************************************************************/
quint32 strToIntDef(const QString &str, quint32 def)
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
/***************************************************************************************************/
qint64 strToIntDef(const QString &str, qint64 def)
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
/***************************************************************************************************/
quint64 strToIntDef(const QString &str, quint64 def)
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
/***************************************************************************************************/
bool isSummerTime(QDateTime dateTime)
{   // летнее время в РФ отменено
    Q_UNUSED( dateTime );

    return false;
}
/***************************************************************************************************/
QString strToHTML(const QString &str) {
    QString res = str;

    int i = res.indexOf("\n");
    while (i>=0) {
        res = res.left(i) + "<br>" + res.right( res.length()-i-1 );
        i = res.indexOf("\n");
    }
    return res;
}
/***************************************************************************************************/
bool isValidLogin(const QString &login)
{
    if(login.isNull() || login.length()<3) return false;
    for(int i=0; i<login.length(); i++) {
        char c = login.at(i).toLatin1();
        if(!(((c >= 'A') && (c <= 'z')) || ( (i>0) && ( ((c >= '0') && (c <= '9')) ||
           (c == '-') || (c == '_') || (c == '.') ) ) )) {
            return false;
        }
    }
    return true;
}
/***************************************************************************************************/
bool isValidEMail(const QString &email)
{
    if(email.isNull() || email.length()<5) return false;
    int n = email.indexOf('@');
    return ( (n>0) && (n<email.length()-1) && (email.indexOf('@', n+1)<0) && (email.indexOf('.', n+2)>n) );
}
/***************************************************************************************************/
int getMaxPrecise(const QString &val1, const QString &val2)
{
    int n1 = val1.indexOf('.');
    int prec1 = (n1 >= 0) ? (val1.length() - n1 - 1) : 0;
    int n2 = val2.indexOf('.');
    int prec2 = (n2 >= 0) ? (val2.length() - n2 - 1) : 0;

    return (prec1 > prec2) ? prec1 : prec2;
}
/***************************************************************************************************/
QString doubleToStr(double val, int precise)
{
    if(precise<1) return QString::number( qRound64(val) );

    QLocale c(QLocale::C);
    c.setNumberOptions( QLocale::OmitGroupSeparator );
    QString valStr = c.toString( val, 'f', precise);
    if(valStr.isNull() || valStr.isEmpty() || (valStr.toUpper()=="NAN")) return "NaN";
    return  wmbus::delPostNulls(wmbus::delPrevNulls(valStr));
}
/***************************************************************************************************/
double  strToDouble(const QString &str, bool *ok /*= Q_NULLPTR*/ )
{
    QLocale c(QLocale::C);
    bool err;
    double val = c.toDouble( str, &err );
    if(!err) val = 0;
    if(ok) *ok = err;
    return val;
}
/***************************************************************************************************/
} // namespace global

/***************************************************************************************************/
