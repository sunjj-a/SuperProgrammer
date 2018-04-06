#include "SSAExcelBaseOper.h"
#include <assert.h>
#include <QDebug>

/*
 *	���캯��
 */
SSAExcelBaseOper::SSAExcelBaseOper()
    :m_pAxObject(nullptr)
{
    m_pAxObject = new QAxObject("Excel.Application");  
    if (!m_pAxObject)  
        assert(false);
    m_pAxObject->dynamicCall("SetVisible(bool)", false);  
}

/*
 *	��������
 */
SSAExcelBaseOper::~SSAExcelBaseOper()
{
    m_pAxObject->dynamicCall("Close()"); 
    m_pAxObject->dynamicCall("Quit()");

    delete m_pAxObject;
    m_pAxObject = nullptr;
}

/*
 *	��ȡExcel�ĵ�Ԫ������
 */
QVariantList SSAExcelBaseOper::records(const SSAExcelParams& oParams, QString& sErrorMsg)
{
    QAxObject* pWorkBooks = m_pAxObject->querySubObject("WorkBooks");  
    QAxObject* pWorkBook = pWorkBooks->querySubObject("Open(QString, QVariant)", oParams.sExcelFilePath); 
    if (!pWorkBook)
    {
        sErrorMsg += QStringLiteral("����Excelʧ�� T_T\n");
        return QVariantList();
    }
        
    QAxObject* pWorkSheet = pWorkBook->querySubObject("WorkSheets(QString)", oParams.sOpenSheetName);
    if (!pWorkSheet)
    {
        sErrorMsg += QStringLiteral("��ҳǩʧ�� T_T\n");
        return QVariantList();
    }
        
    QAxObject* pUsedRange = pWorkSheet->querySubObject("UsedRange");
    QAxObject* pRows = pUsedRange->querySubObject("Rows");  
    QAxObject* pColumns = pUsedRange->querySubObject("Columns");  
    int nRows = pRows->property("Count").toInt();  
    int nCols = pColumns->property("Count").toInt();  
    qDebug() << "xls������"<< nRows;  
    qDebug() << "xls������"<< nCols;  

    QString sCellRange = oParams.sCellRange + QString::number(nRows);  
    QAxObject *pAllEnvData = pWorkSheet->querySubObject("Range(QString)", sCellRange);  
    if (!pAllEnvData)
    {
        sErrorMsg += QStringLiteral("��ѯ��Ԫ������ʧ�� T_T\n");
        return QVariantList();
    }
    return pAllEnvData->property("Value").toList();  
}
