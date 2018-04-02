#include "RecognizeTextOper.h"

#include <assert.h>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#include <QEventLoop>
#include <QImage>
#include <QBuffer>
#include <QDateTime>
#include <QMap>
#include <QRegExp>
#include <QTime>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "log/log.h"
#include "RecognizeSetting.h"

#define RECOGINZE_TEXT_URL "https://api.ai.qq.com/fcgi-bin/ocr/ocr_generalocr"  //ʶ���㷨URL
#define RANDOM_STR "fa475ce340859faff"  //����ַ���
#define APP_KEY "X0NuPZDR79M9PARD"      //appkey
#define APP_ID 1106662901               //appid
#define IMAGE_FILE_EXT "png"            //ͼƬ��Դ��׺
#define TIME_STAMP QDateTime::currentDateTime().toTime_t() //��ǰʱ���
#define RECOGNIZE_OK "ok"               //ʶ��ɹ�
#define SEPERATE_COUNT 4                //�ָ�������
#define ACTIVE_COUNT 4                  //�����������
#define CODE_COUNT 16                   //��Ч�ַ�����

//////////////////////////////////////////////////////////////////////////
//���캯��
RecognizeTextOper::RecognizeTextOper()
{
    m_pRequest = new QNetworkRequest;
    m_pRequest->setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    m_pRequest->setUrl(QUrl(RECOGINZE_TEXT_URL));

    m_pAccessManager = new QNetworkAccessManager;
    connect(m_pAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
}

//////////////////////////////////////////////////////////////////////////
//��������
RecognizeTextOper::~RecognizeTextOper()
{
    if (m_pRequest)
    {
        delete m_pRequest;
        m_pRequest = nullptr;
    }

    if (m_pAccessManager)
    {
        delete m_pAccessManager;
        m_pAccessManager = nullptr;
    }
}

//////////////////////////////////////////////////////////////////////////
//ʶ������
QString RecognizeTextOper::recognizeText(const QString& sImageFile)
{
    m_sRecognizeCode = "";
    QMap<QString, QVariant> oParamMap;
    oParamMap.insert("app_id", APP_ID);
    oParamMap.insert("time_stamp", TIME_STAMP);
    oParamMap.insert("nonce_str", RANDOM_STR);
    oParamMap.insert("image", imageToBase64(sImageFile));
    oParamMap.insert("sign", getSignature(oParamMap));

    int nAppId = oParamMap.value("app_id").toInt();
    int nTimeStamp = oParamMap.value("time_stamp").toInt();
    QString sNonce = oParamMap.value("nonce_str").toString().toUtf8();
    QString sSinge = oParamMap.value("sign").toString().toUtf8();
    QString sImage = oParamMap.value("image").toString().toUtf8().toPercentEncoding();

    QString sParam = QString("app_id=%1&time_stamp=%2&nonce_str=%3&sign=%4&image=%5")
        .arg(nAppId).arg(nTimeStamp).arg(sNonce).arg(sSinge).arg(sImage);
   
    //QTime oTime;
    //oTime.start();

    QEventLoop oEventLoop;
    QNetworkReply* pReply = m_pAccessManager->post(*m_pRequest, sParam.toUtf8());
    connect(pReply, SIGNAL(finished()), &oEventLoop, SLOT(quit()));
    oEventLoop.exec();

    //qDebug() << "Use Time:" << oTime.elapsed();
    return m_sRecognizeCode;
}

//////////////////////////////////////////////////////////////////////////
//�����������
void RecognizeTextOper::requestFinished(QNetworkReply *pReply)
{
    if (pReply->error() == QNetworkReply::NoError)
    {
        QJsonParseError oError;
        QJsonDocument oDocument = QJsonDocument::fromJson(QString(pReply->readAll()).toUtf8(), &oError);
        if ((oError.error == QJsonParseError::NoError) && !oDocument.isEmpty() && oDocument.isObject())
        {
            QString sMsg = oDocument.object().value("msg").toString();
            if (sMsg == RECOGNIZE_OK)
            {
                QStringList oRecognizeList = getRecognizeList(&oDocument);
//                 QString sResult = "";
//                 for (auto pIter = oRecognizeList.begin(); pIter != oRecognizeList.end(); ++pIter)
//                 {
//                     sResult = sResult + "_____" + *pIter;
//                 }
//                 qDebug() << "Recognize result:" << sResult;
                QString sActiveCode = recognizeActiveCode(oRecognizeList);
                m_sRecognizeCode = repairActiveCode(sActiveCode);
            }
        }
    }
    else
    {
        qDebug() << "pReply->errorString(): " << pReply->errorString(); 
    }
    pReply->deleteLater();
}

//////////////////////////////////////////////////////////////////////////
//ʶ�����ֽ��
QStringList RecognizeTextOper::getRecognizeList(QJsonDocument* pJsonDoc)
{
    QStringList oResult;
    QJsonArray oJsonArray = pJsonDoc->object().value("data").toObject().value("item_list").toArray();
    for (int nIndex = 0; nIndex < oJsonArray.size(); ++nIndex)
    {
        QJsonObject oItemObj = oJsonArray.at(nIndex).toObject();
        oResult.append(oItemObj.value("itemstring").toString());
    }
    return oResult;
}

//////////////////////////////////////////////////////////////////////////
//ʶ�𼤻���
QString RecognizeTextOper::recognizeActiveCode(const QStringList& oRecognizeList)
{
    QString sActiveCode = "";
    for (auto pIter = oRecognizeList.begin(); pIter != oRecognizeList.end(); ++pIter)
    {
        QString sCurString = *pIter;
        QString sValidCode = "";
        for (int nIndex = 0; nIndex < sCurString.size(); ++nIndex)
        {
            QChar oChar = sCurString.at(nIndex);
            ushort nShort = oChar.unicode();  
            if(nShort >= 0x4E00 && nShort <= 0x9FA5)  
            {
                //�ų�����
                continue;
            }

            if (oChar.isLetterOrNumber() || oChar == "-")
            {
                sValidCode += oChar;
            }
        }

        if (sValidCode.size() >= CODE_COUNT)
        {
            sActiveCode = sValidCode;
            sActiveCode.toUpper();
            sActiveCode.trimmed();
            sActiveCode.replace(" ", "");
            break;
        }
    }
    return sActiveCode;
}

//////////////////////////////////////////////////////////////////////////
//ʶ�𼤻���
QString RecognizeTextOper::repairActiveCode(const QString& oActiveCode)
{
    QString sActiveCode = oActiveCode;
    if (sActiveCode.isEmpty())
    {
        assert(false);
        return QString();
    }

    //�޸�1. ɾ����Ч�ĺ���"-"
    QStringList oNewCodeList;
    QStringList oOldCodeList = sActiveCode.split("-");
    for (int nIndex = 0; nIndex < oOldCodeList.size(); ++nIndex)
    {
        QString sCode = oOldCodeList.at(nIndex);
        if (sCode.isEmpty())
            continue;
        oNewCodeList.push_back(sCode);
    }
    
    if (oNewCodeList.size() != SEPERATE_COUNT)
    {
        assert(false);
        return QString();
    }

    //�޸�2. ɾ����Ч�ĺ�����β�ַ�
    QStringList oActiveCodeList;
    for (int nIndex = 0; nIndex < SEPERATE_COUNT; ++nIndex)
    {
        QString sCode = oNewCodeList.at(nIndex);
        if (nIndex == 0)
        {
            //��Ԫ��
            if (sCode.size() > ACTIVE_COUNT)
                sCode = sCode.right(ACTIVE_COUNT);
        }
        else if (nIndex == ACTIVE_COUNT - 1)
        {
            //βԪ��
            if (sCode.size() > ACTIVE_COUNT)
                sCode = sCode.left(ACTIVE_COUNT);
        }
        oActiveCodeList.push_back(sCode);
    }

    if (oActiveCodeList.size() != SEPERATE_COUNT)
    {
        assert(false);
        return QString();
    }

    sActiveCode = oActiveCodeList.join("-");
    for (int nIndex = 0; nIndex < sActiveCode.size(); ++nIndex)
    {
        QChar oChar = sActiveCode.at(nIndex);
        QChar oReplaceChar;
        if (RecognizeSetting::instance()->exist(oChar, oReplaceChar))
        {
            //�滻��ƥ����ַ�
            sActiveCode.replace(nIndex, 1, oReplaceChar);
        }
    }
    return sActiveCode;
}

//////////////////////////////////////////////////////////////////////////
//Imageת��ΪBase64
QString RecognizeTextOper::imageToBase64(const QString& sImgPath)
{
    QImage oImage(sImgPath);
    QByteArray oByteArray;
    QBuffer oBuffer(&oByteArray);
    oImage.save(&oBuffer,IMAGE_FILE_EXT, 20);
    QByteArray hexed = oByteArray.toBase64();
    oBuffer.close();
    return hexed;
}

//////////////////////////////////////////////////////////////////////////
//URL������ǩ��
QString RecognizeTextOper::getSignature(const QMap<QString, QVariant>& oParamMap)
{
    QString sUrl = "";
    for (auto pIter = oParamMap.begin(); pIter != oParamMap.end(); ++pIter)
    {
        sUrl += pIter.key() + "=" + pIter.value().toString().toUtf8().toPercentEncoding() + "&";
    }
    sUrl += "app_key=";
    sUrl += APP_KEY;

    QByteArray oByteArray;
    oByteArray = QCryptographicHash::hash(sUrl.toUtf8(), QCryptographicHash::Md5);
    return QString(oByteArray.toHex()).toUpper();
}