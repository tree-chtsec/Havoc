#ifndef HAVOCCLIENT_PAGESCRIPT_H
#define HAVOCCLIENT_PAGESCRIPT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

#include <ui/HcConsole.h>

QT_BEGIN_NAMESPACE

class HavocPagePlugins : public QWidget
{

public:
    QGridLayout*  gridLayout         = nullptr;
    QGridLayout*  gridLayout_2       = nullptr;
    QSplitter*    splitter           = nullptr;
    QSpacerItem*  horizontalSpacer   = nullptr;

    QTabWidget*   TabWidget          = nullptr;
    QWidget*      TabPluginManager   = nullptr;
    QPushButton*  ButtonLoad         = nullptr;
    QLabel*       LabelLoadedPlugins = nullptr;
    QTableWidget* TablePluginsWidget = nullptr;
    HcConsole*    PyConsole          = nullptr;
    QWidget*      TabPluginStore     = nullptr;

    explicit HavocPagePlugins();

    auto retranslateUi() -> void;
};

QT_END_NAMESPACE

#endif
