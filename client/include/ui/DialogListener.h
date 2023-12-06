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
#include <QtWidgets/QStackedWidget>

QT_BEGIN_NAMESPACE

struct ProtclOption {
    std::string name;
    std::string type;
    int         event;
    bool        optional;
    QWidget*    widget;
    json        option;
};

struct ProtclTabOption {
    std::string               name;
    std::vector<ProtclOption> options;
    QWidget*                  tab;
    QGridLayout*              layout;
};

struct Protocol {
    QString                      type;
    std::vector<ProtclTabOption> tabs;
    json                         data;
    QSize                        resize;
    QWidget*                     page;
    QTabWidget*                  tabWidget;
    QGridLayout*                 layout;
};

enum ListenerState {
    None,
    Saved,
    Closed,
    Error
};

const std::string ListenerWidgetTypeLabel  = "label";
const std::string ListenerWidgetTypeInput  = "input";
const std::string ListenerWidgetTypeCombo  = "combo";
const std::string ListenerWidgetTypeSpacer = "spacer";
const std::string ListenerWidgetTypeList   = "list";
const std::string ListenerWidgetTypeToggle = "toggle";
const std::string ListenerWidgetTypeFile   = "file";
const std::string ListenerWidgetTypeText   = "text";
const std::string ListenerWidgetTypeButton = "button";

const int ListenerEventNone   = 0b00000000;
const int ListenerEventEnter  = 0b00000001;
const int ListenerEventChange = 0b00000010;
const int ListenerEventAll    = 0b10000000;

class HavocListener : public QDialog
{
    ListenerState   State            = None;

public:
    QGridLayout*    gridLayout       = nullptr;
    QLabel*         LabelName        = nullptr;
    QLineEdit*      InputName        = nullptr;
    QLabel*         LabelProtocol    = nullptr;
    QComboBox*      ComboProtocol    = nullptr;
    QStackedWidget* StackedProtocols = nullptr;
    QPushButton*    ButtonSave       = nullptr;
    QPushButton*    ButtonClose      = nullptr;
    QSpacerItem*    horizontal[ 4 ]  = {};

    Protocol*             Previous   = nullptr;
    std::vector<Protocol> Protocols  = {};

    explicit HavocListener();

    auto addProtocol(
        const json& data
    ) -> void;

    auto insertPage(
        Protocol& protocol
    ) -> void;

    auto insertTab(
        Protocol&          protocol,
        const std::string& name,
        std::vector<json>& widgets
    ) -> void;

    auto addOption(
        ProtclTabOption& tab,
        ProtclOption&    option
    ) -> void;

    auto getOptions() -> json;

    auto setOption(
        const ProtclOption* option,
        const json&         values
    ) -> void;

    auto getOption(
        const std::string& name
    ) -> ProtclOption*;

    auto getOptionValue(
        const std::string& name
    ) -> json;

    auto eventProcess(
        const json& data
    ) -> void;

    auto getCurrentProtocol() -> Protocol*;
    auto getCloseState() -> ListenerState;
    auto sanityCheckOptions() -> bool;
    auto save() -> void;

    auto changeProtocol(
        const QString& text
    ) -> void;

    auto start() -> void;
    auto retranslateUi() -> void;
};

QT_END_NAMESPACE

#endif //HAVOCCLIENT_DIALOGLISTENER_H
