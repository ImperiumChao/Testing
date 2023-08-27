#include "adduser.h"
#include "ui_adduser.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

addUser::addUser(QWidget *parent, QString group) :
    QWidget(parent),
    ui(new Ui::addUser)
{
    ui->setupUi(this);
    QSqlQuery query("select name_group from groups");
    ui->comboGroup->addItem("-");
    while (query.next())
    {
        ui->comboGroup->addItem(query.value(0).toString());
    }
    ui->comboGroup->setCurrentText(group);
}

void addUser::fillData(QString id)
{
    QSqlQuery query;
    query.prepare("select name, surname, secondname, id_role, name_group, login, password from users where id_user=:id");
    query.bindValue(":id", id);
    query.exec();
    query.next();
    ui->lineName->setText(query.value(0).toString());
    ui->lineSName->setText(query.value(1).toString());
    ui->lineSecName->setText(query.value(2).toString());
    ui->comboRole->setCurrentIndex(query.value(3).toInt(0)-1);
    ui->comboGroup->setCurrentText(query.value(4).toString());
    ui->lineLogin->setText(query.value(5).toString());
    ui->linePassword->setText(query.value(6).toString());
    ui->pushCreate->setText("Сохранить");
    u=id;
}

addUser::~addUser()
{
    delete ui;
}



void addUser::on_pushCancel_clicked()
{
    close();
}

void addUser::on_pushCreate_clicked()
{
    bool r = ui->lineName->text()=="" || ui->lineSName->text()=="" || ui->lineSecName->text()=="" || ui->lineLogin->text()=="" ||
            ui->linePassword->text()=="" || ui->comboRole->currentIndex()==0;
    if (r)
    {
        QMessageBox::warning(0, "Ошибка", "Не все нужные поля заполнены");
        return;
    }
    if (ui->pushCreate->text()=="Сохранить")
    {
        QSqlQuery query;
        query.prepare("update users set name=:name, surname=:surname, secondname=:secondname, id_role=:id_role, login=:login, password=:password, name_group=:name_group"
                      " where id_user=:id");
        query.bindValue(":name", ui->lineName->text());
        query.bindValue(":surname", ui->lineSName->text());
        query.bindValue(":secondname", ui->lineSecName->text());
        query.bindValue(":id_role", ui->comboRole->currentIndex()+1);
        query.bindValue(":login", ui->lineLogin->text());
        query.bindValue(":password", ui->linePassword->text());
        query.bindValue(":id", u);
        if (!(ui->comboGroup->currentText()=="-")) query.bindValue(":name_group", ui->comboGroup->currentText());
        int a = ui->comboRole->currentIndex()+1;
        if (!query.exec()) QMessageBox::warning(0, "Ошибка", query.lastError().text());
        else
        {
            emit addedUser(ui->comboGroup->currentText());
            close();
        }

        return;


    }
    QSqlQuery query;
    query.prepare("insert into users(name, surname, secondname, id_role, login, password, name_group)"
                  " values(:name, :surname, :secondname, :id_role, :login, :password, :name_group)");
    query.bindValue(":name", ui->lineName->text());
    query.bindValue(":surname", ui->lineSName->text());
    query.bindValue(":secondname", ui->lineSecName->text());
    query.bindValue(":id_role", ui->comboRole->currentIndex()+1);
    query.bindValue(":login", ui->lineLogin->text());
    query.bindValue(":password", ui->linePassword->text());
    if (!(ui->comboGroup->currentText()=="-")) query.bindValue(":name_group", ui->comboGroup->currentText());
    int a = ui->comboRole->currentIndex()+1;

    if (!query.exec()) QMessageBox::warning(0, "Ошибка", query.lastError().text());
    else
    {
        emit addedUser(ui->comboGroup->currentText());
        close();
    }
}
