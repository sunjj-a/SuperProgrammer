#include "OddsMonitorWarning.h"

#include <QUrl>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QGroupBox>
#include <QSettings>
#include <QCoreApplication>

#define COlCOUNT 3
#define ROWCOUNT 5


//////////////////////////////////////////////////////////////////////////
//构造函数
OddsMonitorWarning::OddsMonitorWarning(QWidget *parent)
    : QDialog(parent), m_pMonitorThread(nullptr)
{
    //SetLogInformaiton(true, "E://Recognize.html", 1);
    initUIForm();
    initConnect();

    resize(745, 300);
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);  
    setWindowIcon(QIcon(":/OddsMonitorWarning/Resources/VoiceInputBundle.ico"));
    setWindowTitle(QStringLiteral("赔率监控 V1.1 猪八戒网 Super程序猿"));
}

//////////////////////////////////////////////////////////////////////////
//析构函数
OddsMonitorWarning::~OddsMonitorWarning()
{

}

//////////////////////////////////////////////////////////////////////////
//初始化UI窗体
void OddsMonitorWarning::initUIForm()
{
    m_pTableWidget = new QTableWidget(this);
    m_pTableWidget->setColumnCount(COlCOUNT);
    m_pTableWidget->setRowCount(ROWCOUNT);

    m_pTableWidget->setColumnWidth(0, 520);
    m_pTableWidget->setColumnWidth(1, 65);
    m_pTableWidget->setColumnWidth(2, 95);
    setWidgetHeader();

    m_pStartMonitor = new QPushButton(QStringLiteral("开始监控"));
    m_pStopMonitor = new QPushButton(QStringLiteral("暂停监控"));
    m_pClearMonitor = new QPushButton(QStringLiteral("清空数据"));
    m_pShowUserSetting = new QPushButton(QStringLiteral("隐藏设置"));
    m_pStopMonitor->setEnabled(false);

    m_pUserSettingBox = new QGroupBox(QStringLiteral("用户设置"), this);
    QHBoxLayout* pSettingBox = new QHBoxLayout;
    QLabel* pWarningUserLbl = new QLabel(QStringLiteral("预警用户"));
    m_pWarningUserEdt = new QLineEdit();
    QLabel* pWarningIntervalLbl = new QLabel(QStringLiteral("刷新间隔(分钟)"));
    m_pWarningIntervalEdt = new QLineEdit();
    pSettingBox->addWidget(pWarningUserLbl);
    pSettingBox->addWidget(m_pWarningUserEdt);
    pSettingBox->addStretch();
    pSettingBox->addWidget(pWarningIntervalLbl);
    pSettingBox->addWidget(m_pWarningIntervalEdt);
    m_pUserSettingBox->setLayout(pSettingBox);

    UserSetting oUserSetting = monitorUserSetting();
    m_pWarningUserEdt->setText(oUserSetting.sWarningUser);
    m_pWarningIntervalEdt->setText(QString::number(oUserSetting.dMonitorInterval));

    QHBoxLayout *pHBottomBox = new QHBoxLayout;
    pHBottomBox->addWidget(m_pStartMonitor);
    pHBottomBox->addWidget(m_pStopMonitor);
    pHBottomBox->addWidget(m_pClearMonitor);
    pHBottomBox->addWidget(m_pShowUserSetting);

    QVBoxLayout *pMainVBox = new QVBoxLayout;
    pMainVBox->addWidget(m_pTableWidget);
    pMainVBox->addLayout(pHBottomBox);
    pMainVBox->addLayout(pHBottomBox);
    pMainVBox->addWidget(m_pUserSettingBox);
    setLayout(pMainVBox);
}

//////////////////////////////////////////////////////////////////////////
//初始化信号槽
void OddsMonitorWarning::initConnect()
{
    connect(m_pStartMonitor, SIGNAL(clicked()), this, SLOT(startMonitor()));
    connect(m_pStopMonitor, SIGNAL(clicked()), this, SLOT(stopMonitor()));
    connect(m_pClearMonitor, SIGNAL(clicked()), this, SLOT(clearMonitor()));
    connect(m_pShowUserSetting, SIGNAL(clicked()), this, SLOT(showUserSetting()));
}

//////////////////////////////////////////////////////////////////////////
//开始监控
void OddsMonitorWarning::startMonitor()
{
    OmwMonitorContainer oMonitorContainer;
    bool bSccuess = loadMonitorInfo(oMonitorContainer);
    if (!bSccuess || oMonitorContainer.empty())
    {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请输入有效的监控网站或预警值(小数)"));
        return;
    }

    bSccuess = checkUserSetting();
    if (!bSccuess)
    {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请输入有效的邮件地址或刷新间隔(正整数)"));
        return;
    }

    if (!m_pMonitorThread)
    {
        saveUserSetting();
        m_pMonitorThread = new OmwMonitorThread(monitorUserSetting());
        m_pMonitorThread->setMonitorInfo(oMonitorContainer);
        m_pMonitorThread->start();
        
        m_pStartMonitor->setEnabled(false);
        m_pStopMonitor->setEnabled(true);
        m_pClearMonitor->setEnabled(false);
    }
}

//////////////////////////////////////////////////////////////////////////
//暂停监控
void OddsMonitorWarning::stopMonitor()
{
    if (m_pMonitorThread)
    {
        m_pMonitorThread->setStop(true);
        while (!m_pMonitorThread->isFinished())
        {}

        if(m_pMonitorThread->stoped())
        {
            delete m_pMonitorThread;
            m_pMonitorThread = nullptr;

            m_pStartMonitor->setEnabled(true);
            m_pStopMonitor->setEnabled(false);
            m_pClearMonitor->setEnabled(true);
        }
    }
}

//////////////////////////////////////////////////////////////////////////
//加载监控信息
bool OddsMonitorWarning::loadMonitorInfo(OmwMonitorContainer& oMonitorContainer)
{
    bool bValidMonitorInfo = true;
    QTableWidgetItem *pUrlItem(nullptr), *pDataItem(nullptr);
    oMonitorContainer.clear();
    for (int nIndex = 0; nIndex < ROWCOUNT; ++nIndex)
    {
        pUrlItem = m_pTableWidget->item(nIndex, 0);
        pDataItem = m_pTableWidget->item(nIndex, 1);

        if (!pUrlItem && !pDataItem)
        {
            continue;
        }

        if (!pUrlItem || !pDataItem)
        {
            bValidMonitorInfo = false;
            continue;
        }
            
        QString sMonitorUrl = pUrlItem->text();
        if (!QUrl(sMonitorUrl).isValid())
        {
            bValidMonitorInfo = false;
            continue;
        }

        bool bSccuess(false);
        double dWarningValue = pDataItem->text().toDouble(&bSccuess);
        if (!bSccuess)
        {
            bValidMonitorInfo = false;
            continue;
        }

        MonitorWebInfo oMonitorWebInfo;
        oMonitorWebInfo.sMonitorUrl = sMonitorUrl;
        oMonitorWebInfo.dWarningValue = dWarningValue;
        oMonitorContainer.append(oMonitorWebInfo);
    }

    return bValidMonitorInfo;
}

//////////////////////////////////////////////////////////////////////////
//清空监控数据
void OddsMonitorWarning::clearMonitor()
{
    m_pTableWidget->clear();
    setWidgetHeader();
}

//////////////////////////////////////////////////////////////////////////
//设置表格样式
void OddsMonitorWarning::setWidgetHeader()
{
    QStringList oHHeaderItems, oVHeaderItems;
    oHHeaderItems << QStringLiteral("监控网址") << QStringLiteral("预警值") << QStringLiteral("备注"); 
    oVHeaderItems << QStringLiteral("比赛1") << QStringLiteral("比赛2") << QStringLiteral("比赛3") << QStringLiteral("比赛4") << QStringLiteral("比赛5");
    m_pTableWidget->setHorizontalHeaderLabels(oHHeaderItems);
    m_pTableWidget->setVerticalHeaderLabels(oVHeaderItems);
}

//////////////////////////////////////////////////////////////////////////
//网站监控间隔 用户配置
UserSetting OddsMonitorWarning::monitorUserSetting()
{
    UserSetting oUserSetting;
    QString sUserSetting = qApp->applicationDirPath() + "/UserSetting.ini";
    QSettings oSettings(sUserSetting, QSettings::IniFormat);
    oUserSetting.dMonitorInterval = oSettings.value("/MonitorSetting/TimeInterval").toUInt();
    oUserSetting.sWarningUser = oSettings.value("/MonitorSetting/WarningUser").toString();
    return oUserSetting;
}

//////////////////////////////////////////////////////////////////////////
//显示用户设置
void OddsMonitorWarning::showUserSetting()
{
    m_pUserSettingBox->setVisible(!m_pUserSettingBox->isVisible());
    if (m_pUserSettingBox->isVisible())
    {
        m_pShowUserSetting->setText(QStringLiteral("隐藏设置"));
    }
    else
    {
        m_pShowUserSetting->setText(QStringLiteral("显示设置"));
    }
}

//////////////////////////////////////////////////////////////////////////
//检查用户设置
bool OddsMonitorWarning::checkUserSetting()
{
    if (m_pWarningUserEdt->text().isEmpty())
        return false;

    bool bSuccess = false;
    m_pWarningIntervalEdt->text().toInt(&bSuccess);
    if (!bSuccess)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////
//保存用户设置
void OddsMonitorWarning::saveUserSetting()
{
    QString sUserSetting = qApp->applicationDirPath() + "/UserSetting.ini";
    QSettings oSettings(sUserSetting, QSettings::IniFormat);
    oSettings.setValue("/MonitorSetting/WarningUser", m_pWarningUserEdt->text());
    oSettings.setValue("/MonitorSetting/TimeInterval", m_pWarningIntervalEdt->text().toInt());
}
