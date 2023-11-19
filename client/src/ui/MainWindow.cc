#include <Havoc.h>
#include "ui/HavocMainWindow.h"


HavocMainWindow::HavocMainWindow() {
    if ( objectName().isEmpty() )
        setObjectName( QString::fromUtf8( "HavocMainWindow" ) );

    resize( 1124, 659 );

    MainWidget      = new QWidget( this );
    gridLayout      = new QGridLayout( MainWidget );
    SideBarWidget   = new QWidget( MainWidget );
    gridLayout_2    = new QGridLayout( SideBarWidget );
    ButtonListeners = new QPushButton( SideBarWidget );
    ButtonSettings  = new QPushButton( SideBarWidget );
    ButtonScripts   = new QPushButton( SideBarWidget );
    ButtonAgents    = new QPushButton( SideBarWidget );
    ButtonServer    = new QPushButton( SideBarWidget );
    ButtonPayload   = new QPushButton( SideBarWidget );
    Spacer          = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    StackedWidget   = new QStackedWidget( MainWidget );

    MainWidget->setObjectName( QString::fromUtf8( "MainWidget" ) );
    gridLayout->setObjectName( QString::fromUtf8("gridLayout" ) );
    SideBarWidget->setObjectName( QString::fromUtf8( "SideBarWidget" ) );
    gridLayout_2->setObjectName( QString::fromUtf8( "gridLayout_2" ) );
    ButtonListeners->setObjectName( QString::fromUtf8( "ButtonListeners" ) );
    ButtonSettings->setObjectName( QString::fromUtf8( "ButtonSettings" ) );
    ButtonScripts->setObjectName( QString::fromUtf8( "ButtonScripts" ) );
    ButtonAgents->setObjectName( QString::fromUtf8( "ButtonAgents" ) );
    ButtonServer->setObjectName( QString::fromUtf8( "ButtonServer" ) );
    ButtonPayload->setObjectName( QString::fromUtf8( "ButtonPayload" ) );
    StackedWidget->setObjectName( QString::fromUtf8( "StackedWidget" ) );

    gridLayout_2->addWidget( ButtonAgents,    0, 0, 1, 1 );
    gridLayout_2->addWidget( ButtonListeners, 1, 0, 1, 1 );
    gridLayout_2->addWidget( ButtonPayload,   2, 0, 1, 1 );
    gridLayout_2->addWidget( ButtonServer,    3, 0, 1, 1 );
    gridLayout_2->addWidget( ButtonScripts,   4, 0, 1, 1 );
    gridLayout_2->addWidget( ButtonSettings,  5, 0, 1, 1 );

    gridLayout_2->addItem( Spacer, 6, 0, 1, 1 );

    gridLayout->addWidget( SideBarWidget, 0, 0, 1, 1 );
    gridLayout->addWidget( StackedWidget, 0, 1, 1, 1 );

    setCentralWidget( MainWidget );

    retranslateUi();

    QMetaObject::connectSlotsByName( this );
}

auto HavocMainWindow::retranslateUi() -> void {
    setWindowTitle( QCoreApplication::translate( "HavocMainWindow", "MainWindow", nullptr ) );
    ButtonListeners->setText( QCoreApplication::translate( "HavocMainWindow", "Listeners", nullptr ) );
    ButtonSettings->setText( QCoreApplication::translate( "HavocMainWindow", "Settings", nullptr ) );
    ButtonScripts->setText( QCoreApplication::translate( "HavocMainWindow", "Scripts", nullptr ) );
    ButtonAgents->setText( QCoreApplication::translate( "HavocMainWindow", "Agents", nullptr ) );
    ButtonServer->setText( QCoreApplication::translate( "HavocMainWindow", "Server", nullptr ) );
    ButtonPayload->setText( QCoreApplication::translate( "HavocMainWindow", "Payload", nullptr ) );
}

auto HavocMainWindow::renderWindow() -> void {
    show();
}
