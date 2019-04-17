#ifndef CRYPTO_H
#define CRYPTO_H

#include <QtCore>
#include <QByteArray>
#include <QString>

namespace nayk {
//=========================================================================================================
enum CryptoAlg { CryptoAlg_TexasAES128 };
//=========================================================================================================
class Crypto
{
public:
    explicit Crypto(CryptoAlg algorythm = CryptoAlg_TexasAES128);    
    virtual bool encryptData(const QString &inData, const QString &key, QString &outData);
    virtual bool decryptData(const QString &inData, const QString &key, QString &outData);
    virtual bool encryptData(const QByteArray &inData, const QString &key, QByteArray &outData);
    virtual bool decryptData(const QByteArray &inData, const QString &key, QByteArray &outData);
    //
    QString lastError() const { return _lastError; }
    static QString md5(const QString &data);
    static QByteArray md5Bytes(const QString &data);
    static quint8 crc8_Dallas(const QVector<quint8> &data);
    static quint8 crc8_Dallas(const QByteArray &data);
    static quint8 crc8_Dallas(const quint8 * data, qint32 size);

protected:
    QString _lastError;

private:
    CryptoAlg _algorythm;    
    // шифрование aes128 (Texas Instruments alg.)
    QByteArray encryptAES128_Block(const QByteArray &dataBuf, const QByteArray &md5Key);
    QByteArray decryptAES128_Block(const QByteArray &dataBuf, const QByteArray &md5Key);
    QByteArray encryptAES128(const QByteArray &data, const QByteArray &md5Key);
    QByteArray decryptAES128(const QByteArray &data, const QByteArray &md5Key);
};
//=========================================================================================================
} // namespace nayk

#endif // CRYPTO_H
