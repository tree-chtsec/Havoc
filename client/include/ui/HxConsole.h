#ifndef HAVOCCLIENT_HXCONSOLE_H
#define HAVOCCLIENT_HXCONSOLE_H

#include <Havoc.h>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class HxConsole : public QWidget
{
    QGridLayout* gridLayout  = nullptr;
    QLabel*      LabelHeader = nullptr;
    QTextEdit*   Console     = nullptr;
    QLabel*      LabelBottom = nullptr;
    QLabel*      LabelInput  = nullptr;

public:
    QLineEdit* Input = nullptr;

    explicit HxConsole();

    auto setHeaderLabel(
        const QString& text
    ) -> void;

    auto setBottomLabel(
        const QString& text
    ) -> void;

    auto setInputLabel(
        const QString& text
    ) -> void;
};

QT_END_NAMESPACE

#endif //HAVOCCLIENT_HXCONSOLE_H
