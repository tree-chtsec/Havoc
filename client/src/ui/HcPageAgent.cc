#include <Havoc.h>
#include <ui/HcPageAgent.h>

HcPageAgent::HcPageAgent(QWidget* parent ) : QWidget(parent ) {

    if ( objectName().isEmpty() ) {
        setObjectName( QString::fromUtf8( "PageAgent" ) );
    }

    gridLayout = new QGridLayout( this );
    gridLayout->setObjectName( "gridLayout" );

    ComboAgentView = new QComboBox( this );
    ComboAgentView->setObjectName( "ComboAgentView" );
    ComboAgentView->view()->setProperty( "ComboBox", "true" );

    Splitter = new QSplitter( this );
    Splitter->setObjectName( QString::fromUtf8( "Splitter" ) );
    Splitter->setOrientation( Qt::Vertical );

    AgentTable = new QTableWidget( Splitter );
    AgentTable->setObjectName( "AgentTable" );

    TitleAgentID      = new QTableWidgetItem( "ID" );
    TitleInternal     = new QTableWidgetItem( "Internal" );
    TitleUsername     = new QTableWidgetItem( "User" );
    TitleHostname     = new QTableWidgetItem( "Host" );
    TitleSystem       = new QTableWidgetItem( "System" );
    TitleProcessID    = new QTableWidgetItem( "Pid" );
    TitleProcessName  = new QTableWidgetItem( "Process" );
    TitleProcessArch  = new QTableWidgetItem( "Arch" );
    TitleThreadID     = new QTableWidgetItem( "Tid" );
    TitleNote         = new QTableWidgetItem( "Note" );
    TitleLastCallback = new QTableWidgetItem( "Last" );

    if ( AgentTable->columnCount() < 11 ) {
        AgentTable->setColumnCount( 11 );
    }

    /* TODO: get how we should add this from the settings
     * for now we just do a default one */
    AgentTable->setHorizontalHeaderItem( 0,  TitleAgentID      );
    AgentTable->setHorizontalHeaderItem( 1,  TitleInternal     );
    AgentTable->setHorizontalHeaderItem( 2,  TitleUsername     );
    AgentTable->setHorizontalHeaderItem( 3,  TitleHostname     );
    AgentTable->setHorizontalHeaderItem( 4,  TitleSystem       );
    AgentTable->setHorizontalHeaderItem( 5,  TitleProcessName  );
    AgentTable->setHorizontalHeaderItem( 6,  TitleProcessArch  );
    AgentTable->setHorizontalHeaderItem( 7,  TitleProcessID    );
    AgentTable->setHorizontalHeaderItem( 8,  TitleThreadID     );
    AgentTable->setHorizontalHeaderItem( 9,  TitleLastCallback );
    AgentTable->setHorizontalHeaderItem( 10, TitleNote         );

    /* table settings */
    AgentTable->setEnabled( true );
    AgentTable->setShowGrid( false );
    AgentTable->setSortingEnabled( false );
    AgentTable->setWordWrap( true );
    AgentTable->setCornerButtonEnabled( true );
    AgentTable->horizontalHeader()->setVisible( true );
    AgentTable->setSelectionBehavior( QAbstractItemView::SelectRows );
    AgentTable->setContextMenuPolicy( Qt::CustomContextMenu );
    AgentTable->horizontalHeader()->setSectionResizeMode( QHeaderView::ResizeMode::Stretch );
    AgentTable->horizontalHeader()->setStretchLastSection( true );
    AgentTable->verticalHeader()->setVisible( false );
    AgentTable->setFocusPolicy( Qt::NoFocus );

    AgentTab = new QTabWidget( Splitter );
    AgentTab->setObjectName( "AgentTab" );
    AgentTab->setTabsClosable( true );
    AgentTab->setMovable( true );

    AgentDisplayerSessions = new QLabel( this );
    AgentDisplayerSessions->setObjectName( "LabelDisplaySessions" );
    AgentDisplayerSessions->setProperty( "HcLabelDisplay", "true" );

    AgentDisplayerTargets = new QLabel( this );
    AgentDisplayerTargets->setObjectName( "LabelDisplayTargets" );
    AgentDisplayerTargets->setProperty( "HcLabelDisplay", "true" );

    AgentDisplayerPivots = new QLabel( this );
    AgentDisplayerPivots->setObjectName( "LabelDisplayPivots" );
    AgentDisplayerPivots->setProperty( "HcLabelDisplay", "true" );

    AgentDisplayerElevated = new QLabel( this );
    AgentDisplayerElevated->setObjectName( "LabelDisplayElevated" );
    AgentDisplayerElevated->setProperty( "HcLabelDisplay", "true" );

    horizontalSpacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );

    Splitter->addWidget( AgentTable );
    Splitter->addWidget( AgentTab );
    Splitter->handle( 1 )->setEnabled( SplitterMoveToggle ); /* disabled by default */

    gridLayout->addWidget( ComboAgentView,         0, 0, 1, 1 );
    gridLayout->addWidget( Splitter,               1, 0, 1, 6 );
    gridLayout->addWidget( AgentDisplayerSessions, 0, 2, 1, 1 );
    gridLayout->addWidget( AgentDisplayerTargets,  0, 1, 1, 1 );
    gridLayout->addWidget( AgentDisplayerPivots,   0, 3, 1, 1 );
    gridLayout->addWidget( AgentDisplayerElevated, 0, 4, 1, 1 );
    gridLayout->addItem( horizontalSpacer,         0, 5, 1, 1 );

    retranslateUi( );

    QMetaObject::connectSlotsByName( this );
}

HcPageAgent::~HcPageAgent() = default;

auto HcPageAgent::retranslateUi() -> void {
    setStyleSheet( Havoc->getStyleSheet() );

    AgentDisplayerElevated->setText( "Elevated: 0" );
    AgentDisplayerSessions->setText( "Beacons: 0" );
    AgentDisplayerTargets->setText( "Targets: 0" );
    AgentDisplayerPivots->setText( "Pivots: 0" );
    ComboAgentView->addItems( QStringList() << "Sessions" << "Sessions Graph" << "Targets" );
}

auto HcPageAgent::addTab(
    const QString& name,
    QWidget*       widget
) -> void {

}

auto HcPageAgent::addAgent(
    json agent
) -> void {

}
