#include "ParseWord.h"

#include <vector>

#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "xlsxdocument.h"

//////////////////////////////////////////////////////////////////////////
//��д����ԭʼ�ļ�

struct UserInfo
{
    QString sNum;  // �ֻ���
    QString sName; // ����
    QString sCrad; // ���֤��
    bool isOldMan; // 1985��
    bool bAdded;   // �Ѿ�����
public:
    UserInfo()
    {
        sNum = "";
        sName = "";
        sCrad = "";
        isOldMan = false;
        bAdded = false;
    }
};

bool filterAge(int nAge, int nLeft, int nRight)
{
    if (nLeft == -1 && nRight == -1)
    {
        //û�������û���Χ
        return true;
    }

    if (nLeft != -1 && nRight == -1)
    {
        //������
        return nAge >= nLeft;
    }

    if (nLeft != -1 && nRight != -1)
    {
        //��������
        return nAge >= nLeft && nAge <= nRight;
    }

    //��Ч
    return false;
}

void parseWordOper(const QString& sFilePath, int nMinAge, int nMaxAge)
{
    QFile oReadFile(sFilePath);  
    if (!oReadFile.open(QIODevice::ReadOnly | QIODevice::Text))  
        return;  

    std::vector<QString> oLinesVector;
    QTextStream oReadTextStream(&oReadFile);  
    QString line = oReadTextStream.readLine().trimmed();  
    while (!line.isNull()) {  
        qDebug() << line;  
        if (!line.isEmpty())
            oLinesVector.push_back(line);
        
        line = oReadTextStream.readLine();  
    }  

    std::vector<UserInfo> oUserInfo;
    for (auto pIter = oLinesVector.begin(); pIter != oLinesVector.end(); ++pIter)
    {
        QString sLine = *pIter;
        if (sLine.isEmpty())
            continue;

        bool bOk;
        sLine.toLongLong(&bOk);
        //�ֻ���
        if (bOk && sLine.size() == 11)
        {
            //����
            auto pNameNext = pIter;
            ++pNameNext;
            if (pNameNext != oLinesVector.end())
            {
                UserInfo user;
                QString sName = *pNameNext;
                if (sName.contains(QRegExp("[\\x4e00-\\x9fa5]+")))
                {
                    user.sNum = sLine;
                    user.sName = sName;

                    //���֤
                    auto pCardNext = pIter;
                    ++pCardNext;
                    ++pCardNext;
                    ++pCardNext;
                    if (pCardNext != oLinesVector.end())
                    {
                        QString sCard = *pCardNext;
                        bool bOk = false;
                        sCard.toLongLong(&bOk);

                        bool bLastWithX = sCard.endsWith("x", Qt::CaseInsensitive);
                        if ((bOk || bLastWithX) && sCard.size() >= 18)
                        {
                            user.sCrad = sCard;
                            QString sYear = sCard.mid(6, 4);
                            int nYear = sYear.toInt();
                            if (filterAge(nYear, nMinAge, nMaxAge))
                            {
                                user.isOldMan = true;
                            }
                        }
                    }

                    oUserInfo.push_back(user);
                }
                else
                {
                    user.sNum = sLine;
                    oUserInfo.push_back(user);
                }
            }
        }
    }


    {
        QString sAgeFilter = "";


        if (nMinAge != -1 && nMaxAge == -1)
        {
            sAgeFilter = QStringLiteral("%1��").arg(nMinAge);
        } 
        else if (nMinAge != -1 && nMaxAge != -1)
        {
            sAgeFilter = QStringLiteral("%1 - %2").arg(nMinAge).arg(nMaxAge);
        }
        else if (nMinAge == -1 && nMaxAge == -1)
        {
            sAgeFilter = QStringLiteral("ȫ��");
        }

        QFileInfo oFileInfo(sFilePath);
        QDir oFileDir(sFilePath);
        QString sNewFileName = oFileInfo.baseName() + QStringLiteral("-(ɸѡ%1)").arg(sAgeFilter);
        QString sNewFile = oFileInfo.absolutePath() + "//" +sNewFileName + ".xlsx";

        QXlsx::Document oDocument(sNewFile);
        oDocument.setColumnWidth(1, 20);
        oDocument.setColumnWidth(2, 12);
        oDocument.setColumnWidth(3, 26);

        int nUserCount = 1;
        for (size_t nIndex = 0; nIndex < oUserInfo.size(); ++nIndex)
        {
            UserInfo user = oUserInfo.at(nIndex);
            if (user.isOldMan && !user.bAdded)
            {
                oUserInfo[nIndex].bAdded = true;
                oDocument.write(nUserCount, 1, user.sNum);
                oDocument.write(nUserCount, 2, user.sName);
                oDocument.write(nUserCount, 3, user.sCrad);
                ++nUserCount;
            }
        }

        oDocument.save();
    }

    {
        QFileInfo oFileInfo(sFilePath);
        QDir oFileDir(sFilePath);
        QString sNewFileName = oFileInfo.baseName() + QStringLiteral("-(�ֻ���)");
        QString sNewFile = oFileInfo.absolutePath() + "//" +sNewFileName + ".xlsx";

        QXlsx::Document oDocument(sNewFile);
        oDocument.setColumnWidth(1, 20);

        int nUserCount = 1;
        for (size_t nIndex = 0; nIndex < oUserInfo.size(); ++nIndex)
        {
            UserInfo user = oUserInfo.at(nIndex);
            if (!user.sNum.isEmpty() && user.sName.isEmpty() && !user.bAdded)
            {
                oUserInfo[nIndex].bAdded = true;
                oDocument.write(nUserCount, 1, user.sNum);
                ++nUserCount;
            }
        }

        oDocument.save();
    }

    {
        QFileInfo oFileInfo(sFilePath);
        QDir oFileDir(sFilePath);
        QString sNewFileName = oFileInfo.baseName() + QStringLiteral("-(�����ֻ���)");
        QString sNewFile = oFileInfo.absolutePath() + "//" + sNewFileName + ".xlsx";

        QXlsx::Document oDocument(sNewFile);
        oDocument.setColumnWidth(1, 20);
        oDocument.setColumnWidth(2, 12);
        oDocument.setColumnWidth(3, 30);

        //     QFile oWriteFile(sNewFile);   
        //     if(!oWriteFile.open(QIODevice::ReadWrite | QIODevice::Text)) {   
        //         qDebug()<<"Can't open the file!"<<endl;   
        //     }   

        //    QTextStream oWriteTextStream(&oWriteFile);

        int nUserCount = 1;
        for (size_t nIndex = 0; nIndex < oUserInfo.size(); ++nIndex)
        {
            UserInfo user = oUserInfo.at(nIndex);
            if (!user.sNum.isEmpty() && !user.sName.isEmpty() && !user.bAdded)
            {
                oUserInfo[nIndex].bAdded = true;
                oDocument.write(nUserCount, 1, user.sNum);
                oDocument.write(nUserCount, 2, user.sName);
                oDocument.write(nUserCount, 3, user.sCrad);
                ++nUserCount;
            }
        }

        oDocument.save();
        //     oWriteFile.flush();
        //     oWriteFile.close();
    }
}


void parseCheckResult(const QString& sFilePath)
{
    QFile oReadFile(sFilePath);  
    if (!oReadFile.open(QIODevice::ReadOnly | QIODevice::Text))  
        return;  

    std::vector<QString> oLinesVector;
    QTextStream oReadTextStream(&oReadFile);  
    QString line = oReadTextStream.readLine().trimmed();  
    while (!line.isNull()) {  
        qDebug() << line;  
        if (!line.isEmpty())
            oLinesVector.push_back(line);

        line = oReadTextStream.readLine();  
    }  

    std::vector<UserInfo> oUserInfo;
    for (auto pIter = oLinesVector.begin(); pIter != oLinesVector.end(); ++pIter)
    {
        QString sLine = *pIter;
        if (sLine.isEmpty())
            continue;

        bool bOk;
        sLine.toLongLong(&bOk);
        //�ֻ���
        if (bOk && sLine.size() == 11)
        {
            //����
            auto pNameNext = pIter;
            ++pNameNext;
            if (pNameNext != oLinesVector.end())
            {
                UserInfo user;
                QString sName = *pNameNext;
                if (sName.contains(QRegExp("[\\x4e00-\\x9fa5]+")))
                {
                    user.sNum = sLine;
                    user.sName = sName;

                    //���֤
                    auto pCardNext = pIter;
                    --pCardNext;
                    if (pCardNext != oLinesVector.end())
                    {
                        QString sCard = *pCardNext;
                        bool bOk = false;
                        sCard.toLongLong(&bOk);

                        bool bLastWithX = sCard.endsWith("x", Qt::CaseInsensitive);
                        if ((bOk || bLastWithX) && sCard.size() >= 18)
                        {
                            user.sCrad = sCard;

                            bool bRejected = false; //�ܾ�
                            auto pRejectIter = pIter;
                            ++pRejectIter;
                            ++pRejectIter;
                            for (int nIndex = 0; nIndex < 4; ++nIndex)
                            {
                                if (pRejectIter != oLinesVector.end() && (*pRejectIter).contains(QStringLiteral("�ܾ�")))
                                {
                                    bRejected = true;
                                    break;
                                } 
                                else
                                {
                                    ++pRejectIter;
                                }
                            }

                            //û�б��ܾ��Ĳ�����
                            if (!bRejected)
                            {
                                continue;
                            }
                        }
                    }

                    oUserInfo.push_back(user);
                }
//                 else
//                 {
//                     user.sNum = sLine;
//                     oUserInfo.push_back(user);
//                 }
            }
        }
    }


    {
        QFileInfo oFileInfo(sFilePath);
        QDir oFileDir(sFilePath);
        QString sNewFileName = oFileInfo.baseName() + QStringLiteral("-(�����ܾ�)");
        QString sNewFile = oFileInfo.absolutePath() + "//" +sNewFileName + ".xlsx";

        QXlsx::Document oDocument(sNewFile);
        oDocument.setColumnWidth(1, 20);
        oDocument.setColumnWidth(2, 12);
        oDocument.setColumnWidth(3, 26);

        int nUserCount = 1;
        for (size_t nIndex = 0; nIndex < oUserInfo.size(); ++nIndex)
        {
            UserInfo user = oUserInfo.at(nIndex);
            oDocument.write(nUserCount, 1, user.sNum);
            oDocument.write(nUserCount, 2, user.sName);
            oDocument.write(nUserCount, 3, user.sCrad);
            ++nUserCount;
        }

        oDocument.save();
    }
}

//////////////////////////////////////////////////////////////////////////


#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMessageBox>

ParseWord::ParseWord(QWidget *parent)
    : QDialog(parent)
{
    //ui.setupUi(this);

    m_pInfo = new QLabel(QStringLiteral("ԭʼ���ݣ�"));
    m_pFileEdit = new QLineEdit;
    m_pSelect = new QPushButton(QStringLiteral("ѡ���ļ�"));
    m_pBegin = new QPushButton(QStringLiteral("��Ϣ��ȡ"));
    m_pCheck = new QPushButton(QStringLiteral("��ȡδͨ���û�"));

    m_pUserAgeLbl = new QLabel(QStringLiteral("���䷶Χ��"));
    m_pUserAgeEdt = new QLineEdit();

    m_pBegin->setVisible(false);
    m_pUserAgeLbl->setVisible(false);
    m_pUserAgeEdt->setVisible(false);

    connect(m_pSelect, SIGNAL(clicked()), this, SLOT(selectFile()));
    connect(m_pBegin, SIGNAL(clicked()), this, SLOT(beginParse()));
    connect(m_pCheck, SIGNAL(clicked()), this, SLOT(checkReject()));

    QVBoxLayout* pMain = new QVBoxLayout;
    QHBoxLayout* pTopHBox = new QHBoxLayout;
    pTopHBox->addWidget(m_pInfo);
    pTopHBox->addWidget(m_pFileEdit);
    pTopHBox->addWidget(m_pSelect);
    pTopHBox->addWidget(m_pBegin);
    pTopHBox->addWidget(m_pCheck);
    
    QHBoxLayout* pBottomHBox = new QHBoxLayout;
    pBottomHBox->addWidget(m_pUserAgeLbl);
    pBottomHBox->addWidget(m_pUserAgeEdt);

    pMain->addLayout(pTopHBox);
    pMain->addLayout(pBottomHBox);

    setLayout(pMain);

    setWindowTitle(QStringLiteral("��Ϣ�Զ���ȡ"));
    resize(500, 50);
    
}

ParseWord::~ParseWord()
{

}

void ParseWord::selectFile()
{
    QString sFile = QFileDialog::getOpenFileName(this, QStringLiteral("ѡ��Ŀ���ļ�"));
    if (!sFile.isEmpty())
        m_pFileEdit->setText(sFile);
}

void ParseWord::beginParse()
{
    QString sFile = m_pFileEdit->text();
    QString sFilterAge = m_pUserAgeEdt->text();

    int nMinAge = -1, nMaxAge = -1;
    if (sFilterAge.isEmpty())
    {
        nMinAge = -1;
        nMaxAge = -1;
    } 
    else if (sFilterAge.contains("-"))
    {
        QStringList oFilerlist = sFilterAge.split("-");
        nMinAge = oFilerlist.at(0).trimmed().toInt();
        nMaxAge = oFilerlist.at(1).trimmed().toInt();
    }
    else
    {
        nMinAge = sFilterAge.toInt();
    }

    parseWordOper(sFile, nMinAge, nMaxAge);
    QMessageBox::information(this, QStringLiteral("��ʾ"), QStringLiteral("��ȡ�ɹ�"));
}

void ParseWord::checkReject()
{
    QString sFile = m_pFileEdit->text();
    parseCheckResult(sFile);
    QMessageBox::information(this, QStringLiteral("��ʾ"), QStringLiteral("��ȡ�ɹ�"));
}
