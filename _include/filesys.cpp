#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include "filesys.h"

namespace nayk {
//=======================================================================================================
bool FileSys::directoryExists(const QString &dirName)
{
    QDir dir( QDir::homePath() );
    return dir.exists(dirName);
}
//====================================================================================================
bool FileSys::fileExists(const QString &fileName)
{
    return QFile::exists(fileName);
}
//====================================================================================================
bool FileSys::makePath(const QString &dirPath)
{
    QDir dir;
    return dir.mkpath(dirPath);
}
//====================================================================================================
QString FileSys::changeFileExt(const QString &fileName, const QString &newExt)
{
    int i = fileName.lastIndexOf('.');
    if( newExt.length() > 0 ) {

        if(newExt[0] == '.') return ((i>0) ? fileName.left( i ) : fileName) + newExt;
        else return ((i>0) ? fileName.left( i ) : fileName) + "." + newExt;
    }
    else return fileName;
}
//====================================================================================================
QString FileSys::extractFileExt(const QString &fileName, bool withDot)
{
    QString f = extractFileName(fileName);
    int i = f.lastIndexOf('.');
    if(i>=0) {
        if(withDot) return f.right( f.length() - i );
        else return f.right( f.length() - i - 1 );
    }
    else return "";
}
//====================================================================================================
QString FileSys::extractFileName(const QString &fileName)
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
//====================================================================================================
QString FileSys::extractFilePath(const QString &fileName)
{
   QString f = QDir::toNativeSeparators(fileName);
   int i = f.lastIndexOf( QDir::separator() );
   if(i>=0) return f.left(i+1);
   else return "";
}
//====================================================================================================
QString FileSys::applicationFullPath(bool withEndDirSeparator)
{
    QString res = QDir::toNativeSeparators( QCoreApplication::applicationDirPath() );
    if(withEndDirSeparator && (res.right(1) != QDir::separator())) res += QDir::separator();
    return res;
}
//====================================================================================================
bool FileSys::readJsonFromFile(const QString &fileName, QJsonDocument &json, QString &errorString)
{
    errorString = "";
    QFile file( fileName );

    if(!file.exists()) {
        errorString =  QObject::tr("Файл \"%1\" не существует.").arg(extractFileName( fileName ));
        return false;
    }

    if(!file.open(QFile::ReadOnly)) {
        errorString = QObject::tr("Не удалось открыть файл \"%1\" на чтение.").arg( extractFileName( fileName ) );
        return false;
    }

    QJsonParseError parseError;
    json = QJsonDocument::fromJson( file.readAll(), &parseError );
    file.close();

    if(parseError.error != QJsonParseError::NoError) {
        errorString = QObject::tr("Ошибка при разборе файла \"%1\".").arg( extractFileName( fileName ) ) + "\n" + parseError.errorString();
        return false;
    }

    return true;
}
//====================================================================================================
bool FileSys::readJsonFromFile(const QString &fileName, QJsonArray &json, QString &errorString)
{
    QJsonDocument doc;
    if(!readJsonFromFile(fileName, doc, errorString)) return false;

    if(!doc.isArray()) {
        errorString = QObject::tr("Файл \"%1\" не является массивом JSON.").arg( extractFileName( fileName ) );
        return false;
    }

    json = doc.array();
    return true;
}
//====================================================================================================
bool FileSys::readJsonFromFile(const QString &fileName, QJsonObject &json, QString &errorString)
{
    QJsonDocument doc;
    if(!readJsonFromFile(fileName, doc, errorString)) return false;

    if(!doc.isObject()) {
        errorString = QObject::tr("Файл \"%1\" не является объектом JSON.").arg( extractFileName( fileName ) );
        return false;
    }

    json = doc.object();
    return true;
}
//====================================================================================================
bool FileSys::writeJsonToFile(const QString &fileName, const QJsonDocument &json, QString &errorString)
{
    errorString = "";

    QFile file( fileName );
    if(!file.open(QFile::WriteOnly)) {
        errorString = QObject::tr("Не удалось открыть файл \"%1\" на запись.").arg( extractFileName( fileName ) );
        return false;
    }

    QByteArray buf = json.toJson();
    int n = file.write( buf );
    bool res = file.flush();
    file.close();

    if(n != buf.size()) {
        errorString = QObject::tr("Не удалось записать все данные в файл \"%1\".").arg( extractFileName( fileName ) );
        return false;
    }

    if(!res) {
        errorString = QObject::tr("Не удалось сохранить данные в файл \"%1\".").arg( extractFileName( fileName ) );
        return false;
    }

    return true;
}
//====================================================================================================
bool FileSys::writeJsonToFile(const QString &fileName, const QJsonArray &json, QString &errorString)
{
    return writeJsonToFile(fileName, QJsonDocument(json), errorString);
}
//====================================================================================================
bool FileSys::writeJsonToFile(const QString &fileName, const QJsonObject &json, QString &errorString)
{
    return writeJsonToFile(fileName, QJsonDocument(json), errorString);
}
//====================================================================================================

//====================================================================================================
} // namespace nayk

