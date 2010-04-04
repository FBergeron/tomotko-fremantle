#include "Util.h"

const QString Util::languageCodeList[] = { 
    QObject::tr( "en" ), QObject::tr( "fr" ), QObject::tr( "es" ), QObject::tr( "ja" ), QObject::tr( "de" ), 
    QObject::tr( "ar" ), QObject::tr( "bg" ), QObject::tr( "cs" ), QObject::tr( "da" ), QObject::tr( "el" ), 
    QObject::tr( "eo" ), QObject::tr( "fi" ), QObject::tr( "he" ), QObject::tr( "hi" ), QObject::tr( "it" ),
    QObject::tr( "ko" ), QObject::tr( "la" ), QObject::tr( "nl" ), QObject::tr( "no" ), QObject::tr( "pt" ), QObject::tr( "rm" ),
    QObject::tr( "ro" ), QObject::tr( "ru" ), QObject::tr( "sv" ), QObject::tr( "sw" ), QObject::tr( "th" ), 
    QObject::tr( "tl" ), QObject::tr( "vi" ), QObject::tr( "zh" )
};

const QString Util::studyLanguageList[] = { 
    QString( "en" ), QString( "fr" ), QString( "es" ), QString( "ja" ), QString( "de" ), 
    QString( "ar" ), QString( "bg" ), QString( "cs" ), QString( "da" ), QString( "el" ), 
    QString( "eo" ), QString( "fi" ), QString( "he" ), QString( "hi" ), QString( "it" ), 
    QString( "ko" ), QString( "la" ), QString( "nl" ), QString( "no" ), QString( "pt" ), QString( "rm" ), 
    QString( "ro" ), QString( "ru" ), QString( "sv" ), QString( "sw" ), QString( "th" ), 
    QString( "tl" ), QString( "vi" ), QString( "zh" )
};

const QString Util::fontSizeNameList[] = { 
    QT_TRANSLATE_NOOP( "Util", "VerySmall" ), 
    QT_TRANSLATE_NOOP( "Util", "Small" ), 
    QT_TRANSLATE_NOOP( "Util", "Medium" ), 
    QT_TRANSLATE_NOOP( "Util", "Large" ), 
    QT_TRANSLATE_NOOP( "Util", "VeryLarge" ) 
};

QMap<QString,QString>* Util::digraphMap = 0;

QString Util::getLanguageCode( const QString& language ) {
    int languageCodeListLength = sizeof( languageCodeList ) / sizeof( QString );
    for( int i = 0; i < languageCodeListLength; i++ ) {
        if( QApplication::translate( "QObject", languageCodeList[ i ].toLatin1().data() ) == language ) 
            return( languageCodeList[ i ] );
    }
    return( QString::null ); // Should never happen.
}

QString Util::getDefaultLabelsFontFamily() {
    return( getDefaultFontFamily() );
}

int Util::getDefaultLabelsFontSizeModifier() {
    return( 0 );
}

QString Util::getDefaultFontFamily() {
    //QFontDatabase fontDatabase;
    //QStringList families = fontDatabase.families();
    //if( families.contains( "MS Sans Serif" ) )
    //    return( QString( "MS Sans Serif" ) ); // For Windows.
    //else
    //    return( qApp->font().defaultFamily() );
    return( qApp->font().defaultFamily() );
}

int Util::getDefaultFontSizeModifier() {
    return( 0 );
}

int Util::getDefaultQuizLength() {
    return( 1 );
}

QString Util::term2Xml( const Term& term, QStringList* languages, uint indentLevel /* = 0 */ ) {
    QString indent;
    for( uint i = 0; i < indentLevel; i++ )
        indent += QString( "\t" );

    QString xml;
    QTextStream ts( &xml, QIODevice::WriteOnly );
    ts << indent << QString( "<term id=\"" ) << QString::number( term.getId() ) << QString( "\"" );
    if( term.getImagePath() != QString::null ) {
        QString fileExtension = term.getImagePath().right( 4 );
        QString imageFilename = QString::number( term.getId() ) + fileExtension;
        ts << QString( " imagePath=\"" ) << imageFilename << QString( "\"" );
    }
    ts << QString( ">" ) << endl;
    for( Term::TranslationMap::ConstIterator it = term.translationsBegin(); it != term.translationsEnd(); it++ ) {
        const Translation& translation = *it;
        if( !languages || languages->contains( translation.getLanguage() ) ) {
            ts << indent << QString( "\t<trans lang=\"" ) << translation.getLanguage() << QString( "\">" ) << endl;
            if( !translation.getWord().isEmpty() )
                ts << indent << QString( "\t\t<word>" ) << Qt::escape( translation.getWord() ) << QString( "</word>" ) << endl;
            if( !translation.getAlt().isEmpty() )
                ts << indent << QString( "\t\t<alt>" ) << Qt::escape( translation.getAlt() ) << QString( "</alt>" ) << endl;
            ts << indent << QString( "\t</trans>" ) << endl;
        }
    }
    for( Term::CommentMap::ConstIterator it = term.commentsBegin(); it != term.commentsEnd(); it++ ) {
        const BilingualKey& key = it.key();
        const QString& comment = *it;
        if( !comment.isEmpty() ) {
            if( !languages || ( languages->contains( key.getFirstLanguage() ) && languages->contains( key.getSecondLanguage() ) ) )
                ts << indent << "\t<comment languages=\"" << key.toString() << "\">" << Qt::escape( comment ) << "</comment>" << endl;
        }
    }
    ts << indent << QString( "</term>" ) << endl;
    return( xml );
}

bool Util::deleteDirectory( const QString& dir ) {
    QDir d( dir );
    QDir absDir( d.absolutePath() );
    if( !absDir.exists() )
        return( false );

    QStringList entries = absDir.entryList();
    for( QStringList::Iterator it = entries.begin(); it != entries.end(); it++ ) {
        QString entry = *it;
        QString entryPath = absDir.path() + "/" + entry;
        QFileInfo info( entryPath );
        if( info.isDir() ) {
            if( entry != "." && entry != ".." ) {
                if( !deleteDirectory( entryPath ) )
                    return( false );
            }
        }
        else if( info.isFile() ) {
            QFile f( entryPath );
            if( !f.remove() )
                return( false );
        }
    }

    return( absDir.rmdir( d.absolutePath() ) );
}

QString Util::getDigraph( const QString& buffer ) {
    if( !digraphMap ) 
        initDigraphs();
    return( digraphMap->contains( buffer ) ? (*digraphMap)[ buffer ] : QString::null );
}

void Util::initDigraphs() {
    digraphMap = new QMap<QString, QString>();

    QString digraphFilename( ":/lib/toMOTko/digraphs.conf" );
    QFile digraphFile( digraphFilename );

    if( !digraphFile.open( QIODevice::ReadOnly ) ) {
        cerr << "Could not open " << qPrintable( digraphFilename ) << endl;
        return;
    }

    QTextStream ts( &digraphFile );
    while ( !ts.atEnd() ) {
        QString line = ts.readLine();
        if( line.startsWith( "#" ) || line.length() == 0 )
            continue;
        QString digraph = line.left( 2 );
        QString strValue = line.mid( 5, 2 );
        if( !digraph.isNull() && !strValue.isNull() ) {
            bool isOk;
            uint value = strValue.toUInt( &isOk, 16 );
            if( isOk )
                (*digraphMap)[ digraph ] = QString( QChar( value ) );
        }
    }

    digraphFile.close();
}

QAction* Util::createAction( const QString& name, const char** icon, QObject* receiver, const char* slot, QKeySequence defaultKey /*= QKeySequence()*/, bool toggle /*= false*/ ) {
    QAction *a;
    if( icon == NULL )
        a = new QAction( name, receiver );
    else
        a = new QAction( QIcon( ZPIXMAP( icon ) ), name, receiver );
    if( toggle ) {
        a->setCheckable( true );
        QObject::connect( a, SIGNAL( toggled( bool ) ), receiver, slot );
    }
    else
        QObject::connect( a, SIGNAL( triggered() ), receiver, slot );
    if( !defaultKey.isEmpty() )
        a->setShortcut( defaultKey );
    return a;
}

int Util::getStudyLanguagesCount() {
    return( sizeof( Util::studyLanguageList ) / sizeof( QString ) );
}

int Util::getFontSizesCount() {
    return( sizeof( Util::fontSizeNameList ) / sizeof( QString ) );
}
