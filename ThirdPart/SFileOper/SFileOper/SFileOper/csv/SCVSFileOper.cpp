#include "SCVSFileOper.h"
#include <QFile>
#include <QTextStream>

bool SCVSFileOper::readFromFile(const QString& sCVSFile, SCVSContainer& oCVSContainer)
{
    QFile oCVSFile(sCVSFile);
    if (!oCVSFile.open(QIODevice::ReadOnly))
    {
        oCVSFile.close();
        return false;
    }

    oCVSContainer.clear();
    QTextStream oTextStream(&oCVSFile);
    QStringList oRecordList = oTextStream.readAll().split("\n");
    for (auto pIter = oRecordList.begin(); pIter != oRecordList.end(); ++pIter)
    {
        QStringList oRecords = pIter->split(",");
        oCVSContainer.push_back(oRecords);
    }
    return true;
}

