#ifndef MAIN_STATISTICS_H
#define MAIN_STATISTICS_H

#include <QWidget>
#include"Widgets/table.h"
#include <QTimer>
#include <QDate>

namespace Ui {
class main_statistics;
}

class main_statistics : public QWidget
{
    Q_OBJECT

public:
    explicit main_statistics(QWidget *parent = 0 );
    ~main_statistics();

private:
    Ui::main_statistics *ui;
    QTimer *timer;
    QDate date1, date2;
    QList<QStringList> subjects;
    QStringList groups;
    QList<QStringList> teachers;
    QList<QStringList> tests;
private slots:
    void timeOut();
    void test(QString str);


};

#endif // MAIN_STATISTICS_H
