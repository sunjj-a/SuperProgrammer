#include "RecognizeTextFacade.h"

#include <QDebug>
#include <QFileInfo>
#include <QProgressBar>
#include <Windows.h>

#include "ExtractRecognizedImages.h"
#include "RecognizeTextOper.h"

#include "xlsx/xlsxdocument.h"
#include "xlsx/xlsxformat.h"
#include "xlsx/xlsxcellrange.h"
#include "xlsx/xlsxchart.h"

//////////////////////////////////////////////////////////////////////////
//�Զ�ʶ������
void RecognizeTextFacade::autoRecognizeText(const QStringList& sRecognizeExcels)
{
    int nRecognizeExecelCount = sRecognizeExcels.size();
    for (int nRecognizeIndex = 0; nRecognizeIndex < nRecognizeExecelCount; ++nRecognizeIndex)
    {
        QString sExcelPath = sRecognizeExcels.at(nRecognizeIndex);
        m_pProgressBar->setFormat(QStringLiteral("��ȡ��ʶ��ͼƬ..."));

        //��ȡ��ʶ��ͼƬ
        QStringList oRegonizedImages = ExtractRecognizedImages().extractImages(sExcelPath);
        std::sort(oRegonizedImages.begin(), oRegonizedImages.end());
        if (oRegonizedImages.empty()) 
            return;

        //ֻȡ��������ͼƬ
        QString sImagePath = QFileInfo(oRegonizedImages.at(0)).absolutePath();
        int nStartIndex = oRegonizedImages.size() / 2;

        //���⴦����֤�ã���֧��300���ͼƬ��֤������
        if (nStartIndex > 300)
        {
            return;
        }

        m_pProgressBar->setMinimum(0);
        m_pProgressBar->setMaximum(nStartIndex);

        //ʶ��ͼƬ������
        QStringList oRecognizedList;
        std::shared_ptr<RecognizeTextOper> pRecognizeOper = std::make_shared<RecognizeTextOper>();
        for (int nIndex = nStartIndex, nValue = 0; nIndex < oRegonizedImages.size(); ++nIndex, ++nValue)
        {
            QString sRecogninzedImage = sImagePath + QString("/image%1.png").arg(nIndex + 1);
            //qDebug() << "ImageName:" << sRecogninzedImage;
            oRecognizedList.append(pRecognizeOper->recognizeText(sRecogninzedImage));
            m_pProgressBar->setValue(nValue);
            m_pProgressBar->setFormat(QStringLiteral("�ļ�%1/%2��ʶ�𼤻�����...").arg(nRecognizeIndex + 1).arg(nRecognizeExecelCount));
        }

        m_pProgressBar->setValue(nStartIndex);
        m_pProgressBar->setFormat(QStringLiteral("��Ӽ���������..."));

        //��д��ԭʼ�ļ���
        QXlsx::Document oDocument(sExcelPath);
        QXlsx::Format oCellFormat;
        oCellFormat.setFontSize(11);
        oCellFormat.setFontName(QStringLiteral("����"));
        oCellFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
        oDocument.selectSheet("Sheet1");
        oDocument.setColumnWidth(3, 27);
        for (int nIndex = 0; nIndex < oRecognizedList.size(); ++nIndex)
        {
            QString sColIdentify = QString("C%1").arg(nIndex + 1);
            QString sRecognizedText = oRecognizedList.at(nIndex);
            oDocument.write(sColIdentify, sRecognizedText, oCellFormat);
        }
        oDocument.save();
    }
}

//////////////////////////////////////////////////////////////////////////
//���ý���
void RecognizeTextFacade::setRegognizeBar(QProgressBar* pProgressBar)
{
    m_pProgressBar = pProgressBar;
}

