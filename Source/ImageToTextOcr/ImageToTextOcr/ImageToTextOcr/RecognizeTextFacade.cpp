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
//自动识别文字
void RecognizeTextFacade::autoRecognizeText(const QStringList& sRecognizeExcels)
{
    int nRecognizeExecelCount = sRecognizeExcels.size();
    for (int nRecognizeIndex = 0; nRecognizeIndex < nRecognizeExecelCount; ++nRecognizeIndex)
    {
        QString sExcelPath = sRecognizeExcels.at(nRecognizeIndex);
        m_pProgressBar->setFormat(QStringLiteral("获取待识别图片..."));

        //获取待识别图片
        QStringList oRegonizedImages = ExtractRecognizedImages().extractImages(sExcelPath);
        std::sort(oRegonizedImages.begin(), oRegonizedImages.end());
        if (oRegonizedImages.empty()) 
            return;

        //只取出激活码图片
        QString sImagePath = QFileInfo(oRegonizedImages.at(0)).absolutePath();
        int nStartIndex = oRegonizedImages.size() / 2;

        //特殊处理，验证用，不支持300多个图片验证！！！
        if (nStartIndex > 300)
        {
            return;
        }

        m_pProgressBar->setMinimum(0);
        m_pProgressBar->setMaximum(nStartIndex);

        //识别图片的内容
        QStringList oRecognizedList;
        std::shared_ptr<RecognizeTextOper> pRecognizeOper = std::make_shared<RecognizeTextOper>();
        for (int nIndex = nStartIndex, nValue = 0; nIndex < oRegonizedImages.size(); ++nIndex, ++nValue)
        {
            QString sRecogninzedImage = sImagePath + QString("/image%1.png").arg(nIndex + 1);
            //qDebug() << "ImageName:" << sRecogninzedImage;
            oRecognizedList.append(pRecognizeOper->recognizeText(sRecogninzedImage));
            m_pProgressBar->setValue(nValue);
            m_pProgressBar->setFormat(QStringLiteral("文件%1/%2：识别激活码中...").arg(nRecognizeIndex + 1).arg(nRecognizeExecelCount));
        }

        m_pProgressBar->setValue(nStartIndex);
        m_pProgressBar->setFormat(QStringLiteral("添加激活码数据..."));

        //回写到原始文件中
        QXlsx::Document oDocument(sExcelPath);
        QXlsx::Format oCellFormat;
        oCellFormat.setFontSize(11);
        oCellFormat.setFontName(QStringLiteral("宋体"));
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
//设置进度
void RecognizeTextFacade::setRegognizeBar(QProgressBar* pProgressBar)
{
    m_pProgressBar = pProgressBar;
}

