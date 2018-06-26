#pragma once
//////////////////////////////////////////////////////////////////////////
#include <QVector>
#include <QSet>
#include <QString>
#include <QVariantList>
#include "SModelConst.h"

struct SNetRepoInfo
{
    QString s000;
    QString sBarCode;
    QString sGoodNum;
    int nCanUsed;
    int nCanMoved;
    QString s005;
    QString s006;
    int sSellerID;
    QString sSellerName;
    QString s009;
    QString s010;
    QString s011;
    QString s012;
    QString s013;
    QString s014;
};

typedef QVector<SNetRepoInfo*> SNetRepoInfos;
typedef QSet<QString> SSellerSet;
typedef QVariantList SSourceNetRepos;

class SNetRepoEO
{
public:
    SNetRepoEO(const SColumnOrders& oColOrders, const SSourceNetRepos& oSourceInfos);
    ~SNetRepoEO();
    
    void findNetRepoInfo(const SSellerSet& oSellerSet, SNetRepoInfos& oNetRepoInfos) const;

private:
    void fillNetRepoInfos(const SColumnOrders& oColOrders, const SSourceNetRepos& oSourceInfos);
    void freeNetRepoInfos();

private:
    SNetRepoInfos* m_pNetRepoInfos;
};