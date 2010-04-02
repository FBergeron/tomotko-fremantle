#include "PreferencesDialog.h"
#include "icons/flag_en.xpm"
#include "icons/flag_es.xpm"
#include "icons/flag_fr.xpm"
#include "icons/flag_ja.xpm"
#include "icons/flag_zh.xpm"
#include "icons/flag_de.xpm"

const QString PreferencesDialog::fontSizeNameList[] = { 
    QT_TRANSLATE_NOOP( "PreferencesDialog", "VerySmall" ), 
    QT_TRANSLATE_NOOP( "PreferencesDialog", "Small" ), 
    QT_TRANSLATE_NOOP( "PreferencesDialog", "Medium" ), 
    QT_TRANSLATE_NOOP( "PreferencesDialog", "Large" ), 
    QT_TRANSLATE_NOOP( "PreferencesDialog", "VeryLarge" ) 
};

const QString PreferencesDialog::firstLanguageList[] = { 
    QString( "en" ), QString( "fr" ), QString( "es" ), QString( "ja" ) 
};

PreferencesDialog::PreferencesDialog( QWidget* parent, Preferences* prefs ) 
    : QDialog( parent/*, 0, true*/ ), prefs( prefs ) {
    init();
}

PreferencesDialog::~PreferencesDialog() {
}

void PreferencesDialog::init() {
    setModal( true ); 
    
    quizPage = new QWidget();
    quizPageLayout = new QVBoxLayout();
    quizPageLayout->setContentsMargins( 0, 0, 0, 0 );
    quizPage->setLayout( quizPageLayout );
  
    quizPageLabel = new QLabel( tr( "Quiz" ) );

    quizLengthPanel = new QWidget();
    quizLengthPanelLayout = new QHBoxLayout();
    quizLengthPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    quizLengthPanel->setLayout( quizLengthPanelLayout );

    quizLengthLabel = new QLabel( tr( "Length" ) );

    quizLengthSliderPanel = new QWidget();
    quizLengthSliderPanelLayout = new QVBoxLayout();
    quizLengthSliderPanel->setLayout( quizLengthSliderPanelLayout );

    revealingOptionsPanel = new QWidget();
    revealingOptionsPanelLayout = new QVBoxLayout();
    revealingOptionsPanel->setLayout( revealingOptionsPanelLayout );

    revealingOptionsLabel = new QLabel( tr( "RevealingOrders" ) );

    sequencesPanel = new QWidget();
    sequencesPanelLayout = new QHBoxLayout();
    sequencesPanel->setLayout( sequencesPanelLayout );

    sequencesViewPanel = new QWidget(); 
    sequencesViewPanelLayout = new QVBoxLayout();
    sequencesViewPanelLayout->setContentsMargins( 0, 10, 0, 0 );
    sequencesViewPanel->setLayout( sequencesViewPanelLayout );
    //revealingOptionsPanelLayout->addWidget( sequencesViewPanel );
    sequencesPanelLayout->addWidget( sequencesViewPanel );
    //sequencesView = new QTreeWidget();
    sequencesView = new DynamicHeightTreeWidget();
    sequencesView->headerItem()->setHidden( true );
    sequencesViewPanelLayout->addWidget( sequencesView );
    sequencesViewButtons = new QWidget();
    sequencesViewButtonsLayout = new QHBoxLayout();
    sequencesViewButtonsLayout->setContentsMargins( 0, 0, 0, 0 );
    sequencesViewButtons->setLayout( sequencesViewButtonsLayout );
    sequencesViewPanelLayout->addWidget( sequencesViewButtons );
    addSequenceButton = new QPushButton( "+" );
    //sequencesViewButtonsLayout->addStretch();
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

    revealingOptionsPanelLayout->addWidget( revealingOptionsLabel );
    revealingOptionsPanelLayout->addWidget( sequencesPanel );

    sequencesPanelLayout->addWidget( sequencesViewPanel );
    sequencesPanelLayout->addWidget( sequencesLabelBox ); 

    quizLengthSlider = new QSlider( Qt::Horizontal );
    quizLengthSlider->setMinimum( 0 );
    quizLengthSlider->setMaximum( TermScheduler::poolCount - 1 );
    quizLengthSlider->setTickInterval( 1 );
    quizLengthSlider->setTickPosition( QSlider::TicksBothSides );
    quizLengthSlider->setFocusPolicy( Qt::StrongFocus );

    quizLengthSliderPanelLayout->addWidget( quizLengthSlider );
    quizLengthLabelsPanel = new QWidget();
    quizLengthLabelsPanelLayout = new QHBoxLayout();
    quizLengthLabelsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    quizLengthLabelsPanel->setLayout( quizLengthLabelsPanelLayout );
    quizLengthSliderPanelLayout->addWidget( quizLengthLabelsPanel );

    quizLengthShortestLabel = new QLabel( tr( "QuizLengthShortest" ) );
    quizLengthMediumLabel = new QLabel( tr( "QuizLengthMedium" ) );
    quizLengthMediumLabel->setAlignment( Qt::AlignCenter );
    quizLengthLongestLabel = new QLabel( tr( "QuizLengthLongest" ) );
    quizLengthLongestLabel->setAlignment( Qt::AlignRight );
    quizLengthLabelsPanelLayout->addWidget( quizLengthShortestLabel );
    quizLengthLabelsPanelLayout->addWidget( quizLengthMediumLabel );
    quizLengthLabelsPanelLayout->addWidget( quizLengthLongestLabel );

    quizLengthPanelLayout->addWidget( quizLengthLabel );
    quizLengthPanelLayout->addWidget( quizLengthSliderPanel );

    quizLengthSlider->setValue( prefs->getQuizLength() ); 

    quizPageSeparator = new QFrame();
    quizPageSeparator->setFrameStyle( QFrame::HLine );

    quizPageLayout->addWidget( quizPageLabel );
    quizPageLayout->addWidget( quizLengthPanel );
    quizPageLayout->addWidget( revealingOptionsPanel );
    quizPageLayout->addWidget( quizPageSeparator );

    fontPage = new QWidget();
    fontPageLayout = new QVBoxLayout();
    fontPageLayout->setContentsMargins( 0, 0, 0, 0 );
    fontPage->setLayout( fontPageLayout );

    fontPageLabel = new QLabel( tr( "Fonts" ) );

    fontsPanel = new QWidget();
    fontsPanelLayout = new QVBoxLayout();
    fontsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    fontsPanel->setLayout( fontsPanelLayout );

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

    fontOverridesPanel = new QWidget();
    fontOverridesPanelLayout = new QVBoxLayout();
    fontOverridesPanel->setLayout( fontOverridesPanelLayout );

    fontOverridesLabel = new QLabel( tr( "LanguageFontOverrides" ) );

    fontOverridesPanelLayout->addWidget( fontOverridesLabel );

    fontsPanelLayout->addWidget( fontOverridesPanel );
    initFontOverrides();

    fontPageSeparator = new QFrame();
    fontPageSeparator->setFrameStyle( QFrame::HLine );

    fontPageLayout->addWidget( fontPageLabel );
    fontPageLayout->addWidget( fontsPanel );
    fontPageLayout->addWidget( fontPageSeparator );

    interfacePage = new QWidget();
    interfacePageLayout = new QVBoxLayout();
    interfacePageLayout->setContentsMargins( 0, 0, 0, 0 );
    interfacePage->setLayout( interfacePageLayout );

    interfacePageLabel = new QLabel( tr( "Interface" ) );

    digraphCheckBox = new QCheckBox( tr( "DigraphesEnabled" ) );
    digraphCheckBox->setCheckState( prefs->isDigraphEnabled() ? Qt::Checked : Qt::Unchecked );

    hideQuizButtonCheckBox = new QCheckBox( tr( "hideQuizButtons" ) );
    hideQuizButtonCheckBox->setCheckState( prefs->areQuizButtonsHidden() ? Qt::Checked : Qt::Unchecked );

    showAltTextInTermListCheckBox = new QCheckBox( tr( "ShowAltInTermList" ) );
    showAltTextInTermListCheckBox->setChecked( prefs->isAltInTermListShown() ? Qt::Checked : Qt::Unchecked );

    interfaceLanguagePanel = new QWidget();
    interfaceLanguagePanelLayout = new QHBoxLayout();
    interfaceLanguagePanelLayout->setContentsMargins( 0, 0, 0, 0 );
    interfaceLanguagePanel->setLayout( interfaceLanguagePanelLayout );

    interfaceLanguageLabel = new QLabel( tr( "InterfaceLanguage" ) );
    interfaceLanguageField = new QComboBox();
    //languageActionGroup = new QActionGroup( this ); 
    QDir i18nDir( ":/i18n" );
    QStringList langDirs = i18nDir.entryList();
    QStringList sortedLanguages;
    for( int i = 0; i < langDirs.count(); i++ ) {
        if( langDirs[ i ] == QString( "." ) || langDirs[ i ] == QString( ".." ) )
            continue;
        QString locale = langDirs[ i ];
        QString langDirStr = ":/i18n/" + QString( locale );
        QDir langDir( langDirStr );
        if( langDir.exists( QString( "toMOTko.qm" ) ) ) {
            QTranslator translator( this );
            translator.load( QString( "toMOTko.qm" ), langDirStr );
            QString language = translator.translate( "QObject", locale.toLatin1().data() );
            sortedLanguages.append( language );
            availableLanguages[ language ] = locale;
        }
    }
    sortedLanguages.sort();
    for( int i = 0; i < sortedLanguages.count(); i++ ) {
        QIcon icon;
        // Refactor this if later.  Look at createAction() method for hint (maybe).
        QString langCode = availableLanguages[ sortedLanguages[ i ] ];
        if( langCode == QString( "en" ) )
            icon = QIcon( QPixmap( flag_en_xpm ) );
        else if( langCode == QString( "es" ) )
            icon = QIcon( QPixmap( flag_es_xpm ) );
        else if( langCode == QString( "fr" ) )
            icon = QIcon( QPixmap( flag_fr_xpm ) );
        else if( langCode == QString( "ja" ) )
            icon = QIcon( QPixmap( flag_ja_xpm ) );
        else if( langCode == QString( "zh" ) )
            icon = QIcon( QPixmap( flag_zh_xpm ) );
        else if( langCode == QString( "de" ) )
            icon = QIcon( QPixmap( flag_de_xpm ) );

        interfaceLanguageField->addItem( icon, sortedLanguages[ i ] );

        if( prefs->getInterfaceLanguage() == availableLanguages[ sortedLanguages[ i ] ] )
            interfaceLanguageField->setCurrentIndex( i );
    }

    interfaceLanguagePanelLayout->addWidget( interfaceLanguageLabel );
    interfaceLanguagePanelLayout->addWidget( interfaceLanguageField );

    keyboardAccelButton = new QPushButton( tr( "Keyboard Accelerator Definitions" ) );
    connect( keyboardAccelButton, SIGNAL( clicked() ), this, SLOT( invokeKeyboardAcceleratorsDialog() ) );

    studyLanguagesButton = new QPushButton( tr( "Study Language Definitions" ) );
    connect( studyLanguagesButton, SIGNAL( clicked() ), this, SLOT( invokeStudyLanguagesDialog() ) );

    interfacePageSeparator = new QFrame();
    interfacePageSeparator->setFrameStyle( QFrame::HLine );

    interfacePageLayout->addWidget( interfacePageLabel );
    interfacePageLayout->addWidget( interfaceLanguagePanel );
    interfacePageLayout->addWidget( keyboardAccelButton );
    interfacePageLayout->addWidget( studyLanguagesButton );
    interfacePageLayout->addWidget( digraphCheckBox );
    interfacePageLayout->addWidget( hideQuizButtonCheckBox ); // Disabled for Fremantle.
    interfacePageLayout->addWidget( showAltTextInTermListCheckBox );
    interfacePageLayout->addWidget( interfacePageSeparator );

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

    mainLayout = new QHBoxLayout();

    bodyWrapper = new QScrollArea();
    //bodyWrapper->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    bodyPanel = new QWidget();
    bodyPanelLayout = new QVBoxLayout();
    bodyPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    bodyPanel->setLayout( bodyPanelLayout );

    bodyPanelLayout->addWidget( interfacePage );
    bodyPanelLayout->addWidget( quizPage );
    bodyPanelLayout->addWidget( fontPage );
//    bodyPanelLayout->addWidget( languagePage );

    bodyWrapper->setWidget( bodyPanel );

    mainLayout->addWidget( bodyWrapper  );
    mainLayout->addWidget( bottomButtonsPanel );

    setLayout( mainLayout );

    setWindowTitle( tr( "Preferences..." ) );

    updateUi();
}

void PreferencesDialog::initFontFamilyValues( QComboBox* comboBox, bool withEmptyEntry /*= false*/ ) const {
    QFontDatabase fontDatabase;
    QStringList families = fontDatabase.families();
    if( withEmptyEntry ) 
        comboBox->addItem( QString( "" ) );
    comboBox->addItems( families );
}

void PreferencesDialog::initFontSizeValues( QComboBox* comboBox, bool withEmptyEntry /*= false*/ ) const {
    int fontSizeNameListLength = sizeof( fontSizeNameList ) / sizeof( QString );
    if( withEmptyEntry ) 
        comboBox->addItem( QString( "" ) );
    for( int i = 0; i < fontSizeNameListLength; i++ )
        comboBox->addItem( tr( fontSizeNameList[ i ].toLatin1().data() ) );
}

void PreferencesDialog::initFontOverrides() {
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

        fontOverridesPanelLayout->addWidget( fontOverrideBox, 0 );
    }
}

void PreferencesDialog::initSequences() {
    int seqCount = prefs->getRevealingSequenceCount();
    for( int i = 0; i < seqCount; i++ ) {
        Sequence seq = prefs->getRevealingSequenceAt( i );
        SequenceListItem* item = new SequenceListItem( sequencesView, seq.toHumanReadableString(), seq ); 
        item->setOn( seq.isEnabled() );
    }
}

void PreferencesDialog::selectFontFamily( QComboBox* comboBox, const QString& fontFamily ) {
    for( int i = 0; i < comboBox->count(); i++ ) {
        if( comboBox->itemText( i ) == fontFamily )
            comboBox->setCurrentIndex( i );
    }
}

void PreferencesDialog::selectFontSize( QComboBox* comboBox, int fontSizeModifier, bool withEmptyEntry /*= false*/ ) {
    int fontSizeNameListLength = sizeof( fontSizeNameList ) / sizeof( QString );
    int sizeIndex = fontSizeModifier + ( fontSizeNameListLength - 1 ) / 2;
    if( withEmptyEntry )
        sizeIndex++;
    comboBox->setCurrentIndex( sizeIndex );
}

void PreferencesDialog::selectLanguage( QComboBox* comboBox, const QString& langCode ) {
    int itemCount = comboBox->count();
    for( int i = 0; i < itemCount; i++ ) {
        if( comboBox->itemText( i ) == QApplication::translate( "QQObject", langCode.toLatin1().data() ) ) { 
            comboBox->setCurrentIndex( i );
            return;
        }
    }
}

void PreferencesDialog::accept() {
    if( !isRevealingSequenceSelectionValid() ) {
        // TODO: Position the scrollbar at the right location.
        QMessageBox::warning( this, QObject::tr( "Warning" ), tr( "RevealingOrderMandatory" ) );
        return;
    }

    prefs->setQuizLength( quizLengthSlider->value() );

    prefs->clearRevealingSequences();
    for( int i = 0; i < sequencesView->topLevelItemCount(); i++ ) {
        SequenceListItem* item = (SequenceListItem*)sequencesView->topLevelItem( i );
        bool isChecked = ( item->checkState( 0 ) != Qt::Unchecked );
        Sequence seq = item->getSequence();
        seq.setEnabled( isChecked );
        prefs->addRevealingSequence( seq );
    }

    prefs->setLabelsFontFamily( labelsFontFamilyComboBox->currentText() );
    int fontSizeNameListLength = sizeof( fontSizeNameList ) / sizeof( QString );
    int labelsFontSizeModifier =  labelsFontSizeComboBox->currentIndex() - ( fontSizeNameListLength - 1 ) / 2;
    prefs->setLabelsFontSizeModifier( labelsFontSizeModifier );

    prefs->setFontFamily( fontFamilyComboBox->currentText() );
    int fontSizeModifier =  fontSizeComboBox->currentIndex() - ( fontSizeNameListLength - 1 ) / 2;
    prefs->setFontSizeModifier( fontSizeModifier );

    prefs->clearFontOverrideFamilies();
    prefs->clearFontOverrideSizes();
    int fontOverrideCount = fontOverrideLabels.count();
    for( int i = 0; i < fontOverrideCount; i++ ) {
        const QString& language = Util::getLanguageCode( fontOverrideLabels.at( i )->text() );
        if( fontOverrideFamilyComboBoxes.at( i )->currentIndex() > 0 )
            prefs->setFontOverrideFamily( language, fontOverrideFamilyComboBoxes.at( i )->currentText() );
        if( fontOverrideSizeComboBoxes.at( i )->currentIndex() > 0 ) {
            int fontSizeModifier =  ( fontOverrideSizeComboBoxes.at( i )->currentIndex() - 1 ) - ( fontSizeNameListLength - 1 ) / 2;
            prefs->setFontOverrideSize( language, fontSizeModifier );
        }
    }

    bool isDigraphEnabled = ( digraphCheckBox->checkState() != Qt::Unchecked );
    prefs->setDigraphEnabled( isDigraphEnabled );

    bool isQuizButtonHiddenChecked = ( hideQuizButtonCheckBox->checkState() != Qt::Unchecked );
    isQuizButtonHiddenChecked = false; // Always show buttons in Fremantle.
    prefs->setQuizButtonsHidden( isQuizButtonHiddenChecked );

    bool isAltInTermListShownChecked = ( showAltTextInTermListCheckBox->checkState() != Qt::Unchecked );
    prefs->setAltInTermListShown( isAltInTermListShownChecked );

    QString interfaceLanguage = availableLanguages[ interfaceLanguageField->currentText() ];
    prefs->setInterfaceLanguage( interfaceLanguage );
    QDialog::accept();
}

void PreferencesDialog::updateUi() {
    removeSequenceButton->setEnabled( sequencesView->currentItem() ); 
}

bool PreferencesDialog::isRevealingSequenceSelectionValid() const {
    for( int i = 0; i < sequencesView->topLevelItemCount(); i++ ) {
        SequenceListItem* item = (SequenceListItem*)sequencesView->topLevelItem( i );
        bool isChecked = ( item->checkState( 0 ) != Qt::Unchecked );
        if( isChecked )
            return( true );
    }

    return( false );
}

bool PreferencesDialog::isRevealingSequenceDefined( const QString& seqStr ) const {
    for( int i = 0; i < sequencesView->topLevelItemCount(); i++ ) {
        SequenceListItem* item = (SequenceListItem*)sequencesView->topLevelItem( i );
        if( item->getSequence().toHumanReadableString() == seqStr )
            return( true );
    }

    return( false );
}

void PreferencesDialog::resizeEvent( QResizeEvent* evt ) {
    QString pictFilename( ":/pics/SequenceMap" + (QString)( evt->size().width() > evt->size().height() ? "Landscape" : "Portrait" ) + ".png" );
    sequencesLabel->setPixmap( QPixmap( pictFilename ) );
    sequencesViewPanel->setMaximumHeight( sequencesLabelBox->sizeHint().height() );
    QDialog::resizeEvent( evt );
}

void PreferencesDialog::resetDefaultLabelsFont() {
    selectFontFamily( labelsFontFamilyComboBox, prefs->getDefaultLabelsFontFamily() );
    selectFontSize( labelsFontSizeComboBox, prefs->getDefaultLabelsFontSizeModifier() );
}

void PreferencesDialog::resetDefaultFont() {
    selectFontFamily( fontFamilyComboBox, prefs->getDefaultFontFamily() );
    selectFontSize( fontSizeComboBox, prefs->getDefaultFontSizeModifier() );
}

void PreferencesDialog::addFontOverride( const QString& language ) {
    // Do nothing if the font override already exists.
    int fontOverrideCount = fontOverrideLabels.count();
    for( int i = 0; i < fontOverrideCount; i++ ) {
        if( fontOverrideLabels.at( i )->text() == QApplication::translate( "QObject", language.toLatin1().data() ) )
            return;
    }
    
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

    //int indexOfFiller = fontOverridesBoxLayout->indexOf( fontOverridesBoxFiller );
    //fontOverridesBoxLayout->insertWidget( indexOfFiller, fontOverrideBox ); 
    fontOverridesPanelLayout->addWidget( fontOverrideBox ); 
}

void PreferencesDialog::removeFontOverride( const QString& language ) {
    int fontOverrideCount = fontOverrideLabels.count();
    int i = 0;
    for( ; i < fontOverrideCount; i++ ) {
        if( fontOverrideLabels.at( i )->text() == QApplication::translate( "QObject", language.toLatin1().data() ) ) {
            QWidget* fontOverrideBox = fontOverrideBoxes.at( i );

            fontOverrideLabels.removeAt( i );
            fontOverrideFamilyComboBoxes.removeAt( i );
            fontOverrideSizeComboBoxes.removeAt( i );
            fontOverrideBoxes.removeAt( i );

            delete( fontOverrideBox );

            return;
        }
    }
}

void PreferencesDialog::updateFontOverride() {
    QStringList fontOverridesToRemove;
    QList<QString> studyLanguages( prefs->getStudyLanguages() );
    int fontOverrideCount = fontOverrideLabels.count();
    for( int i = 0; i < fontOverrideCount; i++ ) {
        QString fontOverrideLang = Util::getLanguageCode( fontOverrideLabels.at( i )->text() );
        if( studyLanguages.contains( fontOverrideLang ) )
            studyLanguages.removeOne( fontOverrideLang );
        else
            fontOverridesToRemove.append( fontOverrideLang );
    }

    for( QStringList::Iterator it = fontOverridesToRemove.begin(); it != fontOverridesToRemove.end(); it++ ) {
        QString fontOverrideLang = *it;
        removeFontOverride( fontOverrideLang );
    }
    
    int remainingStudyLanguageCount = studyLanguages.count();
    for( int i = 0; i < remainingStudyLanguageCount; i++ ) {
        QString newStudyLanguage = studyLanguages.at( i );
        addFontOverride( newStudyLanguage );
    }
}

void PreferencesDialog::invokeKeyboardAcceleratorsDialog() {
    KeyboardAcceleratorsDialog dialog( this, prefs );
    dialog.show();
    int result = dialog.exec();
    if( result && dialog.areAcceleratorsModified() ) {
        const QMap<Action,int> accel = dialog.getAccelerators();
        for( QMap<Action,int>::ConstIterator it = accel.begin(); it != accel.end(); it++ ) {
            Action actionIndex = it.key();
            int actionKey = it.value();
            action[ actionIndex ]->setShortcut( actionKey );
            prefs->setAccelerator( actionIndex, actionKey );
        }
    }
}

void PreferencesDialog::invokeStudyLanguagesDialog() {
    StudyLanguagesDialog dialog( this, prefs );
    dialog.show();
    int result = dialog.exec();
    if( result ) {
        bool firstLanguageExists = false;
        bool testLanguageExists = false;
        QStringList languages = dialog.getStudyLanguages();
        prefs->clearStudyLanguages();
        int studyLanguageItemCount = languages.count();
        for( int i = 0; i < studyLanguageItemCount; i++ ) {
             QString langCode( languages.at( i ) );
             prefs->addStudyLanguage( langCode );
             if( !firstLanguageExists )
                 firstLanguageExists = ( prefs->getFirstLanguage() == langCode );
             if( !testLanguageExists )
                 testLanguageExists = (prefs->getTestLanguage() == langCode );
        }
        if( !firstLanguageExists )
            prefs->setFirstLanguage( QString( "" ) );
        if( !testLanguageExists )
            prefs->setTestLanguage( QString( "" ) );

        updateFontOverride();
    }
}

void PreferencesDialog::addSequence() {
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

void PreferencesDialog::removeSequence() {
    QTreeWidgetItem* currSeqItem = sequencesView->currentItem();
    if( currSeqItem ) {
        delete( currSeqItem );
        updateUi();
    }
}
