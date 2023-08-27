#include "test.h"
#include "ui_test.h"
#include "Tests/edittest.h"
#include <QSqlQuery>
#include <QDebug>
#include "testsettings.h"
#include <QMessageBox>
#include <QTime>
#include "training.h"
#include "Widgets/querycontrol.h"
#include "qmath.h"

TEST::TEST(QWidget *parent, QString id_test, QStringList u) :
    QWidget(parent),
    ui(new Ui::TEST)
{
    id_t=id_test;
    user=u;
    ui->setupUi(this);
    updateData();

}

TEST::~TEST()
{
    delete ui;
}





void TEST::on_pushTraining_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT training FROM tests WHERE id_test=:idt");
    query.bindValue(":idt", id_t);
    query.exec();
    query.next();
    if (!query.value(0).toBool())
    {
        QMessageBox::information(this, "", "Обучение запрещено");
        return;



    }
    genQuestList();
    if (test.isEmpty())
    {
        QMessageBox::information(this, " ", "У теста нет ни одного вопроса.");
        return;
    }
    Training *t = new Training(0, test, user);


    t->show();


}

QList<int> TEST::myRand(int h)
{
    int n=QTime::currentTime().msecsSinceStartOfDay();
    QList<int> rrr, rrr1;
    for (int i=0; i<h; i++) rrr1.append(i);
    qsrand(n);
    for (int i=h; i>0; i--) rrr.append(rrr1.takeAt(qrand()%i));

    return rrr;

}



void TEST::genQuestList()
{

    test.clear();
    test1.clear();
    QStringList lsd, lsd1, ans;
    int s= test_q.size(), a, n, r;
    if (s==0) return;

    for(int i=0; i<s; i++)
    {

        r=0;
        lsd1.clear();
        lsd.clear();
        n=2;
        ans=test_a[i];
        if (test_q[i].at(4)=="false") continue;
        lsd1.append(test_q[i].at(1));
        /*
         * 0.Правильный ответ
         * 1.Неправильный ответ
         */
        a=ans.size();

        if (a>10) n=3;
        if (a>15) n=4;
        if (a>20) n=5;
        if (a>25) n=6;

        int y[n];
        for (int j=0; j<n; j++)
        {
            if (ans.at(j*5+3)=="true")
            {
                y[j]=1;
                r++;

            }
            else y[j]=0;

        }



        lsd1 << QString::number(y[0]) + ans.at(2) << QString::number(y[1]) + ans.at(7);
        if (n>2) lsd1 << QString::number(y[2]) + ans.at(12);
        if (n>3) lsd1 << QString::number(y[3]) + ans.at(17);
        if (n>4) lsd1 << QString::number(y[4]) + ans.at(22);
        if (n>5) lsd1 << QString::number(y[5]) + ans.at(27);



        int k=0;

        for (int j=0; j<n; j++)
        {
            if (ans.at(j*5+4)=="false")
            {
                lsd.append(lsd1[j+1]);
                k++;
            }
        }

        QList<int> pp=myRand(k);

        for (int j=0; j<k; j++)
        {

            lsd.insert(pp[j]+1, lsd.takeAt(j));

        }


        for (int j=0; j<n; j++)
        {
            if (ans.at((j)*5+4)=="true")
            {
                lsd.insert(j+1, lsd1[j+1]);
            }
        }

        if (test_q[i].at(2)=="false" || r>1) lsd.insert(0 , "1" + test_q[i].at(1));
        else lsd.insert(0 ,"0" + test_q[i].at(1));


        test1.append(lsd);
    }

    QList<int> tt=myRand(test1.size());
    for(int i=0; i<test1.size(); i++) test.append(test1[tt[i]]);


}

void TEST::updateData()
{
    test_a.clear();
    test_q.clear();

    ui->tabWidget->removeTab(1);
    t.clear();
    newQuestions=false;
    QSqlQuery query;
    query.prepare("SELECT t.id_test, t.name_test, t.id_subject, t.num_question, t.time, "
                  "t.training, t.author, t.five, t.four, t.three, s.name_subject, t.binding "
                  "FROM tests AS t, subjects AS s where id_test=:id and t.id_subject=s.id_subject;");
    query.bindValue(":id", id_t);
    query.exec();
    query.next();
    for (int i=0; i<12 ;i++) t.append(query.value(i).toString());
    ui->labelTest->setText(t.at(10) + ": " + t.at(1));

    QSqlQuery query1;
    query1.prepare("SELECT q.id_quest, q.text_question, q.one_answer, q.date, q.active, a.id_answer, a.number, a.text_answer, a.right_answer, a.binding FROM questions AS q, answers AS a WHERE  q.id_test=:id AND q.id_quest=a.id_quest ORDER BY q.id_quest, number");
    query1.bindValue(":id", id_t);
    query1.exec();

    QString str1="", str2="";
    QStringList lst;
    while(query1.next())
    {
        str1= query1.value(0).toString();

        if (str2=="") test_q.append(QStringList() << str1 << query1.value(1).toString() << query1.value(2).toString() << query1.value(3).toString() << query1.value(4).toString());

        if(str1!=str2 && str2!="") //смена (3)
        {
            test_q.append(QStringList() << str1 << query1.value(1).toString() << query1.value(2).toString() << query1.value(3).toString() << query1.value(4).toString());
            test_a.append(lst);
            lst.clear();

        }

        for(int i=5; i<10; i++) lst.append(query1.value(i).toString());

        str2=str1;
    }
    test_a.append(lst);

    ui->tabWidget->setTabText(0, "Прохождение");

    query.prepare("SELECT * FROM owners WHERE id_test=:idt AND id_user=:idu");
    query.bindValue(":idt", id_t);
    query.bindValue(":idu", user[0]);
    query.exec();

    if ((user[4]!="3" && query.next()) || (user[4]=="1"))
    {

        EditTest *e = new EditTest(0,test_q, test_a, id_t);
        ui->tabWidget->addTab(e, "Редактирование");
        connect(e, SIGNAL(updateQuest()), SLOT(updateQuest()));
    }
    else ui->pushSettings->hide();



}

















void TEST::on_pushSettings_clicked()
{
    QStringList sub, user;
    TestSettings *s = new TestSettings(0, sub, user);
    s->show();
    s->fillData(t);
    connect(s, SIGNAL(editedTest()), SLOT(updateData()));









}

void TEST::on_pushControl_clicked()
{

    genQuestList();
    QString str=t.at(3);
    if (str.toInt(0)>test.size())
    {
        QMessageBox::warning(this, "Ошибка", "Не хватает вопросов для контрольного теста!");
        return;
    }

    QueryControl * q = new QueryControl(0, user, test, id_t);
    q->show();
}
