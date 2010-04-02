#ifndef STUDY_LANGUAGES_DIALOG_H
#define STUDY_LANGUAGES_DIALOG_H

#include <QBoxLayout>
#include <QCheckBox>
#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollBar>
#include <QScrollArea>
#include <QStringList>
#include "Preferences.h"
#include "Util.h"

class StudyLanguagesDialog : public QDialog {

    Q_OBJECT
    
public:

    StudyLanguagesDialog( QWidget* parent, Preferences* prefs );
    ~StudyLanguagesDialog(); 

    QStringList getStudyLanguages() const;

public slots:

protected slots:

    virtual void accept();

protected:

    virtual void resizeEvent( QResizeEvent* evt );

private slots:

    void updateUi();

private:

    void init();
    void initStudyLanguageValues();
    bool isStudyLanguageSelectionValid() const;

    Preferences*        prefs;

    QBoxLayout*         mainLayout;

    QScrollArea*        languagesPanelWrapper;
    QWidget*            languagesPanel;
    QGridLayout*        languagesPanelLayout;

    QList<QCheckBox*>   studyLanguagesItem;

    QWidget*            bottomButtonsPanel;
    QVBoxLayout*        bottomButtonsPanelLayout;
    QPushButton*        acceptButton;
    QPushButton*        cancelButton;

};

#endif


