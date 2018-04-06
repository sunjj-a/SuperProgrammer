#ifndef EXCHANGEREPODATA_H
#define EXCHANGEREPODATA_H

#include <QtWidgets/QDialog>
#include "ui_ExchangeRepoData.h"

class QLabel;
class QPushButton;
class QLineEdit;

class ExchangeRepoData : public QDialog
{
    Q_OBJECT

public:
    ExchangeRepoData(QWidget *parent = 0);
    ~ExchangeRepoData();

private slots:
    void selFile();
    void translate();

private:
    Ui::ExchangeRepoDataClass ui;

    QPushButton *m_pSelFileBtn, *m_pBeginTransBtn;
    QLineEdit *m_pSelFileEdt;
};

#endif // EXCHANGEREPODATA_H
