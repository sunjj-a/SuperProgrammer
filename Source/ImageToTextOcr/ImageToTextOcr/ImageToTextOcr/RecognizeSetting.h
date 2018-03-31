#ifndef RECOGNIZESETTING_H
#define RECOGNIZESETTING_H

//////////////////////////////////////////////////////////////////////////
//ʶ������:�ض��ַ�ʵ���滻

#include <QMap>

class QXmlStreamReader;

class RecognizeSetting
{
public:
    static RecognizeSetting* instance();
    ~RecognizeSetting();

    bool exist(const QChar& oChar, QChar& oReplaceChar);

private:
    RecognizeSetting();
    void initSetting();
    void readPatternRule();

private:
    QMap<QChar, QChar>* m_pSettingMap;
    QXmlStreamReader* m_pXMLReader;
};

#endif