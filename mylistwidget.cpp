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
        copyToClipboard();
}

void MyListWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        copyToClipboard();

    }else{
        QListWidget::mousePressEvent(event);
    }
}

void MyListWidget::copyToClipboard()
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
