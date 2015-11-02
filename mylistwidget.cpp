#include <QKeyEvent>
#include <QtGui/QApplication>
#include <QClipboard>
#include "mylistwidget.h"

MyListWidget::MyListWidget(QWidget *parent) :
    QListWidget(parent)
{
}

void MyListWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->matches(QKeySequence::Copy))
    {
        QStringList strings;
        QList<QListWidgetItem*> items;
        items = selectedItems();
        for(int i=0; i < items.size(); ++i)
        {
            strings << items.at(i)->text();
            items.at(i)->setSelected(false);
        }
        QApplication::clipboard()->setText(strings.join("\n"));


    }
}

void MyListWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        QList<QListWidgetItem*> items;
        items = selectedItems();
        for(int i=0; i < items.size(); ++i)
        {
            items.at(i)->setSelected(false);
        }

    }else{
        QListWidget::mousePressEvent(event);
    }
}
