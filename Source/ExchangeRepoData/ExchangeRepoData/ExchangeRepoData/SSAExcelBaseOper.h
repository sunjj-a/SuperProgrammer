#ifndef SSAEXCELBASEOPER_H
#define SSAEXCELBASEOPER_H
/*
 *	excel�������� - ���ٶ�
 */

#include <QAxObject>
#include <QMetaType>
#include <QString>
#include <QString>

/*
 *	���ļ�����
 */
struct SSAExcelParams
{
public:
    QString sExcelFilePath;   //Excel�ļ�·��
    QString sOpenSheetName;   //��ҳǩ����
    QString sCellRange;       //��Ԫ��Χ

public:
    SSAExcelParams(const QString& excelFilePath = "", const QString& openSheetName = "", const QString& cellRange = ""):
      sExcelFilePath(excelFilePath), sOpenSheetName(openSheetName), sCellRange(cellRange){};
};

/*
 *	Excel ��ȡ������
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