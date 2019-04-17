#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <QObject>
#include <QtNetwork>
#include <QByteArray>
#include <QString>
#include <QJsonDocument>

#include "http.h"
#include "convert.h"

//===================================================================================================
namespace nayk {

enum ProxyType { ProxyHTTP, ProxyHTTPS, ProxySOCKS4, ProxySOCKS5 };

//===================================================================================================
class HttpClient : public QObject
{
    Q_OBJECT

public:
    explicit HttpClient(QObject *parent = 0);
    virtual ~HttpClient();    
    virtual bool sendRequest();
    virtual bool sendRequestAPI();
    virtual bool sendRequestMultipart(QHttpMultiPart *multiPart);
    virtual bool sendRequestHttp(bool getRequest = false);
    //
    QString lastError() const { return _lastError; }
    void setProxySettings(bool useProxy = false, const QString &addr = "127.0.0.1", quint16 port = 3128,
                          ProxyType proxyType = ProxyHTTP, const QString &login = QString(), const QString &pas = QString() );
    void setRequestTimeOut(qint64 timeOut) { _requestTimeOut = timeOut; }
    void setURL(const QString &url) { _url = url; }
    void setFileNameForSave(const QString &fileName) { _fileName = fileName; }
    void setRequestData(const QByteArray &data) { _requestData = data; }
    void setContentType(const QString &contentType) { _contentType = contentType; }
    QString contentType() const { return _contentType; }
    QString url() const { return _url; }
    QByteArray replyData() const { return _answer; }
    QByteArray requestData() const { return _requestData; }
    bool sendRequest(qint64 maxWaitTime);
    bool sendRequest(const QByteArray &jsonData);
    bool sendRequest(const QByteArray &jsonData, qint64 maxWaitTime);
    bool sendRequest(const QString &url, const QByteArray &jsonData, qint64 maxWaitTime);
    bool sendRequest(const QString &url);
    bool sendRequestAPI(const QString &cmd);
    bool sendRequestAPI(const QStringList &paramsList);
    bool sendRequestAPI(const QByteArray &jsonData);    
    void addParam(const QString &paramName, const QString &paramValue) { _params[paramName] = paramValue; }
    void addParam(const QString &paramName, qint32 paramValue) { _params[paramName] = QString::number( paramValue ); }
    void addParam(const QString &paramName, qint64 paramValue) { _params[paramName] = QString::number( paramValue ); }
    void addParam(const QString &paramName, double paramValue) { _params[paramName] = Convert::doubleToStr(paramValue, 8); }
    void addParam(const QString &paramName, QDate paramValue) { _params[paramName] = paramValue.toString("yyyy-MM-dd"); }
    void addParam(const QString &paramName, QDateTime paramValue) { _params[paramName] = paramValue.toString("yyyy-MM-dd HH:mm:ss"); }
    QJsonDocument jsonAnswer();
    void clearParams() { _params.clear(); }    
    //
    static bool downloadData(const QString &url, QByteArray &data, qint64 maxWaitTime = 10000,
                             const QMap<QString, QString> &headers = QMap<QString,QString>(),
                             const QMap<QString,QString> &post = QMap<QString,QString>());
    static bool downloadFile(const QString &url, const QString &fileName, qint64 maxWaitTime=0);
    static bool downloadData(const QString &url, const QJsonObject &jsonRequest, QJsonObject &jsonAnswer,
                             qint64 maxWaitTime = 10000, const QMap<QString, QString> &headers = QMap<QString,QString>());
    static bool simpleGet(const QString &url, QString &resultString);
    static bool externalIP(QString &ip);
signals:
    void abortRequest();

private:
    QString _url {""};
    QString _fileName {""};
    QByteArray _requestData;
    qint64 _requestTimeOut {10000};
    bool _useProxy {false};
    QNetworkProxy _proxy;
    QString _contentType {ContentTypeJSON};
    QMap<QString, QString> _params;
    //
    QByteArray requestParamsData() const;

protected:
    QString _lastError {""};
    QByteArray _answer;

};
//===================================================================================================
} // namespace nayk
#endif // HTTP_CLIENT_H
