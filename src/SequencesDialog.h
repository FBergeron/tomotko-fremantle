#ifndef SEQUENCES_DIALOG_H
#define SEQUENCES_DIALOG_H

#include <QBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include "DynamicHeightTreeWidget.h"
#include "Preferences.h"
#include "SequenceDialog.h"
#include "SequenceListItem.h"

class SequencesDialog : public QDialog {

    Q_OBJECT
    
public:

    SequencesDialog( QWidget* parent, Preferences* prefs );
    ~SequencesDialog(); 

    int getSequenceCount() const;
    Sequence getSequenceAt( int index ) const;
    bool isSequenceEnabled( int index ) const;

public slots:

protected slots:

    virtual void accept();

protected:

    virtual void resizeEvent( QResizeEvent* evt );

private slots:

    void updateUi();
    void addSequence();
    void removeSequence();

private:

    void init();
    void initSequences();
    bool isSequenceSelectionValid() const;
    bool isRevealingSequenceDefined( const QString& seqStr ) const;

    Preferences*                prefs;

    QBoxLayout*                 mainLayout;

    QWidget*                    sequencesPanel;
    QHBoxLayout*                sequencesPanelLayout;

    QWidget*                    sequencesViewPanel;
    QBoxLayout*                 sequencesViewPanelLayout;
    DynamicHeightTreeWidget*    sequencesView;
    QWidget*                    sequencesViewButtons;
    QBoxLayout*                 sequencesViewButtonsLayout;
    QPushButton*                addSequenceButton;
    QPushButton*                removeSequenceButton;

    QWidget*                    sequencesLabelBox;
    QVBoxLayout*                sequencesLabelBoxLayout;
    QLabel*                     sequencesLabelHeader;
    QLabel*                     sequencesLabel;

    QWidget*                    bottomButtonsPanel;
    QVBoxLayout*                bottomButtonsPanelLayout;
    QPushButton*                acceptButton;
    QPushButton*                cancelButton;

};

#endif



