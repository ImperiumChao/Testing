#include "fg.h"
#include "ui_fg.h"

fg::fg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fg)
{
    ui->setupUi(this);
    connect(ui->lineEdit, SIGNAL(returnPressed()), SLOT(on_pushButton_clicked()));
    lb = ui->label;
}

fg::~fg()
{
    delete ui;
}

void fg::on_pushButton_2_clicked()
{
    close();
}

void fg::on_pushButton_clicked()
{
    emit returnName(ui->lineEdit->text());
    close();
}
