#include "ExchangeRepoData.h"
#include "SSAExcelBaseOper.h"
#include "xlsxdocument.h"

#include <QFileInfo>
#include <vector>

struct SSANetRepertoryData
{
public:
    QString sProductCode;     //商品编码
    int nRepertory;           //库存量
    QString sShipper;         //货主
    int nShipperId;           //货主Id

public:
    SSANetRepertoryData(const QString& productCode = "", int repertory = 0, const QString& shipper = "", int shipperId = 0)
        :sProductCode(productCode), nRepertory(repertory), sShipper(shipper), nShipperId(shipperId){};
};

typedef std::vector<SSANetRepertoryData> SSANetRepertoryDataContr;

void traslateExcelInfo(const QStringList& oRepoList)
{
    SSAExcelBaseOper oExcelBaseOper;
    for (auto pIter = oRepoList.begin(); pIter != oRepoList.end(); ++pIter)
    {
        QString sRepoExcelPath = *pIter;
        SSAExcelParams oExcelParams;
        oExcelParams.sExcelFilePath = sRepoExcelPath;
        oExcelParams.sCellRange = "A1:N";
        oExcelParams.sOpenSheetName = "EXPORT";

        SSANetRepertoryDataContr oRepertoryDataContr;
        QVariantList oRecords = oExcelBaseOper.records(oExcelParams, QString());
        for (auto pIter = oRecords.begin(); pIter != oRecords.end(); ++pIter)
        {
            QVariantList oRecord = (*pIter).toList();
            SSANetRepertoryData oRepertoryData;
            oRepertoryData.sProductCode = oRecord.at(1).toString();
            oRepertoryData.nRepertory = oRecord.at(4).toInt();
            oRepertoryData.sShipper = oRecord.at(8).toString();
            oRepertoryData.nShipperId = oRecord.at(7).toInt();
            oRepertoryDataContr.push_back(oRepertoryData);
        }

        //before 1/4/7/8
        //after  1/4/6/7
        //添加页签数据
        QXlsx::Document oXlsx;
        oXlsx.deleteSheet("EXPORT");
        oXlsx.addSheet("EXPORT");
        oXlsx.selectSheet("EXPORT");

        //设置标题头信息
        oXlsx.write(1, 1,  QStringLiteral("条码"));
        oXlsx.write(1, 4,  QStringLiteral("可分配数"));
        oXlsx.write(1, 6,  QStringLiteral("货主"));
        oXlsx.write(1, 7,  QStringLiteral("货主ID"));

        //添加单元格内容
        int nRowIndex = 2;
        for (auto pIter = oRepertoryDataContr.begin(); pIter != oRepertoryDataContr.end(); ++pIter, ++nRowIndex)
        {
            SSANetRepertoryData oNetRepertoryData = *pIter;
            oXlsx.write(nRowIndex, 1, oNetRepertoryData.sProductCode);
            oXlsx.write(nRowIndex, 4, oNetRepertoryData.nRepertory);
            oXlsx.write(nRowIndex, 6, oNetRepertoryData.sShipper);
            oXlsx.write(nRowIndex, 7, oNetRepertoryData.nShipperId);
        }

        QFileInfo oFileInfo(sRepoExcelPath);
        QString sFileName = oFileInfo.baseName() + QStringLiteral("_调整后.xlsx");
        QString sFilePath = oFileInfo.absoluteFilePath() + "/" + sFileName;
        oXlsx.saveAs(sFilePath);
    }
}


#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QDirIterator>

ExchangeRepoData::ExchangeRepoData(QWidget *parent)
    : QDialog(parent)
{
    //ui.setupUi(this);

    QLabel *pHintInfo = new QLabel(QStringLiteral("库存信息:"));
    m_pSelFileEdt = new QLineEdit();
    m_pSelFileBtn = new QPushButton(QStringLiteral("选择库存"));
    m_pBeginTransBtn = new QPushButton(QStringLiteral("开始转化"));

    connect(m_pSelFileBtn, SIGNAL(clicked()), this, SLOT(selFile()));
    connect(m_pBeginTransBtn, SIGNAL(clicked()), this, SLOT(translate()));

    QHBoxLayout *pMainBox = new QHBoxLayout;
    pMainBox->addWidget(pHintInfo);
    pMainBox->addWidget(m_pSelFileEdt);
    pMainBox->addWidget(m_pSelFileBtn);
    pMainBox->addWidget(m_pBeginTransBtn);
    setLayout(pMainBox);

    setWindowTitle(QStringLiteral("库存列调整"));
    setFixedSize(600, 50);
}

ExchangeRepoData::~ExchangeRepoData()
{

}

void ExchangeRepoData::selFile()
{
    QString sFilePath = QFileDialog::getExistingDirectory(this, QStringLiteral("库存信息"));
    if (!sFilePath.isEmpty())
        m_pSelFileEdt->setText(sFilePath);
}

void ExchangeRepoData::translate()
{
    QString sFilePath = m_pSelFileEdt->text();
    QStringList oRepoList;

    //获取所选文件类型过滤器
    QStringList filters;
    filters << QString("*.xls");

    //定义迭代器并设置过滤器
    QDirIterator dir_iterator(sFilePath,
        filters,
        QDir::Files | QDir::NoSymLinks,
        QDirIterator::Subdirectories);
    QStringList string_list;
    while(dir_iterator.hasNext())
    {
        dir_iterator.next();
        QFileInfo file_info = dir_iterator.fileInfo();
        oRepoList.push_back(file_info.absoluteFilePath());
    }

    traslateExcelInfo(oRepoList);
    QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("转化完成"));
}
