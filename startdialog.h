#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QDialog>

namespace Ui {
class StartDialog;
}

class StartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartDialog(QWidget *parent = nullptr);
    ~StartDialog();

private:
    Ui::StartDialog *ui;
    int Row=0,Column=0;
    bool check(int x,int y);

signals:
    void buildDMFB(int,int);
    void InLocationSet(int,int);
    void OutLocationSet(int,int);
    void AutoCleanSet(bool);

private slots:
    void on_pushButtonRCSet_clicked();
    void on_pushButtonAdd_clicked();
    void on_pushButtonSet_clicked();
    void on_radioButtonAutoClean_clicked(bool checked);
    void on_pushButtonOK_clicked();
};

#endif // STARTDIALOG_H
