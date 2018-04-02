#include "ParseWord.h"

#include <vector>

#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "xlsxdocument.h"

//////////////////////////////////////////////////////////////////////////
//读写分析原始文件

struct UserInfo
{
    QString sNum;  // 手机号
    QString sName; // 姓名
    QString sCrad; // 身份证号
    bool isOldMan; // 1985后
    bool bAdded;   // 已经处理
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
        //没有输入用户范围
        return true;
    }

    if (nLeft != -1 && nRight == -1)
    {
        //左区间
        return nAge >= nLeft;
    }

    if (nLeft != -1 && nRight != -1)
    {
        //左右区间
        return nAge >= nLeft && nAge <= nRight;
    }

    //无效
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
        //手机号
        if (bOk && sLine.size() == 11)
        {
            //姓名
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

                    //身份证
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
            sAgeFilter = QStringLiteral("%1后").arg(nMinAge);
        } 
        else if (nMinAge != -1 && nMaxAge != -1)
        {
            sAgeFilter = QStringLiteral("%1 - %2").arg(nMinAge).arg(nMaxAge);
        }
        else if (nMinAge == -1 && nMaxAge == -1)
        {
            sAgeFilter = QStringLiteral("全部");
        }

        QFileInfo oFileInfo(sFilePath);
        QDir oFileDir(sFilePath);
        QString sNewFileName = oFileInfo.baseName() + QStringLiteral("-(筛选%1)").arg(sAgeFilter);
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
        QString sNewFileName = oFileInfo.baseName() + QStringLiteral("-(手机号)");
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
        QString sNewFileName = oFileInfo.baseName() + QStringLiteral("-(姓名手机号)");
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
        //手机号
        if (bOk && sLine.size() == 11)
        {
            //姓名
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

                    //身份证
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

                            bool bRejected = false; //拒绝
                            auto pRejectIter = pIter;
                            ++pRejectIter;
                            ++pRejectIter;
                            for (int nIndex = 0; nIndex < 4; ++nIndex)
                            {
                                if (pRejectIter != oLinesVector.end() && (*pRejectIter).contains(QStringLiteral("拒绝")))
                                {
                                    bRejected = true;
                                    break;
                                } 
                                else
                                {
                                    ++pRejectIter;
                                }
                            }

                            //没有被拒绝的不加入
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
        QString sNewFileName = oFileInfo.baseName() + QStringLiteral("-(审批拒绝)");
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

    m_pInfo = new QLabel(QStringLiteral("原始数据："));
    m_pFileEdit = new QLineEdit;
    m_pSelect = new QPushButton(QStringLiteral("选择文件"));
    m_pBegin = new QPushButton(QStringLiteral("信息提取"));
    m_pCheck = new QPushButton(QStringLiteral("提取未通过用户"));

    m_pUserAgeLbl = new QLabel(QStringLiteral("年龄范围："));
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

    setWindowTitle(QStringLiteral("信息自动提取"));
    resize(500, 50);
    
}

ParseWord::~ParseWord()
{

}

void ParseWord::selectFile()
{
    QString sFile = QFileDialog::getOpenFileName(this, QStringLiteral("选择目标文件"));
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
    QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("提取成功"));
}

void ParseWord::checkReject()
{
    QString sFile = m_pFileEdit->text();
    parseCheckResult(sFile);
    QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("提取成功"));
}
