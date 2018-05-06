#include "GenProductTitle.h"

#include "AutoGenUniqueTitle.h"
#include <QDebug>
#include "xlsxdocument.h"

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMessageBox>

GenProductTitle::GenProductTitle(QWidget *parent)
    : QDialog(parent)
{
    //ui.setupUi(this);

    m_pSelectLbl = new QLabel(QStringLiteral("ԭʼ�ļ���"));
    m_pHintInfoEdit = new QLineEdit();
    m_pSelFileBtn = new QPushButton(QStringLiteral("ѡ���ļ�"));
    m_pAutoGenBtn = new QPushButton(QStringLiteral("���ɱ���"));

    QHBoxLayout* pHBox = new QHBoxLayout;
    pHBox->addWidget(m_pSelectLbl);
    pHBox->addWidget(m_pHintInfoEdit);
    pHBox->addWidget(m_pSelFileBtn);
    pHBox->addWidget(m_pAutoGenBtn);
    setLayout(pHBox);

    connect(m_pSelFileBtn, SIGNAL(clicked()), this, SLOT(selSourceFile()));
    connect(m_pAutoGenBtn, SIGNAL(clicked()), this, SLOT(autoGenTitle()));

    resize(600, 60);
    setWindowTitle(QStringLiteral("�Զ����ɱ���-Сκר�����"));
}

GenProductTitle::~GenProductTitle()
{

}

void GenProductTitle::selSourceFile()
{
    QString sSourceFilePath = QFileDialog::getOpenFileName(this, QStringLiteral("ѡ���ļ�"));
    if (!sSourceFilePath.isEmpty())
        m_pHintInfoEdit->setText(sSourceFilePath);
}

void GenProductTitle::autoGenTitle()
{
    QString sFilePath = m_pHintInfoEdit->text();
    SourceWords oSourceWords;

    QXlsx::Document oDocument(sFilePath);
    oDocument.selectSheet("Sheet1");
    int nRowCount = oDocument.dimension().rowCount();
    for (int nIndex = 0, nRowIndex = 0; nRowIndex < nRowCount; ++nRowIndex)
    {
        QString sProductTitle = oDocument.read(nRowIndex + 1, 1).toString();
        if (!sProductTitle.trimmed().isEmpty())
        {
            nIndex += 1;
            oSourceWords.push_back(std::make_pair(nIndex, sProductTitle));
        }
    }
    oDocument.setColumnWidth(4, 50);

    int nStartIndex = 1;
    std::shared_ptr<GenTitleContainer> oGenTitleContainer = AutoGenUniqueTitle().autoGenTitle(oSourceWords);
    for (auto pIter = oGenTitleContainer->begin(); pIter != oGenTitleContainer->end(); ++pIter)
    {
        qDebug() << *pIter;
        oDocument.write(nStartIndex++, 4, *pIter);
    }
    oDocument.save();

    QMessageBox::information(this, QStringLiteral("��ʾ"), QStringLiteral("�Զ����ɱ���ɹ� ^_^"));
}
