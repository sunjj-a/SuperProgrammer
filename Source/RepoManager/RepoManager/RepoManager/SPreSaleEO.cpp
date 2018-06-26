#include "SPreSaleEO.h"
#include "SModelConst.h"

SPreSaleEO::SPreSaleEO(const SSourcePreSales& oSourcePreSales)
    :m_pPreSaleInfos(nullptr), m_pPreSaleResult(nullptr)
{
    m_pPreSaleResult = new SPreSaleResult;
    m_pPreSaleInfos = new SPreSaleInfos;
    fillPreSaleInfos(oSourcePreSales);
}

SPreSaleEO::~SPreSaleEO()
{
    freePreSaleResult();
    freeAndNull(m_pPreSaleResult);
    freeAndNull(m_pPreSaleInfos);
}

bool SPreSaleEO::isPreSale(const QString& sGoodNum)
{
    return m_pPreSaleInfos->contains(sGoodNum);
}

void SPreSaleEO::fillPreSaleInfos(const SSourcePreSales& oSourcePreSales)
{
    m_pPreSaleInfos->clear();
    for (auto pIter = oSourcePreSales.begin(); pIter != oSourcePreSales.end(); ++pIter)
    {
        QStringList oRecords = pIter->toStringList();
        m_pPreSaleInfos->insert(oRecords.at(0));
    }
}

void SPreSaleEO::freePreSaleResult()
{
    for (auto pIter = m_pPreSaleResult->begin(); pIter != m_pPreSaleResult->end(); ++pIter)
    {
        SPreSaleInfo* pPreSaleInfo = *pIter;
        delete pPreSaleInfo;
        pPreSaleInfo = nullptr;
    }
    m_pPreSaleResult->clear();
}

void SPreSaleEO::calcPreSale(const SNetRepoInfos& oNetRepoInfos)
{
    freePreSaleResult();
    for (auto pIter = oNetRepoInfos.begin(); pIter != oNetRepoInfos.end(); ++pIter)
    {
        SNetRepoInfo* pNetRepoInfo = *pIter;
        if (isPreSale(pNetRepoInfo->sGoodNum) && pNetRepoInfo->nCanMoved > 0)
        {
            SPreSaleInfo* pPreSaleInfo = new SPreSaleInfo;
            pPreSaleInfo->sBarCode = pNetRepoInfo->sBarCode;
            pPreSaleInfo->sGoodNum = pNetRepoInfo->sGoodNum;
            pPreSaleInfo->nCanUsed = pNetRepoInfo->nCanUsed;
            pPreSaleInfo->nCanMoved = pNetRepoInfo->nCanMoved;
            m_pPreSaleResult->push_back(pPreSaleInfo);
        }
    }
}

const SPreSaleResult* SPreSaleEO::preSaleResult() const
{
    return m_pPreSaleResult;
}

