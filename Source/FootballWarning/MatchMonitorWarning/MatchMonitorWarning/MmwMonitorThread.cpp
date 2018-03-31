#include "MmwMonitorThread.h"
#include <QTimer>
#include <QDebug>
#include "MmwMonitorOper.h"

//////////////////////////////////////////////////////////////////////////
//构造函数
MmwMonitorThread::MmwMonitorThread(const UserSetting& oUserSetting)
    :m_pTimer(nullptr), m_bStopMonitor(false), m_oUserSetting(oUserSetting)
{
    m_pTimer = std::make_shared<QTimer>(this);
    connect(m_pTimer.get(), SIGNAL(timeout()), this, SLOT(monitorWarningOper()), Qt::QueuedConnection);
    m_pTimer->start(1000 * 60 * m_oUserSetting.dMonitorInterval); 
}

//////////////////////////////////////////////////////////////////////////
//析构函数
MmwMonitorThread::~MmwMonitorThread()
{
    m_pTimer = nullptr;
}

//////////////////////////////////////////////////////////////////////////
//设置监控信息
void MmwMonitorThread::setMonitorInfo(const MmwMonitorContainer& oMonitorContaienr)
{
    m_oMonitorContainer = oMonitorContaienr;
}

//////////////////////////////////////////////////////////////////////////
//线程启动
void MmwMonitorThread::run()
{
    while (!m_bStopMonitor)
    {
        qDebug() << "Runing..." << m_pTimer->remainingTime();
    }
}

//////////////////////////////////////////////////////////////////////////
//监控预警
void MmwMonitorThread::monitorWarningOper()
{
    MddsMonitorOper oOddsMonitorOper;
    for (auto pIter = m_oMonitorContainer.begin(); pIter != m_oMonitorContainer.end(); ++pIter)
    {
        MonitorWebInfo oMonitorInfo = *pIter;
        oOddsMonitorOper.oddsMonitor(oMonitorInfo.sMonitorUrl, oMonitorInfo.dHostWarningValue, oMonitorInfo.dGuestWarningValue);
    }
    oOddsMonitorOper.sendEmail(m_oUserSetting.sWarningUser);
}

//////////////////////////////////////////////////////////////////////////
//设置结束
void MmwMonitorThread::setStop(bool bStopMonitor)
{
    m_bStopMonitor = bStopMonitor;
}

//////////////////////////////////////////////////////////////////////////
//是否结束
bool MmwMonitorThread::stoped()
{
    return m_bStopMonitor;
}

