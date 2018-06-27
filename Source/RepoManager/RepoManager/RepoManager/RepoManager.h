#ifndef REPOMANAGER_H
#define REPOMANAGER_H

#include <QtWidgets/QDialog>
#include "ui_RepoManager.h"

class QLabel;
class QPushButton;
class QLineEdit;

class RepoManager : public QDialog
{
    Q_OBJECT

public:
    RepoManager(QWidget *parent = 0);
    ~RepoManager();

private slots:
    void resetSetting();
    void beginCalcRepo();
    void selSourceDir();
    void selDestDir();

private:
    void initUI();
    void initConnect();
    void hintInfo(const QString& sErrorInfo);
    QString defaultDestDir();

private:
    QLabel *m_pSourceDirLbl, *m_pDestDirLbl, *m_pCalcCircleLbl, *m_pRemoveNumLbl;
    QLineEdit *m_pSourceDirEdt, *m_pDestDirEdt, *m_pCalcCircleEdt, *m_pRemoveNumEdt;
    QPushButton *m_pSourceDirBtn, *m_pDestDirBtn, *m_pCalcRepoBtn, *m_pResetBtn;
};

#endif // REPOMANAGER_H
