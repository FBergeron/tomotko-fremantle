#include "KeyboardAcceleratorsDialog.h"

KeyboardAcceleratorsDialog::KeyboardAcceleratorsDialog( QWidget* parent, Preferences* prefs ) 
    : QDialog( parent ), prefs( prefs ), grabAccelKeyFor( NULL ), keyboardAccelModified( false )  {
    init();
}

KeyboardAcceleratorsDialog::~KeyboardAcceleratorsDialog() {
}

void KeyboardAcceleratorsDialog::init() {
    setModal( true ); 
    
    keyboardAccelPanel = new QWidget();
    keyboardAccelPanelLayout = new QVBoxLayout();
    keyboardAccelPanel->setLayout( keyboardAccelPanelLayout );

    keyboardAccelListView = new QTreeWidget();
    keyboardAccelListView->setAutoScroll( false );
    keyboardAccelListView->setColumnCount( 2 );

    QStringList headerLabels;
    headerLabels << tr( "Action" ) << tr( "Key" ); 
    keyboardAccelListView->setHeaderLabels( headerLabels );
    keyboardAccelListView->header()->setResizeMode( 0, QHeaderView::Stretch );
    keyboardAccelListView->setIconSize( QSize( 24, 24 ) );
    int actionCount = sizeof( action ) / sizeof( QAction* );
    for( int i = actionCount - 1; i >= 0; i-- ) {
        // Skip desactivated actions
        if( i == ACTION_SHOW_ALL_GLOSSARIES_AND_TERMS )
            continue;

        KeyActionListViewItem* actionItem = new KeyActionListViewItem( keyboardAccelListView, action[ i ], (Action)i );
        actionItem->setText( 0, action[ i ]->text() );
        actionItem->setIcon( 0, action[ i ]->icon() );
        keyboardAccelListView->addTopLevelItem( actionItem );
    }
    connect( keyboardAccelListView, SIGNAL( currentItemChanged( QTreeWidgetItem*, QTreeWidgetItem* ) ), this, SLOT( cancelSetAccelKey() ) );

    keyboardAccelPanelLayout->addWidget( keyboardAccelListView );

    keyboardAccelButtonPanel = new QWidget();
    clearAccelKeyButton = new QPushButton( tr( "Clear key" ) );
    setAccelKeyButton = new QPushButton( tr( "Set key" ) );
    resetAccelKeyButton = new QPushButton( tr( "Reset key" ) );
    
    keyboardAccelButtonPanelLayout = new QBoxLayout( QBoxLayout::TopToBottom, keyboardAccelButtonPanel );
    keyboardAccelButtonPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    keyboardAccelButtonPanelLayout->addStretch();
    keyboardAccelButtonPanelLayout->addWidget( clearAccelKeyButton );
    keyboardAccelButtonPanelLayout->addWidget( setAccelKeyButton );
    keyboardAccelButtonPanelLayout->addWidget( resetAccelKeyButton );

    connect( clearAccelKeyButton, SIGNAL( clicked() ), this, SLOT( clearAccelKey() ) );
    connect( setAccelKeyButton, SIGNAL( clicked() ), this, SLOT( setAccelKey() ) );
    connect( resetAccelKeyButton, SIGNAL( clicked() ), this, SLOT( resetAccelKey() ) );

    bottomButtonsPanel = new QWidget();
    bottomButtonsPanelLayout = new QVBoxLayout();
    bottomButtonsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    bottomButtonsPanel->setLayout( bottomButtonsPanelLayout );

    acceptButton = new QPushButton( tr( "Ok" ) );
    connect( acceptButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    cancelButton = new QPushButton( tr( "Cancel" ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

    bottomButtonsPanelLayout->addWidget( keyboardAccelButtonPanel );
    bottomButtonsPanelLayout->addStretch();
    bottomButtonsPanelLayout->addWidget( acceptButton );
    bottomButtonsPanelLayout->addWidget( cancelButton );

    mainLayout = new QHBoxLayout();

    bodyPanel = new QWidget();
    bodyPanelLayout = new QVBoxLayout();
    bodyPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    bodyPanel->setLayout( bodyPanelLayout );

    bodyPanelLayout->addWidget( keyboardAccelPanel );

    mainLayout->addWidget( bodyPanel  );
    mainLayout->addWidget( bottomButtonsPanel );

    setLayout( mainLayout );

    setWindowTitle( tr( "Keyboard Accelerator Definitions" ) );

    updateUi();

    connect( keyboardAccelListView, SIGNAL( itemSelectionChanged() ), this, SLOT( updateUi() ) ); 
}

void KeyboardAcceleratorsDialog::accept() {
    cancelSetAccelKey();

    for( int i = 0; i < keyboardAccelListView->topLevelItemCount(); i++ ) {
        KeyActionListViewItem* item = (KeyActionListViewItem*)keyboardAccelListView->topLevelItem( i );
        setAccelerator( item->getActionIndex(), item->getKey() );
    }

    QDialog::accept();
}

void KeyboardAcceleratorsDialog::updateUi() {
    if( keyboardAccelListView->currentItem() ) {
        clearAccelKeyButton->setEnabled( true ); 
        setAccelKeyButton->setEnabled( true );
        resetAccelKeyButton->setEnabled( true );
    }
    else {
        clearAccelKeyButton->setEnabled( false ); 
        setAccelKeyButton->setEnabled( false );
        resetAccelKeyButton->setEnabled( false );
    }
}

void KeyboardAcceleratorsDialog::setAccelKey() {
    KeyActionListViewItem* item = (KeyActionListViewItem*)keyboardAccelListView->currentItem();
    if( item == NULL ) 
        return;
    item->setText( 1, tr( "<press key>" ) ); 
    grabAccelKeyFor = item;
    grabKeyboard();  
}

void KeyboardAcceleratorsDialog::clearAccelKey() {
    cancelSetAccelKey();

    KeyActionListViewItem* item = (KeyActionListViewItem*)keyboardAccelListView->currentItem();
    if( item == NULL ) 
        return;
    item->setKey( 0 );
    keyboardAccelModified = true;
}

void KeyboardAcceleratorsDialog::resetAccelKey() {
    cancelSetAccelKey();

    KeyActionListViewItem* item = (KeyActionListViewItem*)keyboardAccelListView->currentItem();
    if( item == NULL ) 
        return;
    item->setKey( prefs->getDefaultAccelerator( item->getActionIndex() ) );
    keyboardAccelModified = true;
}

void KeyboardAcceleratorsDialog::cancelSetAccelKey() {
    if( grabAccelKeyFor != NULL ) {
        KeyActionListViewItem* ka = (KeyActionListViewItem*)grabAccelKeyFor;
        ka->updateText();
        grabAccelKeyFor = NULL;
        releaseKeyboard();
    }
}

void KeyboardAcceleratorsDialog::keyPressEvent( QKeyEvent* evt ) {
    if( grabAccelKeyFor != NULL ) {
        switch (evt->key()) {
            case Qt::Key_Shift:
            case Qt::Key_Control:
            case Qt::Key_Meta:
            case Qt::Key_Alt:
            case Qt::Key_CapsLock:
            case Qt::Key_NumLock:
            case Qt::Key_ScrollLock:
            case Qt::Key_F22:
                evt->ignore();
                break;
            default:
                KeyActionListViewItem *ka = (KeyActionListViewItem*)grabAccelKeyFor;

                QKeySequence keyCode( evt->key() );
                if( (evt->modifiers() & Qt::ShiftModifier) != 0 )
                    keyCode = keyCode | Qt::SHIFT;
                if( (evt->modifiers() & Qt::AltModifier) != 0 )
                    keyCode = keyCode | Qt::ALT;
                if( (evt->modifiers() & Qt::ControlModifier ) != 0 )
                    keyCode = keyCode | Qt::CTRL;      

                releaseKeyboard();

                // Check for duplicate key codes
                bool cancelBinding = false;
                for( int i = 0; i < keyboardAccelListView->topLevelItemCount(); i++ ) {
                    KeyActionListViewItem* item = (KeyActionListViewItem*)keyboardAccelListView->topLevelItem( i ); 
                    if( item == ka ) 
                        continue;

                    if( item->getKey() == keyCode ) {
                        int sel = QMessageBox::warning( this, tr( "Duplicate key binding" ),
                        tr( "Key '" ) + keyCode.toString() + tr( "'\nis currently assigned to\n'" ) + item->getAction()->text() +
                            tr( "'.\nOverwrite the current assignment?" ), QMessageBox::No, QMessageBox::Yes );
                        if( sel == QMessageBox::Yes ) 
                            item->setKey( 0 );
                        else {
                            cancelBinding = true;
                            break;
                        }
                    }
                }

                if( cancelBinding ) 
                    ka->updateText();
                else {
                    ka->setKey( keyCode );
                    keyboardAccelModified = true;
                    ka->updateText();
                }

                grabAccelKeyFor = NULL;
                evt->accept();
                break;
        }
    }
    else if( evt->key() == Qt::Key_Return ) {
        setAccelKey();
    }
}

void KeyboardAcceleratorsDialog::setAccelerator( Action action, int newAccel ) {
    accel[ action ] = newAccel;
}

QMap<Action,int> KeyboardAcceleratorsDialog::getAccelerators() const {
    return( accel );
}

bool KeyboardAcceleratorsDialog::areAcceleratorsModified() const {
    return( keyboardAccelModified );
}
