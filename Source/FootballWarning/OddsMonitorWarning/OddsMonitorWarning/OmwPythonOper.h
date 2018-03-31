//#pragma once
#ifndef OWMPYTHONOPER_H
#define OWMPYTHONOPER_H

//////////////////////////////////////////////////////////////////////////
//���Ԥ��py������

#include "Python.h"
#include <QString>

class OmwPythonOper
{
public:
    OmwPythonOper();
    ~OmwPythonOper();

    QString oddsMonitorWarning(const QString& sMonitorUrl, double dWarningValue);
    bool sendEmail(const QString& sHeaderInfo, const QString& sCotentInfo, const QString& sReceiver);

private:
    PyObject* m_pWarningModule;
    PyObject* m_pEmailModule;
};

#endif

