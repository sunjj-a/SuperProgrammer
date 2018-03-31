#include "RecognizeSetting.h"

#include <assert.h>

#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>
#include <QCoreApplication>

//////////////////////////////////////////////////////////////////////////
//单例模式
RecognizeSetting* RecognizeSetting::instance()
{
    static RecognizeSetting oRecognizeSetting;
    return &oRecognizeSetting;
}

//////////////////////////////////////////////////////////////////////////
//构造函数
RecognizeSetting::RecognizeSetting()
    :m_pSettingMap(nullptr), m_pXMLReader(nullptr)
{
    m_pXMLReader = new QXmlStreamReader;
    m_pSettingMap = new QMap<QChar, QChar>;
    initSetting();
}

//////////////////////////////////////////////////////////////////////////
//析构函数
RecognizeSetting::~RecognizeSetting()
{
    delete m_pSettingMap;
    m_pSettingMap = nullptr;

    delete m_pXMLReader;
    m_pXMLReader = nullptr;
}

//////////////////////////////////////////////////////////////////////////
//指定非负数否存在，存在的话返回替换字符
bool RecognizeSetting::exist(const QChar& oChar, QChar& oReplaceChar)
{
    auto pFindIter = m_pSettingMap->find(oChar);
    if (pFindIter != m_pSettingMap->end())
    {
         oReplaceChar = pFindIter.value();
         return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////
//初始化文件设置
void RecognizeSetting::initSetting()
{
    QString sUserSetting = qApp->applicationDirPath()  + "/RecognizeSetting.xml";
    QFile oFile(sUserSetting);
    if (!oFile.open(QIODevice::ReadOnly))
    {
        assert(false);
        return;
    }

    m_pXMLReader->setDevice(&oFile);
    m_pXMLReader->readNext();

    while (!m_pXMLReader->atEnd())
    {
        if (m_pXMLReader->isStartElement() && m_pXMLReader->name() == "pattern")
        {
            readPatternRule();
        }
        m_pXMLReader->readNext();
    }
}

//////////////////////////////////////////////////////////////////////////
//读取匹配规则
void RecognizeSetting::readPatternRule()
{
    while (!(m_pXMLReader->isEndElement() && m_pXMLReader->name() == "pattern"))
    {
        QChar oSrcChar = m_pXMLReader->attributes().value("srcChar").toString().at(0);
        QChar oDstChar = m_pXMLReader->attributes().value("dstChar").toString().at(0);
        m_pSettingMap->insert(oSrcChar, oDstChar);
        m_pXMLReader->readNext();
    }
}



