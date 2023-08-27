#ifndef FG_H
#define FG_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class fg;
}

class fg : public QWidget
{
    Q_OBJECT

public:
    explicit fg(QWidget *parent = 0);
    QLabel *lb;
    ~fg();
signals:
    void returnName(QString name);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::fg *ui;
};

#endif // FG_H
