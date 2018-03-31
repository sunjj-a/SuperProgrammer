#include "ExtractRecognizedImages.h"

#include <QDir>
#include <QFile>
#include <QCoreApplication>

#include "JlCompress.h"

//////////////////////////////////////////////////////////////////////////
//��ѹ��ͼƬ�ļ�

QStringList ExtractRecognizedImages::extractImages(const QString& sExcelPath)
{
    //׼����ʱ�ļ�
    QString sUserSetting = qApp->applicationDirPath() + "/ExtractImages";
    QDir oUserSettingDir(sUserSetting);
    if (oUserSettingDir.exists(sUserSetting))
    {
        oUserSettingDir.removeRecursively();
        oUserSettingDir.mkdir(sUserSetting);
    }

    //��ѹ���ļ�����ȡͼƬ�ļ�
    QStringList oRegonizedImages;
    QStringList oExtractFiles =  JlCompress::extractDir(sExcelPath, sUserSetting);
    for (auto pIter = oExtractFiles.begin(); pIter != oExtractFiles.end(); ++pIter)
    {
        QString sFileName = *pIter;
        if (sFileName.contains("/xl/media") && sFileName.endsWith(".png"))
            oRegonizedImages.append(sFileName);
    }
    
    //���ش���ȡ��ͼƬ�ļ�
    return oRegonizedImages;
}

