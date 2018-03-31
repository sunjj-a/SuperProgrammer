#ifndef ODDSMONITOROPER_H
#define ODDSMONITOROPER_H

#include <QString>
class MmwPythonOper;

class MddsMonitorOper
{
public:
    MddsMonitorOper();
    ~MddsMonitorOper();

    void oddsMonitor(const QString& sMonitorUrl, double dHostWarningValue, double dGuestWarningValue);
    void sendEmail(const QString& sReceiver);

private:
    std::shared_ptr<MmwPythonOper> m_pPythonOper;
    QString m_sWarningResult;
};
#endif