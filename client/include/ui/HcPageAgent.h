#ifndef HAVOCCLIENT_HCPAGEAGENT_H
#define HAVOCCLIENT_HCPAGEAGENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class HcPageAgent : public QWidget
{
    bool SplitterMoveToggle = false;

public:
    QGridLayout*      gridLayout             = nullptr;
    QComboBox*        ComboAgentView         = nullptr;
    QSplitter*        Splitter               = nullptr;
    QTableWidget*     AgentTable             = nullptr;
    QTabWidget*       AgentTab               = nullptr;
    QLabel*           AgentDisplayerElevated = nullptr;
    QLabel*           AgentDisplayerSessions = nullptr;
    QLabel*           AgentDisplayerTargets  = nullptr;
    QLabel*           AgentDisplayerPivots   = nullptr;
    QSpacerItem*      horizontalSpacer       = nullptr;
    QTableWidgetItem* TitleAgentID           = nullptr;
    QTableWidgetItem* TitleInternal          = nullptr;
    QTableWidgetItem* TitleUsername          = nullptr;
    QTableWidgetItem* TitleHostname          = nullptr;
    QTableWidgetItem* TitleSystem            = nullptr;
    QTableWidgetItem* TitleProcessID         = nullptr;
    QTableWidgetItem* TitleProcessName       = nullptr;
    QTableWidgetItem* TitleProcessArch       = nullptr;
    QTableWidgetItem* TitleThreadID          = nullptr;
    QTableWidgetItem* TitleNote              = nullptr;
    QTableWidgetItem* TitleLastCallback      = nullptr;

    explicit HcPageAgent(QWidget* parent = nullptr );
    ~HcPageAgent();

    auto retranslateUi() -> void;

    auto addTab(
        const QString& name,
        QWidget*       widget
    ) -> void;

    auto addAgent(
        json agent
    ) -> void;
};

QT_END_NAMESPACE

#endif //HAVOCCLIENT_HCPAGEAGENT_H
