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
    setWindowTitle(QStringLiteral("������---����ר��"));
    setWindowIcon(QIcon(":/RepoManager/Appication.ico"));
}

RepoManager::~RepoManager()
{

}

void RepoManager::initUI()
{
    m_pSourceDirLbl = new QLabel(QStringLiteral("ԭʼ�ļ��У�"));
    m_pSourceDirEdt = new QLineEdit();
    m_pSourceDirBtn = new QPushButton(QStringLiteral("ѡ��"));
    m_pSourceDirBtn->setIcon(QIcon(":/RepoManager/Iimport.ico"));
    m_pSourceDirBtn->setCursor(Qt::PointingHandCursor);

    m_pDestDirLbl = new QLabel(QStringLiteral("Ŀ���ļ��У�"));
    m_pDestDirEdt = new QLineEdit;
    m_pDestDirBtn = new QPushButton(QStringLiteral("ѡ��"));
    m_pDestDirBtn->setIcon(QIcon(":/RepoManager/Iimport.ico"));
    m_pDestDirBtn->setCursor(Qt::PointingHandCursor);

    m_pCalcCircleLbl = new QLabel(QStringLiteral("�������������ڣ�"));
    m_pCalcCircleEdt = new QLineEdit("3");
    m_pCalcRepoBtn = new QPushButton(QStringLiteral("����"));
    m_pCalcRepoBtn->setIcon(QIcon(":/RepoManager/Calculator.ico"));
    m_pCalcRepoBtn->setCursor(Qt::PointingHandCursor);

    m_pRemoveNumLbl = new QLabel(QStringLiteral("�¿�Ĭ���¼�ֵ��"));
    m_pRemoveNumEdt = new QLineEdit("5");
    m_pResetBtn = new QPushButton(QStringLiteral("����"));
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
    QString sSourceDir = QFileDialog::getExistingDirectory(this, QStringLiteral("ԭʼ�ļ���"));
    if (!sSourceDir.isEmpty())
        m_pSourceDirEdt->setText(sSourceDir);
}

void RepoManager::selDestDir()
{
    QString sDestDir = QFileDialog::getExistingDirectory(this, QStringLiteral("Ŀ���ļ���"));
    if (!sDestDir.isEmpty())
        m_pDestDirEdt->setText(sDestDir);
}

void RepoManager::beginCalcRepo()
{
    QString sSourceDir = m_pSourceDirEdt->text();
    if (!QDir(sSourceDir).exists())
    {
        hintInfo(QStringLiteral("ԭʼ�ļ��в�����"));
        return;
    }

    QString sDestDir = m_pDestDirEdt->text();
    if (!QDir(sDestDir).exists())
    {
        hintInfo(QStringLiteral("Ŀ���ļ��в�����"));
        return;
    }

    bool bCircleSccuess = false;
    int nCalcCircleCount = m_pCalcCircleEdt->text().toInt(&bCircleSccuess);
    if (!bCircleSccuess || nCalcCircleCount < 0)
    {
        hintInfo(QStringLiteral("�������������ڲ��Ϸ�"));
        return;
    }

    int nRemoveNum = m_pRemoveNumEdt->text().toInt(&bCircleSccuess);
    if (!bCircleSccuess || nRemoveNum < 0)
    {
        hintInfo(QStringLiteral("�¿�Ĭ���¼�ֵ���Ϸ�"));
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
        if (sFileName.contains(QStringLiteral("�����ı")))
        {
            oInputContext.sGoodsStateFile = sFilePath;
        } 
        else if (sFileName.contains(QStringLiteral("ExportOrderDetailList")))
        {
            oInputContext.sSaleRecordFile = sFilePath;
        }
        else if (sFileName.contains(QStringLiteral("��������")))
        {
            oInputContext.sNetRepoFile = sFilePath;
        }
        else if (sFileName.contains(QStringLiteral("Ԥ�۹����")))
        {
            oInputContext.sPreSaleFile = sFilePath;
        }
    }

    if (oInputContext.sGoodsStateFile.isEmpty() ||
        oInputContext.sNetRepoFile.isEmpty() ||
        oInputContext.sSaleRecordFile.isEmpty() ||
        oInputContext.sPreSaleFile.isEmpty())
    {
        hintInfo(QStringLiteral("ԭʼ�ļ�ȱʧ"));
        return;
    }

    QString sErrorInfo;
    bool bCalcSccuess = SCalcRepoService(&oInputContext).calcRepo(sErrorInfo);
    if (!bCalcSccuess)
    {
        hintInfo(sErrorInfo);
        return;
    }

    hintInfo(QStringLiteral("�������^_^"));
}

void RepoManager::hintInfo(const QString& sErrorInfo)
{
    QMessageBox::information(this, QStringLiteral("��ʾ"), sErrorInfo);
}
