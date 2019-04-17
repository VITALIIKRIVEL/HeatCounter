#include <QTimer>
#include <QEventLoop>
#include <QProcess>

#ifdef Q_OS_WIN32
#include <windows.h>
#include <versionhelpers.h>
#endif

#ifdef Q_OS_LINUX
#include <unistd.h>
#include <sys/utsname.h>
#endif

#include "system_utils.h"

namespace nayk {
//=======================================================================================================
void System::pause(int ms)
{
    QTimer timer;
    timer.setInterval(ms < 0 ? 1 : ms);
    timer.setSingleShot(true);
    QEventLoop loop;
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start();
    loop.exec();
}
//====================================================================================================
bool System::slotExists(QObject *obj, const char *slot_sign)
{
    if(!obj) return false;
    return (obj->metaObject()->indexOfSlot(QMetaObject::normalizedSignature(qPrintable(slot_sign))) >= 0);
}
//====================================================================================================
bool System::isSummerTime(QDateTime dateTime)
{   // летнее время в РФ отменено
    Q_UNUSED( dateTime );

    return false;
}
//============================================================================================
int System::dayOfWeek_Sakamoto(int year, int month, int day)
{
    if(year <= 1752) return -1;
    if((month < 1) || (month > 12)) return -1;
    if((day < 1) || (day>31)) return -1;

    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    year -= month < 3;
    int dow = (year + year/4 - year/100 + year/400 + t[month-1] + day) % 7;
    if(dow == 0) dow=7;
    if(( dow < 0 ) || ( dow > 7 )) return -1;
    return dow;
}
//============================================================================================
qint64 System::floordiv(qint64 a, int b)
{
    return (a - (a < 0 ? b - 1 : 0)) / b;
}
//========================================================================================
int System::floordiv(int a, int b)
{
    return (a - (a < 0 ? b - 1 : 0)) / b;
}
//========================================================================================
qint64 System::julianDayFromDate(int year, int month, int day)
{
    // Adjust for no year 0
    if (year < 0) ++year;

/*
 * Math from The Calendar FAQ at http://www.tondering.dk/claus/cal/julperiod.php
 * This formula is correct for all julian days, when using mathematical integer
 * division (round to negative infinity), not c++11 integer division (round to zero)
 */
    int    a = floordiv(14 - month, 12);
    qint64 y = (qint64)year + 4800 - a;
    int    m = month + 12 * a - 3;
    return day + floordiv(153 * m + 2, 5) + 365 * y + floordiv(y, 4) - floordiv(y, 100) + floordiv(y, 400) - 32045;
}
//========================================================================================
int System::dayOfWeek(int year, int month, int day)
{
    qint64 jd = julianDayFromDate(year, month, day);

    if (jd >= 0)
        return (jd % 7) + 1;
    else
        return ((jd + 1) % 7) + 7;
}
//========================================================================================
qint32 System::generateId32()
{
    return static_cast<qint32>( generateId64() & 0x7fffffff );
}
//========================================================================================
qint64 System::generateId64()
{
    return QDateTime::currentDateTimeUtc().msecsTo( QDateTime(QDate(2033, 3, 27), QTime(4, 10, 25, 666)) );
}
//========================================================================================
QString System::generatePas(int len, bool onlyNum)
{
    const QString letters = "qwertyupasdfghjkzxcvbnm";
    const QString numbers = "0123456789";
    QString res = "";
    if (len<4) len=4;

    if(onlyNum) {
        for(int i=0; i<len; i++) {
            res += numbers.mid( qrand() % numbers.length(), 1);
        }
        return res;
    }

    for(int i=0; i<len; i++) {
        res += letters.mid( qrand() % letters.length(), 1);
        if( qrand() % 3 == 0 ) res[ res.length()-1 ] = res.at( res.length()-1 ).toUpper();
    }

    int maxCnt = 3;
    if(len>14) maxCnt = 5;
    else if(len>10) maxCnt = 4;

    int cnt=0;
    int n;

    while (cnt<maxCnt) {
        n = qrand() % len;
        if(!numbers.contains( res.at(n) )) {
            res[n] = numbers.at( qrand() % numbers.length() );
            cnt++;
        }
    }
    return res;
}
//============================================================================================
bool System::osCmd(const QString &cmd, QString &out, int timeout)
{
    QProcess process;
#ifdef Q_OS_LINUX
    process.start(QString("bash -c \"%1\"").arg(cmd));
#else
    process.start(QString("cmd -c \"%1\"").arg(cmd));
#endif
    timeout = qBound(300, timeout, 300000);
    if (!process.waitForStarted(timeout)) {
        out = "Timeout wait for started";
        return false;
    }
    if (!process.waitForFinished(timeout)) {
        out = "Timeout wait for finished";
        return false;
    }
    out = QString(process.readAllStandardOutput()).trimmed();
    return true;
}
//============================================================================================
QString System::osName()
{
    QString res = "";

#ifdef Q_OS_WIN32
    res = "Windows";
#endif
#ifdef Q_OS_LINUX
    res = "Linux";
#endif
#ifdef Q_OS_MACOS
    res = "MacOS";
#endif
#ifdef Q_OS_UNIX
    res = "Unix";
#endif
    return res;
}
//============================================================================================
QString System::osVersion()
{
    QString res = "";

#ifdef Q_OS_WIN32
    if (IsWindowsServer()) return "Server";
//    if (IsWindows10OrGreater()) return "10";
    if (IsWindows8Point1OrGreater()) return "8.1";
    if (IsWindows8OrGreater())return "8";
    if (IsWindows7SP1OrGreater()) return "7 SP1";
    if (IsWindows7OrGreater()) return "7";
    if (IsWindowsVistaOrGreater()) return "Vista";
    if (IsWindowsXPOrGreater()) return "XP";
#endif

#ifdef Q_OS_LINUX
    utsname kernelInfo;
    uname(&kernelInfo);

#endif

    return res;
}
//============================================================================================
bool System::os64bit()
{
    bool res = false;
#ifdef Q_OS_WIN32
    BOOL is64bit = FALSE;
    IsWow64Process( GetCurrentProcess(), &is64bit );
    res = (bool)is64bit;
#endif
    return res;
}
//============================================================================================
} // namespace nayk

