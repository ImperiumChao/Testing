#include "testsettings.h"
#include "ui_testsettings.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>


TestSettings::TestSettings(QWidget *parent, QStringList sub, QStringList user) :
    QWidget(parent),
    ui(new Ui::TestSettings)
{
    ui->setupUi(this);

    ui->lineFive->setValidator( new QIntValidator(0, 300, this));
    ui->lineFour->setValidator( new QIntValidator(0, 300, this));
    ui->lineThree->setValidator( new QIntValidator(0, 300, this));
    ui->lineQuestions->setValidator( new QIntValidator(0, 300, this));
    ui->lineTime->setValidator( new QIntValidator(0, 300, this));
    subject=sub;
    u=user;
    if(!u.isEmpty())
    if(u[4]=="2")
    {
        QString str, str1;
        str.append(u[2] + " ");
        str1 = u[1];
        str1= str1.left(1) +".";
        str.append(str1);
        str1 = u[3];
        str1= str1.left(1) +".";
        str.append(str1);

        owners.append(QStringList() << u[0] << str);
        ui->tableOwners->fillTable(owners, QStringList() << "ID" << "ФИО");
    }

    QSqlQuery query;
    query.prepare("SELECT id_user, surname, name, secondname FROM users WHERE id_role=2");
    query.exec();

    QString str;
    ui->comboTeachers->addItem("-");
    while(query.next())
    {
        str=query.value(1).toString() + " " + query.value(2).toString().left(1) + "." + query.value(3).toString().left(1) + ".";
        teachers.append(QStringList() << query.value(0).toString() << str);
        ui->comboTeachers->addItem(str);
    }
}

TestSettings::~TestSettings()
{
    delete ui;
}

void TestSettings::fillData(QStringList test)
{
    neW=true;
    t=test;
    if (test.isEmpty())
    {
        ui->labelSubject->setText("Дисциплина:\t" + subject.at(1));
        delete ui->toolExport;
        delete ui->labelExport;
        delete ui->horizontalLayout_11;
    }
    else
    {
        setWindowTitle("Настройки теста");
        //Это когда открытие для редактирования
        neW=false;
        ui->pushCreate->setText("Сохранить");
        ui->labelSubject->setText("Дисциплина:\t" + test.at(10));
        ui->lineTest->setText(test.at(1));
        ui->lineQuestions->setText(test.at(3));
        ui->lineTime->setText(test.at(4));
        ui->lineFive->setText(test.at(7));
        ui->lineFour->setText(test.at(8));
        ui->lineThree->setText(test.at(9));
        if (test.at(5)=="true") ui->checkTraning->setChecked(true);
        else ui->checkTraning->setChecked(false);
        if (test.at(11)=="true") ui->checkBinding->setChecked(true);
        else ui->checkBinding->setChecked(false);

        QSqlQuery query;
        query.prepare("select u.id_user, u.surname, u.name, u.secondname from owners o, users u WHERE o.id_user=u.id_user AND o.id_test=:idt");
        query.bindValue(":idt", test.at(0));

        query.exec();
        while(query.next())
        {
            owners.append(QStringList() << query.value(0).toString()
                          << query.value(1).toString() + " " + query.value(2).toString().left(1) + "." +
                          query.value(3).toString().left(1) + ".");
        }
        ui->tableOwners->fillTable(owners, QStringList() << "ID" << "ФИО");
    }
}



void TestSettings::on_pushCreate_clicked()
{
    bool check=ui->lineTest->text()!="" && ui->lineFive->text()!="" && ui->lineQuestions->text()!=""
            && ui->lineTime->text()!="";
    if (!check)
    {
        QMessageBox::warning(this, "Ошибка", "Не все нужные поля заполнены");
        return;
    }
    if (!neW)
    {
        
        QSqlQuery query;
        query.prepare("DELETE FROM owners WHERE id_test=:idt");
        query.bindValue(":idt", t.at(0));
        query.exec();
        
        QStringList o;
        for (int i=0; i<owners.size(); i++)
        {
            o=owners[i];
            query.prepare("INSERT INTO owners(id_test, id_user) VALUES(:idt, :idu)");
            query.bindValue(":idt", t.at(0));
            query.bindValue(":idu", o.at(0));
            query.exec();
            
            
            
        }

        query.prepare("UPDATE tests SET name_test=:name, num_question=:num, time=:time, "
                      "training=:training, five=:five, four=:four, three=:three, binding=:binding "
                      "WHERE id_test=:idt");

        query.bindValue(":name", ui->lineTest->text());
        query.bindValue(":num", ui->lineQuestions->text());
        query.bindValue(":time", ui->lineTime->text());
        query.bindValue(":idt", t.at(0));
        query.bindValue(":five", ui->lineFive->text());

        if (ui->lineFour->text()=="" || ui->lineFour->text()=="0") query.bindValue(":four", QVariant( QVariant::Int ));
        else query.bindValue(":four", ui->lineFour->text());

        if (ui->lineThree->text()==""|| ui->lineThree->text()=="0") query.bindValue(":three", QVariant( QVariant::Int ));
        else query.bindValue(":three", ui->lineThree->text());

        if (ui->checkTraning->isChecked()) query.bindValue(":training", "1");
        else query.bindValue(":training", "0");

        if (ui->checkBinding->isChecked()) query.bindValue(":binding", "1");
        else query.bindValue(":binding", "0");

        if (!query.exec()) QMessageBox::warning(0, "Ошибка", query.lastError().text());
        else
        {
            close();
            emit editedTest();
            emit addedTest();
        }
        return;

    }
    
    n_test();
    emit addedTest();
    close();
}



void TestSettings::on_pushCancel_clicked()
{
    close();
}



void TestSettings::on_pushAddOwner_clicked()
{
    int id=ui->comboTeachers->currentIndex();
    if (id==0) return;
    else id--;
    QStringList o=teachers[id];
    QString t=o.at(0);
    bool g=true;
    for (int i=0; i<owners.size(); i++)
    {
        o=owners[i];
        if (o.at(0)==t)
        {
            g=false;
            break;


        }
    }
    if (g)
    {
        owners.append(teachers[id]);
        ui->tableOwners->fillTable(owners, QStringList() << "ID" << "ФИО");
    }




}

void TestSettings::on_pushDelOwner_clicked()
{

    owners.removeAt(ui->tableOwners->id_row);
    ui->tableOwners->fillTable(owners, QStringList() << "ID" << "ФИО");
}



void TestSettings::on_toolImport_clicked()
{
    if (neW)
    {
        QMessageBox msg(this);
        msg.setText("Сохранить настройки теста?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        int p=msg.exec();
        if (p==QMessageBox::Yes)
        {
            bool check=ui->lineTest->text()!="" && ui->lineFive->text()!="" && ui->lineQuestions->text()!=""
                    && ui->lineTime->text()!="";
            if (!check)
            {
                QMessageBox::warning(this, "Ошибка", "Не все нужные поля заполнены");
                return;
            }
            n_test();
            neW=false;
            ui->pushCreate->setText("Сохранить");
        }
        else return;


    }
    QList<QStringList> lsd;
    QStringList quest;
    QString line;
    QFile file(QFileDialog::getOpenFileName(this,"Импорт теста из файла", "/", ""));
    int num_quest=0;
    if (!file.open(QIODevice::ReadOnly)) return;

    QTextStream stream(&file);

    while (true)
    {
       line = stream.readLine(0);
       if (line.isNull() && !quest.isEmpty())
       {

           lsd.append(quest);
           file.close();
           break;
       }
       if (line.left(1)=="#")
       {
           if (!quest.isEmpty())
           {
               lsd.append(quest);
               quest.clear();
           }
           line = line.remove(0,1);
           quest.append(line);
       }
       else
       {
           quest.append(line);
       }
    }

    QSqlQuery query;
    QString id;
    int r;
    for (int i=0; i<lsd.size(); i++)
    {

        quest = lsd[i];
        query.prepare("INSERT INTO questions(text_question, date, one_answer, id_test, active) "
                      "VALUES(:txt, current_date, true, :idt, true) RETURNING id_quest");
        query.bindValue(":txt", quest[0]);
        query.bindValue(":idt", t[0]);
        query.exec();
        query.next();
        id=query.value(0).toString();
        num_quest++;
        r=0;
        for (int j=1; j<quest.size(); j++)
        {
            query.prepare("INSERT INTO answers(text_answer, binding, right_answer, number, id_quest) "
                          "VALUES(:txt, :b, :ra, :n, :idq)");
            line = quest[j];
            if (line.left(1)=="@")
            {
                r++;
                line= line.remove(0,1);
                query.bindValue(":ra", "true");
            }
            else query.bindValue(":ra", "false");
            query.bindValue(":txt", line);
            query.bindValue(":n", QString::number(j));
            query.bindValue(":idq", id);
            query.bindValue(":b", ui->checkBinding->isChecked());
            query.exec();
        }
        if (r==0)
        {
            query.prepare("UPDATE questions SET active=false WHERE id_quest=" + id);
            query.exec();
        }

    }
    file.close();

    QMessageBox::information(this, "", "Добавлено " + QString::number(num_quest) + " вопросов.");

}




void TestSettings::on_toolExport_clicked()
{
    QString str;
    QSqlQuery query, query1;
    query.prepare("SELECT id_quest, text_question FROM questions WHERE id_test=:idt");
    query.bindValue(":idt", t[0]);
    query.exec();
    while (query.next())
    {
        if (!str.isNull()) str.append("\n");
        str.append("#" + query.value(1).toString());
        query1.prepare("SELECT text_answer, right_answer FROM answers WHERE id_quest=:idq ORDER BY number");
        query1.bindValue(":idq", query.value(0));
        query1.exec();
        while(query1.next())
        {
            if (query1.value(1).toBool())
            str.append("\n@" + query1.value(0).toString());
            else
            str.append("\n" + query1.value(0).toString());
        }
    }
    QFile file(QFileDialog::getSaveFileName(this, "Экспорт теста в файл", "/","*.tst"));
    QTextStream stream(&file);
    if (!file.open(QFile::WriteOnly)) return;
    stream << str;
    file.close();
}

void TestSettings::n_test()
{
    QSqlQuery query;
    query.prepare("insert into tests(name_test, id_subject, num_question, time, training, author, date, five, four, three, binding)"
                  " values(:name, :id_sub, :num, :time, :training, :author, :date, :five, :four, :three, :binding) returning id_test");
    query.bindValue(":name", ui->lineTest->text());
    query.bindValue(":id_sub", subject.at(0));
    query.bindValue(":num", ui->lineQuestions->text());
    query.bindValue(":time", ui->lineTime->text());
    query.bindValue(":date", "today");
    query.bindValue(":five", ui->lineFive->text());

    if (ui->lineFour->text()=="") query.bindValue(":four", QVariant( QVariant::Int ));
    else query.bindValue(":four", ui->lineFour->text());

    if (ui->lineThree->text()=="") query.bindValue(":three", QVariant( QVariant::Int ));
    else query.bindValue(":three", ui->lineThree->text());

    if (ui->checkTraning->isChecked()) query.bindValue(":training", "1");
    else query.bindValue(":training", "0");

    if (ui->checkBinding->isChecked()) query.bindValue(":binding", "1");
    else query.bindValue(":binding", "0");

    QString name=u.at(1), secname=u.at(3);
    name=name.left(1) + ".";
    secname=secname.left(1) + "." ;
    query.bindValue(":author", u.at(2) + " " + name + secname);


    query.exec();

    query.next();
    QString idt=query.value(0).toString();
    t.append(idt);

    QStringList o;
    for (int i=0; i<owners.size(); i++)
    {
        o=owners[i];
        query.prepare("INSERT INTO owners(id_test, id_user) VALUES(:idt, :idu)");
        query.bindValue(":idt", idt);
        query.bindValue(":idu", o.at(0));
        query.exec();
    }
}

























