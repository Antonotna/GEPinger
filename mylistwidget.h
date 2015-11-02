#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include <QListWidget>
#include "mainwindow.h"

namespace Ui {
class MyListWidget;
}


class MyListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MyListWidget(QWidget *parent = 0);

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    void copyToClipboard();
    
signals:
    
public slots:
    
};

#endif // MYLISTWIDGET_H
