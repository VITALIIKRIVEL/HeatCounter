#ifndef WMBUS_H
#define WMBUS_H

#include <QObject>
#include <QDateTime>
#include <QVector>

/*******************************************************************************************/
namespace wmbus {

/*******************************************************************************************/
typedef struct ChronosDbStruct {
    QString server;
    QString db;
    QString user;
    QString password;
    int port;
} ChronosDbStruct;
/*******************************************************************************************/
typedef struct ServerSettings { // структура для хранения настроек
    ChronosDbStruct db;
    bool log;
    bool dbg;
    bool getEnable;
    quint16 serverPort;
    QString keyCrypt;
    bool onlyHTTPS;
    //
    QString dirChronos;
    QString dirSoft;
    //
    quint16 queryTimeOutSec;
    quint16 readTimeOutSec;
} ServerSettings;

/*******************************************************************************************/
enum AnswerType { Answer_OK, Answer_Info, Answer_Table, Answer_Error }; // тип ответа

/*******************************************************************************************/
typedef quint16 TErrorCode;
/* Коды ошибок */
const TErrorCode Err_OK                                    = 0x0000;
const TErrorCode Err_Unknown_CMD                           = 0x0001;
const TErrorCode Err_CMD_Not_Found                         = 0x0002;
const TErrorCode Err_Params_Not_Found                      = 0x0003;
const TErrorCode Err_Param_Date_Not_Found                  = 0x0004;
const TErrorCode Err_Param_Time_Not_Found                  = 0x0005;
const TErrorCode Err_Param_Dev_Not_Found                   = 0x0006;
const TErrorCode Err_Param_HardVer_Not_Found               = 0x0007;
const TErrorCode Err_Param_SoftVer_Not_Found               = 0x0008;
const TErrorCode Err_Param_Status_Not_Found                = 0x0009;
const TErrorCode Err_Param_WMBMan_Not_Found                = 0x000A;
const TErrorCode Err_Param_WMBId_Not_Found                 = 0x000B;
const TErrorCode Err_Param_WMBDev_Not_Found                = 0x000C;
const TErrorCode Err_Param_WMBVer_Not_Found                = 0x000D;
const TErrorCode Err_Param_WMBMan_Format                   = 0x000E;
const TErrorCode Err_Param_WMBId_Format                    = 0x000F;
const TErrorCode Err_Param_WMBDev_Format                   = 0x0010;
const TErrorCode Err_Param_WMBVer_Format                   = 0x0011;
const TErrorCode Err_Values_And_Flags_Is_Empty             = 0x0012;
const TErrorCode Err_Server_Connect                        = 0x0013;
const TErrorCode Err_DataBase_Connect                      = 0x0014;
const TErrorCode Err_Server_CodePage                       = 0x0015;
const TErrorCode Err_DataBase_Connect_Not_Found            = 0x0016;
const TErrorCode Err_Init_Settings_Table                   = 0x0017;
const TErrorCode Err_Read_Settings_Table                   = 0x0018;
const TErrorCode Err_GET_Query_Disable                     = 0x0019;
const TErrorCode Err_Device_Table_Query                    = 0x001A;
const TErrorCode Err_Device_Not_Exist                      = 0x001B;
const TErrorCode Err_Record_Exist                          = 0x001C;
const TErrorCode Err_Add_Archive_Record                    = 0x001D;
const TErrorCode Err_Archive_Table_Query                   = 0x001E;
const TErrorCode Err_Get_Archive_Table_Id                  = 0x001F;
const TErrorCode Err_Values_Is_Empty                       = 0x0020;
const TErrorCode Err_Save_Values                           = 0x0021;
const TErrorCode Err_Flags_Is_Empty                        = 0x0022;
const TErrorCode Err_Save_Flags                            = 0x0023;
const TErrorCode Err_Save_ArcId_In_DevTable                = 0x0024;
const TErrorCode Err_Load_SQL_Script_File                  = 0x0025;
const TErrorCode Err_SQL_Script                            = 0x0026;
const TErrorCode Err_Delete_Tables_From_DB                 = 0x0027;
const TErrorCode Err_Region_Table_Query                    = 0x0028;
const TErrorCode Err_Param_SortCol                         = 0x0029;
const TErrorCode Err_Param_SortType                        = 0x002A;
const TErrorCode Err_Param_RowCount_Format                 = 0x002B;
const TErrorCode Err_Param_StartRowNum_Format              = 0x002C;
const TErrorCode Err_Param_RowCount                        = 0x002D;
const TErrorCode Err_Param_StartRowNum                     = 0x002E;
const TErrorCode Err_Rayon_Table_Query                     = 0x002F;
const TErrorCode Err_City_Table_Query                      = 0x0030;
const TErrorCode Err_Punkt_Table_Query                     = 0x0031;
const TErrorCode Err_Street_Table_Query                    = 0x0032;
const TErrorCode Err_House_Table_Query                     = 0x0033;
const TErrorCode Err_Param_SocrId_Format                   = 0x0034;
const TErrorCode Err_Param_SocrIdMin_Format                = 0x0035;
const TErrorCode Err_Param_SocrIdMax_Format                = 0x0036;
const TErrorCode Err_Param_SocrLevel_Format                = 0x0037;
const TErrorCode Err_Socr_Table_Query                      = 0x0038;
const TErrorCode Err_Param_IndexId_Format                  = 0x0039;
const TErrorCode Err_Index_Table_Query                     = 0x003A;
const TErrorCode Err_Param_AddressId_Format                = 0x003B;
const TErrorCode Err_Address_Table_Query                   = 0x003C;
const TErrorCode Err_Temp_File_Create                      = 0x003D;
const TErrorCode Err_Param_RegionCode_Not_Found            = 0x003E;
const TErrorCode Err_Param_RayonCode_Not_Found             = 0x003F;
const TErrorCode Err_Param_CityCode_Not_Found              = 0x0040;
const TErrorCode Err_Param_PunktCode_Not_Found             = 0x0041;
const TErrorCode Err_Param_StreetCode_Not_Found            = 0x0042;
const TErrorCode Err_Param_RegionCode_Format               = 0x0043;
const TErrorCode Err_Param_RayonCode_Format                = 0x0044;
const TErrorCode Err_Param_CityCode_Format                 = 0x0045;
const TErrorCode Err_Param_PunktCode_Format                = 0x0046;
const TErrorCode Err_Param_StreetCode_Format               = 0x0047;
const TErrorCode Err_Param_SocrId_Format_2                 = 0x0048;
const TErrorCode Err_Param_Index_Format                    = 0x0049;
const TErrorCode Err_Address_Table_Record_Exist            = 0x004A;
const TErrorCode Err_Address_Table_Insert                  = 0x004B;
const TErrorCode Err_Param_AddressId_Not_Found             = 0x004C;
const TErrorCode Err_Pole_AddressId_Format                 = 0x004D;
const TErrorCode Err_Address_Use_Id_Device_Table           = 0x004E;
const TErrorCode Err_Address_Use_Id_Abonent_Table          = 0x004F;
const TErrorCode Err_Query_To_DataBase                     = 0x0050;
const TErrorCode Err_Query_Result_Not_Correct              = 0x0051;
const TErrorCode Err_Param_AbonentTypeId_Format            = 0x0052;
const TErrorCode Err_Query_To_AbonentType_Table            = 0x0053;
const TErrorCode Err_Param_DevType_Format                  = 0x0054;
const TErrorCode Err_Query_To_DevType_Table                = 0x0055;
const TErrorCode Err_Param_ArcType_Format                  = 0x0056;
const TErrorCode Err_Query_To_ArcType_Table                = 0x0057;
const TErrorCode Err_Param_AccountId_Format                = 0x0058;
const TErrorCode Err_Query_To_Account_Table                = 0x0059;
const TErrorCode Err_DeviceId_Format                       = 0x005A;
const TErrorCode Err_IdentNum_Format                       = 0x005B;
const TErrorCode Err_Ver_Format                            = 0x005C;
const TErrorCode Err_InstallDate_Format                    = 0x005D;
const TErrorCode Err_InstallDateMin_Format                 = 0x005E;
const TErrorCode Err_InstallDateMax_Format                 = 0x005F;
const TErrorCode Err_LastPovDate_Format                    = 0x0060;
const TErrorCode Err_LastPovDateMin_Format                 = 0x0061;
const TErrorCode Err_LastPovDateMax_Format                 = 0x0062;
const TErrorCode Err_Param_ManId_Not_Found                 = 0x0063;
const TErrorCode Err_Param_IdentNum_Not_Found              = 0x0064;
const TErrorCode Err_Param_DevType_Not_Found               = 0x0065;
const TErrorCode Err_Param_Ver_Not_Found                   = 0x0066;
const TErrorCode Err_Param_ManId_Format                    = 0x0067;
const TErrorCode Err_Query                                 = 0x0068;
const TErrorCode Err_Values_Table_Query                    = 0x0069;
const TErrorCode Err_Errors_Table_Query                    = 0x006A;
const TErrorCode Err_Device_Table_Rec_Exist                = 0x006B;
const TErrorCode Err_Param_PovInterval_Format              = 0x006C;
const TErrorCode Err_Add_Device_Table_Rec                  = 0x006D;
const TErrorCode Err_Param_DevId_Not_Found                 = 0x006E;
const TErrorCode Err_Device_Use_In_Values_Table            = 0x006F;
const TErrorCode Err_Address_Not_Found                     = 0x0070;
const TErrorCode Err_Param_ArcDate_Format                  = 0x0071;
const TErrorCode Err_Param_ArcDateMin_Format               = 0x0072;
const TErrorCode Err_Param_ArcDateMax_Format               = 0x0073;
const TErrorCode Err_Param_AbonentId_Format                = 0x0074;
const TErrorCode Err_Abonent_Table_Query                   = 0x0075;
const TErrorCode Err_Function_Disable                      = 0x0076;

const TErrorCode Err_Param_TimeShift_Not_Found             = 0x007E;
const TErrorCode Err_Param_TimeShift_Format                = 0x007F;
const TErrorCode Err_Pole_ObjectId_Format                  = 0x0080;
const TErrorCode Err_JSON_AnswerType                       = 0x0081;
const TErrorCode Err_Only_HTTPS_Query_Enabled              = 0x0082;
const TErrorCode Err_JSON_Format                           = 0x0083;
const TErrorCode Err_SQL_Query                             = 0x0084;
const TErrorCode Err_Query_Time_Out                        = 0x0085;
const TErrorCode Err_Data_Decrypt                          = 0x0086;
const TErrorCode Err_Decrypt_Format                        = 0x0087;
const TErrorCode Err_Param_AuthData_Not_Found              = 0x0088;
const TErrorCode Err_Param_AuthData_Format                 = 0x0089;

const TErrorCode Err_User_Table_Query                      = 0x008A;
const TErrorCode Err_Login_Password                        = 0x008B;
const TErrorCode Err_User_Disable                          = 0x008C;

const TErrorCode Err_CMD_Not_Access                        = 0x008F;
const TErrorCode Err_Abonent_Not_Found                     = 0x0090;
const TErrorCode Err_Param_ObjectId_Not_Found              = 0x0091;
const TErrorCode Err_Param_ObjectId_Format                 = 0x0092;
const TErrorCode Err_Param_UserAuth_Format                 = 0x0093;
const TErrorCode Err_Insert_UserTable                      = 0x0094;
const TErrorCode Err_Param_UserId_NotFound                 = 0x0095;
const TErrorCode Err_Edit_UserTable                        = 0x0096;

const TErrorCode Err_Login_Busy                            = 0x009A;
const TErrorCode Err_LastId_Query                          = 0x009B;
const TErrorCode Err_Param_Data_Not_Found                  = 0x009C;
const TErrorCode Err_Param_Data_Format                     = 0x009D;
const TErrorCode Err_DateTime_Format                       = 0x009E;
const TErrorCode Err_Param_Dev_Format                      = 0x009F;
const TErrorCode Err_Param_Status_Format                   = 0x00A0;
const TErrorCode Err_Param_Data_Length                     = 0x00A1;
const TErrorCode Err_CRC                                   = 0x00A2;
const TErrorCode Err_DIF_Value                             = 0x00A3;
const TErrorCode Err_VIF_Value                             = 0x00A4;
const TErrorCode Err_Data_End                              = 0x00A5;
const TErrorCode Err_Value                                 = 0x00A6;
const TErrorCode Err_Pak_Length                            = 0x00A7;
const TErrorCode Err_File_Not_Exists                       = 0x00A8;
const TErrorCode Err_File_Not_Open                         = 0x00A9;
const TErrorCode Err_Query_Method                          = 0x00AA;
const TErrorCode Err_Content_Length                        = 0x00AB;
const TErrorCode Err_Content_And_Data_Length               = 0x00AC;
const TErrorCode Err_Content_Type                          = 0x00AD;
const TErrorCode Err_Boundary_Not_Found                    = 0x00AE;
const TErrorCode Err_Read_Content                          = 0x00AF;
const TErrorCode Err_Content_Format                        = 0x00B0;
const TErrorCode Err_Param_Int_Format                      = 0x00B1;
const TErrorCode Err_Flags                                 = 0x00B2;
const TErrorCode Err_DIFE_Count                            = 0x00B3;
const TErrorCode Err_VIFE_Count                            = 0x00B4;
const TErrorCode Err_Numeric_Data_Format                   = 0x00B5;
const TErrorCode Err_DateTime_Not_Found                    = 0x00B6;
const TErrorCode Err_Unknown_Param                         = 0x00B7;
const TErrorCode Err_Unknown_Type                          = 0x00B8;
const TErrorCode Err_DataPak_Length                        = 0x00B9;
const TErrorCode Err_Type_Size                             = 0x00BA;
const TErrorCode Err_Parameters_Count                      = 0x00BB;
const TErrorCode Err_Save_Event                            = 0x00BC;
const TErrorCode Err_Event_Exist                           = 0x00BD;

const TErrorCode Err_DateTime                              = 0x00C0;

const TErrorCode Err_Rec_Exist_In_DB                       = 0x00CB;
const TErrorCode Err_Rec_Is_Busy                           = 0x00CC;

const TErrorCode Err_Read_Query_Result                     = 0x00CE;
const TErrorCode Err_Write_Response                        = 0x00CF;
const TErrorCode Err_Read_HTTP                             = 0x00D0;
const TErrorCode Err_Params_Not_Exists                     = 0x00D1;
const TErrorCode Err_Get_UserId                            = 0x00D2;
const TErrorCode Err_Unknown_UserType                      = 0x00D3;
const TErrorCode Err_User_Not_Activate                     = 0x00D4;
const TErrorCode Err_User_AccessTime                       = 0x00D5;
const TErrorCode Err_User_PasswordTime                     = 0x00D6;
const TErrorCode Err_Param_Year_Format                     = 0x00D7;
const TErrorCode Err_Param_Id_Format                       = 0x00D8;
const TErrorCode Err_Address_Other_Company                 = 0x00D9;
const TErrorCode Err_Param_Password_Not_Found              = 0x00DA;
const TErrorCode Err_Param_ObjType_Not_Found               = 0x00DB;
const TErrorCode Err_Param_ObjType_Format                  = 0x00DC;
const TErrorCode Err_Param_CompanyId_Format                = 0x00DD;
const TErrorCode Err_Post_Request                          = 0x00DE;
const TErrorCode Err_XML_Format                            = 0x00DF;
const TErrorCode Err_XML_ChronosData                       = 0x00E0;
const TErrorCode Err_XML_Info                              = 0x00E1;
const TErrorCode Err_XML_Data                              = 0x00E2;
const TErrorCode Err_XML_ResType                           = 0x00E3;
const TErrorCode Err_XML_ServerVer                         = 0x00E4;
const TErrorCode Err_Param_ObjCount_Big                    = 0x00E5;
const TErrorCode Err_Param_Obj_Not_Found                   = 0x00E6;
const TErrorCode Err_CMD_Format                            = 0x00E7;
const TErrorCode Err_BCD_Format                            = 0x00E8;
const TErrorCode Err_User_Status                           = 0x00E9;
const TErrorCode Err_Param_Name_Not_Found                  = 0x00EA;
const TErrorCode Err_Record_Not_Exists                     = 0x00EB;
const TErrorCode Err_Read_Time_Out                         = 0x00EC;
const TErrorCode Err_Pole_ObjType_Format                   = 0x00ED;
const TErrorCode Err_Pole_AbonentId_Format                 = 0x00EE;
const TErrorCode Err_Param_ResCode_Format                  = 0x00EF;
const TErrorCode Err_Pole_DeviceId_Format                  = 0x00F0;
const TErrorCode Err_Pole_ResCode_Format                   = 0x00F1;
const TErrorCode Err_Pole_TitleId_Format                   = 0x00F2;
const TErrorCode Err_Pole_EdId_Format                      = 0x00F3;
const TErrorCode Err_Pole_PrefixNum_Format                 = 0x00F4;
const TErrorCode Err_Pole_PostfixNum_Format                = 0x00F5;
const TErrorCode Err_Device_Not_Found                      = 0x00F6;
const TErrorCode Err_Pole_DevType_Format                   = 0x00F7;
const TErrorCode Err_Pole_Ver_Format                       = 0x00F8;
const TErrorCode Err_Pole_ModelId_Format                   = 0x00F9;
const TErrorCode Err_Pole_Status_Format                    = 0x00FA;
const TErrorCode Err_Pole_PovStat_Format                   = 0x00FB;
const TErrorCode Err_Pole_PovInterval_Format               = 0x00FC;
const TErrorCode Err_Pole_ManufId_Format                   = 0x00FD;
const TErrorCode Err_Client_Old_Version                    = 0x00FE;
const TErrorCode Err_Soft_Not_Ready                        = 0x00FF;
const TErrorCode Err_Param_Date_Format                     = 0x0100;
const TErrorCode Err_Param_Period_Format                   = 0x0101;
const TErrorCode Err_Pole_Id_Format                        = 0x0102;
const TErrorCode Err_Table_Not_Exist                       = 0x0103;
const TErrorCode Err_Company_Table_Query                   = 0x0104;
const TErrorCode Err_Pole_RegionCode_Format                = 0x0105;
const TErrorCode Err_Pole_RayonCode_Format                 = 0x0106;
const TErrorCode Err_Pole_CityCode_Format                  = 0x0107;
const TErrorCode Err_Pole_PunktCode_Format                 = 0x0108;
const TErrorCode Err_Pole_StreetCode_Format                = 0x0109;
const TErrorCode Err_Pole_SocrId_Format                    = 0x010A;
const TErrorCode Err_Pole_StreetSocrId_Format              = 0x010B;
const TErrorCode Err_Param_UserId_Format                   = 0x010C;
const TErrorCode Err_Pole_ArcDate_Format                   = 0x010D;
const TErrorCode Err_Pole_RecDate_Format                   = 0x010E;
const TErrorCode Err_Param_EdTeplo_Format                  = 0x010F;
const TErrorCode Err_Param_EdElectro_Format                = 0x0110;
const TErrorCode Err_Param_EdVoda_Format                   = 0x0111;
const TErrorCode Err_Param_EdGas_Format                    = 0x0112;
const TErrorCode Err_User_Not_Found                        = 0x0113;
const TErrorCode Err_Param_UserType_Format                 = 0x0114;
const TErrorCode Err_Param_PasOld_Not_Found                = 0x0115;
const TErrorCode Err_PasOld_Value                          = 0x0116;
const TErrorCode Err_Param_OdCount_Format                  = 0x0117;
const TErrorCode Err_Param_KvCount_Format                  = 0x0118;
const TErrorCode Err_Param_KvCount_Not_Found               = 0x0119;
const TErrorCode Err_Param_Index_Value                     = 0x011A;
const TErrorCode Err_Param_Sq_Format                       = 0x011B;
const TErrorCode Err_Param_Login_Not_Found                 = 0x011C;
const TErrorCode Err_Param_PasData_Not_Found               = 0x011D;
const TErrorCode Err_Param_PasData_Format                  = 0x011E;
const TErrorCode Err_Login_Exist_In_DB                     = 0x011F;
const TErrorCode Err_Param_View_Format                     = 0x0120;
const TErrorCode Err_Param_Month_Format                    = 0x0121;
const TErrorCode Err_Param_DateStart_Format                = 0x0122;
const TErrorCode Err_Param_DateEnd_Format                  = 0x0123;
const TErrorCode Err_Pole_DateInstall_Format               = 0x0124;
const TErrorCode Err_Param_Action_Not_Found                = 0x0125;
const TErrorCode Err_Param_Action_Unknown                  = 0x0126;

//====================================================================================================

/*******************************************************************************************/
// значения DIF
const quint8 DIF_NO_DATA = 0x00;
const quint8 DIF_INT_8   = 0x01;
const quint8 DIF_INT_16  = 0x02;
const quint8 DIF_INT_24  = 0x03;
const quint8 DIF_INT_32  = 0x04;
const quint8 DIF_REAL_32 = 0x05;
const quint8 DIF_INT_48  = 0x06;
const quint8 DIF_INT_64  = 0x07;
const quint8 DIF_READOUT = 0x08;
const quint8 DIF_BCD_2   = 0x09;
const quint8 DIF_BCD_4   = 0x0A;
const quint8 DIF_BCD_6   = 0x0B;
const quint8 DIF_BCD_8   = 0x0C;
const quint8 DIF_VAR     = 0x0D;
const quint8 DIF_BCD_12  = 0x0E;
const quint8 DIF_SPECIAL = 0x0F;

/*******************************************************************************************/
// типы устройств
const quint8 Dev_Other             = 0x00;
const quint8 Dev_Electro           = 0x02;
const quint8 Dev_Gas               = 0x03;
const quint8 Dev_Teplo             = 0x04;
const quint8 Dev_GorVoda           = 0x06;
const quint8 Dev_Voda              = 0x07;
const quint8 Dev_HolVoda           = 0x16;
const quint8 Dev_Pressure_Sensor   = 0x18;
const quint8 Dev_Impulse_Counter   = 0x19;
const quint8 Dev_Electro_Privod    = 0x20;
const quint8 Dev_Gas_Privod        = 0x21;
const quint8 Dev_Voda_Privod       = 0x22;
const quint8 Dev_Indicator         = 0x25;
const quint8 Dev_Zamok             = 0x26;
const quint8 Dev_MUC               = 0x31;
const quint8 Dev_RadioModem        = 0x36;
const quint8 Dev_Convertor         = 0x37;

/*******************************************************************************************/
typedef QVector<quint8> ByteVector;

/*******************************************************************************************/
typedef struct UserStruct { // структура записи пользователя из БД
    quint8 status;
    bool nullDatePas;
    bool nullDateActive;
    quint16 userType;
    QDateTime dateReg;
    QDateTime datePas;
    QDateTime dateActive;
    qint64 id;
    QString login;
    QString pasHash;
    QString mail;
    QString phone;
    QString avatarURL;
    QString auth;
} UserStruct;
/*******************************************************************************************/

typedef struct CompanyUserStruct { // структура записи пользователя УК
    qint64 companyId;
    qint64 sotrudnikId;
    quint16 accessType;
    QString companyName;
} CompanyUserStruct;

/*******************************************************************************************/
typedef struct WMBusFieldsStruct { // структура с полями пакета WMBus
    quint8 field_C;
    quint8 field_CI;
    quint8 field_AccNum;
    quint8 field_Status;
    quint16 field_Signature;
} WMBusFieldsStruct;

/*******************************************************************************************/
typedef struct WMBusDevStruct { // структура идентификации WMBus устройства
    quint32 id;
    quint16 manId;
    quint8 ver;
    quint8 devType;
} WMBusDevStruct;

/*******************************************************************************************/
typedef struct WMBusDataStruct { // структура - данные WMBus
    ByteVector dib;
    ByteVector vib;
    ByteVector val;
} WMBusDataStruct;

typedef QVector<WMBusDataStruct> WMBusDataVector;

/*******************************************************************************************/
typedef struct ValueStruct { // структура показаний прибора
    quint16 edId;
    quint16 titleId;
    QString val;
    int prefixNum;
    int postfixNum;
} ValueStruct;

/*******************************************************************************************/
typedef struct FlagStruct { // структура флагов состояний
    quint16 flag;
    int prefixNum;
    int postfixNum;
} FlagStruct;

/*******************************************************************************************/
typedef struct AddressStruct { // структура адрес
    quint16 regionCode;
    quint16 rayonCode;
    quint16 cityCode;
    quint16 punktCode;
    quint16 streetCode;
    quint16 socrId;
    QString houseName;
} AddressStruct;

typedef QVector<AddressStruct> AddressVector;

/*******************************************************************************************/
typedef struct EdResources {
    bool recalcTeplo;
    quint16 edTeplo;
    bool recalcElectro;
    quint16 edElectro;
    bool recalcVoda;
    quint16 edVoda;
    bool recalcGas;
    quint16 edGas;
} EdResources;

/*******************************************************************************************/
// типы концентраторов Гермес:
const quint8 Germes_G       = 0x00;
const quint8 Germes_E       = 0x01;
const quint8 Germes_D_G     = 0x02;
const quint8 Germes_D_E     = 0x03;

/*******************************************************************************************/
// коды статуса концентратора Гермес:
const quint8 GermesStatus_PowerOn       = 0x00;
const quint8 GermesStatus_PowerOn1      = 0x01;
const quint8 GermesStatus_PowerOff      = 0x02;
const quint8 GermesStatus_RestartRF     = 0x03;
const quint8 GermesStatus_CaseOpen      = 0x04;
const quint8 GermesStatus_CaseClosed    = 0x05;
const quint8 GermesStatus_ExCaseOpen    = 0x06;
const quint8 GermesStatus_ExCaseClosed  = 0x07;
const quint8 GermesStatus_RestartIrDA   = 0x08;

/*******************************************************************************************/
// Коды категорий пользователей:

// Значения от $0001 до $0080 включительно   -  категория расширенного доступа
const quint16 User_ROOT                     = 0x0001;
const quint16 User_BigAdmin                 = 0x0002;
const quint16 User_BigOperator              = 0x0003;
const quint16 User_BigUser                  = 0x0004;
// Значения от $0081 до $0100 включительно   -  доступ для сотрудников компаний
const quint16 User_CompanyAdmin             = 0x0081;
const quint16 User_CompanyOperator          = 0x0082;
const quint16 User_CompanyUser              = 0x0083;

/*******************************************************************************************/
// статус учетной записи
const quint8 Status_NotActivate = 0x00;
const quint8 Status_Enable      = 0x01;
const quint8 Status_Disable     = 0x02;

/*******************************************************************************************/
// типы ресурсов
const quint16 ResCode_None           = 0x0000;
const quint16 ResCode_Voda           = 0x0001;
const quint16 ResCode_HolVoda        = 0x0002;
const quint16 ResCode_GorVoda        = 0x0003;
const quint16 ResCode_Electro        = 0x0004;
const quint16 ResCode_Gas            = 0x0005;
const quint16 ResCode_Teplo          = 0x0006;
const quint16 ResCode_HolVoda_ForGor = 0x0007;

/*******************************************************************************************/
// типы объектов
const quint16 ObjType_None         = 0x0000;
const quint16 ObjType_OneHouse     = 0x0001;
const quint16 ObjType_House        = 0x0002;
const quint16 ObjType_KvInHouse    = 0x0003;
const quint16 ObjType_TechInHouse  = 0x0004;
const quint16 ObjType_Office       = 0x0005;
const quint16 ObjType_Stroenie     = 0x0006;
const quint16 ObjType_Kolodets     = 0x0007;
const quint16 ObjType_Obshee       = 0x0008;

/*******************************************************************************************/
// состояние устройства
const quint16 DevStatus_Active    = 0x0000;
const quint16 DevStatus_NotActive = 0x0001;
/*******************************************************************************************/
// коды единиц измерения:
const quint16 Ed_None           = 0x0000;
const quint16 Ed_Wh             = 0x0001;
const quint16 Ed_kWh            = 0x0002;
const quint16 Ed_VARh           = 0x0003;
const quint16 Ed_kVARh          = 0x0004;
const quint16 Ed_J              = 0x0005;
const quint16 Ed_kJ             = 0x0006;
const quint16 Ed_Kal            = 0x0007;
const quint16 Ed_kKal           = 0x0008;
const quint16 Ed_GKal           = 0x0009;
const quint16 Ed_m3             = 0x000A;
const quint16 Ed_l              = 0x000B;
const quint16 Ed_g              = 0x000C;
const quint16 Ed_kg             = 0x000D;
const quint16 Ed_t              = 0x000E;
const quint16 Ed_sec            = 0x000F;
const quint16 Ed_min            = 0x0010;
const quint16 Ed_hour           = 0x0011;
const quint16 Ed_day            = 0x0012;
const quint16 Ed_mounth         = 0x0013;
const quint16 Ed_year           = 0x0014;
const quint16 Ed_W              = 0x0015;
const quint16 Ed_Jh             = 0x0016;
const quint16 Ed_MKal           = 0x0017;
const quint16 Ed_MKalh          = 0x0018;
const quint16 Ed_m3h            = 0x0019;
const quint16 Ed_m3m            = 0x001A;
const quint16 Ed_m3s            = 0x001B;
const quint16 Ed_kgh            = 0x001C;
const quint16 Ed_gradC          = 0x001D;
const quint16 Ed_gradK          = 0x001E;
const quint16 Ed_gradF          = 0x001F;
const quint16 Ed_bar            = 0x0020;
const quint16 Ed_kgcm2          = 0x0021;
const quint16 Ed_MWh            = 0x0022;
const quint16 Ed_MJ             = 0x0023;
const quint16 Ed_GJ             = 0x0024;
const quint16 Ed_kW             = 0x0025;
const quint16 Ed_MW             = 0x0026;
const quint16 Ed_GW             = 0x0027;
const quint16 Ed_proc           = 0x0028;
const quint16 Ed_dBm            = 0x0029;
const quint16 Ed_GWh            = 0x002A;

/*******************************************************************************************/
// коды заголовков
const quint16 Title_None                    = 0x0000;
const quint16 Title_Energy                  = 0x0001;
const quint16 Title_ElectroEnergy           = 0x0002;
const quint16 Title_ReactiveEnergy          = 0x0003;
const quint16 Title_TermalEnergy            = 0x0004;
const quint16 Title_Volume                  = 0x0005;
const quint16 Title_Mass                    = 0x0006;
const quint16 Title_TimeWorking             = 0x0007;
const quint16 Title_TimeCharging            = 0x0008;
const quint16 Title_Power                   = 0x0009;
const quint16 Title_VolumeFlowRate          = 0x000A;
const quint16 Title_MassFlowRate            = 0x000B;
const quint16 Title_TempDiff                = 0x000C;
const quint16 Title_TempExternal            = 0x000D;
const quint16 Title_Pressure                = 0x000E;
const quint16 Title_Duration                = 0x000F;
const quint16 Title_DurationAver            = 0x0010;
const quint16 Title_Temp                    = 0x0011;
const quint16 Title_DeviceModification      = 0x0012;
const quint16 Title_SoftwareVer             = 0x0013;
const quint16 Title_HardwareVer             = 0x0014;
const quint16 Title_State                   = 0x0015;
const quint16 Title_Status                  = 0x0016;
const quint16 Title_Version                 = 0x0017;
const quint16 Title_DateDevTransmit         = 0x0018;
const quint16 Title_TimeDevTransmit         = 0x0019;
const quint16 Title_BatChargeLevel          = 0x001A;
const quint16 Title_SignalLevel             = 0x001B;
const quint16 Title_CoolantVolume           = 0x001C;
const quint16 Title_TermalPower             = 0x001D;
const quint16 Title_TempFlow                = 0x001E;
const quint16 Title_TempReturn              = 0x001F;
const quint16 Title_CoolantFlowRate         = 0x0020;
const quint16 Title_MassOfDemand            = 0x0021;
const quint16 Title_TempColWater            = 0x0022;
const quint16 Title_TempHotWater            = 0x0023;
const quint16 Title_TempAir                 = 0x0024;
const quint16 Title_PeriodNormalWork        = 0x0025;
const quint16 Title_PeriodNotCalc           = 0x0026;
const quint16 Title_VolumeAdditionIn        = 0x0027;
const quint16 Title_ReactiveEnergyRev       = 0x0028;
const quint16 Title_ElectroEnergyRev        = 0x0029;
const quint16 Title_TermalEnergyRev         = 0x002A;
const quint16 Title_EnergyRev               = 0x002B;
const quint16 Title_WaterVolume             = 0x002C;
const quint16 Title_GasVolume               = 0x002D;
const quint16 Title_WaterMass               = 0x002E;
const quint16 Title_PeriodPowerUp           = 0x002F;
const quint16 Title_UnknownVIF              = 0x0030;
const quint16 Title_FirmwareVer             = 0x0031;
const quint16 Title_FirmwareId              = 0x0032;
const quint16 Title_PressureFlow            = 0x0033;
const quint16 Title_PressureReturn          = 0x0034;
const quint16 Title_WaterMassFlow           = 0x0035;
const quint16 Title_WaterMassReturn         = 0x0036;

/*******************************************************************************************/
const QChar charXML[] = {'"', '&', '<', '>', '\n', '\r'};
const QString stringXML[] = {"&quot;", "&amp;", "&lt;", "&gt;", "&#10;", "&#13;"};
/*******************************************************************************************/

QString getAnswerTitle(AnswerType answerType);
QString getAnswerTypeStr(AnswerType answerType);
QString encodeXMLText(const QString &text);
QString decodeXMLText(const QString &text);
QString getErrorText(TErrorCode code);
QString getTitleText(quint16 code);
QString getEdText(quint16 code);
QString getDevTypeNameText(quint8 code);
QString getUserTypeText(quint16 code);
QString getUserStatusText(quint8 code);
bool userTypeExists(quint16 code);
QString getResCodeText(quint16 code);
QString getDevStatusText(quint16 code);
QString getObjTypeText(quint16 code);
QString getGermesTypeText(quint8 code);
QString getGermesStatusText(quint8 code);
QString getShortStrFromDevType(quint8 devType);
quint8 getDevTypeFromStr(const QString &typeStr);
int calcFullPacketLen(quint8 field_L);
int calcBlockCount(quint8 field_L);
int calcLastBlockLen(quint8 field_L);
quint16 crc16(const ByteVector &data, int startIndex, int len);
quint16 crc16(const QByteArray &data, int startIndex, int len);
unsigned short crc16(unsigned char * pcBlock, int len);
QString wordToManId(quint16 val);
quint16 manIdToWord(const QString &manId);
bool itsCorrectManId(const QString &manId);
bool itsCorrectManId(quint16 manId);
bool itsCorrectIdentNum(const QString &identNum);
bool itsCorrectIdentNum(quint32 identNum);
QString getIdentNumText(QVariant idNum);
QString delPrevNulls(const QString &val);
QString delPostNulls(const QString &val, int precise = 1);
QString calcStepen(int stepen, const QString &strVal);
QString md5(const QString &data);
QString getIdText(quint64 value);
QString correctingBCD(const QString &val, bool *ok = Q_NULLPTR);
QString getValueStr(quint8 devType, const ValueStruct &value);
QString getErrorsFlagsText(const WMBusDevStruct &device, quint32 errorCode, int prefix = 0, int postfix = 0);
bool getTarifEnergyRec(quint8 devType, const ByteVector &dif, ValueStruct &value);
bool getTarifReactiveEnergyRec(const ByteVector &dif, ValueStruct &value);
bool getTarifVolumeRec(quint8 devType, const ByteVector &dif, ValueStruct &value);
bool getTarifMassRec(quint8 devType, const ByteVector &dif, ValueStruct &value);
bool getTarifFlowTempRec(const ByteVector &dif, ValueStruct &value);
bool getTarifReturnTempRec(ValueStruct &value);
bool getTarifPressureRec(const ByteVector &dif, ValueStruct &value);
bool getTarifTempDiffRec(const ByteVector &dif, ValueStruct &value);
bool getTarifTempExternalRec(const ByteVector &dif, ValueStruct &value);
bool getTarifDurationRec(const ByteVector &dif, ValueStruct &value);
bool getTarifDurationAverRec(ValueStruct &value);
bool getTarifVolumeFlowRateRec(const ByteVector &dif, ValueStruct &value);
bool getTarifMassFlowRateRec(ValueStruct &value);
bool getTarifPowerRec(quint8 devType, ValueStruct &value);
bool getTarifTimeWorkingRec(ValueStruct &value);
bool getTarifTimeChargingRec(ValueStruct &value);
bool getTarifPeriodPowerUpRec(ValueStruct &value);
bool getTarifPeriodWorkingRec(quint8 devType, const ByteVector &dif, ValueStruct &value);
bool getTarifPeriodNotCalcRec(quint8 devType, const ByteVector &dif, ValueStruct &value);
bool isValue(const ValueStruct &value, bool *isOneTime);
// функция разбора WMBus пакета:
TErrorCode calcSummFromTarifs(const WMBusDevStruct &device, QVector<ValueStruct> &vecVal, QString &logStr);
TErrorCode readValuesFromArray(const QByteArray &data, const WMBusDevStruct &device, QDateTime &dt,
                               QVector<ValueStruct> &vecVal, QVector<FlagStruct> &vecFlag, QString &logStr,
                               QVector<ValueStruct> &vecTech);

TErrorCode readValuesFromByteVector(const ByteVector &data, const WMBusDevStruct &device,
                               QDateTime &dt, QVector<ValueStruct> &vecVal, QVector<FlagStruct> &vecFlag, QString &logStr,
                               QVector<ValueStruct> &vecTech);

void saveErrorVIFToTech(const QString val, const ByteVector &vif, QVector<ValueStruct> &vecTech);
TErrorCode readDeviceFromArray(const QByteArray &data, WMBusDevStruct &device, QString &logStr);
bool itsEnergyTitle(quint8 devType, quint16 titleId);
bool itsVolumeTitle(quint8 devType, quint16 titleId);
bool itsMainValueRec(quint8 devType, const ValueStruct &val, int prefix = 0, int postfix = 0);
bool calcValueToEdId(quint16 edId, ValueStruct &val);
int indexOfValueStruct(const ValueStruct &value, const QVector<ValueStruct> &vecVal);
/*******************************************************************************************/
} // namespace wmbus

#endif // WMBUS_H
