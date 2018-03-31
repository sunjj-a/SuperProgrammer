#include "MmwPythonOper.h"
#pragma execution_character_set("utf-8")

#define FreeAndNilPyObject(pPyObject) {if (pPyObject){Py_DECREF(pPyObject); pPyObject = nullptr;}}

///////////////////////////////////////////////////////////////////////////
//���캯��
MmwPythonOper::MmwPythonOper()
{
    Py_Initialize();
    m_pWarningModule = PyImport_ImportModule("MatchMonitorScript");
    m_pEmailModule = PyImport_ImportModule("AutoSendEmailScript");
}

///////////////////////////////////////////////////////////////////////////
//��������
MmwPythonOper::~MmwPythonOper()
{
    FreeAndNilPyObject(m_pEmailModule);
    FreeAndNilPyObject(m_pWarningModule);
    Py_Finalize();
}

//////////////////////////////////////////////////////////////////////////
//��س�����վ Ԥ��ֵ
QString MmwPythonOper::matchMonitorWarning(const QString& sMonitorUrl, double dHostWarningValue, double dGuestWarningValue)
{
    PyObject *pArgsParam = Py_BuildValue("(sdd)", sMonitorUrl.toStdString().c_str(), dHostWarningValue, dGuestWarningValue); 
    PyObject *pFunction = PyObject_GetAttrString(m_pWarningModule, "matchMonintor");
    PyObject *pReturn = PyEval_CallObject(pFunction, pArgsParam); 

    char* pResult = nullptr;
    PyArg_Parse(pReturn, "s", &pResult);
    QString sResult(pResult);

    FreeAndNilPyObject(pResult);
    FreeAndNilPyObject(pFunction);
    FreeAndNilPyObject(pArgsParam);

    return sResult;
}

//////////////////////////////////////////////////////////////////////////
//�Զ������ʼ�
bool MmwPythonOper::sendEmail(const QString& sHeaderInfo, const QString& sCotentInfo, const QString& sReceiver)
{
    PyObject *pArgsParam = Py_BuildValue("(sss)", sHeaderInfo.toStdString().c_str(), sCotentInfo.toStdString().c_str(), sReceiver.toStdString().c_str()); 
    PyObject *pFunction = PyObject_GetAttrString(m_pEmailModule, "sendEmail");
    PyObject *pReturn = PyEval_CallObject(pFunction, pArgsParam); 

    char* pResult = nullptr;
    PyArg_Parse(pReturn, "s", &pResult);
    QString sResult(pResult);

    FreeAndNilPyObject(pReturn);
    FreeAndNilPyObject(pFunction);
    FreeAndNilPyObject(pArgsParam);
    
    return (sResult == "true");
}



