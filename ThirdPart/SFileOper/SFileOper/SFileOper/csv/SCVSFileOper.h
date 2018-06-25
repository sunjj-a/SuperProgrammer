#pragma once
//////////////////////////////////////////////////////////////////////////
//csv文件操作

#include "sfileoper_global.h"
#include <QStringList>
#include <QVector>
#include <QString>

//cvs结果容器
typedef QVector<QStringList> SCVSContainer;

//cvs文件操作
class SFILEOPER_EXPORT SCVSFileOper
{
public:
    //读取cvs结果
    bool readFromFile(const QString& sCVSFile, SCVSContainer& oCVSContainer);
};