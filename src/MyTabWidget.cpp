#include "MyTabWidget.h"

MyTabWidget::MyTabWidget( QWidget* parent /* = 0 */ ) : QTabWidget( parent ) {
}

void MyTabWidget::mouseDoubleClickEvent( QMouseEvent* event ) {
    if( event->buttons() == Qt::LeftButton )
        emit( doubleClicked() );
}
