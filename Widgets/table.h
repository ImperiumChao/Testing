#ifndef TABLE_H
#define TABLE_H

#include <QTableWidget>
#include <QWidget>

class table : public QTableWidget
{
    Q_OBJECT
public:
    explicit table(QWidget *parent =0);
    QString currentId;
    int id_row;
signals:

public slots:
    void fillTable(QList<QStringList> lst, QStringList head);
    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);
private slots:
    void curId(int id);
};

#endif // TABLE_H
