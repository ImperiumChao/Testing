#include "main_tests.h"
#include "ui_main_tests.h"
#include "widget.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include "Widgets/fg.h"
#include "Tests/testsettings.h"
#include "Tests/test.h"
#include <QSqlError>

main_tests::main_tests(QWidget *parent, QStringList user) :
    QWidget(parent),
    ui(new Ui::main_tests)
{
    ui->setupUi(this);
    u=user;
    updateAll();


}

main_tests::~main_tests()
{
    delete ui;
}


void main_tests::on_comboTest_activated(int index)
{
    if (index==0)
    {
        ui->pushOpenTest->setEnabled(false);
        ui->pushDelTest->setEnabled(false);
        return;
    }

    t=index-1;
    ui->pushOpenTest->setEnabled(true);
    ui->pushDelTest->setEnabled(true);
}


void main_tests::on_comboSubject_activated(int index)
{
    if (index==0)
    {
        ui->pushDelSubject->setEnabled(false);
        ui->pushAddTest->setEnabled(false);
        ui->pushOpenTest->setEnabled(false);
        ui->pushDelTest->setEnabled(false);
        ui->comboTest->clear();
        return;
    }
    ui->pushDelSubject->setEnabled(true);
    ui->pushAddTest->setEnabled(true);
    s=index-1;
    QSqlQuery query;
    query.prepare("select id_test, name_test from tests where id_subject=:sub");
    query.bindValue(":sub", subject[index-1].at(0));
    query.exec();
    ui->comboTest->clear();
    ui->comboTest->addItem("-");
    test.clear();
    while(query.next())
    {
        ui->comboTest->addItem(query.value(1).toString());
        test.append(QStringList() << query.value(0).toString() << query.value(1).toString());


    }
}


void main_tests::on_pushAddSubject_clicked()
{
    fg * f = new fg;
    f->show();
    f->setWindowTitle("Добавить дисциплину");
    connect(f, SIGNAL(returnName(QString)), SLOT(addSubject(QString)));
}


void main_tests::addSubject(QString name)
{
    QSqlQuery query;
    query.prepare("insert into subjects(name_subject) values(:sub)");
    query.bindValue(":sub", name);
    if (!query.exec()) QMessageBox::warning(this, "Ошибка", "Не удалось создать дисциплину");
    else updateAll();
}


void main_tests::updateAll()
{
    if (u[4]=="3")
    {
        ui->pushAddSubject->hide();
        ui->pushAddTest->hide();
        ui->pushDelSubject->hide();
        ui->pushDelTest->hide();
    }
    if (u[4]=="2")
    {
        ui->pushAddSubject->hide();
        ui->pushDelSubject->hide();



    }
    ui->pushOpenTest->setEnabled(false);
    QSqlQuery query("select id_subject, name_subject from subjects");
    ui->comboSubject->clear();
    ui->comboTest->clear();
    ui->comboSubject->addItem("-");
    subject.clear();
    while(query.next())
    {
        ui->comboSubject->addItem(query.value(1).toString());
        subject.append(QStringList() << query.value(0).toString() << query.value(1).toString());
    }
}


void main_tests::on_pushDelSubject_clicked()
{
    int r = ui->comboSubject->currentIndex();
    if (r==0)
    {
        QMessageBox::information(this, "Ошибка", "Не выбрана дисциплина");
        return;

    }
    QMessageBox msg(this);
    msg.setText("Вы уверены, что хотите удалить дисциплину со всеми её тестами?");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    int p=msg.exec();
    if (p==16384)
    {
        QSqlQuery query;
        query.prepare("DELETE FROM subjects WHERE id_subject = :id_sub;");
        query.bindValue(":id_sub", subject[ui->comboSubject->currentIndex()-1].at(0));
        if (!query.exec()) QMessageBox::warning(this, "Ошибка", "Не удалось удалить дисциплину");
        else updateAll();



    }
}


void main_tests::on_pushAddTest_clicked()
{
    TestSettings *t = new TestSettings(0, subject[s], u);
    t->show();
    QStringList lsd;
    t->fillData(lsd);
    connect(t, SIGNAL(addedTest()),SLOT(updateAll()));
}

void main_tests::on_pushDelTest_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT id_user, id_test FROM owners WHERE id_test=:idt AND id_user=:idu");
    query.bindValue(":idt",test[ui->comboTest->currentIndex()-1].at(0));
    query.bindValue(":idu", u[0]);
    query.exec();
    if (!query.next())
    {
        QMessageBox::warning(this, " ", "Вы не являеетесь владельцем теста");
        return;
    }







    QMessageBox msg;
    msg.setText("Вы уверены, что хотите удалить тест?");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    if (msg.exec()==16384)
    {

        query.prepare("DELETE FROM tests WHERE id_test = :id_test;");
        query.bindValue(":id_test", test[ui->comboTest->currentIndex()-1].at(0));
        if (!query.exec()) QMessageBox::warning(this, "Ошибка", "Не удалось удалить тест");
        else updateAll();
    }

}

void main_tests::on_pushOpenTest_clicked()
{
    TEST *e = new TEST(0, test[t].at(0), u);
    e->show();
}
