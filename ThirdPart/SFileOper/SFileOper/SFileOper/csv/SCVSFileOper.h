#pragma once
//////////////////////////////////////////////////////////////////////////
//csv�ļ�����

#include "sfileoper_global.h"
#include <QStringList>
#include <QVector>
#include <QString>

//cvs�������
typedef QVector<QStringList> SCVSContainer;

//cvs�ļ�����
class SFILEOPER_EXPORT SCVSFileOper
{
public:
    //��ȡcvs���
    bool readFromFile(const QString& sCVSFile, SCVSContainer& oCVSContainer);
};