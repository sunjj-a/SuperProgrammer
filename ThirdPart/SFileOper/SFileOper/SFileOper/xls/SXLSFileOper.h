#pragma once
//////////////////////////////////////////////////////////////////////////
//xls�ļ�����
#include "sfileoper_global.h"
#include <QString>
#include <QVariantList>
#include <QAxObject>

//xls�ļ�����
struct SFILEOPER_EXPORT SExcelParams
{
    QString sExcelFilePath;   //Excel�ļ�·��
    QString sOpenSheetName;   //��ҳǩ����
    QString sCellRange;       //��Ԫ��Χ
};

//xls�������
typedef QVariantList SXLSContainer;

//xls�ļ�����
class SFILEOPER_EXPORT SXLSFileOper
{
public:
    SXLSFileOper();
    ~SXLSFileOper();

    //��ȡxls���
    bool readFromFile(const SExcelParams& oExcelParams, SXLSContainer& oXLSContainer);

private:
    QAxObject* m_pAxObject;
};