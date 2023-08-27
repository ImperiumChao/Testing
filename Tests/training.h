#ifndef TRAINING_H
#define TRAINING_H

#include <QWidget>

namespace Ui {
class Training;
}

class Training : public QWidget
{
    Q_OBJECT

public:
    explicit Training(QWidget *parent, QList<QStringList> test, QStringList user);
    ~Training();

private:
    Ui::Training *ui;
    int r, currentQuest, num_right, num_quest, num_skip;
    QList<QStringList> tst;
    QStringList u;
    bool ri,k;
private slots:
    //void quest(QStringList lsd);
    void checked(int index, bool fewa);
    void rrr(bool right);

    void on_pushNext_clicked();

    void on_pushStatistic_clicked();

signals:
    void fillQuest(QStringList lsd, QList<bool> c);
    void check();
};

#endif // TRAINING_H
