#ifndef SEQUENCE_DIALOG_H
#define SEQUENCE_DIALOG_H 

#include <QBoxLayout>
#include <QDialog>
#include <QStack>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QStyle>
#include <QVBoxLayout>
#include <QWidget>
#include "Preferences.h"
#include "Sequence.h"

class SequenceDialog : public QDialog {

    Q_OBJECT
    
public:

    SequenceDialog( Preferences* prefs, QWidget* parent );
    ~SequenceDialog(); 

    Sequence getSequence() const;

public slots:

protected slots:

    virtual void accept();

protected:

    virtual void resizeEvent( QResizeEvent* );

private slots:

    virtual bool eventFilter( QObject* obj, QEvent* evt );

    void addSequenceItem( const QString& itemStr );
    void addSequenceMark();
    void addGroupMark();
    void removeLastToken();

private:

    void init();
    void setUnionButtonsEnabled( bool isEnabled );
    bool isItemUsed( const Sequence::Item& item ) const;
    bool areItemsLeft() const;
    void setItemButtonsEnabled( bool isEnabled );
    //void trace() const;

    Preferences*                prefs;

    Sequence                    sequence;

    QStack<Sequence::Item>      items; 
    Sequence::ItemList          currGroup;

    QBoxLayout*                 mainLayout;

    QWidget*                    sequencePanel;
    QBoxLayout*                 sequencePanelLayout;

    QWidget*                    quizPanel;
    QVBoxLayout*                quizPanelLayout;

    QLabel*                     quizLabel;

    QWidget*                    quizBody;
    QVBoxLayout*                quizBodyLayout;

    QScrollArea*                quizPanelWrapper;

    QWidget*                    quizTopPanel;
    QHBoxLayout*                quizTopPanelLayout;
    QWidget*                    quizTopLeftPanel;
    QVBoxLayout*                quizTopLeftPanelLayout;
    QWidget*                    quizImagePanel;
    QLabel*                     quizImageLabel;
    QVBoxLayout*                quizImagePanelLayout;
    QPushButton*                quizImageButton;
    
    QWidget*                    quizFirstLangPanel;
    QVBoxLayout*                quizFirstLangPanelLayout;
    QLabel*                     quizFirstLangPanelLabel;
    QWidget*                    quizFirstLangPanelWrapper;
    QHBoxLayout*                quizFirstLangPanelWrapperLayout;
    QLabel*                     quizFirstLangLabel;
    QPushButton*                quizFirstLangTermButton;

    QWidget*                    quizTestLangPanel;
    QVBoxLayout*                quizTestLangPanelLayout;
    QLabel*                     quizTestLangPanelLabel;
    QWidget*                    quizTestLangPanelWrapper;
    QHBoxLayout*                quizTestLangPanelWrapperLayout;
    QWidget*                    quizTestLangLabelsPanel;
    QVBoxLayout*                quizTestLangLabelsPanelLayout;
    QWidget*                    quizTestLangButtonsPanel;
    QVBoxLayout*                quizTestLangButtonsPanelLayout;

    QLabel*                     quizTestLangAltLabel;
    QLabel*                     quizTestLangTermLabel;
    QPushButton*                quizTestLangAltButton;
    QPushButton*                quizTestLangTermButton;

    QWidget*                    quizCommentBox;
    QVBoxLayout*                quizCommentBoxLayout;
    QLabel*                     quizCommentLabel;
    QPushButton*                quizCommentButton;

    QWidget*                    sequenceConsolePanel;
    QBoxLayout*                 sequenceConsolePanelLayout;
    QWidget*                    sequenceButtonsPanel;
    QGridLayout*                sequenceButtonsPanelLayout;
    QPushButton*                addSequenceMarkButton;
    QPushButton*                addGroupMarkButton;
    QPushButton*                removeLastMarkButton;

    QWidget*                    sequenceLinePanel;
    QBoxLayout*                 sequenceLinePanelLayout;
    QLabel*                     sequenceLineLabel;
    QLineEdit*                  sequenceLineLineEdit;

    QWidget*                    bottomButtonsPanel;
    QBoxLayout*                 bottomButtonsPanelLayout;
    QPushButton*                acceptButton;
    QPushButton*                cancelButton;

};

#endif
