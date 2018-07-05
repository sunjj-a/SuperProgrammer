#include "SCalcRepoService.h"
#include "SSaleRecordEO.h"
#include "SGoodsStateEO.h"
#include "SNetRepoEO.h"
#include "SDailyReplenishEO.h"

#include "csv/SCVSFileOper.h"
#include "xls/SXLSFileOper.h"
#include "xlsx/xlsxdocument.h"

#include <QDate>
#include <QDateTime>
#include <QDebug>

SCalcRepoService::SCalcRepoService(const SInputContext* pInputContext)
    :m_pInputContext(pInputContext), m_pNetRepoEO(nullptr), m_pSaleRecordEO(nullptr), 
    m_pGoodsStateEO(nullptr), m_pDailyReplenishEO(nullptr), m_pXLSFileOper(nullptr), m_pPreSaleEO(nullptr)
{
    m_pXLSFileOper = new SXLSFileOper;
}

SCalcRepoService::~SCalcRepoService()
{
    freeRecordsEO();
    freeAndNull(m_pXLSFileOper);
}

bool SCalcRepoService::calcRepo(QString& sErrorInfo)
{
    freeRecordsEO();

    {
        //日常补货
        if (!loadSaleRecord(sErrorInfo))
            return false;

        if (!loadGoodState(sErrorInfo))
            return false;

        if (!loadNetRepo(sErrorInfo))
            return false;

        calcDailyReplenish();
    }

    {
        //新款到货
        if (!loadPreSales(sErrorInfo))
            return false;

        calcPreSaleProduct();
    }

    return true;
}

bool SCalcRepoService::loadSaleRecord(QString& sErrorInfo)
{
    SCVSContainer oCVSContainer;
    if (!SCVSFileOper().readFromFile(m_pInputContext->sSaleRecordFile, oCVSContainer))
    {
        sErrorInfo += QStringLiteral("销售记录文件打开失败\n");
        return false;
    }

    SColumnOrders oColumnOrders;
    calcSaleRecordCol(oCVSContainer.first(), oColumnOrders);
    oCVSContainer.removeFirst();
    m_pSaleRecordEO = new SSaleRecordEO(oColumnOrders, oCVSContainer);
    return true;
}

bool SCalcRepoService::loadGoodState(QString& sErrorInfo)
{
    SXLSContainer oXLSContainer;
    SExcelParams oExcelParams;
    oExcelParams.sExcelFilePath = m_pInputContext->sGoodsStateFile;
    oExcelParams.nSheetIndex = FIRST_SHEET;
    oExcelParams.sCellRange = "A4:AD";
    if (!m_pXLSFileOper->readFromFile(oExcelParams, oXLSContainer))
    {
        sErrorInfo += QStringLiteral("生意参谋文件打开失败\n");
        return false;
    }

    SColumnOrders oColumnOrders;
    calcGoodStateCol(oXLSContainer.first().toStringList(), oColumnOrders);
    oXLSContainer.removeFirst();
    m_pGoodsStateEO = new SGoodsStateEO(oColumnOrders, oXLSContainer);
    return true;
}

bool SCalcRepoService::loadNetRepo(QString& sErrorInfo)
{
    SXLSContainer oXLSContainer;
    SExcelParams oExcelParams;
    oExcelParams.sExcelFilePath = m_pInputContext->sNetRepoFile;
    oExcelParams.nSheetIndex = FIRST_SHEET;
    oExcelParams.sCellRange = "A1:O";
    if (!m_pXLSFileOper->readFromFile(oExcelParams, oXLSContainer))
    {
        sErrorInfo += QStringLiteral("网仓记录文件打开失败\n");
        return false;
    }

    SColumnOrders oColumnOrders;
    calcNetRepoCol(oXLSContainer.first().toStringList(), oColumnOrders);
    oXLSContainer.removeFirst();
    m_pNetRepoEO = new SNetRepoEO(oColumnOrders, oXLSContainer);
    return true;
}

bool SCalcRepoService::calcDailyReplenish()
{
    SSaleGoods oSaleGoods;
    SOrderStates oOrderStates;
    oOrderStates.insert(QStringLiteral("交易成功"));
    oOrderStates.insert(QStringLiteral("卖家已发货，等待买家确认"));
    oOrderStates.insert(QStringLiteral("买家已付款，等待卖家发货"));
    m_pSaleRecordEO->findSaleInfo(oOrderStates, oSaleGoods);

    SGoodStates oGoodStates;
    m_pGoodsStateEO->calcGoodNum(oGoodStates);

    SNetRepoInfos oNetRepoInfos;
    SSellerSet oSellerSet;
    oSellerSet.insert(QStringLiteral("烟台拓森服装有限公司"));
    m_pNetRepoEO->findNetRepoInfo(oSellerSet, oNetRepoInfos);

    ReplenishContext oReplenishContext;
    oReplenishContext.nCalcCircleCount = m_pInputContext->nCalcCircleCount;
    oReplenishContext.nRemoveCount = m_pInputContext->nRemoveCount;

    SDailyReplenishEO oDailyReplenishEO(&oSaleGoods, &oGoodStates, &oNetRepoInfos);
    oDailyReplenishEO.calcDailyRepenish(oReplenishContext);
    const SProductStates* pProductStates = oDailyReplenishEO.productStates();

    exportDailyReplenish(pProductStates);
    return true;
}

bool SCalcRepoService::loadPreSales(QString& sErrorInfo)
{
    SXLSContainer oXLSContainer;
    SExcelParams oExcelParams;
    oExcelParams.sExcelFilePath = m_pInputContext->sPreSaleFile;
    oExcelParams.nSheetIndex = FIRST_SHEET;
    oExcelParams.sCellRange = "A1:A";
    if (!m_pXLSFileOper->readFromFile(oExcelParams, oXLSContainer))
    {
        sErrorInfo += QStringLiteral("预售管理文件打开失败\n");
        return false;
    }

    oXLSContainer.removeFirst();
    m_pPreSaleEO = new SPreSaleEO(oXLSContainer);
    return true;
}

bool SCalcRepoService::calcPreSaleProduct()
{
    SNetRepoInfos oNetRepoInfos;
    SSellerSet oSellerSet;
    oSellerSet.insert(QStringLiteral("烟台拓森服装有限公司"));
    m_pNetRepoEO->findNetRepoInfo(oSellerSet, oNetRepoInfos);
    m_pPreSaleEO->calcPreSale(oNetRepoInfos);
    exportPreSaleProduct(m_pPreSaleEO->preSaleResult());
    return true;
}

void SCalcRepoService::exportPreSaleProduct(const SPreSaleResult* pPreSaleResult)
{
    QString sDailyReplenishName = QStringLiteral("新款到货 ") + QDateTime::currentDateTime().toString("yyyy-MM-dd HH-mm-ss") + ".xlsx";
    QString sExportFilePath = m_pInputContext->sOutputDirPath + "\\" + sDailyReplenishName;

    //添加页签数据
    QXlsx::Document oXlsx;
    oXlsx.deleteSheet(DEFAULT_SHEET);
    oXlsx.addSheet(DEFAULT_SHEET);
    oXlsx.selectSheet(DEFAULT_SHEET);

    //设置标题头信息
    oXlsx.write(1, 1,  QStringLiteral("条码"));
    oXlsx.write(1, 2,  QStringLiteral("商品编码"));
    oXlsx.write(1, 3,  QStringLiteral("可用数"));
    oXlsx.write(1, 4,  QStringLiteral("可分配数"));

    //设置标题头属性
    QXlsx::Format oHeaderFormat;                             
    oHeaderFormat.setFontName(QStringLiteral("宋体"));
    oHeaderFormat.setFontSize(11);                                     
    oHeaderFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);    
    oHeaderFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    oXlsx.setRowFormat(1, oHeaderFormat);
    oXlsx.setColumnWidth(1, 4, 16);
    oXlsx.setRowHeight(1, 1, 30);

    //添加单元格内容
    int nRowIndex = 2;
    for (auto pIter = pPreSaleResult->begin(); pIter != pPreSaleResult->end(); ++pIter, ++nRowIndex)
    {
        SPreSaleInfo* pPreSaleInfo = *pIter;
        oXlsx.write(nRowIndex, 1, pPreSaleInfo->sBarCode);
        oXlsx.write(nRowIndex, 2, pPreSaleInfo->sGoodNum);
        oXlsx.write(nRowIndex, 3, QString::number(pPreSaleInfo->nCanUsed));
        oXlsx.write(nRowIndex, 4, QString::number(pPreSaleInfo->nCanMoved));
        oXlsx.setRowFormat(nRowIndex, oHeaderFormat);
    }

    //保存到Excel数据
    oXlsx.saveAs(sExportFilePath);
}

void SCalcRepoService::exportDailyReplenish(const SProductStates* pProductStates)
{
    QString sDailyReplenishName = QStringLiteral("日常补货 ") + QDateTime::currentDateTime().toString("yyyy-MM-dd HH-mm-ss") + ".xlsx";
    QString sExportFilePath = m_pInputContext->sOutputDirPath + "\\" + sDailyReplenishName;

    //添加页签数据
    QXlsx::Document oXlsx;
    oXlsx.deleteSheet(DEFAULT_SHEET);
    oXlsx.addSheet(DEFAULT_SHEET);
    oXlsx.selectSheet(DEFAULT_SHEET);

    //设置标题头信息
    oXlsx.write(1, 1,  QStringLiteral("条码"));
    oXlsx.write(1, 2,  QStringLiteral("商品编码"));
    oXlsx.write(1, 3,  QStringLiteral("可用数"));
    oXlsx.write(1, 4,  QStringLiteral("可分配数"));
    oXlsx.write(1, 5,  QStringLiteral("商品在线状态"));
    oXlsx.write(1, 6,  QStringLiteral("销售量"));
    oXlsx.write(1, 7,  QStringLiteral("需求量"));
    oXlsx.write(1, 8,  QStringLiteral("下架值"));
    oXlsx.write(1, 9,  QStringLiteral("缺货量"));
    oXlsx.write(1, 10,  QStringLiteral("实际留货量"));

    //设置标题头属性
    QXlsx::Format oHeaderFormat;                             
    oHeaderFormat.setFontName(QStringLiteral("宋体"));
    oHeaderFormat.setFontSize(11);                                     
    oHeaderFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);    
    oHeaderFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    oXlsx.setRowFormat(1, oHeaderFormat);
    oXlsx.setColumnWidth(1, 10, 16);
    oXlsx.setRowHeight(1, 1, 30);

    //添加单元格内容
    int nRowIndex = 2;
    for (auto pIter = pProductStates->begin(); pIter != pProductStates->end(); ++pIter, ++nRowIndex)
    {
        SProductState* pProductState = *pIter;
        oXlsx.write(nRowIndex, 1, pProductState->sBarCode);
        oXlsx.write(nRowIndex, 2, pProductState->sGoodNum);
        oXlsx.write(nRowIndex, 3, QString::number(pProductState->nCanUsed));
        oXlsx.write(nRowIndex, 4, QString::number(pProductState->nCanMoved));
        oXlsx.write(nRowIndex, 5, pProductState->sOnlineState);
        oXlsx.write(nRowIndex, 6, pProductState->sSaleNum);
        oXlsx.write(nRowIndex, 7, pProductState->sRequireNum);
        oXlsx.write(nRowIndex, 8, pProductState->sRemoveNum);
        oXlsx.write(nRowIndex, 9, pProductState->sShortNum);
        oXlsx.write(nRowIndex, 10, pProductState->sActualNum);
        oXlsx.setRowFormat(nRowIndex, oHeaderFormat);
    }

    //保存到Excel数据
    oXlsx.saveAs(sExportFilePath);
}

void SCalcRepoService::initColumnOrders(int nColCount, SColumnOrders& oColumnOrders)
{
    oColumnOrders.clear();
    for (int nIndex = 0; nIndex < nColCount; ++nIndex)
    {
        oColumnOrders.push_back(nIndex);
    }
}

void SCalcRepoService::freeRecordsEO()
{
    freeAndNull(m_pNetRepoEO);
    freeAndNull(m_pSaleRecordEO);
    freeAndNull(m_pGoodsStateEO);
    freeAndNull(m_pDailyReplenishEO);
    freeAndNull(m_pPreSaleEO);
}

void SCalcRepoService::calcSaleRecordCol(const QStringList& oSourceCol, SColumnOrders& oColumnOrders)
{
   oColumnOrders.clear();
   QVector<QString> oColNames;
   for (auto pIter = oSourceCol.begin(); pIter != oSourceCol.end(); ++pIter)
   {
       QString sColName = *pIter;
       oColNames.push_back(sColName.trimmed());
   }

   oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("订单编号")));
   oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("标题")));
   oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("价格")));
   oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("购买数量")));
   oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("外部系统编号")));
   oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("商品属性")));
   oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("套餐信息")));
   oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("备注")));
   oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("订单状态")));
   oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("商家编码")));
}

void SCalcRepoService::calcGoodStateCol(const QStringList& oSourceCol, SColumnOrders& oColumnOrders)
{
    oColumnOrders.clear();
    QVector<QString> oColNames;
    for (auto pIter = oSourceCol.begin(); pIter != oSourceCol.end(); ++pIter)
    {
        QString sColName = *pIter;
        oColNames.push_back(sColName.trimmed());
    }

    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("所属终端")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("商品id")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("商品标题")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("商品在线状态")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("商品链接")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("浏览量")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("访客数")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("平均停留时长")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("详情页跳出率")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("下单转化率")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("下单支付转化率")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("支付转化率")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("下单金额")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("下单商品件数")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("下单买家数")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("支付金额")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("支付商品件数")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("加购件数")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("访客平均价值")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("点击次数")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("点击率")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("曝光量")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("收藏人数")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("搜索引导支付买家数")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("客单价")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("搜索支付转化率")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("搜索引导访客数")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("支付买家数")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("售中售后成功退款金额")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("售中售后成功退款笔数")));
}

void SCalcRepoService::calcNetRepoCol(const QStringList& oSourceCol, SColumnOrders& oColumnOrders)
{
    oColumnOrders.clear();
    QVector<QString> oColNames;
    for (auto pIter = oSourceCol.begin(); pIter != oSourceCol.end(); ++pIter)
    {
        QString sColName = *pIter;
        oColNames.push_back(sColName.trimmed());
    }

    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("库位中数")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("条码")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("商品编码")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("可用数")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("可分配数")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("锁定数")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("不合格数")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("货主Id")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("货主")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("商品名称")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("规格编码")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("规格")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("导出排序ukid")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("最小单位库存数")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("商品品类")));
}
