#ifndef RECOGNIZETEXTFACADE_H
#define RECOGNIZETEXTFACADE_H

//////////////////////////////////////////////////////////////////////////
// ʶ������������

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