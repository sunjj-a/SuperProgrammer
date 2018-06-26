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
        pNetRepoInfo->s000 = oRecords.at(0);
        pNetRepoInfo->sBarCode = oRecords.at(1);
        pNetRepoInfo->sGoodNum = oRecords.at(2);
        pNetRepoInfo->nCanUsed = oRecords.at(3).toInt();
        pNetRepoInfo->nCanMoved  = oRecords.at(4).toInt();
        pNetRepoInfo->s005 = oRecords.at(5);
        pNetRepoInfo->s006 = oRecords.at(6);
        pNetRepoInfo->sSellerID  = oRecords.at(7).toInt();
        pNetRepoInfo->sSellerName = oRecords.at(8);
        pNetRepoInfo->s009 = oRecords.at(9);
        pNetRepoInfo->s010 = oRecords.at(10);
        pNetRepoInfo->s011 = oRecords.at(11);
        pNetRepoInfo->s012 = oRecords.at(12);
        pNetRepoInfo->s013 = oRecords.at(13);
        pNetRepoInfo->s014 = oRecords.at(14);
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

