#ifndef QUERYCONTROL_H
#define QUERYCONTROL_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class QueryControl;
}

class QueryControl : public QWidget
{
    Q_OBJECT

public:
    explicit QueryControl(QWidget *parent, QStringList u, QList<QStringList> t, QString id_t);
    ~QueryControl();

private:
    Ui::QueryControl *ui;
    QStringList user;
    QList<QStringList> test;
    QString id_test;
    QTimer *timer;
    bool res;
private slots:
    void checkControl();
    void on_pushButton_clicked();

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *);
};

#endif // QUERYCONTROL_H
