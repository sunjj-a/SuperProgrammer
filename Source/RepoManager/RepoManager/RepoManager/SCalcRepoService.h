#pragma once
//////////////////////////////////////////////////////////////////////////
#include <QString>
#include "SModelConst.h"
#include "SDailyReplenishEO.h"
#include "SPreSaleEO.h"

struct SInputContext
{
    QString sGoodsStateFile;
    QString sSaleRecordFile;
    QString sNetRepoFile;
    QString sPreSaleFile;
    QString sOutputDirPath;
    int nCalcCircleCount;
    int nRemoveCount;
};

class SPreSaleEO;
class SNetRepoEO;
class SSaleRecordEO;
class SGoodsStateEO;
class SDailyReplenishEO;
class SXLSFileOper;

class SCalcRepoService
{
public:
    SCalcRepoService(const SInputContext* pInputContext);
    ~SCalcRepoService();

    bool calcRepo(QString& sErrorInfo);

private:
    bool loadSaleRecord(QString& sErrorInfo);
    bool loadGoodState(QString& sErrorInfo);
    bool loadNetRepo(QString& sErrorInfo);
    bool calcDailyReplenish();
    void exportDailyReplenish(const SProductStates* pProductStates);

    bool loadPreSales(QString& sErrorInfo);
    bool calcPreSaleProduct();
    void exportPreSaleProduct(const SPreSaleResult* pPreSaleResult);

    void initColumnOrders(int nColCount, SColumnOrders& oColumnOrders);
    void freeRecordsEO();


private:
    void calcSaleRecordCol(const QStringList& oSourceCol, SColumnOrders& oColumnOrders);
    void calcGoodStateCol(const QStringList& oSourceCol, SColumnOrders& oColumnOrders);
    void calcNetRepoCol(const QStringList& oSourceCol, SColumnOrders& oColumnOrders);

private:
    const SInputContext* m_pInputContext;

    SXLSFileOper* m_pXLSFileOper;
    SPreSaleEO* m_pPreSaleEO;
    SNetRepoEO* m_pNetRepoEO;
    SSaleRecordEO* m_pSaleRecordEO;
    SGoodsStateEO* m_pGoodsStateEO;
    SDailyReplenishEO* m_pDailyReplenishEO;
};