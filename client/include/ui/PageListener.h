#ifndef HAVOCCLIENT_PAGELISTENER_H
#define HAVOCCLIENT_PAGELISTENER_H

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


class HavocPageListener : public QWidget
{
public:
    QGridLayout*      gridLayout        = nullptr;
    QLabel*           ActiveLabel       = nullptr;
    QSpacerItem*      horizontalSpacer  = nullptr;
    QPushButton*      ButtonNewListener = nullptr;
    QSplitter*        Splitter          = nullptr;
    QTableWidget*     TableWidget       = nullptr;
    QTabWidget*       TabWidget         = nullptr;
    QTableWidgetItem* TitleName         = nullptr;
    QTableWidgetItem* TitleType         = nullptr;
    QTableWidgetItem* TitleHost         = nullptr;
    QTableWidgetItem* TitlePort         = nullptr;
    QTableWidgetItem* TitleStatus       = nullptr;
    std::vector<json> Protocols         = {};

    explicit HavocPageListener();
    auto retranslateUi() -> void;

    auto buttonAddListener() -> void;
};

QT_END_NAMESPACE

#endif //HAVOCCLIENT_PAGELISTENER_H
