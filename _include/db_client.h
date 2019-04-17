#ifndef DB_CLIENT_H
#define DB_CLIENT_H

#include <QtCore>
#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
//
#include "Log"

//=========================================================================================================
namespace nayk {
//=========================================================================================================

//=========================================================================================================
class DBClient : public QObject
{
    Q_OBJECT

public:
    explicit DBClient(QObject *parent = nullptr, const QString &settingsFileName = QString(), const QString &sqlDriver = QString("QPSQL"));
    ~DBClient();
    void setSettings(const QString &server, quint16 port, const QString &userName, const QString &password, const QString &baseName);
    void setSettingsFileName(const QString &settingsFileName);
    void setSqlDriver(const QString &sqlDriver);
    bool readSettings(const QString &settingsFileName = QString());
    bool writeSettings(const QString &settingsFileName = QString());
    bool open(bool reconnect = false);
    bool close();
    bool isOpen();

    const QString& lastError() { return _lastError; }
    const QString& settingsFileName() { return _settingsFileName; }
    const QString& sqlDriverName() { return _sqlDriver; }
    const QString& userName() { return _userName; }
    const QString& server() { return _server; }
    const QString& password() { return _password; }
    const QString& baseName() { return _baseName; }
    quint16 port() { return _port; }
    QSqlDatabase db() { return QSqlDatabase::database(_connectionName, false); }
    bool execSQL(const QString &sqlText, QSqlQuery *query, bool withTransaction = false);

signals:
    void toLog(LogType logType, QString text);
    void dbOpen();
    void dbClose();

private:
    QString _connectionName {"connect_" + QString::number(QDateTime::currentMSecsSinceEpoch())};
    QString _lastError {""};
    QString _settingsFileName {""};
    QString _sqlDriver {"QPSQL"};
    QString _server {"localhost"};
    quint16 _port {5432};
    QString _userName {""};
    QString _password {""};
    QString _baseName {""};
};

//=========================================================================================================
} // namespace nayk

//=========================================================================================================
#endif // DB_CLIENT_H
