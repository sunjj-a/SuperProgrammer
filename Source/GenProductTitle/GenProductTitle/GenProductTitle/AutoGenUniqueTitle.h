#pragma once

#include <QString>
#include <map>
#include <set>
#include <vector>
#include <unordered_set>
using namespace std;

//索引号 关键词
typedef std::vector<std::pair<int, QString>> SourceWords;

//索引号 关键字
typedef std::map<int, QString> SourceWordContainer;

//单词 所在数组
typedef std::map<QString, std::set<int>> WordCountContainer;

//单词自动排序后容器：由高到低
typedef std::vector<std::pair<QString, std::set<int>>> SortedWordContainer;

//排序后的关键索引集合
typedef std::vector<int> SortedIndexContainer;

//生成标语集合
typedef std::vector<QString> GenTitleContainer;

//未匹配上的关键字集合
typedef std::vector<QString> UnMatchWordContainer;

//生成唯一标语结果
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