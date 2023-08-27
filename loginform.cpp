#include "loginform.h"
#include "ui_loginform.h"
#include "widget.h"
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    connect(ui->lineEdit, SIGNAL(returnPressed()), SLOT(on_pushButton_clicked()));
    connect(ui->lineEdit_2, SIGNAL(returnPressed()), SLOT(on_pushButton_clicked()));

}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::on_pushButton_clicked()
{
    QSqlQuery query;
    query.prepare("select id_user, name, surname, secondname, id_role from users where login= :log and password = :pass");
    query.bindValue(":log", ui->lineEdit->text());
    query.bindValue(":pass", ui->lineEdit_2->text());



    query.exec();
    if (query.next())
    {

        emit login(QStringList() << query.value(0).toString() <<
                   query.value(1).toString() << query.value(2).toString() <<
                   query.value(3).toString() << query.value(4).toString(), query.value(4).toInt());




        close();
    }
    else QMessageBox::warning(this, "Ошибка", "Учетные данные не верны или сервер не доступен!");

}
