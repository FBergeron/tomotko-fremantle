#ifndef TERM_DIALOG_H
#define TERM_DIALOG_H 

#include <QAction>
#include <QApplication>
#include <QDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QImageReader>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMovie>
#include <QPixmap>
#include <QPushButton>
#include <QResizeEvent>
#include <QScrollBar>
#include <QScrollArea>
#include <QString>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include "Controller.h"
#include "DigraphLineEdit.h"
#include "DigraphMultiLineEdit.h"
#include "PixMap.h"
#include "Term.h"
#include "Util.h"
#include "Vocabulary.h"

class TermDialog : public QDialog {

    Q_OBJECT
    
public:

    TermDialog( Vocabulary& vocab, Controller* controller, QWidget* parent );
    TermDialog( Vocabulary& vocab, Controller* controller, QWidget* parent, const Term& term );
    ~TermDialog(); 

    const Term& getTerm();
    virtual QSize sizeHint() const;

protected:

    void resizeEvent( QResizeEvent* evt );

private slots:
    
    void setImage();
    void clearImage();

private:

    void init();
    void updateFonts();
    void updateUi();
    void updateModel();

    void initImage( const QString& path );
    void resizeImageBox() const;

    QHBoxLayout*            mainLayout;

    QScrollArea*            bodyWrapper;

    QWidget*                body;
    QVBoxLayout*            bodyLayout;

    QWidget*                topPanel;
    QHBoxLayout*            topPanelLayout;
    QWidget*                topLeftPanel;
    QVBoxLayout*            topLeftPanelLayout;

    QWidget*                firstLangPanel;
    QBoxLayout*             firstLangPanelLayout;
    QLabel*                 firstLangPanelLabel;

    QWidget*                testLangPanel;
    QBoxLayout*             testLangPanelLayout;
    QLabel*                 testLangPanelLabel;

    QWidget*                firstLangTermPanel;
    QHBoxLayout*            firstLangTermPanelLayout;
    QLabel*                 firstLangTermLabel;
    DigraphLineEdit*        firstLangTermLineEdit;

    QWidget*                testLangTopPanel;
    QBoxLayout*             testLangTopPanelLayout;
    QWidget*                testLangLabelsPanel;
    QVBoxLayout*            testLangLabelsPanelLayout;
    QWidget*                testLangFieldsPanel;
    QVBoxLayout*            testLangFieldsPanelLayout;
    
    QLabel*                 testLangTermLabel;
    DigraphLineEdit*        testLangTermLineEdit;

    QLabel*                 testLangTermAltLabel;
    DigraphLineEdit*        testLangTermAltLineEdit;
    
    QWidget*                commentBox;
    QVBoxLayout*            commentBoxLayout;
    QLabel*                 commentLabel;
    DigraphMultiLineEdit*   commentMultiLineEdit;

    QWidget*                imageBox;
    QBoxLayout*             imageBoxLayout;
    QLabel*                 imageBoxLabel;
    QLabel*                 image;
    QWidget*                imageButtonsPanel;
    QHBoxLayout*            imageButtonsPanelLayout;
    QPushButton*            setImageButton;
    QPushButton*            clearImageButton;

    QWidget*                bottomButtonsPanel;
    QVBoxLayout*            bottomButtonsPanelLayout;
    QPushButton*            acceptButton;
    QPushButton*            cancelButton;

    Vocabulary&             vocab;
    Controller*             controller;
    Term*                   editedTerm;

    QAction*                cutAction;
    QAction*                copyAction;
    QAction*                pasteAction;

    QString                 tempImagePath; // Contains absolute path.
    QPixmap*                pixmap;
    QMovie*                 movie;

};

#endif
