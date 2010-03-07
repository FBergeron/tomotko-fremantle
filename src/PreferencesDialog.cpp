#include "PreferencesDialog.h"
#include "icons/flag_en.xpm"
#include "icons/flag_es.xpm"
#include "icons/flag_fr.xpm"
#include "icons/flag_ja.xpm"
#include "icons/flag_zh.xpm"
#include "icons/flag_de.xpm"

const QString PreferencesDialog::fontSizeNameList[] = { 
    QT_TR_NOOP( "VerySmall" ), QT_TR_NOOP( "Small" ), QT_TR_NOOP( "Medium" ), QT_TR_NOOP( "Large" ), QT_TR_NOOP( "VeryLarge" ) 
};

const QString PreferencesDialog::firstLanguageList[] = { 
    QString( "en" ), QString( "fr" ), QString( "es" ), QString( "ja" ) 
};

const QString PreferencesDialog::studyLanguageList[] = { 
    QString( "en" ), QString( "fr" ), QString( "es" ), QString( "ja" ), QString( "de" ), QString( "ar" ), QString( "bg" ), QString( "el" ),
    QString( "it" ), QString( "ko" ), QString( "nl" ), QString( "pt" ), QString( "ru" ), QString( "sw" ), QString( "th" ), QString( "vi" ),
    QString( "zh" )
};

PreferencesDialog::PreferencesDialog( QWidget* parent, Preferences* prefs ) 
    : QDialog( parent/*, 0, true*/ ), prefs( prefs ), grabAccelKeyFor( NULL ), keyboardAccelModified( false )  {
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
    sequencesView = new QTreeWidget();
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

    keyboardAccelPanel = new QWidget();
    keyboardAccelPanelLayout = new QVBoxLayout();
    keyboardAccelPanel->setLayout( keyboardAccelPanelLayout );

    keyboardAccelLabel = new QLabel( tr( "Keybord Accelerators" ) );

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
        KeyActionListViewItem* actionItem = new KeyActionListViewItem( keyboardAccelListView, action[ i ], (Action)i );
        actionItem->setText( 0, action[ i ]->text() );
        actionItem->setIcon( 0, action[ i ]->icon() );
        keyboardAccelListView->addTopLevelItem( actionItem );
    }
    connect( keyboardAccelListView, SIGNAL( currentItemChanged( QTreeWidgetItem*, QTreeWidgetItem* ) ), this, SLOT( cancelSetAccelKey() ) );

    keyboardAccelPanelLayout->addWidget( keyboardAccelLabel );
    keyboardAccelPanelLayout->addWidget( keyboardAccelListView );

    keyboardAccelButtonPanel = new QWidget();
    keyboardAccelPanelLayout->addWidget( keyboardAccelButtonPanel );
    clearAccelKeyButton = new QPushButton( tr( "Clear key" ) );
    setAccelKeyButton = new QPushButton( tr( "Set key" ) );
    resetAccelKeyButton = new QPushButton( tr( "Reset key" ) );
    
    keyboardAccelButtonPanelLayout = new QHBoxLayout( keyboardAccelButtonPanel );
    keyboardAccelButtonPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    keyboardAccelButtonPanelLayout->addStretch();
    keyboardAccelButtonPanelLayout->addWidget( clearAccelKeyButton );
    keyboardAccelButtonPanelLayout->addWidget( setAccelKeyButton );
    keyboardAccelButtonPanelLayout->addWidget( resetAccelKeyButton );

    connect( clearAccelKeyButton, SIGNAL( clicked() ), this, SLOT( clearAccelKey() ) );
    connect( setAccelKeyButton, SIGNAL( clicked() ), this, SLOT( setAccelKey() ) );
    connect( resetAccelKeyButton, SIGNAL( clicked() ), this, SLOT( resetAccelKey() ) );

    interfacePageSeparator = new QFrame();
    interfacePageSeparator->setFrameStyle( QFrame::HLine );

    interfacePageLayout->addWidget( interfacePageLabel );
    interfacePageLayout->addWidget( interfaceLanguagePanel );
    //interfacePageLayout->addWidget( keyboardAccelPanel ); // Disabled for Fremantle.
    interfacePageLayout->addWidget( digraphCheckBox );
    interfacePageLayout->addWidget( hideQuizButtonCheckBox ); // Disabled for Fremantle.
    interfacePageLayout->addWidget( showAltTextInTermListCheckBox );
    interfacePageLayout->addWidget( interfacePageSeparator );

    languagePage = new QWidget();
    languagePageLayout = new QVBoxLayout();
    languagePageLayout->setContentsMargins( 0, 0, 0, 0 );
    languagePage->setLayout( languagePageLayout );

    languagePageLabel = new QLabel( tr( "StudyLanguages" ) );

    languagesPanel = new QWidget();
    //languagesPanelLayout = new QVBoxLayout();
    languagesPanelLayout = new QGridLayout();
    languagesPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    languagesPanel->setLayout( languagesPanelLayout );
   
    initStudyLanguageValues();

    languagePageSeparator = new QFrame();
    languagePageSeparator->setFrameStyle( QFrame::HLine );

    languagePageLayout->addWidget( languagePageLabel );
    languagePageLayout->addWidget( languagesPanel );
    languagePageLayout->addWidget( languagePageSeparator );

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
    bodyPanelLayout->addWidget( languagePage );

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

void PreferencesDialog::initStudyLanguageValues() {
    QStringList sortedLanguages;
    int studyLanguageListLength = sizeof( studyLanguageList ) / sizeof( QString );
    for( int i = 0; i < studyLanguageListLength; i++ ) 
        sortedLanguages.append( QApplication::translate( "QObject", studyLanguageList[ i ].toLatin1().data() ) );
    sortedLanguages.sort();

    int languageCount = sortedLanguages.count();
    int languageCountDividedBy2 = (int)( languageCount / 2 + 0.5 );
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
        connect( languageCheckBox, SIGNAL( stateChanged( int ) ), this, SLOT( updateFontOverride() ) );
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
    cancelSetAccelKey();

    if( !isRevealingSequenceSelectionValid() ) {
        // TODO: Position the scrollbar at the right location.
        QMessageBox::warning( this, QObject::tr( "Warning" ), tr( "RevealingOrderMandatory" ) );
        return;
    }

    if( !isStudyLanguageSelectionValid() ) {
        // TODO: Position the scrollbar at the right location.
        QMessageBox::warning( this, QObject::tr( "Warning" ), tr( "StudyLanguagesMandatory" ) );
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

    bool firstLanguageExists = false;
    bool testLanguageExists = false;
    prefs->clearStudyLanguages();
    int studyLanguageItemCount = studyLanguagesItem.count();
    for( int i = 0; i < studyLanguageItemCount; i++ ) {
        QCheckBox* languageCheckBox = studyLanguagesItem.at( i );
        bool isChecked = ( languageCheckBox->checkState() != Qt::Unchecked );
        if( isChecked ) {
            QString langCode( Util::getLanguageCode( languageCheckBox->text() ) );
            prefs->addStudyLanguage( langCode );
            if( !firstLanguageExists )
                firstLanguageExists = ( prefs->getFirstLanguage() == langCode );
            if( !testLanguageExists )
                testLanguageExists = (prefs->getTestLanguage() == langCode );
        }
    }
    if( !firstLanguageExists )
        prefs->setFirstLanguage( QString( "" ) );
    if( !testLanguageExists )
        prefs->setTestLanguage( QString( "" ) );

    bool isDigraphEnabled = ( digraphCheckBox->checkState() != Qt::Unchecked );
    prefs->setDigraphEnabled( isDigraphEnabled );

    bool isQuizButtonHiddenChecked = ( hideQuizButtonCheckBox->checkState() != Qt::Unchecked );
    isQuizButtonHiddenChecked = false; // Always show buttons in Fremantle.
    prefs->setQuizButtonsHidden( isQuizButtonHiddenChecked );

    bool isAltInTermListShownChecked = ( showAltTextInTermListCheckBox->checkState() != Qt::Unchecked );
    prefs->setAltInTermListShown( isAltInTermListShownChecked );

    if( keyboardAccelModified ) {
        for( int i = 0; i < keyboardAccelListView->topLevelItemCount(); i++ ) {
            KeyActionListViewItem* item = (KeyActionListViewItem*)keyboardAccelListView->topLevelItem( i );
            QAction *action = item->getAction();
            action->setShortcut( item->getKey() );
            prefs->setAccelerator( item->getActionIndex(), item->getKey() );
        }
    }

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

bool PreferencesDialog::isStudyLanguageSelectionValid() const {
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
    for( int i = 0; i < fontOverrideCount; i++ ) {
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
    int studyLanguageItemCount = studyLanguagesItem.count();
    for( int i = 0; i < studyLanguageItemCount; i++ ) {
        QCheckBox* languageCheckBox = (QCheckBox*)studyLanguagesItem.at( i );
        QString checkBoxLang( Util::getLanguageCode( languageCheckBox->text() ) );
        bool isChecked = ( languageCheckBox->checkState() != Qt::Unchecked );
        int fontOverrideItemCount = fontOverrideLabels.count();
        for( int j = 0; j < fontOverrideItemCount; j++ ) {
            if( checkBoxLang == Util::getLanguageCode( fontOverrideLabels.at( j )->text() ) )
                if( !isChecked )
                    removeFontOverride( checkBoxLang );
        }
        if( isChecked )
            addFontOverride( checkBoxLang );
    }
}

void PreferencesDialog::setAccelKey() {
    KeyActionListViewItem* item = (KeyActionListViewItem*)keyboardAccelListView->currentItem();
    if( item == NULL ) 
        return;
    item->setText( 1, tr( "<press key>" ) ); 
    grabAccelKeyFor = item;
    grabKeyboard();  
}

void PreferencesDialog::clearAccelKey() {
    cancelSetAccelKey();

    KeyActionListViewItem* item = (KeyActionListViewItem*)keyboardAccelListView->currentItem();
    if( item == NULL ) 
        return;
    item->setKey( 0 );
    keyboardAccelModified = true;
}

void PreferencesDialog::resetAccelKey() {
    cancelSetAccelKey();

    KeyActionListViewItem* item = (KeyActionListViewItem*)keyboardAccelListView->currentItem();
    if( item == NULL ) 
        return;
    item->setKey( prefs->getDefaultAccelerator( item->getActionIndex() ) );
    keyboardAccelModified = true;
}

void PreferencesDialog::cancelSetAccelKey() {
    if( grabAccelKeyFor != NULL ) {
        KeyActionListViewItem* ka = (KeyActionListViewItem*)grabAccelKeyFor;
        ka->updateText();
        grabAccelKeyFor = NULL;
        releaseKeyboard();
    }
}

void PreferencesDialog::keyPressEvent( QKeyEvent* evt ) {
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
