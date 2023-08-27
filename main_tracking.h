#ifndef MAIN_TRACKING_H
#define MAIN_TRACKING_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class main_tracking;
}

class main_tracking : public QWidget
{
    Q_OBJECT

public:
    explicit main_tracking(QWidget *parent, QStringList u);
    ~main_tracking();

private:
    Ui::main_tracking *ui;
    QTimer *timer;
    QList<QStringList> t_user;
    QStringList user, idt;
private slots:
    void timeOut();
    void on_pushControl_clicked();
    void on_pushControlAll_clicked();
    void on_pushReset_clicked();
    void on_pushFail_clicked();
};

#endif // MAIN_TRACKING_H
