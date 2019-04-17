#include <QTimer>
#include <QEventLoop>
#include <QFile>
#include <QTextStream>

#include "http_client.h"

namespace nayk {
//----------------------------------------------------------------------------------
HttpClient::HttpClient(QObject *parent) : QObject(parent)
{
    setProxySettings();
}
//----------------------------------------------------------------------------------
HttpClient::~HttpClient()
{
    emit abortRequest();
}
//----------------------------------------------------------------------------------
void HttpClient::setProxySettings(bool useProxy, const QString &addr, quint16 port, ProxyType proxyType, const QString &login, const QString &pas)
{
    _useProxy = useProxy;
    _proxy.setHostName( addr );
    _proxy.setPort( port );
    _proxy.setType( _useProxy ?
                      ((proxyType == ProxyHTTP) ? QNetworkProxy::HttpProxy : QNetworkProxy::Socks5Proxy)
                      : QNetworkProxy::NoProxy );
    _proxy.setUser( login );
    _proxy.setPassword( pas );
}
//----------------------------------------------------------------------------------
bool HttpClient::sendRequestMultipart(QHttpMultiPart *multiPart)
{
    QEventLoop loop;
    QTimer timer;
    timer.setInterval(_requestTimeOut);
    timer.setSingleShot(true);

    _answer.clear();
    _lastError = "";

    QNetworkRequest request{ QUrl { _url } };    
    request.setSslConfiguration( QSslConfiguration::defaultConfiguration() );

    QNetworkAccessManager manager;

    if(_useProxy) { // настройки прокси если включен
        manager.setProxy(_proxy);
    }

    QNetworkReply* reply = manager.post(request, multiPart);

    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    QObject::connect(&timer, &QTimer::timeout, reply, &QNetworkReply::abort);
    QObject::connect(this, &HttpClient::abortRequest, reply, &QNetworkReply::abort);
    QObject::connect(this, &HttpClient::abortRequest, &timer, &QTimer::stop);
    QObject::connect(this, &HttpClient::abortRequest, &loop, &QEventLoop::quit);

    timer.start();
    loop.exec();

    if (reply->isFinished() && reply->error() == QNetworkReply::NoError)
    {
        _answer = reply->readAll();
    }
    else
    {
        _lastError = reply->errorString();
    }

    if(!_fileName.isEmpty()) {
        QFile file(_fileName);
        if(file.open(QIODevice::WriteOnly)) {
            QTextStream stream( &file );
            if(_lastError.isEmpty())
                stream << QString::fromUtf8( _answer );
            else
                stream << _lastError << "\n";
            file.close();
        }
    }

    reply->deleteLater();

    return _lastError.isEmpty();
}
//-----------------------------------------------------------------------------------
bool HttpClient::sendRequest()
{
    QEventLoop loop;
    QTimer timer;
    timer.setInterval(_requestTimeOut);
    timer.setSingleShot(true);

    _answer.clear();
    _lastError = "";

    QNetworkRequest request{ QUrl { _url } };
    request.setHeader(QNetworkRequest::ContentTypeHeader, _contentType);
    request.setSslConfiguration( QSslConfiguration::defaultConfiguration() );

    QNetworkAccessManager manager;

    if(_useProxy) { // настройки прокси если включен
        manager.setProxy(_proxy);
    }

    QNetworkReply* reply = manager.post(request, _requestData);

    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    QObject::connect(&timer, &QTimer::timeout, reply, &QNetworkReply::abort);
    QObject::connect(this, &HttpClient::abortRequest, reply, &QNetworkReply::abort);
    QObject::connect(this, &HttpClient::abortRequest, &timer, &QTimer::stop);
    QObject::connect(this, &HttpClient::abortRequest, &loop, &QEventLoop::quit);

    timer.start();
    loop.exec();

    if (reply->isFinished() && reply->error() == QNetworkReply::NoError)
    {
        _answer = reply->readAll();
    }
    else
    {
        _lastError = reply->errorString();
    }

    if(!_fileName.isEmpty()) {
        QFile file(_fileName);
        if(file.open(QIODevice::WriteOnly)) {
            QTextStream stream( &file );
            if(_lastError.isEmpty())
                stream << QString::fromUtf8( _answer );
            else
                stream << _lastError << "\n";
            file.close();
        }
    }

    reply->deleteLater();

    return _lastError.isEmpty();
}
//----------------------------------------------------------------------------------
QByteArray HttpClient::requestParamsData() const
{
    auto b = _params.begin();
    auto e = _params.end();

    QByteArray byteArrayData;

    while (b != e) {

        byteArrayData.append(b.key());
        byteArrayData.append('=');
        byteArrayData.append( QUrl::toPercentEncoding( b.value() )  );
        byteArrayData.append('&');

        b++;
    }

    byteArrayData.chop(1);
    return byteArrayData;
}
//----------------------------------------------------------------------------------
bool HttpClient::sendRequestHttp(bool getRequest)
{
    QEventLoop loop;
    QTimer timer;
    timer.setInterval(_requestTimeOut);
    timer.setSingleShot(true);

    _answer.clear();
    _lastError = "";

    QUrl url(_url);
    if(getRequest) url.setQuery( requestParamsData() );

    QNetworkRequest request( url );
    if(!getRequest) request.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeWWWForm);
    request.setSslConfiguration( QSslConfiguration::defaultConfiguration() );

    QNetworkAccessManager manager;

    if(_useProxy) { // настройки прокси если включен
        manager.setProxy(_proxy);
    }

    QNetworkReply* reply = getRequest ? manager.get(request) : manager.post(request, requestParamsData());

    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    QObject::connect(&timer, &QTimer::timeout, reply, &QNetworkReply::abort);
    QObject::connect(this, &HttpClient::abortRequest, reply, &QNetworkReply::abort);
    QObject::connect(this, &HttpClient::abortRequest, &timer, &QTimer::stop);
    QObject::connect(this, &HttpClient::abortRequest, &loop, &QEventLoop::quit);

    timer.start();
    loop.exec();

    if (reply->isFinished() && reply->error() == QNetworkReply::NoError)
    {
        _answer = reply->readAll();
    }
    else
    {
        _lastError = reply->errorString();
    }

    if(!_fileName.isEmpty()) {
        QFile file(_fileName);
        if(file.open(QIODevice::WriteOnly)) {
            QTextStream stream( &file );
            if(_lastError.isEmpty())
                stream << QString::fromUtf8( _answer );
            else
                stream << _lastError << "\n";
            file.close();
        }
    }

    reply->deleteLater();

    return _lastError.isEmpty();
}
//----------------------------------------------------------------------------------
bool HttpClient::sendRequest(qint64 maxWaitTime)
{
    _requestTimeOut = maxWaitTime;
    return sendRequest();
}
//----------------------------------------------------------------------------------
bool HttpClient::sendRequest(const QByteArray &jsonData)
{
    _requestData = jsonData;
    return sendRequest();
}
//----------------------------------------------------------------------------------
bool HttpClient::sendRequest(const QByteArray &jsonData, qint64 maxWaitTime)
{
    _requestTimeOut = maxWaitTime;
    _requestData = jsonData;
    return sendRequest();
}
//----------------------------------------------------------------------------------
bool HttpClient::sendRequest(const QString &url, const QByteArray &jsonData, qint64 maxWaitTime)
{
    _requestTimeOut = maxWaitTime;
    _requestData = jsonData;
    _url = url;
    return sendRequest();
}
//----------------------------------------------------------------------------------
bool HttpClient::sendRequest(const QString &url)
{
    _url = url;
    return sendRequest();
}
//----------------------------------------------------------------------------------
bool HttpClient::sendRequestAPI(const QByteArray &jsonData)
{
    _params.clear();
    _requestData = jsonData;
    return sendRequestAPI();
}
//----------------------------------------------------------------------------------
bool HttpClient::sendRequestAPI(const QString &cmd)
{
    _params.clear();
    addParam("cmd", cmd);
    return sendRequestAPI();
}
//----------------------------------------------------------------------------------
bool HttpClient::sendRequestAPI(const QStringList &paramsList)
{
    _params.clear();
    for(int i=0; i<paramsList.size(); i++) {
        QString str = paramsList.at(i);
        QStringList sl = str.split("=", QString::SkipEmptyParts);
        if(sl.size() == 2) addParam( sl.first(), sl.last() );
    }
    return sendRequestAPI();
}
//----------------------------------------------------------------------------------
bool HttpClient::sendRequestAPI()
{
    QEventLoop loop;
    QTimer timer;
    timer.setInterval(_requestTimeOut);
    timer.setSingleShot(true);

    _answer.clear();
    _lastError = "";

    QNetworkRequest request{ QUrl { _url } };    
    request.setSslConfiguration( QSslConfiguration::defaultConfiguration() );

    QNetworkAccessManager manager;

    if(_useProxy) { // настройки прокси если включен
        manager.setProxy(_proxy);
    }

    if(_params.count()) {
        _requestData.clear();
        QMap<QString, QString>::iterator itr;

        for (itr = _params.begin(); itr != _params.end(); ++itr) {
            _requestData.append(itr.key());
            _requestData.append('=');
            _requestData.append( QUrl::toPercentEncoding( itr.value() )  );
            _requestData.append('&');
        }
        _requestData.chop(1);
        request.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeWWWForm);
    }
    else {
        request.setHeader(QNetworkRequest::ContentTypeHeader, ContentTypeJSON);
    }

    QNetworkReply* reply = manager.post(request, _requestData);

    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    QObject::connect(&timer, &QTimer::timeout, reply, &QNetworkReply::abort);
    QObject::connect(this, &HttpClient::abortRequest, reply, &QNetworkReply::abort);
    QObject::connect(this, &HttpClient::abortRequest, &timer, &QTimer::stop);
    QObject::connect(this, &HttpClient::abortRequest, &loop, &QEventLoop::quit);

    timer.start();
    loop.exec();

    if (reply->isFinished() && (reply->error() == QNetworkReply::NoError))
    {
        _answer = reply->readAll();
    }
    else
    {
        _lastError = reply->errorString();
        if(_lastError.isNull() || _lastError.isEmpty()) _lastError = QObject::tr("Неизвестная ошибка.");
    }

    if(!_fileName.isEmpty()) {
        QFile file(_fileName);
        if(file.open(QIODevice::WriteOnly)) {
            QTextStream stream( &file );
            if(_lastError.isEmpty())
                stream << QString::fromUtf8( _answer );
            else
                stream << _lastError << "\n";
            file.close();
        }
    }

    reply->deleteLater();

    if(_lastError.isEmpty()) {
        QJsonDocument doc = QJsonDocument::fromJson( _answer );
        if(doc.isNull() || doc.isEmpty() || !doc.isObject()) {
            _lastError = QObject::tr("Ошибка при разборе ответа JSON.");
            return false;
        }
        QJsonObject obj = doc.object();
        if(!obj.contains("error") || !obj.contains("server") || !obj.value("error").isObject() || !obj.value("server").isObject()) {
            _lastError = QObject::tr("Некорректный ответ JSON.");
            return false;
        }
        QJsonObject err = obj.value("error").toObject();
        if(err.value("code").toInt(0) > 0) {
            _lastError = err.value("text").toString();
            if(_lastError.isNull() || _lastError.isEmpty()) _lastError = QObject::tr("Неизвестная ошибка.");
            return false;
        }
    }

    return _lastError.isEmpty();
}
//----------------------------------------------------------------------------------
QJsonDocument HttpClient::jsonAnswer()
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson( _answer, &error );

    if (error.error != QJsonParseError::NoError) {
        _lastError = error.errorString();
        return QJsonDocument();
    }
    return doc;
}
//=============================================================================================
bool HttpClient::downloadFile(const QString &url, const QString &fileName, qint64 maxWaitTime)
{
    QByteArray data;
    if(!downloadData( url, data, maxWaitTime )) return false;

    QFile file( fileName );
    if(!file.open( QIODevice::WriteOnly )) return false;

    file.write(data);

    file.close();
    return true;
}
//----------------------------------------------------------------------------------
bool HttpClient::externalIP(QString &ip)
{
    QStringList urls = { "https://icanhazip.com", "https://api.ipify.org", "http://smart-ip.net/myip",
                         "http://icanhazip.com", "http://api.ipify.org",
                         "http://grio.ru/myip.php", "http://ifconfig.me/ip" };
    for(auto i=0; i<urls.size(); ++i) {
        if(simpleGet( urls.at(i), ip )) {

            QStringList ip4 = ip.split('.', QString::SkipEmptyParts);
            if(ip4.size() != 4) continue;
            bool okIP = true;
            for(auto j=0; j<ip4.size(); j++) {
                bool ok;
                int n = QString(ip4.at(j)).toInt(&ok, 10);
                okIP = okIP && ok && (n>=0) && (n<=255);
                if(!okIP) break;
            }
            if(okIP) return true;
        }
    }
    return false;
}
//----------------------------------------------------------------------------------
bool HttpClient::simpleGet(const QString &url, QString &resultString)
{
    bool res = false;
    QEventLoop loop;
    QTimer timer;
    timer.setInterval(4000);
    timer.setSingleShot(true);

    QNetworkRequest request{ QUrl { url } };
    QNetworkAccessManager manager;

    QNetworkReply* reply = manager.get(request);
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    QObject::connect(&timer, &QTimer::timeout, reply, &QNetworkReply::abort);
    timer.start();
    loop.exec();
    if (reply->isFinished() && reply->error() == QNetworkReply::NoError) {

        resultString = QString(reply->readAll()).trimmed();
        res = true;
    }
    reply->deleteLater();
    return res;
}
//-----------------------------------------------------------------------------------
bool HttpClient::downloadData(const QString &url, QByteArray &data, qint64 maxWaitTime,
                            const QMap<QString,QString> &headers, const QMap<QString,QString> &post)
{
    bool res = false;
    QEventLoop loop;
    QTimer timer;
    timer.setInterval((maxWaitTime == 0) ? 2000 : maxWaitTime);
    timer.setSingleShot(true);

    data.clear();
    QNetworkRequest request{ QUrl { url } };

    if(!headers.isEmpty()) {
        foreach (QString strHeader, headers.keys()) {
            request.setRawHeader( strHeader.toUtf8(), QString(headers.value(strHeader)).toUtf8() );
        }
    }
    QByteArray postData;
    if(!post.isEmpty()) {
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        foreach (QString strPar, post.keys()) {
            postData.append( strPar );
            postData.append( "=" );
            postData.append( QUrl::toPercentEncoding( post.value(strPar) ) );
            postData.append( "&" );
        }
        postData.chop(1);
    }

    QNetworkAccessManager manager;

    QNetworkReply* reply = post.isEmpty() ? manager.get(request) : manager.post(request, postData);
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    QObject::connect(&timer, &QTimer::timeout, reply, &QNetworkReply::abort);
    timer.start();
    loop.exec();
    if (reply->isFinished() && reply->error() == QNetworkReply::NoError) {

        data = reply->readAll();
        res = true;
    }
    reply->deleteLater();
    return res;
}
//=============================================================================================
bool HttpClient::downloadData(const QString &url, const QJsonObject &jsonRequest, QJsonObject &jsonAnswer,
                             qint64 maxWaitTime, const QMap<QString, QString> &headers)
{
    bool res = false;
    QEventLoop loop;
    QTimer timer;
    timer.setInterval((maxWaitTime == 0) ? 2000 : maxWaitTime);
    timer.setSingleShot(true);

    QNetworkRequest request{ QUrl { url } };

    if(!headers.isEmpty()) {
        foreach (QString strHeader, headers.keys()) {
            request.setRawHeader( strHeader.toUtf8(), QString(headers.value(strHeader)).toUtf8() );
        }
    }
    QByteArray postData = QJsonDocument(jsonRequest).toJson(QJsonDocument::Compact);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkAccessManager manager;

    QNetworkReply* reply = manager.post(request, postData);
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    QObject::connect(&timer, &QTimer::timeout, reply, &QNetworkReply::abort);
    timer.start();
    loop.exec();
    if (reply->isFinished() && reply->error() == QNetworkReply::NoError) {

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson( reply->readAll(), &err );
        res = (err.error == QJsonParseError::NoError);
        if(res && !doc.isNull() && doc.isObject()) jsonAnswer = doc.object();
        else res = false;
    }
    reply->deleteLater();
    return res;
}
//=============================================================================================
} // namespace nayk
