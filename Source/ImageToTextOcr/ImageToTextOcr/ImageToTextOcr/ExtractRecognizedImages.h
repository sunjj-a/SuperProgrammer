#ifndef EXTRACTRECOGNIZEDIMAGES_H
#define EXTRACTRECOGNIZEDIMAGES_H

//////////////////////////////////////////////////////////////////////////
//��ȡ��ʶ���ͼƬ��Դ

#include <QString>

class ExtractRecognizedImages
{
public:
    QStringList extractImages(const QString& sExcelPath);
};
#endif