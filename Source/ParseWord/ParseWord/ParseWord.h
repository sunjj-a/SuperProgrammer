#ifndef PARSEWORD_H
#define PARSEWORD_H

#include <QtWidgets/QDialog>
#include "ui_ParseWord.h"
class QLabel;

class ParseWord : public QDialog
{
    Q_OBJECT

public:
    ParseWord(QWidget *parent = 0);
    ~ParseWord();

private:
    Ui::ParseWordClass ui;

private slots:
    void selectFile();
    void checkReject();
    void beginParse();

private:
    QLineEdit* m_pFileEdit;
    QLabel* m_pInfo;
    QPushButton* m_pBegin;
    QPushButton* m_pSelect;
    QPushButton* m_pCheck;

    QLineEdit* m_pUserAgeEdt;
    QLabel* m_pUserAgeLbl;
};

#endif // PARSEWORD_H
