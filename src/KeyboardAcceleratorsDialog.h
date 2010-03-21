/*
 * This class contains some code borrowed from 
 * keyboard_shortcuts.h (rev. 1.2) and
 * keyboard_shortcuts.cpp (rev. 1.3) of ZBEDic.
 * Copyright (C) 2004 Rafal Mantiuk <rafm@users.sourceforge.org>
 */

#ifndef KEYBOARD_ACCELERATORS_DIALOG_H
#define KEYBOARD_ACCELERATORS_DIALOG_H 

#include <QAction>
#include <QBoxLayout>
#include <QDialog>
#include <QHeaderView>
#include <QKeyEvent>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollBar>
#include <QString>
#include <QTreeWidget>
#include <QVBoxLayout>
#include "KeyActionListViewItem.h"
#include "Preferences.h"
#include "Util.h"

extern QAction* action[ ACTION_COUNT ];

class KeyboardAcceleratorsDialog : public QDialog {

    Q_OBJECT
    
public:

    KeyboardAcceleratorsDialog( QWidget* parent, Preferences* prefs );
    ~KeyboardAcceleratorsDialog(); 

    QMap<Action, int> getAccelerators() const;
    bool areAcceleratorsModified() const;

public slots:

protected slots:

    virtual void accept();

private slots:

    void clearAccelKey();
    void setAccelKey();
    void resetAccelKey();
    void cancelSetAccelKey();
    void updateUi();

private:

    void init();
    void keyPressEvent( QKeyEvent *evt );
    void setAccelerator( Action action, int accel );

    Preferences*        prefs;
    QMap<Action,int>    accel;

    QHBoxLayout*        mainLayout;

    QWidget*            bodyPanel;
    QVBoxLayout*        bodyPanelLayout;

    QBoxLayout*         keyboardAccelButtonPanelLayout;

    QWidget*            keyboardAccelPanel;
    QVBoxLayout*        keyboardAccelPanelLayout;

    QTreeWidgetItem*    grabAccelKeyFor;
    QTreeWidget*        keyboardAccelListView;
    bool                keyboardAccelModified;

    QWidget*            keyboardAccelButtonPanel;
    QPushButton*        clearAccelKeyButton;
    QPushButton*        setAccelKeyButton;
    QPushButton*        resetAccelKeyButton;

    QWidget*            bottomButtonsPanel;
    QVBoxLayout*        bottomButtonsPanelLayout;
    QPushButton*        acceptButton;
    QPushButton*        cancelButton;

};

#endif

