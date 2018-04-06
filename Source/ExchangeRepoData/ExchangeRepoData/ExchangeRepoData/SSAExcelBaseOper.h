#ifndef SSAEXCELBASEOPER_H
#define SSAEXCELBASEOPER_H
/*
 *	excel基本操作 - 快速度
 */

#include <QAxObject>
#include <QMetaType>
#include <QString>
#include <QString>

/*
 *	读文件参数
 */
struct SSAExcelParams
{
public:
    QString sExcelFilePath;   //Excel文件路径
    QString sOpenSheetName;   //打开页签名称
    QString sCellRange;       //单元格范围

public:
    SSAExcelParams(const QString& excelFilePath = "", const QString& openSheetName = "", const QString& cellRange = ""):
      sExcelFilePath(excelFilePath), sOpenSheetName(openSheetName), sCellRange(cellRange){};
};

/*
 *	Excel 读取操作类
 */
class SSAExcelBaseOper
{
public:
    SSAExcelBaseOper();
    ~SSAExcelBaseOper();

    QVariantList records(const SSAExcelParams& oParams, QString& sErrorMsg);

private:
    QAxObject* m_pAxObject;
};
#endif