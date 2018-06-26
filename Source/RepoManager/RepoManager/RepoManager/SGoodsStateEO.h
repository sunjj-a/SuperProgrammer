#pragma once
//////////////////////////////////////////////////////////////////////////
//商品状态实体
#include <QVector>
#include <QMap>
#include <QString>
#include <QVariantList>
#include "SModelConst.h"

//单个商品状态信息
struct SGoodInfo
{
    QString sMidCalc;
    QString sGoodNum;

    QString s000;
    QString s001;
    QString sGoodTitle;
    QString sOnlineState;
    QString s004;
    QString s005;
    QString s006;
    QString s007;
    QString s008;
    QString s009;
    QString s010;
    QString s011;
    QString s012;
    QString s013;
    QString s014;
    QString s015;
    QString s016;
    QString s017;
    QString s018;
    QString s019;
    QString s020;
    QString s021;
    QString s022;
    QString s023;
    QString s024;
    QString s025;
    QString s026;
    QString s027;
    QString s028;
    QString s029;
};

//原始商品状态
typedef QVariantList SSourceGoods;
//商品状态集合
typedef QVector<SGoodInfo*> SGoodRecords;
//商品编码-在线状态
typedef QMap<QString, QString> SGoodStates;

class SGoodsStateEO
{
public:
    SGoodsStateEO(const SColumnOrders& oColOrders, const SSourceGoods& oSourceInfos);
    ~SGoodsStateEO();

    //计算商品编码-在线状态
    void calcGoodNum(SGoodStates& oGoodStates);

    //返回所有的商品状态信息
    const SGoodRecords* goodRecords() const;

private:
    void fillGoodRecords(const SColumnOrders& oColOrders, const SSourceGoods& oSourceInfos);
    void freeGoodRecords();
    QString subGoodNum(const QString& sMidCalc);

private:
    SGoodRecords* m_pGoodRecords;
};