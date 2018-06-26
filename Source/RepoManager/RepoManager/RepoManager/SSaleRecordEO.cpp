#include "SSaleRecordEO.h"
#include <assert.h>
#define COL_COUNT 10 

SSaleRecordEO::SSaleRecordEO(const SColumnOrders& oColOrders, const SSourceInfos& oSourceInfos)
{
    m_pSaleRecords = new SSaleRecords;
    fillSaleInfos(oColOrders, oSourceInfos);
}

SSaleRecordEO::~SSaleRecordEO()
{
    freeSaleInfo();

    delete m_pSaleRecords;
    m_pSaleRecords = nullptr;
}

void SSaleRecordEO::findSaleInfo(const SOrderStates& oOrderStates, SSaleRecords& oSaleInfos) const
{
    oSaleInfos.clear();
    for (auto pIter = m_pSaleRecords->begin(); pIter != m_pSaleRecords->end(); ++pIter)
    {
        SSaleInfo* pSaleInfo = *pIter;
        if (oOrderStates.contains(pSaleInfo->sState))
        {
            oSaleInfos.push_back(pSaleInfo);
            continue;
        }
    }
}

void SSaleRecordEO::findSaleInfo(const SOrderStates& oOrderStates, SSaleGoods& oSaleGoods) const
{
    SSaleRecords oSaleRecords;
    findSaleInfo(oOrderStates, oSaleRecords);

    oSaleGoods.clear();
    for (auto pIter = oSaleRecords.begin(); pIter != oSaleRecords.end(); ++pIter)
    {
        SSaleInfo* pSaleInfo = *pIter;
        auto pFindIter = oSaleGoods.find(pSaleInfo->sSystemNum);
        if (pFindIter != oSaleGoods.end())
        {
            pFindIter.value() += 1;
        }
        else
        {
            oSaleGoods.insert(pSaleInfo->sSystemNum, 1);
        }
    }
}

void SSaleRecordEO::fillSaleInfos(const SColumnOrders& oColOrders, const SSourceInfos& oSourceInfos)
{
    if (oColOrders.size() != COL_COUNT)
    {
        assert(false);
        return;
    }
    
    freeSaleInfo();
    for (auto pIter = oSourceInfos.begin(); pIter != oSourceInfos.end(); ++pIter)
    {
        QStringList oRecords = *pIter;
        SSaleInfo* pSaleInfo = new SSaleInfo;
        pSaleInfo->sOrderNum = oRecords.at(0);
        pSaleInfo->sTitile = oRecords.at(1);
        pSaleInfo->dPrice = oRecords.at(2).toDouble();
        pSaleInfo->nBuyNum = oRecords.at(3).toInt();
        pSaleInfo->sSystemNum = oRecords.at(4);
        pSaleInfo->sProperty = oRecords.at(5);
        pSaleInfo->sTCInfo = oRecords.at(6);
        pSaleInfo->sRemark = oRecords.at(7);
        pSaleInfo->sState = oRecords.at(8);
        pSaleInfo->sSellerNum = oRecords.at(9);
        m_pSaleRecords->push_back(pSaleInfo);
    }
}

void SSaleRecordEO::freeSaleInfo()
{
    for (auto pIter = m_pSaleRecords->begin(); pIter != m_pSaleRecords->end(); ++pIter)
    {
        SSaleInfo* pSaleInfo = *pIter;
        delete pSaleInfo;
        pSaleInfo = nullptr;
    }
    m_pSaleRecords->clear();
}


