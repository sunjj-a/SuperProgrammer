#pragma once
//////////////////////////////////////////////////////////////////////////
//Ԥ����Ʒʵ��
#include <QSet>
#include <QVector>
#include <QString>
#include <QVariantList>
#include "SNetRepoEO.h"

//����Ԥ����Ϣ
struct SPreSaleInfo
{
    QString sBarCode;
    QString sGoodNum;
    int nCanUsed;
    int nCanMoved;
};

typedef QVector<SPreSaleInfo*> SPreSaleResult;

//Ԥ����Ϣ����
typedef QSet<QString> SPreSaleInfos;
//ԭʼԤ����Ϣ
typedef QVariantList SSourcePreSales;

class SPreSaleEO
{
public:
    SPreSaleEO(const SSourcePreSales& oSourcePreSales);
    ~SPreSaleEO();

    //����Ԥ�۵���Ϣ
    void calcPreSale(const SNetRepoInfos& oNetRepoInfos);

    //Ԥ�۽��
    const SPreSaleResult* preSaleResult() const;

private:
    bool isPreSale(const QString& sGoodNum);
    void fillPreSaleInfos(const SSourcePreSales& oSourcePreSales);
    void freePreSaleResult();

private:
    SPreSaleInfos* m_pPreSaleInfos;
    SPreSaleResult* m_pPreSaleResult;
};