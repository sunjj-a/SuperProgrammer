#ifndef ODDSMONITORWARNING_H
#define ODDSMONITORWARNING_H

#include <QtWidgets/QDialog>
#include "OmwMonitorThread.h"

class QTableWidget;
class QPushButton;
class QGroupBox;
class QLineEdit;
class OmwMonitorThread;

class OddsMonitorWarning : public QDialog
{
    Q_OBJECT

public:
    OddsMonitorWarning(QWidget *parent = 0);
    ~OddsMonitorWarning();

private:
    void initUIForm();
    void initConnect();

private slots:
    void startMonitor();
    void stopMonitor();
    void clearMonitor();
    void showUserSetting();

private:
    bool loadMonitorInfo(OmwMonitorContainer& oMonitorContainer);
    void setWidgetHeader();

    bool checkUserSetting();
    void saveUserSetting();
    UserSetting monitorUserSetting();

private:
    QPushButton *m_pStartMonitor, *m_pStopMonitor, *m_pClearMonitor, *m_pShowUserSetting;
    QLineEdit *m_pWarningUserEdt, *m_pWarningIntervalEdt; 
    QTableWidget* m_pTableWidget;
    QGroupBox* m_pUserSettingBox;
    OmwMonitorThread* m_pMonitorThread;
};

#endif // ODDSMONITORWARNING_H
