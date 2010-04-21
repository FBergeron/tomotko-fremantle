#include "HelpBrowser.h"

HelpBrowser::HelpBrowser( const QString& path, QWidget* parent /* = 0 */ ) : QWidget( parent ) {
    textBrowser = new QTextBrowser();
    textBrowser->setOpenLinks( false );
    QString stylesheet( "body { margin: 10px; }" );
    textBrowser->document()->setDefaultStyleSheet( stylesheet );
    homeButton = new QPushButton( tr( "Home" ) );
    backButton = new QPushButton( tr( "Back" ) );

    QBoxLayout* buttonLayout = new QBoxLayout( QBoxLayout::TopToBottom );
    buttonLayout->addWidget( homeButton );
    buttonLayout->addStretch();
    buttonLayout->addWidget( backButton );

    QBoxLayout* mainLayout = new QBoxLayout( QHBoxLayout::LeftToRight );
    mainLayout->addWidget( textBrowser );
    mainLayout->addLayout( buttonLayout );
    setLayout( mainLayout );

    connect( homeButton, SIGNAL( clicked() ), textBrowser, SLOT( home() ) );
    connect( backButton, SIGNAL( clicked() ), textBrowser, SLOT( backward() ) );
    connect( textBrowser, SIGNAL( sourceChanged( const QUrl& ) ), this, SLOT( updateWindowTitle() ) );
    connect( textBrowser, SIGNAL( anchorClicked( const QUrl& ) ), this, SLOT( openLink( const QUrl& ) ) );

    textBrowser->setSearchPaths( QStringList() << path << ":/img" );
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
    textBrowser->setSource( page );
    show();
}

