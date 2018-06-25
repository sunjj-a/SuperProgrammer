#ifndef REPOMANAGER_H
#define REPOMANAGER_H

#include <QtWidgets/QDialog>
#include "ui_RepoManager.h"

class RepoManager : public QDialog
{
    Q_OBJECT

public:
    RepoManager(QWidget *parent = 0);
    ~RepoManager();

private:
    Ui::RepoManagerClass ui;
};

#endif // REPOMANAGER_H
