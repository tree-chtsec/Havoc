#ifndef HAVOCCLIENT_PAGEAGENT_H
#define HAVOCCLIENT_PAGEAGENT_H

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

class HavocPageAgent : public QWidget
{
    bool SplitterMoveToggle = false;

public:
    QGridLayout*  gridLayout             = nullptr;
    QComboBox*    ComboAgentView         = nullptr;
    QSplitter*    Splitter               = nullptr;
    QTableWidget* AgentTable             = nullptr;
    QTabWidget*   AgentTab               = nullptr;
    QLabel*       AgentDisplayerElevated = nullptr;
    QLabel*       AgentDisplayerSessions = nullptr;
    QLabel*       AgentDisplayerTargets  = nullptr;
    QLabel*       AgentDisplayerPivots   = nullptr;
    QSpacerItem*  horizontalSpacer       = nullptr;

    explicit HavocPageAgent( QWidget* parent = nullptr );
    ~HavocPageAgent();

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

#endif //HAVOCCLIENT_PAGEAGENT_H
