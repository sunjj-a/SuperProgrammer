#ifndef RECOGNIZETEXTFACADE_H
#define RECOGNIZETEXTFACADE_H

//////////////////////////////////////////////////////////////////////////
// 识别文字门面类

#include <QStringList>

class QProgressBar;

class RecognizeTextFacade
{
public:
    void autoRecognizeText(const QStringList& sRecognizeExcels);
    void setRegognizeBar(QProgressBar* pProgressBar);

private:
    QProgressBar* m_pProgressBar;
};
#endif