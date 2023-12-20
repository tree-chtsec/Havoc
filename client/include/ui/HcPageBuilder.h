#ifndef HAVOCCLIENT_HCPAGEBUILDER_H
#define HAVOCCLIENT_HCPAGEBUILDER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class HcPageBuilder : public QWidget
{
public:
    QGridLayout*    gridLayout    = nullptr;
    QLabel*         LabelPayload  = nullptr;
    QComboBox*      ComboPayload  = nullptr;
    QSpacerItem*    Spacer        = nullptr;
    QStackedWidget* StackedWidget = nullptr;

    explicit HcPageBuilder();

    auto retranslateUi() -> void;

};

QT_END_NAMESPACE

#endif //HAVOCCLIENT_HCPAGEBUILDER_H
