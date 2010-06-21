#include "SequenceDialog.h"

SequenceDialog::SequenceDialog( Preferences* prefs, QWidget* parent ) 
    : QDialog( parent ), prefs( prefs ) {
    init();
}

SequenceDialog::~SequenceDialog() {
}

void SequenceDialog::init() {
    setModal( true );

    sequencePanel = new QWidget();
    sequencePanelLayout = new QBoxLayout( QBoxLayout::TopToBottom );
    sequencePanel->setLayout( sequencePanelLayout );

    quizPanel = new QWidget();
    quizPanelLayout = new QVBoxLayout();
    quizPanel->setLayout( quizPanelLayout );

    quizLabel = new QLabel( tr( "Quiz" ) );

    quizBody = new QWidget();
    quizBodyLayout = new QVBoxLayout();
    quizBodyLayout->setContentsMargins( 0, 0, 0, 0 );
    quizBody->setLayout( quizBodyLayout );

    quizTopPanel = new QWidget();
    quizTopPanelLayout = new QHBoxLayout();
    quizTopPanel->setLayout( quizTopPanelLayout );
    quizBodyLayout->addWidget( quizTopPanel );

    quizTopLeftPanel = new QWidget();
    quizTopLeftPanelLayout = new QVBoxLayout();
    quizTopLeftPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    quizTopLeftPanel->setLayout( quizTopLeftPanelLayout );
    quizTopPanelLayout->addWidget( quizTopLeftPanel );
    quizTopLeftPanel->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Preferred ) );

    quizFirstLangPanel = new QWidget();
    quizFirstLangPanelLayout = new QVBoxLayout();
    quizFirstLangPanelLayout->setContentsMargins( 0, 0, 0, 0 );

    quizFirstLangPanel->setLayout( quizFirstLangPanelLayout );
    quizTopLeftPanelLayout->addWidget( quizFirstLangPanel );

    quizFirstLangPanelWrapper = new QWidget();
    quizFirstLangPanelWrapperLayout = new QHBoxLayout();
    quizFirstLangPanelWrapper->setLayout( quizFirstLangPanelWrapperLayout );

    quizFirstLangPanelLabel = new QLabel( tr( "FirstLanguage" ) );

    quizFirstLangPanelLayout->addWidget( quizFirstLangPanelLabel );
    quizFirstLangPanelLayout->addWidget( quizFirstLangPanelWrapper );
    quizFirstLangLabel = new QLabel( tr( "Word/Expr." ) );
    quizFirstLangPanelWrapperLayout->addWidget( quizFirstLangLabel );
    quizFirstLangTermButton = new QPushButton( "a" );
    quizFirstLangPanelWrapperLayout->addWidget( quizFirstLangTermButton, 1 );
    quizFirstLangTermButton->installEventFilter( this );

    quizTestLangPanel = new QWidget();
    quizTestLangPanelLayout = new QVBoxLayout();
    quizTestLangPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    quizTestLangPanel->setLayout( quizTestLangPanelLayout );
    quizTopLeftPanelLayout->addWidget( quizTestLangPanel );

    quizTestLangPanelWrapper = new QWidget();
    quizTestLangPanelWrapperLayout = new QHBoxLayout();
    quizTestLangPanelWrapper->setLayout( quizTestLangPanelWrapperLayout );

    quizTestLangPanelLabel = new QLabel( tr( "TestLanguage" ) );
    
    quizTestLangPanelLayout->addWidget( quizTestLangPanelLabel );
    quizTestLangPanelLayout->addWidget( quizTestLangPanelWrapper );
    quizTestLangLabelsPanel = new QWidget();
    quizTestLangLabelsPanelLayout = new QVBoxLayout();
    quizTestLangLabelsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    quizTestLangLabelsPanel->setLayout( quizTestLangLabelsPanelLayout );
    quizTestLangPanelWrapperLayout->addWidget( quizTestLangLabelsPanel );
    quizTestLangButtonsPanel = new QWidget();
    quizTestLangButtonsPanelLayout = new QVBoxLayout();
    quizTestLangButtonsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    quizTestLangButtonsPanel->setLayout( quizTestLangButtonsPanelLayout );
    quizTestLangPanelWrapperLayout->addWidget( quizTestLangButtonsPanel, 1 );
    quizTestLangAltLabel = new QLabel( tr( "Alt./Phon." ) );
    quizTestLangLabelsPanelLayout->addWidget( quizTestLangAltLabel );
    quizTestLangTermLabel = new QLabel( tr( "Word/Expr." ) );
    quizTestLangLabelsPanelLayout->addWidget( quizTestLangTermLabel );
    quizTestLangAltButton = new QPushButton( "b" );
    quizTestLangButtonsPanelLayout->addWidget( quizTestLangAltButton );
    quizTestLangAltButton->installEventFilter( this );
    quizTestLangTermButton = new QPushButton( "c" );
    quizTestLangButtonsPanelLayout->addWidget( quizTestLangTermButton );
    quizTestLangTermButton->installEventFilter( this );

    quizImagePanel = new QWidget();
    quizImagePanelLayout = new QVBoxLayout();
    quizImagePanel->setLayout( quizImagePanelLayout );
    quizTopPanelLayout->addWidget( quizImagePanel );
    quizImagePanel->setFixedWidth( 130 );
    quizImageLabel = new QLabel( tr( "Image" ) );
    quizImageButton = new QPushButton( "e" );
    quizImagePanelLayout->addWidget( quizImageLabel );
    quizImagePanelLayout->addWidget( quizImageButton );
    quizImageButton->installEventFilter( this );
    quizImageButton->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );

    quizCommentBox = new QWidget();
    quizCommentBox->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
    quizCommentBoxLayout = new QVBoxLayout();
    quizCommentBox->setLayout( quizCommentBoxLayout );
    quizBodyLayout->addWidget( quizCommentBox, 1 );

    quizCommentLabel = new QLabel( tr( "CommentLabelPanel" ) );
    quizCommentBoxLayout->addWidget( quizCommentLabel );
    quizCommentButton = new QPushButton( "d" );
    quizCommentButton->setMinimumHeight( 100 );
    quizCommentButton->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
    quizCommentBoxLayout->addWidget( quizCommentButton, 1 );
    quizCommentButton->installEventFilter( this );

    sequenceLinePanel = new QWidget();
    sequenceLinePanelLayout = new QBoxLayout( QBoxLayout::LeftToRight );
    sequenceLinePanelLayout->setContentsMargins( 0, 0, 0, 0 );
    sequenceLinePanelLayout->setSpacing( 0 );
    sequenceLinePanel->setLayout( sequenceLinePanelLayout );
    sequenceLineLabel = new QLabel( tr( "Sequence" ) );
    //sequenceLinePanelLayout->addWidget( sequenceLineLabel ); // No space for this label, really.
    sequenceLineLineEdit = new QLineEdit();
    sequenceLinePanelLayout->addWidget( sequenceLineLineEdit, 1 );
    sequenceLineLineEdit->setReadOnly( true );

    sequenceConsolePanel = new QWidget();
    sequenceConsolePanelLayout = new QBoxLayout( QBoxLayout::LeftToRight );
    sequenceConsolePanel->setLayout( sequenceConsolePanelLayout );

    sequenceConsolePanelLayout->addWidget( sequenceLinePanel );

    sequenceButtonsPanel = new QWidget();
    sequenceButtonsPanelLayout = new QGridLayout();
    sequenceButtonsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    sequenceButtonsPanel->setLayout( sequenceButtonsPanelLayout );

    addSequenceMarkButton = new QPushButton( ">" );
    addSequenceMarkButton->setEnabled( false );
    sequenceButtonsPanelLayout->addWidget( addSequenceMarkButton, 0, 0 );
    connect( addSequenceMarkButton, SIGNAL( clicked() ), this, SLOT( addSequenceMark() ) );

    addGroupMarkButton = new QPushButton( "+" );
    addGroupMarkButton->setEnabled( false );
    sequenceButtonsPanelLayout->addWidget( addGroupMarkButton, 0, 1 );
    connect( addGroupMarkButton, SIGNAL( clicked() ), this, SLOT( addGroupMark() ) );

    removeLastMarkButton = new QPushButton( "X" );
    removeLastMarkButton->setEnabled( false );
    sequenceButtonsPanelLayout->addWidget( removeLastMarkButton, 0, 2 );
    connect( removeLastMarkButton, SIGNAL( clicked() ), this, SLOT( removeLastToken() ) );

    sequenceConsolePanelLayout->addWidget( sequenceButtonsPanel );

    bottomButtonsPanel = new QWidget();
    bottomButtonsPanelLayout = new QBoxLayout( QBoxLayout::TopToBottom );
    bottomButtonsPanel->setLayout( bottomButtonsPanelLayout );

    acceptButton = new QPushButton( tr( "Ok" ) );
    connect( acceptButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    cancelButton = new QPushButton( tr( "Cancel" ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

    bottomButtonsPanelLayout->addStretch();
    bottomButtonsPanelLayout->addWidget( acceptButton );
    bottomButtonsPanelLayout->addWidget( cancelButton );

    quizPanelLayout->addWidget( quizLabel );
    quizPanelLayout->addWidget( quizBody );

    quizPanelWrapper = new QScrollArea();
    quizPanelWrapper->setWidget( quizPanel );

    sequencePanelLayout->addWidget( quizPanelWrapper, 1 );
    sequencePanelLayout->addWidget( sequenceConsolePanel );

    mainLayout = new QBoxLayout( QBoxLayout::LeftToRight );
    mainLayout->addWidget( sequencePanel, 1 );
    mainLayout->addWidget( bottomButtonsPanel );
    mainLayout->activate();

    setLayout( mainLayout );

    setWindowTitle( tr( "AddRevealingSequence" ) );
}

Sequence SequenceDialog::getSequence() const {
    return( sequence );
}

void SequenceDialog::accept() {
    if( sequenceLineLineEdit->text().isEmpty() ) {
        QMessageBox::warning( this, QObject::tr( "Warning" ), tr( "SequenceIsEmpty" ) );
        return;
    }

    QString lastChar = sequenceLineLineEdit->text().right( 1 );
    if( lastChar == " " || lastChar == "+" ) {
        QMessageBox::warning( this, QObject::tr( "Warning" ), tr( "SequenceNotTerminated" ) );
        return;
    }

    currGroup.append( items.top() );
    sequence.addGroup( currGroup );
    currGroup.clear();

    //trace();

    QDialog::accept();
}

bool SequenceDialog::eventFilter( QObject* obj, QEvent* evt ) {
    if( evt->type() == QEvent::MouseButtonRelease ) {
        QMouseEvent* mouseEvt = (QMouseEvent*)evt;
        if( mouseEvt->button() == Qt::LeftButton ) {
            if( obj && obj->inherits( "QPushButton" ) ) {
                QPushButton* button = (QPushButton*)obj;
                if( button->isEnabled() ) {
                    addSequenceItem( button->text() );
                    return( true );
                }
            }
        }
    }
    return( false );
}

void SequenceDialog::addSequenceItem( const QString& itemStr ) {
    sequenceLineLineEdit->setText( items.count() == 0 ? itemStr : sequenceLineLineEdit->text() + itemStr );
    Sequence::Item item = Sequence::stringToItem( itemStr );
    items.push( item );
    setItemButtonsEnabled( false );
    setUnionButtonsEnabled( true );
    removeLastMarkButton->setEnabled( true );
    sequenceLineLineEdit->deselect();
    //trace();
}

void SequenceDialog::addSequenceMark() {
    currGroup.append( items.top() );
    sequence.addGroup( currGroup );
    currGroup.clear();
    sequenceLineLineEdit->setText( sequenceLineLineEdit->text() + " > " );
    setItemButtonsEnabled( true );
    setUnionButtonsEnabled( false );
    removeLastMarkButton->setEnabled( true );
    //trace();
}

void SequenceDialog::addGroupMark() {
    currGroup.append( items.top() );
    sequenceLineLineEdit->setText( sequenceLineLineEdit->text() + "+" );
    setItemButtonsEnabled( true );
    setUnionButtonsEnabled( false );
    removeLastMarkButton->setEnabled( true );
    //trace();
}

void SequenceDialog::removeLastToken() {
    QString seqStr = sequenceLineLineEdit->text();
    QString lastChar = seqStr.right( 1 );
    if( lastChar == "+" ) {
        int index = currGroup.indexOf( items.top() );
        currGroup.removeAt( index );
        setItemButtonsEnabled( false );
        setUnionButtonsEnabled( true );
        sequenceLineLineEdit->setText( seqStr.left( seqStr.length() - 1 ) );
    }
    else if( lastChar == " " ) { /* " > " */
        Sequence::ItemList lastGroup = sequence.getGroupAt( sequence.getGroupCount() - 1 );
        sequence.removeLastGroup();
        currGroup = lastGroup;
        if( !currGroup.isEmpty() )
            currGroup.removeAt( currGroup.count() - 1 );
        setItemButtonsEnabled( false );
        setUnionButtonsEnabled( true );
        sequenceLineLineEdit->setText( seqStr.left( seqStr.length() - 3 ) );
    }
    else { /* "a", "b", "c", "d" or "e" */
        items.pop();
        setItemButtonsEnabled( true );
        setUnionButtonsEnabled( false );
        sequenceLineLineEdit->setText( seqStr.left( seqStr.length() - 1 ) );
    }
    removeLastMarkButton->setEnabled( sequenceLineLineEdit->text().length() > 0 );
    //trace();
}

void SequenceDialog::setUnionButtonsEnabled( bool isEnabled ) {
    addSequenceMarkButton->setEnabled( isEnabled && areItemsLeft() );
    addGroupMarkButton->setEnabled( isEnabled && areItemsLeft() );
}

bool SequenceDialog::isItemUsed( const Sequence::Item& item ) const {
    return( items.contains( item ) );
}

bool SequenceDialog::areItemsLeft() const {
    return( !isItemUsed( Sequence::FIRST_LANG_TERM ) || !isItemUsed( Sequence::TEST_LANG_ALT ) || !isItemUsed( Sequence::TEST_LANG_TERM ) ||
        !isItemUsed( Sequence::COMMENT ) || !isItemUsed( Sequence::IMAGE ) );
}

void SequenceDialog::setItemButtonsEnabled( bool isEnabled ) {
    quizFirstLangTermButton->setEnabled( isEnabled && !isItemUsed( Sequence::FIRST_LANG_TERM ) );
    quizTestLangAltButton->setEnabled( isEnabled && !isItemUsed( Sequence::TEST_LANG_ALT ) );
    quizTestLangTermButton->setEnabled( isEnabled && !isItemUsed( Sequence::TEST_LANG_TERM ) );
    quizCommentButton->setEnabled( isEnabled && !isItemUsed( Sequence::COMMENT ) );
    quizImageButton->setEnabled( isEnabled && !isItemUsed( Sequence::IMAGE ) );
}

void SequenceDialog::resizeEvent( QResizeEvent* ) {
    quizPanelWrapper->widget()->resize( quizPanelWrapper->maximumViewportSize().width() - quizPanelWrapper->verticalScrollBar()->size().width() - 6, 
        quizPanelWrapper->widget()->size().height() );
}

//void SequenceDialog::trace() const {
//    QString itemsDelim;
//    QString currGroupDelim;
//    cout << "debug items=[";
//    for( int i = 0; i < items.count(); i++ ) {
//        cout << qPrintable( itemsDelim ) << items.at( i );
//        itemsDelim = ",";
//    }
//    cout << "]";
//    cout << " currGroup=[";
//    for( int i = 0; i < currGroup.count(); i++ ) {
//        cout << qPrintable( currGroupDelim ) << currGroup.at( i );
//        currGroupDelim = ",";
//    }
//    cout << "] seq=" << qPrintable( sequence.toHumanReadableString() ) << endl;
//}
