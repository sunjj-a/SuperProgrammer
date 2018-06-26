#pragma once
//////////////////////////////////////////////////////////////////////////
//销售实体信息

#include <QVector>
#include <QSet>
#include <QMap>
#include "SModelConst.h"

//单个销售信息
struct SSaleInfo
{
    QString sOrderNum;
    QString sTitile;
    QString sPrice;
    QString sBuyNum;
    QString sSystemNum;
    QString sProperty;
    QString sTCInfo;
    QString sRemark;
    QString sState;
    QString sSellerNum;
};

//原始销售信息集
typedef QVector<QStringList> SSourceInfos;
//销售信息集合
typedef QVector<SSaleInfo*> SSaleRecords;
//销售订单状态
typedef QSet<QString> SOrderStates;
//外部系统编码，销售量
typedef QMap<QString, QString> SSaleGoods;

class SSaleRecordEO
{
public:
    SSaleRecordEO(const SColumnOrders& oColOrders, const SSourceInfos& oSourceInfos);
    ~SSaleRecordEO();

    //根据销售订单状态查找销售信息
    void findSaleInfo(const SOrderStates& oOrderStates, SSaleRecords& oSaleInfos) const;

    //根据销售订单状态查找系统编码/销售量信息
    void findSaleInfo(const SOrderStates& oOrderStates, SSaleGoods& oSaleGoods) const;

private:
    void fillSaleInfos(const SColumnOrders& oColOrders, const SSourceInfos& oSourceInfos);
    void freeSaleInfo();
private:
    SSaleRecords* m_pSaleRecords;
};
