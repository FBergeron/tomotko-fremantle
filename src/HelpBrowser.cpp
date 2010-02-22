#include "HelpBrowser.h"

HelpBrowser::HelpBrowser( const QString& path, const QString& page, QWidget* parent /* = 0 */ ) : QWidget( parent ) {
    setAttribute( Qt::WA_DeleteOnClose );
    setAttribute( Qt::WA_GroupLeader );

    textBrowser = new QTextBrowser();
    textBrowser->setOpenLinks( false );
    QString stylesheet( "body { margin: 10px; }" );
    textBrowser->document()->setDefaultStyleSheet( stylesheet );
    homeButton = new QPushButton( tr( "Home" ) );
    backButton = new QPushButton( tr( "Back" ) );

    QVBoxLayout* buttonLayout = new QVBoxLayout();
    buttonLayout->addWidget( homeButton );
    buttonLayout->addStretch();
    buttonLayout->addWidget( backButton );

    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addWidget( textBrowser );
    mainLayout->addLayout( buttonLayout );
    setLayout( mainLayout );

    connect( homeButton, SIGNAL( clicked() ), textBrowser, SLOT( home() ) );
    connect( backButton, SIGNAL( clicked() ), textBrowser, SLOT( backward() ) );
    connect( textBrowser, SIGNAL( sourceChanged( const QUrl& ) ), this, SLOT( updateWindowTitle() ) );
    connect( textBrowser, SIGNAL( anchorClicked( const QUrl& ) ), this, SLOT( openLink( const QUrl& ) ) );

    textBrowser->setSearchPaths( QStringList() << path << ":/img" );
    textBrowser->setSource( page );
}

void HelpBrowser::updateWindowTitle() {
    setWindowTitle( tr( "Help: %1" ).arg( textBrowser->documentTitle() ) );
}

void HelpBrowser::openLink( const QUrl& url ) {
    if( url.toString().indexOf( "http://" ) == 0 )
        QDesktopServices::openUrl( url );
    else
        textBrowser->setSource( url );
}

void HelpBrowser::showPage( const QString& page ) {
    QString path = ":/help/en/html";
    HelpBrowser* browser = new HelpBrowser( path, page );
    browser->resize( 600, 600 );
#if defined(WINCE)
    browser->showMaximized();
#else
    browser->show();
#endif
}

