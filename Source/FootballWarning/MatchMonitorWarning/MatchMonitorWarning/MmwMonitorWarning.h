#ifndef ODDSMONITORWARNING_H
#define ODDSMONITORWARNING_H

#include <QtWidgets/QDialog>
#include "MmwMonitorThread.h"

class QTableWidget;
class QPushButton;
class QGroupBox;
class QLineEdit;
class MmwMonitorThread;

class MmwMonitorWarning : public QDialog
{
    Q_OBJECT

public:
    MmwMonitorWarning(QWidget *parent = 0);
    ~MmwMonitorWarning();

private:
    void initUIForm();
    void initConnect();

private slots:
    void startMonitor();
    void stopMonitor();
    void clearMonitor();
    void showUserSetting();

private:
    bool loadMonitorInfo(MmwMonitorContainer& oMonitorContainer);
    void setWidgetHeader();

    bool checkUserSetting();
    void saveUserSetting();
    UserSetting monitorUserSetting();

private:
    QPushButton *m_pStartMonitor, *m_pStopMonitor, *m_pClearMonitor, *m_pShowUserSetting;
    QLineEdit *m_pWarningUserEdt, *m_pWarningIntervalEdt; 
    QTableWidget* m_pTableWidget;
    QGroupBox* m_pUserSettingBox;
    MmwMonitorThread* m_pMonitorThread;
};

#endif // ODDSMONITORWARNING_H
