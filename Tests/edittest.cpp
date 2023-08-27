#include "edittest.h"
#include "ui_edittest.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QScrollBar>



EditTest::EditTest(QWidget *parent, QList<QStringList> q, QList<QStringList> a, QString id_t) :
    QWidget(parent),
    ui(new Ui::EditTest)
{
    ui->setupUi(this);
    id_test=id_t;
    test_a=a;
    if (test_a.size()==1 && test_q.isEmpty())
    {
        QStringList lst=test_a[0];
        if(lst.isEmpty()) test_a.clear();
    }



    test_q=q;
    num_quest=0;

    num_answers=2;
    ui->lineIndex->setAlignment(Qt::AlignHCenter);
    ui->lineIndex->setValidator( new QIntValidator(0, 5000, this));
    ui->lineIndex->setText("1");
    connect(ui->lineIndex, SIGNAL(returnPressed()), SLOT(index()));

    ui->groupAnswer4->hide();
    ui->groupAnswer5->hide();
    ui->groupAnswer6->hide();
    connect(ui->checkVisible3, SIGNAL(clicked(bool)), SLOT(checking(bool)));
    connect(ui->checkVisible4, SIGNAL(clicked(bool)), SLOT(checking(bool)));
    connect(ui->checkVisible5, SIGNAL(clicked(bool)), SLOT(checking(bool)));
    connect(ui->checkVisible6, SIGNAL(clicked(bool)), SLOT(checking(bool)));
    connect(ui->checkRight1, SIGNAL(clicked(bool)), SLOT(fewAnswers()));
    connect(ui->checkRight2, SIGNAL(clicked(bool)), SLOT(fewAnswers()));
    connect(ui->checkRight3, SIGNAL(clicked(bool)), SLOT(fewAnswers()));
    connect(ui->checkRight4, SIGNAL(clicked(bool)), SLOT(fewAnswers()));
    connect(ui->checkRight5, SIGNAL(clicked(bool)), SLOT(fewAnswers()));
    connect(ui->checkRight6, SIGNAL(clicked(bool)), SLOT(fewAnswers()));

//    if (test_q.isEmpty())
//    {
//        ui->pushDelQuestion->setText("Очистить");
//        new_quest=true;
//        return;
//    }
    fillData(0);
    changing();

}

EditTest::~EditTest()
{
    delete ui;
}

void EditTest::changing()
{
    connect(ui->checkActive, SIGNAL(clicked(bool)), SLOT(questChanged()));
    connect(ui->checkFewAnswers, SIGNAL(clicked(bool)), SLOT(questChanged()));
    connect(ui->textQuestion, SIGNAL(textChanged()), SLOT(questChanged()));
    connect(ui->textA1, SIGNAL(textChanged()), SLOT(answerChanged()));
    connect(ui->textA2, SIGNAL(textChanged()), SLOT(answerChanged()));
    connect(ui->textA3, SIGNAL(textChanged()), SLOT(answerChanged()));
    connect(ui->textA4, SIGNAL(textChanged()), SLOT(answerChanged()));
    connect(ui->textA5, SIGNAL(textChanged()), SLOT(answerChanged()));
    connect(ui->textA6, SIGNAL(textChanged()), SLOT(answerChanged()));
    connect(ui->checkVisible3, SIGNAL(clicked(bool)), SLOT(answerChanged()));
    connect(ui->checkVisible4, SIGNAL(clicked(bool)), SLOT(answerChanged()));
    connect(ui->checkVisible5, SIGNAL(clicked(bool)), SLOT(answerChanged()));
    connect(ui->checkVisible6, SIGNAL(clicked(bool)), SLOT(answerChanged()));
    connect(ui->checkRight1, SIGNAL(clicked(bool)), SLOT(answerChanged()));
    connect(ui->checkRight2, SIGNAL(clicked(bool)), SLOT(answerChanged()));
    connect(ui->checkRight3, SIGNAL(clicked(bool)), SLOT(answerChanged()));
    connect(ui->checkRight4, SIGNAL(clicked(bool)), SLOT(answerChanged()));
    connect(ui->checkRight5, SIGNAL(clicked(bool)), SLOT(answerChanged()));
    connect(ui->checkRight6, SIGNAL(clicked(bool)), SLOT(answerChanged()));
    connect(ui->checkBind1, SIGNAL(clicked(bool)), SLOT(answerChanged()));
    connect(ui->checkBind2, SIGNAL(clicked(bool)), SLOT(answerChanged()));
    connect(ui->checkBind3, SIGNAL(clicked(bool)), SLOT(answerChanged()));
    connect(ui->checkBind4, SIGNAL(clicked(bool)), SLOT(answerChanged()));
    connect(ui->checkBind5, SIGNAL(clicked(bool)), SLOT(answerChanged()));
    connect(ui->checkBind6, SIGNAL(clicked(bool)), SLOT(answerChanged()));
}

void EditTest::qa_update()
{
    if (q_change)
    {

        QSqlQuery query;
        query.prepare("UPDATE questions SET text_question=:text, one_answer=:oa, active=:a WHERE id_quest=:id");
        query.bindValue(":id", test_q[num_quest].at(0));
        query.bindValue(":text", ui->textQuestion->toPlainText());
        QString str1,str2;
        if (ui->checkFewAnswers->isChecked() && fewa) str1="false";
        else str1="true";
        query.bindValue(":oa", str1);

        if (ui->checkActive->isChecked()) str2="true";
        else str2="false";
        query.bindValue(":a", str2);

        query.exec();
        test_q[num_quest] = QStringList() << test_q[num_quest].at(0) << ui->textQuestion->toPlainText() << str1 << test_q[num_quest].at(3) << str2;
        emit updateQuest();
    }
    if (a_change)
    {

        QString str1, str2, id_quest=test_q[num_quest].at(0);
        QSqlQuery query;

        QStringList lst;

        query.prepare("UPDATE answers SET text_answer=:text, binding=:bd, right_answer=:right WHERE id_quest=:id AND number=:num");
        query.bindValue(":id", id_quest);
        query.bindValue(":text", ui->textA1->toPlainText());
        if (ui->checkBind1->isChecked()) str1="true";
        else str1="false";
        if (ui->checkRight1->isChecked()) str2="true";
        else str2="false";
        query.bindValue(":bd", str1);
        query.bindValue(":right", str2);
        query.bindValue(":num", "1");
        query.exec();

        lst << id_quest << "1" << ui->textA1->toPlainText() << str2 << str1;

        query.prepare("UPDATE answers SET text_answer=:text, binding=:bd, right_answer=:right WHERE id_quest=:id AND number=:num");
        query.bindValue(":id", id_quest);
        query.bindValue(":text", ui->textA2->toPlainText());
        if (ui->checkBind2->isChecked()) str1="true";
        else str1="false";
        if (ui->checkRight2->isChecked()) str2="true";
        else str2="false";
        query.bindValue(":bd", str1);
        query.bindValue(":right", str2);
        query.bindValue(":num", "2");
        query.exec();

        lst << id_quest << "2" << ui->textA2->toPlainText() << str2 << str1;

        if (num_answers>2)
        {
            query.prepare("UPDATE answers SET text_answer=:text, binding=:bd, right_answer=:right WHERE id_quest=:id AND number=:num");
            query.bindValue(":id", id_quest);
            query.bindValue(":text", ui->textA3->toPlainText());
            if (ui->checkBind3->isChecked()) str1="true";
            else str1="false";
            if (ui->checkRight3->isChecked()) str2="true";
            else str2="false";
            query.bindValue(":bd", str1);
            query.bindValue(":right", str2);
            query.bindValue(":num", "3");
            query.exec();

            lst << id_quest << "3" << ui->textA3->toPlainText() << str2 << str1;
        }
        if (num_answers>3)
        {
            query.prepare("UPDATE answers SET text_answer=:text, binding=:bd, right_answer=:right WHERE id_quest=:id AND number=:num");
            query.bindValue(":id", id_quest);
            query.bindValue(":text", ui->textA4->toPlainText());
            if (ui->checkBind4->isChecked()) str1="true";
            else str1="false";
            if (ui->checkRight4->isChecked()) str2="true";
            else str2="false";
            query.bindValue(":bd", str1);
            query.bindValue(":right", str2);
            query.bindValue(":num", "4");
            query.exec();

            lst << id_quest << "4" << ui->textA4->toPlainText() << str2 << str1;
        }
        if (num_answers>4)
        {
            query.prepare("UPDATE answers SET text_answer=:text, binding=:bd, right_answer=:right WHERE id_quest=:id AND number=:num");
            query.bindValue(":id", id_quest);
            query.bindValue(":text", ui->textA5->toPlainText());
            if (ui->checkBind5->isChecked()) str1="true";
            else str1="false";
            if (ui->checkRight5->isChecked()) str2="true";
            else str2="false";
            query.bindValue(":bd", str1);
            query.bindValue(":right", str2);
            query.bindValue(":num", "5");
            query.exec();

            lst << id_quest << "5" << ui->textA5->toPlainText() << str2 << str1;
        }
        if (num_answers>5)
        {
            query.prepare("UPDATE answers SET text_answer=:text, binding=:bd, right_answer=:right WHERE id_quest=:id AND number=:num");
            query.bindValue(":id", id_quest);
            query.bindValue(":text", ui->textA6->toPlainText());
            if (ui->checkBind6->isChecked()) str1="true";
            else str1="false";
            if (ui->checkRight6->isChecked()) str2="true";
            else str2="false";
            query.bindValue(":bd", str1);
            query.bindValue(":right", str2);
            query.bindValue(":num", "6");
            query.exec();

            lst << id_quest << "6" << ui->textA6->toPlainText() << str2 << str1;
        }
        test_a[num_quest] = lst;
        emit updateQuest();


//        query.prepare("DELETE FROM answers WHERE id_quest=:id");
//        query.bindValue(":id", id_quest);
//        query.exec();

//        QStringList lst;


//        query.prepare("INSERT INTO answers(text_answer, binding, right_answer, number, id_quest) VALUES (:text, :bd, :right, :num, :id)");
//        query.bindValue(":id", id_quest);
//        query.bindValue(":text", ui->textA1->toPlainText());
//        if (ui->checkBind1->isChecked()) str1="true";
//        else str1="false";
//        if (ui->checkRight1->isChecked()) str2="true";
//        else str2="false";
//        query.bindValue(":bd", str1);
//        query.bindValue(":right", str2);
//        query.bindValue(":num", "1");
//        query.exec();

//        query.prepare("SELECT max(id_quest) FROM questions");
//        query.exec();
//        query.next();

//        lst << query.value(0).toString() << "1" << ui->textA1->toPlainText() << str2 << str1;

//        query.prepare("INSERT INTO answers(text_answer, binding, right_answer, number, id_quest) VALUES (:text, :bd, :right, :num, :id)");
//        query.bindValue(":id", id_quest);
//        query.bindValue(":text", ui->textA2->toPlainText());
//        if (ui->checkBind2->isChecked()) str1="true";
//        else str1="false";
//        if (ui->checkRight2->isChecked()) str2="true";
//        else str2="false";
//        query.bindValue(":bd", str1);
//        query.bindValue(":right", str2);
//        query.bindValue(":num", "2");
//        if (!query.exec()) QMessageBox::warning(0,"ошибка", query.lastError().text());

//        query.prepare("SELECT max(id_quest) FROM questions");
//        query.exec();
//        query.next();

//        lst << query.value(0).toString() << "2" << ui->textA2->toPlainText() << str2 << str1;


//        if (num_answers>2)
//        {
//            query.prepare("INSERT INTO answers(text_answer, binding, right_answer, number, id_quest) VALUES (:text, :bd, :right, :num, :id)");
//            query.bindValue(":id", id_quest);
//            query.bindValue(":text", ui->textA3->toPlainText());
//            if (ui->checkBind3->isChecked()) str1="true";
//            else str1="false";
//            if (ui->checkRight3->isChecked()) str2="true";
//            else str2="false";
//            query.bindValue(":bd", str1);
//            query.bindValue(":right", str2);
//            query.bindValue(":num", "3");
//            query.exec();

//            query.prepare("SELECT max(id_quest) FROM questions");
//            query.exec();
//            query.next();

//            lst << query.value(0).toString() << "3" << ui->textA3->toPlainText() << str2 << str1;
//        }
//        if (num_answers>3)
//        {
//            query.prepare("INSERT INTO answers(text_answer, binding, right_answer, number, id_quest) VALUES (:text, :bd, :right, :num, :id)");
//            query.bindValue(":id", id_quest);
//            query.bindValue(":text", ui->textA4->toPlainText());
//            if (ui->checkBind4->isChecked()) str1="true";
//            else str1="false";
//            if (ui->checkRight4->isChecked()) str2="true";
//            else str2="false";
//            query.bindValue(":bd", str1);
//            query.bindValue(":right", str2);
//            query.bindValue(":num", "4");
//            query.exec();

//            query.prepare("SELECT max(id_quest) FROM questions");
//            query.exec();
//            query.next();

//            lst << query.value(0).toString() << "4" << ui->textA4->toPlainText() << str2 << str1;
//        }
//        if (num_answers>4)
//        {
//            query.prepare("INSERT INTO answers(text_answer, binding, right_answer, number, id_quest) VALUES (:text, :bd, :right, :num, :id)");
//            query.bindValue(":id", id_quest);
//            query.bindValue(":text", ui->textA5->toPlainText());
//            if (ui->checkBind5->isChecked()) str1="true";
//            else str1="false";
//            if (ui->checkRight5->isChecked()) str2="true";
//            else str2="false";
//            query.bindValue(":bd", str1);
//            query.bindValue(":right", str2);
//            query.bindValue(":num", "5");
//            query.exec();

//            query.prepare("SELECT max(id_quest) FROM questions");
//            query.exec();
//            query.next();

//            lst << query.value(0).toString() << "5" << ui->textA5->toPlainText() << str2 << str1;
//        }
//        if (num_answers>5)
//        {
//            query.prepare("INSERT INTO answers(text_answer, binding, right_answer, number, id_quest) VALUES (:text, :bd, :right, :num, :id)");
//            query.bindValue(":id", id_quest);
//            query.bindValue(":text", ui->textA6->toPlainText());
//            if (ui->checkBind6->isChecked()) str1="true";
//            else str1="false";
//            if (ui->checkRight6->isChecked()) str2="true";
//            else str2="false";
//            query.bindValue(":bd", str1);
//            query.bindValue(":right", str2);
//            query.bindValue(":num", "6");
//            query.exec();

//            query.prepare("SELECT max(id_quest) FROM questions");
//            query.exec();
//            query.next();

//            lst << query.value(0).toString() << "6" << ui->textA6->toPlainText() << str2 << str1;
//        }
//        test_a[num_quest] = lst;
//        emit updateQuest();

    }
}

bool EditTest::checkFill()
{

    bool ch = ui->textQuestion->toPlainText()=="";
    if (ch) return false;


    ch= ui->textA1->toPlainText()=="" || ui->textA2->toPlainText()=="";

    if (ch)
    {
        QMessageBox::warning(this, "", "Заполните хотя бы два ответа!");
        return true;


    }

    ch= (ui->textA3->isEnabled() && ui->textA3->toPlainText()=="") ||
            (ui->textA4->isEnabled() && ui->textA4->toPlainText()=="") ||
            (ui->textA5->isEnabled() && ui->textA5->toPlainText()=="") ||
            (ui->textA6->isEnabled() && ui->textA6->toPlainText()=="");


    if (ch && ui->checkActive->isChecked())
    {
        QMessageBox msg(this);
        msg.setText("Не все ответы заполнены. Сохранить вопрос неактивным?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        int p=msg.exec();
        if (p==QMessageBox::Yes )
        {
            ui->checkActive->setChecked(false);
            q_change=true;
        }
        if (p==QMessageBox::Cancel ) return true;
    }
    int m=0;
    if (ui->checkRight1->isChecked()) m++;
    if (ui->checkRight2->isChecked()) m++;
    if (ui->checkRight3->isChecked()) m++;
    if (ui->checkRight4->isChecked()) m++;
    if (ui->checkRight5->isChecked()) m++;
    if (ui->checkRight6->isChecked()) m++;



    if (m==0 && ui->checkActive->isChecked())
    {
        QMessageBox msg(this);
        msg.setText("Не выбран правильный ответ. Сохранить вопрос неактивным?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        int p=msg.exec();
        if (p==QMessageBox::Yes )
        {
            ui->checkActive->setChecked(false);
            q_change=true;
        }
        if (p==QMessageBox::Cancel ) return true;
    }
    return false;
}

void EditTest::nquest()
{
    QSqlQuery query;
    query.prepare("INSERT INTO questions(text_question, one_answer, date, id_test, active) VALUES (:text, :oa, 'today', :id, :a) RETURNING id_quest, date");
    query.bindValue(":id", id_test);
    qDebug() << id_test;
    query.bindValue(":text", ui->textQuestion->toPlainText());
    QString str1,str2;
    if (ui->checkFewAnswers->isChecked()) str1="false";
    else str1="true";
    query.bindValue(":oa", str1);
    if (ui->checkActive->isChecked()) str2="true";
    else str2="false";
    query.bindValue(":a", str2);
     if(!query.exec()) QMessageBox::warning(0, "", query.lastQuery());

    query.next();
    QString id_quest=query.value(0).toString();
    test_q.append(QStringList() << id_quest << ui->textQuestion->toPlainText() << str1 << query.value(1).toString() << str2);


    // ОТВЕТЫ

    if (!ui->checkVisible3->isChecked()) num_answers=2;
    else if (!ui->checkVisible4->isChecked()) num_answers=3;
    else if (!ui->checkVisible5->isChecked()) num_answers=4;
    else if (!ui->checkVisible6->isChecked()) num_answers=5;
    else if (ui->checkVisible6->isChecked()) num_answers=6;

    QStringList lst;


    query.prepare("INSERT INTO answers(text_answer, binding, right_answer, number, id_quest) VALUES (:text, :bd, :right, :num, :id)");
    query.bindValue(":id", id_quest);
    query.bindValue(":text", ui->textA1->toPlainText());
    if (ui->checkBind1->isChecked()) str1="true";
    else str1="false";
    if (ui->checkRight1->isChecked()) str2="true";
    else str2="false";
    query.bindValue(":bd", str1);
    query.bindValue(":right", str2);
    query.bindValue(":num", "1");
    query.exec();

    lst << id_quest << "1" << ui->textA1->toPlainText() << str2 << str1;

    query.prepare("INSERT INTO answers(text_answer, binding, right_answer, number, id_quest) VALUES (:text, :bd, :right, :num, :id)");
    query.bindValue(":id", id_quest);
    query.bindValue(":text", ui->textA2->toPlainText());
    if (ui->checkBind2->isChecked()) str1="true";
    else str1="false";
    if (ui->checkRight2->isChecked()) str2="true";
    else str2="false";
    query.bindValue(":bd", str1);
    query.bindValue(":right", str2);
    query.bindValue(":num", "2");
    query.exec();


    lst << id_quest << "2" << ui->textA2->toPlainText() << str2 << str1;

    if (num_answers>2)
    {
        query.prepare("INSERT INTO answers(text_answer, binding, right_answer, number, id_quest) VALUES (:text, :bd, :right, :num, :id)");
        query.bindValue(":id", id_quest);
        query.bindValue(":text", ui->textA3->toPlainText());
        if (ui->checkBind3->isChecked()) str1="true";
        else str1="false";
        if (ui->checkRight3->isChecked()) str2="true";
        else str2="false";
        query.bindValue(":bd", str1);
        query.bindValue(":right", str2);
        query.bindValue(":num", "3");
        query.exec();

        lst << id_quest << "3" << ui->textA3->toPlainText() << str2 << str1;
    }
    if (num_answers>3)
    {
        query.prepare("INSERT INTO answers(text_answer, binding, right_answer, number, id_quest) VALUES (:text, :bd, :right, :num, :id)");
        query.bindValue(":id", id_quest);
        query.bindValue(":text", ui->textA4->toPlainText());
        if (ui->checkBind4->isChecked()) str1="true";
        else str1="false";
        if (ui->checkRight4->isChecked()) str2="true";
        else str2="false";
        query.bindValue(":bd", str1);
        query.bindValue(":right", str2);
        query.bindValue(":num", "4");
        query.exec();

        lst << id_quest << "4" << ui->textA4->toPlainText() << str2 << str1;
    }
    if (num_answers>4)
    {
        query.prepare("INSERT INTO answers(text_answer, binding, right_answer, number, id_quest) VALUES (:text, :bd, :right, :num, :id)");
        query.bindValue(":id", id_quest);
        query.bindValue(":text", ui->textA5->toPlainText());
        if (ui->checkBind5->isChecked()) str1="true";
        else str1="false";
        if (ui->checkRight5->isChecked()) str2="true";
        else str2="false";
        query.bindValue(":bd", str1);
        query.bindValue(":right", str2);
        query.bindValue(":num", "5");
        query.exec();

        lst << id_quest << "5" << ui->textA5->toPlainText() << str2 << str1;
    }
    if (num_answers>5)
    {
        query.prepare("INSERT INTO answers(text_answer, binding, right_answer, number, id_quest) VALUES (:text, :bd, :right, :num, :id)");
        query.bindValue(":id", id_quest);
        query.bindValue(":text", ui->textA6->toPlainText());
        if (ui->checkBind6->isChecked()) str1="true";
        else str1="false";
        if (ui->checkRight6->isChecked()) str2="true";
        else str2="false";
        query.bindValue(":bd", str1);
        query.bindValue(":right", str2);
        query.bindValue(":num", "6");
        query.exec();

        lst << id_quest << "6" << ui->textA6->toPlainText() << str2 << str1;
    }
    qDebug() << "test_a.size() до = " << test_a.size();
    test_a.append(lst);
    qDebug() << "test_a.size() после = " << test_a.size();

    qDebug() << "test_a[0] в nquest = " << test_a.at(0);
    qDebug() << "lst в nquest = " << lst;
    new_quest= false;
    clearAll();


}


void EditTest::checking(bool check)
{
    if (!ui->checkVisible3->isChecked()) num_answers=2;
    else if (!ui->checkVisible4->isChecked()) num_answers=3;
    else if (!ui->checkVisible5->isChecked()) num_answers=4;
    else if (!ui->checkVisible6->isChecked()) num_answers=5;
    else if (ui->checkVisible6->isChecked()) num_answers=6;
if (check)
{

    if (num_answers>2)
    {
        ui->labelA3->setEnabled(true);
        ui->textA3->setEnabled(true);
        ui->checkBind3->setEnabled(true);
        ui->checkRight3->setEnabled(true);
        ui->groupAnswer4->show();
    }
    if (num_answers>3)
    {
        ui->labelA4->setEnabled(true);
        ui->textA4->setEnabled(true);
        ui->checkBind4->setEnabled(true);
        ui->checkRight4->setEnabled(true);
        ui->groupAnswer5->show();
    }
    if (num_answers>4)
    {
        ui->labelA5->setEnabled(true);
        ui->textA5->setEnabled(true);
        ui->checkBind5->setEnabled(true);
        ui->checkRight5->setEnabled(true);
        ui->groupAnswer6->show();
    }
    if (num_answers>5)
    {
        ui->labelA6->setEnabled(true);
        ui->textA6->setEnabled(true);
        ui->checkBind6->setEnabled(true);
        ui->checkRight6->setEnabled(true);
    }

}
else
{
    if (num_answers<6)
        {
            ui->labelA6->setEnabled(false);
            ui->textA6->setEnabled(false);
            ui->checkBind6->setEnabled(false);
            ui->checkRight6->setEnabled(false);
            ui->checkVisible6->setChecked(false);
        }
        if (num_answers<5)
        {
            ui->labelA5->setEnabled(false);
            ui->textA5->setEnabled(false);
            ui->checkBind5->setEnabled(false);
            ui->checkRight5->setEnabled(false);
            ui->checkVisible5->setChecked(false);
            ui->groupAnswer6->hide();
        }
        if (num_answers<4)
        {
            ui->labelA4->setEnabled(false);
            ui->textA4->setEnabled(false);
            ui->checkBind4->setEnabled(false);
            ui->checkRight4->setEnabled(false);
            ui->checkVisible4->setChecked(false);
            ui->groupAnswer5->hide();
        }
        if (num_answers<3)
        {
            ui->labelA3->setEnabled(false);
            ui->textA3->setEnabled(false);
            ui->checkBind3->setEnabled(false);
            ui->checkRight3->setEnabled(false);
            ui->checkVisible3->setChecked(false);
            ui->groupAnswer4->hide();
        }
}




}

void EditTest::fewAnswers()
{
    int m=0;
    if (ui->checkRight1->isChecked()) m++;
    if (ui->checkRight2->isChecked()) m++;
    if (ui->checkRight3->isChecked()) m++;
    if (ui->checkRight4->isChecked()) m++;
    if (ui->checkRight5->isChecked()) m++;
    if (ui->checkRight6->isChecked()) m++;

    if(m>1)
    {
        ui->checkFewAnswers->setChecked(true);
        ui->checkFewAnswers->setDisabled(true);
    }
    else
    {
        ui->checkFewAnswers->setChecked(fewa);
        ui->checkFewAnswers->setDisabled(false);
    }
}


void EditTest::on_checkFewAnswers_clicked(bool checked)
{
    fewa=checked;
}


void EditTest::fillData(int num)
{

    if (test_q.size()<=num  || test_q.isEmpty())
    {
        new_quest= true;
        ui->pushDelQuestion->setText("Очистить");
        QSqlQuery query;
        query.prepare("SELECT binding FROM tests WHERE id_test=:idt");
        query.bindValue(":idt", id_test);
        query.exec();
        query.next();
        bool binding = query.value(0).toBool();
        ui->checkBind1->setChecked(binding);
        ui->checkBind2->setChecked(binding);
        ui->checkBind3->setChecked(binding);
        ui->checkBind4->setChecked(binding);
        ui->checkBind5->setChecked(binding);
        ui->checkBind6->setChecked(binding);
        return;
    }
    ui->pushDelQuestion->setText("Удалить");
    clearAll();

    new_quest=false;


    QStringList lsd1= test_q[num], lsd2=test_a[num];
    ui->textQuestion->setText(lsd1.at(1));

    if (lsd1.at(2)=="false") ui->checkFewAnswers->setChecked(true);
    else ui->checkFewAnswers->setChecked(false);

    if (lsd1.at(4)=="true") ui->checkActive->setChecked(true);
    else ui->checkActive->setChecked(false);

    qDebug() << "test_a.[0] = " <<  test_a[0];
    ui->textA1->setText(lsd2.at(2));
    ui->textA2->setText(lsd2.at(7));

    if (lsd2.at(4)=="true") ui->checkBind1->setChecked(true);
    else ui->checkBind1->setChecked(false);

    if (lsd2.at(9)=="true") ui->checkBind2->setChecked(true);
    else ui->checkBind2->setChecked(false);

    if (lsd2.at(3)=="true") ui->checkRight1->setChecked(true);
    else ui->checkRight1->setChecked(false);
    if (lsd2.at(8)=="true") ui->checkRight2->setChecked(true);
    else ui->checkRight2->setChecked(false);

    int k=lsd2.size();
    if (k>10)
    {
        ui->checkVisible3->setChecked(true);
        ui->textA3->setText(lsd2.at(12));
        if (lsd2.at(14)=="true") ui->checkBind3->setChecked(true);
        else ui->checkBind3->setChecked(false);

        if (lsd2.at(13)=="true") ui->checkRight3->setChecked(true);
        else ui->checkRight3->setChecked(false);
    }
    if (k>15)
    {
        ui->checkVisible4->setChecked(true);
        ui->textA4->setText(lsd2.at(17));
        if (lsd2.at(19)=="true") ui->checkBind4->setChecked(true);
        else ui->checkBind4->setChecked(false);

        if (lsd2.at(18)=="true") ui->checkRight4->setChecked(true);
        else ui->checkRight4->setChecked(false);
    }
    if (k>20)
    {
        ui->checkVisible5->setChecked(true);
        ui->textA5->setText(lsd2.at(22));
        if (lsd2.at(24)=="true") ui->checkBind5->setChecked(true);
        else ui->checkBind5->setChecked(false);

        if (lsd2.at(23)=="true") ui->checkRight5->setChecked(true);
        else ui->checkRight5->setChecked(false);
    }
    if (k>25)
    {
        ui->checkVisible6->setChecked(true);
        ui->textA6->setText(lsd2.at(27));
        if (lsd2.at(29)=="true") ui->checkBind6->setChecked(true);
        else ui->checkBind6->setChecked(false);

        if (lsd2.at(28)=="true") ui->checkRight6->setChecked(true);
        else ui->checkRight6->setChecked(false);
    }

    checking(true);
    q_change=false;
    a_change=false;
    fewa=ui->checkFewAnswers->isChecked();

    fewAnswers();


}


void EditTest::on_pushNext_clicked()
{
    ui->textQuestion->setFocus();
    if (checkFill()) return;



    if (new_quest && ui->textQuestion->toPlainText()!="")
    {
        nquest();
        num_quest++;
        fillData(num_quest);
        ui->lineIndex->setText(QString::number(num_quest+1));
        ui->scrollArea->verticalScrollBar()->setValue(0);

        return;
    }
    if (num_quest==test_q.size()) return;
    qa_update();



    clearAll();

    num_quest++;
    ui->lineIndex->setText(QString::number(num_quest+1));
    fillData(num_quest);






ui->scrollArea->verticalScrollBar()->setValue(0);

}

void EditTest::clearAll()
{

    ui->labelA3->setEnabled(false);
    ui->labelA4->setEnabled(false);
    ui->labelA5->setEnabled(false);
    ui->labelA6->setEnabled(false);
    ui->textA3->setEnabled(false);
    ui->textA4->setEnabled(false);
    ui->textA5->setEnabled(false);
    ui->textA6->setEnabled(false);
    ui->checkBind3->setEnabled(false);
    ui->checkBind4->setEnabled(false);
    ui->checkBind5->setEnabled(false);
    ui->checkBind6->setChecked(false);
    ui->checkBind1->setChecked(false);
    ui->checkBind2->setChecked(false);
    ui->checkBind3->setChecked(false);
    ui->checkBind4->setChecked(false);
    ui->checkBind5->setChecked(false);
    ui->checkBind6->setChecked(false);
    ui->checkRight3->setEnabled(false);
    ui->checkRight4->setEnabled(false);
    ui->checkRight5->setEnabled(false);
    ui->checkRight6->setEnabled(false);
    ui->checkRight1->setChecked(false);
    ui->checkRight2->setChecked(false);
    ui->checkRight3->setChecked(false);
    ui->checkRight4->setChecked(false);
    ui->checkRight5->setChecked(false);
    ui->checkRight6->setChecked(false);
    ui->checkVisible3->setChecked(false);
    ui->checkVisible4->setChecked(false);
    ui->checkVisible5->setChecked(false);
    ui->checkVisible6->setChecked(false);
    ui->textQuestion->clear();
    ui->checkActive->setChecked(true);
    ui->textA1->clear();
    ui->textA2->clear();
    ui->textA3->clear();
    ui->textA4->clear();
    ui->textA5->clear();
    ui->textA6->clear();
    ui->groupAnswer4->hide();
    ui->groupAnswer5->hide();
    ui->groupAnswer6->hide();
    ui->checkFewAnswers->setEnabled(true);
    ui->checkFewAnswers->setChecked(false);
    a_change=false;
    q_change=false;

}

void EditTest::questChanged()
{
    q_change=true;
}

void EditTest::answerChanged()
{
    a_change=true;
}

void EditTest::index()
{
    if ((ui->lineIndex->text().toInt(0)-1)>=test_q.size())
    {
        ui->lineIndex->setText(QString::number(num_quest+1));
        return;
    }
    num_quest=ui->lineIndex->text().toInt(0)-1;

    fillData(num_quest);
    ui->scrollArea->verticalScrollBar()->setValue(0);
}



void EditTest::on_pushPrev_clicked()
{
    if (num_quest<=0) return;
    if (checkFill()) return;


    if (new_quest && ui->textQuestion->toPlainText()!="")
    {

        nquest();
        num_quest--;
        fillData(num_quest);
        ui->lineIndex->setText(QString::number(num_quest+1));
        ui->scrollArea->verticalScrollBar()->setValue(0);
        return;
    }

    qa_update();



    clearAll();
    num_quest--;
    ui->lineIndex->setText(QString::number(num_quest+1));
    fillData(num_quest);
    ui->scrollArea->verticalScrollBar()->setValue(0);

}


void EditTest::on_pushDelQuestion_clicked()
{
    if (new_quest || test_q.isEmpty())
    {
        clearAll();
        return;
    }
    QMessageBox msg(this);
    msg.setText("Вы уверены, что хотите удалить вопрос?");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    int p=msg.exec();
    if (p!=16384) return;

    QSqlQuery query;
    query.prepare("DELETE FROM answers WHERE id_quest = :id");
    query.bindValue(":id", test_q[num_quest].at(0));
    query.exec();
    query.prepare("DELETE FROM questions WHERE id_quest = :id");
    query.bindValue(":id", test_q[num_quest].at(0));
    query.exec();
    test_q.takeAt(num_quest);
    test_a.takeAt(num_quest);
    if (test_q.size()<=num_quest) num_quest--;
    if (num_quest<0)
    {
        clearAll();
        return;

    }
    fillData(num_quest);



}

void EditTest::on_pushEnd_clicked()
{
    if (num_quest==test_q.size()) return;
    if (checkFill()) return;

    qa_update();



    clearAll();

    num_quest =test_q.size()-1;
    ui->lineIndex->setText(QString::number(num_quest+1));
    fillData(num_quest);
    ui->scrollArea->verticalScrollBar()->setValue(0);
}

void EditTest::on_pushBegin_clicked()
{
    if (num_quest==0) return;
    if (checkFill()) return;

    qa_update();

    clearAll();

    num_quest =0;
    ui->lineIndex->setText(QString::number(num_quest+1));
    fillData(num_quest);
    ui->scrollArea->verticalScrollBar()->setValue(0);
}
