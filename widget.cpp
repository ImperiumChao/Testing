#include "widget.h"
#include "ui_widget.h"
#include "loginform.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include "main_statistics.h"
#include "main_tests.h"
#include "main_tracking.h"
#include "main_users.h"
//#include <main_users.h>
#include <QDialog>
#include <QLabel>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QPSQL");
    //db.setHostName("192.168.1.215");
    db.setHostName("77.93.110.35");
    db.setDatabaseName("postgres");
    db.setUserName("postgres");
    db.setPassword("f22c73d");
    timer = new QTimer(this);
    timer->start(5000);
    connect(timer, SIGNAL(timeout()), SLOT(connectDB()));
    if (connectDB()) login();
    else
    {
        QMessageBox::information(this, "", "Нет доступа к серверу!");
        close();
    }


}

Widget::~Widget()
{
    delete ui;
}

bool Widget::connectDB()
{
    if(!db.commit()) db.open();
    return db.commit();




//    if (!db.open())
//    {
//        db.setHostName("77.93.110.35");
//        if (!db.open())
//        {
//            QMessageBox::information(this, "", "Нет доступа к серверу!");
//            close();
//            return;

//        }
//    }

}

void Widget::login()
{
    LoginForm * l = new LoginForm;
    l->show();
    connect(l, SIGNAL(login(QStringList,int)), SLOT(getUser(QStringList,int)));
}

void Widget::updateAll()
{
    ui->tabWidget->clear();
    main_tests *tests = new main_tests(0, u);
    ui->tabWidget->addTab(tests, "ТЕСТЫ");

    if (r==1)
    {
        main_tracking *track = new main_tracking(0, u);
        ui->tabWidget->addTab(track, "Слежение");

        main_users *users = new main_users;
        ui->tabWidget->addTab(users, "Пользователи и группы");

        main_statistics *stat = new main_statistics;
        ui->tabWidget->addTab(stat, "Статистика");
    }
    if (r==2)
    {
        main_tracking *track = new main_tracking(0, u);
        ui->tabWidget->addTab(track, "Слежение");

        main_statistics *stat = new main_statistics;
        ui->tabWidget->addTab(stat, "Статистика");
    }
    if (r!=1)
    ui->label->setText("Вход выполнен: " + u[2] + " " + u[1]);
    else ui->label->setText("Вход выполнен: admin");



}

void Widget::getUser(QStringList user, int role)
{
    u=user;
    if (r!=role)
    {

        r=role;
        updateAll();
    }

    show();

}

void Widget::on_pushButton_clicked()
{
    login();
    hide();
}
