#ifndef DYNAMIC_HEIGHT_TREE_WIDGET_H
#define DYNAMIC_HEIGHT_TREE_WIDGET_H 

#include <iostream>
#include <QHeaderView>
#include <QTreeWidget>

using namespace std;

class DynamicHeightTreeWidget : public QTreeWidget {

    Q_OBJECT
    
public:

    DynamicHeightTreeWidget( QWidget* parent = 0 );
    ~DynamicHeightTreeWidget(); 

protected:

    virtual QSize sizeHint() const;

};

#endif

