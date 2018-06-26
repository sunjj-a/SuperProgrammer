#pragma once
//////////////////////////////////////////////////////////////////////////
//����ʵ����Ϣ

#include <QVector>
#include <QSet>
#include <QMap>
#include "SModelConst.h"

//����������Ϣ
struct SSaleInfo
{
    QString sOrderNum;
    QString sTitile;
    QString sPrice;
    QString sBuyNum;
    QString sSystemNum;
    QString sProperty;
    QString sTCInfo;
    QString sRemark;
    QString sState;
    QString sSellerNum;
};

//ԭʼ������Ϣ��
typedef QVector<QStringList> SSourceInfos;
//������Ϣ����
typedef QVector<SSaleInfo*> SSaleRecords;
//���۶���״̬
typedef QSet<QString> SOrderStates;
//�ⲿϵͳ���룬������
typedef QMap<QString, QString> SSaleGoods;

class SSaleRecordEO
{
public:
    SSaleRecordEO(const SColumnOrders& oColOrders, const SSourceInfos& oSourceInfos);
    ~SSaleRecordEO();

    //�������۶���״̬����������Ϣ
    void findSaleInfo(const SOrderStates& oOrderStates, SSaleRecords& oSaleInfos) const;

    //�������۶���״̬����ϵͳ����/��������Ϣ
    void findSaleInfo(const SOrderStates& oOrderStates, SSaleGoods& oSaleGoods) const;

private:
    void fillSaleInfos(const SColumnOrders& oColOrders, const SSourceInfos& oSourceInfos);
    void freeSaleInfo();
private:
    SSaleRecords* m_pSaleRecords;
};
