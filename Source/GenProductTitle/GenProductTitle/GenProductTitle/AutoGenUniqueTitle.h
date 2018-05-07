#pragma once

#include <QString>
#include <map>
#include <set>
#include <vector>
#include <unordered_set>
using namespace std;

//������ �ؼ���
typedef std::vector<std::pair<int, QString>> SourceWords;

//������ �ؼ���
typedef std::map<int, QString> SourceWordContainer;

//���� ��������
typedef std::map<QString, std::set<int>> WordCountContainer;

//�����Զ�������������ɸߵ���
typedef std::vector<std::pair<QString, std::set<int>>> SortedWordContainer;

//�����Ĺؼ���������
typedef std::vector<int> SortedIndexContainer;

//���ɱ��Ｏ��
typedef std::vector<QString> GenTitleContainer;

//δƥ���ϵĹؼ��ּ���
typedef std::vector<QString> UnMatchWordContainer;

//����Ψһ������
struct GenUniqueTitleResult
{
    std::shared_ptr<GenTitleContainer> pGenTitleContainer;
    std::shared_ptr<UnMatchWordContainer> pUnMatchWordContainer;

    GenUniqueTitleResult()
    {
        pGenTitleContainer = std::make_shared<GenTitleContainer>();
        pUnMatchWordContainer = std::make_shared<UnMatchWordContainer>();
    }

    void clear()
    {
        pGenTitleContainer->clear();
        pUnMatchWordContainer->clear();
    }
};

class AutoGenUniqueTitle
{
public:
    AutoGenUniqueTitle();
    std::shared_ptr<GenUniqueTitleResult> autoGenTitle(int nMaxMatchCount, const SourceWords& oSourceWords);

private:
    void initSourceWords(const SourceWords& oSourceWords);
    void calcWordFrequence();
    void sortWordFrequence();
    void calcSortedIndex();
    void genUniqueTitle(int nMaxMatchCount);
    void reCalcMatchWord();
    void circleReCalcMatch();

    bool existedKeyWord(const QString& sOldKeyWord);
    bool concatKeyWords(QString& sNewKeyWord, const QString& sOldKeyWord);
    bool isValidKeyWord(const QString& sNewKeyWord);
    bool exceedMaxCount(int nMaxMatchCount);

private:
    std::shared_ptr<SourceWordContainer> m_pSourceWordContainer;
    std::shared_ptr<WordCountContainer> m_pWordCountContainer;
    std::shared_ptr<SortedWordContainer> m_pSortedWordContainer;
    std::shared_ptr<GenUniqueTitleResult> m_pGenUniqueTitleResult;
    std::shared_ptr<SortedIndexContainer> m_pSortedIndexContainer;
};