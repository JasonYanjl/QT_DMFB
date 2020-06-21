#ifndef CLEANINGDIALOG_H
#define CLEANINGDIALOG_H

#include <QDialog>

namespace Ui {
class CleaningDialog;
}

class CleaningDialog : public QDialog
{
    Q_OBJECT

signals:
    void buildCleaning(int,int,int,int,bool);

public:
    explicit CleaningDialog(QWidget *parent = nullptr);
    ~CleaningDialog();

private slots:
    void on_pushButtonOK_clicked();

private:
    Ui::CleaningDialog *ui;
};

#endif // CLEANINGDIALOG_H
