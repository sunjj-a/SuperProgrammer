#include "ImageToTextOcr.h"

#include <QDir>
#include <QDebug>
#include <QFile>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressBar>
#include <QDirIterator>

#include "RecognizeTextFacade.h"
#include "log/log.h"

//#include "RecognizeTextOper.h"
#include <QApplication>

//////////////////////////////////////////////////////////////////////////
//构造函数
ImageToTextOcr::ImageToTextOcr(QWidget *parent)
    : QDialog(parent)
{
    //SetLogInformaiton(true, "E://Recognize.html", 1);

    //qDebug() << QStringLiteral("识别结果：") << RecognizeTextOper().recognizeText("E:\\1234.jpg");
    initUIForm();
    initConnect();
}

//////////////////////////////////////////////////////////////////////////
//析构函数
ImageToTextOcr::~ImageToTextOcr()
{

}

//////////////////////////////////////////////////////////////////////////
//UI初始化
void ImageToTextOcr::initUIForm()
{
    m_pLable = new QLabel(QStringLiteral("原始数据："));
    m_pSelectedFileEidt = new QLineEdit();
    m_pSelectedFileBtn = new QPushButton(QStringLiteral("选择文件夹"));
    m_pRecognizedTextBtn = new QPushButton(QStringLiteral("激活码识别"));
    m_pProgressBar = new QProgressBar(this);
    m_pProgressBar->setVisible(false);

    QVBoxLayout* pMainVBox = new QVBoxLayout;
    QHBoxLayout* pMainHBox = new QHBoxLayout;
    pMainHBox->addWidget(m_pLable);
    pMainHBox->addWidget(m_pSelectedFileEidt, 2);
    pMainHBox->addWidget(m_pSelectedFileBtn);
    pMainHBox->addWidget(m_pRecognizedTextBtn);
    pMainVBox->addLayout(pMainHBox);
    pMainVBox->addWidget(m_pProgressBar);
    setLayout(pMainVBox);

    resize(600, 80);
    setWindowTitle(QStringLiteral("批量识别激活码"));
    setWindowIcon(QIcon(":/ImageToTextOcr/ICON.ico"));
}

//////////////////////////////////////////////////////////////////////////
//信号槽初始化
void ImageToTextOcr::initConnect()
{
    connect(m_pSelectedFileBtn, SIGNAL(clicked()), this, SLOT(selectedFile()));
    connect(m_pRecognizedTextBtn, SIGNAL(clicked()), this, SLOT(recognizedText()));
}

//////////////////////////////////////////////////////////////////////////
//选择文件
void ImageToTextOcr::selectedFile()
{
    QString sSelectedFile = QFileDialog::getExistingDirectory(this, QStringLiteral("原始数据"));
    if (sSelectedFile.isEmpty())
        return;

    m_pSelectedFileEidt->setText(sSelectedFile);
}

//////////////////////////////////////////////////////////////////////////
//识别文本
void ImageToTextOcr::recognizedText()
{
    QString sRegonizedPath = m_pSelectedFileEidt->text();
    if (!QDir().exists(sRegonizedPath))
    {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("文件夹不存在，请重新选择"));
        return;
    }

    QStringList oFilters;
    oFilters << QString("*.xlsx");

    QDirIterator oDirIterator(sRegonizedPath, oFilters, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
    QStringList oRecognizedExcels;
    while(oDirIterator.hasNext())
    {
        oDirIterator.next();
        QString sRecognizedFile = oDirIterator.fileInfo().absoluteFilePath();
        oRecognizedExcels.append(sRecognizedFile);
    }

    m_pProgressBar->setVisible(true);
    RecognizeTextFacade oRecognizeText;
    oRecognizeText.setRegognizeBar(m_pProgressBar);
    oRecognizeText.autoRecognizeText(oRecognizedExcels);
    m_pProgressBar->setVisible(false);
    QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("识别成功"));
}


