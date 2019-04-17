#ifndef SYSUTILS_H
#define SYSUTILS_H

#include <QtCore>

//=========================================================================================================
namespace nayk {
//=========================================================================================================

//=========================================================================================================
class System
{
public:
    static void pause(int ms = 1000);
    static void pauseMS(int ms = 1000) { pause(ms); }
    static void pauseSec(int sec = 1) { pause(sec * 1000); }
    static void pauseMin(int min = 1) { pauseSec(min * 60); }
    static bool slotExists(QObject *obj, const char *slot_sign);
    static bool isSummerTime(QDateTime dateTime);
    static int dayOfWeek_Sakamoto(int year, int month, int day);
    static int dayOfWeek(int year, int month, int day);
    static QString generatePas(int len = 8, bool onlyNum = false);
    static qint32 generateId32();
    static qint64 generateId64();
    static qint64 floordiv(qint64 a, int b);
    static int floordiv(int a, int b);
    static qint64 julianDayFromDate(int year, int month, int day);
    static bool osCmd(const QString &cmd, QString &out, int timeout=5000);
    static QString osName();
    static QString osVersion();
    static bool os64bit();
    //
    System() = delete; // < std=c++11, обеспечивает запрет на создание любого экземпляра System
};

//=========================================================================================================
} // namespace nayk

//=========================================================================================================
#endif // SYSUTILS_H
