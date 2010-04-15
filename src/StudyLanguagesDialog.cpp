#include "StudyLanguagesDialog.h"

StudyLanguagesDialog::StudyLanguagesDialog( QWidget* parent, Preferences* prefs ) 
    : QDialog( parent ), prefs( prefs ) {
    init();
}

StudyLanguagesDialog::~StudyLanguagesDialog() {
}

void StudyLanguagesDialog::init() {
    setModal( true ); 
    
    languagesPanel = new QWidget();
    languagesPanelLayout = new QGridLayout();
    languagesPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    languagesPanel->setLayout( languagesPanelLayout );
   
    initStudyLanguageValues();

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

    languagesPanelWrapper = new QScrollArea();
    languagesPanelWrapper->setWidget( languagesPanel );

    mainLayout->addWidget( languagesPanelWrapper  );
    mainLayout->addWidget( bottomButtonsPanel );

    setLayout( mainLayout );

    setWindowTitle( tr( "Study Language Definitions" ) );
}

QStringList StudyLanguagesDialog::getStudyLanguages() const {
    QStringList languages;
    int langItemCount = studyLanguagesItem.count();
    for( int i = 0; i < langItemCount; i++ ) {
        QCheckBox* languageCheckBox = studyLanguagesItem.at( i );
        bool isChecked = ( languageCheckBox->checkState() != Qt::Unchecked );
        if( isChecked ) {
            QString langCode( Util::getLanguageCode( languageCheckBox->text() ) );
            languages.append( langCode );
        }
    }
    return( languages );
}

void StudyLanguagesDialog::initStudyLanguageValues() {
    QStringList sortedLanguages;
    int studyLanguageListLength = Util::getStudyLanguagesCount();
    for( int i = 0; i < studyLanguageListLength; i++ ) 
        sortedLanguages.append( QApplication::translate( "QObject", Util::studyLanguageList[ i ].toLatin1().data() ) );
    sortedLanguages.sort();

    int languageCount = sortedLanguages.count();
    for( int i = 0; i < languageCount; i++ ) {
        QString lang = sortedLanguages.at( i );
        QString langCode( Util::getLanguageCode( lang ) );
        bool isStudied( prefs->isStudyLanguage( langCode ) );
        QCheckBox* languageCheckBox = new QCheckBox( lang );
        languageCheckBox->setCheckState( isStudied ? Qt::Checked : Qt::Unchecked );
        //int row = i / 2;
        //int col = i % 2;
        int col = ( i < ( languageCount / 2 + 1 ) ? 0 : 1 );
        int row = i - ( col * ( languageCount / 2 + 1 ) );
        languagesPanelLayout->addWidget( languageCheckBox, row, col );
        studyLanguagesItem.append( languageCheckBox );
    }
}

void StudyLanguagesDialog::accept() {
    if( !isStudyLanguageSelectionValid() ) {
        QMessageBox::warning( this, QObject::tr( "Warning" ), tr( "StudyLanguagesMandatory" ) );
        return;
    }

    QDialog::accept();
}

bool StudyLanguagesDialog::isStudyLanguageSelectionValid() const {
    int checkedLangCount = 0;
    int studyLanguageItemCount = studyLanguagesItem.count();
    for( int i = 0; i < studyLanguageItemCount; i++ ) {
        QCheckBox* languageCheckBox = studyLanguagesItem.at( i );
        bool isChecked = ( languageCheckBox->checkState() != Qt::Unchecked );
        if( isChecked )
            checkedLangCount++;
    }
    return( checkedLangCount >= 2 );
}

void StudyLanguagesDialog::resizeEvent( QResizeEvent* ) {
    languagesPanelWrapper->widget()->resize( languagesPanelWrapper->maximumViewportSize().width() - languagesPanelWrapper->verticalScrollBar()->size().width() - 40, 
        languagesPanel->size().height() );
}

