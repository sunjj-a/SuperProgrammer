#ifndef EXTRACTRECOGNIZEDIMAGES_H
#define EXTRACTRECOGNIZEDIMAGES_H

//////////////////////////////////////////////////////////////////////////
//提取待识别的图片资源

#include <QString>

class ExtractRecognizedImages
{
public:
    QStringList extractImages(const QString& sExcelPath);
};
#endif