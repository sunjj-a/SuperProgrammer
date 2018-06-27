#include "RepoManager.h"
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QGridLayout>
#include <QMessageBox>
#include <QDir>
#include <QDirIterator>
#include "SCalcRepoService.h"

RepoManager::RepoManager(QWidget *parent)
    : QDialog(parent)
{
    initUI();
    initConnect();
    setWindowTitle(QStringLiteral("库存管理---她池专用"));
    setWindowIcon(QIcon(":/RepoManager/Appication.ico"));
}

RepoManager::~RepoManager()
{

}

void RepoManager::initUI()
{
    m_pSourceDirLbl = new QLabel(QStringLiteral("原始文件夹："));
    m_pSourceDirEdt = new QLineEdit();
    m_pSourceDirBtn = new QPushButton(QStringLiteral("选择"));
    m_pSourceDirBtn->setIcon(QIcon(":/RepoManager/Iimport.ico"));
    m_pSourceDirBtn->setCursor(Qt::PointingHandCursor);

    m_pDestDirLbl = new QLabel(QStringLiteral("目标文件夹："));
    m_pDestDirEdt = new QLineEdit;
    m_pDestDirBtn = new QPushButton(QStringLiteral("选择"));
    m_pDestDirBtn->setIcon(QIcon(":/RepoManager/Iimport.ico"));
    m_pDestDirBtn->setCursor(Qt::PointingHandCursor);

    m_pCalcCircleLbl = new QLabel(QStringLiteral("需求量计算周期："));
    m_pCalcCircleEdt = new QLineEdit("3");
    m_pCalcRepoBtn = new QPushButton(QStringLiteral("计算"));
    m_pCalcRepoBtn->setIcon(QIcon(":/RepoManager/Calculator.ico"));
    m_pCalcRepoBtn->setCursor(Qt::PointingHandCursor);

    m_pRemoveNumLbl = new QLabel(QStringLiteral("新款默认下架值："));
    m_pRemoveNumEdt = new QLineEdit("5");
    m_pResetBtn = new QPushButton(QStringLiteral("重置"));
    m_pResetBtn->setIcon(QIcon(":/RepoManager/Reset.ico"));
    m_pResetBtn->setCursor(Qt::PointingHandCursor);

    QGridLayout* pGridLayout = new QGridLayout;
    pGridLayout->addWidget(m_pSourceDirLbl, 1, 1, 1, 1);
    pGridLayout->addWidget(m_pSourceDirEdt, 1, 2, 1, 2);
    pGridLayout->addWidget(m_pSourceDirBtn, 1, 4, 1, 1, Qt::AlignCenter);

    pGridLayout->addWidget(m_pDestDirLbl, 2, 1, 1, 1);
    pGridLayout->addWidget(m_pDestDirEdt, 2, 2, 1, 2);
    pGridLayout->addWidget(m_pDestDirBtn, 2, 4, 1, 1, Qt::AlignCenter);

    pGridLayout->addWidget(m_pCalcCircleLbl, 3, 1, 1, 1);
    pGridLayout->addWidget(m_pCalcCircleEdt, 3, 2, 1, 2);
    pGridLayout->addWidget(m_pCalcRepoBtn, 3, 4, 1, 1, Qt::AlignCenter);

    pGridLayout->addWidget(m_pRemoveNumLbl, 4, 1, 1, 1);
    pGridLayout->addWidget(m_pRemoveNumEdt, 4, 2, 1, 2);
    pGridLayout->addWidget(m_pResetBtn, 4, 4, 1, 1, Qt::AlignCenter);
    setLayout(pGridLayout);
    setFixedSize(600, 140);
}

void RepoManager::initConnect()
{
    connect(m_pSourceDirBtn, SIGNAL(clicked()), this, SLOT(selSourceDir()));
    connect(m_pDestDirBtn, SIGNAL(clicked()), this, SLOT(selDestDir()));
    connect(m_pCalcRepoBtn, SIGNAL(clicked()), this, SLOT(beginCalcRepo()));
    connect(m_pResetBtn, SIGNAL(clicked()), this, SLOT(resetSetting()));
}

void RepoManager::resetSetting()
{
    m_pSourceDirEdt->clear();
    m_pDestDirEdt->clear();
    m_pCalcCircleEdt->setText("3");
    m_pRemoveNumEdt->setText("5");
}

void RepoManager::selSourceDir()
{
    QString sSourceDir = QFileDialog::getExistingDirectory(this, QStringLiteral("原始文件夹"));
    if (!sSourceDir.isEmpty())
        m_pSourceDirEdt->setText(sSourceDir);
}

void RepoManager::selDestDir()
{
    QString sDestDir = QFileDialog::getExistingDirectory(this, QStringLiteral("目标文件夹"));
    if (!sDestDir.isEmpty())
        m_pDestDirEdt->setText(sDestDir);
}

void RepoManager::beginCalcRepo()
{
    QString sSourceDir = m_pSourceDirEdt->text();
    if (!QDir(sSourceDir).exists())
    {
        hintInfo(QStringLiteral("原始文件夹不存在"));
        return;
    }

    QString sDestDir = m_pDestDirEdt->text();
    if (!QDir(sDestDir).exists())
    {
        hintInfo(QStringLiteral("目标文件夹不存在"));
        return;
    }

    bool bCircleSccuess = false;
    int nCalcCircleCount = m_pCalcCircleEdt->text().toInt(&bCircleSccuess);
    if (!bCircleSccuess || nCalcCircleCount < 0)
    {
        hintInfo(QStringLiteral("需求量计算周期不合法"));
        return;
    }

    int nRemoveNum = m_pRemoveNumEdt->text().toInt(&bCircleSccuess);
    if (!bCircleSccuess || nRemoveNum < 0)
    {
        hintInfo(QStringLiteral("新款默认下架值不合法"));
        return;
    }
    
    SInputContext oInputContext;
    oInputContext.nRemoveCount = nRemoveNum;
    oInputContext.nCalcCircleCount = nCalcCircleCount;
    oInputContext.sOutputDirPath = sDestDir;

    QStringList oFilter;
    oFilter << ("*.xls") << ("*.csv");
    QDirIterator oDirIterator(sSourceDir, oFilter,  QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
    while (oDirIterator.hasNext())
    {
        oDirIterator.next();
        QString sFileName = oDirIterator.fileName();
        QString sFilePath = oDirIterator.fileInfo().absoluteFilePath();
        if (sFileName.contains(QStringLiteral("生意参谋")))
        {
            oInputContext.sGoodsStateFile = sFilePath;
        } 
        else if (sFileName.contains(QStringLiteral("ExportOrderDetailList")))
        {
            oInputContext.sSaleRecordFile = sFilePath;
        }
        else if (sFileName.contains(QStringLiteral("导出数据")))
        {
            oInputContext.sNetRepoFile = sFilePath;
        }
        else if (sFileName.contains(QStringLiteral("预售管理表")))
        {
            oInputContext.sPreSaleFile = sFilePath;
        }
    }

    if (oInputContext.sGoodsStateFile.isEmpty() ||
        oInputContext.sNetRepoFile.isEmpty() ||
        oInputContext.sSaleRecordFile.isEmpty() ||
        oInputContext.sPreSaleFile.isEmpty())
    {
        hintInfo(QStringLiteral("原始文件缺失"));
        return;
    }

    QString sErrorInfo;
    bool bCalcSccuess = SCalcRepoService(&oInputContext).calcRepo(sErrorInfo);
    if (!bCalcSccuess)
    {
        hintInfo(sErrorInfo);
        return;
    }

    hintInfo(QStringLiteral("计算完毕^_^"));
}

void RepoManager::hintInfo(const QString& sErrorInfo)
{
    QMessageBox::information(this, QStringLiteral("提示"), sErrorInfo);
}
