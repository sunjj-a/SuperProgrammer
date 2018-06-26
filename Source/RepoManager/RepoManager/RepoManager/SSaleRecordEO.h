#pragma once
//////////////////////////////////////////////////////////////////////////
#include <QVector>
#include <QSet>
#include <QMap>
#include "SModelConst.h"

struct SSaleInfo
{
    QString sOrderNum;
    QString sTitile;
    double dPrice;
    int nBuyNum;
    QString sSystemNum;
    QString sProperty;
    QString sTCInfo;
    QString sRemark;
    QString sState;
    QString sSellerNum;
};

typedef QVector<QStringList> SSourceInfos;
typedef QVector<SSaleInfo*> SSaleRecords;
typedef QSet<QString> SOrderStates;
typedef QMap<QString, int> SSaleGoods;

class SSaleRecordEO
{
public:
    SSaleRecordEO(const SColumnOrders& oColOrders, const SSourceInfos& oSourceInfos);
    ~SSaleRecordEO();

    void findSaleInfo(const SOrderStates& oOrderStates, SSaleRecords& oSaleInfos) const;

    void findSaleInfo(const SOrderStates& oOrderStates, SSaleGoods& oSaleGoods) const;

private:
    void fillSaleInfos(const SColumnOrders& oColOrders, const SSourceInfos& oSourceInfos);
    void freeSaleInfo();
private:
    SSaleRecords* m_pSaleRecords;
};
