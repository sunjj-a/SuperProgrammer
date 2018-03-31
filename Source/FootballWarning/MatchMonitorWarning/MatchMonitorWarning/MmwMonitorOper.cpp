#include "MmwMonitorOper.h"
#include "MmwPythonOper.h"

//////////////////////////////////////////////////////////////////////////
//构造函数
MddsMonitorOper::MddsMonitorOper()
    :m_pPythonOper(nullptr)
{
    m_pPythonOper = std::make_shared<MmwPythonOper>();
}

//////////////////////////////////////////////////////////////////////////
//析构函数
MddsMonitorOper::~MddsMonitorOper()
{
    m_pPythonOper = nullptr;
}

//////////////////////////////////////////////////////////////////////////
//赔率监控
void MddsMonitorOper::oddsMonitor(const QString& sMonitorUrl, double dHostWarningValue, double dGuestWarningValue)
{
    QString sWarningResult = m_pPythonOper->matchMonitorWarning(sMonitorUrl, dHostWarningValue, dGuestWarningValue);
    if (!sWarningResult.isEmpty())
        m_sWarningResult += "\n\n" + sWarningResult;
}

//////////////////////////////////////////////////////////////////////////
//自动发送邮件
void MddsMonitorOper::sendEmail(const QString& sReceiver)
{
    if (!m_sWarningResult.isEmpty())
        m_pPythonOper->sendEmail(QStringLiteral("初盘监控预警"), m_sWarningResult, sReceiver);
}

