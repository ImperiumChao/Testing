#ifndef CONTROLTEST_H
#define CONTROLTEST_H

#include <QWidget>
#include <QTimer>
#include <QTime>

namespace Ui {
class ControlTest;
}

class ControlTest : public QWidget
{
    Q_OBJECT

public:
    explicit ControlTest(QWidget *parent, QList<QStringList> t, QString id_t, QStringList u);
    ~ControlTest();

private:
    Ui::ControlTest *ui;
    QList<QStringList> test;
    QString id_test;
    QStringList user, testSet;
    QList<QList<bool> > checkedItems;
    QList<bool> ok;
    QTimer *timer;
    QTime time;
    int currentItem;
    bool end;
    void endTest();
signals:
    void fillQuest(QStringList lsd, QList<bool> c);
    void check();
private slots:
    void on_pushNext_clicked();
    void on_pushPrev_clicked();
    void on_pushEnd_clicked();
    void timeOut();
    void index();
    void checked(int id, bool fewa);

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // CONTROLTEST_H
