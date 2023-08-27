#include "main_tracking.h"
#include "ui_main_tracking.h"
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>


main_tracking::main_tracking(QWidget *parent, QStringList u) :
    QWidget(parent),
    ui(new Ui::main_tracking)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    timer->start(3000);
    user=u;
    timeOut();
    connect(timer, SIGNAL(timeout()), SLOT(timeOut()));


}

main_tracking::~main_tracking()
{
    delete ui;
}

void main_tracking::timeOut()
{
    t_user.clear();
    idt.clear();
    QSqlQuery query;
    if (user[4]!="1") query.prepare("SELECT u.id_user, u.surname, u.name, u.secondname, u.name_group, t.name_test, u.control, u.id_test "
                  "FROM users u, tests t WHERE control>0 AND u.id_test=t.id_test AND u.id_test IN(SELECT id_test FROM owners WHERE id_user=:idu) ORDER BY u.id_user");
    else query.prepare("SELECT u.id_user, u.surname, u.name, u.secondname, u.name_group, t.name_test, u.control, u.id_test "
                       "FROM users u, tests t WHERE control>0 AND u.id_test=t.id_test ORDER BY u.id_user");
    query.bindValue(":idu", user[0]);
    query.exec();
    QStringList lsd;
    QString str;
    while(query.next())
    {
        lsd.clear();
        lsd.append(query.value(0).toString());
        str=query.value(1).toString() + " " + query.value(2).toString().left(1).toUpper() + "."
                                            + query.value(3).toString().left(1).toUpper() + ".";
        lsd.append(str);
        lsd.append(query.value(4).toString());
        lsd.append(query.value(5).toString());
        str=query.value(6).toString();
        if (str=="1") str="Ожидает разрешения";
        else if (str=="2") str="Разрешение получено";
        else if (str=="3") str="Проходит тест";
        lsd.append(str);
        idt.append(query.value(7).toString());
        t_user.append(lsd);
    }
    ui->tableTrack->fillTable(t_user,QStringList() << "ID" << "ФИО" << "Группа" << "Тест" << "Статус");


}

void main_tracking::on_pushControl_clicked()
{
    int id=ui->tableTrack->currentId.toInt(0);
    if (id==-1) return;
    QSqlQuery query;
    query.prepare("UPDATE users SET control=2 WHERE id_user=:id");
    query.bindValue(":id", QString::number(id));
    query.exec();
    query.clear();

    query.prepare("INSERT INTO results (id_student, id_teacher, num_right, date, id_test) values(:ids, :idt, 0, current_date, :id)");
    query.bindValue(":ids", QString::number(id));
    query.bindValue(":idt", user.at(0));
    query.bindValue(":id", idt.at(ui->tableTrack->id_row));
    if (query.exec())
    timeOut();
}



void main_tracking::on_pushControlAll_clicked()
{
    QSqlQuery query;

    for (int i=0; i<t_user.size(); i++)
    {
        if (t_user[i].at(4)=="Ожидает разрешения")
        {

            query.prepare("UPDATE users SET control=2 WHERE id_user=:id");
            query.bindValue(":id", t_user[i].at(0));
            query.exec();
            query.prepare("INSERT INTO results (id_student, id_teacher, num_right, date, id_test) values(:ids, :idt, 0, current_date, :id)");
            query.bindValue(":ids", t_user[i].at(0));
            query.bindValue(":idt", user.at(0));
            query.bindValue(":id", idt.at(i));
            query.exec();
        }
    }
    timeOut();
}

void main_tracking::on_pushReset_clicked()
{
    int id=ui->tableTrack->currentId.toInt(0);
    QSqlQuery query;
//    query.prepare("DELETE FROM results WHERE id_student=:idu AND id_test=:idt AND id_teacher=:teacher AND date=current_date AND num_right=0");
//    query.bindValue(":idu", QString::number(id));
//    query.bindValue(":idt", idt.at(ui->tableTrack->id_row));
//    query.bindValue(":teacher", user.at(0));
//    query.exec();




    query.prepare("UPDATE users SET control=0, id_test=:idt WHERE id_user=:id");
    query.bindValue(":id", QString::number(id));
    query.bindValue(":idt", QVariant(QVariant::Int));
    query.exec();
    query.clear();
//    query.prepare("DELETE FROM results WHERE num_right=0 AND id_student=:idu "
//                  "AND id_test=:idt AND id_teacher=:teacher AND id_result= "
//                  "(SELECT MAX(id_result) FROM results  "
//                  "WHERE num_right=0 AND id_student=:idu AND id_test=:idt AND id_teacher=:teacher)");
//    query.bindValue(":idu", QString::number(id));
//    query.bindValue(":idt", idt.at(ui->tableTrack->id_row));
//    query.bindValue(":teacher", user.at(0));
//    query.exec();
    timeOut();

}

void main_tracking::on_pushFail_clicked()
{
    int id=ui->tableTrack->currentId.toInt(0);
    QSqlQuery query;

//    query.prepare("DELETE FROM results WHERE id_student=:idu AND id_test=:idt AND id_teacher=:teacher AND date=current_date AND num_right=0");
//    query.bindValue(":idu", QString::number(id));
//    query.bindValue(":idt", idt.at(ui->tableTrack->id_row));
//    query.bindValue(":teacher", user.at(0));
//    query.exec();

    query.prepare("UPDATE users SET control=0, id_test=:idt WHERE id_user=:id");
    query.bindValue(":id", QString::number(id));
    query.bindValue(":idt", QVariant(QVariant::Int));
    query.exec();
    timeOut();
}
