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

    static const QString fontSizeNameList[];
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

    void resetDefaultLabelsFont();
    void resetDefaultFont();
    void addFontOverride( const QString& language );
    void removeFontOverride( const QString& language );
    void updateFontOverride();
    void invokeKeyboardAcceleratorsDialog();
    void invokeStudyLanguagesDialog();
    void addSequence();
    void removeSequence();
    void updateUi();

private:

    void init();
    void initFontFamilyValues( QComboBox* comboBox, bool withEmptyEntry = false ) const;
    void initFontSizeValues( QComboBox* comboBox, bool withEmptyEntry = false ) const;
    void initFontOverrides();
    void initSequences();
    void selectFontFamily( QComboBox* comboBox, const QString& fontFamily );
    void selectFontSize( QComboBox* comboBox, int fontSizeModifier, bool withEmptyEntry = false );
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

    QWidget*                    fontPage;
    QVBoxLayout*                fontPageLayout;
    QFrame*                     fontPageSeparator;
    QLabel*                     fontPageLabel;

    QWidget*                    interfacePage;
    QVBoxLayout*                interfacePageLayout;
    QFrame*                     interfacePageSeparator;
    QLabel*                     interfacePageLabel;

    QPushButton*                studyLanguagesButton;

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
