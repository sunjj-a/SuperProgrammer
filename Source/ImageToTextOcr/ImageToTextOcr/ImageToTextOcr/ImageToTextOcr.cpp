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
//���캯��
ImageToTextOcr::ImageToTextOcr(QWidget *parent)
    : QDialog(parent)
{
    //SetLogInformaiton(true, "E://Recognize.html", 1);

    //qDebug() << QStringLiteral("ʶ������") << RecognizeTextOper().recognizeText("E:\\1234.jpg");
    initUIForm();
    initConnect();
}

//////////////////////////////////////////////////////////////////////////
//��������
ImageToTextOcr::~ImageToTextOcr()
{

}

//////////////////////////////////////////////////////////////////////////
//UI��ʼ��
void ImageToTextOcr::initUIForm()
{
    m_pLable = new QLabel(QStringLiteral("ԭʼ���ݣ�"));
    m_pSelectedFileEidt = new QLineEdit();
    m_pSelectedFileBtn = new QPushButton(QStringLiteral("ѡ���ļ���"));
    m_pRecognizedTextBtn = new QPushButton(QStringLiteral("������ʶ��"));
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
    setWindowTitle(QStringLiteral("����ʶ�𼤻���"));
    setWindowIcon(QIcon(":/ImageToTextOcr/ICON.ico"));
}

//////////////////////////////////////////////////////////////////////////
//�źŲ۳�ʼ��
void ImageToTextOcr::initConnect()
{
    connect(m_pSelectedFileBtn, SIGNAL(clicked()), this, SLOT(selectedFile()));
    connect(m_pRecognizedTextBtn, SIGNAL(clicked()), this, SLOT(recognizedText()));
}

//////////////////////////////////////////////////////////////////////////
//ѡ���ļ�
void ImageToTextOcr::selectedFile()
{
    QString sSelectedFile = QFileDialog::getExistingDirectory(this, QStringLiteral("ԭʼ����"));
    if (sSelectedFile.isEmpty())
        return;

    m_pSelectedFileEidt->setText(sSelectedFile);
}

//////////////////////////////////////////////////////////////////////////
//ʶ���ı�
void ImageToTextOcr::recognizedText()
{
    QString sRegonizedPath = m_pSelectedFileEidt->text();
    if (!QDir().exists(sRegonizedPath))
    {
        QMessageBox::information(this, QStringLiteral("��ʾ"), QStringLiteral("�ļ��в����ڣ�������ѡ��"));
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
    QMessageBox::information(this, QStringLiteral("��ʾ"), QStringLiteral("ʶ��ɹ�"));
}


