#include "OmwPythonOper.h"
#pragma execution_character_set("utf-8")

#define FreeAndNilPyObject(pPyObject) {if (pPyObject){Py_DECREF(pPyObject); pPyObject = nullptr;}}

///////////////////////////////////////////////////////////////////////////
//���캯��
OmwPythonOper::OmwPythonOper()
{
    Py_Initialize();
    m_pWarningModule = PyImport_ImportModule("OddsMonitorScript");
    m_pEmailModule = PyImport_ImportModule("AutoSendEmailScript");
}

///////////////////////////////////////////////////////////////////////////
//��������
OmwPythonOper::~OmwPythonOper()
{
    FreeAndNilPyObject(m_pEmailModule);
    FreeAndNilPyObject(m_pWarningModule);
    Py_Finalize();
}

//////////////////////////////////////////////////////////////////////////
//���������վ Ԥ��ֵ
QString OmwPythonOper::oddsMonitorWarning(const QString& sMonitorUrl, double dWarningValue)
{
    PyObject *pArgsParam = Py_BuildValue("(sd)", sMonitorUrl.toStdString().c_str(), dWarningValue); 
    PyObject *pFunction = PyObject_GetAttrString(m_pWarningModule, "oddsMonintor");
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
bool OmwPythonOper::sendEmail(const QString& sHeaderInfo, const QString& sCotentInfo, const QString& sReceiver)
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

