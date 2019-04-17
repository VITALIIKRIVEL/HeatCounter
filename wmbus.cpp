#include <QCryptographicHash>
#include <QLocale>
#include "wmbus.h"
#include "global.h"

/*******************************************************************************************/
namespace wmbus {

/*******************************************************************************************/
QString getAnswerTitle(AnswerType answerType)
{
    switch (answerType) {
    case Answer_OK:    return QObject::tr("Запрос выполнен");
    case Answer_Info:  return QObject::tr("Информация");
    case Answer_Table: return QObject::tr("Результат запроса");
    case Answer_Error: return QObject::tr("Ошибка при выполнении");
    default: break;
    }
    return "";
}
/*******************************************************************************************/
QString getAnswerTypeStr(AnswerType answerType)
{
    switch (answerType) {
    case Answer_OK:    return "OK";
    case Answer_Info:  return "INFO";
    case Answer_Table: return "TABLE";
    case Answer_Error: return "ERROR";
    default: break;
    }
    return "";
}
/*******************************************************************************************/
QString encodeXMLText(const QString &text)
{
    QString res = "";
    bool ok;

    for(int i=0; i<text.length(); i++) {
        for(int j=0; j<6; j++) {
            ok=false;
            if( text.at(i) == charXML[j] ) {
                ok = true;
                res += stringXML[j];
                break;
            }
        }
        if (!ok) res += text.mid(i,1);
    }

    return res;
}
/*******************************************************************************************/
QString decodeXMLText(const QString &text)
{
    QString res = text;

    for(int j=0; j<6; j++) {
        int n = res.indexOf( stringXML[j] );
        while(n >= 0) {
            res.remove( n, stringXML[j].length() );
            res.insert(n, charXML[j]);
            n = res.indexOf( stringXML[j] );
        }
    }

    return res;
}
/*******************************************************************************************/
QString getErrorText(TErrorCode code)
{
    switch (code) {
    case Err_OK:                            return QObject::tr("OK.");
    case Err_Unknown_CMD:                   return QObject::tr("Неизвестная команда (параметр CMD).");
    case Err_CMD_Not_Found:                 return QObject::tr("Параметр CMD отсутствует.");
    case Err_Params_Not_Found:              return QObject::tr("Пустая строка параметров.");
    case Err_Param_Date_Not_Found:          return QObject::tr("Отсутствует параметр Date, обязательный для данного запроса.");
    case Err_Param_Time_Not_Found:          return QObject::tr("Отсутствует параметр Time, обязательный для данного запроса.");
    case Err_Param_Dev_Not_Found:           return QObject::tr("Отсутствует параметр Dev, обязательный для данного запроса.");
    case Err_Param_HardVer_Not_Found:       return QObject::tr("Отсутствует параметр HardVer, обязательный для данного запроса.");
    case Err_Param_SoftVer_Not_Found:       return QObject::tr("Отсутствует параметр SoftVer, обязательный для данного запроса.");
    case Err_Param_Status_Not_Found:        return QObject::tr("Отсутствует параметр Status, обязательный для данного запроса.");
    case Err_Param_WMBMan_Not_Found:        return QObject::tr("Отсутствует параметр wmbMan, необходимый для идентификации устройства.");
    case Err_Param_WMBId_Not_Found:         return QObject::tr("Отсутствует параметр wmbId, необходимый для идентификации устройства.");
    case Err_Param_WMBDev_Not_Found:        return QObject::tr("Отсутствует параметр wmbDev, необходимый для идентификации устройства.");
    case Err_Param_WMBVer_Not_Found:        return QObject::tr("Отсутствует параметр wmbVer, необходимый для идентификации устройства.");
    case Err_Param_WMBMan_Format:           return QObject::tr("Неправильный формат параметра wmbMan.");
    case Err_Param_WMBId_Format:            return QObject::tr("Неправильный формат параметра wmbId.");
    case Err_Param_WMBDev_Format:           return QObject::tr("Неправильный формат параметра wmbDev.");
    case Err_Param_WMBVer_Format:           return QObject::tr("Неправильный формат параметра wmbVer.");
    case Err_Values_And_Flags_Is_Empty:     return QObject::tr("Отсутствуют показания и флаги состояний устройства. Нет данных для сохранения.");
    case Err_Server_Connect:                return QObject::tr("Не удалось подключиться к серверу БД.");
    case Err_DataBase_Connect:              return QObject::tr("Не удалось подключиться к БД после соединения с сервером.");
    case Err_Server_CodePage:               return QObject::tr("Не удалось установить нужную кодировку на сервере БД.");
    case Err_DataBase_Connect_Not_Found:    return QObject::tr("Отсутствует соединение с БД.");
    case Err_Init_Settings_Table:           return QObject::tr("Ошибка при проверке и создании таблицы настроек.");
    case Err_Read_Settings_Table:           return QObject::tr("Не удалось прочитать таблицу настроек.");
    case Err_GET_Query_Disable:             return QObject::tr("GET запросы запрещены в настройках.");
    case Err_Device_Table_Query:            return QObject::tr("Ошибка при выполнении запроса к таблице устройств.");
    case Err_Device_Not_Exist:              return QObject::tr("Устройство отсутствует в БД.");
    case Err_Record_Exist:                  return QObject::tr("Запись для данного устройства на указанную дату уже существует.");
    case Err_Add_Archive_Record:            return QObject::tr("Не удалось добавить запись в архив.");
    case Err_Archive_Table_Query:           return QObject::tr("Ошибка при выполнении запроса к таблице архива.");
    case Err_Get_Archive_Table_Id:          return QObject::tr("Не удалось получить идентификатор записи архива.");
    case Err_Values_Is_Empty:               return QObject::tr("Нет значений в списке показаний.");
    case Err_Save_Values:                   return QObject::tr("Не удалось сохранить показания устройства.");
    case Err_Flags_Is_Empty:                return QObject::tr("Нет значений в списке флагов состояний.");
    case Err_Save_Flags:                    return QObject::tr("Не удалось сохранить флаги состояний устройства.");
    case Err_Save_ArcId_In_DevTable:        return QObject::tr("Не удалось сохранить идентификатор архивной записи в таблице устройства.");
    case Err_Load_SQL_Script_File:          return QObject::tr("Не удалось загрузить файл SQL-скрипта.");
    case Err_SQL_Script:                    return QObject::tr("Ошибка при выполнении SQL-скрипта.");
    case Err_Delete_Tables_From_DB:         return QObject::tr("Ошибка при удалении таблиц БД.");
    case Err_Region_Table_Query:            return QObject::tr("Ошибка при выполнении запроса к таблице Регионов КЛАДРа.");
    case Err_Param_SortCol:                 return QObject::tr("Параметр SortCol задан неверно.");
    case Err_Param_SortType:                return QObject::tr("Параметр SortType задан неверно.");
    case Err_Param_RowCount_Format:         return QObject::tr("Неправильный формат параметра RowCount.");
    case Err_Param_StartRowNum_Format:      return QObject::tr("Неправильный формат параметра StartRowNum.");
    case Err_Param_RowCount:                return QObject::tr("Параметр RowCount задан неверно.");
    case Err_Param_StartRowNum:             return QObject::tr("Параметр StartRowNum задан неверно.");
    case Err_Rayon_Table_Query:             return QObject::tr("Ошибка при выполнении запроса к таблице Районов КЛАДРа.");
    case Err_City_Table_Query:              return QObject::tr("Ошибка при выполнении запроса к таблице Городов КЛАДРа.");
    case Err_Punkt_Table_Query:             return QObject::tr("Ошибка при выполнении запроса к таблице Населенных пунктов КЛАДРа.");
    case Err_Street_Table_Query:            return QObject::tr("Ошибка при выполнении запроса к таблице Улиц КЛАДРа.");
    case Err_House_Table_Query:             return QObject::tr("Ошибка при выполнении запроса к таблице Домов КЛАДРа.");
    case Err_Param_SocrId_Format:           return QObject::tr("Неправильный формат параметра SocrId.");
    case Err_Param_SocrIdMin_Format:        return QObject::tr("Неправильный формат параметра SocrIdMin.");
    case Err_Param_SocrIdMax_Format:        return QObject::tr("Неправильный формат параметра SocrIdMax.");
    case Err_Param_SocrLevel_Format:        return QObject::tr("Неправильный формат параметра SocrLevel.");
    case Err_Socr_Table_Query:              return QObject::tr("Ошибка при выполнении запроса к таблице Типов КЛАДРа.");
    case Err_Param_IndexId_Format:          return QObject::tr("Неправильный формат параметра IndexId.");
    case Err_Index_Table_Query:             return QObject::tr("Ошибка при выполнении запроса к таблице Индексов КЛАДРа.");
    case Err_Param_AddressId_Format:        return QObject::tr("Неправильный формат параметра AddressId.");
    case Err_Address_Table_Query:           return QObject::tr("Ошибка при выполнении запроса к таблице Адресов.");
    case Err_Temp_File_Create:              return QObject::tr("Не удалось создать временный файл.");
    case Err_Param_RegionCode_Not_Found:    return QObject::tr("Отсутствует параметр RegionCode, обязательный для данного запроса.");
    case Err_Param_RayonCode_Not_Found:     return QObject::tr("Отсутствует параметр RayonCode, обязательный для данного запроса.");
    case Err_Param_CityCode_Not_Found:      return QObject::tr("Отсутствует параметр CityCode, обязательный для данного запроса.");
    case Err_Param_PunktCode_Not_Found:     return QObject::tr("Отсутствует параметр PunktCode, обязательный для данного запроса.");
    case Err_Param_StreetCode_Not_Found:    return QObject::tr("Отсутствует параметр StreetCode, обязательный для данного запроса.");
    case Err_Param_RegionCode_Format:       return QObject::tr("Неправильный формат параметра RegionCode.");
    case Err_Param_RayonCode_Format:        return QObject::tr("Неправильный формат параметра RayonCode.");
    case Err_Param_CityCode_Format:         return QObject::tr("Неправильный формат параметра CityCode.");
    case Err_Param_PunktCode_Format:        return QObject::tr("Неправильный формат параметра PunktCode.");
    case Err_Param_StreetCode_Format:       return QObject::tr("Неправильный формат параметра StreetCode.");
    case Err_Param_SocrId_Format_2:         return QObject::tr("Неправильный формат параметра SocrId.");
    case Err_Param_Index_Format:            return QObject::tr("Неправильный формат параметра Index.");
    case Err_Address_Table_Record_Exist:    return QObject::tr("В таблице Адресов уже имеется такая запись.");
    case Err_Address_Table_Insert:          return QObject::tr("Не удалось вставить запись в таблицу Адресов.");
    case Err_Param_AddressId_Not_Found:     return QObject::tr("Отсутствует параметр AddressId, обязательный для данного запроса.");
    case Err_Pole_AddressId_Format:         return QObject::tr("Неправильный формат поля AddressId.");
    case Err_Address_Use_Id_Device_Table:   return QObject::tr("Указанный адрес используется в таблице Устройств.");
    case Err_Address_Use_Id_Abonent_Table:  return QObject::tr("Указанный адрес используется в таблице Абонентов.");
    case Err_Query_To_DataBase:             return QObject::tr("Не удалось выполнить запрос к БД.");
    case Err_Query_Result_Not_Correct:      return QObject::tr("Некорректный результат запроса.");
    case Err_Param_AbonentTypeId_Format:    return QObject::tr("Неправильный формат параметра AbonentType.");
    case Err_Query_To_AbonentType_Table:    return QObject::tr("Ошибка при выполнении запроса к таблице Типов абонентов.");
    case Err_Param_DevType_Format:          return QObject::tr("Неправильный формат параметра DevType.");
    case Err_Query_To_DevType_Table:        return QObject::tr("Ошибка при выполнении запроса к таблице Типов устройств.");
    case Err_Param_ArcType_Format:          return QObject::tr("Неправильный формат параметра ArcType.");
    case Err_Query_To_ArcType_Table:        return QObject::tr("Ошибка при выполнении запроса к таблице Типов архивных записей.");
    case Err_Param_AccountId_Format:        return QObject::tr("Неправильный формат параметра AccountId.");
    case Err_Query_To_Account_Table:        return QObject::tr("Ошибка при выполнении запроса к таблице Лицевых счетов.");
    case Err_DeviceId_Format:               return QObject::tr("Неправильный формат параметра DeviceId.");
    case Err_IdentNum_Format:               return QObject::tr("Неправильный формат параметра IdentNum.");
    case Err_Ver_Format:                    return QObject::tr("Неправильный формат параметра Ver.");
    case Err_InstallDate_Format:            return QObject::tr("Неправильный формат параметра InstallDate.");
    case Err_InstallDateMin_Format:         return QObject::tr("Неправильный формат параметра InstallDateMin.");
    case Err_InstallDateMax_Format:         return QObject::tr("Неправильный формат параметра InstallDateMax.");
    case Err_LastPovDate_Format:            return QObject::tr("Неправильный формат параметра LastPovDate.");
    case Err_LastPovDateMin_Format:         return QObject::tr("Неправильный формат параметра LastPovDateMin.");
    case Err_LastPovDateMax_Format:         return QObject::tr("Неправильный формат параметра LastPovDateMax.");
    case Err_Param_ManId_Not_Found:         return QObject::tr("Отсутствует параметр ManId, необходимый для идентификации устройства.");
    case Err_Param_IdentNum_Not_Found:      return QObject::tr("Отсутствует параметр IdentNum, необходимый для идентификации устройства.");
    case Err_Param_DevType_Not_Found:       return QObject::tr("Отсутствует параметр DevType, необходимый для идентификации устройства.");
    case Err_Param_Ver_Not_Found:           return QObject::tr("Отсутствует параметр Ver, необходимый для идентификации устройства.");
    case Err_Param_ManId_Format:            return QObject::tr("Неправильный формат параметра ManId.");
    case Err_Query:                         return QObject::tr("Ошибка при выполнении запроса.");
    case Err_Values_Table_Query:            return QObject::tr("Ошибка при выполнении запроса к таблице Показаний.");
    case Err_Errors_Table_Query:            return QObject::tr("Ошибка при выполнении запроса к таблице Флагов состояний.");
    case Err_Device_Table_Rec_Exist:        return QObject::tr("В таблице Устройств уже имеется такая запись.");
    case Err_Param_PovInterval_Format:      return QObject::tr("Неправильный формат параметра PovInterval.");
    case Err_Add_Device_Table_Rec:          return QObject::tr("Не удалось вставить запись в таблицу Устройств.");
    case Err_Param_DevId_Not_Found:         return QObject::tr("Отсутствует параметр Dev_Id, обязательный для данного запроса.");
    case Err_Device_Use_In_Values_Table:    return QObject::tr("Указанное устройство используется в таблице Показаний.");
    case Err_Address_Not_Found:             return QObject::tr("Адрес не найден.");
    case Err_Param_ArcDate_Format:          return QObject::tr("Неправильный формат параметра ArcDate.");
    case Err_Param_ArcDateMin_Format:       return QObject::tr("Неправильный формат параметра ArcDateMin.");
    case Err_Param_ArcDateMax_Format:       return QObject::tr("Неправильный формат параметра ArcDateMax.");
    case Err_Param_AbonentId_Format:        return QObject::tr("Неправильный формат параметра AbonentId.");
    case Err_Abonent_Table_Query:           return QObject::tr("Ошибка при выполнении запроса к таблице Абонентов.");
    case Err_Function_Disable:              return QObject::tr("Фунция временно отключена. Идут работы на сервере.");
    case 0x0077:                            return QObject::tr("Отсутствует параметр AddressId.");
    case 0x0078:                            return QObject::tr("Отсутствует параметр AbonentName.");
    case 0x0079:                            return QObject::tr("Неправильный формат параметра AbonentName.");
    case 0x007A:                            return QObject::tr("В таблице Абонентов уже имеется такая запись.");
    case 0x007B:                            return QObject::tr("Не удалось вставить запись в таблицу Абонентов.");
    case 0x007C:                            return QObject::tr("Отсутствует параметр AbonentId.");
    case 0x007D:                            return QObject::tr("Неправильный формат параметра AbonentId.");
    case Err_Param_TimeShift_Not_Found:     return QObject::tr("Отсутствует параметр TimeShift.");
    case Err_Param_TimeShift_Format:        return QObject::tr("Неправильный формат параметра TimeShift.");
    case Err_Pole_ObjectId_Format:          return QObject::tr("Неправильный формат параметра ObjectId.");
    case Err_JSON_AnswerType:               return QObject::tr("Ошибка Json: несоответствие типа ответа.");
    case Err_Only_HTTPS_Query_Enabled:      return QObject::tr("Разрешены только защищенные HTTPS соединения.");
    case Err_JSON_Format:                   return QObject::tr("Неправильный формат JSON.");
    case Err_SQL_Query:                     return QObject::tr("Ошибка при выполнении SQL запроса.");
    case Err_Query_Time_Out:                return QObject::tr("Тайм-аут передачи данных.");
    case Err_Data_Decrypt:                  return QObject::tr("Не удалось дешифровать данные.");
    case Err_Decrypt_Format:                return QObject::tr("Неверный формат строки данных после дешифрования.");
    case Err_Param_AuthData_Not_Found:      return QObject::tr("Отсутствует параметр AuthData.");
    case Err_Param_AuthData_Format:         return QObject::tr("Неправильный формат параметра AuthData.");
    case Err_User_Table_Query:              return QObject::tr("Ошибка при выполнении запроса к таблице Пользователей.");
    case Err_Login_Password:                return QObject::tr("Неверное имя пользователя или пароль.");
    case Err_User_Disable:                  return QObject::tr("Пользователь отключен.");
    case 0x008D:                            return QObject::tr("Не найдены настройки уровня доступа для пользователя.");
    case 0x008E:                            return QObject::tr("Неверный формат настройки уровня доступа для пользователя.");
    case Err_CMD_Not_Access:                return QObject::tr("Команда недоступна для указанного пользователя.");
    case Err_Abonent_Not_Found:             return QObject::tr("Абонент не найден.");
    case Err_Param_ObjectId_Not_Found:      return QObject::tr("Отсутствует параметр ObjectId.");
    case Err_Param_ObjectId_Format:         return QObject::tr("Неправильный формат параметра ObjectId.");
    case Err_Param_UserAuth_Format:         return QObject::tr("Неправильный формат параметра UserAuth.");
    case Err_Insert_UserTable:              return QObject::tr("Не удалось вставить запись в Таблицу Пользователей.");
    case Err_Param_UserId_NotFound:         return QObject::tr("Отсутствует параметр UserId.");
    case Err_Edit_UserTable:                return QObject::tr("Не удалось изменить запись в Таблице Пользователей.");
    case 0x0097:                            return QObject::tr("Отсутствует параметр UserEnable.");
    case 0x0098:                            return QObject::tr("Неверный формат параметра UserEnable.");
    case 0x0099:                            return QObject::tr("Не удалось удалить запись из Таблицы Пользователей.");
    case Err_Login_Busy:                    return QObject::tr("Указанный логин занят другим пользователем.");
    case Err_LastId_Query:                  return QObject::tr("Ошибка при выполнении запроса на получение идентификатора.");
    case Err_Param_Data_Not_Found:          return QObject::tr("Отсутствует параметр DATA, обязательный для данного запроса.");
    case Err_Param_Data_Format:             return QObject::tr("Неизвестный формат параметра DATA.");
    case Err_DateTime_Format:               return QObject::tr("Неправильный формат даты или времени.");
    case Err_Param_Dev_Format:              return QObject::tr("Неправильный формат параметра Dev.");
    case Err_Param_Status_Format:           return QObject::tr("Неправильный формат параметра Status.");
    case Err_Param_Data_Length:             return QObject::tr("Несоответствие размера данных параметра DATA.");
    case Err_CRC:                           return QObject::tr("Неверная контрольная сумма.");
    case Err_DIF_Value:                     return QObject::tr("Неверное значение DIF для данного устройства.");
    case Err_VIF_Value:                     return QObject::tr("Неверное значение VIF для данного устройства.");
    case Err_Data_End:                      return QObject::tr("Неожиданный конец данных - несоответствие формата.");
    case Err_Value:                         return QObject::tr("Неверное значение показаний прибора.");
    case Err_Pak_Length:                    return QObject::tr("Неверная длина пакета данных устройства.");
    case Err_File_Not_Exists:               return QObject::tr("Файл не найден.");
    case Err_File_Not_Open:                 return QObject::tr("Не удалось открыть файл.");
    case Err_Query_Method:                  return QObject::tr("Неверный метод запроса.");
    case Err_Content_Length:                return QObject::tr("Параметр Content-Length задан неверно.");
    case Err_Content_And_Data_Length:       return QObject::tr("Параметр Content-Length не совпадает с длиной данных.");
    case Err_Content_Type:                  return QObject::tr("Неизвестный тип параметра Content-Type.");
    case Err_Boundary_Not_Found:            return QObject::tr("Неверный формат заголовка. Разделитель (boundary) не найден.");
    case Err_Read_Content:                  return QObject::tr("Не удалось прочитать Content данные.");
    case Err_Content_Format:                return QObject::tr("Неверный формат Content данных.");
    case Err_Param_Int_Format:              return QObject::tr("Неправильный формат числового параметра.");
    case Err_Flags:                         return QObject::tr("Ошибка при разборе флагов состояний.");
    case Err_DIFE_Count:                    return QObject::tr("Превышение количества DIFE байт.");
    case Err_VIFE_Count:                    return QObject::tr("Превышение количества VIFE байт.");
    case Err_Numeric_Data_Format:           return QObject::tr("Неправильный формат числовых данных.");
    case Err_DateTime_Not_Found:            return QObject::tr("Дата и время не найдены.");
    case Err_Unknown_Param:                 return QObject::tr("Неизвестный параметр.");
    case Err_Unknown_Type:                  return QObject::tr("Неизвестный тип данных.");
    case Err_DataPak_Length:                return QObject::tr("Несоответствие размера пакета данных.");
    case Err_Type_Size:                     return QObject::tr("Неверное значение размера для указанного типа данных.");
    case Err_Parameters_Count:              return QObject::tr("Недостаточно параметров для сохранения события.");
    case Err_Save_Event:                    return QObject::tr("Не удалось сохранить событие в БД.");
    case Err_Event_Exist:                   return QObject::tr("Событие уже есть в БД.");
    case 0x00BE:                            return QObject::tr("Не удалось прочитать журнал событий.");
    case 0x00BF:                            return QObject::tr("Не удалось распознать тип события.");
    case Err_DateTime:                      return QObject::tr("Неверная дата или время.");
    case 0x00C1:                            return QObject::tr("Отсутствует параметр DoorLockId, обязательный для данного запроса.");
    case 0x00C2:                            return QObject::tr("Указанный замок отсутствует в БД.");
    case 0x00C3:                            return QObject::tr("В БД уже есть замок с указанным идентификатором.");
    case 0x00C4:                            return QObject::tr("Отсутствует параметр SotrudnikId, обязательный для данного запроса.");
    case 0x00C5:                            return QObject::tr("Указанный сотрудник отсутствует в БД.");
    case 0x00C6:                            return QObject::tr("Отсутствует параметр DoorKeyId, обязательный для данного запроса.");
    case 0x00C7:                            return QObject::tr("Указанный ключ отсутствует в БД.");
    case 0x00C8:                            return QObject::tr("В БД уже есть ключ с указанным идентификатором.");
    case 0x00C9:                            return QObject::tr("Отсутствует параметр DolgnostId, обязательный для данного запроса.");
    case 0x00CA:                            return QObject::tr("Отсутствует параметр DolgnostName, обязательный для данного запроса.");
    case Err_Rec_Exist_In_DB:               return QObject::tr("В БД уже имеется такая запись.");
    case Err_Rec_Is_Busy:                   return QObject::tr("Указанная запись используется в других таблицах.");
    case 0x00CD:                            return QObject::tr("Отсутствует параметр SotrudnikName, обязательный для данного запроса.");
    case Err_Read_Query_Result:             return QObject::tr("Ошибка при чтении результата запроса.");
    case Err_Write_Response:                return QObject::tr("Ошибка при записи содержимого HTTP ответа.");
    case Err_Read_HTTP:                     return QObject::tr("Ошибка при считывании содержимого HTTP запроса.");
    case Err_Params_Not_Exists:             return QObject::tr("Отсутствуют обязательные параметры.");
    case Err_Get_UserId:                    return QObject::tr("Не удалось получить идентификатор пользователя.");
    case Err_Unknown_UserType:              return QObject::tr("Неизвестный тип пользователя.");
    case Err_User_Not_Activate:             return QObject::tr("Учетная запись не активирована.");
    case Err_User_AccessTime:               return QObject::tr("Истекло время действия учетной записи.");
    case Err_User_PasswordTime:             return QObject::tr("Истекло время действия пароля.");
    case Err_Param_Year_Format:             return QObject::tr("Неправильный формат параметра Year.");
    case Err_Param_Id_Format:               return QObject::tr("Неправильный формат параметра Id.");
    case Err_Address_Other_Company:         return QObject::tr("Указанный адрес пренадлежит другой компании.");
    case Err_Param_Password_Not_Found:      return QObject::tr("Отсутствует параметр Password, обязательный для данного запроса.");
    case Err_Param_ObjType_Not_Found:       return QObject::tr("Отсутствует параметр ObjType.");
    case Err_Param_ObjType_Format:          return QObject::tr("Неправильный формат параметра ObjType.");
    case Err_Param_CompanyId_Format:        return QObject::tr("Неправильный формат параметра CompanyId.");
    case Err_Post_Request:                  return QObject::tr("Не удалось отправить POST запрос на сервер.");
    case Err_XML_Format:                    return QObject::tr("Неправильный формат XML.");
    case Err_XML_ChronosData:               return QObject::tr("Ошибка XML: отсутствует основной раздел <chronosData>.");
    case Err_XML_Info:                      return QObject::tr("Ошибка XML: отсутствует раздел <info>.");
    case Err_XML_Data:                      return QObject::tr("Ошибка XML: отсутствует раздел <data>.");
    case Err_XML_ResType:                   return QObject::tr("Ошибка XML: отсутствует параметр <ResultType>.");
    case Err_XML_ServerVer:                 return QObject::tr("Ошибка XML: отсутствует параметр <ServerSoftVer>.");
    case Err_Param_ObjCount_Big:            return QObject::tr("Слишком большое значение параметра ObjCount.");
    case Err_Param_Obj_Not_Found:           return QObject::tr("Отсутствует параметр Obj.");
    case Err_CMD_Format:                    return QObject::tr("Неправильный формат параметра CMD.");
    case Err_BCD_Format:                    return QObject::tr("Некорректное значение в BCD формате.");
    case Err_User_Status:                   return QObject::tr("Неизвестный статус пользователя.");
    case Err_Param_Name_Not_Found:          return QObject::tr("Отсутствует параметр Name.");
    case Err_Record_Not_Exists:             return QObject::tr("Указанная запись отсутствует в БД.");
    case Err_Read_Time_Out:                 return QObject::tr("Тайм-аут ожидания данных.");
    case Err_Pole_ObjType_Format:           return QObject::tr("Неправильный формат поля ObjType.");
    case Err_Pole_AbonentId_Format:         return QObject::tr("Неправильный формат поля AbonentId.");
    case Err_Param_ResCode_Format:          return QObject::tr("Неправильный формат параметра ResCode.");
    case Err_Pole_DeviceId_Format:          return QObject::tr("Неправильный формат поля DeviceId.");
    case Err_Pole_ResCode_Format:           return QObject::tr("Неправильный формат поля ResCode.");
    case Err_Pole_TitleId_Format:           return QObject::tr("Неправильный формат поля TitleId.");
    case Err_Pole_EdId_Format:              return QObject::tr("Неправильный формат поля EdId.");
    case Err_Pole_PrefixNum_Format:         return QObject::tr("Неправильный формат поля PrefixNum.");
    case Err_Pole_PostfixNum_Format:        return QObject::tr("Неправильный формат поля PostfixNum.");
    case Err_Device_Not_Found:              return QObject::tr("Устройство не найдено.");
    case Err_Pole_DevType_Format:           return QObject::tr("Неправильный формат поля DevType.");
    case Err_Pole_Ver_Format:               return QObject::tr("Неправильный формат поля Ver.");
    case Err_Pole_ModelId_Format:           return QObject::tr("Неправильный формат поля ModelId.");
    case Err_Pole_Status_Format:            return QObject::tr("Неправильный формат поля Status.");
    case Err_Pole_PovStat_Format:           return QObject::tr("Неправильный формат поля PovStat.");
    case Err_Pole_PovInterval_Format:       return QObject::tr("Неправильный формат поля PovInterval.");
    case Err_Pole_ManufId_Format:           return QObject::tr("Неправильный формат поля ManufId.");
    case Err_Client_Old_Version:            return QObject::tr("Вы используете устаревшую версию ПО. Пожалуйста, обновите программу.");
    case Err_Soft_Not_Ready:                return QObject::tr("Эта часть программы еще не реализована.");
    case Err_Param_Date_Format:             return QObject::tr("Неправильный формат параметра Date.");
    case Err_Param_Period_Format:           return QObject::tr("Неправильный формат параметра Period.");
    case Err_Pole_Id_Format:                return QObject::tr("Неправильный формат поля Id.");
    case Err_Table_Not_Exist:               return QObject::tr("Не существует таблицы в БД.");
    case Err_Company_Table_Query:           return QObject::tr("Ошибка при выполнении запроса к таблице компаний.");
    case Err_Pole_RegionCode_Format:        return QObject::tr("Неправильный формат поля RegionCode.");
    case Err_Pole_RayonCode_Format:         return QObject::tr("Неправильный формат поля RayonCode.");
    case Err_Pole_CityCode_Format:          return QObject::tr("Неправильный формат поля CityCode.");
    case Err_Pole_PunktCode_Format:         return QObject::tr("Неправильный формат поля PunktCode.");
    case Err_Pole_StreetCode_Format:        return QObject::tr("Неправильный формат поля StreetCode.");
    case Err_Pole_SocrId_Format:            return QObject::tr("Неправильный формат поля SocrId.");
    case Err_Pole_StreetSocrId_Format:      return QObject::tr("Неправильный формат поля StreetSocrId.");
    case Err_Param_UserId_Format:           return QObject::tr("Неправильный формат параметра UserId.");
    case Err_Pole_ArcDate_Format:           return QObject::tr("Неправильный формат поля ArcDate.");
    case Err_Pole_RecDate_Format:           return QObject::tr("Неправильный формат поля RecDate.");
    case Err_Param_EdTeplo_Format:          return QObject::tr("Неправильный формат параметра EdTeplo.");
    case Err_Param_EdElectro_Format:        return QObject::tr("Неправильный формат параметра EdElectro.");
    case Err_Param_EdVoda_Format:           return QObject::tr("Неправильный формат параметра EdVoda.");
    case Err_Param_EdGas_Format:            return QObject::tr("Неправильный формат параметра EdGas.");
    case Err_User_Not_Found:                return QObject::tr("Пользователь отсутствует в БД.");
    case Err_Param_UserType_Format:         return QObject::tr("Неправильный формат параметра UserType.");
    case Err_Param_PasOld_Not_Found:        return QObject::tr("Отсутствует параметр PasOld.");
    case Err_PasOld_Value:                  return QObject::tr("Старый пароль указан неверно.");
    case Err_Param_OdCount_Format:          return QObject::tr("Неправильный формат параметра OdCount.");
    case Err_Param_KvCount_Format:          return QObject::tr("Неправильный формат параметра KvCount.");
    case Err_Param_KvCount_Not_Found:       return QObject::tr("Отсутствует параметр KvCount.");
    case Err_Param_Index_Value:             return QObject::tr("Неверное значение параметра PostIndex.");
    case Err_Param_Sq_Format:               return QObject::tr("Неправильный формат параметра Sq.");
    case Err_Param_Login_Not_Found:         return QObject::tr("Отсутствует параметр Login.");
    case Err_Param_PasData_Not_Found:       return QObject::tr("Отсутствует параметр PasData.");
    case Err_Param_PasData_Format:          return QObject::tr("Неправильный формат параметра PasData.");
    case Err_Login_Exist_In_DB:             return QObject::tr("Запись с таким логином уже существует.");
    case Err_Param_View_Format:             return QObject::tr("Неправильный формат параметра View.");
    case Err_Param_Month_Format:            return QObject::tr("Неправильный формат параметра Month.");
    case Err_Param_DateStart_Format:        return QObject::tr("Неправильный формат параметра DateStart.");
    case Err_Param_DateEnd_Format:          return QObject::tr("Неправильный формат параметра DateEnd.");
    case Err_Pole_DateInstall_Format:       return QObject::tr("Неправильный формат поля DateInstall.");
    case Err_Param_Action_Not_Found:        return QObject::tr("Не найден параметр Action.");
    case Err_Param_Action_Unknown:          return QObject::tr("Неизвестное значение параметра Action.");
    }
    return QObject::tr("Неизвестная ошибка.");
}
/*******************************************************************************************/
bool isValue(const ValueStruct &value, bool *isOneTime)
{
    *isOneTime = false;

    switch (value.titleId) {
    case Title_Energy:              return true;
    case Title_ElectroEnergy:       return true;
    case Title_ReactiveEnergy:      return true;
    case Title_TermalEnergy:        return true;
    case Title_Volume:              return true;
    case Title_Mass:                return true;
    case Title_Power:               *isOneTime = true; return true;
    case Title_VolumeFlowRate:      *isOneTime = true; return true;
    case Title_MassFlowRate:        *isOneTime = true; return true;
    case Title_TempDiff:            *isOneTime = true; return true;
    case Title_TempExternal:        *isOneTime = true; return true;
    case Title_Pressure:            *isOneTime = true; return true;
    case Title_Duration:            return true;
    case Title_DurationAver:        *isOneTime = true; return true;
    case Title_Temp:                *isOneTime = true; return true;
    case Title_CoolantVolume:       return true;
    case Title_TermalPower:         *isOneTime = true; return true;
    case Title_CoolantFlowRate:     *isOneTime = true; return true;
    case Title_TempFlow:            *isOneTime = true; return true;
    case Title_TempReturn:          *isOneTime = true; return true;
    case Title_MassOfDemand:        return true;
    case Title_TempColWater:        *isOneTime = true; return true;
    case Title_TempHotWater:        *isOneTime = true; return true;
    case Title_TempAir:             *isOneTime = true; return true;
    case Title_PeriodNormalWork:    return true;
    case Title_PeriodNotCalc:       return true;
    case Title_VolumeAdditionIn:    return true;
    case Title_ReactiveEnergyRev:   return true;
    case Title_ElectroEnergyRev:    return true;
    case Title_TermalEnergyRev:     return true;
    case Title_EnergyRev:           return true;
    case Title_WaterVolume:         return true;
    case Title_GasVolume:           return true;
    case Title_WaterMass:           return true;
    case Title_PeriodPowerUp:       *isOneTime = true; return true;
    case Title_PressureFlow:        *isOneTime = true; return true;
    case Title_PressureReturn:      *isOneTime = true; return true;
    case Title_WaterMassFlow:       return true;
    case Title_WaterMassReturn:     return true;
    }
    return false;
}
/*******************************************************************************************/
QString getTitleText(quint16 code)
{
    switch (code) {
    case Title_None:                return "";
    case Title_Energy:              return QObject::tr("Энергия");
    case Title_ElectroEnergy:       return QObject::tr("Электроэнергия");
    case Title_ReactiveEnergy:      return QObject::tr("Реактивная энергия");
    case Title_TermalEnergy:        return QObject::tr("Тепловая энергия");
    case Title_Volume:              return QObject::tr("Объем");
    case Title_Mass:                return QObject::tr("Масса");
    case Title_TimeWorking:         return QObject::tr("Время работы");
    case Title_TimeCharging:        return QObject::tr("Время заряда");
    case Title_Power:               return QObject::tr("Мощность");
    case Title_VolumeFlowRate:      return QObject::tr("Объемный расход");
    case Title_MassFlowRate:        return QObject::tr("Массовый расход");
    case Title_TempDiff:            return QObject::tr("Разность температур");
    case Title_TempExternal:        return QObject::tr("Внешняя температура");
    case Title_Pressure:            return QObject::tr("Давление");
    case Title_Duration:            return QObject::tr("Длительность");
    case Title_DurationAver:        return QObject::tr("Средняя длительность");
    case Title_DeviceModification:  return QObject::tr("Модификация устройства");
    case Title_HardwareVer:         return QObject::tr("Версия аппаратной части");
    case Title_SoftwareVer:         return QObject::tr("Версия программной части");
    case Title_State:               return QObject::tr("Состояние");
    case Title_Status:              return QObject::tr("Статус");
    case Title_Version:             return QObject::tr("Версия");
    case Title_DateDevTransmit:     return QObject::tr("Дата, переданная устройством");
    case Title_TimeDevTransmit:     return QObject::tr("Время, переданное устройством");
    case Title_BatChargeLevel:      return QObject::tr("Уровень заряда аккумуляторной батареи");
    case Title_SignalLevel:         return QObject::tr("Уровень сигнала");
    case Title_Temp:                return QObject::tr("Температура");
    case Title_CoolantVolume:       return QObject::tr("Объем теплоносителя");
    case Title_TermalPower:         return QObject::tr("Тепловая мощность");
    case Title_CoolantFlowRate:     return QObject::tr("Расход теплоносителя");
    case Title_TempFlow:            return QObject::tr("Температура на подающем трубопроводе");
    case Title_TempReturn:          return QObject::tr("Температура на обратном трубопроводе");
    case Title_MassOfDemand:        return QObject::tr("Масса водоразбора");
    case Title_TempColWater:        return QObject::tr("Температура холодной воды");
    case Title_TempHotWater:        return QObject::tr("Температура горячей воды");
    case Title_TempAir:             return QObject::tr("Температура воздуха");
    case Title_PeriodNormalWork:    return QObject::tr("Период нормальной работы");
    case Title_PeriodNotCalc:       return QObject::tr("Период отсутствия счета");
    case Title_VolumeAdditionIn:    return QObject::tr("Объем воды по дополнительному входу");
    case Title_ReactiveEnergyRev:   return QObject::tr("Реактивная энергия отпущенная");
    case Title_ElectroEnergyRev:    return QObject::tr("Электроэнергия отпущенная");
    case Title_TermalEnergyRev:     return QObject::tr("Тепловая энергия отпущенная");
    case Title_EnergyRev:           return QObject::tr("Энергия отпущенная");
    case Title_WaterVolume:         return QObject::tr("Объем воды");
    case Title_GasVolume:           return QObject::tr("Объем газа");
    case Title_WaterMass:           return QObject::tr("Масса воды");
    case Title_PeriodPowerUp:       return QObject::tr("Период запуска");
    case Title_UnknownVIF:          return QObject::tr("Неизвестное значение VIB");
    case Title_FirmwareVer:         return QObject::tr("Firmware ver");
    case Title_FirmwareId:          return QObject::tr("Firmware id");
    case Title_PressureFlow:        return QObject::tr("Давление в подающем трубопроводе");
    case Title_PressureReturn:      return QObject::tr("Давление в обратном трубопроводе");
    case Title_WaterMassFlow:       return QObject::tr("Масса воды в подающем трубопроводе");
    case Title_WaterMassReturn:     return QObject::tr("Масса воды в обратном трубопроводе");
    }
    return "";
}
/*******************************************************************************************/
QString getEdText(quint16 code)
{
    switch (code) {
    case Ed_None:          return "";
    case Ed_Wh:            return QObject::tr("Вт.час");
    case Ed_kWh:           return QObject::tr("кВт.час");
    case Ed_VARh:          return QObject::tr("ВАР.час");
    case Ed_kVARh:         return QObject::tr("кВАР.час");
    case Ed_J:             return QObject::tr("Дж");
    case Ed_kJ:            return QObject::tr("кДж");
    case Ed_Kal:           return QObject::tr("кал");
    case Ed_kKal:          return QObject::tr("ккал");
    case Ed_GKal:          return QObject::tr("Гкал");
    case Ed_m3:            return QObject::tr("м.куб");
    case Ed_l:             return QObject::tr("л");
    case Ed_g:             return QObject::tr("г");
    case Ed_kg:            return QObject::tr("кг");
    case Ed_t:             return QObject::tr("т");
    case Ed_sec:           return QObject::tr("сек");
    case Ed_min:           return QObject::tr("мин");
    case Ed_hour:          return QObject::tr("час");
    case Ed_day:           return QObject::tr("дн");
    case Ed_mounth:        return QObject::tr("мес");
    case Ed_year:          return QObject::tr("год");
    case Ed_W:             return QObject::tr("Вт");
    case Ed_Jh:            return QObject::tr("Дж/час");
    case Ed_MKal:          return QObject::tr("Мкал");
    case Ed_MKalh:         return QObject::tr("Мкал/час");
    case Ed_m3h:           return QObject::tr("м.куб/час");
    case Ed_m3m:           return QObject::tr("м.куб/мин");
    case Ed_m3s:           return QObject::tr("м.куб/сек");
    case Ed_kgh:           return QObject::tr("кг/час");
    case Ed_gradC:         return QObject::tr("°С");
    case Ed_gradK:         return QObject::tr("K");
    case Ed_gradF:         return QObject::tr("°F");
    case Ed_bar:           return QObject::tr("бар");
    case Ed_kgcm2:         return QObject::tr("кг/см.кв");
    case Ed_MWh:           return QObject::tr("МВт.час");
    case Ed_MJ:            return QObject::tr("МДж");
    case Ed_GJ:            return QObject::tr("ГДж");
    case Ed_kW:            return QObject::tr("кВт");
    case Ed_MW:            return QObject::tr("МВт");
    case Ed_GW:            return QObject::tr("ГВт");
    case Ed_proc:          return QObject::tr("%");
    case Ed_dBm:           return QObject::tr("дБм");
    case Ed_GWh:           return QObject::tr("ГВт.час");
    }
    return "";
}
/*******************************************************************************************/
QString getDevTypeNameText(quint8 code)
{
    switch (code) {
    case Dev_Other:           return QObject::tr("Другие");
    case Dev_Electro:         return QObject::tr("Электричество (счетчик)");
    case Dev_Gas:             return QObject::tr("Газ (счетчик)");
    case Dev_Teplo:           return QObject::tr("Тепло (счетчик)");
    case Dev_GorVoda:         return QObject::tr("Горячая вода (счетчик)");
    case Dev_Voda:            return QObject::tr("Вода (счетчик)");
    case Dev_HolVoda:         return QObject::tr("Холодная вода (счетчик)");
    case Dev_Pressure_Sensor: return QObject::tr("Датчик давления");
    case Dev_Impulse_Counter: return QObject::tr("Счетчик импульсов");
    case Dev_Electro_Privod:  return QObject::tr("Электричество (привод)");
    case Dev_Gas_Privod:      return QObject::tr("Газ (привод)");
    case Dev_Voda_Privod:     return QObject::tr("Вода (привод)");
    case Dev_Indicator:       return QObject::tr("Индикатор");
    case Dev_Zamok:           return QObject::tr("Электронный замок");
    case Dev_MUC:             return QObject::tr("Концентратор");
    case Dev_RadioModem:      return QObject::tr("Радиомодем");
    case Dev_Convertor:       return QObject::tr("Преобразователь интерфейсов");
    }
    return QObject::tr("Неизвестно");
}
/*******************************************************************************************/
QString getUserTypeText(quint16 code)
{
    switch(code) {
    case User_ROOT:                   return QObject::tr("root");
    case User_BigAdmin:               return QObject::tr("Администратор с расширенным доступом");
    case User_BigOperator:            return QObject::tr("Оператор с расширенным доступом");
    case User_BigUser:                return QObject::tr("Пользователь с расширенным доступом");
    case User_CompanyAdmin:           return QObject::tr("Администратор (организация)");
    case User_CompanyOperator:        return QObject::tr("Оператор (организация)");
    case User_CompanyUser:            return QObject::tr("Пользователь (организация)");    
    }
    return global::intToHex(code, 4, true) + QObject::tr(" - Неизвестный");
}
/*******************************************************************************************/
QString getUserStatusText(quint8 code)
{
    switch (code) {
    case Status_NotActivate:   return QObject::tr("Учетная запись не активирована");
    case Status_Enable:        return QObject::tr("Действующий пользователь");
    case Status_Disable:       return QObject::tr("Учетная запись выключена");
    }
    return global::intToHex(code, 2, true) + QObject::tr(" - Неизвестный");
}
/*******************************************************************************************/
bool userTypeExists(quint16 code)
{
    return (code == User_ROOT) ||
            (code == User_BigAdmin) ||
            (code == User_BigOperator) ||
            (code == User_BigUser) ||
            (code == User_CompanyAdmin) ||
            (code == User_CompanyOperator) ||
            (code == User_CompanyUser);
}
/*******************************************************************************************/
QString getResCodeText(quint16 code)
{
    switch (code) {
    case ResCode_None:           return QObject::tr("Нет");
    case ResCode_Voda:           return QObject::tr("Водоснабжение");
    case ResCode_HolVoda:        return QObject::tr("Водоснабжение (холодная вода)");
    case ResCode_GorVoda:        return QObject::tr("Водоснабжение (горячая вода)");
    case ResCode_Electro:        return QObject::tr("Электроснабжение");
    case ResCode_Gas:            return QObject::tr("Газоснабжение");
    case ResCode_Teplo:          return QObject::tr("Теплоснабжение");
    case ResCode_HolVoda_ForGor: return QObject::tr("Водоснабжение (холодная вода для горячего водоснабжения)");
    }
    return global::intToHex(code, 4, true) + QObject::tr(" - Неизвестно");
}
/*******************************************************************************************/
QString getDevStatusText(quint16 code)
{
    switch (code) {
    case DevStatus_Active:     return QObject::tr("Действующее устройство");
    case DevStatus_NotActive:  return QObject::tr("Недействующее устройство");
    }
    return global::intToHex(code, 4, true) + QObject::tr(" - Неизвестно");
}
/*******************************************************************************************/
QString getObjTypeText(quint16 code)
{
    switch (code) {
    case ObjType_None:           return QObject::tr("Нет");
    case ObjType_OneHouse:       return QObject::tr("Частный дом");
    case ObjType_House:          return QObject::tr("Многоквартирный дом");

    case ObjType_KvInHouse:      return QObject::tr("Жилое помещение");
    case ObjType_TechInHouse:    return QObject::tr("Места общего пользования");
    case ObjType_Office:         return QObject::tr("Нежилое помещение");

    case ObjType_Stroenie:       return QObject::tr("Хозяйственная постройка");
    case ObjType_Kolodets:       return QObject::tr("Смотровой колодец");

    case ObjType_Obshee:         return QObject::tr("Общедомовые");
    }
    return global::intToHex(code, 4, true) + QObject::tr(" - Неизвестно");
}
/*******************************************************************************************/
QString getGermesTypeText(quint8 code)
{
    switch (code) {
    case Germes_G:          return QObject::tr("Germes_G");
    case Germes_E:          return QObject::tr("Germes_E");
    case Germes_D_G:        return QObject::tr("Germes_D.G");
    case Germes_D_E:        return QObject::tr("Germes_D.E");
    }
    return global::intToHex(code, 2, true) + QObject::tr(" - Неизвестно");
}
/*******************************************************************************************/
QString getGermesStatusText(quint8 code)
{
    switch (code) {
    case GermesStatus_PowerOn:           return QObject::tr("POWER_ON");
    case GermesStatus_PowerOn1:          return QObject::tr("POWER_ON1");
    case GermesStatus_PowerOff:          return QObject::tr("POWER_OFF");
    case GermesStatus_RestartRF:         return QObject::tr("RESTART_RF");
    case GermesStatus_CaseOpen:          return QObject::tr("CASE_OPEN");
    case GermesStatus_CaseClosed:        return QObject::tr("CASE_CLOSED");
    case GermesStatus_ExCaseOpen:        return QObject::tr("EX_CASE_OPEN");
    case GermesStatus_ExCaseClosed:      return QObject::tr("EX_CASE_CLOSED");
    case GermesStatus_RestartIrDA:       return QObject::tr("RESTART_IrDA");
    }
    return global::intToHex(code, 2, true) + QObject::tr(" - Неизвестно");
}
/*******************************************************************************************/
QString getShortStrFromDevType(quint8 devType)
{
    switch (devType) {
    case Dev_Electro:
        return "EM";
        break;
    case Dev_HolVoda:
        return "CWM";
        break;
    case Dev_Teplo:
        return "HM";
        break;
    case Dev_GorVoda:
        return "WWM";
        break;
    case Dev_Voda:
        return "WM";
        break;
    case Dev_Gas:
        return "GM";
        break;
    default:
        return QString::number(devType);
        break;
    }
    return QString::number(devType);
}
/*******************************************************************************************/
quint8 getDevTypeFromStr(const QString &typeStr)
{
    if(typeStr == "EM") return Dev_Electro;
    if(typeStr == "CWM") return Dev_HolVoda;
    if(typeStr == "HM") return Dev_Teplo;
    if(typeStr == "WWM") return Dev_GorVoda;
    if(typeStr == "WM") return Dev_Voda;
    if(typeStr == "GM") return Dev_Gas;
    return global::strToIntDef( typeStr, 0xFF);
}

/*******************************************************************************************
 * CRC: Cyclic redundancy check
 * The CRC polynomial is: x16 + x13 + x12 + x11 + x10 + x8 + x6 + x5 + x2 + 1
 * The initial value is: 0
 * The final CRC is complemented
 ******************************************************************************************/
const unsigned short wmPoly = 0x3D65;

quint16 crc16(const ByteVector &data, int startIndex, int len)
{
    int endIndex = (len == 0) ? data.size()-1 : startIndex + len - 1 ;
    if(endIndex > data.size()-1) endIndex = data.size()-1;

    quint16 result = 0x0000;

    for(int i=startIndex; i<=endIndex; i++) {
        result = result ^ ( (static_cast<quint16>(data.at(i)) & 0x00FF) << 8);
        for (int n = 0; n < 8; n++)
            result = result & 0x8000 ? (result << 1) ^ wmPoly : result << 1;
    }
    return result ^ 0xFFFF;
}

quint16 crc16(const QByteArray &data, int startIndex, int len)
{
    int endIndex = (len == 0) ? data.size()-1 : startIndex + len - 1 ;
    if(endIndex > data.size()-1) endIndex = data.size()-1;

    quint16 result = 0x0000;

    for(int i=startIndex; i<=endIndex; i++) {
        result = result ^ ( (static_cast<quint16>(data.at(i)) & 0x00FF) << 8);
        for (int n = 0; n < 8; n++)
            result = result & 0x8000 ? (result << 1) ^ wmPoly : result << 1;
    }
    return result ^ 0xFFFF;
}

unsigned short crc16(unsigned char * pcBlock, int len)
{
    unsigned short crc = 0x0000;
    while (len--) {
        crc = crc ^ ((unsigned short)*pcBlock++  << 8);
        for (int n = 0; n < 8; n++)
            crc = crc & 0x8000 ? (crc << 1) ^ wmPoly : crc << 1;
    }
    return crc ^ 0xFFFF;
}
/*******************************************************************************************/
QString wordToManId(quint16 val)
{
    QString res = "???";
    res[0] = QChar(64 +  global::lo( static_cast<quint16>( (val & 0x7C00) >> 10)));
    res[1] = QChar(64 +  global::lo( static_cast<quint16>((val & 0x03E0) >> 5)));
    res[2] = QChar(64 +  global::lo( static_cast<quint16>( val & 0x001F) ));
    return res;
}
/*******************************************************************************************/
quint16 manIdToWord(const QString &manId)
{
    if(manId.length() != 3) return 0;
    QByteArray a = manId.toLocal8Bit();
    return ( static_cast<quint16>(a.at(0) - 64 ) << 10) |
    ( static_cast<quint16>(a.at(1) - 64 ) <<  5 ) |
    static_cast<quint16>(a.at(2) - 64 );
}
/*******************************************************************************************/
bool itsCorrectManId(const QString &manId)
{
    if(manId.length() != 3) return false;
    QByteArray a = manId.toLocal8Bit();
    for(int i=0; i<3; i++) if((a.at(i) < 65) || (a.at(i) > 90)) return false;
    return true;
}
/*******************************************************************************************/
bool itsCorrectManId(quint16 manId)
{
    return itsCorrectManId( wordToManId(manId) );
}
/*******************************************************************************************/
bool itsCorrectIdentNum(const QString &identNum)
{
    bool ok = false;
    int base = ((identNum.length()>2) && (identNum.left(2).toLower()=="0x")) ? 16 : 10;
    long num = identNum.toLong(&ok, base);
    return (ok && (num>=0) && (num <= 99999999));
}
/*******************************************************************************************/
bool itsCorrectIdentNum(quint32 identNum)
{
    int n;
    for(int i=0; i<4; i++) {
        n =  ((identNum >> (i*8)) & 0x0F);
        if ((n<0) || (n>9)) return false;
        n =  ((identNum >> (i*8)) & 0xF0) >> 4;
        if ((n<0) || (n>9)) return false;
    }
    return true;
}
/*******************************************************************************************/
QString getIdentNumText(QVariant idNum)
{
    bool ok;
    quint32 val = static_cast<quint32>(idNum.toInt(&ok));
    if(!ok) return "";

    QString res = QString::number( val );
    while (res.length() < 8) res = "0" + res;
    return res;
}
/*******************************************************************************************/
QString delPrevNulls(const QString &val)
{
    QString dataStr = val.trimmed(), s = "";
    if (dataStr.length()==0) return "0";

    if (dataStr.at(0) == '-') {
        s = "-";
        dataStr.remove(0,1);
        if (dataStr.length()==0) return "0";
    }

    while ((dataStr.length()>1) && (dataStr.at(0)=='0')) {
        dataStr.remove(0,1);
    }
    if (dataStr.length()==0) dataStr = "0";
    if (dataStr.at(0)=='.') dataStr = "0"+dataStr;

    return s+dataStr;
}
/*******************************************************************************************/
QString delPostNulls(const QString &val, int precise)
{
    QString dataStr = val.trimmed(), s = "";
    if (dataStr.length()==0) return "0";

    if (dataStr.at(0) == '-') {
        s = "-";
        dataStr.remove(0,1);
        if (dataStr.length()==0) return "0";
    }

    while ((dataStr.length()>1) && (dataStr.at( dataStr.length()-1 )=='0')) {
        dataStr.remove(dataStr.length()-1,1);
    }
    if (dataStr.length()==0) dataStr = "0";
    if (dataStr.at(dataStr.length()-1)=='.') dataStr+="0";

    if(precise > 1) {

        int n = dataStr.indexOf('.');
        if(n>=0) {
            while( ( dataStr.length()-n-1 ) < precise ) dataStr += "0";
        }
    }

    return s+dataStr;
}
/*******************************************************************************************/
QString calcStepen(int stepen, const QString &strVal)
{
    int n = strVal.indexOf(".");
    QString s1=strVal, s2="", s="";

    if (n>=0) {
        s1 = strVal.left( n );
        s2 = strVal.right( strVal.length() - n - 1 );
    }
    if (s1.length()==0) return "0";

    if (s1.at(0) == '-') {
        s = "-";
        s1.remove(0,1);
        if (s1.length()==0) return "0";
    }

    if (stepen>0) {
        for (int i=0; i<stepen; i++) {
            if (s2.length()>0) {
                s1+=s2.left(1);
                s2.remove(0,1);
            }
            else s1+="0";
        }
    }
    else {
        for(int i=0; i < -stepen; i++) {
            if (s1.length()>0) {
                s2=s1.right(1)+s2;
                s1.remove(s1.length()-1,1);
            }
            else s2="0"+s2;
        }
    }

    if (s1.length()==0) s1 = "0";
    s1=s+s1;

    if (s2.length()==0) return s1;
    else return s1+"."+s2;
}
/*******************************************************************************************/
QString getIdText(quint64 value)
{
    return wordToManId(  static_cast<quint16>( (value >> 48) & 0xFFFF ))+" "+
            global::intToHex( static_cast<quint32>((value >> 16) & 0xFFFFFFFF), 8)+" "+
            global::intToHex( static_cast<quint8>((value >> 8) & 0xFF), 2) + " "+
            global::intToHex( static_cast<quint8>(value & 0xFF), 2);
}
/*******************************************************************************************/
QString md5(const QString &data)
{
    return QString::fromLocal8Bit(
                QCryptographicHash::hash( data.toLocal8Bit(), QCryptographicHash::Md5 ).toHex()
           ).toLower();
}

/*******************************************************************************************/
QString correctingBCD(const QString &val, bool *ok /*= Q_NULLPTR*/)
{    /* корректировка строки BCD фомата */

    if (val.isNull() || val.isEmpty()) {
        if(ok) *ok = false;
        return val;
    }

    QString res = "";
    int cnt = val.length();
    if(val.left(1).toUpper() == "F") {
        res = "-";
        cnt--;
    }

    bool resOK;
    val.right(cnt).toLongLong(&resOK, 10);
    if(ok) *ok = resOK;
    return res + val.right(cnt);
}
/*******************************************************************************************/
bool getTarifEnergyRec(quint8 devType, const ByteVector &dif, ValueStruct &value)
{
    switch (devType) {
    case Dev_Electro: value.titleId = Title_ElectroEnergy; break;
    case Dev_Teplo:  value.titleId = Title_TermalEnergy; break;
    default: value.titleId = Title_Energy; break;
    }

    if( (devType == Dev_Teplo) && (dif.size()==2) && (dif.last() == 0x00) ) {
        value.prefixNum = 1; // номер ввода
    }
    else if( (devType == Dev_Teplo) && (dif.size()==2) && (dif.last() == 0x20) ) {
        value.prefixNum = 1; // номер ввода
        value.postfixNum = 3; // номер трубы
    }
    else if( (devType == Dev_Teplo) && (dif.size()==2) && (dif.last() == 0x40) ) {
        value.prefixNum = 2; // номер ввода
    }
    else if( (devType == Dev_Teplo) && (dif.size()==2) && (dif.last() == 0x60) ) {
        value.prefixNum = 2; // номер ввода
        value.postfixNum = 3; // номер трубы
    }
    if( (devType == Dev_Electro) && (dif.size()==2) &&
            ( (dif.last() == 0x10) || (dif.last() == 0x20) || (dif.last() == 0x30) ) ) {
        value.postfixNum = dif.last() >> 4; // - номер тарифа
    }
    else if( (devType == Dev_Electro) && (dif.size()==2) &&
             ( (dif.last() == 0x50) || (dif.last() == 0x60) || (dif.last() == 0x70) ) ) {
        value.postfixNum = (dif.last() & 0xBF) >> 4; // - номер тарифа
        value.titleId = Title_ElectroEnergyRev;
    }
    else if((devType == Dev_Electro) && (dif.size()==3) &&
            ( (dif.last() == 0x10) || (dif.last() == 0x20) || (dif.last() == 0x30) ) ) {
        value.postfixNum = 3 + ( dif.last() >> 4 ); // - номер тарифа
        if ((dif.at(1) & 0x40) == 0x40) value.titleId = Title_ElectroEnergyRev;
    }
    return true;
}
/*******************************************************************************************/
bool getTarifReactiveEnergyRec(const ByteVector &dif, ValueStruct &value)
{
    value.titleId = Title_ReactiveEnergy;

    if( (dif.size()==2) && ( (dif.last() == 0x10) || (dif.last() == 0x20) || (dif.last() == 0x30) ) ) {
        value.postfixNum = dif.last() >> 4; // - номер тарифа
        value.titleId = Title_ReactiveEnergy;
    }
    else if( (dif.size()==2) && ( (dif.last() == 0x50) || (dif.last() == 0x60) || (dif.last() == 0x70) ) ) {
        value.postfixNum = (dif.last() & 0xBF) >> 4; // - номер тарифа
        value.titleId = Title_ReactiveEnergyRev;
    }
    else if( (dif.size()==3) && ( (dif.last() == 0x10) || (dif.last() == 0x20) || (dif.last() == 0x30) ) ) {
        value.postfixNum = 3 + ( dif.last() >> 4 ); // - номер тарифа
        if ((dif.at(1) & 0x40) == 0x40) value.titleId = Title_ReactiveEnergyRev;
    }
    return true;
}
/*******************************************************************************************/
bool getTarifVolumeRec(quint8 devType, const ByteVector &dif, ValueStruct &value)
{
    switch (devType) {
    case Dev_Gas:  value.titleId = Title_GasVolume; break;
    case Dev_Teplo:  value.titleId = Title_CoolantVolume; break;
    case Dev_GorVoda:
    case Dev_Voda:
    case Dev_HolVoda:  value.titleId = Title_WaterVolume; break;
    default: value.titleId = Title_Volume; break;
    }

    if( (dif.size()==2) && ( (dif.last() == 0x00) || (dif.last() == 0x10) || (dif.last() == 0x20) ) ) {
        value.prefixNum = 1; // номер ввода
        value.postfixNum = 1 + (dif.last() >> 4); // - номер трубопровода
    }
    else if( (dif.size()==2) && ( (dif.last() == 0x40) || (dif.last() == 0x50) || (dif.last() == 0x60) ) ) {
        value.prefixNum = 2; // номер ввода
        value.postfixNum = ((dif.last() & 0xBF) >> 4) - 3; // - номер трубопровода
    }
    else if( (dif.size()==2) && (dif.last() == 0x30) ) {
        value.titleId = Title_VolumeAdditionIn;
    }
    return true;
}
/*******************************************************************************************/
bool getTarifMassRec(quint8 devType, const ByteVector &dif, ValueStruct &value)
{
    switch (devType) {
    case Dev_Teplo:
    case Dev_GorVoda:
    case Dev_Voda:
    case Dev_HolVoda:  value.titleId = Title_WaterMass; break;
    default: value.titleId = Title_Mass; break;
    }

    if( (dif.size()==2) && ( (dif.last() == 0x00) || (dif.last() == 0x10) || (dif.last() == 0x20) ) ) {
        value.prefixNum = 1; // номер ввода
        value.postfixNum = 1 + (dif.last() >> 4); // - номер трубопровода
    }
    else if( (dif.size()==2) && ( (dif.last() == 0x40) || (dif.last() == 0x50) || (dif.last() == 0x60) ) ) {
        value.prefixNum = 2; // номер ввода
        value.postfixNum = ((dif.last() & 0xBF) >> 4) - 3; // - номер трубопровода
    }
    else if( (dif.size()==2) && (dif.last() == 0x30) ) {
        value.prefixNum = 1; // номер ввода
        value.titleId = Title_MassOfDemand;
    }
    else if( (dif.size()==2) && (dif.last() == 0x70) ) {
        value.prefixNum = 2; // номер ввода
        value.titleId = Title_MassOfDemand;
    }
    return true;
}
/*******************************************************************************************/
bool getTarifFlowTempRec(const ByteVector &dif, ValueStruct &value)
{
    value.titleId = Title_TempFlow;

    if( (dif.size()==2) && ( (dif.last() == 0x00) || (dif.last() == 0x10) || (dif.last() == 0x20) ) ) {
        value.prefixNum = 1; // номер ввода
        value.postfixNum = 1 + (dif.last() >> 4); // - номер трубопровода
        value.titleId = Title_Temp;
    }
    else if( (dif.size()==2) && ( (dif.last() == 0x40) || (dif.last() == 0x50) || (dif.last() == 0x60) ) ) {
        value.prefixNum = 2; // номер ввода
        value.postfixNum = ((dif.last() & 0xBF) >> 4) - 3; // - номер трубопровода
        value.titleId = Title_Temp;
    }
    return true;
}
/*******************************************************************************************/
bool getTarifReturnTempRec(ValueStruct &value)
{
    value.titleId = Title_TempReturn;
    return true;
}
/*******************************************************************************************/
bool getTarifPressureRec(const ByteVector &dif, ValueStruct &value)
{
    value.titleId = Title_Pressure;

    if( (dif.size()==2) && ( (dif.last() == 0x00) || (dif.last() == 0x10) || (dif.last() == 0x20) ) ) {
        value.prefixNum = 1; // номер ввода
        value.postfixNum = 1 + (dif.last() >> 4); // - номер трубопровода
    }
    else if( (dif.size()==2) && ( (dif.last() == 0x40) || (dif.last() == 0x50) || (dif.last() == 0x60) ) ) {
        value.prefixNum = 2; // номер ввода
        value.postfixNum = ((dif.last() & 0xBF) >> 4) - 3; // - номер трубопровода
    }
    return true;
}
/*******************************************************************************************/
bool getTarifTempDiffRec(const ByteVector &dif, ValueStruct &value)
{
    value.titleId = Title_TempDiff;

    if( (dif.size()==2) && (dif.last() == 0x20) ) {
        value.prefixNum = 1; // номер ввода
    }
    else if( (dif.size()==2) && (dif.last() == 0x40) ) {
        value.prefixNum = 2; // номер ввода
    }
    return true;
}
/*******************************************************************************************/
bool getTarifTempExternalRec(const ByteVector &dif, ValueStruct &value)
{
    value.titleId = Title_TempExternal;

    if( (dif.size()==2) && (dif.last() == 0x00) ) {
        value.prefixNum = 1; // номер ввода
        value.titleId = Title_TempColWater;
    }
    else if( (dif.size()==2) && (dif.last() == 0x10) ) {
        value.prefixNum = 1; // номер ввода
        value.titleId = Title_TempAir;
    }
    else if( (dif.size()==2) && (dif.last() == 0x40) ) {
        value.prefixNum = 2; // номер ввода
        value.titleId = Title_TempColWater;
    }
    else if( (dif.size()==2) && (dif.last() == 0x50) ) {
        value.prefixNum = 2; // номер ввода
        value.titleId = Title_TempAir;
    }
    return true;
}
/*******************************************************************************************/
bool getTarifDurationRec(const ByteVector &dif, ValueStruct &value)
{
    Q_UNUSED(dif);
    value.titleId = Title_Duration;

    return true;
}
/*******************************************************************************************/
bool getTarifDurationAverRec(ValueStruct &value)
{
    value.titleId = Title_DurationAver;

    return true;
}
/*******************************************************************************************/
bool getTarifVolumeFlowRateRec(const ByteVector &dif, ValueStruct &value)
{
    value.titleId = Title_CoolantFlowRate;

    if( (dif.size()==2) && ( (dif.last() == 0x00) || (dif.last() == 0x10) || (dif.last() == 0x20) ) ) {
        value.prefixNum = 1; // номер ввода
        value.postfixNum = 1 + (dif.last() >> 4); // - номер трубопровода
        value.titleId = Title_VolumeFlowRate;
    }
    else if( (dif.size()==2) && ( (dif.last() == 0x40) || (dif.last() == 0x50) || (dif.last() == 0x60) ) ) {
        value.prefixNum = 2; // номер ввода
        value.postfixNum = ((dif.last() & 0xBF) >> 4) - 3; // - номер трубопровода
        value.titleId = Title_VolumeFlowRate;
    }
    return true;
}
/*******************************************************************************************/
bool getTarifPowerRec(quint8 devType, ValueStruct &value)
{
    switch (devType) {
    case Dev_Teplo: value.titleId = Title_TermalPower; break;
    default: value.titleId = Title_Power; break;
    }
    return true;
}
/*******************************************************************************************/
bool getTarifMassFlowRateRec(ValueStruct &value)
{
    value.titleId = Title_MassFlowRate;
    return true;
}
/*******************************************************************************************/
bool getTarifTimeWorkingRec(ValueStruct &value)
{
    value.titleId = Title_TimeWorking;
    return true;
}
/*******************************************************************************************/
bool getTarifPeriodWorkingRec(quint8 devType, const ByteVector &dif, ValueStruct &value)
{
    Q_UNUSED(dif);
    Q_UNUSED(devType);
    value.titleId = Title_PeriodNormalWork;
    /*
    if( (dif.size()==2) && (dif.last() == 0x00) ) {
        value.prefixNum = 1; // номер ввода
        value.titleId = Title_PeriodNormalWork;
    }
    else if( (dif.size()==2) && (dif.last() == 0x10) ) {
        value.prefixNum = 1; // номер ввода
        value.titleId = Title_PeriodNotCalc;
    }
    else if( (dif.size()==2) && (dif.last() == 0x40) ) {
        value.prefixNum = 2; // номер ввода
        value.titleId = Title_PeriodNormalWork;
    }
    else if( (dif.size()==2) && (dif.last() == 0x50) ) {
        value.prefixNum = 2; // номер ввода
        value.titleId = Title_PeriodNotCalc;
    }
    */
    return true;
}
/*******************************************************************************************/
bool getTarifPeriodNotCalcRec(quint8 devType, const ByteVector &dif, ValueStruct &value)
{
    Q_UNUSED(dif);
    Q_UNUSED(devType);
    value.titleId = Title_PeriodNotCalc;
    /*
    if( (dif.size()==2) && (dif.last() == 0x00) ) {
        value.prefixNum = 1; // номер ввода
        value.titleId = Title_PeriodNormalWork;
    }
    else if( (dif.size()==2) && (dif.last() == 0x10) ) {
        value.prefixNum = 1; // номер ввода
        value.titleId = Title_PeriodNotCalc;
    }
    else if( (dif.size()==2) && (dif.last() == 0x40) ) {
        value.prefixNum = 2; // номер ввода
        value.titleId = Title_PeriodNormalWork;
    }
    else if( (dif.size()==2) && (dif.last() == 0x50) ) {
        value.prefixNum = 2; // номер ввода
        value.titleId = Title_PeriodNotCalc;
    }
    */
    return true;
}
/*******************************************************************************************/
bool getTarifPeriodPowerUpRec(ValueStruct &value)
{
    value.titleId = Title_PeriodPowerUp;
    return true;
}
/*******************************************************************************************/
bool getTarifTimeChargingRec(ValueStruct &value)
{
    value.titleId = Title_TimeCharging;
    return true;
}
/*******************************************************************************************/
QString getErrorsFlagsText(const WMBusDevStruct &device, quint32 errorCode, int prefix, int postfix)
{
    QString res = "", prefixStr, postfixStr;
    int n;

    prefixStr = (prefix>0) ? QObject::tr("Канал ") +QString::number(prefix) + ": " : "";
    postfixStr = (postfix>0) ? " (" +QString::number(postfix) + ")" : "";

    switch (device.devType) {
    case Dev_Electro:

        if((errorCode & 0x00000100) == 0x00000100) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Различный ток нагрузки по нулевому и фазному проводам") + postfixStr;
        }
        if((errorCode & 0x00000200) == 0x00000200) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Отрицательная потребляемая мощность") + postfixStr;
        }
        n = ((errorCode & 0x00000C00) >> 10);
        if(n == 1) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Ток нагрузки предельно мал") + postfixStr;
        }
        else if(n == 2) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Ток нагрузки превышает максимальный") + postfixStr;
        }
        n = ((errorCode & 0x00003000) >> 12);
        if(n == 1) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Напряжение ниже минимального") + postfixStr;
        }
        else if(n == 2) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Напряжение выше максимального") + postfixStr;
        }        
        if((errorCode & 0x00004000) == 0x00004000) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Отсутствует ток нагрузки") + postfixStr;
        }


        if((errorCode & 0x00000001) == 0x00000001) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Ранее токи нагрузки по нулевому и фазному проводам были различны") + postfixStr;
        }
        if((errorCode & 0x00000002) == 0x00000002) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Ранее была отрицательная потребляемая мощность") + postfixStr;
        }
        if((errorCode & 0x00000004) == 0x00000004) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Ранее было воздействие внешнего магнитного поля") + postfixStr;
        }
        if((errorCode & 0x00000008) == 0x00000008) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Ранее была открыта крышка клеммной коробки") + postfixStr;
        }
        if((errorCode & 0x00000010) == 0x00000010) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Ранее был вскрыт корпус") + postfixStr;
        }
        if((errorCode & 0x00000020) == 0x00000020) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Открыт корпус") + postfixStr;
        }
        if((errorCode & 0x00000040) == 0x00000040) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Воздействие внешнего магнитного поля") + postfixStr;
        }
        if((errorCode & 0x00000080) == 0x00000080) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Открыта крышка клеммной коробки") + postfixStr;
        }
        break;
    case Dev_Gas:

        break;
    case Dev_Teplo:
       n = (errorCode & 0x0000000F);

       if(n==1) {
           if(!res.isEmpty()) res += "\n";
           res += prefixStr + QObject::tr("Обратное вращение вертушки") + postfixStr;
       }
       else if(n==2) {
           if(!res.isEmpty()) res += "\n";
           res += prefixStr + QObject::tr("Вертушка не вращается") + postfixStr;
       }
       else if(n==3) {
           if(!res.isEmpty()) res += "\n";
           res += prefixStr + QObject::tr("Расход меньше минимального") + postfixStr;
       }
       else if(n==4) {
           if(!res.isEmpty()) res += "\n";
           res += prefixStr + QObject::tr("Расход больше максимального") + postfixStr;
       }

       n = ((errorCode >> 4) & 0x0000000F);

       if(n==1) {
           if(!res.isEmpty()) res += "\n";
           res += prefixStr + QObject::tr("Разность температуры отрицательна") + postfixStr;
       }
       else if(n==2) {
           if(!res.isEmpty()) res += "\n";
           res += prefixStr + QObject::tr("Разность температуры меньше 3 °С") + postfixStr;
       }
       else if(n==3) {
           if(!res.isEmpty()) res += "\n";
           res += prefixStr + QObject::tr("Разность температуры больше 145 °С") + postfixStr;
       }
       else if(n==4) {
           if(!res.isEmpty()) res += "\n";
           res += prefixStr + QObject::tr("Ошибка расчета разности температур из-за ошибки измерения одной из температур") + postfixStr;
       }
       else if(n==5) {
           if(!res.isEmpty()) res += "\n";
           res += prefixStr + QObject::tr("Разность температуры меньше -5 °С") + postfixStr;
       }
       else if(n==6) {
           if(!res.isEmpty()) res += "\n";
           res += prefixStr + QObject::tr("Разность температуры меньше 0.5 °С") + postfixStr;
       }

       n = ((errorCode >> 8) & 0x0000000F);

       if(n==1) {
           if(!res.isEmpty()) res += "\n";
           res += prefixStr + QObject::tr("Температура датчика обратного трубопровода меньше минимальной") + postfixStr;
       }
       else if(n==2) {
           if(!res.isEmpty()) res += "\n";
           res += prefixStr + QObject::tr("Температура датчика обратного трубопровода больше максимальной") + postfixStr;
       }
       else if(n==3) {
           if(!res.isEmpty()) res += "\n";
           res += prefixStr + QObject::tr("Обрыв токовой цепи датчика обратного трубопровода") + postfixStr;
       }
       else if(n==4) {
           if(!res.isEmpty()) res += "\n";
           res += prefixStr + QObject::tr("Обрыв потенциальной цепи датчика обратного трубопровода") + postfixStr;
       }
       else if(n==5) {
           if(!res.isEmpty()) res += "\n";
           res += prefixStr + QObject::tr("Короткое замыкание потенциальной цепи датчика обратного трубопровода") + postfixStr;
       }

        n = ((errorCode >> 12) & 0x0000000F);
        if(n==1) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Температура датчика подающего трубопровода меньше минимальной") + postfixStr;
        }
        else if(n==2) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Температура датчика подающего трубопровода больше максимальной") + postfixStr;
        }
        else if(n==3) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Обрыв токовой цепи датчика подающего трубопровода") + postfixStr;
        }
        else if(n==4) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Обрыв потенциальной цепи датчика подающего трубопровода") + postfixStr;
        }
        else if(n==5) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Короткое замыкание потенциальной цепи датчика подающего трубопровода") + postfixStr;
        }

        break;
    case Dev_GorVoda:
    case Dev_Voda:
    case Dev_HolVoda:
        if((errorCode & 0x00000001) == 0x00000001) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Воздействие внешнего магнитного поля") + postfixStr;
        }
        break;
    case Dev_Impulse_Counter:
        if((errorCode & 0x00000001) == 0x00000001) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Воздействие внешнего магнитного поля") + postfixStr;
        }
        if((errorCode & 0x00000002) == 0x00000002) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Обрыв линии") + postfixStr;
        }
        if((errorCode & 0x00000004) == 0x00000004) {
            if(!res.isEmpty()) res += "\n";
            res += prefixStr + QObject::tr("Короткое замыкание линии") + postfixStr;
        }
        break;
    default:
        if (errorCode > 0) return global::intToHex(errorCode, 8, true);
        break;
    }

    return res;
}
/*******************************************************************************************/
QString getValueStr(quint8 devType, const ValueStruct &value)
{
    QString strTmp = wmbus::getTitleText(value.titleId);
    if(value.prefixNum > 0) strTmp = "("+QString::number(value.prefixNum, 10) + ") "+strTmp;
    if(value.postfixNum > 0) {
        switch (devType) {
        case Dev_Electro: strTmp += QObject::tr(" (тариф ") + QString::number(value.postfixNum) + ")"; break;
        case Dev_Teplo: strTmp += QObject::tr(" (трубопровод ") + QString::number(value.postfixNum) + ")"; break;
        default: strTmp += QObject::tr(" (") + QString::number(value.postfixNum) + ")"; break;
        }
    }
    strTmp = strTmp.trimmed();
    if(!strTmp.isEmpty()) strTmp += ": ";
    strTmp += value.val + " " + wmbus::getEdText(value.edId);

    return strTmp.trimmed();
}
/*******************************************************************************************/
// Сумма тарифов в общий для некоторых устройств, если нет в показаниях
TErrorCode calcSummFromTarifs(const WMBusDevStruct &device, QVector<ValueStruct> &vecVal, QString &logStr)
{
    if(vecVal.isEmpty()) return Err_OK;

    for(int i=0; i<vecVal.size(); i++) {
        if(!vecVal.at(i).val.isEmpty() && (vecVal.at(i).val.left(1) == "-")) {

            if( (vecVal.at(i).titleId == Title_ElectroEnergy) || (vecVal.at(i).titleId == Title_Energy) ||
                    (vecVal.at(i).titleId == Title_TermalEnergy) || (vecVal.at(i).titleId == Title_Volume) ||
                    (vecVal.at(i).titleId == Title_WaterVolume) || (vecVal.at(i).titleId == Title_GasVolume) ||
                    (vecVal.at(i).titleId == Title_WaterMass) || (vecVal.at(i).titleId == Title_Mass) ) {
                return Err_Value;
            }
        }
    }

    if(device.devType != Dev_Electro) return Err_OK;

    bool ok = false;
    ValueStruct value;

    for(int i=0; i<vecVal.size(); i++) {
        value = vecVal.at(i);

        if((value.titleId == Title_ElectroEnergy) || (value.titleId == Title_Energy)) {
            if((value.postfixNum==0)) return Err_OK;
            else ok = true;
        }
    }
    if(!ok) return Err_OK;

    logStr += QObject::tr("   Сумма показаний по тарифам...\n");

    double sumVal = 0.0;
    quint16 edId = Ed_None;
    int precise = 2;

    for(int i=0; i<vecVal.size(); i++) {

        value = vecVal.at(i);
        if((value.titleId == Title_ElectroEnergy) || (value.titleId == Title_Energy)) {

            if(edId == Ed_None) edId = value.edId;

            if (edId != Ed_None) {
                if(!calcValueToEdId(edId, value)) return Err_Numeric_Data_Format;
            }

            int prec = global::getMaxPrecise( value.val, value.val );
            if(prec > precise) precise = prec;

            double tmpVal = global::strToDouble( value.val, &ok);
            if(!ok) return Err_Numeric_Data_Format;

            sumVal += tmpVal;
        }
    }

    value.edId = edId;
    value.postfixNum = 0;
    value.prefixNum = 0;
    value.titleId = Title_ElectroEnergy;
    value.val = global::doubleToStr(sumVal, precise);
    vecVal.insert(0, value);

    logStr += "   " + getValueStr(device.devType, value) + "\n";

    return Err_OK;
}
/*******************************************************************************************/
// Разбор пакета WMBus:
TErrorCode readValuesFromArray(const QByteArray &data, const WMBusDevStruct &device,
                                QDateTime &dt, QVector<ValueStruct> &vecVal, QVector<FlagStruct> &vecFlag,
                               QString &logStr, QVector<ValueStruct> &vecTech)
{
    if(data.count()<3) return Err_Data_End;

    ByteVector dataVec;
    for(int i=0; i<data.size(); i++) dataVec.append( static_cast<quint8>(data.at(i)) );

    logStr = "";

    TErrorCode res = readValuesFromByteVector( dataVec, device, dt, vecVal, vecFlag, logStr, vecTech );

    if(res != Err_OK) return res;
    return calcSummFromTarifs(device, vecVal, logStr);
}
/*******************************************************************************************/
TErrorCode readValuesFromByteVector(const ByteVector &data, const WMBusDevStruct &device,
                               QDateTime &dt, QVector<ValueStruct> &vecVal, QVector<FlagStruct> &vecFlag, QString &logStr,
                               QVector<ValueStruct> &vecTech)
{
    if(data.count()<3) return Err_Data_End;

    ByteVector vif, dif;
    int indx = 0, valSize = 0;
    ValueStruct value;
    FlagStruct flag;
    QString valStr, valStrUnsign, strTmp;
    bool ok;
    global::SingleUnion intToFloat;
    global::ShortUnion union16;
    global::ByteUnion  union8;
    global::SingleUnion union32;
    global::DoubleUnion union64;

    quint64 ui64;
    quint32 ui32;
    quint16 ui16;

    dt = QDateTime::fromMSecsSinceEpoch(-1);

    while(indx < data.size()) {
        valStr = "";
        valStrUnsign = "";
        valSize = 0;
        vif.clear();
        dif.clear();

        // DIF:
        dif.append( data.at(indx++) );
        if(dif.last() == 0xFF) break;
        if(indx >= data.count()) return Err_Data_End;

        // DIFE:
        while((dif.last() & 0x80) == 0x80) {
            if(dif.size() > 11) return Err_DIFE_Count;
            dif.append( data.at(indx++) );
            if((dif.last() == 0xFF) || (indx >= data.size())) return Err_Data_End;
        }

        strTmp = "";
        for(int i=0; i<dif.size(); i++) strTmp += global::intToHex(dif.at(i), 2, true) + " ";
        logStr += "   DIB: " + strTmp;

        // VIF:
        vif.append( data.at(indx++) );
        if ((vif.last() == 0x00) || (indx >= data.size())) return Err_Data_End;

        // VIFE:
        while((vif.last() & 0x80) == 0x80) {
            if(vif.size() > 11) return Err_VIFE_Count;
            vif.append( data.at(indx++) );
            if((vif.last() == 0x00) || (indx >= data.size())) return Err_Data_End;
        }

        strTmp = "";
        for(int i=0; i<vif.size(); i++) strTmp += global::intToHex(vif.at(i), 2, true) + " ";
        logStr += "  VIB: " + strTmp;

        // Value: (тип и размер данных)
        logStr += "  VAL: ";

        switch (dif.first() & 0x0F) {
        case DIF_NO_DATA: // нет данных?

            break;
        case DIF_INT_8: // знаковое целое 1 байт
            if(indx >= data.size()) return Err_Data_End;
            valSize = 8;
            union8.val_uint = data.at(indx++);
            logStr += global::intToHex( union8.val_uint, 2, true );
            valStrUnsign = QString::number( union8.val_uint, 10 );
            valStr = QString::number( union8.val_int, 10 );
            break;
        case DIF_INT_16: // знаковое целое 2 байта
            if((indx+1) >= data.size()) {
                logStr += "\n";
                return Err_Data_End;
            }
            valSize = 16;
            union16.val_uint = (static_cast<quint16>(data.at(indx)) & 0x00FF) |
                    ((static_cast<quint16>(data.at(indx+1)) & 0x00FF) << 8);
            logStr += global::intToHex(union16.val_uint, 4, true);
            valStrUnsign = QString::number( union16.val_uint, 10 );
            valStr = QString::number(union16.val_int, 10 );
            indx += 2;
            break;
        case DIF_INT_24: // знаковое целое 3 байта
            if((indx+2) >= data.size()) {
                logStr += "\n";
                return Err_Data_End;
            }
            valSize = 24;
            ui32 = (static_cast<quint32>(data.at(indx)) & 0x000000FF) |
                    ((static_cast<quint32>(data.at(indx+1)) & 0x000000FF) << 8) |
                    ((static_cast<quint32>(data.at(indx+2)) & 0x000000FF) << 16);
            logStr += global::intToHex(ui32,6,true);
            valStrUnsign = QString::number(ui32, 10 );
            if ((ui32 & 0x800000) == 0x800000) {
                valStr = "-" + QString::number( ~(ui32 & 0x7FFFFF) , 10 );
            }
            else {
                valStr = valStrUnsign;
            }
            indx += 3;
            break;
        case DIF_INT_32: // знаковое целое 4 байта
            if((indx+3) >= data.size()) {
                logStr += "\n";
                return Err_Data_End;
            }
            valSize = 32;
            union32.val_uint = (static_cast<quint32>(data.at(indx)) & 0x000000FF) |
                    ((static_cast<quint32>(data.at(indx+1)) & 0x000000FF) << 8) |
                    ((static_cast<quint32>(data.at(indx+2)) & 0x000000FF) << 16) |
                    ((static_cast<quint32>(data.at(indx+3)) & 0x000000FF) << 24);
            logStr += global::intToHex(union32.val_uint,8,true);
            valStrUnsign = QString::number(union32.val_uint, 10 );
            valStr = QString::number(union32.val_int, 10 );
            indx += 4;
            break;
        case DIF_REAL_32: // IEEE 754 - вещественное 4 байта
            if((indx+3) >= data.size()) {
                logStr += "\n";
                return Err_Data_End;
            }
            valSize = 32;
            intToFloat.val_uint = (static_cast<quint32>(data.at(indx)) & 0x000000FF) |
                    ((static_cast<quint32>(data.at(indx+1)) & 0x000000FF) << 8) |
                    ((static_cast<quint32>(data.at(indx+2)) & 0x000000FF) << 16) |
                    ((static_cast<quint32>(data.at(indx+3)) & 0x000000FF) << 24);
            logStr += global::intToHex( intToFloat.val_uint, 8, true);
            valStr = global::doubleToStr(  intToFloat.val_float  );
            indx += 4;
            break;
        case DIF_INT_48: // знаковое целое 6 байт
            if((indx+5) >= data.size()) {
                logStr += "\n";
                return Err_Data_End;
            }
            valSize = 48;
            ui64 = (static_cast<quint64>(data.at(indx)) & 0x00000000000000FF) |
                    ((static_cast<quint64>(data.at(indx+1)) & 0x00000000000000FF) << 8) |
                    ((static_cast<quint64>(data.at(indx+2)) & 0x00000000000000FF) << 16) |
                    ((static_cast<quint64>(data.at(indx+3)) & 0x00000000000000FF) << 24) |
                    ((static_cast<quint64>(data.at(indx+4)) & 0x00000000000000FF) << 32) |
                    ((static_cast<quint64>(data.at(indx+5)) & 0x00000000000000FF) << 40);
            logStr += global::intToHex(ui64,12, true);
            valStrUnsign = QString::number(ui64, 10 );

            if ((ui64 & 0x800000000000) == 0x800000000000) {
                valStr = QString::number( ~(ui64 & 0x7FFFFFFFFFFF), 10 );
            }
            else {
                valStr = valStrUnsign;
            }

            indx += 6;
            break;
        case DIF_INT_64: // знаковое целое 8 байт
            if((indx+7) >= data.size()) {
                logStr += "\n";
                return Err_Data_End;
            }
            valSize = 64;
            union64.val_uint = (static_cast<quint64>(data.at(indx)) & 0x00000000000000FF) |
                    ((static_cast<quint64>(data.at(indx+1)) & 0x00000000000000FF) << 8) |
                    ((static_cast<quint64>(data.at(indx+2)) & 0x00000000000000FF) << 16) |
                    ((static_cast<quint64>(data.at(indx+3)) & 0x00000000000000FF) << 24) |
                    ((static_cast<quint64>(data.at(indx+4)) & 0x00000000000000FF) << 32) |
                    ((static_cast<quint64>(data.at(indx+5)) & 0x00000000000000FF) << 40) |
                    ((static_cast<quint64>(data.at(indx+6)) & 0x00000000000000FF) << 48) |
                    ((static_cast<quint64>(data.at(indx+7)) & 0x00000000000000FF) << 56);
            logStr += global::intToHex(union64.val_uint,16,true);
            valStrUnsign = QString::number(union64.val_uint, 10 );
            valStr = QString::number(union64.val_int, 10 );
            indx += 8;
            break;
        case DIF_READOUT: // ?

            break;
        case DIF_BCD_2: // знаковое целое BCD 2 цифры (1 для отрицательных)
            if(indx >= data.size()) {
                logStr += "\n";
                return Err_Data_End;
            }
            valSize = 8;
            logStr += global::intToHex(data.at(indx++),2,true);
            valStr = correctingBCD( global::intToHex( data.at(indx++), 2, false ), &ok );
            if(!ok)  {
                logStr += "\n";
                return Err_BCD_Format;
            }
            break;
        case DIF_BCD_4: // знаковое целое BCD 4 цифры (3 для отрицательных)
            if(indx+1 >= data.size()) {
                logStr += "\n";
                return Err_Data_End;
            }
            valSize = 16;
            ui16 = (static_cast<quint16>(data.at(indx)) & 0x00FF) |
                    ((static_cast<quint16>(data.at(indx+1)) & 0x00FF) << 8);
            logStr += global::intToHex(ui16,4,true);
            valStr = correctingBCD( global::intToHex( ui16, 4, false ), &ok );
            if(!ok) {
                logStr += "\n";
                return Err_BCD_Format;
            }
            indx+=2;
            break;
        case DIF_BCD_6: // знаковое целое BCD 6 цифр (5 для отрицательных)
            if(indx+2 >= data.size()) {
                logStr += "\n";
                return Err_Data_End;
            }
            valSize = 24;
            ui32 = (static_cast<quint32>(data.at(indx)) & 0x000000FF) |
                    ((static_cast<quint32>(data.at(indx+1)) & 0x000000FF) << 8) |
                    ((static_cast<quint32>(data.at(indx+2)) & 0x000000FF) << 16);
            logStr += global::intToHex(ui32,6,true);
            valStr = correctingBCD( global::intToHex( ui32, 6, false ), &ok );
            if(!ok) {
                logStr += "\n";
                return Err_BCD_Format;
            }
            indx+=3;
            break;
        case DIF_BCD_8: // знаковое целое BCD 8 цифр (7 для отрицательных)
            if(indx+3 >= data.size()) {
                logStr += "\n";
                return Err_Data_End;
            }
            valSize = 32;
            ui32 = (static_cast<quint32>(data.at(indx)) & 0x000000FF) |
                    ((static_cast<quint32>(data.at(indx+1)) & 0x000000FF) << 8) |
                    ((static_cast<quint32>(data.at(indx+2)) & 0x000000FF) << 16) |
                    ((static_cast<quint32>(data.at(indx+3)) & 0x000000FF) << 24);
            logStr += global::intToHex(ui32,8,true);
            valStr = correctingBCD( global::intToHex( ui32, 8, false ), &ok );
            if(!ok) {
                logStr += "\n";
                return Err_BCD_Format;
            }
            indx+=4;
            break;
        case DIF_VAR: // ?

            break;
        case DIF_BCD_12: // знаковое целое BCD 12 цифр (11 для отрицательных)
            if(indx+5 >= data.size()) {
                logStr += "\n";
                return Err_Data_End;
            }
            valSize = 32;
            ui64 = (static_cast<quint64>(data.at(indx)) & 0x00000000000000FF) |
                    ((static_cast<quint64>(data.at(indx+1)) & 0x00000000000000FF) << 8) |
                    ((static_cast<quint64>(data.at(indx+2)) & 0x00000000000000FF) << 16) |
                    ((static_cast<quint64>(data.at(indx+3)) & 0x00000000000000FF) << 24) |
                    ((static_cast<quint64>(data.at(indx+4)) & 0x00000000000000FF) << 32) |
                    ((static_cast<quint64>(data.at(indx+5)) & 0x00000000000000FF) << 40);
            logStr += global::intToHex(ui64,12, true);
            valStr = correctingBCD( global::intToHex( ui64, 12, false ), &ok );
            if(!ok) {
                logStr += "\n";
                return Err_BCD_Format;
            }
            indx+=6;
            break;
        case DIF_SPECIAL: // ?

            break;
        default:

            break;
        }
        logStr += "\n";

        if(valStr.isEmpty()) return Err_DIF_Value;
        if(valSize==0) return Err_Numeric_Data_Format;

        // разбор значений:
        switch (vif.size()) {
        case 1:
            if (vif.first() == 0x6D) { // Дата и время
                quint64 dateVal = global::strToIntDef(  (valStrUnsign.isEmpty() ? valStr : valStrUnsign), (quint64)0 );
                quint16 yyyy;
                quint8  MM, dd, HH=0, mm=0, ss=0;

                if (valSize == 16) {
                    dd = static_cast<quint8>( dateVal & 0x1F );
                    MM = static_cast<quint8>( (dateVal >> 8) & 0x0F );
                    yyyy = static_cast<quint16>(((dateVal >> 5) & 0x07)  | ((dateVal >> 9) & 0x78));
                    if (yyyy<=80) yyyy += 2000; else yyyy += 1900;
                }
                else if (valSize == 32) {                    
                    mm = static_cast<quint8>(dateVal & 0x3F);
                    HH = static_cast<quint8>((dateVal >> 8) & 0x1F);
                    dd = static_cast<quint8>((dateVal >> 16) & 0x1F);
                    MM = static_cast<quint8>((dateVal >> 24) & 0x0F);
                    yyyy = static_cast<quint16>(
                                1900 + 100*((dateVal >> 13) & 0x03) +
                                (((dateVal >> 21) & 0x07) | ((dateVal >> 25) & 0x78))
                                );
                }
                else if (valSize == 48) {
                    if (((dateVal >> 15) & 1) == 1) {
                        logStr += QObject::tr("   Дата и время: установлен бит <Time Invalid>.") + "\n";
                    }
                    else {
                        ss = static_cast<quint8>( dateVal & 0x3F );
                        mm = static_cast<quint8>( (dateVal >> 8) & 0x3F );
                        HH = static_cast<quint8>( (dateVal >> 16) & 0x1F );
                    }
                    dd = static_cast<quint8>( (dateVal >> 24) & 0x1F );
                    MM = static_cast<quint8>( (dateVal >> 32) & 0x0F );
                    yyyy = static_cast<quint16>( ((dateVal >> 29) & 0x07) | ((dateVal >> 33) & 0x78) );
                    if (yyyy<=80) yyyy += 2000; else yyyy += 1900;
                }
                else return Err_DateTime_Format;

                strTmp = QString::number(yyyy, 10) + "-";
                if(MM<10) strTmp += "0";
                strTmp += QString::number(MM, 10)+"-";
                if(dd<10) strTmp += "0";
                strTmp += QString::number(dd, 10)+" ";
                if(HH<10) strTmp += "0";
                strTmp += QString::number(HH, 10)+":";
                if(mm<10) strTmp += "0";
                strTmp += QString::number(mm, 10)+":";
                if(ss<10) strTmp += "0";
                strTmp += QString::number(ss, 10);
                logStr += "   Date and Time: " + strTmp + "\n";
                dt = QDateTime::fromString(strTmp, "yyyy-MM-dd HH:mm:ss");
                if(!dt.isValid()) return Err_DateTime_Format;
            }
            else { // показания
                value.val = "";
                value.postfixNum = 0;
                value.prefixNum = 0;
                value.titleId = Title_None;
                value.edId = Ed_None;
                //
                if (vif.first() == 0x6E) { // без ед.измерения:
                    value.val = valStr;
                    vecVal.append( value );
                    logStr += "   Value: " + value.val + "\n";
                    continue;
                }

                QString sign = ""; // знак числа
                quint8 valVif = vif.first() & 0x7F; // vif без старшего бита
                qint32 st = 0; // степень ( значение * 10^st )
                bool valFind = false; // признак наличия показания

                int dotPos = valStr.indexOf(".");
                if(dotPos>0) {
                    st = dotPos + 1 - valStr.length();
                    valStr.remove(dotPos, 1);
                }

                if (valStr.left(1) == "-") { // если отрицательное
                    sign = "-"; // запоминаем знак
                    valStr.remove(0, 1);
                }

                // разбор по стандарту WMBus:
                switch ((valVif & 0x78)) {
                case 0x00: // энергия кВатт.час
                    st += (static_cast<qint32>(valVif & 0x07) - 6);
                    value.edId = Ed_kWh;
                    getTarifEnergyRec(device.devType, dif, value);
                    valFind = true;
                    break;
                case 0x08: // энергия Гкал, Дж <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

                    if((device.devType == Dev_Teplo) && (device.manId == manIdToWord("BTR")) && (device.ver == 0)) {
                        st += (static_cast<qint32>(valVif & 0x07) - 9);
                        value.edId = Ed_GKal;
                    }
                    else {
                        st += (static_cast<qint32>(valVif & 0x07) - 3);
                        value.edId = Ed_kJ;
                    }
                    getTarifEnergyRec(device.devType, dif, value);
                    valFind = true;
                    break;
               case 0x10: // объем м.куб
                    st += (static_cast<qint32>(valVif & 0x07) - 6);
                    value.edId = Ed_m3;
                    getTarifVolumeRec(device.devType, dif, value);
                    valFind = true;
                    break;
               case 0x18: // масса кг, т
                    if (device.devType == Dev_Teplo) {
                        st += (static_cast<qint32>(valVif & 0x07) - 6);
                        value.edId = Ed_t;
                    }
                    else {
                        st += (static_cast<qint32>(valVif & 0x07) - 3);
                        value.edId = Ed_kg;
                    }
                    getTarifMassRec(device.devType, dif, value);
                    valFind = true;
                    break;
               case 0x28: // мощность Ватт
                    st += (static_cast<qint32>(valVif & 0x07) - 3);
                    value.edId = Ed_W;
                    getTarifPowerRec(device.devType, value);
                    valFind = true;
                    break;
               case 0x30: // мощность Мкал/час
                    st += (static_cast<qint32>(valVif & 0x07) - 6);
                    value.edId = Ed_MKalh;
                    getTarifPowerRec(device.devType, value);
                    valFind = true;
                    break;
               case 0x38: // Объемный расход м.куб/час
                    st += (static_cast<qint32>(valVif & 0x07) - 6);
                    value.edId = Ed_m3h;
                    getTarifVolumeFlowRateRec(dif, value);
                    valFind = true;
                    break;
               case 0x40: // Объемный расход м.куб/мин
                    st += (static_cast<qint32>(valVif & 0x07) - 7);
                    value.edId = Ed_m3m;
                    getTarifVolumeFlowRateRec(dif, value);
                    valFind = true;
                    break;
               case 0x48: // Объемный расход м.куб/сек
                    st += (static_cast<qint32>(valVif & 0x07) - 9);
                    value.edId = Ed_m3s;
                    getTarifVolumeFlowRateRec(dif, value);
                    valFind = true;
                    break;
               case 0x50: // Массовый расход кг/час
                    st += (static_cast<qint32>(valVif & 0x07) - 3);
                    value.edId = Ed_kgh;
                    getTarifMassFlowRateRec(value);
                    valFind = true;
                    break;
               default: valFind = false; break;
               }

                if(!valFind) // если еще не найдено, разбор дальше
                switch ((valVif & 0x7C)) {
                case 0x20: // Период работы
                    switch (valVif & 0x03) {
                    case 0x00:  value.edId = Ed_sec;  break;
                    case 0x01:  value.edId = Ed_min;  break;
                    case 0x02:  value.edId = Ed_hour; break;
                    case 0x03:
                    default:    value.edId = Ed_day;  break;
                    }
                    getTarifTimeWorkingRec(value);
                    valFind = true;
                    break;
                case 0x24: // период нормальной работы
                    switch (valVif & 0x03) {
                    case 0x00:  value.edId = Ed_sec;  break;
                    case 0x01:  value.edId = Ed_min;  break;
                    case 0x02:  value.edId = Ed_hour; break;
                    case 0x03:
                    default:    value.edId = Ed_day;  break;
                    }
                    getTarifPeriodWorkingRec(device.devType, dif, value);
                    valFind = true;
                    break;
                case 0x58: // Температура подачи град.цел
                    st += (static_cast<qint32>(valVif & 0x03) - 3);
                    value.edId = Ed_gradC;
                    getTarifFlowTempRec(dif, value);
                    valFind = true;
                    break;
                case 0x5C: // Температура обратки град.цел
                    st += (static_cast<qint32>(valVif & 0x03) - 3);
                    value.edId = Ed_gradC;
                    getTarifReturnTempRec(value);
                    valFind = true;
                    break;
                case 0x60: // Разность температур Град.кел
                    st += (static_cast<qint32>(valVif & 0x03) - 3);
                    value.edId = Ed_gradK;
                    getTarifTempDiffRec(dif, value);
                    valFind = true;
                    break;
                case 0x64: // Внешняя температура Град.цел
                    st += (static_cast<qint32>(valVif & 0x03) - 3);
                    value.edId = Ed_gradC;
                    getTarifTempExternalRec(dif, value);
                    valFind = true;
                    break;
                case 0x68: // Давление бар
                    st += (static_cast<qint32>(valVif & 0x03) - 3);
                    if (device.devType == Dev_Teplo) {
                        value.edId = Ed_kgcm2;
                    }
                    else {
                        value.edId = Ed_bar;
                    }
                    getTarifPressureRec(dif, value);
                    valFind = true;
                    break;
                case 0x70: // Средняя Длительность
                    switch (valVif & 0x03) {
                    case 0x00:  value.edId = Ed_sec;  break;
                    case 0x01:  value.edId = Ed_min;  break;
                    case 0x02:  value.edId = Ed_hour; break;
                    case 0x03:
                    default:    value.edId = Ed_day;  break;
                    }
                    getTarifDurationAverRec(value);
                    valFind = true;
                    break;
                case 0x74: // Реальная Длительность
                    switch (valVif & 0x03) {
                    case 0x00:  value.edId = Ed_sec;  break;
                    case 0x01:  value.edId = Ed_min;  break;
                    case 0x02:  value.edId = Ed_hour; break;
                    case 0x03:
                    default:    value.edId = Ed_day;  break;
                    }
                    getTarifDurationRec(dif, value);
                    valFind = true;
                    break;

                default: valFind = false; break;
                }

                if(!valFind) {
                    logStr += "   - Неизвестное значение VIF: " + global::intToHex(vif.first(), 2, true) + "\n";
                    saveErrorVIFToTech( valStr, vif, vecTech );
                    break;
                }


                if(valStr.toLower()=="nan") {
                    value.val = valStr;
                }
                else {
                    qint32 n = qAbs(st);
                    if(st<0) {
                        while( valStr.length() < (n+1) ) valStr = "0" + valStr;
                        valStr.insert( valStr.length()-n, '.' );
                    }
                    else {
                        n = 1;
                        while(n <= st) {
                            valStr += "0";
                            n++;
                        }
                    }
                    value.val = delPrevNulls( sign + valStr );
                }
                vecVal.append( value );
                logStr += "   " + getValueStr(device.devType, value) + "\n";
            }
            break;
        case 2:
            if(vif.first() == 0xFB) {
                switch (vif.last()) {
                case 0x02:
                case 0x04: // - реактивная энергия
                    value.postfixNum = 0;
                    value.prefixNum = 0;
                    value.val = valStr;
                    value.edId = (vif.last() == 0x02) ? Ed_kVARh : Ed_VARh;
                    getTarifReactiveEnergyRec(dif, value);
                    vecVal.append( value );
                    logStr += "   " + getValueStr(device.devType, value) + "\n";
                    break;
                case 0x09: // - Энергия ГДж
                    value.postfixNum = 0;
                    value.prefixNum = 0;
                    value.val = valStr;
                    value.edId = Ed_GJ;
                    getTarifEnergyRec(device.devType, dif, value);
                    vecVal.append( value );
                    logStr += "   " + getValueStr(device.devType, value) + "\n";
                    break;
                case 0x0D: // - Энергия МКал
                    value.postfixNum = 0;
                    value.prefixNum = 0;
                    value.val = valStr;
                    value.edId = Ed_MKal;
                    getTarifEnergyRec(device.devType, dif, value);
                    vecVal.append( value );
                    logStr += "   " + getValueStr(device.devType, value) + "\n";
                    break;
                default:
                    logStr += "   - Неизвестное значение VIFE: " + global::intToHex(vif.last(), 2, true) + "\n";
                    saveErrorVIFToTech( valStr, vif, vecTech );
                    break;
                }
            }
            else if(vif.first() == 0xFD) {
                switch (vif.last()) {
                case 0x0E: // - Firmware Ver
                    while(valStr.length()<3) { valStr = "0" + valStr; }
                    valStr.insert(valStr.length()-2, '.');
                    value.postfixNum = 0;
                    value.prefixNum = 0;
                    value.val = valStr;
                    value.edId = Ed_None;
                    value.titleId = Title_FirmwareVer;
                    vecTech.append( value );

                    logStr += "   Firmware Ver: " + valStr + "\n";
                    break;
                case 0x17: // - флаги состояния
                    flag.flag =  valStrUnsign.isEmpty() ? valStr.toInt(&ok, 10) : valStrUnsign.toInt(&ok, 10);

                    if(valSize>8) {
                        flag.flag = ( static_cast<quint16>( global::lo(flag.flag) ) << 8) | global::hi(flag.flag);
                    }

                    if(!ok) return Err_Flags;
                    flag.postfixNum = 0;
                    flag.prefixNum = 0;
                    vecFlag.append(flag);
                    logStr += "   Flags: " + global::intToHex(flag.flag, 4, true) + "\n";
                    break;
                default:
                    logStr += "   - Неизвестное значение VIFE: " + global::intToHex(vif.last(), 2, true) + "\n";
                    saveErrorVIFToTech( valStr, vif, vecTech );
                    break;
                }
            }
            else if(vif.first() == 0xFF) {
                switch (vif.last()) {
                case 0x12: // - Firmware Id
                    if(!valStrUnsign.isEmpty()) {
                        valStr = valStrUnsign;
                    }
                    value.val = global::intToHex(global::strToIntDef(valStr, 0), valSize/4, true);
                    value.postfixNum = 0;
                    value.prefixNum = 0;
                    value.edId = Ed_None;
                    value.titleId = Title_FirmwareId;
                    vecTech.append( value );

                    logStr += "   Firmware Id: " + valStr + "\n";
                    break;
                case 0x17: // RSSI
                    value.postfixNum = 0;
                    value.prefixNum = 0;
                    value.val = valStr;
                    value.edId = Ed_dBm;
                    value.titleId = Title_SignalLevel;
                    vecTech.append( value );
                    logStr += "   " + getValueStr(device.devType, value) + "\n";
                    break;
                case 0x18: // Период отсутствия счета
                    value.postfixNum = 0;
                    value.prefixNum = 0;
                    value.val = valStr;
                    value.edId = Ed_hour;
                    getTarifPeriodNotCalcRec(device.devType, dif, value);
                    vecVal.append( value );
                    logStr += "   " + getValueStr(device.devType, value) + "\n";
                    break;
                case 0x13: // - Событие Event
                case 0x14: // - Key Id
                case 0x15: // - Phone
                default:
                    logStr += "   - Неизвестное значение VIFE: " + global::intToHex(vif.last(), 2, true) + "\n";
                    saveErrorVIFToTech( valStr, vif, vecTech );
                    break;
                }
            }
            else {
                logStr += "   - Неизвестное значение VIFE: " + global::intToHex(vif.first(), 2, true) + "\n";
                saveErrorVIFToTech( valStr, vif, vecTech );
                break;
            }

            break;
        default:
            logStr += "   - Неизвестное значение VIF: " +
                    ( vif.isEmpty() ? "" : global::intToHex(vif.first(), 2, true)) + "\n";
            saveErrorVIFToTech( valStr, vif, vecTech );
            break;
        }

    } // while ---------------------------------------------

    return Err_OK;
}
/*******************************************************************************************/
void saveErrorVIFToTech(const QString val, const ByteVector &vif, QVector<ValueStruct> &vecTech)
{
    ValueStruct value;
    value.postfixNum = 0;
    value.prefixNum = 0;
    value.edId = Ed_None;
    //
    QString vifStr = "0x";
    for(int i=0; i<vif.size(); i++) vifStr += global::intToHex( vif.at(i), 2, false );
    value.titleId = Title_UnknownVIF;
    value.val = vifStr;
    vecTech.append( value );
    //
    if(!val.isEmpty()) {
        value.titleId = Title_None;
        value.val = val;
        vecTech.append( value );
    }
}
/*******************************************************************************************/
TErrorCode readDeviceFromArray(const QByteArray &data, WMBusDevStruct &device, QString &logStr)
{   // чтение WMBus параметров устройства

    logStr = "";
    if(data.size() < 8) return Err_Data_End;

    device.manId = static_cast<quint8>(data.at(0)) | ( (static_cast<quint16>(data.at(1)) & 0x00FF) << 8 );
    logStr += "   wmbManId: " + global::intToHex(device.manId, 4, true)+ " (" + wordToManId(device.manId) + ")\n";
    if( !itsCorrectManId(device.manId) ) return Err_Param_WMBMan_Format;

    device.id = static_cast<quint8>(data.at(2)) | ( (static_cast<quint16>(data.at(3)) & 0x00FF) << 8 ) |
            ( (static_cast<quint32>(data.at(4)) & 0x000000FF) << 16 ) |
            ( (static_cast<quint32>(data.at(5)) & 0x000000FF) << 24 );
    logStr += "   wmbIdentNum: " + global::intToHex(device.id, 8, false) + "\n";
    if( !itsCorrectIdentNum(device.id) ) return Err_Param_WMBId_Format;

    device.ver = static_cast<quint8>(data.at(6));
    logStr += "   wmbVer: " + global::intToHex(device.ver, 2, true) + "\n";

    device.devType = static_cast<quint8>(data.at(7));
    logStr += "   wmbDevType: " + global::intToHex(device.devType, 2, true) + " - " + getDevTypeNameText(device.devType)+"\n";

    return Err_OK;
}
/*******************************************************************************************/
bool itsEnergyTitle(quint8 devType, quint16 titleId)
{
    return ( (devType == Dev_Electro) && ((titleId == Title_ElectroEnergy) || (titleId == Title_Energy)) ) ||
           ( (devType == Dev_Teplo) && ((titleId == Title_TermalEnergy) || (titleId == Title_Energy)) );
}
/*******************************************************************************************/
bool itsVolumeTitle(quint8 devType, quint16 titleId)
{
    return (((devType == Dev_Voda) || (devType == Dev_GorVoda) || (devType == Dev_HolVoda)) &&
            ((titleId == Title_Volume) || (titleId == Title_WaterVolume)))    ||
            ((devType == Dev_Gas) &&
            ((titleId == Title_Volume) || (titleId == Title_GasVolume)));
}
/*******************************************************************************************/
bool itsMainValueRec(quint8 devType, const ValueStruct &val, int prefix, int postfix)
{
    return (val.postfixNum==postfix) && (val.prefixNum==prefix) &&
           (itsEnergyTitle(devType, val.titleId) || itsVolumeTitle(devType, val.titleId));
}
/*******************************************************************************************/
int indexOfValueStruct(const ValueStruct &value, const QVector<ValueStruct> &vecVal)
{
    for(int i=0; i<vecVal.size(); i++) {
        ValueStruct val = vecVal.at(i);

        if( (value.edId == val.edId) && (value.titleId == val.titleId) && (value.prefixNum == val.prefixNum) &&
                (value.postfixNum == val.postfixNum) ) {
            return i;
        }
    }
    return -1;
}
/*******************************************************************************************/
bool calcValueToEdId(quint16 edId, ValueStruct &val)
{
    bool ok;    
    double dVal = global::strToDouble( val.val, &ok);
    if(!ok) return false;
    bool bFind = false;

    switch (edId) {
    case Ed_J: //---------------------------------------
        switch (val.edId) {
        case Ed_kJ:
            dVal = dVal * 1000.0;
            bFind = true; break;
        case Ed_MJ:
            dVal = dVal * 1000.0 * 1000.0;
            bFind = true; break;
        case Ed_GJ:
            dVal = dVal * 1000.0 * 1000.0 * 1000.0;
            bFind = true; break;
        case Ed_Wh:
            dVal = dVal * 3600.0;
            bFind = true; break;
        case Ed_kWh:
            dVal = dVal * 3600.0 * 1000.0;
            bFind = true; break;
        case Ed_MWh:
            dVal = dVal * 3600.0 * 1000.0 * 1000.0;
            bFind = true; break;
        case Ed_GWh:
            dVal = dVal * 3600.0 * 1000.0 * 1000.0 * 1000.0;
            bFind = true; break;
        case Ed_Kal:
            dVal = dVal * 4.1868;
            bFind = true; break;
        case Ed_kKal:
            dVal = dVal * 4.1868 * 1000.0;
            bFind = true; break;
        case Ed_MKal:
            dVal = dVal * 4.1868 * 1000.0 * 1000.0;
            bFind = true; break;
        case Ed_GKal:
            dVal = dVal * 4.1868 * 1000.0 * 1000.0 * 1000.0;
            bFind = true; break;
        default: return true;
        }
        break;
    case Ed_kJ: //---------------------------------------
        switch (val.edId) {
        case Ed_J:
            dVal = dVal / 1000.0;
            bFind = true; break;
        case Ed_MJ:
            dVal = dVal * 1000.0;
            bFind = true; break;
        case Ed_GJ:
            dVal = dVal * 1000.0 * 1000.0;
            bFind = true; break;
        case Ed_Wh:
            dVal = dVal * 3600.0 / 1000.0;
            bFind = true; break;
        case Ed_kWh:
            dVal = dVal * 3600.0;
            bFind = true; break;
        case Ed_MWh:
            dVal = dVal * 3600.0 * 1000.0;
            bFind = true; break;
        case Ed_GWh:
            dVal = dVal * 3600.0 * 1000.0 * 1000.0;
            bFind = true; break;
        case Ed_Kal:
            dVal = dVal * 4.1868 / 1000.0;
            bFind = true; break;
        case Ed_kKal:
            dVal = dVal * 4.1868;
            bFind = true; break;
        case Ed_MKal:
            dVal = dVal * 4.1868 * 1000.0;
            bFind = true; break;
        case Ed_GKal:
            dVal = dVal * 4.1868 * 1000.0 * 1000.0;
            bFind = true; break;
        default: return true;
        }
        break;
    case Ed_MJ: //---------------------------------------
        switch (val.edId) {
        case Ed_J:
            dVal = dVal / (1000.0 * 1000.0);
            bFind = true; break;
        case Ed_kJ:
            dVal = dVal / 1000.0;
            bFind = true; break;
        case Ed_GJ:
            dVal = dVal * 1000.0;
            bFind = true; break;
        case Ed_Wh:
            dVal = dVal * 3600.0 / (1000.0 * 1000.0);
            bFind = true; break;
        case Ed_kWh:
            dVal = dVal * 3600.0 / 1000.0;
            bFind = true; break;
        case Ed_MWh:
            dVal = dVal * 3600.0;
            bFind = true; break;
        case Ed_GWh:
            dVal = dVal * 3600.0 * 1000.0;
            bFind = true; break;
        case Ed_Kal:
            dVal = dVal * 4.1868 / (1000.0 * 1000.0);
            bFind = true; break;
        case Ed_kKal:
            dVal = dVal * 4.1868 / 1000.0;
            bFind = true; break;
        case Ed_MKal:
            dVal = dVal * 4.1868;
            bFind = true; break;
        case Ed_GKal:
            dVal = dVal * 4.1868 * 1000.0;
            bFind = true; break;
        default: return true;
        }
        break;
    case Ed_GJ: //---------------------------------------
        switch (val.edId) {
        case Ed_J:
            dVal = dVal / (1000.0 * 1000.0 * 1000.0);
            bFind = true; break;
        case Ed_kJ:
            dVal = dVal / (1000.0 * 1000.0);
            bFind = true; break;
        case Ed_MJ:
            dVal = dVal / 1000.0;
            bFind = true; break;
        case Ed_Wh:
            dVal = dVal * 3600.0 / (1000.0 * 1000.0 * 1000.0);
            bFind = true; break;
        case Ed_kWh:
            dVal = dVal * 3600.0 / (1000.0 * 1000.0);
            bFind = true; break;
        case Ed_MWh:
            dVal = dVal * 3600.0 / 1000.0;
            bFind = true; break;
        case Ed_GWh:
            dVal = dVal * 3600.0;
            bFind = true; break;
        case Ed_Kal:
            dVal = dVal * 4.1868 / (1000.0 * 1000.0 * 1000.0);
            bFind = true; break;
        case Ed_kKal:
            dVal = dVal * 4.1868 / (1000.0 * 1000.0);
            bFind = true; break;
        case Ed_MKal:
            dVal = dVal * 4.1868 / 1000.0;
            bFind = true; break;
        case Ed_GKal:
            dVal = dVal * 4.1868;
            bFind = true; break;
        default: return true;
        }
        break;
    case Ed_Wh: //---------------------------------------
        switch (val.edId) {
        case Ed_J:
            dVal = dVal * 0.00027778;
            bFind = true; break;
        case Ed_kJ:
            dVal = dVal * 0.27778;
            bFind = true; break;
        case Ed_MJ:
            dVal = dVal * 277.777778;
            bFind = true; break;
        case Ed_GJ:
            dVal = dVal * 277777.7777778;
            bFind = true; break;
        case Ed_kWh:
            dVal = dVal * 1000.0;
            bFind = true; break;
        case Ed_MWh:
            dVal = dVal * 1000.0 * 1000.0;
            bFind = true; break;
        case Ed_GWh:
            dVal = dVal * 1000.0 * 1000.0 * 1000.0;
            bFind = true; break;
        case Ed_Kal:
            dVal = dVal * 1.163;
            bFind = true; break;
        case Ed_kKal:
            dVal = dVal * 1.163 * 1000.0;
            bFind = true; break;
        case Ed_MKal:
            dVal = dVal * 1.163 * 1000.0 * 1000.0;
            bFind = true; break;
        case Ed_GKal:
            dVal = dVal * 1.163 * 1000.0 * 1000.0 * 1000.0;
            bFind = true; break;
        default: return true;
        }
        break;
    case Ed_kWh: //---------------------------------------
        switch (val.edId) {
        case Ed_J:
            dVal = dVal * 0.00027778 / 1000.0;
            bFind = true; break;
        case Ed_kJ:
            dVal = dVal * 0.27778 / 1000.0;
            bFind = true; break;
        case Ed_MJ:
            dVal = dVal * 277.777778 / 1000.0;
            bFind = true; break;
        case Ed_GJ:
            dVal = dVal * 277777.7777778 / 1000.0;
            bFind = true; break;
        case Ed_Wh:
            dVal = dVal / 1000.0;
            bFind = true; break;
        case Ed_MWh:
            dVal = dVal * 1000.0;
            bFind = true; break;
        case Ed_GWh:
            dVal = dVal * 1000.0 * 1000.0;
            bFind = true; break;
        case Ed_Kal:
            dVal = dVal * 1.163 / 1000.0;
            bFind = true; break;
        case Ed_kKal:
            dVal = dVal * 1.163;
            bFind = true; break;
        case Ed_MKal:
            dVal = dVal * 1.163 * 1000.0;
            bFind = true; break;
        case Ed_GKal:
            dVal = dVal * 1.163 * 1000.0 * 1000.0;
            bFind = true; break;
        default: return true;
        }
        break;
    case Ed_MWh: //---------------------------------------
        switch (val.edId) {
        case Ed_J:
            dVal = dVal * 0.00027778 / (1000.0 * 1000.0);
            bFind = true; break;
        case Ed_kJ:
            dVal = dVal * 0.27778 / (1000.0 * 1000.0);
            bFind = true; break;
        case Ed_MJ:
            dVal = dVal * 277.777778 / (1000.0 * 1000.0);
            bFind = true; break;
        case Ed_GJ:
            dVal = dVal * 277777.7777778 / (1000.0 * 1000.0);
            bFind = true; break;
        case Ed_Wh:
            dVal = dVal / (1000.0 * 1000.0);
            bFind = true; break;
        case Ed_kWh:
            dVal = dVal / 1000.0;
            bFind = true; break;
        case Ed_GWh:
            dVal = dVal * 1000.0;
            bFind = true; break;
        case Ed_Kal:
            dVal = dVal * 1.163 / (1000.0 * 1000.0);
            bFind = true; break;
        case Ed_kKal:
            dVal = dVal * 1.163 / 1000.0;
            bFind = true; break;
        case Ed_MKal:
            dVal = dVal * 1.163;
            bFind = true; break;
        case Ed_GKal:
            dVal = dVal * 1.163 * 1000.0;
            bFind = true; break;
        default: return true;
        }
        break;
    case Ed_GWh: //---------------------------------------
        switch (val.edId) {
        case Ed_J:
            dVal = dVal * 0.00027778 / (1000.0 * 1000.0 * 1000.0);
            bFind = true; break;
        case Ed_kJ:
            dVal = dVal * 0.27778 / (1000.0 * 1000.0 * 1000.0);
            bFind = true; break;
        case Ed_MJ:
            dVal = dVal * 277.777778 / (1000.0 * 1000.0 * 1000.0);
            bFind = true; break;
        case Ed_GJ:
            dVal = dVal * 277777.7777778 / (1000.0 * 1000.0 * 1000.0);
            bFind = true; break;
        case Ed_Wh:
            dVal = dVal / (1000.0 * 1000.0 * 1000.0);
            bFind = true; break;
        case Ed_kWh:
            dVal = dVal / (1000.0 * 1000.0);
            bFind = true; break;
        case Ed_MWh:
            dVal = dVal / 1000.0;
            bFind = true; break;
        case Ed_Kal:
            dVal = dVal * 1.163 / (1000.0 * 1000.0 * 1000.0);
            bFind = true; break;
        case Ed_kKal:
            dVal = dVal * 1.163 / (1000.0 * 1000.0);
            bFind = true; break;
        case Ed_MKal:
            dVal = dVal * 1.163 / 1000.0;
            bFind = true; break;
        case Ed_GKal:
            dVal = dVal * 1.163;
            bFind = true; break;
        default: return true;
        }
        break;
    case Ed_Kal: //---------------------------------------
        switch (val.edId) {
        case Ed_J:
            dVal = dVal * 0.239;
            bFind = true; break;
        case Ed_kJ:
            dVal = dVal * 0.239 * 1000.0;
            bFind = true; break;
        case Ed_MJ:
            dVal = dVal * 0.239 * 1000.0 * 1000.0;
            bFind = true; break;
        case Ed_GJ:
            dVal = dVal * 0.239 * 1000.0 * 1000.0 * 1000.0;
            bFind = true; break;
        case Ed_Wh:
            dVal = dVal * 859.5;
            bFind = true; break;
        case Ed_kWh:
            dVal = dVal * 859.5 * 1000.0;
            bFind = true; break;
        case Ed_MWh:
            dVal = dVal * 859.5 * 1000.0 * 1000.0;
            bFind = true; break;
        case Ed_GWh:
            dVal = dVal * 859.5 * 1000.0 * 1000.0 * 1000.0;
            bFind = true; break;
        case Ed_kKal:
            dVal = dVal * 1000.0;
            bFind = true; break;
        case Ed_MKal:
            dVal = dVal * 1000.0 * 1000.0;
            bFind = true; break;
        case Ed_GKal:
            dVal = dVal * 1000.0 * 1000.0 * 1000.0;
            bFind = true; break;
        default: return true;
        }
        break;
    case Ed_kKal: //---------------------------------------
        switch (val.edId) {
        case Ed_J:
            dVal = dVal * 0.239 / 1000.0;
            bFind = true; break;
        case Ed_kJ:
            dVal = dVal * 0.239;
            bFind = true; break;
        case Ed_MJ:
            dVal = dVal * 0.239 * 1000.0;
            bFind = true; break;
        case Ed_GJ:
            dVal = dVal * 0.239 * 1000.0 * 1000.0;
            bFind = true; break;
        case Ed_Wh:
            dVal = dVal * 859.5 / 1000.0;
            bFind = true; break;
        case Ed_kWh:
            dVal = dVal * 859.5;
            bFind = true; break;
        case Ed_MWh:
            dVal = dVal * 859.5 * 1000.0;
            bFind = true; break;
        case Ed_GWh:
            dVal = dVal * 859.5 * 1000.0 * 1000.0;
            bFind = true; break;
        case Ed_Kal:
            dVal = dVal / 1000.0;
            bFind = true; break;
        case Ed_MKal:
            dVal = dVal * 1000.0;
            bFind = true; break;
        case Ed_GKal:
            dVal = dVal * 1000.0 * 1000.0;
            bFind = true; break;
        default: return true;
        }
        break;
    case Ed_MKal: //---------------------------------------
        switch (val.edId) {
        case Ed_J:
            dVal = dVal * 0.239 / (1000.0 * 1000.0);
            bFind = true; break;
        case Ed_kJ:
            dVal = dVal * 0.239 / 1000.0;
            bFind = true; break;
        case Ed_MJ:
            dVal = dVal * 0.239;
            bFind = true; break;
        case Ed_GJ:
            dVal = dVal * 0.239 * 1000.0;
            bFind = true; break;
        case Ed_Wh:
            dVal = dVal * 859.5 / (1000.0 * 1000.0);
            bFind = true; break;
        case Ed_kWh:
            dVal = dVal * 859.5 / 1000.0;
            bFind = true; break;
        case Ed_MWh:
            dVal = dVal * 859.5;
            bFind = true; break;
        case Ed_GWh:
            dVal = dVal * 859.5 * 1000.0;
            bFind = true; break;
        case Ed_Kal:
            dVal = dVal / (1000.0 * 1000.0);
            bFind = true; break;
        case Ed_kKal:
            dVal = dVal / 1000.0;
            bFind = true; break;
        case Ed_GKal:
            dVal = dVal * 1000.0;
            bFind = true; break;
        default: return true;
        }
        break;
    case Ed_GKal: //---------------------------------------
        switch (val.edId) {
        case Ed_J:
            dVal = dVal * 0.239 / (1000.0 * 1000.0 * 1000.0);
            bFind = true; break;
        case Ed_kJ:
            dVal = dVal * 0.239 / (1000.0 * 1000.0);
            bFind = true; break;
        case Ed_MJ:
            dVal = dVal * 0.239 / 1000.0;
            bFind = true; break;
        case Ed_GJ:
            dVal = dVal * 0.239;
            bFind = true; break;
        case Ed_Wh:
            dVal = dVal * 859.5 / (1000.0 * 1000.0 * 1000.0);
            bFind = true; break;
        case Ed_kWh:
            dVal = dVal * 859.5 / (1000.0 * 1000.0);
            bFind = true; break;
        case Ed_MWh:
            dVal = dVal * 859.5 / 1000.0;
            bFind = true; break;
        case Ed_GWh:
            dVal = dVal * 859.5;
            bFind = true; break;
        case Ed_Kal:
            dVal = dVal / (1000.0 * 1000.0 * 1000.0);
            bFind = true; break;
        case Ed_kKal:
            dVal = dVal / (1000.0 * 1000.0);
            bFind = true; break;
        case Ed_MKal:
            dVal = dVal / 1000.0;
            bFind = true; break;
        default: return true;
        }
        break;
    case Ed_l: //---------------------------------------
        if(val.edId == Ed_m3) {
            dVal = dVal * 1000.0;
            bFind = true;
        }
        break;
    case Ed_m3: //---------------------------------------
        if(val.edId == Ed_l) {
            dVal = dVal / 1000.0;
            bFind = true;
        }
        break;
    default:
        return true;
    }

    if(bFind) {
        val.val = global::doubleToStr( dVal );
        val.edId = edId;
    }
    return true;
}
/*******************************************************************************************/
int calcFullPacketLen(quint8 field_L)
{
    return (field_L + 3 + 2*((field_L + 6) / 16));
}
/*******************************************************************************************/
int calcBlockCount(quint8 field_L)
{
    return (calcFullPacketLen(field_L) - 1 - field_L) / 2;
}
/*******************************************************************************************/
int calcLastBlockLen(quint8 field_L)
{
    return (field_L - 9) % 16;
}
/*******************************************************************************************/

} // namespace wmbus
