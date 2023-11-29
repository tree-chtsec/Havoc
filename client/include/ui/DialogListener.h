#ifndef HAVOCCLIENT_DIALOGLISTENER_H
#define HAVOCCLIENT_DIALOGLISTENER_H

#include <Common.h>

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class HavocListener : public QDialog
{
public:
    QGridLayout* gridLayout         = nullptr;
    QLabel*      LabelListenerName  = nullptr;
    QLabel*      LabelProtocol      = nullptr;
    QLineEdit*   InputListenerName  = nullptr;
    QComboBox*   ComboProtocol      = nullptr;
    QGroupBox*   GroupOptions       = nullptr;

    explicit HavocListener();

    auto addProtocol(
        const json& data
    ) -> void;

    auto getOptions() -> json;

    auto retranslateUi() -> void;
};

QT_END_NAMESPACE

#endif //HAVOCCLIENT_DIALOGLISTENER_H
