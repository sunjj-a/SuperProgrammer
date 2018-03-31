#ifndef ODDSMONITOROPER_H
#define ODDSMONITOROPER_H

#include <QString>
class OmwPythonOper;

class OddsMonitorOper
{
public:
    OddsMonitorOper();
    ~OddsMonitorOper();

    void oddsMonitor(const QString& sMonitorUrl, double dWarningValue);
    void sendEmail(const QString& sReceiver);

private:
    std::shared_ptr<OmwPythonOper> m_pPythonOper;
    QString m_sWarningResult;
};
#endif