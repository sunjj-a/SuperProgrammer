#ifndef OMWMONITORTHREAD_H
#define OMWMONITORTHREAD_H

//////////////////////////////////////////////////////////////////////////
//智能监控线程
#include <QVector>
#include <QThread>

class QTimer;
struct MonitorWebInfo  //带监控网站信息
{
    QString sMonitorUrl;
    double dWarningValue;
};
typedef QVector<MonitorWebInfo> OmwMonitorContainer;

struct UserSetting    //用户配置
{
    int dMonitorInterval;  
    QString sWarningUser;
};

class OmwMonitorThread: public QThread
{
    Q_OBJECT

public:
    OmwMonitorThread(const UserSetting& oUserSetting);
    ~OmwMonitorThread();

    void setStop(bool bStopMonitor);
    bool stoped();
    void setMonitorInfo(const OmwMonitorContainer& oMonitorContaienr);

protected:
    virtual void run();

private slots:
    void monitorWarningOper();

private:
    std::shared_ptr<QTimer> m_pTimer;
    OmwMonitorContainer m_oMonitorContainer;
    UserSetting m_oUserSetting;
    bool m_bStopMonitor;
};
#endif