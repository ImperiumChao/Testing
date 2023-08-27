#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <Widgets/table.h>
#include <QSqlDatabase>
#include <QTimer>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QStringList u;
    int r;

private:
    QTimer *timer;

    Ui::Widget *ui;
//    QWidget * track, *users, *stat;
    void login();
    QSqlDatabase db;
    void updateAll();
private slots:
    bool connectDB();
    void getUser(QStringList user, int role);
    void on_pushButton_clicked();
};

#endif // WIDGET_H
