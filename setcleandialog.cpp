#include "setcleandialog.h"
#include "ui_setcleandialog.h"

setcleanDialog::setcleanDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setcleanDialog)
{
    ui->setupUi(this);

    QFont font("Microsoft YaHei");
    ui->labelIn->setFont(font);
    ui->labelIn_x->setFont(font);
    ui->labelIn_y->setFont(font);
    ui->lineEditIn_x->setFont(font);
    ui->lineEditIn_y->setFont(font);
    ui->pushButtonAdd->setFont(font);

    this->setWindowTitle("Set Clean");
}

setcleanDialog::~setcleanDialog()
{
    delete ui;
}

void setcleanDialog::on_pushButtonAdd_clicked()
{
    int x=ui->lineEditIn_x->text().toInt();
    int y=ui->lineEditIn_y->text().toInt();
    emit AddCleanPlace(x,y);
}
