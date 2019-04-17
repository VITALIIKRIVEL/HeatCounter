#include "xls.h"
#include "filesys.h"

namespace nayk {
//=======================================================================================================
Xls::Xls(QObject *parent, const QString &fileName, bool visible): QObject(parent)
{
    _visible = visible;

    excel = new QAxObject( "Excel.Application", this );
    _ok = excel;
    if(!_ok) {
        _lastError = "Не найден Microsoft Excel.";
        return;
    }
    excel->setProperty( "DisplayAlerts", 0 );
    excel->dynamicCall( "SetVisible(bool)", _visible );

    workbooks = excel->querySubObject( "Workbooks" );
    _ok = workbooks;
    if(!_ok) {
        _lastError = "Ошибка доступа к Excel.";
        return;
    }

    if(fileName.isNull() || fileName.isEmpty()) {
        workbook = workbooks->querySubObject( "Add()" );
        _ok = workbook;
        if(!_ok) {
            _lastError = QString("Не удалось создать документ.");
            return;
        }
    }
    else if(!FileSys::fileExists(fileName)) {
        _lastError = QString("Файл '%1' не найден.").arg(fileName);
        _ok = false;
        return;
    }
    else {
        workbook = workbooks->querySubObject( "Open(const QString&)", fileName );
        _ok = workbook;
        if(!_ok) {
            _lastError = QString("Не удалось открыть файл '%1'.").arg(fileName);
            return;
        }
        _fileName = fileName;
    }

    sheets = workbook->querySubObject( "Sheets" );
    _ok = sheets;
    if(!_ok) {
        _lastError = "Ошибка доступа к страницам книги.";
        return;
    }
}
//=======================================================================================================
Xls::~Xls()
{
    close();
}
//=======================================================================================================
bool Xls::setVisible(bool visible)
{
    if(excel) {
        _visible = visible;
        excel->dynamicCall( "SetVisible(bool)", _visible );
        return true;
    }
    _lastError = "Excel не найден.";
    return false;
}
//=======================================================================================================
bool Xls::save(const QString &fileName)
{
    if(!workbook) {
        _lastError = QString("Документ не создан.");
        return false;
    }
    if(fileName.isNull() || fileName.isEmpty()) {

        if(_fileName.isNull() || _fileName.isEmpty()) {
            _lastError = "Не указано имя файла.";
            return false;
        }
        workbook->dynamicCall( "SaveAs(const QString&)", _fileName );
    }
    else {
        workbook->dynamicCall( "SaveAs(const QString&)", fileName );
        _fileName = fileName;
    }
    return true;
}
//=======================================================================================================
bool Xls::close()
{
    if(workbook) workbook->dynamicCall("Close()");
    if(excel) excel->dynamicCall("Quit()");

    if(sheets) delete sheets;
    if(workbook) delete workbook;
    if(workbooks) delete workbooks;
    if(excel) delete excel;

    sheets = nullptr;
    workbook = nullptr;
    workbooks = nullptr;
    excel = nullptr;

    return true;
}
//=======================================================================================================
bool Xls::addSheet(const QString &templateFile)
{
    if(!workbook) {
        _lastError = QString("Документ не создан.");
        return false;
    }
    if(!sheets) {
        _lastError = QString("Документ не содержит страниц.");
        return false;
    }

    if(templateFile.isNull() || templateFile.isEmpty()) {
        //
    }
    else if(!FileSys::fileExists(templateFile)) {
        _lastError = QString("Файл '%1' не найден.").arg(templateFile);
        return false;
    }
    else {

        QAxObject * w = workbooks->querySubObject( "Add(const QString&)", templateFile );
        if(!w) {
            _lastError = QString("Не удалось открыть файл '%1'.").arg(templateFile);
            return false;
        }

        QAxObject *ss = w->querySubObject( "Sheets" );
        if(!ss) {
            _lastError = QString("Не удалось получить страницы книги.");
            return false;
        }

        QAxObject *s = ss->querySubObject( "Item(int)", 1 );
        if(!s) {
            _lastError = QString("Не удалось получить страницу книги.");
            return false;
        }

        s->querySubObject( "Copy(const QVariant&)",
                           sheets->querySubObject( "Item(int)", 1 )->asVariant() );

        w->dynamicCall("Close()");
        delete s;
        delete ss;
        delete w;

        return true;
    }

    return false;
}
//=======================================================================================================
int Xls::sheetsCount()
{
    if(!sheets) {
        _lastError = QString("Не удалось получить страницы книги.");
        return -1;
    }

    return sheets->dynamicCall("Count()").toInt();
}
//=======================================================================================================
bool Xls::delSheet(int num)
{
    QAxObject *s = getSheet(num);
    if(!s) {
        return false;
    }
    s->dynamicCall("Delete()").toBool();
    delete s;
    return true;
}
//=======================================================================================================
bool Xls::setSheetName(int num, const QString &name)
{
    QAxObject *s = getSheet(num);
    if(!s) {
        return false;
    }
    s->setProperty("Name", name);
    delete s;
    return true;
}
//=======================================================================================================
bool Xls::addRow(int sheetNum, int rowNum, int rowCount)
{
    QAxObject *s = getSheet(sheetNum);
    if(!s) {
        return false;
    }
    bool res = true;

    for(auto i=0; i<rowCount; ++i) {

        QAxObject *rangec1 = s->querySubObject( "Range(const QVariant&)",QVariant(QString("%1:%2").arg(rowNum).arg(rowNum) ));
        if(!rangec1) {
            _lastError = QString("Не удалось найти ячейку.");
            res = false;
            break;
        }
        else {
            rangec1->dynamicCall("Select()");
            rangec1->dynamicCall("Insert");
            delete rangec1;
        }
    }

    return res;
}
//=======================================================================================================
QAxObject* Xls::getSheet(int num)
{
    QAxObject* s = nullptr;

    if(!sheets) {
        _lastError = QString("Не удалось получить страницы книги.");
    }
    else if( (num < 1) || (num > sheetsCount())) {
        _lastError = QString("Номер страницы за пределами книги.");
    }
    else {
        s = sheets->querySubObject("Item(int)", num);
        if(!s) {
            _lastError = QString("Не удалось получить страницу книги.");
        }
    }
    return s;
}
//=======================================================================================================
bool Xls::setCell(int sheetNum, int rowNum, int colNum, int cellCount, bool mergeCells,
                  XlBorderWeight borderWeight, XlLineStyle lineStyle, const QString &text)
{
    QAxObject *s = getSheet(sheetNum);
    if(!s) {
        return false;
    }

    QAxObject *range = s->querySubObject( "Range(const QVariant&)",QVariant(QString("%1:%2")
                                                                            .arg( cellName(rowNum, colNum) )
                                                                            .arg( cellName(rowNum, colNum + cellCount - 1) ) ));
    if(!range) {
        _lastError = QString("Не удалось найти ячейку.");
        return false;
    }

    range->setProperty("MergeCells", QVariant(mergeCells));
    QAxObject *borders = range->querySubObject("Borders");
    if(borders) {
        borders->setProperty("LineStyle", QVariant(lineStyle));
        borders->setProperty("Weight", QVariant(borderWeight));
        delete borders;
    }

    range->setProperty("Value", QVariant(text));

    delete range;
    return true;
}
//=======================================================================================================
QString Xls::cellName(quint32 row, quint8 col)
{
    if (col == 0) col = 1;
    if (row == 0) row = 1;
    QString res = "";

    if(col > 26) {
        res = QString( QChar(64 + col/26)) + QString(QChar( 64 + col % 26 ) );
    }
    else {
        res = QString( QChar(64 + col) );
    }
    return res + QString::number(row);
}
//=======================================================================================================
} // namespace nayk

