#include "OmwMonitorOper.h"
#include "OmwPythonOper.h"

//////////////////////////////////////////////////////////////////////////
//���캯��
OddsMonitorOper::OddsMonitorOper()
    :m_pPythonOper(nullptr)
{
    m_pPythonOper = std::make_shared<OmwPythonOper>();
}

//////////////////////////////////////////////////////////////////////////
//��������
OddsMonitorOper::~OddsMonitorOper()
{
    m_pPythonOper = nullptr;
}

//////////////////////////////////////////////////////////////////////////
//���ʼ��
void OddsMonitorOper::oddsMonitor(const QString& sMonitorUrl, double dWarningValue)
{
    QString sWarningResult = m_pPythonOper->oddsMonitorWarning(sMonitorUrl, dWarningValue);
    if (!sWarningResult.isEmpty())
        m_sWarningResult += "\n\n" + sWarningResult;
}

//////////////////////////////////////////////////////////////////////////
//�Զ������ʼ�
void OddsMonitorOper::sendEmail(const QString& sReceiver)
{
    if (!m_sWarningResult.isEmpty())
        m_pPythonOper->sendEmail(QStringLiteral("���ʼ��Ԥ��"), m_sWarningResult, sReceiver);
}

