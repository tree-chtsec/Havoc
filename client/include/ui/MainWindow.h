#ifndef HAVOCCLIENT_MAINWINDOW_H
#define HAVOCCLIENT_MAINWINDOW_H

#include <Havoc.h>

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class HavocButton : public QPushButton
{
    bool    buttonUsed         = false;
    QString originalObjectName = {};
public:
    explicit HavocButton( QWidget* w );

    auto currentlyUsed() const -> bool;
    auto setUsed( bool state ) -> void;
};

class HavocMainWindow : public QMainWindow
{

public:
    QWidget*            MainWidget      = nullptr;
    QGridLayout*        gridLayout      = nullptr;
    QGridLayout*        gridLayout_2    = nullptr;
    QWidget*            SideBarWidget   = nullptr;
    HavocButton*        ButtonHavoc     = nullptr;
    HavocButton*        ButtonListeners = nullptr;
    HavocButton*        ButtonSettings  = nullptr;
    HavocButton*        ButtonScripts   = nullptr;
    HavocButton*        ButtonAgents    = nullptr;
    HavocButton*        ButtonServer    = nullptr;
    HavocButton*        ButtonPayload   = nullptr;
    QSpacerItem*        Spacer          = nullptr;
    QStackedWidget*     StackedWidget   = nullptr;
    HavocPageAgent*     PageAgent       = nullptr;
    HavocPageListener*  PageListener    = nullptr;
    HavocPageBuilder*   PagePayload     = nullptr;
    QWidget*            PageServer      = nullptr;
    HavocPageScript*    PageScripts     = nullptr;
    QWidget*            PageSettings    = nullptr;

    explicit HavocMainWindow( );

    auto retranslateUi() -> void;
    auto renderWindow() -> void;
    auto unusedSideButtons() -> void;
    auto useSideButton(
        HavocButton*   button,
        const QString& name
    ) -> void;

    auto switchPageAgent() -> void;
    auto switchPageListener() -> void;
    auto switchPagePayload() -> void;
    auto switchPageServer() -> void;
    auto switchPageScripts() -> void;
    auto switchPageSettings() -> void;
};

QT_END_NAMESPACE

#endif //HAVOCCLIENT_MAINWINDOW_H
