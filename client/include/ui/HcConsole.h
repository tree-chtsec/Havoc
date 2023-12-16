#ifndef HAVOCCLIENT_HCCONSOLE_H
#define HAVOCCLIENT_HCCONSOLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class HcConsole : public QWidget
{
    QGridLayout* gridLayout  = {};
    QLabel*      LabelHeader = {};
    QTextEdit*   Console     = {};
    QLabel*      LabelBottom = {};
    QLabel*      LabelInput  = {};

public:
    QLineEdit*                Input         = nullptr;
    std::vector<py11::object> InputCallback = {};

    explicit HcConsole(
        QWidget* parent
    );

    auto setHeaderLabel(
        const QString& text
    ) -> void;

    auto setBottomLabel(
        const QString& text
    ) -> void;

    auto setInputLabel(
        const QString& text
    ) -> void;

    auto appendConsole(
        const QString& text
    ) -> void;
};

QT_END_NAMESPACE

#endif //HAVOCCLIENT_HCCONSOLE_H
