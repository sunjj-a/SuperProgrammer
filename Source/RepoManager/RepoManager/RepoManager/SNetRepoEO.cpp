#include "SNetRepoEO.h"
#include <assert.h>
#define COL_COUNT 15

SNetRepoEO::SNetRepoEO(const SColumnOrders& oColOrders, const SSourceNetRepos& oSourceInfos)
    :m_pNetRepoInfos(nullptr)
{
    m_pNetRepoInfos = new SNetRepoInfos;
    fillNetRepoInfos(oColOrders, oSourceInfos);
}

SNetRepoEO::~SNetRepoEO()
{
    freeNetRepoInfos();

    delete m_pNetRepoInfos;
    m_pNetRepoInfos = nullptr;
}

void SNetRepoEO::findNetRepoInfo(const SSellerSet& oSellerSet, SNetRepoInfos& oNetRepoInfos) const
{
    oNetRepoInfos.clear();
    for (auto pIter = m_pNetRepoInfos->begin(); pIter != m_pNetRepoInfos->end(); ++pIter)
    {
        SNetRepoInfo* pNetRepoInfo = *pIter;
        if (oSellerSet.contains(pNetRepoInfo->sSellerName))
        {
            oNetRepoInfos.push_back(pNetRepoInfo);
            continue;
        }
    }
}

void SNetRepoEO::fillNetRepoInfos(const SColumnOrders& oColOrders, const SSourceNetRepos& oSourceInfos)
{
    if (oColOrders.size() != COL_COUNT)
    {
        assert(false);
        return;
    }

    for (auto pIter = oSourceInfos.begin(); pIter != oSourceInfos.end(); ++pIter)
    {
        QStringList oRecords = pIter->toStringList();
        SNetRepoInfo* pNetRepoInfo = new SNetRepoInfo;
        pNetRepoInfo->s000 = oRecords.at(oColOrders.at(0));
        pNetRepoInfo->sBarCode = oRecords.at(oColOrders.at(1));
        pNetRepoInfo->sGoodNum = oRecords.at(oColOrders.at(2));
        pNetRepoInfo->nCanUsed = oRecords.at(oColOrders.at(3)).toInt();
        pNetRepoInfo->nCanMoved  = oRecords.at(oColOrders.at(4)).toInt();
        pNetRepoInfo->s005 = oRecords.at(oColOrders.at(5));
        pNetRepoInfo->s006 = oRecords.at(oColOrders.at(6));
        pNetRepoInfo->sSellerID  = oRecords.at(oColOrders.at(7)).toInt();
        pNetRepoInfo->sSellerName = oRecords.at(oColOrders.at(8));
        pNetRepoInfo->s009 = oRecords.at(oColOrders.at(9));
        pNetRepoInfo->s010 = oRecords.at(oColOrders.at(10));
        pNetRepoInfo->s011 = oRecords.at(oColOrders.at(11));
        pNetRepoInfo->s012 = oRecords.at(oColOrders.at(12));
        pNetRepoInfo->s013 = oRecords.at(oColOrders.at(13));
        pNetRepoInfo->s014 = oRecords.at(oColOrders.at(14));
        m_pNetRepoInfos->push_back(pNetRepoInfo);
    }
}

void SNetRepoEO::freeNetRepoInfos()
{
    for (auto pIter = m_pNetRepoInfos->begin(); pIter != m_pNetRepoInfos->end(); ++pIter)
    {
        SNetRepoInfo* pNetRepoInfo = *pIter;
        delete pNetRepoInfo;
        pNetRepoInfo = nullptr;
    }
    m_pNetRepoInfos->clear();
}

