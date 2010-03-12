/*
 * This class contains some code borrowed from 
 * keyboard_shortcuts.h (rev. 1.2) and
 * keyboard_shortcuts.cpp (rev. 1.3) of ZBEDic.
 * Copyright (C) 2004 Rafal Mantiuk <rafm@users.sourceforge.org>
 */
#ifndef PREFERENCES_DIALOG_H
#define PREFERENCES_DIALOG_H 

#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QList>
#include <QMessageBox>
#include <QPushButton>
#include <QResizeEvent>
#include <QScrollArea>
#include <QSlider>
#include <QString>
#include <QTabWidget>
#include <QTranslator>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include "KeyActionListViewItem.h"
#include "Preferences.h"
#include "SequenceDialog.h"
#include "SequenceListItem.h"
#include "TermScheduler.h"
#include "Util.h"

extern QAction* action[ ACTION_COUNT ];

class PreferencesDialog : public QDialog {

    Q_OBJECT
    
public:

    static const QString fontSizeNameList[];
    static const QString interfaceLanguageList[];
    static const QString interfaceLanguageCodeList[];
    static const QString firstLanguageList[];
    static const QString studyLanguageList[];
    static const QString studyLanguageCodeList[];

    PreferencesDialog( QWidget* parent, Preferences* prefs );
    ~PreferencesDialog(); 

public slots:

protected slots:

    virtual void accept();

protected:

    virtual void resizeEvent( QResizeEvent* evt );

private slots:

    void resetDefaultLabelsFont();
    void resetDefaultFont();
    void addFontOverride( const QString& language );
    void removeFontOverride( const QString& language );
    void updateFontOverride();
    void clearAccelKey();
    void setAccelKey();
    void resetAccelKey();
    void cancelSetAccelKey();
    void addSequence();
    void removeSequence();
    void updateUi();

private:

    void init();
    void initFontFamilyValues( QComboBox* comboBox, bool withEmptyEntry = false ) const;
    void initFontSizeValues( QComboBox* comboBox, bool withEmptyEntry = false ) const;
    void initFontOverrides();
    void initSequences();
    void initStudyLanguageValues();
    void selectFontFamily( QComboBox* comboBox, const QString& fontFamily );
    void selectFontSize( QComboBox* comboBox, int fontSizeModifier, bool withEmptyEntry = false );
    void selectLanguage( QComboBox* comboBox, const QString& langCode );
    bool isRevealingSequenceSelectionValid() const;
    bool isRevealingSequenceDefined( const QString& seqStr ) const;
    bool isStudyLanguageSelectionValid() const;
    void keyPressEvent( QKeyEvent *evt );

    Preferences*                prefs;

    QMap<QString,QString>       availableLanguages;

    QHBoxLayout*                mainLayout;

    QScrollArea*                bodyWrapper;
    QWidget*                    bodyPanel;
    QVBoxLayout*                bodyPanelLayout;

    QWidget*                    languagePage;
    QVBoxLayout*                languagePageLayout;
    QFrame*                     languagePageSeparator;
    QLabel*                     languagePageLabel;

    QWidget*                    quizPage;
    QVBoxLayout*                quizPageLayout;
    QFrame*                     quizPageSeparator;
    QLabel*                     quizPageLabel;

    QWidget*                    fontPage;
    QVBoxLayout*                fontPageLayout;
    QFrame*                     fontPageSeparator;
    QLabel*                     fontPageLabel;

    QWidget*                    interfacePage;
    QVBoxLayout*                interfacePageLayout;
    QFrame*                     interfacePageSeparator;
    QLabel*                     interfacePageLabel;

    QWidget*                    languagesPanel;
    QGridLayout*                languagesPanelLayout;

    QList<QCheckBox*>           studyLanguagesItem;

    QWidget*                    quizLengthPanel;
    QHBoxLayout*                quizLengthPanelLayout;

    QLabel*                     quizLengthLabel;

    QWidget*                    quizLengthSliderPanel;
    QVBoxLayout*                quizLengthSliderPanelLayout;

    QLabel*                     revealingOptionsLabel;

    QWidget*                    revealingOptionsPanel;
    QVBoxLayout*                revealingOptionsPanelLayout;

    QWidget*                    sequencesPanel;
    QHBoxLayout*                sequencesPanelLayout;

    QWidget*                    sequencesViewPanel;
    QVBoxLayout*                sequencesViewPanelLayout;
    QTreeWidget*                sequencesView;
    QWidget*                    sequencesViewButtons;
    QHBoxLayout*                sequencesViewButtonsLayout;
    QPushButton*                addSequenceButton;
    QPushButton*                removeSequenceButton;

    QWidget*                    sequencesLabelBox;
    QVBoxLayout*                sequencesLabelBoxLayout;
    QLabel*                     sequencesLabelHeader;
    QLabel*                     sequencesLabel;

    QSlider*                    quizLengthSlider;
    QWidget*                    quizLengthLabelsPanel;
    QHBoxLayout*                quizLengthLabelsPanelLayout;
    QLabel*                     quizLengthShortestLabel;
    QLabel*                     quizLengthMediumLabel;
    QLabel*                     quizLengthLongestLabel;
   
    QHBoxLayout*                keyboardAccelButtonPanelLayout;

    QWidget*                    labelsFontPanel;
    QHBoxLayout*                labelsFontPanelLayout;
    QLabel*                     labelsFontLabel;
    QComboBox*                  labelsFontFamilyComboBox;
    QComboBox*                  labelsFontSizeComboBox;
    QPushButton*                resetDefaultLabelsFontButton;

    QWidget*                    fontsPanel;
    QVBoxLayout*                fontsPanelLayout;

    QWidget*                    fontPanel;
    QHBoxLayout*                fontPanelLayout;
    QLabel*                     fontLabel;
    QComboBox*                  fontFamilyComboBox;
    QComboBox*                  fontSizeComboBox;
    QPushButton*                resetDefaultFontButton;

    QWidget*                    fontOverridesPanel;
    QVBoxLayout*                fontOverridesPanelLayout;

    QLabel*                     fontOverridesLabel;

    QList<QWidget*>             fontOverrideBoxes;
    QList<QLabel*>              fontOverrideLabels;
    QList<QComboBox*>           fontOverrideFamilyComboBoxes;
    QList<QComboBox*>           fontOverrideSizeComboBoxes;

    QCheckBox*                  digraphCheckBox;

    QCheckBox*                  hideQuizButtonCheckBox;

    QCheckBox*                  showAltTextInTermListCheckBox;

    QWidget*                    keyboardAccelPanel;
    QVBoxLayout*                keyboardAccelPanelLayout;

    QLabel*                     keyboardAccelLabel;
    QTreeWidgetItem*            grabAccelKeyFor;
    QTreeWidget*                keyboardAccelListView;
    bool                        keyboardAccelModified;

    QWidget*                    keyboardAccelButtonPanel;
    QPushButton*                clearAccelKeyButton;
    QPushButton*                setAccelKeyButton;
    QPushButton*                resetAccelKeyButton;

    QWidget*                    interfaceLanguagePanel;
    QHBoxLayout*                interfaceLanguagePanelLayout;
    QLabel*                     interfaceLanguageLabel;
    QComboBox*                  interfaceLanguageField;

    QWidget*                    bottomButtonsPanel;
    QVBoxLayout*                bottomButtonsPanelLayout;
    QPushButton*                acceptButton;
    QPushButton*                cancelButton;

};

#endif
