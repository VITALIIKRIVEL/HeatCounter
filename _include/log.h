#ifndef LOGSAVER_H
#define LOGSAVER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
//======================================================================================================
namespace nayk {
//======================================================================================================
enum LogType { LogInfo, LogWarning, LogError, LogIn, LogOut, LogText, LogDbg, LogOther };
enum LogFormat { PlainLog, RichLog, HtmlLog };

//======================================================================================================
class Log : public QObject
{
    Q_OBJECT

public:
    explicit Log(QObject *parent = 0, QDateTime startTime = QDateTime(), const QString &logDir = QString(), bool startLogging = false );
    ~Log();
    bool startLog(const QString &fileName = QString(), bool saveDbg = true);
    void setDbgSave(bool on = true) { _dbg = on; }
    bool dbgSave() { return _dbg; }
    bool write(LogType logType, const QString &text);
    QString logFileName() const { return logPath + logFile; }
    QString lastError() const { return _lastError; }
    static QString highlightLog(LogFormat logFormat, const QString &logText, bool darkBackground = false);
    static QString highlightLogString(LogFormat logFormat, LogType logType, const QString &text, bool darkBackground = false);
    static QString readLog(LogFormat logFormat, const QString &fileName, bool darkBackground = false);
    static QStringList logFiles(const QString &logDir);
    static QStringList logFiles(const QString &rootLogDir, QDate date);

private:
    QDateTime startingTime;
    QString logFile;
    QString logPath;
    QFile file;
    QTextStream stream;
    QString _lastError;
    bool _dbg {true};

    bool writeFirstLine();
    bool writeLastLine();

signals:
    void openFile(QString);
    void closeFile(QString);

public slots:
    void onLog(LogType logType, QString text);
};
//======================================================================================================
} // namespace nayk
#endif // LOGSAVER_H
