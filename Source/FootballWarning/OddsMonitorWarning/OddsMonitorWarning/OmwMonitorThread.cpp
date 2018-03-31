#include "OmwMonitorThread.h"
#include <QTimer>
#include <QDebug>
#include "OmwMonitorOper.h"

//////////////////////////////////////////////////////////////////////////
//���캯��
OmwMonitorThread::OmwMonitorThread(const UserSetting& oUserSetting)
    :m_pTimer(nullptr), m_bStopMonitor(false), m_oUserSetting(oUserSetting)
{
    m_pTimer = std::make_shared<QTimer>(this);
    connect(m_pTimer.get(), SIGNAL(timeout()), this, SLOT(monitorWarningOper()), Qt::QueuedConnection);
    m_pTimer->start(1000 * 60 * m_oUserSetting.dMonitorInterval); 
}

//////////////////////////////////////////////////////////////////////////
//��������
OmwMonitorThread::~OmwMonitorThread()
{
    m_pTimer = nullptr;
}

//////////////////////////////////////////////////////////////////////////
//���ü����Ϣ
void OmwMonitorThread::setMonitorInfo(const OmwMonitorContainer& oMonitorContaienr)
{
    m_oMonitorContainer = oMonitorContaienr;
}

//////////////////////////////////////////////////////////////////////////
//�߳�����
void OmwMonitorThread::run()
{
    while (!m_bStopMonitor)
    {
        qDebug() << "Runing..." << m_pTimer->remainingTime();
    }
}

//////////////////////////////////////////////////////////////////////////
//���Ԥ��
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
//���ý���
void OmwMonitorThread::setStop(bool bStopMonitor)
{
    m_bStopMonitor = bStopMonitor;
}

//////////////////////////////////////////////////////////////////////////
//�Ƿ����
bool OmwMonitorThread::stoped()
{
    return m_bStopMonitor;
}
