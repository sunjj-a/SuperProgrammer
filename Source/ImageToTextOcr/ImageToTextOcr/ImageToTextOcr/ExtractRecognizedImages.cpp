#include "ExtractRecognizedImages.h"

#include <QDir>
#include <QFile>
#include <QCoreApplication>

#include "JlCompress.h"

//////////////////////////////////////////////////////////////////////////
//解压缩图片文件

QStringList ExtractRecognizedImages::extractImages(const QString& sExcelPath)
{
    //准备临时文件
    QString sUserSetting = qApp->applicationDirPath() + "/ExtractImages";
    QDir oUserSettingDir(sUserSetting);
    if (oUserSettingDir.exists(sUserSetting))
    {
        oUserSettingDir.removeRecursively();
        oUserSettingDir.mkdir(sUserSetting);
    }

    //解压缩文件并提取图片文件
    QStringList oRegonizedImages;
    QStringList oExtractFiles =  JlCompress::extractDir(sExcelPath, sUserSetting);
    for (auto pIter = oExtractFiles.begin(); pIter != oExtractFiles.end(); ++pIter)
    {
        QString sFileName = *pIter;
        if (sFileName.contains("/xl/media") && sFileName.endsWith(".png"))
            oRegonizedImages.append(sFileName);
    }
    
    //返回待提取的图片文件
    return oRegonizedImages;
}

