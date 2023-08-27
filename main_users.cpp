#include "main_users.h"
#include "ui_main_users.h"
#include <QSqlQuery>
#include "Widgets/fg.h"
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include "adduser.h"

main_users::main_users(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::main_users)
{
    ui->setupUi(this);
    ui->pushDelGroup->setEnabled(false);
    updateData();
}

main_users::~main_users()
{
    delete ui;
}



void main_users::on_comboGroup_activated(const QString &arg1)
{
    if (arg1=="-") ui->pushDelGroup->setEnabled(false);
    else ui->pushDelGroup->setEnabled(true);
    gr= arg1;
    QSqlQuery query;
    query.prepare("select users.id_user, roles.name_role, users.surname, users.name, users.secondname, "
                  "users.login from users, roles where users.id_role=roles.id_role and users.name_group=:ng");
    query.bindValue(":ng", arg1);
    query.exec();
    QList<QStringList> lst;
    while (query.next())
    {
        lst.append(QStringList() << query.value(0).toString() << query.value(1).toString() << query.value(2).toString()
                   << query.value(3).toString() << query.value(4).toString() << query.value(5).toString());
    }
    ui->tableUsers->fillTable(lst, QStringList() << "ID" << "Роль" << "Фамилия" << "Имя" << "Отчество" << "Логин");
}

void main_users::updateData(QString group)
{
    QSqlQuery query("select name_group from groups");
    QString str;
    ui->comboGroup->clear();
    ui->comboGroup->addItem("-");
    while (query.next())
    {
        str=query.value(0).toString();
        groups.append(str);
        ui->comboGroup->addItem(str);
    }
    if (!group.isNull())
    {
        ui->comboGroup->setCurrentText(group);
        on_comboGroup_activated(group);

    }
}

void main_users::on_pushAddGroup_clicked()
{
    fg * f = new fg;
    f->show();
    f->setWindowTitle("Добавить группу");
    f->lb->setText("Введите название группы:");
    connect(f, SIGNAL(returnName(QString)), SLOT(addedGroup(QString)));
}

void main_users::addedGroup(QString group)
{
    QSqlQuery query;
    query.prepare("insert into groups(name_group) values(:group)");
    query.bindValue(":group", group);
    if (query.exec())
    {
        ui->comboGroup->addItem(group);
        ui->comboGroup->setCurrentIndex(ui->comboGroup->findText(group));
        on_comboGroup_activated(group);

    }
    else QMessageBox::information(0, "Ошибка", query.lastError().text());




}

void main_users::on_pushDelGroup_clicked()
{
    QMessageBox msg(this);
    msg.setText("Вы уверены, что хотите удалить группу со всеми её учениками?");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    int p=msg.exec();
    if (p==16384)
    {
        QSqlQuery query;
        query.prepare("delete from groups where name_group=:group");
        query.bindValue(":group", gr);
        if (query.exec()) updateData();
        else QMessageBox::information(0, "Ошибка", "Не удалось удалить группу");
        on_comboGroup_activated("-");


    }

}

void main_users::on_pushAddUser_clicked()
{
    addUser *a = new addUser(0, gr);
    a->show();
    connect(a, SIGNAL(addedUser(QString)), SLOT(updateData(QString)));

}



void main_users::on_pushDelUser_clicked()
{
    QMessageBox msg(this);
    msg.setText("Вы уверены, что хотите удалить пользователя?");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    int p=msg.exec();
    if (p==16384)
    {
        QSqlQuery query;
        query.prepare("delete from users where id_user=:id");
        query.bindValue(":id", ui->tableUsers->currentId);

        if (query.exec()) on_comboGroup_activated(ui->comboGroup->currentText());
        else QMessageBox::warning(0, "Ошибка", "Не удалось удалить пользователя");
    }
}

void main_users::on_pushEdit_clicked()
{
    addUser *a = new addUser(0, gr);
    a->show();
    a->fillData(ui->tableUsers->currentId);

}
