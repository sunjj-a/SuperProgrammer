#include "SXLSFileOper.h"
#include <assert.h>
#include <QDebug>

SXLSFileOper::SXLSFileOper()
    :m_pAxObject(nullptr)
{
    m_pAxObject = new QAxObject("Excel.Application");  
    if (!m_pAxObject)  
        assert(false);
    m_pAxObject->dynamicCall("SetVisible(bool)", false);  
}

SXLSFileOper::~SXLSFileOper()
{
    //m_pAxObject->dynamicCall("Close()"); 
    m_pAxObject->dynamicCall("Quit()");

    delete m_pAxObject;
    m_pAxObject = nullptr;
}

bool SXLSFileOper::readFromFile(const SExcelParams& oExcelParams, SXLSContainer& oXLSContainer)
{
    QAxObject* pWorkBooks = m_pAxObject->querySubObject("WorkBooks");  
    QAxObject* pWorkBook = pWorkBooks->querySubObject("Open(QString, QVariant)", oExcelParams.sExcelFilePath); 
    if (!pWorkBook)
    {
        assert(false);
        return false;
    }

    QAxObject* pWorkSheet = pWorkBook->querySubObject("WorkSheets(int)", oExcelParams.nSheetIndex);
    if (!pWorkSheet)
    {
        assert(false);
        return false;
    }

    QAxObject* pUsedRange = pWorkSheet->querySubObject("UsedRange");
    QAxObject* pRows = pUsedRange->querySubObject("Rows");  
    QAxObject* pColumns = pUsedRange->querySubObject("Columns");  
    int nRows = pRows->property("Count").toInt();  
    int nCols = pColumns->property("Count").toInt();  
    qDebug() << "xls行数："<< nRows;  
    qDebug() << "xls列数："<< nCols;  

    QString sCellRange = oExcelParams.sCellRange + QString::number(nRows);  
    QAxObject *pAllEnvData = pWorkSheet->querySubObject("Range(QString)", sCellRange);  
    if (!pAllEnvData)
    {
        assert(false);
        return false;
    }
    
    oXLSContainer = pAllEnvData->property("Value").toList(); 
    return true;
}

