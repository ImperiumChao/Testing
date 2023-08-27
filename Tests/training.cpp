#include "training.h"
#include "ui_training.h"
#include <QDebug>
#include <QMessageBox>


Training::Training(QWidget *parent, QList<QStringList> test, QStringList user) :
    QWidget(parent),
    ui(new Ui::Training)
{
    k=false;
    num_right=0;
    num_skip=0;
    num_quest=0;

    u=user;
    currentQuest=0;
    r=0;
    ui->setupUi(this);

    ui->answer1->id = 1;
    ui->answer2->id = 2;
    ui->answer3->id = 3;
    ui->answer4->id = 4;
    ui->answer5->id = 5;
    ui->answer6->id = 6;

    connect(this, SIGNAL(fillQuest(QStringList,QList<bool>)), ui->answer1, SLOT(fillAnswer(QStringList,QList<bool>)));
    connect(this, SIGNAL(fillQuest(QStringList,QList<bool>)), ui->answer2, SLOT(fillAnswer(QStringList,QList<bool>)));
    connect(this, SIGNAL(fillQuest(QStringList,QList<bool>)), ui->answer3, SLOT(fillAnswer(QStringList,QList<bool>)));
    connect(this, SIGNAL(fillQuest(QStringList,QList<bool>)), ui->answer4, SLOT(fillAnswer(QStringList,QList<bool>)));
    connect(this, SIGNAL(fillQuest(QStringList,QList<bool>)), ui->answer5, SLOT(fillAnswer(QStringList,QList<bool>)));
    connect(this, SIGNAL(fillQuest(QStringList,QList<bool>)), ui->answer6, SLOT(fillAnswer(QStringList,QList<bool>)));
    connect(ui->answer1, SIGNAL(checked(int,bool)), SLOT(checked(int,bool)));
    connect(ui->answer2, SIGNAL(checked(int,bool)), SLOT(checked(int,bool)));
    connect(ui->answer3, SIGNAL(checked(int,bool)), SLOT(checked(int,bool)));
    connect(ui->answer4, SIGNAL(checked(int,bool)), SLOT(checked(int,bool)));
    connect(ui->answer5, SIGNAL(checked(int,bool)), SLOT(checked(int,bool)));
    connect(ui->answer6, SIGNAL(checked(int,bool)), SLOT(checked(int,bool)));
    connect(this, SIGNAL(check()), ui->answer1, SLOT(check()));
    connect(this, SIGNAL(check()), ui->answer2, SLOT(check()));
    connect(this, SIGNAL(check()), ui->answer3, SLOT(check()));
    connect(this, SIGNAL(check()), ui->answer4, SLOT(check()));
    connect(this, SIGNAL(check()), ui->answer5, SLOT(check()));
    connect(this, SIGNAL(check()), ui->answer6, SLOT(check()));
    connect(ui->answer1, SIGNAL(rrr(bool)), SLOT(rrr(bool)));
    connect(ui->answer2, SIGNAL(rrr(bool)), SLOT(rrr(bool)));
    connect(ui->answer3, SIGNAL(rrr(bool)), SLOT(rrr(bool)));
    connect(ui->answer4, SIGNAL(rrr(bool)), SLOT(rrr(bool)));
    connect(ui->answer5, SIGNAL(rrr(bool)), SLOT(rrr(bool)));
    connect(ui->answer6, SIGNAL(rrr(bool)), SLOT(rrr(bool)));

    tst=test;
    QList<bool> c;
    emit fillQuest(tst[currentQuest], c);
    QString s=tst[currentQuest].at(0);
    s.remove(0,1);
    ui->labelQuestion->setText(s);

}


Training::~Training()
{
    delete ui;
}


void Training::checked(int index, bool fewa)
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
    if (index==1) ui->answer1->rb->setChecked(true);
    else if (index==2) ui->answer2->rb->setChecked(true);
    else if (index==3) ui->answer3->rb->setChecked(true);
    else if (index==4) ui->answer4->rb->setChecked(true);
    else if (index==5) ui->answer5->rb->setChecked(true);
    else if (index==6) ui->answer6->rb->setChecked(true);
    if (index==1) ui->answer1->ch=true;
    else if (index==2) ui->answer2->ch=true;
    else if (index==3) ui->answer3->ch=true;
    else if (index==4) ui->answer4->ch=true;
    else if (index==5) ui->answer5->ch=true;
    else if (index==6) ui->answer6->ch=true;
    r=0;
}

void Training::rrr(bool right)
{
    static bool t=false;

    if (r==0) ri=true;
    ri = ri && right;
    r++;

    if((r==6 && (ri || t)))
    {
        k=false;
        if (tst.size()<=currentQuest+1) currentQuest=-1;
        QList<bool> c;
        emit fillQuest(tst[++currentQuest], c);
        QString s=tst[currentQuest].at(0);
        s.remove(0,1);
        ui->labelQuestion->setText(s);
        r=0;
        t=false;
        num_quest++;
        num_right++;
    }
    else if (r==6)
    {
        k=true;
        int i=0;
        if (ui->answer1->ch) i++;
        if (ui->answer2->ch) i++;
        if (ui->answer3->ch) i++;
        if (ui->answer4->ch) i++;
        if (ui->answer5->ch) i++;
        if (ui->answer6->ch) i++;
        if (i==0)
        {
            qDebug() << "skip";
            num_skip++;
        }
        num_right--;
        r=0;
        t=true;

    }
}



void Training::on_pushNext_clicked()
{
    emit check();
}

void Training::on_pushStatistic_clicked()
{
    QString str;
    int nr= num_right, nq=num_quest;
    float per_right, per_skip;
    if (k)
    {
        nr++;
        nq++;
    }

    if (nq==0) per_skip=0;
    else per_skip= float(num_skip)/nq*100;
    if (nq==0) per_right=0;
    else per_right= float(nr)/nq*100;



    str="Количество пройденных вопросов:\t" + QString::number(nq);
    str.append("\nКоличество правилных ответов:\t" + QString::number(nr) + " (" + QString::number(per_right) + "%)");
    str.append("\nКоличество пропущенных вопросов:\t" + QString::number(num_skip)+ " (" + QString::number(per_skip) + "%)");
    QMessageBox::information(this, "", str);
}
