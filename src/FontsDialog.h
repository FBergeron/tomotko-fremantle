#ifndef FONTS_DIALOG_H
#define FONTS_DIALOG_H

#include <QBoxLayout>
#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QStringList>
#include "Preferences.h"
#include "Util.h"

class FontsDialog : public QDialog {

    Q_OBJECT
    
public:

    FontsDialog( QWidget* parent, Preferences* prefs );
    ~FontsDialog(); 

    QString getLabelsFontFamily() const;
    int getLabelsFontSizeModifier() const;

    QString getFieldsFontFamily() const;
    int getFieldsFontSizeModifier() const;

    QStringList getFontOverrideLanguages() const;
    bool isFontOverrideFamilyDefined( const QString& lang ) const;
    QString getFontOverrideFamily( const QString& lang ) const;
    bool isFontOverrideSizeModifierDefined( const QString& lang ) const;
    int getFontOverrideSizeModifier( const QString& lang ) const;

public slots:

protected slots:

    virtual void accept();

protected:

    virtual void resizeEvent( QResizeEvent* );

private slots:

    void resetDefaultLabelsFont();
    void resetDefaultFont();

private:

    void init();
    void initFontFamilyValues( QComboBox* comboBox, bool withEmptyEntry = false ) const;
    void initFontSizeValues( QComboBox* comboBox, bool withEmptyEntry = false ) const;
    void initFontOverrides();
    void selectFontFamily( QComboBox* comboBox, const QString& fontFamily );
    void selectFontSize( QComboBox* comboBox, int fontSizeModifier, bool withEmptyEntry = false );

    Preferences*            prefs;

    QString                 labelsFontFamily;
    int                     labelsFontSizeModifier;

    QString                 fieldsFontFamily;
    int                     fieldsFontSizeModifier;

    QMap<QString,QString>   overridesFontFamily;
    QMap<QString,int>       overridesFontSizeModifier;

    QBoxLayout*             mainLayout;

    QScrollArea*            fontsPanelWrapper;
    QWidget*                fontsPanel;
    QVBoxLayout*            fontsPanelLayout;

    QWidget*                labelsFontPanel;
    QHBoxLayout*            labelsFontPanelLayout;
    QLabel*                 labelsFontLabel;
    QComboBox*              labelsFontFamilyComboBox;
    QComboBox*              labelsFontSizeComboBox;
    QPushButton*            resetDefaultLabelsFontButton;

    QWidget*                fontPanel;
    QHBoxLayout*            fontPanelLayout;
    QLabel*                 fontLabel;
    QComboBox*              fontFamilyComboBox;
    QComboBox*              fontSizeComboBox;
    QPushButton*            resetDefaultFontButton;

    QLabel*                 fontOverridesLabel;

    QList<QWidget*>         fontOverrideBoxes;
    QList<QLabel*>          fontOverrideLabels;
    QList<QComboBox*>       fontOverrideFamilyComboBoxes;
    QList<QComboBox*>       fontOverrideSizeComboBoxes;

    QWidget*                bottomButtonsPanel;
    QBoxLayout*             bottomButtonsPanelLayout;
    QPushButton*            acceptButton;
    QPushButton*            cancelButton;

};

#endif
