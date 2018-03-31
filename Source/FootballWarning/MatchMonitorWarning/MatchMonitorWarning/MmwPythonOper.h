#ifndef MWMPYTHONOPER_H
#define MWMPYTHONOPER_H

//////////////////////////////////////////////////////////////////////////
//���Ԥ��py������

#include "Python.h"
#include <QString>

class MmwPythonOper
{
public:
    MmwPythonOper();
    ~MmwPythonOper();

    QString matchMonitorWarning(const QString& sMonitorUrl, double dHostWarningValue, double dGuestWarningValue);
    bool sendEmail(const QString& sHeaderInfo, const QString& sCotentInfo, const QString& sReceiver);

private:
    PyObject* m_pWarningModule;
    PyObject* m_pEmailModule;
};

#endif

