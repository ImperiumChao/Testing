#ifndef MAIN_USERS_H
#define MAIN_USERS_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class main_users;
}

class main_users : public QWidget
{
    Q_OBJECT

public:
    explicit main_users(QWidget *parent = 0);
    QLabel *lbl;
    ~main_users();

private slots:


    void on_comboGroup_activated(const QString &arg1);

    void updateData(QString group = 0);

    void on_pushAddGroup_clicked();
    void addedGroup(QString group);

    void on_pushDelGroup_clicked();

    void on_pushAddUser_clicked();


    void on_pushDelUser_clicked();

    void on_pushEdit_clicked();

private:
    Ui::main_users *ui;
    QStringList groups;
    QString gr;
};

#endif // MAIN_USERS_H
