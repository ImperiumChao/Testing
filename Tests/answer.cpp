#include "answer.h"
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QDebug>

Answer::Answer(QWidget *parent) : QWidget(parent)
{
    ch=false;
    QHBoxLayout *lt = new QHBoxLayout;
    setLayout(lt);
    rb = new QRadioButton;
    cb = new QCheckBox;
    lb = new QLabel;
    lt->addWidget(rb);
    lt->addWidget(cb);
    lt->addWidget(lb);
    lb->setWordWrap(true);
    QSizePolicy pol;
    pol.setHorizontalPolicy(QSizePolicy::Fixed);
    rb->setSizePolicy(pol);
    cb->setSizePolicy(pol);
    show();
    connect(rb, SIGNAL(clicked(bool)), SLOT(setch(bool)));
    connect(cb, SIGNAL(clicked(bool)), SLOT(setch(bool)));

}

void Answer::fillAnswer(QStringList lsd, QList<bool> c)
{
    show();
    QPalette pal(lb->palette());
    pal.setColor(QPalette::WindowText,Qt::black);
    lb->setPalette(pal);
    if (c.isEmpty())
    {

        rb->setChecked(false);
        cb->setChecked(false);
        ch=false;
    }
    else
    {
        rb->setChecked(c[id-1]);
        cb->setChecked(c[id-1]);
        ch = c[id-1];

    }

    QString str = lsd[0];
    if (str.left(1)=="1")   //многоооо ответов
    {
        rb->hide();
        cb->show();
        fewa=true;

    }
    else
    {
        rb->show();
        cb->hide();
        fewa=false;


    }
    if (lsd.size()<=id)
    {
        right=true;
        r=false;
        hide();
        return;
    }
    QString s=lsd[id];
    if (s.left(1)== "1") r=true;
    else r=false;
    s.remove(0,1);
    lb->setText(s);

    right = (!(r^ch));

}

void Answer::check()
{
    //r - правильный ответ
    //right - соответстие чека и правильности
    right = (!(r^ch));
    if (r && !ch)
    {
        QPalette pal(lb->palette());
        pal.setColor(QPalette::WindowText,Qt::green);
        lb->setPalette(pal);




    }
    else if (!right)
    {
        QPalette pal(lb->palette());
        pal.setColor(QPalette::WindowText,Qt::red);
        lb->setPalette(pal);
    }
    emit rrr(right);
}

void Answer::setch(bool c)
{
    ch= c;
    rb->setChecked(c);
    cb->setChecked(c);
    right = !(r^ch);
    emit checked(id, fewa);
}

void Answer::mousePressEvent(QMouseEvent *event)
{

    if (event->button()==Qt::LeftButton)
    {
        if (!rb->isEnabled())
        {
            event->ignore();
            return;


        }
        if (ch)
        {
            ch=false;
            rb->setChecked(false);
            cb->setChecked(false);
        }
        else
        {
            ch=true;
            rb->setChecked(true);
            cb->setChecked(true);
            emit checked(id ,fewa);
        }
        right = !(r^ch);





//        if (rb->isChecked())
//        {
//            rb->setChecked(false);
//            ch=false;
//            right = !(r^ch);
//        }
//        else
//        {
//            rb->setChecked(true);
//            ch=true;
//            emit checked(id ,fewa);
//        }
//        if (cb->isChecked())
//        {
//            cb->setChecked(false);
//            ch = false;

//        }
//        else
//        {
//            cb->setChecked(true);
//            ch=true;

//        }
//        right = !(r^ch);

    }

}

