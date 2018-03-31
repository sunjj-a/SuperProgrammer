#include "MmwMonitorThread.h"
#include <QTimer>
#include <QDebug>
#include "MmwMonitorOper.h"

//////////////////////////////////////////////////////////////////////////
//���캯��
MmwMonitorThread::MmwMonitorThread(const UserSetting& oUserSetting)
    :m_pTimer(nullptr), m_bStopMonitor(false), m_oUserSetting(oUserSetting)
{
    m_pTimer = std::make_shared<QTimer>(this);
    connect(m_pTimer.get(), SIGNAL(timeout()), this, SLOT(monitorWarningOper()), Qt::QueuedConnection);
    m_pTimer->start(1000 * 60 * m_oUserSetting.dMonitorInterval); 
}

//////////////////////////////////////////////////////////////////////////
//��������
MmwMonitorThread::~MmwMonitorThread()
{
    m_pTimer = nullptr;
}

//////////////////////////////////////////////////////////////////////////
//���ü����Ϣ
void MmwMonitorThread::setMonitorInfo(const MmwMonitorContainer& oMonitorContaienr)
{
    m_oMonitorContainer = oMonitorContaienr;
}

//////////////////////////////////////////////////////////////////////////
//�߳�����
void MmwMonitorThread::run()
{
    while (!m_bStopMonitor)
    {
        qDebug() << "Runing..." << m_pTimer->remainingTime();
    }
}

//////////////////////////////////////////////////////////////////////////
//���Ԥ��
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
//���ý���
void MmwMonitorThread::setStop(bool bStopMonitor)
{
    m_bStopMonitor = bStopMonitor;
}

//////////////////////////////////////////////////////////////////////////
//�Ƿ����
bool MmwMonitorThread::stoped()
{
    return m_bStopMonitor;
}

