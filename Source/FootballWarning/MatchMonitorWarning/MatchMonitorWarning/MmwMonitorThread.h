#ifndef MMWMONITORTHREAD_H
#define MMWMONITORTHREAD_H

//////////////////////////////////////////////////////////////////////////
//���ܼ���߳�
#include <QVector>
#include <QThread>

class QTimer;
struct MonitorWebInfo  //�������վ��Ϣ
{
    QString sMonitorUrl;
    double dHostWarningValue; //�� Crown
    double dGuestWarningValue; //�� ��ʤ��
};
typedef QVector<MonitorWebInfo> MmwMonitorContainer;

struct UserSetting    //�û�����
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