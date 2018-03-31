#ifndef RECOGMIZETEXTOPERR_H
#define RECOGMIZETEXTOPERR_H

//////////////////////////////////////////////////////////////////////////
//����ʶ������㷨
//��ϸ�㷨ʵ����ο�https://ai.qq.com/doc/ocrgeneralocr.shtml ͨ��ocrʶ��

#include <QObject>

class QNetworkRequest;
class QNetworkReply;
class QNetworkAccessManager;

class RecognizeTextOper: public QObject
{
    Q_OBJECT

public:
    RecognizeTextOper();
    ~RecognizeTextOper();

    QString recognizeText(const QString& sImageFile);

private slots:
    void requestFinished(QNetworkReply *pReply);

private:
    QString imageToBase64(const QString& sImgPath);
    QString getSignature(const QMap<QString, QVariant>& oParamMap);
    QStringList getRecognizeList(QJsonDocument* pJsonDoc);
    QString recognizeActiveCode(const QStringList& oRecognizeList);
    QString repairActiveCode(const QString& oActiveCode);

private:
    QString m_sRecognizeCode;
    QNetworkRequest* m_pRequest;
    QNetworkAccessManager* m_pAccessManager;
};

#endif