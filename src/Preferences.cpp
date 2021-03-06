#include "Preferences.h"

const qint32 Preferences::magicNumber = qint32( 0x77556644 );
const uint Preferences::fontSizeList[] = { 10, 12, 14, 18, 24, 36, 48, 64 };

Preferences::Preferences() 
    : quizLength( SHORT ), interfaceLanguage( QString( "en" ) ), digraphEnabled( false ), quizButtonsHidden( false ), altInTermListShown( false ),
        firstLanguage( QString( "en" ) ), testLanguage( QString( "ja" ) ),
            labelsFontFamily( Util::getDefaultLabelsFontFamily() ), labelsFontSizeModifier( Util::getDefaultLabelsFontSizeModifier() ),
                fontFamily( Util::getDefaultFontFamily() ), fontSizeModifier( Util::getDefaultFontSizeModifier() ),
                    languageFilterEnabled( true ) {
    initDefaultKeyboardAccelerators();

    sequences.clear();
    sequences.append( Sequence( "0+4-2-1+3:off" ) );
    sequences.append( Sequence( "0-2+4-1+3:on" ) );
    sequences.append( Sequence( "2+4-1-0+3:off" ) );

    studyLanguages.clear();
    studyLanguages.append( QString( "en" ) );
    studyLanguages.append( QString( "ja" ) );
}

Preferences::~Preferences() {
}

bool Preferences::load() {
    QFile prefsFile( prefsFilename );
    if( prefsFile.exists() ) {
        if( !prefsFile.open( QIODevice::ReadOnly ) ) {
            cerr << "Cannot open prefs data file: " << qPrintable( prefsFilename ) << endl;
            return( false );
        }

        QByteArray compressedData( prefsFile.readAll() );
        QByteArray data( qUncompress( compressedData ) );

        QDataStream in( data/*, QIODevice::ReadOnly*/ );

        qint32                  tempMagicNumber;
        qint16                  tempVersion;

        uint                    tempQuizLength;
        SequenceList            tempSequences;
        int                     tempLanguageFilterEnabledAsInt;
        QString                 tempInterfaceLanguage;
        int                     tempDigraphEnabledAsInt;
        int                     tempQuizButtonsHiddenAsInt;
        int                     tempAltInTermListShownAsInt;
        QString                 tempFirstLanguage;
        QString                 tempTestLanguage;
        QList<QString>          tempStudyLanguages;
        QList<int>              tempClosedFolders;
        QString                 tempLabelsFontFamily;
        int                     tempLabelsFontSizeModifier;
        QString                 tempFontFamily;
        int                     tempFontSizeModifier;
        QMap<QString,QString>   tempFontOverrideFamilies;
        QMap<QString,int>       tempFontOverrideSizes;
        QMap<int,int>           tempAccel;

        in >> tempMagicNumber >> tempVersion;

        if( tempMagicNumber != Preferences::magicNumber )
            cerr << "Wrong magic number: Incompatible data file for prefs file." << endl;
        if( tempVersion > 0x0011 )
            cerr << "Prefs data file is from a more recent version.  Upgrade toMOTko." << endl;

        in.setVersion( QDataStream::Qt_2_1 );
        in >> tempQuizLength >> tempSequences;
        in >> tempLanguageFilterEnabledAsInt >> tempInterfaceLanguage >> tempDigraphEnabledAsInt >> tempQuizButtonsHiddenAsInt >> tempAltInTermListShownAsInt;
        in >> tempFirstLanguage >> tempTestLanguage;
        in >> tempStudyLanguages;
        in >> tempClosedFolders; 
        in >> tempLabelsFontFamily >> tempLabelsFontSizeModifier >> tempFontFamily >> tempFontSizeModifier >> tempFontOverrideFamilies >> tempFontOverrideSizes;
        in >> tempAccel;

        prefsFile.close();
    
        quizLength = tempQuizLength;
        sequences = tempSequences;
        languageFilterEnabled = ( tempLanguageFilterEnabledAsInt == 1 );
        interfaceLanguage = tempInterfaceLanguage;
        digraphEnabled = ( tempDigraphEnabledAsInt == 1 );
        quizButtonsHidden = ( tempQuizButtonsHiddenAsInt == 1 );
        altInTermListShown = ( tempAltInTermListShownAsInt == 1 );
        firstLanguage = tempFirstLanguage;
        testLanguage = tempTestLanguage;
        studyLanguages = tempStudyLanguages;
        closedFolders = tempClosedFolders;
        labelsFontFamily = tempLabelsFontFamily;
        labelsFontSizeModifier = tempLabelsFontSizeModifier;
        fontFamily = tempFontFamily;
        fontSizeModifier = tempFontSizeModifier;
        fontOverrideFamilies = tempFontOverrideFamilies;
        fontOverrideSizes = tempFontOverrideSizes;
        accel = tempAccel;
    }
    return( true );
}

bool Preferences::save() {
    QByteArray data;

    QDataStream out( &data, QIODevice::WriteOnly );
    out.setVersion( QDataStream::Qt_2_1 );

    // 0x0011 means 0.11.x version.
    out << qint32( Preferences::magicNumber ) << qint16( 0x0011 );

    out << quizLength << sequences;
    int languageFilterEnabledAsInt = ( languageFilterEnabled ? 1 : 0 );
    int digraphEnabledAsInt = ( digraphEnabled ? 1 : 0 ); 
    int quizButtonsHiddenAsInt = ( quizButtonsHidden ? 1 : 0 );
    int altInTermListShownAsInt = ( altInTermListShown ? 1 : 0 );
    out << languageFilterEnabledAsInt << interfaceLanguage << digraphEnabledAsInt << quizButtonsHiddenAsInt << altInTermListShownAsInt;
    out << firstLanguage << testLanguage;
    out << studyLanguages;
    out << closedFolders;
    out << labelsFontFamily << labelsFontSizeModifier << fontFamily << fontSizeModifier << fontOverrideFamilies << fontOverrideSizes;
    out << accel;

    QByteArray compressedData( qCompress( data ) ); 

    QFile dataFile( prefsFilename );
    QFileInfo dataFileInfo( dataFile );

    QDir dataFileDir = dataFileInfo.absoluteDir();
    if( !dataFileDir.mkpath( dataFileDir.absolutePath() ) )
        return( false );

    if( !dataFile.open( QIODevice::WriteOnly ) )
        return( false );

    int ret = dataFile.write( compressedData );
    dataFile.close();

    if( ret == -1 || dataFile.error() != QFile::NoError ) {
        dataFile.unsetError();
        return( false );
    }

    return( true );
}

uint Preferences::getQuizLength() const {
    return( quizLength );
}

void Preferences::setQuizLength( uint quizLength ) {
    this->quizLength = quizLength;
}

void Preferences::clearRevealingSequences() {
    sequences.clear();
}

void Preferences::addRevealingSequence( Sequence sequence ) {
    sequences.append( sequence );
}

int Preferences::getRevealingSequence( const QString& seqStr ) {
    int i = 0; 
    for( SequenceList::ConstIterator it = sequences.begin(); it != sequences.end(); it++, i++ ) {
        Sequence seq = *it;
        if( seq.toHumanReadableString() == seqStr )
            return( i );
    }
    return( -1 );
}

void Preferences::removeRevealingSequence( int index ) {
    sequences.removeAt( index );
}

void Preferences::removeRevealingSequence( const QString& seqStr ) {
    for( int i = 0; i < sequences.size(); i++ ) {
        if( sequences[ i ].toHumanReadableString() == seqStr ) {
            sequences.removeAt( i );
            return;
        }
    }
}

int Preferences::getActiveRevealingSequenceCount() {
    int count = 0;
    for( SequenceList::Iterator it = sequences.begin(); it != sequences.end(); it++ ) {
        Sequence seq = *it;
        if( seq.isEnabled() )
            count++;
    }
    return( count );
}

int Preferences::getRevealingSequenceCount() const {
    return( sequences.count() );
}

Sequence Preferences::getRevealingSequenceAt( int index ) {
    return( sequences[ index ] );
}

QString Preferences::getInterfaceLanguage() const {
    return( interfaceLanguage );
}

void Preferences::setInterfaceLanguage( const QString& interfaceLanguage ) {
    this->interfaceLanguage = interfaceLanguage;
}

bool Preferences::isDigraphEnabled() const {
    return( digraphEnabled );
}

void Preferences::setDigraphEnabled( bool isEnabled ) {
    this->digraphEnabled = isEnabled;
}

bool Preferences::areQuizButtonsHidden() const {
    return( quizButtonsHidden );
}

void Preferences::setQuizButtonsHidden( bool areHidden ) {
    this->quizButtonsHidden = areHidden;
}

bool Preferences::isAltInTermListShown() const {
    return( altInTermListShown );
}

void Preferences::setAltInTermListShown( bool isShown ) {
    this->altInTermListShown = isShown;
}

QString Preferences::getFirstLanguage() const {
    return( firstLanguage );
}

void Preferences::setFirstLanguage( const QString& firstLanguage ) {
    this->firstLanguage = firstLanguage;
}

QList<QString> Preferences::getStudyLanguages() {
    return( studyLanguages );
}

bool Preferences::isStudyLanguage( const QString& language ) {
    return( studyLanguages.contains( language ) );
}

void Preferences::addStudyLanguage( const QString& language ) {
    studyLanguages.append( language );
}

void Preferences::removeStudyLanguage( const QString& language ) {
    int index = studyLanguages.indexOf( language );
    if( index != -1 )
        studyLanguages.removeAt( index );
}

void Preferences::clearStudyLanguages() {
    studyLanguages.clear();
}

QString Preferences::getTestLanguage() const {
    return( testLanguage );
}

void Preferences::setTestLanguage( const QString& testLanguage ) {
    this->testLanguage = testLanguage;
}

QString Preferences::getFontFamily() const {
    return( fontFamily );
}

void Preferences::setFontFamily( const QString& fontFamily ) {
    this->fontFamily = fontFamily;
}

QString Preferences::getDefaultFontFamily() const {
    return( Util::getDefaultFontFamily() );
}

int Preferences::getDefaultFontSizeModifier() const {
    return( Util::getDefaultFontSizeModifier() );
}

int Preferences::getFontSizeModifier() const {
    return( fontSizeModifier );
}

void Preferences::setFontSizeModifier( int fontSizeModifier ) {
    this->fontSizeModifier = fontSizeModifier;
}

QFont Preferences::getLargeFont( const QString& language /*= QString::null*/ ) const {
    QString family( isFontOverrideFamilyDefined( language ) ? getFontOverrideFamily( language ) : fontFamily );
    int sizeModif( isFontOverrideSizeDefined( language ) ? getFontOverrideSize( language ) : getFontSizeModifier() );
    return( getFont( family, fontSizeList[ 4 + sizeModif ] ) );
}

QFont Preferences::getMediumFont( const QString& language /*= QString::null*/ ) const {
    QString family( isFontOverrideFamilyDefined( language ) ? getFontOverrideFamily( language ) : fontFamily );
    int sizeModif( isFontOverrideSizeDefined( language ) ? getFontOverrideSize( language ) : getFontSizeModifier() );
    return( getFont( family, fontSizeList[ 3 + sizeModif ] ) );
}

QFont Preferences::getSmallFont( const QString& language /*= QString::null*/ ) const {
    QString family( isFontOverrideFamilyDefined( language ) ? getFontOverrideFamily( language ) : fontFamily );
    int sizeModif( isFontOverrideSizeDefined( language ) ? getFontOverrideSize( language ) : getFontSizeModifier() );
    return( getFont( family, fontSizeList[ 2 + sizeModif ] ) );
}

QFont Preferences::getBestFont( const QString& lang1, const QString& lang2 ) const {
    if( lang1 == QString( "en" ) )
        return( getMediumFont( lang2 ) );
    else if( lang2 == QString( "en" ) )
        return( getMediumFont( lang1 ) );
    else {
        QFont font1( getMediumFont( lang1 ) );
        QFont font2( getMediumFont( lang2 ) );
        if( font1.family() == font2.family() ) {
            int pointSize = font1.pointSize() > font2.pointSize() ? font1.pointSize() : font2.pointSize();
            return( QFont( font1.family(), pointSize ) );
        }
        else
            return( getMediumFont() );
    }
}

QString Preferences::getLabelsFontFamily() const {
    return( labelsFontFamily );
}

void Preferences::setLabelsFontFamily( const QString& labelsFontFamily ) {
    this->labelsFontFamily = labelsFontFamily;
}

QString Preferences::getDefaultLabelsFontFamily() const {
    return( Util::getDefaultLabelsFontFamily() );
}

int Preferences::getDefaultLabelsFontSizeModifier() const {
    return( Util::getDefaultLabelsFontSizeModifier() );
}

int Preferences::getLabelsFontSizeModifier() const {
    return( labelsFontSizeModifier );
}

void Preferences::setLabelsFontSizeModifier( int labelsFontSizeModifier ) {
    this->labelsFontSizeModifier = labelsFontSizeModifier;
}

QFont Preferences::getLabelsFont() const {
    return( getFont( labelsFontFamily, fontSizeList[ 3 + getLabelsFontSizeModifier() ] ) );
}

QString Preferences::getFontOverrideFamily( const QString& language ) const {
    return( fontOverrideFamilies[ language ] );
}

int Preferences::getFontOverrideSize( const QString& language ) const {
    return( fontOverrideSizes[ language ] );
}

void Preferences::setFontOverrideFamily( const QString& language, const QString& fontFamily ) {
    fontOverrideFamilies[ language ] = fontFamily;
}

void Preferences::setFontOverrideSize( const QString& language, int fontSize ) {
    fontOverrideSizes[ language ] = fontSize;
}

bool Preferences::isFontOverrideFamilyDefined( const QString& language ) const {
    return( fontOverrideFamilies.contains( language ) );
}

bool Preferences::isFontOverrideSizeDefined( const QString& language ) const {
    return( fontOverrideSizes.contains( language ) );
}

void Preferences::clearFontOverrideFamilies() {
    fontOverrideFamilies.clear();
}

void Preferences::clearFontOverrideSizes() {
    fontOverrideSizes.clear();
}

void Preferences::setLanguageFilterEnabled( bool isEnabled ) {
    languageFilterEnabled = isEnabled;
}

bool Preferences::isLanguageFilterEnabled() const {
    return( languageFilterEnabled );
}

bool Preferences::isFolderOpen( int folderId ) const {
    return( !closedFolders.contains( folderId ) );
}

void Preferences::setFolderOpen( int folderId, bool isOpen ) {
    if( isOpen ) {
        int index = closedFolders.indexOf( folderId );
        if( index != -1 )
            closedFolders.removeAt( index );
    }
    else {
        if( !closedFolders.contains( folderId ) ) 
            closedFolders.append( folderId );
    }
}

void Preferences::setApplicationDirName( const QString& applDir ) {
    prefsXmlFilename = applDir + QString( "/prefs.xml" );
    prefsFilename = applDir + QString( "/prefs.dat.z" );
}

int Preferences::getDefaultAccelerator( Action action ) {
    return( defaultAccel[ action ] );
}

void Preferences::setAccelerator( Action action, int newAccel ) {
    if( defaultAccel[ action ] == newAccel ) {
        if( accel.contains( action ) )
            accel.remove( action );
    }
    else
        accel[ action ] = newAccel;
}

int Preferences::getAccelerator( Action action ) {
    if( accel.contains( action ) )
        return( accel[ action ] );
    else
        return( getDefaultAccelerator( action ) );
}

void Preferences::initDefaultKeyboardAccelerators() {
    defaultAccel[ ACTION_REVEAL ] = Qt::Key_Space;
    defaultAccel[ ACTION_RIGHT_ANSWER ] = Qt::Key_Return;
    defaultAccel[ ACTION_WRONG_ANSWER ] = Qt::Key_Backspace;
    defaultAccel[ ACTION_EDIT_QUIZ_TERM ] = Qt::CTRL + Qt::Key_Return;
    defaultAccel[ ACTION_START_QUIZ ] = Qt::CTRL + Qt::Key_S;
    defaultAccel[ ACTION_MANAGE_GLOSSARIES ] = Qt::CTRL + Qt::Key_M;
    defaultAccel[ ACTION_IMPORT ] = 0;
    defaultAccel[ ACTION_EXPORT ] = 0;
    defaultAccel[ ACTION_SHOW_ALL_GLOSSARIES_AND_TERMS ] = 0;
    defaultAccel[ ACTION_PREFERENCES ] = Qt::CTRL + Qt::Key_P;
    defaultAccel[ ACTION_QUIT ] = Qt::CTRL + Qt::Key_Q;
    defaultAccel[ ACTION_ADD_FOLDER ] = 0; // Qt::CTRL + Qt::Key_F now triggers Search;
    defaultAccel[ ACTION_ADD_GLOSSARY ] = 0; // Qt::CTRL + Qt::Key_G is not set for this version.
    defaultAccel[ ACTION_REMOVE_ITEM ] = 0; // Qt::CTRL + Qt::Key_Backspace is not set for this version.
    defaultAccel[ ACTION_ADD_TERM ] = Qt::CTRL + Qt::Key_W;
    defaultAccel[ ACTION_EDIT_TERM ] = Qt::CTRL + Qt::Key_E;
    defaultAccel[ ACTION_REMOVE_TERMS ] = Qt::CTRL + Qt::Key_D;
    defaultAccel[ ACTION_CHECK_ALL_TERMS ] = Qt::CTRL + Qt::Key_A;
    defaultAccel[ ACTION_INVERSE_CHECKED_TERMS ] = Qt::CTRL + Qt::Key_I;
    defaultAccel[ ACTION_MAXIMIZE ] = 0; // Qt::CTRL + Qt::Key_Space;  This accelerator is the default accelerator to switch language input on Fremantle.
    defaultAccel[ ACTION_SEARCH ] = Qt::CTRL + Qt::Key_F; // Replaced from Qt::CTRL + Qt::Key_Comma.
}

QFont Preferences::getFont( const QString& fontFamily, uint size ) const {
    return( QFont( fontFamily, size, QFont::Normal, false ) );
}

