#include "OmwMonitorThread.h"
#include <QTimer>
#include <QDebug>
#include "OmwMonitorOper.h"

//////////////////////////////////////////////////////////////////////////
//构造函数
OmwMonitorThread::OmwMonitorThread(const UserSetting& oUserSetting)
    :m_pTimer(nullptr), m_bStopMonitor(false), m_oUserSetting(oUserSetting)
{
    m_pTimer = std::make_shared<QTimer>(this);
    connect(m_pTimer.get(), SIGNAL(timeout()), this, SLOT(monitorWarningOper()), Qt::QueuedConnection);
    m_pTimer->start(1000 * 60 * m_oUserSetting.dMonitorInterval); 
}

//////////////////////////////////////////////////////////////////////////
//析构函数
OmwMonitorThread::~OmwMonitorThread()
{
    m_pTimer = nullptr;
}

//////////////////////////////////////////////////////////////////////////
//设置监控信息
void OmwMonitorThread::setMonitorInfo(const OmwMonitorContainer& oMonitorContaienr)
{
    m_oMonitorContainer = oMonitorContaienr;
}

//////////////////////////////////////////////////////////////////////////
//线程启动
void OmwMonitorThread::run()
{
    while (!m_bStopMonitor)
    {
        qDebug() << "Runing..." << m_pTimer->remainingTime();
    }
}

//////////////////////////////////////////////////////////////////////////
//监控预警
void OmwMonitorThread::monitorWarningOper()
{
    OddsMonitorOper oOddsMonitorOper;
    for (auto pIter = m_oMonitorContainer.begin(); pIter != m_oMonitorContainer.end(); ++pIter)
    {
        MonitorWebInfo oMonitorInfo = *pIter;
        oOddsMonitorOper.oddsMonitor(oMonitorInfo.sMonitorUrl, oMonitorInfo.dWarningValue);
    }
    oOddsMonitorOper.sendEmail(m_oUserSetting.sWarningUser);
}

//////////////////////////////////////////////////////////////////////////
//设置结束
void OmwMonitorThread::setStop(bool bStopMonitor)
{
    m_bStopMonitor = bStopMonitor;
}

//////////////////////////////////////////////////////////////////////////
//是否结束
bool OmwMonitorThread::stoped()
{
    return m_bStopMonitor;
}
