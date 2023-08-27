#include "querycontrol.h"
#include "ui_querycontrol.h"
#include <QSqlQuery>
#include <QDebug>
#include "Tests/controltest.h"
#include <QMessageBox>

QueryControl::QueryControl(QWidget *parent, QStringList u, QList<QStringList> t, QString id_t) :
    QWidget(parent),
    ui(new Ui::QueryControl)
{

    res=false;
    id_test=id_t;
    test=t;
    ui->setupUi(this);
    user=u;



    QSqlQuery query;

//    query.prepare("SELECT num_question FROM tests WHERE id_test=:idt");
//    query.bindValue(":idt", id_t);
//    query.exec();
//    query.next();
//    if (query.value(0).toInt(0)>t.size())
//    {
//        QMessageBox::warning(this, "Ошибка", "Не хватает вопросов для контрольного теста!");
//        this->destroy();
//        return;
//    }



    query.prepare("UPDATE users SET control = 1, id_test=:id_t WHERE id_user = :id_u;");
    query.bindValue(":id_u", u[0]);
    query.bindValue(":id_t", id_t);
    query.exec();
    timer = new QTimer(this);
    timer->start(3000);
    connect(timer, SIGNAL(timeout()), SLOT(checkControl()));
    setWindowFlags( Qt::Dialog);
}

QueryControl::~QueryControl()
{
    delete ui;
}

void QueryControl::checkControl()
{
    QSqlQuery query;
    query.prepare("SELECT control FROM users WHERE id_user= :id_u");
    query.bindValue(":id_u", user[0]);

    query.exec();
    if (!query.next()) return;
    if (query.value(0).toInt(0)==2)
    {
        ui->label->setText("Преподаватель дал разрешение на прохождение теста");
        ui->pushButton->setText("Начать тест");
        timer->stop();
        res = true;





    }

}

void QueryControl::on_pushButton_clicked()
{
    if (res)
    {
        ControlTest *ct = new ControlTest(0, test, id_test, user);
        ct->show();
        close();


    }
    else
    {
        QSqlQuery query;
        query.prepare("UPDATE users SET control = 0, id_test=:id_t WHERE id_user = :id_u;");
        query.bindValue(":id_u", user[0]);
        query.bindValue(":id_t", id_test);
        query.exec();
        close();
    }
}

void QueryControl::closeEvent(QCloseEvent *)
{
    if (res) return;
    QSqlQuery query;
    query.prepare("UPDATE users SET control = 0, id_test=:id_t WHERE id_user = :id_u;");
    query.bindValue(":id_u", user[0]);
    query.bindValue(":id_t", QVariant( QVariant::Int ));
    query.exec();
}


