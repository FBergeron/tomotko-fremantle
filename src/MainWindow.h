#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <QActionGroup>
#include <QApplication>
#include <QCloseEvent>
#include <QComboBox>
#include <QDir>
#include <QKeySequence>
#include <QLabel>
#include <QMainWindow> 
#include <QMap>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QString>
#include <QStringList>
#include <QToolBar>
#include <QTranslator>
#include <QStackedWidget>
#include "Controller.h"
#include "HelpBrowser.h"
#include "PreferencesDialog.h"
#include "QuizFrame.h"
#include "VocabularyManagerFrame.h"

extern QAction* action[ ACTION_COUNT ];

class MainWindow : public QMainWindow {

    // Should be an enumeration.
    static const int frameQuizIndex         = 0;
    static const int frameVocabManagerIndex = 1;
    
    Q_OBJECT
    
public:

    MainWindow( QApplication& app, Controller* controller );
    ~MainWindow();

    QSize sizeHint() const;

    Controller* controller();

    bool isDigraphEnabled() const;

public slots:

    void updateMenus( QTreeWidgetItem* currItem );
    void updateFonts();
    void setDigraphEnabled( bool isEnabled );
    void retranslateUi();
    void switchLanguage( const QString& language );

protected:

    void closeEvent( QCloseEvent* );

private slots:

    void about();
    void help();
    void startQuiz();
    void showQuiz();
    void invokeVocabularyManager();
    void importData();
    void exportData();
    void preferences();
    void copy();
    void cut();
    void paste();
    void toggleLanguageFilter();
    void quit();
    void setFirstLanguage( const QString& lang );
    void setTestLanguage( const QString& lang );
    void switchFirstAndTestLanguages();
    void toggleMaximize( bool isOn ); 
    void search();
    
private:

    //void setLanguageFilterEnabled( bool isEnabled );

    QApplication&           app;
    Controller*             control;

    QStackedWidget*         mainPanel;

    QuizFrame*              quizFrame;
    VocabularyManagerFrame* vocabManagerFrame;

    QMenu*                  mainMenu;

    QAction*                copyAction;
    QAction*                cutAction;
    QAction*                pasteAction;

    QAction*                aboutAction;
    QAction*                helpAction;

    QTranslator*            translator;

};

#endif
