#pragma once
//////////////////////////////////////////////////////////////////////////
//预售商品实体
#include <QSet>
#include <QVector>
#include <QString>
#include <QVariantList>
#include "SNetRepoEO.h"

//单条预售信息
struct SPreSaleInfo
{
    QString sBarCode;
    QString sGoodNum;
    int nCanUsed;
    int nCanMoved;
};

typedef QVector<SPreSaleInfo*> SPreSaleResult;

//预售信息集合
typedef QSet<QString> SPreSaleInfos;
//原始预售信息
typedef QVariantList SSourcePreSales;

class SPreSaleEO
{
public:
    SPreSaleEO(const SSourcePreSales& oSourcePreSales);
    ~SPreSaleEO();

    //计算预售的信息
    void calcPreSale(const SNetRepoInfos& oNetRepoInfos);

    //预售结果
    const SPreSaleResult* preSaleResult() const;

private:
    bool isPreSale(const QString& sGoodNum);
    void fillPreSaleInfos(const SSourcePreSales& oSourcePreSales);
    void freePreSaleResult();

private:
    SPreSaleInfos* m_pPreSaleInfos;
    SPreSaleResult* m_pPreSaleResult;
};