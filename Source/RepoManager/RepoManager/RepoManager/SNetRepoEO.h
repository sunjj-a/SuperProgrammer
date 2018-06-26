#pragma once
//////////////////////////////////////////////////////////////////////////
//网仓数据实体
#include <QVector>
#include <QSet>
#include <QString>
#include <QVariantList>
#include "SModelConst.h"

//单条网仓记录
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

//网仓数据集合
typedef QVector<SNetRepoInfo*> SNetRepoInfos;
//货主集合
typedef QSet<QString> SSellerSet;
//原始网仓数据
typedef QVariantList SSourceNetRepos;

class SNetRepoEO
{
public:
    SNetRepoEO(const SColumnOrders& oColOrders, const SSourceNetRepos& oSourceInfos);
    ~SNetRepoEO();
    
    //查找指定获取的网仓数据
    void findNetRepoInfo(const SSellerSet& oSellerSet, SNetRepoInfos& oNetRepoInfos) const;

private:
    void fillNetRepoInfos(const SColumnOrders& oColOrders, const SSourceNetRepos& oSourceInfos);
    void freeNetRepoInfos();

private:
    SNetRepoInfos* m_pNetRepoInfos;
};