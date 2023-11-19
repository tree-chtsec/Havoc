#include <Havoc.h>
#include <ui/PageAgent.h>

HavocPageAgent::HavocPageAgent( QWidget* parent ) : QWidget( parent ) {

    if ( objectName().isEmpty() ) {
        setObjectName( QString::fromUtf8( "PageAgent" ) );
    }

    setStyleSheet( Havoc->getStyleSheet() );

    gridLayout = new QGridLayout( this );
    gridLayout->setObjectName( "gridLayout" );

    AgentDisplayerElevated = new QLabel( this );
    AgentDisplayerElevated->setObjectName( "AgentDisplayerElevated" );

    ComboAgentView = new QComboBox( this );
    ComboAgentView->setObjectName( "ComboAgentView" );

    gridLayout->addWidget( AgentDisplayerElevated, 0, 4, 1, 1 );
    gridLayout->addWidget( ComboAgentView, 0, 0, 1, 1 );

    Splitter = new QSplitter( this );
    Splitter->setObjectName( QString::fromUtf8( "Splitter" ) );
    Splitter->setOrientation( Qt::Vertical );

    AgentTable = new QTableWidget( Splitter );
    AgentTable->setObjectName( "AgentTable" );

    AgentTab = new QTabWidget( Splitter );
    AgentTab->setObjectName( "AgentTab" );
    AgentTab->setTabsClosable( true );
    AgentTab->setMovable( true );

    AgentDisplayerSessions = new QLabel( this );
    AgentDisplayerSessions->setObjectName( "AgentDisplayerSessions" );

    AgentDisplayerTargets = new QLabel( this );
    AgentDisplayerTargets->setObjectName( "AgentDisplayerTargets" );

    AgentDisplayerPivots = new QLabel( this );
    AgentDisplayerPivots->setObjectName( "AgentDisplayerPivots" );

    horizontalSpacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );

    Splitter->addWidget( AgentTable );
    Splitter->addWidget( AgentTab );
    Splitter->handle( 1 )->setEnabled( SplitterMoveToggle ); /* disabled by default */

    gridLayout->addWidget( Splitter,               1, 0, 1, 6 );
    gridLayout->addWidget( AgentDisplayerSessions, 0, 2, 1, 1 );
    gridLayout->addWidget( AgentDisplayerTargets,  0, 1, 1, 1 );
    gridLayout->addWidget( AgentDisplayerPivots,   0, 3, 1, 1 );
    gridLayout->addItem( horizontalSpacer, 0, 5, 1, 1 );

    retranslateUi( );

    QMetaObject::connectSlotsByName( this );
}

HavocPageAgent::~HavocPageAgent() = default;

auto HavocPageAgent::retranslateUi() -> void {
    AgentDisplayerElevated->setText( "Elevated: 0" );
    AgentDisplayerSessions->setText( "Beacons: 0" );
    AgentDisplayerTargets->setText( "Targets: 0" );
    AgentDisplayerPivots->setText( "Pivots: 0" );

    ComboAgentView->addItems( QStringList() << "Sessions" << "Sessions Graph" << "Targets" );
}

auto HavocPageAgent::addTab(
    const QString& name,
    QWidget*       widget
) -> void {

}

auto HavocPageAgent::addAgent(
    json agent
) -> void {

}
