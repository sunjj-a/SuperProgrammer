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
        //�ճ�����
        if (!loadSaleRecord(sErrorInfo))
            return false;

        if (!loadGoodState(sErrorInfo))
            return false;

        if (!loadNetRepo(sErrorInfo))
            return false;

        calcDailyReplenish();
    }

    {
        //�¿��
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
        sErrorInfo += QStringLiteral("���ۼ�¼�ļ���ʧ��\n");
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
        sErrorInfo += QStringLiteral("�����ı�ļ���ʧ��\n");
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
        sErrorInfo += QStringLiteral("���ּ�¼�ļ���ʧ��\n");
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
    oOrderStates.insert(QStringLiteral("���׳ɹ�"));
    oOrderStates.insert(QStringLiteral("�����ѷ������ȴ����ȷ��"));
    oOrderStates.insert(QStringLiteral("����Ѹ���ȴ����ҷ���"));
    m_pSaleRecordEO->findSaleInfo(oOrderStates, oSaleGoods);

    SGoodStates oGoodStates;
    m_pGoodsStateEO->calcGoodNum(oGoodStates);

    SNetRepoInfos oNetRepoInfos;
    SSellerSet oSellerSet;
    oSellerSet.insert(QStringLiteral("��̨��ɭ��װ���޹�˾"));
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
        sErrorInfo += QStringLiteral("Ԥ�۹����ļ���ʧ��\n");
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
    oSellerSet.insert(QStringLiteral("��̨��ɭ��װ���޹�˾"));
    m_pNetRepoEO->findNetRepoInfo(oSellerSet, oNetRepoInfos);
    m_pPreSaleEO->calcPreSale(oNetRepoInfos);
    exportPreSaleProduct(m_pPreSaleEO->preSaleResult());
    return true;
}

void SCalcRepoService::exportPreSaleProduct(const SPreSaleResult* pPreSaleResult)
{
    QString sDailyReplenishName = QStringLiteral("�¿�� ") + QDateTime::currentDateTime().toString("yyyy-MM-dd HH-mm-ss") + ".xlsx";
    QString sExportFilePath = m_pInputContext->sOutputDirPath + "\\" + sDailyReplenishName;

    //���ҳǩ����
    QXlsx::Document oXlsx;
    oXlsx.deleteSheet(DEFAULT_SHEET);
    oXlsx.addSheet(DEFAULT_SHEET);
    oXlsx.selectSheet(DEFAULT_SHEET);

    //���ñ���ͷ��Ϣ
    oXlsx.write(1, 1,  QStringLiteral("����"));
    oXlsx.write(1, 2,  QStringLiteral("��Ʒ����"));
    oXlsx.write(1, 3,  QStringLiteral("������"));
    oXlsx.write(1, 4,  QStringLiteral("�ɷ�����"));

    //���ñ���ͷ����
    QXlsx::Format oHeaderFormat;                             
    oHeaderFormat.setFontName(QStringLiteral("����"));
    oHeaderFormat.setFontSize(11);                                     
    oHeaderFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);    
    oHeaderFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    oXlsx.setRowFormat(1, oHeaderFormat);
    oXlsx.setColumnWidth(1, 4, 16);
    oXlsx.setRowHeight(1, 1, 30);

    //��ӵ�Ԫ������
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

    //���浽Excel����
    oXlsx.saveAs(sExportFilePath);
}

void SCalcRepoService::exportDailyReplenish(const SProductStates* pProductStates)
{
    QString sDailyReplenishName = QStringLiteral("�ճ����� ") + QDateTime::currentDateTime().toString("yyyy-MM-dd HH-mm-ss") + ".xlsx";
    QString sExportFilePath = m_pInputContext->sOutputDirPath + "\\" + sDailyReplenishName;

    //���ҳǩ����
    QXlsx::Document oXlsx;
    oXlsx.deleteSheet(DEFAULT_SHEET);
    oXlsx.addSheet(DEFAULT_SHEET);
    oXlsx.selectSheet(DEFAULT_SHEET);

    //���ñ���ͷ��Ϣ
    oXlsx.write(1, 1,  QStringLiteral("����"));
    oXlsx.write(1, 2,  QStringLiteral("��Ʒ����"));
    oXlsx.write(1, 3,  QStringLiteral("������"));
    oXlsx.write(1, 4,  QStringLiteral("�ɷ�����"));
    oXlsx.write(1, 5,  QStringLiteral("��Ʒ����״̬"));
    oXlsx.write(1, 6,  QStringLiteral("������"));
    oXlsx.write(1, 7,  QStringLiteral("������"));
    oXlsx.write(1, 8,  QStringLiteral("�¼�ֵ"));
    oXlsx.write(1, 9,  QStringLiteral("ȱ����"));
    oXlsx.write(1, 10,  QStringLiteral("ʵ��������"));

    //���ñ���ͷ����
    QXlsx::Format oHeaderFormat;                             
    oHeaderFormat.setFontName(QStringLiteral("����"));
    oHeaderFormat.setFontSize(11);                                     
    oHeaderFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);    
    oHeaderFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    oXlsx.setRowFormat(1, oHeaderFormat);
    oXlsx.setColumnWidth(1, 10, 16);
    oXlsx.setRowHeight(1, 1, 30);

    //��ӵ�Ԫ������
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

    //���浽Excel����
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

   oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�������")));
   oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("����")));
   oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�۸�")));
   oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("��������")));
   oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�ⲿϵͳ���")));
   oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("��Ʒ����")));
   oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�ײ���Ϣ")));
   oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("��ע")));
   oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("����״̬")));
   oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�̼ұ���")));
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

    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�����ն�")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("��Ʒid")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("��Ʒ����")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("��Ʒ����״̬")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("��Ʒ����")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�����")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�ÿ���")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("ƽ��ͣ��ʱ��")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("����ҳ������")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�µ�ת����")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�µ�֧��ת����")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("֧��ת����")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�µ����")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�µ���Ʒ����")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�µ������")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("֧�����")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("֧����Ʒ����")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�ӹ�����")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�ÿ�ƽ����ֵ")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�������")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�����")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�ع���")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�ղ�����")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("��������֧�������")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�͵���")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("����֧��ת����")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("���������ÿ���")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("֧�������")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�����ۺ�ɹ��˿���")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�����ۺ�ɹ��˿����")));
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

    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("��λ����")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("����")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("��Ʒ����")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("������")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("�ɷ�����")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("������")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("���ϸ���")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("����Id")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("����")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("��Ʒ����")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("������")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("���")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("��������ukid")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("��С��λ�����")));
    oColumnOrders.push_back(oColNames.indexOf(QStringLiteral("��ƷƷ��")));
}
