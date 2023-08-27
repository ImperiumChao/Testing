#include "main_statistics.h"
#include "ui_main_statistics.h"
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>

main_statistics::main_statistics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::main_statistics)
{
    ui->setupUi(this);
    QSqlQuery query;
    query.prepare("SELECT current_date-30, current_date");
    query.exec();
    query.next();
    QStringList lst;
    lst = query.value(0).toString().split("-");
    int y,m,d;
    QString str;
    str=lst.at(0);
    y=str.toInt(0);
    str=lst.at(1);
    m=str.toInt(0);
    str=lst.at(2);
    d=str.toInt(0);
    date1.setDate(y, m, d);
    ui->date1->setDate(date1);

    lst = query.value(1).toString().split("-");
    str=lst.at(0);
    y=str.toInt(0);
    str=lst.at(1);
    m=str.toInt(0);
    str=lst.at(2);
    d=str.toInt(0);
    date2.setDate(y, m, d);
    ui->date2->setDate(date2);
    lst.clear();
    query.prepare("SELECT id_subject, name_subject FROM subjects");
    query.exec();
    ui->comboSubject->addItem("-");

    while(query.next())
    {
        lst.clear();
        lst << query.value(0).toString() << query.value(1).toString();
        subjects.append(lst);
        ui->comboSubject->addItem(lst[1]);
    }


    query.prepare("SELECT name_group FROM groups");
    query.exec();
    ui->comboGroup->addItem("-");

    while(query.next())
    {
        str= query.value(0).toString();
        groups << str;
        ui->comboGroup->addItem(str);
    }

    query.prepare("SELECT id_user, surname, name, secondName FROM users WHERE id_role=2");
    query.exec();
    ui->comboPrepod->addItem("-");

    while(query.next())
    {
        lst.clear();
        str= query.value(1).toString() + " " + query.value(2).toString().left(1) + "." + query.value(3).toString().left(1);
        lst << query.value(0).toString() << query.value(1).toString();
        teachers.append(lst);
        ui->comboPrepod->addItem(str);
    }
    ui->comboTest->addItem("-");


    timer =new QTimer;
    timer->start(5000);
    connect(timer, SIGNAL(timeout()), SLOT(timeOut()));
    connect(ui->comboGroup, SIGNAL(activated(int)), SLOT(timeOut()));
    connect(ui->comboPrepod, SIGNAL(activated(int)), SLOT(timeOut()));
    connect(ui->comboSubject, SIGNAL(activated(QString)), SLOT(test(QString)));
    connect(ui->comboTest, SIGNAL(activated(int)), SLOT(timeOut()));
    connect(ui->date1, SIGNAL(dateChanged(QDate)), SLOT(timeOut()));
    connect(ui->date2, SIGNAL(dateChanged(QDate)), SLOT(timeOut()));
    timeOut();
}

main_statistics::~main_statistics()
{
    delete ui;
}

void main_statistics::timeOut()
{

    QSqlQuery query;
    QString str;




    QStringList lsd;

    if (ui->comboTest->currentIndex()!=0) lsd.append("r.id_test=" + tests[ui->comboTest->currentIndex()-1].at(0));
    else if (ui->comboSubject->currentIndex()!=0) lsd.append("t.id_subject=" + subjects[ui->comboSubject->currentIndex()-1].at(0));
    if (ui->comboGroup->currentIndex()!=0) lsd.append("u.name_group='" + ui->comboGroup->currentText()+"'");
    if (ui->comboPrepod->currentIndex()!=0) lsd.append("r.id_teacher=" + teachers[ui->comboPrepod->currentIndex()-1].at(0));
    lsd.append("r.date BETWEEN '" + ui->date1->text() + "' AND '" + ui->date2->text() + "'");
    str =  lsd.join(" AND ");

    query.prepare("SELECT r.date, u.name_group, u.surname, u.name, u.secondname, t.name_test, "
                  "r.num_right, t.five, t.four, t.three, teacher.surname, teacher.name, "
                  "teacher.secondname FROM results r, users u, tests t, "
                  "(SELECT id_user, surname, name, secondname FROM users) teacher"
                  " WHERE r.id_test=t.id_test AND r.id_student=u.id_user AND r.id_teacher=teacher.id_user AND " + str);
    if (!query.exec()) QMessageBox::information(0,"", query.lastError().text());
    QList<QStringList> dataTable;
    while (query.next())
    {
        lsd.clear();
        lsd.append(query.value(0).toString());
        lsd.append(query.value(1).toString());
        lsd.append(query.value(2).toString() + " " + query.value(3).toString().left(1) + "." + query.value(4).toString().left(1) + ".");
        lsd.append(query.value(5).toString());
        int nr=query.value(6).toInt(0), r5=query.value(7).toInt(0), r4=query.value(8).toInt(0), r3=query.value(9).toInt(0), rat;
        if(nr>=r5) rat=5;
        else if (nr>=r4) rat=4;
        else if (nr>=r3) rat=3;
        else if (nr<r3) rat=2;
        lsd.append(QString::number(rat));
        lsd.append(query.value(10).toString() + " " + query.value(11).toString().left(1) + "." + query.value(12).toString().left(1) + ".");
        dataTable.append(lsd);
    }
    ui->tableStat->fillTable(dataTable, QStringList() << "Дата" << "Группа" << "ФИО" << "Тест"  << "Оценка" << "Преподаватель");

}

void main_statistics::test(QString str)
{
    static QString s="-";
    if (s==str)
    {

      timeOut();
      return;
    }
    QSqlQuery query;

    ui->comboTest->clear();
    ui->comboTest->addItem("-");

    if (ui->comboSubject->currentIndex()!=0)
    {
        query.prepare("SELECT id_test, name_test FROM tests WHERE id_subject=:ids");
        query.bindValue(":ids", subjects[ui->comboSubject->currentIndex()-1].at(0));
        query.exec();

        //int c=ui->comboTest->currentIndex();

        tests.clear();



        while (query.next())
        {
            tests.append(QStringList() << query.value(0).toString() << query.value(1).toString());
            ui->comboTest->addItem(query.value(1).toString());
        }
        //ui->comboTest->setCurrentIndex(c);
    }

    s=str;
    timeOut();
}


