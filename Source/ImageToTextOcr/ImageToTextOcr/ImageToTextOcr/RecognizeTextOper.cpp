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

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "log/log.h"
#include "RecognizeSetting.h"

#define RECOGINZE_TEXT_URL "https://api.ai.qq.com/fcgi-bin/ocr/ocr_generalocr"  //识别算法URL
#define RANDOM_STR "fa475ce340859faff"  //随机字符串
#define APP_KEY "X0NuPZDR79M9PARD"      //appkey
#define APP_ID 1106662901               //appid
#define IMAGE_FILE_EXT "png"            //图片资源后缀
#define TIME_STAMP QDateTime::currentDateTime().toTime_t() //当前时间戳
#define RECOGNIZE_OK "ok"               //识别成功
#define SEPERATE_COUNT 3                //分隔符数量

//////////////////////////////////////////////////////////////////////////
//构造函数
RecognizeTextOper::RecognizeTextOper()
{
    m_pRequest = new QNetworkRequest;
    m_pRequest->setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    m_pRequest->setUrl(QUrl(RECOGINZE_TEXT_URL));

    m_pAccessManager = new QNetworkAccessManager;
    connect(m_pAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
}

//////////////////////////////////////////////////////////////////////////
//析构函数
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
//识别文字
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
   
    QEventLoop oEventLoop;
    QNetworkReply* pReply = m_pAccessManager->post(*m_pRequest, sParam.toUtf8());
    connect(pReply, SIGNAL(finished()), &oEventLoop, SLOT(quit()));
    oEventLoop.exec();

    return m_sRecognizeCode;
}

//////////////////////////////////////////////////////////////////////////
//请求结束后处理
void RecognizeTextOper::requestFinished(QNetworkReply *pReply)
{
//     QString sErrorType = "pReply->error(): " + QString::number(pReply->error());
//     LOG_INFO(sErrorType.toStdWString().c_str());
// 
//     QString sErrorString = "pReply->errorString():" + pReply->errorString();
//     LOG_INFO(sErrorString.toStdWString().c_str());

    if (pReply->error() == QNetworkReply::NoError)
    {
        QJsonParseError oError;
        QJsonDocument oDocument = QJsonDocument::fromJson(QString(pReply->readAll()).toUtf8(), &oError);
        if ((oError.error == QJsonParseError::NoError) && !oDocument.isEmpty() && oDocument.isObject())
        {
//             int nReturn = oDocument.object().value("ret").toInt();
//             QString sReturn = "URL_Return:" + QString::number(nReturn);
//             LOG_INFO(sReturn.toStdWString().c_str());
//             QString sURLMsg = "URL_Msg:" + sMsg;
//             LOG_INFO(sURLMsg.toStdWString().c_str());

            QString sMsg = oDocument.object().value("msg").toString();
            if (sMsg == RECOGNIZE_OK)
            {
                QStringList oRecognizeList = getRecognizeList(&oDocument);
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
//识别文字结果
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
//识别激活码
QString RecognizeTextOper::recognizeActiveCode(const QStringList& oRecognizeList)
{
    QString sActiveCode = "";
    for (auto pIter = oRecognizeList.begin(); pIter != oRecognizeList.end(); ++pIter)
    {
        QString sCurString = *pIter;
        if (sCurString.count("-") == SEPERATE_COUNT)
        {
            sActiveCode = sCurString;
            break;
        }
    }
    return sActiveCode;
}

//////////////////////////////////////////////////////////////////////////
//识别激活码
QString RecognizeTextOper::repairActiveCode(const QString& oActiveCode)
{
    QString sActiveCode = oActiveCode;
    sActiveCode.toUpper();
    sActiveCode.trimmed();
    sActiveCode.replace(" ", "");

    QRegExp oRegExp("([0-9a-zA-Z]{4}-[0-9a-zA-Z]{4}-[0-9a-zA-Z]{4}-[0-9a-zA-Z]{4})");
    int nPosition = oRegExp.indexIn(sActiveCode);
    if (nPosition < 0)
    {
        assert(false);
        return sActiveCode;
    }

    sActiveCode = oRegExp.cap(0);
    for (int nIndex = 0; nIndex < sActiveCode.size(); ++nIndex)
    {
        QChar oChar = sActiveCode.at(nIndex);
        QChar oReplaceChar;
        if (RecognizeSetting::instance()->exist(oChar, oReplaceChar))
        {
            //替换不匹配的字符
            sActiveCode.replace(nIndex, 1, oReplaceChar);
        }
    }
    return sActiveCode;
}

//////////////////////////////////////////////////////////////////////////
//Image转化为Base64
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
//URL的数字签名
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