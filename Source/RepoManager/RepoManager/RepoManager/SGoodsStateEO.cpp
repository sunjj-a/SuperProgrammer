#include "SGoodsStateEO.h"
#include <assert.h>
#define COL_COUNT 30
#define MID_CALC_COUNT 9
#define UN_EXISTED "-" 

SGoodsStateEO::SGoodsStateEO(const SColumnOrders& oColOrders, const SSourceGoods& oSourceInfos)
    :m_pGoodRecords(nullptr), m_pGoodStates(nullptr)
{
    m_pGoodStates = new SGoodStates;
    m_pGoodRecords = new SGoodRecords;
}

SGoodsStateEO::~SGoodsStateEO()
{
    freeGoodRecords();

    delete m_pGoodRecords;
    m_pGoodRecords = nullptr;
    delete m_pGoodStates;
    m_pGoodStates = nullptr;
}

QString SGoodsStateEO::findOnlineState(const QString& sGoodNum) const
{
    return m_pGoodStates->value(sGoodNum, UN_EXISTED);
}

void SGoodsStateEO::calcGoodNum()
{
    for (auto pIter = m_pGoodRecords->begin(); pIter != m_pGoodRecords->end(); ++pIter)
    {
        SGoodInfo* pGoodInfo = *pIter;
        pGoodInfo->sMidCalc = pGoodInfo->sGoodTitle.right(MID_CALC_COUNT);
        pGoodInfo->sGoodNum = subGoodNum(pGoodInfo->sMidCalc);
        m_pGoodStates->insert(pGoodInfo->sGoodNum, pGoodInfo->sOnlineState);
    }
}

const SGoodRecords* SGoodsStateEO::goodRecords() const
{
    return m_pGoodRecords;
}

void SGoodsStateEO::freeGoodRecords()
{
    for (auto pIter = m_pGoodRecords->begin(); pIter != m_pGoodRecords->end(); ++pIter)
    {
        SGoodInfo* pGoodInfo = *pIter;
        delete pGoodInfo;
        pGoodInfo = nullptr;
    }
    m_pGoodRecords->clear();
}

void SGoodsStateEO::fillGoodRecords(const SColumnOrders& oColOrders, const SSourceGoods& oSourceInfos)
{
    if (oColOrders.size() != COL_COUNT)
    {
        assert(false);
        return;
    }

    freeGoodRecords();
    for (auto pIter = oSourceInfos.begin(); pIter != oSourceInfos.end(); ++pIter)
    {
        QStringList oRecords = pIter->toStringList();
        SGoodInfo* pGoodInfo = new SGoodInfo;
        pGoodInfo->s000 = oRecords.at(0);
        pGoodInfo->s001 = oRecords.at(1);
        pGoodInfo->sGoodTitle = oRecords.at(2);
        pGoodInfo->sOnlineState = oRecords.at(3);
        pGoodInfo->s004 = oRecords.at(4);
        pGoodInfo->s005 = oRecords.at(5);
        pGoodInfo->s006 = oRecords.at(6);
        pGoodInfo->s007 = oRecords.at(7);
        pGoodInfo->s008 = oRecords.at(8);
        pGoodInfo->s009 = oRecords.at(9);
        pGoodInfo->s010 = oRecords.at(10);
        pGoodInfo->s011 = oRecords.at(11);
        pGoodInfo->s012 = oRecords.at(12);
        pGoodInfo->s013 = oRecords.at(13);
        pGoodInfo->s014 = oRecords.at(14);
        pGoodInfo->s015 = oRecords.at(15);
        pGoodInfo->s016 = oRecords.at(16);
        pGoodInfo->s017 = oRecords.at(17);
        pGoodInfo->s018 = oRecords.at(18);
        pGoodInfo->s019 = oRecords.at(19);
        pGoodInfo->s020 = oRecords.at(20);
        pGoodInfo->s021 = oRecords.at(21);
        pGoodInfo->s022 = oRecords.at(22);
        pGoodInfo->s023 = oRecords.at(23);
        pGoodInfo->s024 = oRecords.at(24);
        pGoodInfo->s025 = oRecords.at(25);
        pGoodInfo->s026 = oRecords.at(26);
        pGoodInfo->s027 = oRecords.at(27);
        pGoodInfo->s028 = oRecords.at(28);
        pGoodInfo->s029 = oRecords.at(29);
        m_pGoodRecords->push_back(pGoodInfo);
    }
}

QString SGoodsStateEO::subGoodNum(const QString& sMidCalc)
{
    QString sGoodNum;
    for (int nIndex = 0; nIndex < sMidCalc.size(); ++nIndex)
    {
        QChar oChar = sMidCalc.at(nIndex);  
        ushort uUnicode = oChar.unicode();  
         if(oChar >= 0x4E00 && oChar <= 0x9FA5)  
             continue;

         sGoodNum += oChar;
    }
    return sGoodNum;
}

