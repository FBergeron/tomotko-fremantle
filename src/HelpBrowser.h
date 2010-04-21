#ifndef HELP_BROWSER_H
#define HELP_BROWSER_H

#include <iostream>
#include <QApplication>
#include <QDesktopServices>
#include <QBoxLayout>
#include <QPushButton>
#include <QTextBrowser>
#include <QWidget>

using namespace std;

class HelpBrowser : public QWidget {

    Q_OBJECT

public:

    HelpBrowser( const QString& path, QWidget* parent = 0 );

    void showPage( const QString& page );

private slots:

    void updateWindowTitle();
    void openLink( const QUrl& url );

private:
    
    QTextBrowser*   textBrowser;
    QPushButton*    homeButton;
    QPushButton*    backButton;

};

#endif
