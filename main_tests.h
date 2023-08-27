#ifndef MAIN_TESTS_H
#define MAIN_TESTS_H

#include <QWidget>
#include <QComboBox>

namespace Ui {
class main_tests;
}

class main_tests : public QWidget
{
    Q_OBJECT

public:
    explicit main_tests(QWidget *parent, QStringList user);
    ~main_tests();
    QList<QStringList> subject, test;
    int s, t;
    QStringList u;

private slots:
    void on_comboTest_activated(int index);

    void on_comboSubject_activated(int index);

    void on_pushAddSubject_clicked();
    void addSubject(QString name);

    void on_pushDelSubject_clicked();

    void on_pushAddTest_clicked();
    void updateAll();
    void on_pushDelTest_clicked();

    void on_pushOpenTest_clicked();

private:
    Ui::main_tests *ui;

};

#endif // MAIN_TESTS_H
