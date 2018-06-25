#include "AutoGenUniqueTitle.h"
#include <QDebug>

#define CHAR_COUNT 40
#define CIRCLE_COUNT 3 //Ĭ�������ֱ���

AutoGenUniqueTitle::AutoGenUniqueTitle()
{
    m_pSourceWordContainer = std::make_shared<SourceWordContainer>();
    m_pWordCountContainer = std::make_shared<WordCountContainer>();
    m_pSortedWordContainer = std::make_shared<SortedWordContainer>();
    m_pGenUniqueTitleResult = std::make_shared<GenUniqueTitleResult>();
    m_pSortedIndexContainer = std::make_shared<SortedIndexContainer>();
}

std::shared_ptr<GenUniqueTitleResult> AutoGenUniqueTitle::autoGenTitle(int nMaxMatchCount, const SourceWords& oSourceWords)
{
    m_pGenUniqueTitleResult->clear();
    initSourceWords(oSourceWords);
    calcWordFrequence();
    sortWordFrequence();
    calcSortedIndex();
    genUniqueTitle(nMaxMatchCount);
    reCalcMatchWord();
    return m_pGenUniqueTitleResult;
}

void AutoGenUniqueTitle::initSourceWords(const SourceWords& oSourceWords)
{
    for (auto pIter = oSourceWords.begin(); pIter != oSourceWords.end(); ++pIter)
    {
        int nIndex = pIter->first;
        QString sSourceWord = pIter->second;
        sSourceWord = sSourceWord.trimmed();
        sSourceWord = sSourceWord.toLower();
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


void AutoGenUniqueTitle::genUniqueTitle(int nMaxMatchCount)
{
    m_pGenUniqueTitleResult->clear();

    QString sNewKeyWord("");
    for (auto pIter = m_pSortedIndexContainer->begin(); pIter != m_pSortedIndexContainer->end(); ++pIter)
    {
        QString sOldKeyWord = m_pSourceWordContainer->find(*pIter)->second;

        //���ؼ���ƥ�������Ѿ���������±���������
        if (exceedMaxCount(nMaxMatchCount))
        {
            m_pGenUniqueTitleResult->pUnMatchWordContainer->push_back(sOldKeyWord);
            continue;
        }

        //�±��Ｏ���Ѿ����ڸñ����ҿ���ƴ���µĺ��ϵĹؼ���
        if (!existedKeyWord(sOldKeyWord) && !concatKeyWords(sNewKeyWord, sOldKeyWord))
        {
            m_pGenUniqueTitleResult->pGenTitleContainer->push_back(sNewKeyWord);
            sNewKeyWord = sOldKeyWord;

            //���ؼ���ƥ����:�պõ�������±���������
            if (exceedMaxCount(nMaxMatchCount))
            {
                m_pGenUniqueTitleResult->pUnMatchWordContainer->push_back(sOldKeyWord);
                continue;
            }
        }

        //���ؼ���ƥ������δ��������±������������һ���ؼ��֣���ɹ�����Ϊ����ֵ
        if (pIter == m_pSortedIndexContainer->end() - 1)
        {
            m_pGenUniqueTitleResult->pGenTitleContainer->push_back(sNewKeyWord);
        }
    }
}

int keyWordCharSize(const QString& sNewKeyWord)
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
    return nCharCount;
}


void AutoGenUniqueTitle::reCalcMatchWord()
{
    for (int nIndex = 0; nIndex < CIRCLE_COUNT; ++nIndex)
    {
        circleReCalcMatch();
    }
}

//�µı�����ƥ��һ�ֶ���Ĺؼ���
void AutoGenUniqueTitle::circleReCalcMatch()
{
    std::shared_ptr<GenTitleContainer> pGenTitleContainer = m_pGenUniqueTitleResult->pGenTitleContainer;
    std::shared_ptr<UnMatchWordContainer> pUnMatchWordContainer = m_pGenUniqueTitleResult->pUnMatchWordContainer;
    for (auto pGenTitleIter = pGenTitleContainer->begin(); pGenTitleIter != pGenTitleContainer->end(); ++pGenTitleIter)
    {
        QString& sNewKeyWord = *pGenTitleIter;
        for (auto pUnMatchIter = pUnMatchWordContainer->begin(); pUnMatchIter != pUnMatchWordContainer->end(); ++pUnMatchIter)
        {
            QString& sOldKeyWord = *pUnMatchIter;
            if (concatKeyWords(sNewKeyWord, sOldKeyWord))
            {
                qDebug() << "Word size: " << keyWordCharSize(sNewKeyWord);
                pUnMatchWordContainer->erase(pUnMatchIter);
                break;
            }
        }
    }
}

//�����ɵı��Ｏ�����Ƿ�������йؼ���
bool AutoGenUniqueTitle::existedKeyWord(const QString& sOldKeyWord)
{
    for (auto pIter = m_pGenUniqueTitleResult->pGenTitleContainer->begin(); pIter != m_pGenUniqueTitleResult->pGenTitleContainer->end(); ++pIter)
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

bool AutoGenUniqueTitle::exceedMaxCount(int nMaxMatchCount)
{
    return m_pGenUniqueTitleResult->pGenTitleContainer->size() == nMaxMatchCount;
}