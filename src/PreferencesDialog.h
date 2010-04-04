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
#include "DynamicHeightTreeWidget.h"
#include "FontsDialog.h"
#include "KeyActionListViewItem.h"
#include "KeyboardAcceleratorsDialog.h"
#include "StudyLanguagesDialog.h"
#include "Preferences.h"
#include "SequenceDialog.h"
#include "SequenceListItem.h"
#include "TermScheduler.h"
#include "Util.h"

extern QAction* action[ ACTION_COUNT ];

class PreferencesDialog : public QDialog {

    Q_OBJECT
    
public:

    static const QString interfaceLanguageList[];
    static const QString interfaceLanguageCodeList[];
    static const QString firstLanguageList[];

    PreferencesDialog( QWidget* parent, Preferences* prefs );
    ~PreferencesDialog(); 

public slots:

protected slots:

    virtual void accept();

protected:

    virtual void resizeEvent( QResizeEvent* evt );

private slots:

    void invokeKeyboardAcceleratorsDialog();
    void invokeStudyLanguagesDialog();
    void invokeFontsDialog();
    void addSequence();
    void removeSequence();
    void updateUi();

private:

    void init();
    void initSequences();
    void selectLanguage( QComboBox* comboBox, const QString& langCode );
    bool isRevealingSequenceSelectionValid() const;
    bool isRevealingSequenceDefined( const QString& seqStr ) const;

    Preferences*                prefs;

    QMap<QString,QString>       availableLanguages;

    QHBoxLayout*                mainLayout;

    QScrollArea*                bodyWrapper;
    QWidget*                    bodyPanel;
    QVBoxLayout*                bodyPanelLayout;

    QWidget*                    quizPage;
    QVBoxLayout*                quizPageLayout;
    QFrame*                     quizPageSeparator;
    QLabel*                     quizPageLabel;

    QWidget*                    interfacePage;
    QVBoxLayout*                interfacePageLayout;
    QFrame*                     interfacePageSeparator;

    QPushButton*                studyLanguagesButton;
    QPushButton*                fontsButton;

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
    DynamicHeightTreeWidget*    sequencesView;
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
   
    QPushButton*                keyboardAccelButton;

    QCheckBox*                  digraphCheckBox;

    QCheckBox*                  hideQuizButtonCheckBox;

    QCheckBox*                  showAltTextInTermListCheckBox;

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
