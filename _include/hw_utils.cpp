#include <QTimer>
#include <QEventLoop>
#include <QThread>
#include <QFile>
#include <QTextStream>
//
#ifdef Q_OS_LINUX
#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>
#include <time.h>
#endif
//
#include "system_utils.h"
#include "hw_utils.h"

namespace nayk {
//=======================================================================================================
#ifdef Q_OS_LINUX
typedef struct proc_info_t {
    qreal usage[16];
} proc_info_t;
//=======================================================================================================
bool getDiskIOLoad(const QString &dev, double &io)
{
    FILE *fp;
    if (!(fp = fopen("/proc/diskstats", "r"))) {
        return false;
    }

    bool res = false;
    char name[1024];
    while(fgets(name, sizeof(name)-1, fp) ) {
        QString str(name);
        if( !str.contains( " " + dev + " " ) ) continue;
        QStringList sl = str.split( " ", QString::SkipEmptyParts );
        if( sl.size() < 10 ) break;
        bool ok;
        qint64 sectors = QString( sl.at(5) ).toLongLong(&ok);
        if(!ok) break;
        sectors += QString( sl.at(9) ).toLongLong(&ok);
        if(!ok) break;
        io = sectors/2.0;
        res = true;
    }
    fclose(fp);
    return res;
}
//=======================================================================================================
bool getNetInfo(const QString &iface, double &rxKB, double &txKB)
{
    FILE *fp;
    if (!(fp = fopen("/proc/net/dev", "r"))) {
        return false;
    }

    bool res = false;
    char name[1024];
    while(fgets(name, sizeof(name)-1, fp) ) {
        QString str(name);
        if( !str.contains( iface + ":" ) ) continue;
        QStringList sl = str.split( " ", QString::SkipEmptyParts );
        if( sl.size() < 10 ) break;
        bool ok;
        quint64 rx = QString( sl.at(1) ).toULongLong(&ok);
        if(!ok) break;
        quint64 tx = QString( sl.at(9) ).toULongLong(&ok);
        if(!ok) break;

        rxKB = (qreal)rx/1024.0;
        txKB = (qreal)tx/1024.0;

        res = true;
        break;
    }
    fclose(fp);
    return res;
}
//=======================================================================================================
int getProcessorLoad(proc_info_t *info)
{
    static unsigned int pre_total[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, pre_used[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    unsigned int cpu[16], nice[16], system[16], idle[16];

    FILE *fp;
    if (!(fp = fopen("/proc/stat", "r"))) {
        return 0;
    }

    int count = 0;
    char name[1024];
    while(fgets(name, sizeof(name)-1, fp) && count<16) {
        if((name[0] != 'c') || (name[1] != 'p') || (name[2] != 'u')) break;
        sscanf(name, "%*s %u %u %u %u", &cpu[count], &nice[count], &system[count], &idle[count]);
        count++;
    }
    fclose(fp);

    unsigned int used[16];
    unsigned int total[16];
    for(int k = 0; k < count; k++) {
        used[k] = cpu[k] + system[k] + nice[k];
        total[k] = cpu[k] + nice[k] + system[k] + idle[k];
    }

    for(int k = 0; k < count; k++) {
        if((pre_total[k] == 0)||(pre_used[k] == 0)) {
            info->usage[k] = 0.0;
        }
        else {
            info->usage[k] = ((100.0 * (qreal)(used[k] - pre_used[k]))/(qreal)(total[k] - pre_total[k]));
        }

        pre_used[k] = used[k];
        pre_total[k] = total[k];
    }
    return count;
}
//=======================================================================================================
quint64 getMemValueFromStr(const QString &str)
{
    int n = str.indexOf(" ");
    if(n < 1) return 0;
    bool ok;
    quint64 val = str.mid(0, n).trimmed().toULongLong(&ok);
    if(!ok) return 0;
    QString ed = str.mid(n).trimmed().toLower().left(1);
    if(ed == "b") val /= 1024;
    else if(ed == "m") val *= 1024;
    else if(ed == "g") val *= 1024*1024;
    return val;
}
#endif
//=======================================================================================================
int HW::cpuCount()
{
    return QThread::idealThreadCount();
}
//=======================================================================================================
bool HW::cpuLoadPercent(QVector<qreal> &vecLoad, quint8 intervalMS)
{
    vecLoad.clear();
#ifdef Q_OS_LINUX
    proc_info_t info;
    int load = getProcessorLoad(&info);
    if(intervalMS > 0) {
        System::pauseMS(intervalMS);
        load = getProcessorLoad(&info);
    }
    for(int i=0; i<load; ++i) vecLoad.append( info.usage[i] );
#else
    Q_UNUSED(intervalMS);
#endif
    return true;
}
//=======================================================================================================
bool HW::memInfo(quint64 &memTotalKB, quint64 &memFreeKB, quint64 &memAvailableKB, quint64 &swapTotalKB, quint64 &swapFreeKB)
{
    memTotalKB = 0;
    memFreeKB = 0;
    swapTotalKB = 0;
    swapFreeKB = 0;
    memAvailableKB = 0;

#ifdef Q_OS_LINUX

    FILE *fp;
    if (!(fp = fopen("/proc/meminfo", "r"))) {
        return false;
    }

    char name[512];
    while(fgets(name, sizeof(name)-1, fp)) {
        QString str(name);

        if(str.indexOf("MemTotal:") == 0) {
            memTotalKB = getMemValueFromStr(str.mid(9).trimmed());
        }
        else if(str.indexOf("MemFree:") == 0) {
            memFreeKB = getMemValueFromStr(str.mid(8).trimmed());
        }
        else if(str.indexOf("MemAvailable:") == 0) {
            memAvailableKB = getMemValueFromStr(str.mid(13).trimmed());
        }
        else if(str.indexOf("SwapTotal:") == 0) {
            swapTotalKB = getMemValueFromStr(str.mid(10).trimmed());
        }
        else if(str.indexOf("SwapFree:") == 0) {
            swapFreeKB = getMemValueFromStr(str.mid(9).trimmed());
        }
    }
    fclose(fp);

#endif
    return true;
}
//=======================================================================================================
bool HW::diskInfo(const QString &dev, quint64 &sizeKB, quint64 &freeKB)
{
    sizeKB = 0;
    freeKB = 0;

#ifdef Q_OS_LINUX

    QString outStr;
    if(!System::osCmd("df -k --output=target,size,avail | grep '" + dev + " '", outStr, 300)) return false;
    QStringList sl = outStr.split(" ", QString::SkipEmptyParts);
    if(sl.size() < 3) return false;
    if(sl.at(0) != dev) return false;
    bool ok;
    sizeKB = QString(sl.at(1)).toULongLong(&ok);
    if(!ok) return false;
    freeKB = QString(sl.at(2)).toULongLong(&ok);
    if(!ok) return false;

#else
    Q_UNUSED(dev);
#endif

    return true;
}
//=======================================================================================================
bool HW::diskIOKB(const QString &dev, qreal &ioKB)
{
    ioKB = 0.0;

#ifdef Q_OS_LINUX
    return getDiskIOLoad(dev, ioKB);
#else
    Q_UNUSED(dev);
#endif
    return true;
}
//=======================================================================================================
bool HW::netInfo(const QString &iface, qreal &rxKB, qreal &txKB)
{
    rxKB = 0;
    txKB = 0;

#ifdef Q_OS_LINUX
    return getNetInfo(iface, rxKB, txKB);
#else
    Q_UNUSED(iface);
#endif

    return true;
}
//=======================================================================================================
} // namespace nayk

