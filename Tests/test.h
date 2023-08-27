#ifndef TEST_H
#define TEST_H

#include <QWidget>

namespace Ui {
class TEST;
}

class TEST : public QWidget
{
    Q_OBJECT

public:
    explicit TEST(QWidget *parent, QString id_test, QStringList u);
    ~TEST();

private slots:


    void on_pushTraining_clicked();
    void updateQuest()
    {
        newQuestions=true;
    }
    QList<int> myRand(int h);
    void on_pushSettings_clicked();

    void on_pushControl_clicked();
    void updateData();
private:
    Ui::TEST *ui;
    QList<QStringList> test_q, test_a, test, test1;
    QStringList t;
    QString id_t;
    QStringList user;
    bool newQuestions;
    void genQuestList();


};

#endif // TEST_H
