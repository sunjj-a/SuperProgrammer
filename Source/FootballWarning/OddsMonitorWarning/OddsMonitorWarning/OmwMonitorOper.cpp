#include "OmwMonitorOper.h"
#include "OmwPythonOper.h"

//////////////////////////////////////////////////////////////////////////
//构造函数
OddsMonitorOper::OddsMonitorOper()
    :m_pPythonOper(nullptr)
{
    m_pPythonOper = std::make_shared<OmwPythonOper>();
}

//////////////////////////////////////////////////////////////////////////
//析构函数
OddsMonitorOper::~OddsMonitorOper()
{
    m_pPythonOper = nullptr;
}

//////////////////////////////////////////////////////////////////////////
//赔率监控
void OddsMonitorOper::oddsMonitor(const QString& sMonitorUrl, double dWarningValue)
{
    QString sWarningResult = m_pPythonOper->oddsMonitorWarning(sMonitorUrl, dWarningValue);
    if (!sWarningResult.isEmpty())
        m_sWarningResult += "\n\n" + sWarningResult;
}

//////////////////////////////////////////////////////////////////////////
//自动发送邮件
void OddsMonitorOper::sendEmail(const QString& sReceiver)
{
    if (!m_sWarningResult.isEmpty())
        m_pPythonOper->sendEmail(QStringLiteral("赔率监控预警"), m_sWarningResult, sReceiver);
}

