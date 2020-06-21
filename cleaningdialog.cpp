#include "cleaningdialog.h"
#include "ui_cleaningdialog.h"

CleaningDialog::CleaningDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CleaningDialog)
{
    ui->setupUi(this);

    QFont font("Microsoft YaHei");
    ui->labelIn->setFont(font);
    ui->labelIn_x->setFont(font);
    ui->labelIn_y->setFont(font);
    ui->labelOut->setFont(font);
    ui->labelOut_x->setFont(font);
    ui->labelOut_y->setFont(font);
    ui->lineEditIn_x->setFont(font);
    ui->lineEditIn_y->setFont(font);
    ui->lineEditOut_x->setFont(font);
    ui->lineEditOut_y->setFont(font);
    ui->pushButtonOK->setFont(font);
    ui->radioButtonLimit->setFont(font);

    this->setWindowTitle("Cleaning");
    ui->radioButtonLimit->setChecked(false);
}

CleaningDialog::~CleaningDialog()
{
    delete ui;
}

void CleaningDialog::on_pushButtonOK_clicked()
{
    int Inx=ui->lineEditIn_x->text().toInt();
    int Iny=ui->lineEditIn_y->text().toInt();
    int Outx=ui->lineEditOut_x->text().toInt();
    int Outy=ui->lineEditOut_y->text().toInt();
    bool ButtonCheck=ui->radioButtonLimit->isChecked();
    emit buildCleaning(Inx,Iny,Outx,Outy,ButtonCheck);
    close();
}
