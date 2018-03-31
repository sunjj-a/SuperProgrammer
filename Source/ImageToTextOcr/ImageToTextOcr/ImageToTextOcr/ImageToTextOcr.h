#ifndef IMAGETOTEXTOCR_H
#define IMAGETOTEXTOCR_H

#include <QtWidgets/QDialog>
class QLabel;
class QLineEdit;
class QPushButton;
class QProgressBar;

class ImageToTextOcr : public QDialog
{
    Q_OBJECT

public:
    ImageToTextOcr(QWidget *parent = 0);
    ~ImageToTextOcr();

private slots:
    void selectedFile();
    void recognizedText();

private:
     void initUIForm();
     void initConnect();

private:
    QLabel* m_pLable;
    QLineEdit* m_pSelectedFileEidt;
    QPushButton* m_pSelectedFileBtn;
    QPushButton* m_pRecognizedTextBtn;
    QProgressBar* m_pProgressBar;
};

#endif // IMAGETOTEXTOCR_H
