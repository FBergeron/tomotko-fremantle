#include "DynamicHeightTreeWidget.h"

DynamicHeightTreeWidget::DynamicHeightTreeWidget( QWidget* parent /* = 0 */ )
    : QTreeWidget( parent ) {
}

DynamicHeightTreeWidget::~DynamicHeightTreeWidget() {
}

QSize DynamicHeightTreeWidget::sizeHint() const {
    int rowCount = topLevelItemCount();
    if( rowCount == 0 )
        return( QTreeWidget::sizeHint() );
    else {
        const QModelIndex firstRowIndex = indexFromItem( topLevelItem( 0 ) );
        int height = header()->height() + rowCount * rowHeight( firstRowIndex );
        return( QSize( QTreeWidget::sizeHint().width(), height ) );
    }
}
