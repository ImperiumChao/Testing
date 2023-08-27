#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>

namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = 0);
    ~LoginForm();

private:
    Ui::LoginForm *ui;
signals:
    void login(QStringList user, int role);
private slots:
    void on_pushButton_clicked();
};

#endif // LOGINFORM_H
