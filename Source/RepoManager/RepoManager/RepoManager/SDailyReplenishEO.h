#pragma once
//////////////////////////////////////////////////////////////////////////
#include <QVector>
#include <QString>
#include "SSaleRecordEO.h"
#include "SGoodsStateEO.h"
#include "SNetRepoEO.h"
#include "SModelConst.h"

struct SProductState
{
    QString sBarCode;
    QString sGoodNum;
    int nCanUsed;
    int nCanMoved;
    QString sOnlineState;
    QString sSaleNum;
    QString sRequireNum;
    QString sRemoveNum;
    QString sShortNum;
};

struct ReplenishContext
{
    int nCalcCircleCount;
    int nRemoveCount;
};

typedef QVector<SProductState*> SProductStates;

class SDailyReplenishEO
{
public:
    SDailyReplenishEO(SSaleGoods* pSaleGoods, SGoodStates* pGoodStates, SNetRepoInfos* pNetRepoInfos);
    ~SDailyReplenishEO();

    void calcDailyRepenish(const ReplenishContext& oReplenishContext);
    const SProductStates* productStates() const;

private:
    void freeProductStates();
    QString calcOnlineState(const QString& sGoodNum);
    QString calcSaleCount(const QString& sBarCode);
    QString calcRequireCount(const QString& sSaleCount, int nCalcCircleCount);
    QString calcRemoveCount(const SProductState* pProductState, int nRemoveCount);
    QString calcShortCount(const SProductState* pProductState);

private:
    SSaleGoods* m_pSaleGoods;
    SGoodStates* m_pGoodStates;
    SNetRepoInfos* m_pNetRepoInfos;

    SProductStates* m_pProductStates;
};