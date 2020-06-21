#include "startdialog.h"
#include "ui_startdialog.h"
#include<QtDebug>
#include<QMessageBox>
#include<QIntValidator>

StartDialog::StartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartDialog)
{
    ui->setupUi(this);

    ui->radioButtonAutoClean->setChecked(false);

    QFont font("Microsoft YaHei");
    ui->labelIn->setFont(font);
    ui->labelOut->setFont(font);
    ui->labelRow->setFont(font);
    ui->labelColumn->setFont(font);
    ui->labelIn_x->setFont(font);
    ui->labelIn_y->setFont(font);
    ui->labelOut_x->setFont(font);
    ui->labelOut_y->setFont(font);
    ui->lineEditRow->setFont(font);
    ui->lineEditColumn->setFont(font);
    ui->lineEditIn_x->setFont(font);
    ui->lineEditIn_y->setFont(font);
    ui->lineEditOut_x->setFont(font);
    ui->lineEditOut_y->setFont(font);
    ui->pushButtonAdd->setFont(font);
    ui->pushButtonSet->setFont(font);
    ui->pushButtonRCSet->setFont(font);
    ui->radioButtonAutoClean->setFont(font);
    ui->pushButtonOK->setFont(font);

    this->setWindowTitle("Start");
}

StartDialog::~StartDialog()
{
    delete ui;
}

bool StartDialog::check(int x, int y)
{
    if (x<1 || y<1 || x>Row || y>Column) return 0;
    if (x==1 || y==1 || x==Row || y==Column) return 1;
    return 0;
}

void StartDialog::on_pushButtonRCSet_clicked()
{
    int x=ui->lineEditRow->text().toInt();
    int y=ui->lineEditColumn->text().toInt();
    if (x<=3 && y<=3) {
        QMessageBox::critical(NULL, "Warning", "Rows and columns cannot be equal to or less than 3 at the same time",
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else if (x>=13 || y>=13) {
        QMessageBox::critical(NULL, "Warning", "The number of row or column is too large",
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else {
        Row=x;
        Column=y;
        emit buildDMFB(x,y);
    }
}

void StartDialog::on_pushButtonAdd_clicked()
{
    int x=ui->lineEditIn_x->text().toInt();
    int y=ui->lineEditIn_y->text().toInt();
    if (Row==0 && Column==0) {
        QMessageBox::critical(NULL, "Warning", "Rows and columns should be defined first",
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else if (!check(x,y)) {
        QMessageBox::critical(NULL, "Warning", "Location is not at the border",
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else {
        emit InLocationSet(x,y);
    }
}

void StartDialog::on_pushButtonSet_clicked()
{
    int x=ui->lineEditOut_x->text().toInt();
    int y=ui->lineEditOut_y->text().toInt();
    if (Row==0 && Column==0) {
        QMessageBox::critical(NULL, "Warning", "Rows and columns should be defined first",
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else if (!check(x,y)) {
        QMessageBox::critical(NULL, "Warning", "Location is not at the border",
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else {
        emit OutLocationSet(x,y);
    }
}

void StartDialog::on_radioButtonAutoClean_clicked(bool checked)
{
    emit AutoCleanSet(checked);
}

void StartDialog::on_pushButtonOK_clicked()
{
    close();
}
