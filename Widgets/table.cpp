#include "table.h"
#include <QResizeEvent>
#include <QDebug>
#include <QPushButton>

table::table(QWidget *parent) : QTableWidget(parent)
{
    connect(this, SIGNAL(cellClicked(int,int)), SLOT(curId(int)));
    currentId ="-1";
    id_row=-1;


    setEditTriggers(QAbstractItemView::NoEditTriggers);

}

void table::fillTable(QList<QStringList> lst, QStringList head)
{
    clear();
    setColumnCount(head.size());
    setHorizontalHeaderLabels(head);
    if (lst.isEmpty())
    {
        setRowCount(0);
        currentId="-1";
        return;
    }
    int m=lst.size();
    int n=lst[0].size();
    setColumnCount(n);
    setRowCount(m);
    QTableWidgetItem *itm;

    for(int i=0; i<m; i++)
        for(int j=0; j<n; j++)
        {
            itm = new QTableWidgetItem;
            itm->setText(lst[i].at(j));
            setItem(i,j,itm);
        }

}

void table::resizeEvent(QResizeEvent *event)
{
    int m=columnCount();
    for(int i=0; i<m; i++)
    {
        setColumnWidth(i,event->size().width()/m);
    }

}

void table::curId(int id)
{
    currentId = item(id,0)->text();
    if (rowCount()==0) currentId="-1";
    id_row=id;
}

