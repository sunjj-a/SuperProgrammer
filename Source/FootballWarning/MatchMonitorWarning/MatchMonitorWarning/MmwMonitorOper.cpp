#include "MmwMonitorOper.h"
#include "MmwPythonOper.h"

//////////////////////////////////////////////////////////////////////////
//���캯��
MddsMonitorOper::MddsMonitorOper()
    :m_pPythonOper(nullptr)
{
    m_pPythonOper = std::make_shared<MmwPythonOper>();
}

//////////////////////////////////////////////////////////////////////////
//��������
MddsMonitorOper::~MddsMonitorOper()
{
    m_pPythonOper = nullptr;
}

//////////////////////////////////////////////////////////////////////////
//���ʼ��
void MddsMonitorOper::oddsMonitor(const QString& sMonitorUrl, double dHostWarningValue, double dGuestWarningValue)
{
    QString sWarningResult = m_pPythonOper->matchMonitorWarning(sMonitorUrl, dHostWarningValue, dGuestWarningValue);
    if (!sWarningResult.isEmpty())
        m_sWarningResult += "\n\n" + sWarningResult;
}

//////////////////////////////////////////////////////////////////////////
//�Զ������ʼ�
void MddsMonitorOper::sendEmail(const QString& sReceiver)
{
    if (!m_sWarningResult.isEmpty())
        m_pPythonOper->sendEmail(QStringLiteral("���̼��Ԥ��"), m_sWarningResult, sReceiver);
}

