#ifndef ANSWER_H
#define ANSWER_H

#include <QWidget>
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>

class Answer : public QWidget
{
    Q_OBJECT
public:
    explicit Answer(QWidget *parent = 0);
    int id;
    QRadioButton *rb;
    QCheckBox *cb;
    bool ch;
signals:
    void checked(int index, bool fewa);
    void rrr(bool right);
public slots:
    void fillAnswer(QStringList lsd, QList<bool> c);
    void check();
private:


    QLabel *lb;

    bool fewa;
    bool right,r;
private slots:
    void setch(bool c);
    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // ANSWER_H
