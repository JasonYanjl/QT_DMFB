#ifndef SETCLEANDIALOG_H
#define SETCLEANDIALOG_H

#include <QDialog>

namespace Ui {
class setcleanDialog;
}

class setcleanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit setcleanDialog(QWidget *parent = nullptr);
    ~setcleanDialog();

signals:
    void AddCleanPlace(int,int);

private slots:
    void on_pushButtonAdd_clicked();

private:
    Ui::setcleanDialog *ui;
};

#endif // SETCLEANDIALOG_H
