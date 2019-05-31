#include "smartstend.h"
#include "global.h"

SmartStend::SmartStend(QObject *parent) : QObject(parent)
{
    portStend = new QSerialPort();

//    Обмен с компьютером UART2
//    Скорость 115200
//    Длина слова 8 бит
//    1 стоп-бит
//    Контроля чётности нет
    portStend->setBaudRate(QSerialPort::Baud115200);
    portStend->setDataBits(QSerialPort::Data8);
    portStend->setParity(QSerialPort::NoParity);
    portStend->setStopBits(QSerialPort::OneStop);

}

SmartStend::~SmartStend()
{
    portStend->close();
}

void SmartStend::getPortPointer(QSerialPort * port)
{
    portStend = port;
}

void SmartStend::sendCommandToStend(QString cmd)
{

}

void SmartStend::slotGetAnsFromStend(QString answer)
{

//                                      Прерывание	Ответ РС
//Нажата кнопка ПРОШИТЬ	                BUTPRG=1	OK
//Нажата кнопка КАЛИБРОВКА	            BUTCAL=1	OK
//Ток потребления платы превышен	    OVRLD	    OK
//Ток потребления интерфейса превышен	INVALID	    OK

   if(answer == "BUTPRG=1") {//начать программирование bsl
       sendCommandToStend("OK");
       //start bsl
   }

   if(answer == "BUTCAL=1") {//начать процесс выполнения команд
       sendCommandToStend("OK");
       //start write & calibration
   }

   if(answer == "OVRLD") {//?
       sendCommandToStend("OK");
   }

   if(answer == "INVALID") {//?
       sendCommandToStend("OK");
   }

}

//------------------Проверка импульсных выходов-------------------

void SmartStend::readPulsesChannel1()
{
   //Чтение значения счётчика импульсов, канал 1	CNT1?	CNT1=xxx
    QByteArray buffer;
    QString bufferStr;

    portStend->clear();

    if(!portStend->isOpen()) {
        if(!portStend->open(QIODevice::ReadWrite)) {
//                  QMessageBox::information(this, "", "Не удалось открыть порт УСО-2. Рабочее место: " + QString::number(workPlaceNumber + 1));
            portStend->close();
//            label_StatusBar = ("Не удалось открыть порт УСО-2. Рабочее место: " +
//                                         QString::number(workPlaceNumber + 1));
//            emit errorStringSignal(label_StatusBar + '\n');
//            vectorIndicatorStateMatrix[currentBoxNumber][currentIndicatorNumber] = true;

//            emit workPlaceOff(currentIndicatorNumber);
//            emit checkWritingError(currentIndicatorNumber);

//                vectorIsErrorOccured[workPlaceNumber] = true;
            return;
        }
    }

    sendCommandToStend("CNT1?");

    global::pause(10);

    buffer = portStend->readAll();
    bufferStr = QString::fromLocal8Bit(buffer);

    if(bufferStr.left(4) != "CNT1") {//ошибка
        return;
    }




}

void SmartStend::readPulsesChannel2()
{
   //Чтение значения счётчика импульсов, канал 2	CNT2?	CNT2=xxx
}

void SmartStend::pulsesReset()
{
   //Сбросить счётчик импульсов, оба канала	CNTCLR	ОК
}

//------------------Проверка импульсных входов--------------------

void SmartStend::writePulsesToGenChannel1()
{
   //Запись числа импульсов в генератор импульсов, канал 1	GEN1=xxx	OK
}

void SmartStend::writePulsesToGenChannel2()
{
   //Запись числа импульсов в генератор импульсов, канал 2	GEN2=xxx	OK
}

//-----------------Внешний интерфейс обмена-----------------------

void SmartStend::externalInterfaceOn()
{
   //Подключить внешний интерфейс	EXT=1	OK
}

void SmartStend::externalInterfaceOff()
{
   //Отключить внешний интерфейс	 EXT=0	OK
}

void SmartStend::setTokInterfaceOff()
{
   //Установить ток отключения (в мА)	EXTSET=xxx	OK
}

//-----------------Калибровка частоты 512------------------------

void SmartStend::freqMeterOn()
{
   //Включить частотомер	FREQ=1	OK
}

void SmartStend::freqMeterOff()
{
   //Выключить частотомер	FREQ=0	OK
}

void SmartStend::readFreq()
{
   //Прочитать значение частоты	FREQ?	FREQ=xxx..xxx(в Гц)
}

//-----------------Ток потребления платы--------------------------

void SmartStend::plataOn()
{
   //Подать питание на плату	POW=1	OK
}

void SmartStend::plataOff()
{
   //Отключить питание от платы	POW=0	OK
}

void SmartStend::readTok()
{
   //Прочитать ток потребления	POW?	POW=xxxxxx(в мкА)
}

void SmartStend::setTokPlataOff()
{
   //Установить ток отключения (в мА)	POWSET=xxx	OK
}

//--------------------Программирование-----------------------------

void SmartStend::programmatorOn()
{
   //Подключить программатор к плате	PRG=1	OK
}

void SmartStend::programmatorOff()
{
   //Отключить программатор от платы	PRG=0	OK
}

//--------------------Индикация-------------------------------------

void SmartStend::finishIndicatorOn()
{
   //Включить индикатор ЗАВЕРШЕНО	FIN=1	OK
}

void SmartStend::finishIndicatorOff()
{
   //Отключить индикатор ЗАВЕРШЕНО	FIN=0	OK
}

void SmartStend::errorIndicatoOn()
{
   //Включить индикатор АВАРИЯ	ALR=1	OK
}

void SmartStend::errorIndicatoOff()
{
   //Отключить индикатор АВАРИЯ	ALR=0	OK
}

//--------------------Служебные------------------------------------

void SmartStend::testCommand()
{
   //Тестовая команда	STENDx	OK
}




