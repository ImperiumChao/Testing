#ifndef EDITTEST_H
#define EDITTEST_H

#include <QWidget>

namespace Ui {
class EditTest;
}

class EditTest : public QWidget
{
    Q_OBJECT

public:
    explicit EditTest(QWidget *parent, QList<QStringList> q, QList<QStringList> a, QString id_t);
    ~EditTest();

private:
    Ui::EditTest *ui;
    QString id_test;
    int num_answers, num_quest;
    bool fewa, q_change, a_change, new_quest;
    QList<QStringList> test_q, test_a;
    void changing();
    void qa_update();
    bool checkFill();
    void nquest();
private slots:
    void checking(bool check);
    void fewAnswers();
    void on_checkFewAnswers_clicked(bool checked);
    void fillData(int num);
    void on_pushNext_clicked();
    void clearAll();
    void questChanged();
    void answerChanged();
    void index();

    void on_pushPrev_clicked();

    void on_pushDelQuestion_clicked();
    void on_pushEnd_clicked();

    void on_pushBegin_clicked();

signals:
    void updateQuest();
};

#endif // EDITTEST_H
