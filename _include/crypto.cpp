// Qt:
#include <QCryptographicHash>
#include <QObject>
// Common:
#include "crypto.h"

//==========================================================================================================
namespace nayk {

/*  AES-128 Texas Instruments Algorithm ================================================================= */

// foreward sbox
const unsigned char sbox[256] =   {
//0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,   //0
0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,   //1
0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,   //2
0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,   //3
0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,   //4
0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,   //5
0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,   //6
0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,   //7
0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,   //8
0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,   //9
0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,   //A
0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,   //B
0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,   //C
0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,   //D
0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,   //E
0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }; //F

// inverse sbox
const unsigned char rsbox[256] =
{ 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb
, 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb
, 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e
, 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25
, 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92
, 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84
, 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06
, 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b
, 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73
, 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e
, 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b
, 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4
, 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f
, 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef
, 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61
, 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };

// round constant
const unsigned char Rcon[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

// multiply by 2 in the galois field
unsigned char galois_mul2(unsigned char value)
{
  signed char temp;
  // cast to signed value
  temp = (signed char) value;
  // if MSB is 1, then this will signed extend and fill the temp variable with 1's
  temp = temp >> 7;
  // AND with the reduction variable
  temp = temp & 0x1b;
  // finally shift and reduce the value
  return ((value << 1)^temp);
}

// AES encryption and decryption function
// The code was optimized for memory (flash and ram)
// Combining both encryption and decryption resulted in a slower implementation
// but much smaller than the 2 functions separated
// This function only implements AES-128 encryption and decryption (AES-192 and
// AES-256 are not supported by this code)
void aes_enc_dec(unsigned char *state, unsigned char *key, unsigned char dir)
{
  unsigned char buf1, buf2, buf3, buf4, round, i;

  // In case of decryption
  if (dir) {
    // compute the last key of encryption before starting the decryption
    for (round = 0 ; round < 10; round++) {
      //key schedule
      key[0] = sbox[key[13]]^key[0]^Rcon[round];
      key[1] = sbox[key[14]]^key[1];
      key[2] = sbox[key[15]]^key[2];
      key[3] = sbox[key[12]]^key[3];
      for (i=4; i<16; i++) {
        key[i] = key[i] ^ key[i-4];
      }
    }

    //first Addroundkey
    for (i = 0; i <16; i++){
      state[i]=state[i] ^ key[i];
    }
  }

  // main loop
  for (round = 0; round < 10; round++){
    if (dir){
      //Inverse key schedule
      for (i=15; i>3; --i) {
    key[i] = key[i] ^ key[i-4];
      }
      key[0] = sbox[key[13]]^key[0]^Rcon[9-round];
      key[1] = sbox[key[14]]^key[1];
      key[2] = sbox[key[15]]^key[2];
      key[3] = sbox[key[12]]^key[3];
    } else {
      for (i = 0; i <16; i++){
        // with shiftrow i+5 mod 16
    state[i]=sbox[state[i] ^ key[i]];
      }
      //shift rows
      buf1 = state[1];
      state[1] = state[5];
      state[5] = state[9];
      state[9] = state[13];
      state[13] = buf1;

      buf1 = state[2];
      buf2 = state[6];
      state[2] = state[10];
      state[6] = state[14];
      state[10] = buf1;
      state[14] = buf2;

      buf1 = state[15];
      state[15] = state[11];
      state[11] = state[7];
      state[7] = state[3];
      state[3] = buf1;
    }
    //mixcol - inv mix
    if ((round > 0 && dir) || (round < 9 && !dir)) {
      for (i=0; i <4; i++){
        buf4 = (i << 2);
        if (dir){
          // precompute for decryption
          buf1 = galois_mul2(galois_mul2(state[buf4]^state[buf4+2]));
          buf2 = galois_mul2(galois_mul2(state[buf4+1]^state[buf4+3]));
          state[buf4] ^= buf1; state[buf4+1] ^= buf2; state[buf4+2] ^= buf1; state[buf4+3] ^= buf2;
        }
        // in all cases
        buf1 = state[buf4] ^ state[buf4+1] ^ state[buf4+2] ^ state[buf4+3];
        buf2 = state[buf4];
        buf3 = state[buf4]^state[buf4+1]; buf3=galois_mul2(buf3); state[buf4] = state[buf4] ^ buf3 ^ buf1;
        buf3 = state[buf4+1]^state[buf4+2]; buf3=galois_mul2(buf3); state[buf4+1] = state[buf4+1] ^ buf3 ^ buf1;
        buf3 = state[buf4+2]^state[buf4+3]; buf3=galois_mul2(buf3); state[buf4+2] = state[buf4+2] ^ buf3 ^ buf1;
        buf3 = state[buf4+3]^buf2;     buf3=galois_mul2(buf3); state[buf4+3] = state[buf4+3] ^ buf3 ^ buf1;
      }
    }

    if (dir) {
      //Inv shift rows
      // Row 1
      buf1 = state[13];
      state[13] = state[9];
      state[9] = state[5];
      state[5] = state[1];
      state[1] = buf1;
      //Row 2
      buf1 = state[10];
      buf2 = state[14];
      state[10] = state[2];
      state[14] = state[6];
      state[2] = buf1;
      state[6] = buf2;
      //Row 3
      buf1 = state[3];
      state[3] = state[7];
      state[7] = state[11];
      state[11] = state[15];
      state[15] = buf1;

      for (i = 0; i <16; i++){
        // with shiftrow i+5 mod 16
        state[i]=rsbox[state[i]] ^ key[i];
      }
    } else {
      //key schedule
      key[0] = sbox[key[13]]^key[0]^Rcon[round];
      key[1] = sbox[key[14]]^key[1];
      key[2] = sbox[key[15]]^key[2];
      key[3] = sbox[key[12]]^key[3];
      for (i=4; i<16; i++) {
        key[i] = key[i] ^ key[i-4];
      }
    }
  }
  if (!dir) {
  //last Addroundkey
    for (i = 0; i <16; i++){
      // with shiftrow i+5 mod 16
      state[i]=state[i] ^ key[i];
    } // enf for
  } // end if (!dir)
} // end function

/* AES-128 End Section ===================================================================================*/

quint8 reverse8(quint8 value)
{
    value = (value & 0xF0) >> 4 | (value & 0x0F) << 4;
    value = (value & 0xCC) >> 2 | (value & 0x33) << 2;
    value = (value & 0xAA) >> 1 | (value & 0x55) << 1;
    return value;
}
//==========================================================================================================
const QString Err_Empty_InData          = QObject::tr("Нет входящих данных.");
const QString Err_Length_InData         = QObject::tr("Неверная длина входящих данных.");
const QString Err_Empty_Key             = QObject::tr("Не указан ключ шифрования.");

//==========================================================================================================
Crypto::Crypto(CryptoAlg algorythm)
{
    _algorythm = algorythm;
    _lastError = "";
}
//==========================================================================================================
bool Crypto::encryptData(const QString &inData, const QString &key, QString &outData)
{
    QByteArray inDataBuf = inData.toUtf8();
    QByteArray outDataBuf;
    if (!encryptData(inDataBuf, key, outDataBuf)) return false;
    outData = QString(outDataBuf.toHex()).toLower();
    return true;
}
//==========================================================================================================
bool Crypto::decryptData(const QString &inData, const QString &key, QString &outData)
{
    QByteArray inDataBuf = QByteArray::fromHex( inData.toUtf8() );
    QByteArray outDataBuf;
    if (!decryptData(inDataBuf, key, outDataBuf)) return false;
    outData = QString::fromUtf8( outDataBuf ).trimmed();
    return true;
}
//==========================================================================================================
bool Crypto::encryptData(const QByteArray &inData, const QString &key, QByteArray &outData)
{
    _lastError = "";
    if (inData.isNull() || inData.isEmpty()) {
        _lastError = Err_Empty_InData;
        return false;
    }
    if (key.isNull() || key.isEmpty()) {
        _lastError = Err_Empty_Key;
        return false;
    }

    QByteArray md5Key = QCryptographicHash::hash( key.toUtf8(), QCryptographicHash::Md5 );
    outData.clear();

    switch (_algorythm) {
    case CryptoAlg_TexasAES128:
        outData = encryptAES128(inData, md5Key);
        break;
    default:
        break;
    }

    return true;
}
//==========================================================================================================
bool Crypto::decryptData(const QByteArray &inData, const QString &key, QByteArray &outData)
{
    _lastError = "";
    if (inData.isNull() || inData.isEmpty() || ((inData.size() % 16) != 0)) {
        _lastError = Err_Length_InData;
        return false;
    }
    if (key.isNull() || key.isEmpty()) {
        _lastError = Err_Empty_Key;
        return false;
    }

    QByteArray md5Key = QCryptographicHash::hash( key.toUtf8(), QCryptographicHash::Md5 );
    outData.clear();

    switch (_algorythm) {
    case CryptoAlg_TexasAES128:
        outData = decryptAES128(inData, md5Key);
        break;
    default:
        break;
    }

    return true;
}
//==========================================================================================================
QByteArray Crypto::encryptAES128(const QByteArray &data, const QByteArray &md5Key)
{
    if (data.isEmpty()) return QByteArray();
    QByteArray dataBuf(data);
    QByteArray resBuf;
    while (dataBuf.size() % 16 != 0) dataBuf.append( static_cast<char>(0) );
    for(int i=0; i<dataBuf.size(); i+=16) {
        resBuf.append( encryptAES128_Block( dataBuf.mid(i, 16), md5Key ) );
    }
    return resBuf;
}
//==========================================================================================================
QByteArray Crypto::decryptAES128(const QByteArray &data, const QByteArray &md5Key)
{
    if (data.isEmpty() || ((data.size() % 16) != 0)) return QByteArray();
    QByteArray resBuf;

    for(int i=0; i<data.size(); i+=16) {
        resBuf.append( decryptAES128_Block( data.mid(i, 16), md5Key ) );
    }
    return resBuf;
}
//==========================================================================================================
QByteArray Crypto::encryptAES128_Block(const QByteArray &dataBuf, const QByteArray &md5Key)
{
    unsigned char *ucBuf;
    unsigned char *ucKey;
    int keySize, bufSize;

    keySize = md5Key.size(); // 16 байт = 128 бит
    ucKey = new unsigned char [keySize];
    for(int i=0; i<keySize; i++) {
        ucKey[i] = static_cast<unsigned char>( md5Key.at(i) & 0xFF );
    }

    bufSize = dataBuf.size();
    while (bufSize%16 != 0) bufSize++; // размер должен быть кратным 128 бит
    ucBuf = new unsigned char [bufSize];
    for(int i=0; i<bufSize; i++) {
        ucBuf[i] = (i<dataBuf.size()) ? static_cast<unsigned char>( dataBuf.at(i) & 0xFF ) : 0;
    }

    // шифрование:
    aes_enc_dec( ucBuf, ucKey, 0 );

    QByteArray res;
    for(int i=0; i<bufSize; i++) res.append( ucBuf[i] );

    delete [] ucBuf;
    delete [] ucKey;

    return res;
}
//==========================================================================================================
QByteArray Crypto::decryptAES128_Block(const QByteArray &dataBuf, const QByteArray &md5Key)
{
    unsigned char *ucBuf;
    unsigned char *ucKey;
    int keySize, bufSize;

    keySize = md5Key.size(); // 16 байт = 128 бит
    ucKey = new unsigned char [keySize];
    for(int i=0; i<keySize; i++) {
        ucKey[i] = static_cast<unsigned char>( md5Key.at(i) & 0xFF );
    }

    bufSize = dataBuf.size();
    while (bufSize%16 != 0) bufSize++; // размер должен быть кратным 128 бит
    ucBuf = new unsigned char [bufSize];
    for(int i=0; i<bufSize; i++) {
        ucBuf[i] = static_cast<unsigned char>( dataBuf.at(i) & 0xFF );
    }

    // дешифрование:
    aes_enc_dec( ucBuf, ucKey, 1 );

    QByteArray res;
    for(int i=0; i<bufSize; i++) res.append( ucBuf[i] );

    delete [] ucBuf;
    delete [] ucKey;

    return res;
}
//==========================================================================================================
QString Crypto::md5(const QString &data)
{
    return QString::fromUtf8(
                QCryptographicHash::hash( data.toUtf8(), QCryptographicHash::Md5 ).toHex()
           ).toLower();
}
//====================================================================================================
QByteArray Crypto::md5Bytes(const QString &data)
{
    return QCryptographicHash::hash( data.toUtf8(), QCryptographicHash::Md5 );
}
//====================================================================================================
/*
Name :   CRC-8
Width :  8
Poly :   0x31 = x^8 + x^5 + x^4 + x^0
Init :   0x00
RefIn :  True
RefOut : True
XorOut : 0x00
Check :  0xA1 = "123456789"
*/
quint8 Crypto::crc8_Dallas(const QByteArray &data)
{
    quint8 crc = 0;
    for(auto n=0; n<data.size(); ++n) {
        quint8 inbyte = static_cast<quint8>( data.at(n) & 0xFF );
        for (quint8 i = 8; i; i--) {
            quint8 mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix) crc ^= 0x8C;
            inbyte >>= 1;
        }
    }
    return crc;
}
//====================================================================================================
quint8 Crypto::crc8_Dallas(const QVector<quint8> &data)
{
    quint8 crc = 0;
    for(auto n=0; n<data.size(); ++n) {
        quint8 inbyte = data.at(n);
        for (quint8 i = 8; i; i--) {
            quint8 mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix) crc ^= 0x8C;
            inbyte >>= 1;
        }
    }
    return crc;
}
//==========================================================================================================
quint8 Crypto::crc8_Dallas(const quint8 * data, qint32 size)
{
    quint8 crc = 0;
    while (size--) {
        quint8 inbyte = *data++;
        for (quint8 i = 8; i; i--) {
            quint8 mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix) crc ^= 0x8C;
            inbyte >>= 1;
        }
    }
    return crc;
}
//==========================================================================================================

//==========================================================================================================
} // namespace nayk

