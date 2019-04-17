#include <QTextCodec>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QTimer>

#include <string>
#include <iostream>
#include "stdio.h"

#ifdef Q_OS_WIN32
#include "fcntl.h"
#include "io.h"
#endif

#include "http_server.h"
#include "system_utils.h"

extern char** environ;

namespace nayk {

const int QueryTimeOutMax = 300000;
const int ReadTimeOutMax  = 60000;

//======================================================================================================
HttpServer::HttpServer(QObject *parent) : QObject(parent)
{

}
//======================================================================================================
HttpServer::~HttpServer()
{

}
//======================================================================================================
bool HttpServer::readRequestContent(QByteArray &buf)
{
    bool ok;
    int contentLength = mRequestHeaders.value(ServerHeaderContentLength).toInt(&ok);
    if(!ok) {
        _lastError = QObject::tr("Неверный формат заголовка запроса 'Content-Length'.");
        return false;
    }
    if(_dbg) emit toLog( LogDbg, QObject::tr("Начало получения содержимого запроса.") );

#ifdef Q_OS_WIN32
    _setmode(_fileno(stdin), _O_BINARY);
#endif
    QFile standardInput;
    if(!standardInput.open( stdin, QIODevice::ReadOnly )) {
        _lastError = QObject::tr("Не удалось открыть стандартный поток на чтение.");
        return false;
    }

    bool readTimeOut = false;
    bool queryTimeOut = false;
    int blockCnt = 0;
    QString queStr = "";
    qint64 startTimeMSec = QDateTime::currentMSecsSinceEpoch();
    qint64 curInTimeMSec = startTimeMSec;
    qint64 curMS, timeMS, sumTime = 0;

    while(!standardInput.atEnd() && !readTimeOut && !queryTimeOut) {

        curMS = QDateTime::currentMSecsSinceEpoch();

        if((curMS - curInTimeMSec) > ReadTimeOutMax) {
            readTimeOut = true;
            break;
        }
        if((curMS - startTimeMSec) > QueryTimeOutMax) {
            queryTimeOut = true;
            break;
        }

        // засекаем текущее время:
        curMS = QDateTime::currentMSecsSinceEpoch();
        QByteArray readBuf = standardInput.read(1024);
        // вычисляем разницу:
        timeMS = QDateTime::currentMSecsSinceEpoch() - curMS;
        sumTime += timeMS;

        if(!queStr.isEmpty()) queStr += ", ";
        queStr += QString::number( readBuf.size() ) + "=" + QString::number( timeMS ) + QObject::tr(" мсек");

        if(readBuf.isEmpty()) {
            System::pause(20);
            continue;
        }

        buf.append(readBuf);

        QString logStr = "";
        for(int i=0; i<readBuf.size(); i++) {
            logStr += " " + QString(readBuf.mid(i,1).toHex()).toUpper();
        }
        logStr = QString::number(blockCnt) + " =" + logStr;
        if(blockCnt < 10) logStr = "0" + logStr;
        blockCnt++;
        if(_dbg) emit toLog( LogDbg, QObject::tr("БЛОК ") + logStr + " [" + QString::number(readBuf.size()) + QObject::tr(" Б]") );

        if(buf.count() >= contentLength) break;
        curInTimeMSec = QDateTime::currentMSecsSinceEpoch();
    }
    standardInput.close();

    if(_dbg) {
        emit toLog( LogDbg, QObject::tr("Окончание получения содержимого запроса.") );
        emit toLog( LogDbg, QObject::tr("Кол-во считанных байт и время передачи последовательно: ") + queStr );
        emit toLog( LogDbg, QObject::tr("Всего прочитано байт: ") + QString::number(buf.size()) +
                    QObject::tr("; Не прочитано (разница с 'Content-Length'): ") + QString::number(contentLength - buf.size()) );
        emit toLog( LogDbg, QObject::tr("Общее время на чтение содержимого запроса: ") + QString::number(sumTime) + QObject::tr(" мсек.") );
    }
    if(readTimeOut) {
        _lastError = QObject::tr("Тайм-аут ожидания данных содержимого запроса.");
        return false;
    }
    if(queryTimeOut) {
        _lastError = QObject::tr("Тайм-аут запроса. Слишком долгая передача данных.");
        return false;
    }

    if(buf.count() != contentLength) {
        emit toLog( LogWarning, QObject::tr("Значение 'Content-Length' не соответствует фактической длине ") +
                    QString::number(buf.count()) );
    }
    return true;
}
//======================================================================================================
QMap<QString, QString> HttpServer::decodeQuery(const QString &strQuery, const QString &strPairSeparator)
{
    QMap<QString, QString> qmQuery;
    QStringList qslParameters = strQuery.split(strPairSeparator, QString::SkipEmptyParts);

    foreach (QString strPair, qslParameters) {

        QStringList qslPair = strPair.split("=", QString::SkipEmptyParts);
        if(!qslPair.isEmpty()) {
            QString val = (qslPair.size()>1) ? QString(qslPair.at(1)) : QString("");
            qmQuery.insert(QUrl::fromPercentEncoding(QString(qslPair.at(0)).toLatin1()).replace("+", " "),
                           QUrl::fromPercentEncoding(val.toLatin1()).replace("+", " "));
        }
    }
    return qmQuery;
}
//================================================================================================
QString HttpServer::encodeQuery(QMap<QString, QString> qmQuery, const QString &strPairSeparator)
{
    QStringList qslQuery;

    foreach (QString strKey, qmQuery) {

        QStringList qslPair;

        qslPair.append(QString(QUrl::toPercentEncoding(strKey)).replace(" ", "+"));
        qslPair.append(QString(QUrl::toPercentEncoding(qmQuery.value(strKey))).replace(" ", "+"));
        qslQuery.append(qslPair.join("="));
    }

    return qslQuery.join(strPairSeparator);
}
//================================================================================================
QString HttpServer::encodeQuery(QVariantMap qvmQuery, const QString &strPairSeparator)
{
    QStringList qslQuery;

    foreach (QString strKey, qvmQuery.keys()) {

        QStringList qslPair;

        qslPair.append(QString(QUrl::toPercentEncoding(strKey)).replace(" ", "+"));
        qslPair.append(QString(QUrl::toPercentEncoding(qvmQuery.value(strKey).toString())).replace(" ", "+"));
        qslQuery.append(qslPair.join("="));
    }

    return qslQuery.join(strPairSeparator);
}
//================================================================================================
void HttpServer::processCookies()
{
    QString strCookies = mRequestHeaders.value(ServerHeaderHttpCookie);
    if (!strCookies.isEmpty()) {
        mRequestCookies = decodeQuery(strCookies, ";");
    }
}
//===================================================================================================
void HttpServer::processHeaders()
{
    for( int i=0; environ[i] != NULL; ++i ) {
        QString strVal = QString(environ[i]);
        int n = strVal.indexOf("=");
        if(n<1) continue;
        QString strName = strVal.left(n).trimmed().toUpper();
        strVal = strVal.remove(0,n+1).trimmed();
        if(strVal.isEmpty()) continue;
        mRequestHeaders.insert(strName, strVal);
    }
}
//===================================================================================================
void HttpServer::processGet()
{
    QString strQuery = mRequestHeaders.value(ServerHeaderQueryString);
    if (!strQuery.isEmpty()) {
        mRequestGetParameters = decodeQuery(strQuery);
    }
}
//===================================================================================================
bool HttpServer::processPost()
{
    if (mRequestHeaders.value(ServerHeaderRequestMethod).toUpper() != MethodPost) return true;

    bool ok;
    int contentLength = mRequestHeaders.value(ServerHeaderContentLength).toInt(&ok);
    if(!ok || (contentLength==0)) {
        _lastError = QObject::tr("Неверный формат заголовка запроса 'Content-Length'.");
        return false;
    }

    QString contentType = mRequestHeaders.value(ServerHeaderContentType);
    if(contentType.isNull() || contentType.isEmpty()) {
        _lastError = QObject::tr("Не найден заголовок запроса 'Content-Type'.");
        return false;
    }

    QStringList sList =  contentType.split(";", QString::SkipEmptyParts);
    _requestContentType = QString(sList.first()).trimmed().toLower();
    _requestCharset = "utf-8";

    for(int i=1; i<sList.size(); i++) {
        QStringList sTmp = QString(sList.at(i)).split("=", QString::SkipEmptyParts);
        if((sTmp.size() == 2) && (QString(sTmp.first()).toLower() == "charset" )) {
            _requestCharset = QString(sTmp.last()).toLower();
            if(_requestCharset == "utf8") _requestCharset = "utf-8";
            break;
        }
    }

    if(_requestContentType == ContentTypeWWWForm) {
        std::string strPostData;
        std::getline(std::cin, strPostData);
        if (!QString::fromStdString(strPostData).isEmpty()) {
            mRequestPostParameters = decodeQuery(QString().fromStdString(strPostData));
        }
        return true;
    }

    if(!readRequestContent(_requestContent)) return false;

    if(_requestContentType == ContentTypeJSON) {

        QJsonDocument doc = QJsonDocument::fromJson( _requestContent );
        if(doc.isNull() || doc.isEmpty() || !doc.isObject()) {
            _lastError = QObject::tr("Неверный формат запроса JSON.");
            _requestContent.clear();
            return false;
        }
    }
    else if(_requestContentType == ContentTypeMultipartForm) {

        QString boundary = "";

        for(int i=1; i<sList.size(); i++) {
            QString strVal = QString(sList.at(i)).trimmed();
            if (strVal.toUpper().left(9) == "BOUNDARY=") {
                strVal.remove(0,9);
                boundary = strVal.trimmed();
                if((boundary.length()>2) && (boundary.left(1) == "\"") && (boundary.right(1) == "\"")) {
                    boundary.remove(0,1);
                    boundary.chop(1);
                }
                break;
            }
        }
        if(boundary.isEmpty()) {
            _lastError = QObject::tr("Не найдено значение разделителя 'Boundary'.");
            _requestContent.clear();
            return false;
        }

        QString endl = "\r\n";
        int ii = 0;

        if( _requestContent.contains( QString("\r\n--"+boundary+"\r\n").toLocal8Bit() ) ) endl = "\r\n"; else
        if( _requestContent.contains( QString("\r--"+boundary+"\r").toLocal8Bit() ) ) endl = "\r"; else
        if( _requestContent.contains( QString("\n--"+boundary+"\n").toLocal8Bit() ) ) endl = "\n";

        QString boundaryMiddle = "--" + boundary + endl, boundaryEnd = "--" + boundary + "--";
        bool itsEnd = false;

        while((_requestContent.size()>0) && (!itsEnd)) {
            // 1) удаляем часть контента до первого разделителя включая разделитель:
            ii = _requestContent.indexOf( boundaryMiddle.toLocal8Bit() );
            if(ii<0) break;
            _requestContent.remove(0, ii+boundaryMiddle.length());
            if(_requestContent.size() < 1) break;

            // 2) Определить положение следующего разделителя:
            ii = _requestContent.indexOf( QString(endl + boundaryMiddle).toLocal8Bit() );
            if(ii<0) {
                itsEnd = true;
                ii = _requestContent.indexOf( QString(endl + boundaryEnd).toLocal8Bit() );
                if (ii < 0) ii = _requestContent.indexOf( QString(boundaryEnd).toLocal8Bit() );
                if(ii < 0) ii = _requestContent.count();
            }
            if(ii<1) break;
            // 3) копировать часть до разделителя и удалить из буфера:
            QByteArray bVal = _requestContent.left(ii);
            _requestContent.remove(0,ii);

            // 4) отделить заголовок от данных:
            ii = bVal.indexOf( QString(endl+endl).toLocal8Bit() );
            if(ii<1) continue;

            // 5) проверяем заголовок
            sList = QString(bVal).left(ii).split(endl, QString::SkipEmptyParts);
            bVal.remove(0, ii + 2*endl.length() );
            if(bVal.count()<1) continue;

            QString sName = "";
            QString sFileName = "";
            QString sType = "BINARY";
            bool binType = false;
            bool jsonType = false;

            for(int i=0; i<sList.size(); i++) {

                if( QString( sList.at(i) ).toUpper().contains("CONTENT-DISPOSITION:") ) {
                    ii = QString( sList.at(i) ).toUpper().indexOf(" NAME=\"");
                    if(ii<0) ii = QString( sList.at(i) ).toUpper().indexOf(";NAME=\"");
                    if(ii>=0) {
                        sName = QString( sList.at(i) ).right( QString(sList.at(i)).length() - ii - 7 );
                        ii = sName.indexOf("\"");
                        sName = (ii>0) ? sName.left(ii) : "";
                    }
                    ii = QString( sList.at(i) ).toUpper().indexOf(" FILENAME=\"");
                    if(ii<0) ii = QString( sList.at(i) ).toUpper().indexOf(";FILENAME=\"");
                    if(ii>=0) {
                        sFileName = QString( sList.at(i) ).right( QString(sList.at(i)).length() - ii - 11 );
                        ii = sFileName.indexOf("\"");
                        sFileName = (ii>0) ? sFileName.left(ii) : "";
                    }
                }
                else if( QString( sList.at(i) ).toUpper().contains("CONTENT-TYPE:") ) {
                    binType = QString( sList.at(i) ).toLower().contains(ContentTypeBinary);
                    jsonType = QString( sList.at(i) ).toLower().contains(ContentTypeJSON);
                }
                else {
                    ii = QString( sList.at(i) ).toUpper().indexOf("CONTENT-TRANSFER-ENCODING:");
                    if(ii>=0) {
                        sType = QString( sList.at(i) ).right(
                                    QString(sList.at(i)).length() - ii - 26 ).trimmed().toUpper();
                        binType = (sType == "BINARY");
                    }
                }
            }
            if(sName.isEmpty()) continue;
            //
            if(binType || !sFileName.isEmpty()) {
                mRequestBinParameters.insert(sName, bVal);
            }
            else if(jsonType) {

                QJsonDocument doc = QJsonDocument::fromJson(bVal);
                if(doc.isNull() || doc.isEmpty() || !doc.isObject()) {
                    _lastError = QObject::tr("Неверный формат параметра запроса JSON.");
                    _requestContent.clear();
                    return false;
                }
                mRequestJsonParameters.insert( sName, doc.object() );
            }
            else {
                QString val = QString(bVal).trimmed();
                if(val.isEmpty()) val = "1";
                mRequestPostParameters.insert( sName, val );
            }
        }
        _requestContent.clear();
    }

    return true;
}
//=================================================================================================
void HttpServer::readRequest(bool *ok /* = nullptr */)
{
    if(ok) {
        _readRequestOK = processReadRequest();
        *ok = _readRequestOK;
    }
    else QTimer::singleShot(1, this, SLOT(startReadRequest()));
}
//===================================================================================================
bool HttpServer::processReadRequest()
{
    emit toLog( LogInfo, QObject::tr("Чтение HTTP запроса."));

    mRequestHeaders.clear();
    mRequestCookies.clear();
    mRequestGetParameters.clear();
    mRequestPostParameters.clear();
    mRequestBinParameters.clear();
    mRequestJsonParameters.clear();
    _requestContent.clear();
    _readRequestOK = true;

    QMap<QString, QString>::iterator itr;

    processHeaders();
    if(_dbg && mRequestHeaders.count()) {
        emit toLog(LogDbg, QObject::tr("Заголовки HTTP запроса:"));
        for (itr = mRequestHeaders.begin(); itr != mRequestHeaders.end(); ++itr) {
            emit toLog(LogDbg, itr.key() + ": " + itr.value());
        }
    }

    processCookies();
    if(_dbg && mRequestCookies.count()) {
        emit toLog(LogDbg, QObject::tr("Cookies:"));
        for (itr = mRequestCookies.begin(); itr != mRequestCookies.end(); ++itr) {
            emit toLog(LogDbg, itr.key() + ": " + itr.value());
        }
    }

    processGet();
    if(_dbg && mRequestGetParameters.count()) {
        emit toLog(LogDbg, QObject::tr("GET параметры:"));
        for (itr = mRequestGetParameters.begin(); itr != mRequestGetParameters.end(); ++itr) {
            if((itr.key().toLower() == "password") || (itr.key().toLower() == "auth") || (itr.key().toLower() == "token"))
                emit toLog(LogDbg, itr.key() + " = ********");
            else
                emit toLog(LogDbg, itr.key() + " = " + itr.value());
        }
    }

    bool ok = processPost();
    if(_dbg && mRequestPostParameters.count()) {
        emit toLog(LogDbg, QObject::tr("POST параметры:"));
        for (itr = mRequestPostParameters.begin(); itr != mRequestPostParameters.end(); ++itr) {
            if((itr.key().toLower() == "password") || (itr.key().toLower() == "auth") || (itr.key().toLower() == "token"))
                emit toLog(LogDbg, itr.key() + " = ********");
            else
                emit toLog(LogDbg, itr.key() + " = " + itr.value());
        }
    }

    if(_dbg && (_requestContentType == ContentTypeJSON) && !_requestContent.isEmpty()) {
        QJsonDocument doc = QJsonDocument::fromJson(_requestContent);
        emit toLog(LogDbg, QObject::tr("JSON содержимое запроса:\n") + QString(doc.toJson()) );
    }

    if(ok) {
        emit toLog(LogInfo, QObject::tr("Чтение HTTP запроса завершено успешно."));
    }
    else {
        emit toLog(LogError, _lastError);
    }
    return ok;
}
//===================================================================================================
void HttpServer::startReadRequest()
{
    _readRequestOK = processReadRequest();
    emit readRequestFinished(_readRequestOK);
}
//===================================================================================================
bool HttpServer::processWriteResponse()
{
    emit toLog( LogInfo, QObject::tr("Отправка HTTP ответа."));

    bool ok = writeResponseContent();
    if(ok) {
        emit toLog(LogInfo, QObject::tr("Отправка HTTP ответа завершена успешно."));
    }
    else {
        emit toLog(LogError, _lastError);
    }
    return ok;
}
//===================================================================================================
void HttpServer::startWriteResponse()
{
    _writeResponseOK = processWriteResponse();
    emit writeResponseFinished(_writeResponseOK);
}
//===================================================================================================
void HttpServer::writeResponse(bool *ok /* = nullptr */)
{
    if(ok) {
        _writeResponseOK = processWriteResponse();
        *ok = _writeResponseOK;
    }
    else QTimer::singleShot(1, this, SLOT(startWriteResponse()));
}
//===================================================================================================
bool HttpServer::writeResponseContent()
{
    mResponseHeaders.insert( HeaderContentLength, QString::number(_responseContent.size()) );
    if(!mResponseHeaders.contains(HeaderContentType))
        mResponseHeaders.insert(HeaderContentType, ContentTypeJSON);

    QByteArray headers;
    QMap<QString, QString>::iterator itr;

    if(_dbg) emit toLog(LogDbg, QObject::tr("Заголовки ответа:"));

    for (itr = mResponseHeaders.begin(); itr != mResponseHeaders.end(); ++itr) {
        if(!itr.value().isEmpty()) {
            headers.append( QString(itr.key() + ": " + itr.value() + "\r\n").toUtf8() );
            if(_dbg) emit toLog(LogDbg, itr.key() + ": " + itr.value());
        }
    }
    for (itr = mResponseCookies.begin(); itr != mResponseCookies.end(); ++itr) {
        if(!itr.value().isEmpty()) {
            headers.append( QString(HeaderCookie + ": " + itr.key() + "=" + itr.value() + "\r\n").toUtf8() );
            if(_dbg) emit toLog(LogDbg, HeaderCookie + ": " + itr.key() + "=" + itr.value());
        }
    }
    headers.append( QString("\r\n").toUtf8() );

#ifdef Q_OS_WIN32
    _setmode(_fileno(stdout), _O_BINARY);
#endif

    QFile standardOutput;
    if(!standardOutput.open(stdout, QIODevice::WriteOnly)) {
        _lastError = QObject::tr("Не удалось открыть стандартный поток записи для отправки ответа.");
        return false;
    }

    qint64 cnt = 0;
    qint64 maxCount = 10 + headers.size() / 1024;

    while((cnt<maxCount) && !headers.isEmpty() ) {
        qint64 n = standardOutput.write( headers );
        headers.remove(0, n);
        cnt++;
    }

    if(!headers.isEmpty()) {
        _lastError = QObject::tr("Не удалось отправить заголовки ответа.");
        return false;
    }

    QByteArray content = _responseContent;
    cnt = 0;
    maxCount = 10 + content.size() / 1024;

    while((cnt<maxCount) && !content.isEmpty() ) {
        qint64 n = standardOutput.write( content );
        content.remove(0, n);
        cnt++;
    }

    if(!content.isEmpty()) {
        _lastError = QObject::tr("Не удалось отправить содержимое ответа.");
        return false;
    }

    if(_dbg && mResponseHeaders.value(HeaderContentType).contains(ContentTypeJSON)) {
        emit toLog(LogDbg, QObject::tr("JSON содержимое ответа:"));
        QString logStr = QJsonDocument::fromJson(_responseContent).toJson();
        if(logStr.length() > 5000) logStr = logStr.left(5000) + "\n...";
        emit toLog(LogDbg, logStr);
    }

    return true;
}
//===================================================================================================
QVariant HttpServer::requestParameter(const QString &name) const
{
    if (mRequestPostParameters.contains(name)) {
        return QVariant(mRequestPostParameters.value(name));
    }
    if (mRequestGetParameters.contains(name)) {
        return QVariant(mRequestGetParameters.value(name));
    }
    if (mRequestBinParameters.contains(name)) {
        return QVariant(mRequestBinParameters.value(name));
    }
    if (mRequestJsonParameters.contains(name)) {
        return QVariant(mRequestJsonParameters.value(name));
    }

    return QVariant();
}
//===================================================================================================
QString HttpServer::requestPostParameter(const QString &name) const
{
    if (mRequestPostParameters.contains(name)) {
        return mRequestPostParameters.value(name);
    }
    return QString();
}
//===================================================================================================
QString HttpServer::requestGetParameter(const QString &name) const
{
    if (mRequestGetParameters.contains(name)) {
        return mRequestGetParameters.value(name);
    }
    return QString();
}
//===================================================================================================
QString HttpServer::requestHeader(const QString &name) const
{
    if (mRequestHeaders.contains(name)) {
        return mRequestHeaders.value(name);
    }
    return QString();
}
//===================================================================================================
QString HttpServer::requestCookie(const QString &name) const
{
    if (mRequestCookies.contains(name)) {
        return mRequestCookies.value(name);
    }
    return QString();
}
//===================================================================================================
QByteArray HttpServer::requestBinParameter(const QString &name) const
{
    if (mRequestBinParameters.contains(name)) {
        return mRequestBinParameters.value(name);
    }
    return QByteArray();
}
//===================================================================================================
QJsonObject HttpServer::requestJsonParameter(const QString &name) const
{
    if (mRequestJsonParameters.contains(name)) {
        return mRequestJsonParameters.value(name);
    }
    return QJsonObject();
}
//===================================================================================================
QString HttpServer::responseHeader(const QString &name) const
{
    if (mResponseHeaders.contains(name)) {
        return mResponseHeaders.value(name);
    }
    return QString();
}
//===================================================================================================
QString HttpServer::responseCookie(const QString &name) const
{
    if (mResponseCookies.contains(name)) {
        return mResponseCookies.value(name);
    }
    return QString();
}
//===================================================================================================
} // namespace nayk
