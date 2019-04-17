#ifndef HWUTILS_H
#define HWUTILS_H

#include <QtCore>

//=========================================================================================================
namespace nayk {
//=========================================================================================================

//=========================================================================================================
class HW
{
public:
    static int cpuCount();
    static bool cpuLoadPercent(QVector<qreal> &vecLoad, quint8 intervalMS=0);
    static bool memInfo(quint64 &memTotalKB, quint64 &memFreeKB, quint64 &memAvailableKB, quint64 &swapTotalKB, quint64 &swapFreeKB);
    static bool diskInfo(const QString &dev, quint64 &sizeKB, quint64 &freeKB);
    static bool diskIOKB(const QString &dev, qreal &ioKB);
    static bool netInfo(const QString &iface, qreal &rxKB, qreal &txKB);
    //
    HW() = delete; 
};

//=========================================================================================================
} // namespace nayk

//=========================================================================================================
#endif // HWUTILS_H
