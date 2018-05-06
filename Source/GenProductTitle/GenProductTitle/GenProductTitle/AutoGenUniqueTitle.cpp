#include "AutoGenUniqueTitle.h"

#define CHAR_COUNT 40

AutoGenUniqueTitle::AutoGenUniqueTitle()
{
    m_pSourceWordContainer = std::make_shared<SourceWordContainer>();
    m_pWordCountContainer = std::make_shared<WordCountContainer>();
    m_pSortedWordContainer = std::make_shared<SortedWordContainer>();
    m_pGenTitleContainer = std::make_shared<GenTitleContainer>();
    m_pSortedIndexContainer = std::make_shared<SortedIndexContainer>();
}

std::shared_ptr<GenTitleContainer> AutoGenUniqueTitle::autoGenTitle(const SourceWords& oSourceWords)
{
    initSourceWords(oSourceWords);
    calcWordFrequence();
    sortWordFrequence();
    calcSortedIndex();
    genUniqueTitle();
    return m_pGenTitleContainer;
}

void AutoGenUniqueTitle::initSourceWords(const SourceWords& oSourceWords)
{
    for (auto pIter = oSourceWords.begin(); pIter != oSourceWords.end(); ++pIter)
    {
        int nIndex = pIter->first;
        QString sSourceWord = pIter->second;
        sSourceWord = sSourceWord.trimmed();
        sSourceWord.replace(" ", "");
        m_pSourceWordContainer->insert(std::make_pair(nIndex, sSourceWord));
    }
}


void AutoGenUniqueTitle::calcWordFrequence()
{
    m_pWordCountContainer->clear();
    for (auto pIter = m_pSourceWordContainer->begin(); pIter !=  m_pSourceWordContainer->end(); ++pIter)
    {
        int nIndex = pIter->first;
        QString sKeyWord = pIter->second;

        for (auto pIter = sKeyWord.begin(); pIter != sKeyWord.end(); ++pIter)
        {
            auto pFindIter = m_pWordCountContainer->find(*pIter);
            if (pFindIter != m_pWordCountContainer->end())
            {
                std::set<int>& oIndexSet = pFindIter->second;
                oIndexSet.insert(nIndex);
            } 
            else
            {
                std::set<int> oIndexSet;
                oIndexSet.insert(nIndex);
                m_pWordCountContainer->insert(std::make_pair(*pIter, oIndexSet));
            }
        }
    }
}

bool compareWordFrequence(const std::pair<QString, std::set<int>>& oFirst, 
                       const std::pair<QString, std::set<int>>& oSecond)
{
    return oFirst.second.size() > oSecond.second.size();
}

void AutoGenUniqueTitle::sortWordFrequence()
{
    m_pSortedWordContainer->clear();
    for (auto pIter = m_pWordCountContainer->begin(); pIter != m_pWordCountContainer->end(); ++pIter)
    {
        std::pair<QString, std::set<int>> oWordFrequence(pIter->first, pIter->second);
        m_pSortedWordContainer->push_back(oWordFrequence);
    }
    std::sort(m_pSortedWordContainer->begin(), m_pSortedWordContainer->end(), compareWordFrequence);
}

void AutoGenUniqueTitle::calcSortedIndex()
{
    m_pSortedIndexContainer->clear();
    for (auto pIter = m_pSortedWordContainer->begin(); pIter != m_pSortedWordContainer->end(); ++pIter)
    {
        std::set<int> oIndexSet = pIter->second;
        for (auto pIter = oIndexSet.begin(); pIter != oIndexSet.end(); ++pIter)
        {
            auto pFindIter = std::find(m_pSortedIndexContainer->begin(), m_pSortedIndexContainer->end(), *pIter);
            if (pFindIter == m_pSortedIndexContainer->end())
                m_pSortedIndexContainer->push_back(*pIter);
        }
    }
}


void AutoGenUniqueTitle::genUniqueTitle()
{
    m_pGenTitleContainer->clear();

    QString sNewKeyWord("");
    for (auto pIter = m_pSortedIndexContainer->begin(); pIter != m_pSortedIndexContainer->end(); ++pIter)
    {
        QString sOldKeyWord = m_pSourceWordContainer->find(*pIter)->second;

        //�±��Ｏ���Ѿ����ڸñ���
        if (existedKeyWord(sOldKeyWord))
            continue;

        //ƴ���µĺ��ϵĹؼ���
        if (!concatKeyWords(sNewKeyWord, sOldKeyWord))
        {
            m_pGenTitleContainer->push_back(sNewKeyWord);
            sNewKeyWord = sOldKeyWord;
        }
    }
}

//�����ɵı��Ｏ�����Ƿ�������йؼ���
bool AutoGenUniqueTitle::existedKeyWord(const QString& sOldKeyWord)
{
    for (auto pIter = m_pGenTitleContainer->begin(); pIter != m_pGenTitleContainer->end(); ++pIter)
    {
        QString sNewKeyWord = *pIter;
        bool bExistedInNewKeyWord = true;
        for (auto pIter = sOldKeyWord.begin(); pIter != sOldKeyWord.end(); ++pIter)
        {
            QString sWord = *pIter;
            if (!sNewKeyWord.contains(sWord))
            {
                bExistedInNewKeyWord = false;
                continue;
            }
        }

        if (bExistedInNewKeyWord)
        {
            return true;
        }
    }

    return false;
}

//ƴ�ӹؼ���
bool AutoGenUniqueTitle::concatKeyWords(QString& sNewKeyWord, const QString& sOldKeyWord)
{
    QString sBackUpKeyWord = sNewKeyWord;
    for (auto pIter = sOldKeyWord.begin(); pIter != sOldKeyWord.end(); ++pIter)
    {
        QString sWord = *pIter;
        if (!sNewKeyWord.contains(sWord))
            sNewKeyWord.push_back(sWord);
    }

    if (!isValidKeyWord(sNewKeyWord))
    {
        sNewKeyWord = sBackUpKeyWord;
        return false;
    }

    return true;
}

//ʹ�ú�����Ч���±��� <= 40���ַ�
bool AutoGenUniqueTitle::isValidKeyWord(const QString& sNewKeyWord)
{ 
    int nCharCount = 0;
    for(int nIndex = 0 ; nIndex < sNewKeyWord.length(); nIndex++)  
    {  
        ushort uUnicode = sNewKeyWord.at(nIndex).unicode();
        if(uUnicode >= 0x4E00 && uUnicode <= 0x9FA5)  
        {  
            nCharCount += 2;
        } 
        else
        {
            nCharCount += 1;
        }
    }  
    return nCharCount <= CHAR_COUNT;
}
