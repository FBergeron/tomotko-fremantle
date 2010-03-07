#include "PropertiesPanel.h"

PropertiesPanel::PropertiesPanel( const Preferences& prefs, QWidget* parent ) 
    : QWidget( parent ), prefs( prefs ), editedVocab( NULL ), editedFolder( NULL ) {
    init();
}

void PropertiesPanel::setVocabulary( Vocabulary* vocab ) {
    removeListeners();

    editedVocab = vocab;

    mainLayout->setContentsMargins( 8, 8, 8, 8 );
    titleLineEdit->setText( editedVocab->getTitle() );
    descriptionMultiLineEdit->setText( editedVocab->getDescription() );
    authorField->setText( editedVocab->getAuthor() );
    creationDateValueLabel->setText( editedVocab->getCreationDate().toString() );
    modificationDateValueLabel->setText( editedVocab->getModificationDate().toString() );

    updateCounters();

    addListeners();
}

void PropertiesPanel::setFolder( Folder* folder ) {
    removeListeners();

    editedFolder = folder;

    mainLayout->setContentsMargins( 0, 0, 0, 0 );
    titleLineEdit->setText( editedFolder->getTitle() );
    descriptionMultiLineEdit->setText( editedFolder->getDescription() );
    authorField->setText( editedFolder->getAuthor() );
    creationDateValueLabel->setText( editedFolder->getCreationDate().toString() );
    modificationDateValueLabel->setText( editedFolder->getModificationDate().toString() );

    updateCounters();

    addListeners();
}

void PropertiesPanel::updateCounters() {
    if( getType() == QString( "Vocabulary" ) ) {
        uint termCount = 0;
        uint checkedTermCount = 0;
        uint selectedTermCount = 0;
        if( editedVocab ) {
            bool isReachableFromRoot = editedVocab->isMarkedForStudy() && editedVocab->getParent()->isReachableFromRoot();
            if( prefs.isLanguageFilterEnabled() )
                editedVocab->getItemsCount( &termCount, &checkedTermCount, &selectedTermCount, isReachableFromRoot, 
                    prefs.getFirstLanguage(), prefs.getTestLanguage() );
            else
                editedVocab->getItemsCount( &termCount, &checkedTermCount, &selectedTermCount, isReachableFromRoot );
        }

        contentListView->clear();

        QStringList values;
        values << tr( "Words" ) << QString::number( selectedTermCount ) << QString::number( checkedTermCount ) << QString::number( termCount );
        QTreeWidgetItem* termCountItem = new QTreeWidgetItem( contentListView, values );
        for( int i = 1; i <= 3; i++ )
            termCountItem->setTextAlignment( i, Qt::AlignRight | Qt::AlignVCenter );
        contentListView->addTopLevelItem( termCountItem );
    }
    else if( getType() == QString( "Folder" ) ) {
        uint termCount = 0;
        uint vocabCount = 0;
        uint folderCount = 0;
        uint checkedTermCount = 0;
        uint checkedVocabCount = 0;
        uint checkedFolderCount = 0;
        uint selectedTermCount = 0;
        uint selectedVocabCount = 0;
        uint selectedFolderCount = 0;

        if( editedFolder ) {
            if( prefs.isLanguageFilterEnabled() )
                editedFolder->getItemsCount( &termCount, &vocabCount, &folderCount, 
                    &checkedTermCount, &checkedVocabCount, &checkedFolderCount, 
                        &selectedTermCount, &selectedVocabCount, &selectedFolderCount, 
                            editedFolder->isReachableFromRoot(),
                                prefs.getFirstLanguage(), prefs.getTestLanguage() );
            else
                editedFolder->getItemsCount( &termCount, &vocabCount, &folderCount, 
                    &checkedTermCount, &checkedVocabCount, &checkedFolderCount,
                        &selectedTermCount, &selectedVocabCount, &selectedFolderCount, editedFolder->isReachableFromRoot() );
        }

        contentListView->clear();

        QStringList termCountValues;
        termCountValues << tr( "Words" ) << QString::number( selectedTermCount ) << QString::number( checkedTermCount ) << QString::number( termCount );
        QTreeWidgetItem* termCountItem = new QTreeWidgetItem( contentListView, termCountValues );
        for( int i = 1; i <= 3; i++ )
            termCountItem->setTextAlignment( i, Qt::AlignRight | Qt::AlignVCenter );
        contentListView->addTopLevelItem( termCountItem );

        QStringList vocabCountValues;
        vocabCountValues << tr( "Glossaries" ) << QString::number( selectedVocabCount ) << QString::number( checkedVocabCount ) << QString::number( vocabCount );
        QTreeWidgetItem* vocabCountItem = new QTreeWidgetItem( contentListView, vocabCountValues );
        for( int i = 1; i <= 3; i++ )
            vocabCountItem->setTextAlignment( i, Qt::AlignRight | Qt::AlignVCenter );
        contentListView->addTopLevelItem( vocabCountItem );

        QStringList folderCountValues;
        folderCountValues << tr( "Folders" ) << QString::number( selectedFolderCount ) << QString::number( checkedFolderCount ) << QString::number( folderCount );
        QTreeWidgetItem* folderCountItem = new QTreeWidgetItem( contentListView, folderCountValues );
        for( int i = 1; i <= 3; i++ )
            folderCountItem->setTextAlignment( i, Qt::AlignRight | Qt::AlignVCenter );
        contentListView->addTopLevelItem( folderCountItem );
    }
    contentListView->updateGeometry();
    layout()->invalidate();
}

void PropertiesPanel::init() {
    QFont mediumFont( prefs.getMediumFont() );
    QFont labelsFont( prefs.getLabelsFont() );

    mainLayout = new QVBoxLayout();
    setLayout( mainLayout );

    titlePanel = new QWidget();
    titlePanelLayout = new QHBoxLayout();
    titlePanelLayout->setContentsMargins( 0, 0, 0, 0 );
    titlePanel->setLayout( titlePanelLayout );
    titleLabel = new QLabel( tr( "Title" ) );
    titleLineEdit = new DigraphLineEdit();
    titleLineEdit->setFont( mediumFont );
    titlePanelLayout->addWidget( titleLabel );
    titlePanelLayout->addWidget( titleLineEdit );

    descriptionLabel = new QLabel( tr( "Description" ) );
    descriptionMultiLineEdit = new DigraphMultiLineEdit();
    descriptionMultiLineEdit->setFont( mediumFont );

    contentLabel = new QLabel( tr( "Content" ) );
    contentListView = new QTreeWidget();
    contentListView->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    contentListView->setAllColumnsShowFocus( true );
    contentListView->setSelectionMode( QAbstractItemView::ExtendedSelection );
    contentListView->header()->setStretchLastSection( false );
    QStringList headerLabels;
    headerLabels << tr( "Items" ) << tr( "Selected" ) << tr( "Checked" ) << tr( "Total" );

    contentListView->setHeaderLabels( headerLabels );
    contentListView->header()->setResizeMode( 0, QHeaderView::Stretch );
    contentListView->headerItem()->setTextAlignment( 1, Qt::AlignRight | Qt::AlignVCenter );
    contentListView->headerItem()->setTextAlignment( 2, Qt::AlignRight | Qt::AlignVCenter );
    contentListView->headerItem()->setTextAlignment( 3, Qt::AlignRight | Qt::AlignVCenter );

    simplePropsPanel = new QWidget();
    simplePropsPanelLayout = new QHBoxLayout();
    simplePropsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    simplePropsPanel->setLayout( simplePropsPanelLayout );

    simplePropsLabelsPanel = new QWidget();
    simplePropsLabelsPanelLayout = new QVBoxLayout();
    simplePropsLabelsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    simplePropsLabelsPanel->setLayout( simplePropsLabelsPanelLayout );
    simplePropsPanelLayout->addWidget( simplePropsLabelsPanel );

    simplePropsFieldsPanel = new QWidget();
    simplePropsFieldsPanelLayout = new QVBoxLayout();
    simplePropsFieldsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    simplePropsFieldsPanel->setLayout( simplePropsFieldsPanelLayout );
    simplePropsPanelLayout->addWidget( simplePropsFieldsPanel );

    authorLabel = new QLabel( tr( "Author" ) );
    simplePropsLabelsPanelLayout->addWidget( authorLabel );
    authorField = new DigraphLineEdit(); 
    simplePropsFieldsPanelLayout->addWidget( authorField );
    authorField->setFont( mediumFont );

    creationDateLabel = new QLabel( tr( "CreationDate" ) );
    simplePropsLabelsPanelLayout->addWidget( creationDateLabel );
    creationDateValueLabel = new QLabel();
    simplePropsFieldsPanelLayout->addWidget( creationDateValueLabel );
    creationDateValueLabel->setFont( mediumFont );

    modificationDateLabel = new QLabel( tr( "ModificationDate" ) );
    simplePropsLabelsPanelLayout->addWidget( modificationDateLabel );
    modificationDateValueLabel = new QLabel();
    simplePropsFieldsPanelLayout->addWidget( modificationDateValueLabel );
    modificationDateValueLabel->setFont( mediumFont );

    authorLabel->setMinimumHeight( authorField->sizeHint().height() );
    creationDateLabel->setMinimumHeight( authorField->sizeHint().height() );
    creationDateValueLabel->setMinimumHeight( authorField->sizeHint().height() );
    modificationDateLabel->setMinimumHeight( authorField->sizeHint().height() );
    modificationDateValueLabel->setMinimumHeight( authorField->sizeHint().height() );

    mainLayout->addWidget( titlePanel );
    mainLayout->addWidget( descriptionLabel );
    mainLayout->addWidget( descriptionMultiLineEdit, 1 );
    mainLayout->addWidget( contentLabel );
    mainLayout->addWidget( contentListView );
    mainLayout->addWidget( simplePropsPanel );
}

PropertiesPanel::~PropertiesPanel() {
}

void PropertiesPanel::updateTitle( const QString& title ) {
    if( getType() == QString( "Vocabulary" ) ) {
        editedVocab->setTitle( title );
        editedVocab->setModificationDate( QDateTime::currentDateTime() );
        editedVocab->setDirty( true );
    }
    else if( getType() == QString( "Folder" ) ) {
        editedFolder->setTitle( title );
        editedFolder->setModificationDate( QDateTime::currentDateTime() );
        editedFolder->setDirty( true );
    }
    emit( titleChanged( title ) );
}

void PropertiesPanel::updateAuthor( const QString& author ) {
    if( getType() == QString( "Vocabulary" ) ) {
        editedVocab->setAuthor( author );
        editedVocab->setModificationDate( QDateTime::currentDateTime() );
        editedVocab->setDirty( true );
    }
    else if( getType() == QString( "Folder" ) ) {
        editedFolder->setAuthor( author );
        editedFolder->setModificationDate( QDateTime::currentDateTime() );
        editedFolder->setDirty( true );
    }
}

void PropertiesPanel::updateDescription() {
    if( getType() == QString( "Vocabulary" ) ) {
        editedVocab->setDescription( descriptionMultiLineEdit->toPlainText() );
        editedVocab->setModificationDate( QDateTime::currentDateTime() );
        editedVocab->setDirty( true );
    }
    else if( getType() == QString( "Folder" ) ) {
        editedFolder->setDescription( descriptionMultiLineEdit->toPlainText() );
        editedFolder->setModificationDate( QDateTime::currentDateTime() );
        editedFolder->setDirty( true );
    }
}

QString PropertiesPanel::getType() const {
    return( editedVocab ? QString( "Vocabulary" ) : QString( "Folder" ) );
}

void PropertiesPanel::updateFonts() {
    QFont mediumFont( prefs.getMediumFont() );
    QFont labelsFont( prefs.getLabelsFont() );

    descriptionLabel->setFont( labelsFont );
    descriptionMultiLineEdit->setFont( mediumFont );
    contentLabel->setFont( labelsFont );
    for( int i = 0; i < contentListView->columnCount(); i++ )
        contentListView->headerItem()->setFont( i, labelsFont );
    contentListView->setFont( mediumFont ); 
    titleLabel->setFont( labelsFont );
    titleLineEdit->setFont( mediumFont );
    authorLabel->setFont( labelsFont );
    authorField->setFont( mediumFont );
    creationDateLabel->setFont( labelsFont );
    creationDateValueLabel->setFont( mediumFont );
    modificationDateLabel->setFont( labelsFont );
    modificationDateValueLabel->setFont( mediumFont );
}

void PropertiesPanel::setDigraphEnabled( bool isEnabled ) {
    titleLineEdit->setDigraphEnabled( isEnabled );
    descriptionMultiLineEdit->setDigraphEnabled( isEnabled );
    authorField->setDigraphEnabled( isEnabled );
}

bool PropertiesPanel::isDigraphEnabled() const {
    return( descriptionMultiLineEdit->isDigraphEnabled() );
}

void PropertiesPanel::retranslateUi() {
    descriptionLabel->setText( tr( "Description" ) );
    contentLabel->setText( tr( "Content" ) );
    QStringList headerLabels;
    headerLabels << tr( "Items" ) << tr( "Selected" ) << tr( "Checked" ) << tr( "Total" );
    contentListView->setHeaderLabels( headerLabels );
    titleLabel->setText( tr( "Title" ) );
    authorLabel->setText( tr( "Author" ) );
    creationDateLabel->setText( tr( "CreationDate" ) );
    modificationDateLabel->setText( tr( "ModificationDate" ) );
}

void PropertiesPanel::addListeners() {
    connect( titleLineEdit, SIGNAL( textChanged( const QString& ) ),
        this, SLOT( updateTitle( const QString& ) ) );
    connect( authorField, SIGNAL( textChanged( const QString& ) ),
        this, SLOT( updateAuthor( const QString& ) ) );
    connect( descriptionMultiLineEdit, SIGNAL( textChanged() ),
        this, SLOT( updateDescription() ) );
}

void PropertiesPanel::removeListeners() {
    disconnect( titleLineEdit, SIGNAL( textChanged( const QString& ) ),
        this, SLOT( updateTitle( const QString& ) ) );
    disconnect( authorField, SIGNAL( textChanged( const QString& ) ),
        this, SLOT( updateAuthor( const QString& ) ) );
    disconnect( descriptionMultiLineEdit, SIGNAL( textChanged() ),
        this, SLOT( updateDescription() ) );
}

void PropertiesPanel::resizeEvent( QResizeEvent* evt ) {
    QWidget::resizeEvent( evt );

    int maxCounterColumnHeaderWidth = -1;
    for( int c = 1; c < 4; c++ ) {
        QFontMetrics fm( contentListView->headerItem()->font( c ) );
        int counterColumnHeaderWidth = fm.width( contentListView->headerItem()->text( c ) ) + 10;
        if( counterColumnHeaderWidth > maxCounterColumnHeaderWidth )
            maxCounterColumnHeaderWidth = counterColumnHeaderWidth;
    }
    contentListView->header()->resizeSection( 1, maxCounterColumnHeaderWidth );
    contentListView->header()->resizeSection( 2, maxCounterColumnHeaderWidth );
    contentListView->header()->resizeSection( 3, maxCounterColumnHeaderWidth );
}
