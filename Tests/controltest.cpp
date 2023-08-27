#include "controltest.h"
#include "ui_controltest.h"
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QTime>
#include <QCloseEvent>

ControlTest::ControlTest(QWidget *parent, QList<QStringList> t, QString id_t, QStringList u) :
    QWidget(parent),
    ui(new Ui::ControlTest)
{
    ui->setupUi(this);
    end=false;
    ui->pushPrev->setDisabled(true);
    currentItem=0;
    ui->answer1->id =1;
    ui->answer2->id =2;
    ui->answer3->id =3;
    ui->answer4->id =4;
    ui->answer5->id =5;
    ui->answer6->id =6;

    test=t;
    id_test=id_t;
    user=u;

    QSqlQuery query;
    query.prepare("SELECT num_question, time, five, four, three, name_test from tests WHERE id_test=:id_t");
    query.bindValue(":id_t", id_test);
    query.exec();
    query.next();
    for (int i=0; i<6; i++)
    {

        if (query.isNull(i))
        {
            testSet.append("");
        }
        else testSet.append(query.value(i).toString());
    }
    QString s=testSet.at(0);

    query.prepare("UPDATE users SET control=3 WHERE id_user = :id");
    query.bindValue(":id", u[0]);
    if (!query.exec()) QMessageBox::information(0, "", query.lastError().text());

    setWindowTitle("Итоговый тест: "+ testSet.at(5));

    test = test.mid(0,s.toInt(0));
    connect(this, SIGNAL(fillQuest(QStringList,QList<bool>)), ui->answer1, SLOT(fillAnswer(QStringList,QList<bool>)));
    connect(this, SIGNAL(fillQuest(QStringList,QList<bool>)), ui->answer2, SLOT(fillAnswer(QStringList,QList<bool>)));
    connect(this, SIGNAL(fillQuest(QStringList,QList<bool>)), ui->answer3, SLOT(fillAnswer(QStringList,QList<bool>)));
    connect(this, SIGNAL(fillQuest(QStringList,QList<bool>)), ui->answer4, SLOT(fillAnswer(QStringList,QList<bool>)));
    connect(this, SIGNAL(fillQuest(QStringList,QList<bool>)), ui->answer5, SLOT(fillAnswer(QStringList,QList<bool>)));
    connect(this, SIGNAL(fillQuest(QStringList,QList<bool>)), ui->answer6, SLOT(fillAnswer(QStringList,QList<bool>)));
    connect(this, SIGNAL(check()), ui->answer1, SLOT(check()));
    connect(this, SIGNAL(check()), ui->answer2, SLOT(check()));
    connect(this, SIGNAL(check()), ui->answer3, SLOT(check()));
    connect(this, SIGNAL(check()), ui->answer4, SLOT(check()));
    connect(this, SIGNAL(check()), ui->answer5, SLOT(check()));
    connect(this, SIGNAL(check()), ui->answer6, SLOT(check()));
    connect(ui->answer1, SIGNAL(checked(int,bool)), this, SLOT(checked(int,bool)));
    connect(ui->answer2, SIGNAL(checked(int,bool)), this, SLOT(checked(int,bool)));
    connect(ui->answer3, SIGNAL(checked(int,bool)), this, SLOT(checked(int,bool)));
    connect(ui->answer4, SIGNAL(checked(int,bool)), this, SLOT(checked(int,bool)));
    connect(ui->answer5, SIGNAL(checked(int,bool)), this, SLOT(checked(int,bool)));
    connect(ui->answer6, SIGNAL(checked(int,bool)), this, SLOT(checked(int,bool)));


    for (int i=0; i<s.toInt(0); i++)
    {
        QList<bool> lst;
        ok.append(false);
        for (int j=0; j<6; j++)
        {
            lst.append(false);

        }
        checkedItems.append(lst);
    }
    QString str=test[currentItem].at(0);
    str=str.remove(0,1);
    emit fillQuest(test[currentItem], checkedItems[currentItem]);
    ui->labelQuest->setText(str);
    ui->lineNum->setText(QString::number(currentItem+1));
    ui->lineNum->setAlignment(Qt::AlignHCenter);
    ui->lineNum->setValidator( new QIntValidator(1, test.size(), this));
    connect(ui->lineNum, SIGNAL(returnPressed()), SLOT(index()));


    timer = new QTimer(this);
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), SLOT(timeOut()));



    time = QTime::fromString(testSet[1], "m");
    ui->labelTime->setText(time.toString("mm:ss"));




}

ControlTest::~ControlTest()
{
    delete ui;
}

void ControlTest::endTest()
{
    QStringList s;
    QString l;
    bool r1, r2=true;
    int num_right=0;

    for (int i=0; i<test.size(); i++)   //QList<QStringList> test;
    {
        s=test[i];
        r2=true;
        for (int j=1; j<s.size(); j++)  //QStringList s;
        {
            l=s.at(j);
            if (l.left(1)=="1") r1= true;
            else r1= false;
            if (r1 == checkedItems[i].at(j-1)) r1=true;
            else r1=false;
            r2=(r1 && r2);

        }
        if (r2) num_right++;
    }
    timer->stop();
    end=true;
    int five=0, four=0,three=0;
    QString str;
    str = testSet[2];
    five=str.toInt(0);
    str = testSet[3];
    if (str!="") four=str.toInt(0);
    str = testSet[4];
    if (str!="") three=str.toInt(0);

    QString rating;

    if (three==0)
        if (num_right>=five) rating="Зачет";
        else rating="Незачет";
    else
    if (num_right>=five) rating="5";
    else if (num_right>=four) rating="4";
    else if (num_right>=three) rating="3";
    else rating="2";

    str="Количество вопросов: " + QString::number(test.size())
            + "\nКоличество правильных ответов: " + QString::number(num_right)
            + "\nОценка: " + rating;

    QMessageBox::information(0,"",str);

    QSqlQuery query;
    query.prepare("UPDATE results SET num_right=:nr WHERE num_right=0 AND id_student=:idu "
                  "AND id_test=:idt AND id_result= "
                  "(SELECT MAX(id_result) FROM results  "
                  "WHERE num_right=0 AND id_student=:idu AND id_test=:idt)");
    query.bindValue(":idt", id_test);
    query.bindValue(":idu", user.at(0));
    query.bindValue(":nr", QString::number(num_right));
    query.exec();

    ui->pushEnd->setEnabled(false);
    ui->answer1->cb->setEnabled(false);
    ui->answer1->rb->setEnabled(false);
    ui->answer2->cb->setEnabled(false);
    ui->answer2->rb->setEnabled(false);
    ui->answer3->cb->setEnabled(false);
    ui->answer3->rb->setEnabled(false);
    ui->answer4->cb->setEnabled(false);
    ui->answer4->rb->setEnabled(false);
    ui->answer5->cb->setEnabled(false);
    ui->answer5->rb->setEnabled(false);
    ui->answer6->cb->setEnabled(false);
    ui->answer6->rb->setEnabled(false);

    query.prepare("UPDATE users SET control=0, id_test=:idt WHERE id_user=:id");
    query.bindValue(":id", user.at(0));
    query.bindValue(":idt", QVariant(QVariant::Int));
    query.exec();
    emit check();

}

void ControlTest::on_pushNext_clicked()
{

    ui->pushPrev->setEnabled(true);

    QList<bool> lst;
    lst.append(ui->answer1->ch);
    lst.append(ui->answer2->ch);
    lst.append(ui->answer3->ch);
    lst.append(ui->answer4->ch);
    lst.append(ui->answer5->ch);
    lst.append(ui->answer6->ch);
    checkedItems[currentItem] = lst;
    currentItem++;
    emit fillQuest(test[currentItem], checkedItems[currentItem]);
    ui->lineNum->setText(QString::number(currentItem+1));
    ok[currentItem]=(ui->answer1->rb->isChecked() || ui->answer2->rb->isChecked() || ui->answer3->rb->isChecked() ||
                    ui->answer4->rb->isChecked() || ui->answer5->rb->isChecked() || ui->answer6->rb->isChecked());
    QString str=test[currentItem].at(0);
    str=str.remove(0,1);
    ui->labelQuest->setText(str);



    if (end) emit check();
    if (checkedItems.size()==(currentItem+1))
    {

        ui->pushNext->setDisabled(true);
        return;
    }
    else ui->pushNext->setDisabled(false);

}

void ControlTest::on_pushPrev_clicked()
{

    ui->pushNext->setEnabled(true);

    QList<bool> lst;
    lst.append(ui->answer1->ch);
    lst.append(ui->answer2->ch);
    lst.append(ui->answer3->ch);
    lst.append(ui->answer4->ch);
    lst.append(ui->answer5->ch);
    lst.append(ui->answer6->ch);
    checkedItems[currentItem] = lst;
    currentItem--;
    emit fillQuest(test[currentItem], checkedItems[currentItem]);
    ui->lineNum->setText(QString::number(currentItem+1));


    QString str=test[currentItem].at(0);
    str=str.remove(0,1);
    ui->labelQuest->setText(str);



    if (end) emit check();
    if (currentItem==0)
    {
        ui->pushPrev->setDisabled(true);
        return;
    }
    else ui->pushPrev->setEnabled(true);
}


void ControlTest::on_pushEnd_clicked()
{

    QList<bool> ls;
    ls.append(ui->answer1->ch);
    ls.append(ui->answer2->ch);
    ls.append(ui->answer3->ch);
    ls.append(ui->answer4->ch);
    ls.append(ui->answer5->ch);
    ls.append(ui->answer6->ch);
    checkedItems[currentItem] = ls;

    bool f;
    QList<bool> lst;
    QStringList lsd;
    for (int i=0; i<checkedItems.size(); i++)
    {
        lst=checkedItems[i];
        f=false;
        for (int j=0; j<lst.size(); j++)
        {
            f= (lst[j] || f);
            if (f) break;

        }
        if (!f) lsd.append(QString::number(i+1));
    }
    if (!lsd.isEmpty())
    {
        QMessageBox::warning(0,"", "Вы не ответили на следующие вопросы: " + lsd.join(", "));
        return;
    }
    else
    {
        QMessageBox msg(this);
        msg.setText("Вы уверены, что хотите закончить тест?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        int p=msg.exec();
        if (p==16384)
        {
            endTest();

        }



    }

}

void ControlTest::timeOut()
{
    static int t=0;
    if(t==0)
    {

        QSqlQuery query;
        query.prepare("SELECT * FROM users WHERE id_user=:idu AND control=3");
        query.bindValue(":idu", user.at(0));
        query.exec();


        if (!query.next())      //Заканчиваем тест, либо сбросили, либо поставили незачет
        {
            end=true;
            QSqlQuery query1;
            query1.prepare("SELECT * FROM results  "
                          "WHERE num_right=0 AND id_student=:idu AND id_test=:idt AND date=current_date");
            query1.bindValue(":idt", id_test);
            query1.bindValue(":idu", user.at(0));
            query1.exec();
            if(query1.next()) QMessageBox::information(this, "Незачет", "Преподаватель поставил вам незачет!");
            else QMessageBox::information(this, "", "Преподаватель сбросил ваш сеанс!");
            ui->pushEnd->setEnabled(false);
            ui->answer1->cb->setEnabled(false);
            ui->answer1->rb->setEnabled(false);
            ui->answer2->cb->setEnabled(false);
            ui->answer2->rb->setEnabled(false);
            ui->answer3->cb->setEnabled(false);
            ui->answer3->rb->setEnabled(false);
            ui->answer4->cb->setEnabled(false);
            ui->answer4->rb->setEnabled(false);
            ui->answer5->cb->setEnabled(false);
            ui->answer5->rb->setEnabled(false);
            ui->answer6->cb->setEnabled(false);
            ui->answer6->rb->setEnabled(false);
            timer->stop();

}

}
    t++;
    if (t==6) t=0;

    time = time.addSecs(-1);
    ui->labelTime->setText(time.toString("mm:ss"));
    if (time==QTime::fromString("0","s"))
    {
        QMessageBox::information(this, "", "Время вышло!");
        QList<bool> ls;
        ls.append(ui->answer1->ch);
        ls.append(ui->answer2->ch);
        ls.append(ui->answer3->ch);
        ls.append(ui->answer4->ch);
        ls.append(ui->answer5->ch);
        ls.append(ui->answer6->ch);
        checkedItems[currentItem] = ls;
        endTest();
    }



}

void ControlTest::index()
{
    if (ui->lineNum->text().toInt(0)>1) ui->pushPrev->setEnabled(true);
    else ui->pushPrev->setEnabled(false);
    if (ui->lineNum->text().toInt(0)<test.size()) ui->pushNext->setEnabled(true);
    else ui->pushNext->setEnabled(false);

    int y=ui->lineNum->text().toInt(0)-1;
    if (test.size()<y)
    {
        ui->lineNum->setText(QString::number(test.size()-1));

        return;
    }
    currentItem = y;
    emit fillQuest(test[currentItem], checkedItems[currentItem]);
    QString str=test[currentItem].at(0);
    str=str.remove(0,1);
    ui->labelQuest->setText(str);
    ui->lineNum->setText(QString::number(currentItem+1));
}

void ControlTest::checked(int id, bool fewa)
{
    if (fewa) return;
    ui->answer1->ch=false;
    ui->answer2->ch=false;
    ui->answer3->ch=false;
    ui->answer4->ch=false;
    ui->answer5->ch=false;
    ui->answer6->ch=false;
    ui->answer1->rb->setChecked(false);
    ui->answer2->rb->setChecked(false);
    ui->answer3->rb->setChecked(false);
    ui->answer4->rb->setChecked(false);
    ui->answer5->rb->setChecked(false);
    ui->answer6->rb->setChecked(false);
    if (id==1) ui->answer1->rb->setChecked(true);
    else if (id==2) ui->answer2->rb->setChecked(true);
    else if (id==3) ui->answer3->rb->setChecked(true);
    else if (id==4) ui->answer4->rb->setChecked(true);
    else if (id==5) ui->answer5->rb->setChecked(true);
    else if (id==6) ui->answer6->rb->setChecked(true);
    if (id==1) ui->answer1->ch=true;
    else if (id==2) ui->answer2->ch=true;
    else if (id==3) ui->answer3->ch=true;
    else if (id==4) ui->answer4->ch=true;
    else if (id==5) ui->answer5->ch=true;
    else if (id==6) ui->answer6->ch=true;
    //r=0;
}


void ControlTest::closeEvent(QCloseEvent *event)
{

    QMessageBox msg(this);
    msg.setText("Вы уверены, что хотите закончить тест?");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    int p=msg.exec();
    if (p==QMessageBox::Yes)
    {
        QList<bool> ls;
        ls.append(ui->answer1->ch);
        ls.append(ui->answer2->ch);
        ls.append(ui->answer3->ch);
        ls.append(ui->answer4->ch);
        ls.append(ui->answer5->ch);
        ls.append(ui->answer6->ch);
        checkedItems[currentItem] = ls;
        endTest();
    }
    if (p==QMessageBox::Cancel)
    {
        event->ignore();
    }

}
