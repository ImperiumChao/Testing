#ifndef ADDUSER_H
#define ADDUSER_H

#include <QWidget>

namespace Ui {
class addUser;
}

class addUser : public QWidget
{
    Q_OBJECT

public:
    explicit addUser(QWidget *parent, QString group);
    void fillData(QString id);
    ~addUser();

private slots:


    void on_pushCancel_clicked();

    void on_pushCreate_clicked();
signals:
    void addedUser(QString group);
private:
    QString u;
    Ui::addUser *ui;
};

#endif // ADDUSER_H
