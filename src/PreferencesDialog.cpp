#include "PreferencesDialog.h"
#include "icons/flag_en.xpm"
#include "icons/flag_es.xpm"
#include "icons/flag_fr.xpm"
#include "icons/flag_ja.xpm"
#include "icons/flag_zh.xpm"
#include "icons/flag_de.xpm"

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
  
    quizLengthPanel = new QWidget();
    quizLengthPanelLayout = new QHBoxLayout();
    quizLengthPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    quizLengthPanel->setLayout( quizLengthPanelLayout );

    quizLengthLabel = new QLabel( tr( "Length" ) );

    quizLengthSliderPanel = new QWidget();
    quizLengthSliderPanelLayout = new QVBoxLayout();
    quizLengthSliderPanel->setLayout( quizLengthSliderPanelLayout );

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

    miscOptionsPanel = new QWidget();
    miscOptionsPanelLayout = new QGridLayout();
    miscOptionsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    miscOptionsPanel->setLayout( miscOptionsPanelLayout );

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

    fontsButton = new QPushButton( tr( "Fonts Settings" ) );
    connect( fontsButton, SIGNAL( clicked() ), this, SLOT( invokeFontsDialog() ) );

    sequencesButton = new QPushButton( tr( "Revealing Sequences Settings" ) );
    connect( sequencesButton, SIGNAL( clicked() ), this, SLOT( invokeSequencesDialog() ) );

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

    miscOptionsPanelLayout->addWidget( studyLanguagesButton, 0, 0 );
    miscOptionsPanelLayout->addWidget( digraphCheckBox, 0, 1 );
    miscOptionsPanelLayout->addWidget( fontsButton, 1, 0 );
    miscOptionsPanelLayout->addWidget( hideQuizButtonCheckBox, 1, 1 );
    miscOptionsPanelLayout->addWidget( keyboardAccelButton, 2, 0 );
    miscOptionsPanelLayout->addWidget( showAltTextInTermListCheckBox, 2, 1);
    miscOptionsPanelLayout->addWidget( sequencesButton, 3, 0 );

    bodyPanelLayout->addWidget( interfaceLanguagePanel );
    bodyPanelLayout->addWidget( quizLengthPanel );
    bodyPanelLayout->addWidget( miscOptionsPanel );
    bodyPanelLayout->addWidget( quizPage );

    bodyWrapper->setWidget( bodyPanel );

    mainLayout->addWidget( bodyWrapper  );
    mainLayout->addWidget( bottomButtonsPanel );

    setLayout( mainLayout );

    setWindowTitle( tr( "Preferences..." ) );
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
    prefs->setQuizLength( quizLengthSlider->value() );

    bool isDigraphEnabled = ( digraphCheckBox->checkState() != Qt::Unchecked );
    prefs->setDigraphEnabled( isDigraphEnabled );

    bool isQuizButtonHiddenChecked = ( hideQuizButtonCheckBox->checkState() != Qt::Unchecked );
    prefs->setQuizButtonsHidden( isQuizButtonHiddenChecked );

    bool isAltInTermListShownChecked = ( showAltTextInTermListCheckBox->checkState() != Qt::Unchecked );
    prefs->setAltInTermListShown( isAltInTermListShownChecked );

    QString interfaceLanguage = availableLanguages[ interfaceLanguageField->currentText() ];
    prefs->setInterfaceLanguage( interfaceLanguage );
    QDialog::accept();
}

void PreferencesDialog::resizeEvent( QResizeEvent* ) {
    bodyWrapper->widget()->resize( bodyWrapper->maximumViewportSize().width() - bodyWrapper->verticalScrollBar()->size().width() - 6, bodyPanel->size().height() );
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
    }
}

void PreferencesDialog::invokeFontsDialog() {
    FontsDialog dialog( this, prefs );
    dialog.show();
    int result = dialog.exec();
    if( result ) {
        prefs->setLabelsFontFamily( dialog.getLabelsFontFamily() );
        prefs->setLabelsFontSizeModifier( dialog.getLabelsFontSizeModifier() );

        prefs->setFontFamily( dialog.getFieldsFontFamily() );
        prefs->setFontSizeModifier( dialog.getFieldsFontSizeModifier() );

        prefs->clearFontOverrideFamilies();
        prefs->clearFontOverrideSizes();
        QStringList fontOverrideLanguages = dialog.getFontOverrideLanguages();
        int fontOverrideCount = fontOverrideLanguages.count(); 
        for( int i = 0; i < fontOverrideCount; i++ ) {
            const QString& language = fontOverrideLanguages.at( i );
            if( dialog.isFontOverrideFamilyDefined( language ) )
                prefs->setFontOverrideFamily( language, dialog.getFontOverrideFamily( language ) );
            if( dialog.isFontOverrideSizeModifierDefined( language ) ) 
                prefs->setFontOverrideSize( language, dialog.getFontOverrideSizeModifier( language ) );
        }
    }
}

void PreferencesDialog::invokeSequencesDialog() {
    SequencesDialog dialog( this, prefs );
    dialog.show();
    int result = dialog.exec();
    if( result ) {
        prefs->clearRevealingSequences();
        int seqCount = dialog.getSequenceCount();
        for( int i = 0; i < seqCount; i++ ) {
            Sequence seq = dialog.getSequenceAt( i );
            bool isEnabled = dialog.isSequenceEnabled( i );
            seq.setEnabled( isEnabled );
            prefs->addRevealingSequence( seq );
        }
    }
}
