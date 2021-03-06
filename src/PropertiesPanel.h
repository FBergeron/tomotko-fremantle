#ifndef PROPERTIES_PANEL_H
#define PROPERTIES_PANEL_H 

#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QLineEdit>
#include <QTextEdit>
#include <QTreeWidget>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>
#include "DigraphLineEdit.h"
#include "DigraphMultiLineEdit.h"
#include "DynamicHeightTreeWidget.h"
#include "Folder.h"
#include "Preferences.h"
#include "Vocabulary.h"

class PropertiesPanel : public QWidget {

    Q_OBJECT
    
public:

    PropertiesPanel( const Preferences& prefs, QWidget* parent );
    ~PropertiesPanel(); 

    void setVocabulary( Vocabulary* vocab );
    void setFolder( Folder* folder );

    void updateFonts();
    void setDigraphEnabled( bool isEnabled );
    bool isDigraphEnabled() const;
    void retranslateUi();

public slots:

    void updateCounters();

signals:

    void titleChanged( const QString& title );

protected:

    virtual void resizeEvent( QResizeEvent* evt );

private slots:
    
    void updateTitle( const QString& title );
    void updateDescription();
    void updateAuthor( const QString& author );

private:

    void init();
    void updateModel();

    void addListeners();
    void removeListeners();

    QString getType() const;

    QVBoxLayout*                mainLayout;
    QWidget*                    simplePropsPanel;
    QHBoxLayout*                simplePropsPanelLayout;

    QWidget*                    simplePropsLabelsPanel;
    QVBoxLayout*                simplePropsLabelsPanelLayout;
    QWidget*                    simplePropsFieldsPanel;
    QVBoxLayout*                simplePropsFieldsPanelLayout;

    QWidget*                    titlePanel;
    QHBoxLayout*                titlePanelLayout;
    QLabel*                     titleLabel;
    DigraphLineEdit*            titleLineEdit;

    QLabel*                     descriptionLabel;
    DigraphMultiLineEdit*       descriptionMultiLineEdit;

    QLabel*                     contentLabel;
    DynamicHeightTreeWidget*    contentListView;

    QLabel*                     authorLabel;
    DigraphLineEdit*            authorField;

    QLabel*                     creationDateLabel;
    QLabel*                     creationDateValueLabel;

    QLabel*                     modificationDateLabel;
    QLabel*                     modificationDateValueLabel;

    const Preferences&          prefs;
    Vocabulary*                 editedVocab;
    Folder*                     editedFolder;

};

#endif
