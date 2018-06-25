#pragma once
//////////////////////////////////////////////////////////////////////////
//xls文件操作
#include "sfileoper_global.h"
#include <QString>
#include <QVariantList>
#include <QAxObject>

//xls文件参数
struct SFILEOPER_EXPORT SExcelParams
{
    QString sExcelFilePath;   //Excel文件路径
    QString sOpenSheetName;   //打开页签名称
    QString sCellRange;       //单元格范围
};

//xls结果容器
typedef QVariantList SXLSContainer;

//xls文件操作
class SFILEOPER_EXPORT SXLSFileOper
{
public:
    SXLSFileOper();
    ~SXLSFileOper();

    //读取xls结果
    bool readFromFile(const SExcelParams& oExcelParams, SXLSContainer& oXLSContainer);

private:
    QAxObject* m_pAxObject;
};