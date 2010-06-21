#include "TermDialog.h"

TermDialog::TermDialog( Vocabulary& vocab, Controller* controller, QWidget* parent ) 
    : QDialog( parent ), vocab( vocab ), controller( controller ), editedTerm( new Term( vocab.getMaxTermId() + 1, vocab.getId() ) ), pixmap( NULL ), movie( NULL ) {
    init();
}

TermDialog::TermDialog( Vocabulary& vocab, Controller* controller, QWidget* parent, const Term& term ) 
    : QDialog( parent ), vocab( vocab ), controller( controller ), editedTerm( new Term( term ) ), pixmap( NULL ), movie( NULL ) {
    init();
}

void TermDialog::init() {
    setModal( false );

    Preferences& prefs = controller->getPreferences();

    QString firstLang( prefs.getFirstLanguage() );
    QString testLang( prefs.getTestLanguage() );
    bool isDigraphEnabled( prefs.isDigraphEnabled() );

    mainLayout = new QHBoxLayout();
    setLayout( mainLayout );

    body = new QWidget();
    bodyLayout = new QVBoxLayout();
    bodyLayout->setContentsMargins( 0, 0, 0, 0 );
    body->setLayout( bodyLayout );

    topPanel = new QWidget();
    topPanelLayout = new QHBoxLayout();
    topPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    topPanel->setLayout( topPanelLayout );

    topLeftPanel = new QWidget();
    topLeftPanel->setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Maximum ) ); 
    topLeftPanelLayout = new QVBoxLayout();
    topLeftPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    topLeftPanel->setLayout( topLeftPanelLayout );
    topPanelLayout->addWidget( topLeftPanel, 1 );

    firstLangPanel = new QWidget();
    firstLangPanelLayout = new QBoxLayout( QBoxLayout::TopToBottom );
    firstLangPanelLayout->setContentsMargins( 0, 0, 0, 0 ); 
    firstLangPanel->setLayout( firstLangPanelLayout );
    topLeftPanelLayout->addWidget( firstLangPanel );

    testLangPanel = new QWidget();
    testLangPanelLayout = new QBoxLayout( QBoxLayout::TopToBottom );
    testLangPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    testLangPanel->setLayout( testLangPanelLayout );
    topLeftPanelLayout->addWidget( testLangPanel );

    firstLangTermPanel = new QWidget();
    firstLangTermPanelLayout = new QHBoxLayout();
    firstLangTermPanel->setLayout( firstLangTermPanelLayout );

    firstLangPanelLabel = new QLabel( QApplication::translate( "QObject", firstLang.toLatin1().data() ) );
    firstLangPanelLayout->addWidget( firstLangPanelLabel );
    firstLangPanelLayout->addWidget( firstLangTermPanel );

    firstLangTermLabel = new QLabel( tr( "Word/Expr." ) );
    firstLangTermPanelLayout->addWidget( firstLangTermLabel );
    firstLangTermLineEdit = new DigraphLineEdit();
    firstLangTermPanelLayout->addWidget( firstLangTermLineEdit );
    firstLangTermLineEdit->setFont( prefs.getMediumFont( firstLang ) );
    firstLangTermLineEdit->setDigraphEnabled( isDigraphEnabled );

    testLangTopPanel = new QWidget();
    testLangTopPanelLayout = new QHBoxLayout();
    testLangTopPanel->setLayout( testLangTopPanelLayout );

    testLangPanelLabel = new QLabel( QApplication::translate( "QObject", testLang.toLatin1().data() ));
    testLangPanelLayout->addWidget( testLangPanelLabel );
    testLangPanelLayout->addWidget( testLangTopPanel );

    testLangLabelsPanel = new QWidget();
    testLangLabelsPanelLayout = new QVBoxLayout();
    testLangLabelsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    testLangLabelsPanel->setLayout( testLangLabelsPanelLayout );

    testLangTermAltLabel = new QLabel( tr( "Alt./Phon." ) );
    testLangLabelsPanelLayout->addWidget( testLangTermAltLabel );
    testLangTermLabel = new QLabel( tr( "Word/Expr." ) );
    testLangLabelsPanelLayout->addWidget( testLangTermLabel );

    testLangFieldsPanel = new QWidget();
    testLangFieldsPanelLayout = new QVBoxLayout();
    testLangFieldsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    testLangFieldsPanel->setLayout( testLangFieldsPanelLayout );

    testLangTermAltLineEdit = new DigraphLineEdit();
    testLangFieldsPanelLayout->addWidget( testLangTermAltLineEdit );
    testLangTermAltLineEdit->setFont( prefs.getMediumFont( testLang ) );
    testLangTermAltLineEdit->setDigraphEnabled( isDigraphEnabled );
    testLangTermLineEdit = new DigraphLineEdit();
    testLangFieldsPanelLayout->addWidget( testLangTermLineEdit );
    testLangTermLineEdit->setFont( prefs.getLargeFont( testLang ) );
    testLangTermLineEdit->setDigraphEnabled( isDigraphEnabled );
   
    testLangTopPanelLayout->addWidget( testLangLabelsPanel );
    testLangTopPanelLayout->addWidget( testLangFieldsPanel, 1 );

    commentBox = new QWidget();
    commentBoxLayout = new QVBoxLayout();
    commentBoxLayout->setContentsMargins( 0, 0, 0, 0 );
    commentBox->setLayout( commentBoxLayout );

    commentLabel = new QLabel( tr( "Examples/Comments" ) );
    commentMultiLineEdit = new DigraphMultiLineEdit();
    commentMultiLineEdit->setFont( prefs.getBestFont( firstLang, testLang ) );
    commentMultiLineEdit->setDigraphEnabled( isDigraphEnabled );
    commentBoxLayout->addWidget( commentLabel );
    commentBoxLayout->addWidget( commentMultiLineEdit, 1 );

    imageBox = new QWidget();
    imageBoxLayout = new QBoxLayout( QBoxLayout::TopToBottom );
    imageBox->setLayout( imageBoxLayout );
    topPanelLayout->addWidget( imageBox );

    image = new QLabel();
    image->setAlignment( Qt::AlignCenter );

    imageButtonsPanel = new QWidget();
    imageButtonsPanelLayout = new QHBoxLayout();
    imageButtonsPanelLayout->setContentsMargins( 0, 0, 0, 0 );
    imageButtonsPanel->setLayout( imageButtonsPanelLayout );
    setImageButton = new QPushButton( tr( "setImage" ) );
    imageButtonsPanelLayout->addWidget( setImageButton );
    //setImageButton->setToolTip( tr( "setImageTooltip" ) );
    connect( setImageButton, SIGNAL( clicked() ), this, SLOT( setImage() ) );
    clearImageButton = new QPushButton( tr( "clearImage" ) );
    imageButtonsPanelLayout->addWidget( clearImageButton );
    //clearImageButton->setToolTip( tr( "clearImageTooltip" ) );
    connect( clearImageButton, SIGNAL( clicked() ), this, SLOT( clearImage() ) );

    imageBoxLabel = new QLabel( tr( "Image" ) );
    imageBoxLayout->addWidget( imageBoxLabel );

    imageBoxLayout->addWidget( image, 1 );
    imageBoxLayout->addWidget( imageButtonsPanel );

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

    bodyLayout->addWidget( topPanel );
    bodyLayout->addWidget( commentBox, 1 );

    bodyWrapper = new QScrollArea();
    bodyWrapper->setWidget( body );

    mainLayout->addWidget( bodyWrapper );
    mainLayout->addWidget( bottomButtonsPanel );
    mainLayout->activate();

    setWindowTitle( tr( "EditTerm" ) );

    updateFonts();

    updateUi();
}

TermDialog::~TermDialog() {
    if( pixmap ) {
        delete( pixmap );
        pixmap = NULL;
    }
    if( movie ) {
        delete( movie );
        movie = NULL;
    }
}

void TermDialog::updateModel() {
    if( !editedTerm->isTranslationExists( controller->getPreferences().getFirstLanguage() ) )
        editedTerm->addTranslation( controller->getPreferences().getFirstLanguage() );
    Translation& firstLangTranslation = editedTerm->getTranslation( controller->getPreferences().getFirstLanguage() );
    firstLangTranslation.setWord( firstLangTermLineEdit->text() );

    if( !editedTerm->isTranslationExists( controller->getPreferences().getTestLanguage() ) )
        editedTerm->addTranslation( controller->getPreferences().getTestLanguage() );

    Translation& testLangTranslation = editedTerm->getTranslation( controller->getPreferences().getTestLanguage() );
    testLangTranslation.setWord( testLangTermLineEdit->text() );
    testLangTranslation.setAlt( testLangTermAltLineEdit->text() );

    BilingualKey commentKey( controller->getPreferences().getFirstLanguage(), controller->getPreferences().getTestLanguage() );
    editedTerm->addComment( commentKey, commentMultiLineEdit->toPlainText() );

    // If the path refers to an image in toMOTko's vocabulary, we use a relative path instead.
    QString vocabLocation = controller->getApplicationDirName() + "/" + vocab.getParent()->getPath() +
        "/v-" + QString::number( vocab.getId() ) + "/";
    QString imagePath = tempImagePath.left( vocabLocation.length() ) == vocabLocation ? 
            tempImagePath.right( tempImagePath.length() - vocabLocation.length() ) : tempImagePath;
    editedTerm->setImagePath( imagePath );
}

void TermDialog::setImage() {
    QDir dir = QDir::home();
    if( !tempImagePath.isEmpty() )
        dir = QFileInfo( tempImagePath ).dir();

    QString imageFile = QFileDialog::getOpenFileName( this, tr( "SetImage..." ), dir.path(), tr( "Images (*.png *.gif)" ) );
    if( !imageFile.isEmpty() ) {
        initImage( imageFile );
        resizeImageBox();
    }
}

void TermDialog::clearImage() {
    image->clear();
    tempImagePath = QString::null;
    if( pixmap ) {
        delete( pixmap );
        pixmap = NULL;
    }
    if( movie ) {
        delete( movie );
        movie = NULL;
    }
}

void TermDialog::initImage( const QString& path ) {
    clearImage();
    if( !path.isNull() ) {
        QFileInfo info( path );
        if( info.exists() ) {
            QByteArray imageFormat = QImageReader::imageFormat( path );
            if( imageFormat == "gif" || imageFormat == "png" ) {
                tempImagePath = path;
                // Even for animated gif, we create a pixmap.  
                // It will be used to determine the size of the movie.
                pixmap = new QPixmap( path );
                if( pixmap && pixmap->isNull() ) {
                    QMessageBox::warning( this, QObject::tr( "Error" ), tr( "CannotReadImage" ) );
                    pixmap = NULL;
                    return;
                }
                if( imageFormat == "gif" )
                    movie = new QMovie( path );
            }
        }
    }
}

void TermDialog::resizeEvent( QResizeEvent* /*evt*/ ) {
    resizeImageBox();
    bodyWrapper->widget()->resize( bodyWrapper->maximumViewportSize().width() - bodyWrapper->verticalScrollBar()->size().width() - 6, body->size().height() );
}

void TermDialog::resizeImageBox() const {
    imageBox->setMaximumHeight( topLeftPanel->sizeHint().height() );
    imageBoxLayout->activate();
    if( movie || pixmap ) {
        int left, top, right, bottom;
        imageBoxLayout->getContentsMargins( &left, &top, &right, &bottom );
        int imageHeight = imageBoxLayout->contentsRect().height() - top - bottom - imageButtonsPanel->sizeHint().height();
        if( movie ) {
            // Use the pixmap to compute the scaled size.
            int proportionalWidth = ( imageHeight - 20 ) * pixmap->width() / pixmap->height(); 
            movie->setScaledSize( QSize( proportionalWidth, imageHeight ) ); 
            image->setMovie( movie );
            movie->start();
        }
        else if( pixmap ) {
            QPixmap scaledPixmap( pixmap->scaledToHeight( imageHeight - 20, Qt::SmoothTransformation ) );
            image->setPixmap( scaledPixmap );
        }
    }
}

const Term& TermDialog::getTerm() {
    updateModel();
    return( *editedTerm );
}

QSize TermDialog::sizeHint() const {
    return( QSize( 640, 480 ) );
}

void TermDialog::updateFonts() {
    QFont largeFont( controller->getPreferences().getLargeFont() );
    QFont mediumFont( controller->getPreferences().getMediumFont() );
    QFont labelsFont( controller->getPreferences().getLabelsFont() );
    QString firstLang( controller->getQuizFirstLanguage() );
    QString testLang( controller->getQuizTestLanguage() );

    firstLangTermLabel->setFont( labelsFont );
    firstLangTermLineEdit->setFont( controller->getPreferences().getMediumFont( firstLang ) );
    firstLangPanel->setFont( labelsFont );

    testLangTermAltLabel->setFont( labelsFont );
    testLangTermAltLineEdit->setFont( controller->getPreferences().getMediumFont( testLang ) );
    testLangTermLabel->setFont( labelsFont );

    testLangTermLineEdit->setFont( controller->getPreferences().getLargeFont( testLang ) );

    testLangPanel->setFont( labelsFont );

    imageBox->setFont( labelsFont );
    setImageButton->setFont( labelsFont );
    clearImageButton->setFont( labelsFont );

    acceptButton->setFont( labelsFont );
    cancelButton->setFont( labelsFont );

    commentLabel->setFont( labelsFont );
    commentMultiLineEdit->setFont( controller->getPreferences().getBestFont( firstLang, testLang ) );

    firstLangTermPanel->updateGeometry();
    firstLangTermPanel->layout()->invalidate();

    testLangLabelsPanel->updateGeometry();
    testLangLabelsPanel->layout()->invalidate();

    testLangFieldsPanel->updateGeometry();
    testLangFieldsPanel->layout()->invalidate();

    update();
    updateGeometry();
}

void TermDialog::updateUi() {
    if( editedTerm ) {
        if( editedTerm->isTranslationExists( controller->getPreferences().getFirstLanguage() ) ) {
            Translation& firstLangTranslation = editedTerm->getTranslation( controller->getPreferences().getFirstLanguage() );
            firstLangTermLineEdit->setText( firstLangTranslation.getWord() );
            firstLangTermLineEdit->setCursorPosition( 0 );
        }

        if( editedTerm->isTranslationExists( controller->getPreferences().getTestLanguage() ) ) {
            Translation& testLangTranslation = editedTerm->getTranslation( controller->getPreferences().getTestLanguage() );
            testLangTermLineEdit->setText( testLangTranslation.getWord() );
            testLangTermLineEdit->setCursorPosition( 0 );
            testLangTermAltLineEdit->setText( testLangTranslation.getAlt() );
            testLangTermAltLineEdit->setCursorPosition( 0 );
            BilingualKey commentKey( controller->getPreferences().getFirstLanguage(), controller->getPreferences().getTestLanguage() );
            if( editedTerm->isCommentExists( commentKey ) )
                commentMultiLineEdit->setText( editedTerm->getComment( commentKey ) );
        }

        QString absPath = controller->getResolvedImagePath( editedTerm->getImagePath(), vocab );
        initImage( absPath );
    }
}
