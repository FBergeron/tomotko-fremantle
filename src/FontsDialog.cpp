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
    selectFontFamily( labelsFontFamilyComboBox, prefs->getLabelsFontFamily() );
    initFontSizeValues( labelsFontSizeComboBox );
    selectFontSize( labelsFontSizeComboBox, prefs->getLabelsFontSizeModifier() );

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

    initFontFamilyValues( fontFamilyComboBox );
    selectFontFamily( fontFamilyComboBox, prefs->getFontFamily() );
    initFontSizeValues( fontSizeComboBox );
    selectFontSize( fontSizeComboBox, prefs->getFontSizeModifier() );

    fontOverridesLabel = new QLabel( tr( "LanguageFontOverrides" ) );
    fontsPanelLayout->addWidget( fontOverridesLabel );

    initFontOverrides();

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

    fontsPanelWrapper->setWidget( fontsPanel );

    mainLayout->addWidget( fontsPanelWrapper  );
    mainLayout->addWidget( bottomButtonsPanel );

    setLayout( mainLayout );

    setWindowTitle( tr( "Fonts Settings" ) );
}

void FontsDialog::initFontFamilyValues( QComboBox* comboBox, bool withEmptyEntry /*= false*/ ) const {
    QFontDatabase fontDatabase;
    QStringList families = fontDatabase.families();
    if( withEmptyEntry ) 
        comboBox->addItem( QString( "" ) );
    comboBox->addItems( families );
}

void FontsDialog::initFontSizeValues( QComboBox* comboBox, bool withEmptyEntry /*= false*/ ) const {
    int fontSizeNameListLength = Util::getFontSizesCount();
    if( withEmptyEntry ) 
        comboBox->addItem( QString( "" ) );
    for( int i = 0; i < fontSizeNameListLength; i++ )
        comboBox->addItem( tr( Util::fontSizeNameList[ i ].toLatin1().data() ) );
}

void FontsDialog::initFontOverrides() {
    QList<QString> lang( prefs->getStudyLanguages() );
    for( int i = 0; i < lang.count(); i++ ) {
        QString language = lang.at( i );
        QWidget* fontOverrideBox = new QWidget();
        QHBoxLayout* fontOverrideBoxLayout = new QHBoxLayout();
        fontOverrideBox->setLayout( fontOverrideBoxLayout );
        QLabel* fontOverrideLabel = new QLabel( QApplication::translate( "QObject", language.toLatin1().data() ) );
        QComboBox* fontOverrideFamilyComboBox = new QComboBox();
        initFontFamilyValues( fontOverrideFamilyComboBox, true );
        if( prefs->isFontOverrideFamilyDefined( language ) ) 
            selectFontFamily( fontOverrideFamilyComboBox, prefs->getFontOverrideFamily( language ) );
        QComboBox* fontOverrideSizeComboBox = new QComboBox();
        initFontSizeValues( fontOverrideSizeComboBox, true );
        if( prefs->isFontOverrideSizeDefined( language ) ) 
            selectFontSize( fontOverrideSizeComboBox, prefs->getFontOverrideSize( language ), true );
        fontOverrideBoxLayout->addWidget( fontOverrideLabel );
        fontOverrideBoxLayout->addWidget( fontOverrideFamilyComboBox );
        fontOverrideBoxLayout->addWidget( fontOverrideSizeComboBox );

        fontOverrideBoxes.append( fontOverrideBox );
        fontOverrideLabels.append( fontOverrideLabel );
        fontOverrideFamilyComboBoxes.append( fontOverrideFamilyComboBox );
        fontOverrideSizeComboBoxes.append( fontOverrideSizeComboBox );

        fontsPanelLayout->addWidget( fontOverrideBox, 0 );
    }
}

void FontsDialog::selectFontFamily( QComboBox* comboBox, const QString& fontFamily ) {
    for( int i = 0; i < comboBox->count(); i++ ) {
        if( comboBox->itemText( i ) == fontFamily )
            comboBox->setCurrentIndex( i );
    }
}

void FontsDialog::selectFontSize( QComboBox* comboBox, int fontSizeModifier, bool withEmptyEntry /*= false*/ ) {
    int fontSizeNameListLength = Util::getFontSizesCount();
    int sizeIndex = fontSizeModifier + ( fontSizeNameListLength - 1 ) / 2;
    if( withEmptyEntry )
        sizeIndex++;
    comboBox->setCurrentIndex( sizeIndex );
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

void FontsDialog::accept() {
    labelsFontFamily = labelsFontFamilyComboBox->currentText();
    labelsFontSizeModifier =  labelsFontSizeComboBox->currentIndex() - ( Util::getFontSizesCount() - 1 ) / 2;

    fieldsFontFamily = fontFamilyComboBox->currentText();
    fieldsFontSizeModifier =  fontSizeComboBox->currentIndex() - ( Util::getFontSizesCount() - 1 ) / 2;

    overridesFontFamily.clear();
    overridesFontSizeModifier.clear();
    int fontOverrideCount = fontOverrideLabels.count();
    for( int i = 0; i < fontOverrideCount; i++ ) {
        const QString& language = Util::getLanguageCode( fontOverrideLabels.at( i )->text() );
        if( fontOverrideFamilyComboBoxes.at( i )->currentIndex() > 0 )
            overridesFontFamily[ language ] = fontOverrideFamilyComboBoxes.at( i )->currentText();
        if( fontOverrideSizeComboBoxes.at( i )->currentIndex() > 0 ) {
            int fontSizeModifier =  ( fontOverrideSizeComboBoxes.at( i )->currentIndex() - 1 ) - ( Util::getFontSizesCount() - 1 ) / 2;
            overridesFontSizeModifier[ language ] = fontSizeModifier;
        }
    }

    QDialog::accept();
}

void FontsDialog::resizeEvent( QResizeEvent* ) {
    fontsPanelWrapper->widget()->resize( fontsPanelWrapper->maximumViewportSize().width() - fontsPanelWrapper->verticalScrollBar()->size().width() - 40, 
        fontsPanel->size().height() );
}

void FontsDialog::resetDefaultLabelsFont() {
    selectFontFamily( labelsFontFamilyComboBox, prefs->getDefaultLabelsFontFamily() );
    selectFontSize( labelsFontSizeComboBox, prefs->getDefaultLabelsFontSizeModifier() );
}

void FontsDialog::resetDefaultFont() {
    selectFontFamily( fontFamilyComboBox, prefs->getDefaultFontFamily() );
    selectFontSize( fontSizeComboBox, prefs->getDefaultFontSizeModifier() );
}

