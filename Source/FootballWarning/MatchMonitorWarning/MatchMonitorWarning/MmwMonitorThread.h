#ifndef MMWMONITORTHREAD_H
#define MMWMONITORTHREAD_H

//////////////////////////////////////////////////////////////////////////
//智能监控线程
#include <QVector>
#include <QThread>

class QTimer;
struct MonitorWebInfo  //带监控网站信息
{
    QString sMonitorUrl;
    double dHostWarningValue; //主 Crown
    double dGuestWarningValue; //客 易胜博
};
typedef QVector<MonitorWebInfo> MmwMonitorContainer;

struct UserSetting    //用户配置
{
    int dMonitorInterval;  
    QString sWarningUser;
};

class MmwMonitorThread: public QThread
{
    Q_OBJECT

public:
    MmwMonitorThread(const UserSetting& oUserSetting);
    ~MmwMonitorThread();

    void setStop(bool bStopMonitor);
    bool stoped();
    void setMonitorInfo(const MmwMonitorContainer& oMonitorContaienr);

protected:
    virtual void run();
    
private slots:
    void monitorWarningOper();

private:
    std::shared_ptr<QTimer> m_pTimer;
    MmwMonitorContainer m_oMonitorContainer;
    UserSetting m_oUserSetting;
    bool m_bStopMonitor;
};
#endif