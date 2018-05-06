#include "GenProductTitle.h"

#include "AutoGenUniqueTitle.h"
#include <QDebug>
#include "xlsxdocument.h"

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

GenProductTitle::GenProductTitle(QWidget *parent)
    : QDialog(parent)
{
    //ui.setupUi(this);
    m_pMaxCountLbl = new QLabel(QStringLiteral("标语数量："));
    m_pMaxCountEdt = new QLineEdit();
    m_pMaxCountEdt->setText("6");

    m_pSelectLbl = new QLabel(QStringLiteral("原始文件："));
    m_pHintInfoEdit = new QLineEdit();
    m_pSelFileBtn = new QPushButton(QStringLiteral("选择文件"));
    m_pAutoGenBtn = new QPushButton(QStringLiteral("生成标题"));

    QHBoxLayout* pHBox1 = new QHBoxLayout;
    pHBox1->addWidget(m_pMaxCountLbl);
    pHBox1->addWidget(m_pMaxCountEdt);
    pHBox1->addStretch();

    QHBoxLayout* pHBox2 = new QHBoxLayout;
    pHBox2->addWidget(m_pSelectLbl);
    pHBox2->addWidget(m_pHintInfoEdit);
    pHBox2->addWidget(m_pSelFileBtn);
    pHBox2->addWidget(m_pAutoGenBtn);

    QVBoxLayout* pVMainBox = new QVBoxLayout;
    pVMainBox->addLayout(pHBox1);
    pVMainBox->addLayout(pHBox2);
    setLayout(pVMainBox);

    connect(m_pSelFileBtn, SIGNAL(clicked()), this, SLOT(selSourceFile()));
    connect(m_pAutoGenBtn, SIGNAL(clicked()), this, SLOT(autoGenTitle()));

    resize(600, 90);
    setWindowTitle(QStringLiteral("自动生成标题-直通车-小魏专用软件"));
}

GenProductTitle::~GenProductTitle()
{

}

void GenProductTitle::selSourceFile()
{
    QString sSourceFilePath = QFileDialog::getOpenFileName(this, QStringLiteral("选择文件"));
    if (!sSourceFilePath.isEmpty())
        m_pHintInfoEdit->setText(sSourceFilePath);
}

void GenProductTitle::autoGenTitle()
{
    bool bSuccess = true;
    int nMaxMatchCount = m_pMaxCountEdt->text().toInt(&bSuccess);
    if (!bSuccess)
    {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请输入有效的最大标语数量 ^_^"));
        return;
    }

    QXlsx::Document oDocument(m_pHintInfoEdit->text());
    QStringList oSheetList = oDocument.sheetNames();

    for (int nSheetIndex = 0; nSheetIndex < oSheetList.size(); ++nSheetIndex)
    {
        SourceWords oSourceWords;
        oDocument.selectSheet(oSheetList.at(nSheetIndex));
        int nRowCount = oDocument.dimension().rowCount();
        int nStartIndex = 1;
        for (int nRowIndex = 0; nRowIndex < nRowCount; ++nRowIndex)
        {
            int nCurRowIndex = nRowIndex + 1;
            oDocument.write(nCurRowIndex, 3, "");
            oDocument.write(nCurRowIndex, 4, "");
            QString sProductTitle = oDocument.read(nCurRowIndex, 1).toString();
            if (!sProductTitle.trimmed().isEmpty())
            {
                qDebug() << sProductTitle;
                oSourceWords.push_back(std::make_pair(nStartIndex++, sProductTitle));
            }
        }

        oDocument.setColumnWidth(3, 50);
        oDocument.setColumnWidth(4, 18);

        std::shared_ptr<GenUniqueTitleResult> oGenUniqueTitleResult = AutoGenUniqueTitle().autoGenTitle(nMaxMatchCount, oSourceWords);
        std::shared_ptr<GenTitleContainer> pGenTitleContainer = oGenUniqueTitleResult->pGenTitleContainer;
        std::shared_ptr<UnMatchWordContainer> pUnMatchWordContainer = oGenUniqueTitleResult->pUnMatchWordContainer;

        nStartIndex = 1;
        for (auto pIter = pGenTitleContainer->begin(); pIter != pGenTitleContainer->end(); ++pIter)
        {
            qDebug() << *pIter;
            oDocument.write(nStartIndex++, 3, *pIter);
        }

        nStartIndex = 1;
        for (auto pIter = pUnMatchWordContainer->begin(); pIter != pUnMatchWordContainer->end(); ++pIter)
        {
            qDebug() << *pIter;
            oDocument.write(nStartIndex++, 4, *pIter);
        }
    }

    oDocument.selectSheet(oSheetList.at(0));
    oDocument.save();
    QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("自动生成标题成功 ^_^"));
}
