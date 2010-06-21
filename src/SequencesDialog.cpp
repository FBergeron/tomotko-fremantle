#include "SequencesDialog.h"

SequencesDialog::SequencesDialog( QWidget* parent, Preferences* prefs ) 
    : QDialog( parent ), prefs( prefs ) {
    init();
}

SequencesDialog::~SequencesDialog() {
}

void SequencesDialog::init() {
    setModal( true ); 
    
    sequencesPanel = new QWidget();
    sequencesPanelLayout = new QHBoxLayout();
    sequencesPanel->setLayout( sequencesPanelLayout );

    sequencesViewPanel = new QWidget(); 
    sequencesViewPanelLayout = new QBoxLayout( QBoxLayout::TopToBottom );
    sequencesViewPanelLayout->setContentsMargins( 0, 10, 0, 0 );
    sequencesViewPanel->setLayout( sequencesViewPanelLayout );
    //revealingOptionsPanelLayout->addWidget( sequencesViewPanel );
    sequencesPanelLayout->addWidget( sequencesViewPanel );
    sequencesView = new DynamicHeightTreeWidget();
    sequencesView->setHeaderLabel( tr( "Existing Sequences" ) );
    sequencesViewPanelLayout->addWidget( sequencesView );
    sequencesViewButtons = new QWidget();
    sequencesViewButtonsLayout = new QBoxLayout( QBoxLayout::LeftToRight );
    sequencesViewButtonsLayout->setContentsMargins( 0, 0, 0, 0 );
    sequencesViewButtons->setLayout( sequencesViewButtonsLayout );
    sequencesViewPanelLayout->addWidget( sequencesViewButtons );
    addSequenceButton = new QPushButton( "+" );
    sequencesViewButtonsLayout->addStretch();
    sequencesViewButtonsLayout->addWidget( addSequenceButton );
    //addSequenceButton->setToolTip( tr( "Add revealing sequence" ) );
    connect( addSequenceButton, SIGNAL( clicked() ), this, SLOT( addSequence() ) );
    removeSequenceButton = new QPushButton( "-" );
    sequencesViewButtonsLayout->addWidget( removeSequenceButton );
    //removeSequenceButton->setToolTip( tr( "Remove revealing sequence" ) );
    connect( removeSequenceButton, SIGNAL( clicked() ), this, SLOT( removeSequence() ) );

    initSequences();
    connect( sequencesView, SIGNAL( itemSelectionChanged() ), this, SLOT( updateUi() ) );

    sequencesLabelBox = new QWidget();
    sequencesLabelBoxLayout = new QVBoxLayout();
    sequencesLabelBox->setLayout( sequencesLabelBoxLayout );

    sequencesLabelHeader = new QLabel( tr( "QuizWindow" ) );
    sequencesLabelBoxLayout->addWidget( sequencesLabelHeader );

    sequencesLabel = new QLabel();
    sequencesLabelBoxLayout->addWidget( sequencesLabel );
    sequencesLabel->setPixmap( QPixmap( ":/pics/SequenceMapLandscape.png" ) );

    sequencesPanelLayout->addWidget( sequencesViewPanel );
    sequencesPanelLayout->addWidget( sequencesLabelBox ); 

    bottomButtonsPanel = new QWidget();
    bottomButtonsPanelLayout = new QVBoxLayout();
    bottomButtonsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    bottomButtonsPanel->setLayout( bottomButtonsPanelLayout );

    acceptButton = new QPushButton( tr( "Ok" ) );
    connect( acceptButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    cancelButton = new QPushButton( tr( "Cancel" ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

    bottomButtonsPanelLayout->addStretch();
    bottomButtonsPanelLayout->addWidget( acceptButton );
    bottomButtonsPanelLayout->addWidget( cancelButton );

    mainLayout = new QBoxLayout( QBoxLayout::LeftToRight );

    mainLayout->addWidget( sequencesPanel );
    mainLayout->addWidget( bottomButtonsPanel );

    setLayout( mainLayout );

    setWindowTitle( tr( "RevealingOrders" ) );

    updateUi();
}

void SequencesDialog::accept() {
    if( !isSequenceSelectionValid() ) {
        QMessageBox::warning( this, QObject::tr( "Warning" ), tr( "RevealingOrderMandatory" ) );
        return;
    }

    QDialog::accept();
}

void SequencesDialog::initSequences() {
    int seqCount = prefs->getRevealingSequenceCount();
    for( int i = 0; i < seqCount; i++ ) {
        Sequence seq = prefs->getRevealingSequenceAt( i );
        SequenceListItem* item = new SequenceListItem( sequencesView, seq.toHumanReadableString(), seq ); 
        item->setOn( seq.isEnabled() );
    }
}

bool SequencesDialog::isSequenceSelectionValid() const {
    for( int i = 0; i < sequencesView->topLevelItemCount(); i++ ) {
        SequenceListItem* item = (SequenceListItem*)sequencesView->topLevelItem( i );
        bool isChecked = ( item->checkState( 0 ) != Qt::Unchecked );
        if( isChecked )
            return( true );
    }

    return( false );
}

void SequencesDialog::resizeEvent( QResizeEvent* evt ) {
    QString pictFilename( ":/pics/SequenceMap" + (QString)( evt->size().width() > evt->size().height() ? "Landscape" : "Portrait" ) + ".png" );
    sequencesLabel->setPixmap( QPixmap( pictFilename ) );
    sequencesViewPanel->setMaximumHeight( sequencesLabelBox->sizeHint().height() );
}

void SequencesDialog::updateUi() {
    removeSequenceButton->setEnabled( sequencesView->currentItem() ); 
}

bool SequencesDialog::isRevealingSequenceDefined( const QString& seqStr ) const {
    for( int i = 0; i < sequencesView->topLevelItemCount(); i++ ) {
        SequenceListItem* item = (SequenceListItem*)sequencesView->topLevelItem( i );
        if( item->getSequence().toHumanReadableString() == seqStr )
            return( true );
    }

    return( false );
}

void SequencesDialog::addSequence() {
    SequenceDialog dialog( prefs, this );
    dialog.show();
    int result = dialog.exec();
    if( result ) {
        Sequence sequence = dialog.getSequence();
        // Just add new sequence.  Ignore duplicates.
        if( !isRevealingSequenceDefined( sequence.toHumanReadableString() ) ) {
            SequenceListItem* item = new SequenceListItem( sequencesView, sequence.toHumanReadableString(), sequence ); 
            item->setOn( true );
        }
    }
}

void SequencesDialog::removeSequence() {
    QTreeWidgetItem* currSeqItem = sequencesView->currentItem();
    if( currSeqItem ) {
        delete( currSeqItem );
        updateUi();
    }
}

int SequencesDialog::getSequenceCount() const {
    return( sequencesView->topLevelItemCount() );
}

Sequence SequencesDialog::getSequenceAt( int index ) const {
    SequenceListItem* item = (SequenceListItem*)sequencesView->topLevelItem( index );
    Sequence seq = item->getSequence();
    return( seq );
}

bool SequencesDialog::isSequenceEnabled( int index ) const {
    SequenceListItem* item = (SequenceListItem*)sequencesView->topLevelItem( index );
    bool isChecked = ( item->checkState( 0 ) != Qt::Unchecked );
    return( isChecked );
}

