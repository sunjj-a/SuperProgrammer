#ifndef GENPRODUCTTITLE_H
#define GENPRODUCTTITLE_H

#include <QtWidgets/QDialog>
#include "ui_GenProductTitle.h"


class QLabel;
class QPushButton;
class QLineEdit;

class GenProductTitle : public QDialog
{
    Q_OBJECT

public:
    GenProductTitle(QWidget *parent = 0);
    ~GenProductTitle();

private slots:
    void selSourceFile();
    void autoGenTitle();

private:
    //Ui::GenProductTitleClass ui;
    QLabel* m_pMaxCountLbl;
    QLineEdit* m_pMaxCountEdt;

    QLabel* m_pSelectLbl;
    QPushButton* m_pSelFileBtn, *m_pAutoGenBtn;
    QLineEdit* m_pHintInfoEdit;
};

#endif // GENPRODUCTTITLE_H
