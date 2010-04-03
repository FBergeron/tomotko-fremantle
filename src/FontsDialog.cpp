#include "FontsDialog.h"

FontsDialog::FontsDialog( QWidget* parent, Preferences* prefs ) 
    : QDialog( parent ), prefs( prefs ) {
    init();
}

FontsDialog::~FontsDialog() {
}

void FontsDialog::init() {
    setModal( true ); 

    mainLayout = new QBoxLayout( QBoxLayout::LeftToRight );

    fontsPanel = new QWidget();
    fontsPanelLayout = new QVBoxLayout();
    fontsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    fontsPanel->setLayout( fontsPanelLayout );

    fontsPanelWrapper = new QScrollArea();
    fontsPanelWrapper->setWidget( fontsPanel );

    labelsFontPanel = new QWidget();
    labelsFontPanelLayout = new QHBoxLayout();
    labelsFontPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    labelsFontPanel->setLayout( labelsFontPanelLayout );
    fontsPanelLayout->addWidget( labelsFontPanel );
    labelsFontLabel = new QLabel( tr( "LabelsFont" ) );
    labelsFontFamilyComboBox = new QComboBox();
    labelsFontSizeComboBox = new QComboBox();
    resetDefaultLabelsFontButton = new QPushButton( tr( "ResetDefaultValue" ) );
    connect( resetDefaultLabelsFontButton, SIGNAL( clicked() ), this, SLOT( resetDefaultLabelsFont() ) );
    labelsFontPanelLayout->addWidget( labelsFontLabel );
    labelsFontPanelLayout->addWidget( labelsFontFamilyComboBox );
    labelsFontPanelLayout->addWidget( labelsFontSizeComboBox );
    labelsFontPanelLayout->addWidget( resetDefaultLabelsFontButton );

    initFontFamilyValues( labelsFontFamilyComboBox );
    //selectFontFamily( labelsFontFamilyComboBox, prefs->getLabelsFontFamily() );
    initFontSizeValues( labelsFontSizeComboBox );
    //selectFontSize( labelsFontSizeComboBox, prefs->getLabelsFontSizeModifier() );

    fontPanel = new QWidget();
    fontPanelLayout = new QHBoxLayout();
    fontPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    fontPanel->setLayout( fontPanelLayout );
    fontsPanelLayout->addWidget( fontPanel );
    fontLabel = new QLabel( tr( "FieldsFont" ) );
    fontFamilyComboBox = new QComboBox();
    fontSizeComboBox = new QComboBox();
    resetDefaultFontButton = new QPushButton( tr( "ResetDefaultValue" ) );
    connect( resetDefaultFontButton, SIGNAL( clicked() ), this, SLOT( resetDefaultFont() ) );
    fontPanelLayout->addWidget( fontLabel );
    fontPanelLayout->addWidget( fontFamilyComboBox);
    fontPanelLayout->addWidget( fontSizeComboBox);
    fontPanelLayout->addWidget( resetDefaultFontButton );

    //initFontFamilyValues( fontFamilyComboBox );
    //selectFontFamily( fontFamilyComboBox, prefs->getFontFamily() );
    //initFontSizeValues( fontSizeComboBox );
    //selectFontSize( fontSizeComboBox, prefs->getFontSizeModifier() );

    //fontOverridesPanel = new QWidget();
    //fontOverridesPanelLayout = new QVBoxLayout();
    //fontOverridesPanel->setLayout( fontOverridesPanelLayout );

    //fontOverridesLabel = new QLabel( tr( "LanguageFontOverrides" ) );

    //fontOverridesPanelLayout->addWidget( fontOverridesLabel );

    //fontsPanelLayout->addWidget( fontOverridesPanel );
    //initFontOverrides();

    bottomButtonsPanel = new QWidget();
    bottomButtonsPanelLayout = new QBoxLayout( QBoxLayout::TopToBottom );
    bottomButtonsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    bottomButtonsPanel->setLayout( bottomButtonsPanelLayout );

    acceptButton = new QPushButton( tr( "Ok" ) );
    connect( acceptButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    cancelButton = new QPushButton( tr( "Cancel" ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

    bottomButtonsPanelLayout->addStretch();
    bottomButtonsPanelLayout->addWidget( acceptButton );
    bottomButtonsPanelLayout->addWidget( cancelButton );

    mainLayout->addWidget( fontsPanelWrapper  );
    mainLayout->addWidget( bottomButtonsPanel );

    setLayout( mainLayout );

    setWindowTitle( tr( "Fonts Settings" ) );

    updateUi();
}

void FontsDialog::initFontFamilyValues( QComboBox* comboBox, bool withEmptyEntry /*= false*/ ) const {
    QFontDatabase fontDatabase;
    QStringList families = fontDatabase.families();
    if( withEmptyEntry ) 
        comboBox->addItem( QString( "" ) );
    comboBox->addItems( families );
}

void FontsDialog::initFontSizeValues( QComboBox* comboBox, bool withEmptyEntry /*= false*/ ) const {
    //int fontSizeNameListLength = sizeof( fontSizeNameList ) / sizeof( QString );
    //if( withEmptyEntry ) 
    //    comboBox->addItem( QString( "" ) );
    //for( int i = 0; i < fontSizeNameListLength; i++ )
    //    comboBox->addItem( tr( fontSizeNameList[ i ].toLatin1().data() ) );
}

QString FontsDialog::getLabelsFontFamily() const {
    return( labelsFontFamily );
}

int FontsDialog::getLabelsFontSizeModifier() const {
    return( labelsFontSizeModifier );
}

QString FontsDialog::getFieldsFontFamily() const {
    return( fieldsFontFamily );
}

int FontsDialog::getFieldsFontSizeModifier() const {
    return( fieldsFontSizeModifier );
}

QStringList FontsDialog::getFontOverrideLanguages() const {
    QStringList languages;
    for( QMap<QString,QString>::ConstIterator it = overridesFontFamily.constBegin(); it != overridesFontFamily.constEnd(); it++ ) {
        const QString lang = it.key();
        if( !languages.contains( lang ) )
            languages.append( lang );
    }
    for( QMap<QString,int>::ConstIterator it = overridesFontSizeModifier.constBegin(); it != overridesFontSizeModifier.constEnd(); it++ ) {
        const QString lang = it.key();
        if( !languages.contains( lang ) )
            languages.append( lang );
    }
    return( languages );
}

bool FontsDialog::isFontOverrideFamilyDefined( const QString& lang ) const {
    return( overridesFontFamily.contains( lang ) );  
}

QString FontsDialog::getFontOverrideFamily( const QString& lang ) const {
    return( overridesFontFamily[ lang ] );  
}

bool FontsDialog::isFontOverrideSizeModifierDefined( const QString& lang ) const {
    return( overridesFontSizeModifier.contains( lang ) );  
}

int FontsDialog::getFontOverrideSizeModifier( const QString& lang ) const {
    return( overridesFontSizeModifier[ lang ] );  
}

//void FontsDialog::initStudyLanguageValues() {
//    QStringList sortedLanguages;
//    int studyLanguageListLength = Util::getStudyLanguagesCount();
//    for( int i = 0; i < studyLanguageListLength; i++ ) 
//        sortedLanguages.append( QApplication::translate( "QObject", Util::studyLanguageList[ i ].toLatin1().data() ) );
//    sortedLanguages.sort();
//
//    int languageCount = sortedLanguages.count();
//    for( int i = 0; i < languageCount; i++ ) {
//        QString lang = sortedLanguages.at( i );
//        QString langCode( Util::getLanguageCode( lang ) );
//        bool isStudied( prefs->isStudyLanguage( langCode ) );
//        QCheckBox* languageCheckBox = new QCheckBox( lang );
//        languageCheckBox->setCheckState( isStudied ? Qt::Checked : Qt::Unchecked );
//        //int row = i / 2;
//        //int col = i % 2;
//        int col = ( i < ( languageCount / 2 + 1 ) ? 0 : 1 );
//        int row = i - ( col * ( languageCount / 2 + 1 ) );
//        languagesPanelLayout->addWidget( languageCheckBox, row, col );
//        studyLanguagesItem.append( languageCheckBox );
//    }
//}

void FontsDialog::accept() {
    //if( !isStudyLanguageSelectionValid() ) {
    //    QMessageBox::warning( this, QObject::tr( "Warning" ), tr( "StudyLanguagesMandatory" ) );
    //    return;
    //}

    QDialog::accept();
}

//bool FontsDialog::isStudyLanguageSelectionValid() const {
//    //int checkedLangCount = 0;
//    //int studyLanguageItemCount = studyLanguagesItem.count();
//    //for( int i = 0; i < studyLanguageItemCount; i++ ) {
//    //    QCheckBox* languageCheckBox = studyLanguagesItem.at( i );
//    //    bool isChecked = ( languageCheckBox->checkState() != Qt::Unchecked );
//    //    if( isChecked )
//    //        checkedLangCount++;
//    //}
//    //return( checkedLangCount >= 2 );
//}

void FontsDialog::resizeEvent( QResizeEvent* evt ) {
    //languagesPanelWrapper->widget()->resize( languagesPanelWrapper->maximumViewportSize().width() - languagesPanelWrapper->verticalScrollBar()->size().width() - 40, 
    //    languagesPanel->size().height() );
}

void FontsDialog::updateUi() {
    //if( keyboardAccelListView->currentItem() ) {
    //    clearAccelKeyButton->setEnabled( true ); 
    //    setAccelKeyButton->setEnabled( true );
    //    resetAccelKeyButton->setEnabled( true );
    //}
    //else {
    //    clearAccelKeyButton->setEnabled( false ); 
    //    setAccelKeyButton->setEnabled( false );
    //    resetAccelKeyButton->setEnabled( false );
    //}
}


void FontsDialog::resetDefaultLabelsFont() {
    //selectFontFamily( labelsFontFamilyComboBox, prefs->getDefaultLabelsFontFamily() );
    //selectFontSize( labelsFontSizeComboBox, prefs->getDefaultLabelsFontSizeModifier() );
}

void FontsDialog::resetDefaultFont() {
    //selectFontFamily( fontFamilyComboBox, prefs->getDefaultFontFamily() );
    //selectFontSize( fontSizeComboBox, prefs->getDefaultFontSizeModifier() );
}

