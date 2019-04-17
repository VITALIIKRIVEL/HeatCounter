#ifndef FILESYS_H
#define FILESYS_H

#include <QString>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

//=========================================================================================================
namespace nayk {
//=========================================================================================================

//=========================================================================================================
class FileSys
{
public:
    static QString applicationFullPath(bool withEndDirSeparator = true);
    static bool directoryExists(const QString &dirName);
    static bool fileExists(const QString &fileName);
    static bool makePath(const QString &dirPath);
    static QString changeFileExt(const QString &fileName, const QString &newExt);
    static QString extractFileExt(const QString &fileName, bool withDot = true);
    static QString extractFileName(const QString &fileName);
    static QString extractFilePath(const QString &fileName);
    static bool readJsonFromFile(const QString &fileName, QJsonDocument &json, QString &errorString);
    static bool readJsonFromFile(const QString &fileName, QJsonArray &json, QString &errorString);
    static bool readJsonFromFile(const QString &fileName, QJsonObject &json, QString &errorString);
    static bool writeJsonToFile(const QString &fileName, const QJsonDocument &json, QString &errorString);
    static bool writeJsonToFile(const QString &fileName, const QJsonArray &json, QString &errorString);
    static bool writeJsonToFile(const QString &fileName, const QJsonObject &json, QString &errorString);
    //
    FileSys() = delete; // < std=c++11, обеспечивает запрет на создание любого экземпляра FileSys
};

//=========================================================================================================
} // namespace nayk

//=========================================================================================================
#endif // FILESYS_H
