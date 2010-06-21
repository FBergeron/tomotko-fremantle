#ifndef MY_TABWIDGET_H
#define MY_TABWIDGET_H

#include <iostream>
#include <QMouseEvent>
#include <QTabWidget>

using namespace std;

class MyTabWidget : public QTabWidget {

    Q_OBJECT

public:

    MyTabWidget( QWidget* parent = 0 );

protected:

    virtual void mouseDoubleClickEvent( QMouseEvent* event );

signals:

    void doubleClicked();

};

#endif
