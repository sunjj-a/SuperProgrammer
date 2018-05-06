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

class AutoGenUniqueTitle
{
public:
    AutoGenUniqueTitle();
    std::shared_ptr<GenTitleContainer> autoGenTitle(const SourceWords& oSourceWords);

private:
    void initSourceWords(const SourceWords& oSourceWords);
    void calcWordFrequence();
    void sortWordFrequence();
    void calcSortedIndex();
    void genUniqueTitle();

    bool existedKeyWord(const QString& sOldKeyWord);
    bool concatKeyWords(QString& sNewKeyWord, const QString& sOldKeyWord);
    bool isValidKeyWord(const QString& sNewKeyWord);

private:
    std::shared_ptr<SourceWordContainer> m_pSourceWordContainer;
    std::shared_ptr<WordCountContainer> m_pWordCountContainer;
    std::shared_ptr<SortedWordContainer> m_pSortedWordContainer;
    std::shared_ptr<GenTitleContainer> m_pGenTitleContainer;
    std::shared_ptr<SortedIndexContainer> m_pSortedIndexContainer;
};