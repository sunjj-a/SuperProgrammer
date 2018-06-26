#include "SDailyReplenishEO.h"
#define UN_EXISTED "-" 
#define ZERO_COUNT "0"
#define GOOD_ONLINET_STATE QStringLiteral("当前在线")

SDailyReplenishEO::SDailyReplenishEO(SSaleGoods* pSaleGoods, SGoodStates* pGoodStates, SNetRepoInfos* pNetRepoInfos)
    :m_pProductStates(nullptr), m_pGoodStates(pGoodStates), m_pSaleGoods(pSaleGoods), m_pNetRepoInfos(pNetRepoInfos)
{
    m_pProductStates = new SProductStates;
}

SDailyReplenishEO::~SDailyReplenishEO()
{
    delete m_pProductStates;
    m_pProductStates = nullptr;

    m_pGoodStates = nullptr;
    m_pNetRepoInfos = nullptr;
    m_pProductStates = nullptr;
}

bool compareRemoveNum(SProductState* pFirstProduct, SProductState* pSecondProduct)
{
    return pFirstProduct->sRemoveNum.toInt() < pSecondProduct->sRemoveNum.toInt();
}

void SDailyReplenishEO::calcDailyRepenish(const ReplenishContext& oReplenishContext)
{
    freeProductStates();
    for (auto pIter = m_pNetRepoInfos->begin(); pIter != m_pNetRepoInfos->end(); ++pIter)
    {
        SNetRepoInfo* pNetRepoInfo = *pIter;
        SProductState* pProductState = new SProductState;
        pProductState->sBarCode = pNetRepoInfo->sBarCode;
        pProductState->sGoodNum = pNetRepoInfo->sGoodNum;
        pProductState->nCanUsed = pNetRepoInfo->nCanUsed;
        pProductState->nCanMoved = pNetRepoInfo->nCanMoved;
        pProductState->sOnlineState = calcOnlineState(pProductState->sGoodNum);
        pProductState->sSaleNum = calcSaleCount(pProductState->sBarCode);
        pProductState->sRequireNum = calcRequireCount(pProductState->sSaleNum, oReplenishContext.nCalcCircleCount);
        pProductState->sRemoveNum = calcRemoveCount(pProductState, oReplenishContext.nRemoveCount);
        m_pProductStates->push_back(pProductState);
    }
    qSort(m_pProductStates->begin(), m_pProductStates->end(), compareRemoveNum);
}

const SProductStates* SDailyReplenishEO::productStates() const
{
    return m_pProductStates;
}

void SDailyReplenishEO::freeProductStates()
{
    for (auto pIter = m_pProductStates->begin(); pIter != m_pProductStates->end(); ++pIter)
    {
        SProductState* pProductState = *pIter;
        delete pProductState;
        pProductState = nullptr;
    }
    m_pProductStates->clear();
}

QString SDailyReplenishEO::calcOnlineState(const QString& sGoodNum)
{
    return m_pGoodStates->value(sGoodNum, UN_EXISTED);
}

QString SDailyReplenishEO::calcSaleCount(const QString& sBarCode)
{
    return m_pSaleGoods->value(sBarCode, UN_EXISTED);
}

QString SDailyReplenishEO::calcRequireCount(const QString& sSaleCount, int nCalcCircleCount)
{
    if (sSaleCount == UN_EXISTED)
        return UN_EXISTED;

    return QString::number(sSaleCount.toInt() * nCalcCircleCount);
}

QString SDailyReplenishEO::calcRemoveCount(const SProductState* pProductState, int nRemoveCount)
{
    if (pProductState->sOnlineState == GOOD_ONLINET_STATE)
    {
        bool bSccuess = false;
        int nRequireNum = pProductState->sRequireNum.toInt(&bSccuess);
        if (bSccuess && nRequireNum > 0)
        {
            return pProductState->sRequireNum;
        }
        else
        {
            return QString::number(nRemoveCount);
        }
    }
    else
    {
        return ZERO_COUNT;
    }
}

