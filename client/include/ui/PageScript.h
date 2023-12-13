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

QT_BEGIN_NAMESPACE

class HavocPageScript : public QWidget
{
public:
    QGridLayout*  gridLayout           = nullptr;
    QSpacerItem*  Spacer               = nullptr;
    QSplitter*    Splitter             = nullptr;
    QTableWidget* ScriptTableWidget    = nullptr;
    QTabWidget*   ScriptTabWidget      = nullptr;
    QWidget*      TabScriptInterpreter = nullptr;
    QPushButton*  ButtonLoadScript     = nullptr;
    QLabel*       label                = nullptr;

    explicit HavocPageScript();

    auto retranslateUi() -> void;

};

QT_END_NAMESPACE

#endif
