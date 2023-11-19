#ifndef HAVOCCLIENT_HAVOCMAINWINDOW_H
#define HAVOCCLIENT_HAVOCMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class HavocMainWindow : public QMainWindow
{
public:
    QWidget*        MainWidget      = nullptr;
    QGridLayout*    gridLayout      = nullptr;
    QGridLayout*    gridLayout_2    = nullptr;
    QWidget*        SideBarWidget   = nullptr;
    QPushButton*    ButtonListeners = nullptr;
    QPushButton*    ButtonSettings  = nullptr;
    QPushButton*    ButtonScripts   = nullptr;
    QPushButton*    ButtonAgents    = nullptr;
    QPushButton*    ButtonServer    = nullptr;
    QPushButton*    ButtonPayload   = nullptr;
    QSpacerItem*    Spacer          = nullptr;
    QStackedWidget* StackedWidget   = nullptr;

    explicit HavocMainWindow( );

    auto retranslateUi() -> void;
    auto renderWindow() -> void;
};

QT_END_NAMESPACE

#endif //HAVOCCLIENT_HAVOCMAINWINDOW_H
