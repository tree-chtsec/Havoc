#include <Havoc.h>
#include "ui/PageListener.h"


HavocPageListener::HavocPageListener() {
    if ( objectName().isEmpty() ) {
        setObjectName( QString::fromUtf8( "PageListener" ) );
    }

    gridLayout = new QGridLayout( this );
    gridLayout->setObjectName( QString::fromUtf8( "gridLayout" ) );

    ActiveLabel = new QLabel( this );
    ActiveLabel->setObjectName( QString::fromUtf8("LabelDisplayListenerActive" ) );
    ActiveLabel->setProperty( "labelDisplay", "true" );

    horizontalSpacer  = new QSpacerItem( 1157, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    ButtonNewListener = new QPushButton( this );
    ButtonNewListener->setObjectName( QString::fromUtf8( "ButtonNewListener" ) );
    ButtonNewListener->setProperty( "ClickButton", "true" );

    Splitter = new QSplitter( this );
    Splitter->setObjectName( QString::fromUtf8( "Splitter" ) );
    Splitter->setOrientation( Qt::Vertical );

    TableWidget = new QTableWidget( Splitter );
    TableWidget->setObjectName( QString::fromUtf8( "TableWidget" ) );

    TitleName   = new QTableWidgetItem( "Name" );
    TitleType   = new QTableWidgetItem( "Type" );
    TitleHost   = new QTableWidgetItem( "Host" );
    TitlePort   = new QTableWidgetItem( "Port" );
    TitleStatus = new QTableWidgetItem( "Status" );

    if ( TableWidget->columnCount() < 5 ) {
        TableWidget->setColumnCount( 5 );
    }

    TableWidget->setHorizontalHeaderItem( 0, TitleName   );
    TableWidget->setHorizontalHeaderItem( 1, TitleType   );
    TableWidget->setHorizontalHeaderItem( 2, TitleHost   );
    TableWidget->setHorizontalHeaderItem( 3, TitlePort   );
    TableWidget->setHorizontalHeaderItem( 4, TitleStatus );

    /* table settings */
    TableWidget->setEnabled( true );
    TableWidget->setShowGrid( false );
    TableWidget->setSortingEnabled( false );
    TableWidget->setWordWrap( true );
    TableWidget->setCornerButtonEnabled( true );
    TableWidget->horizontalHeader()->setVisible( true );
    TableWidget->setSelectionBehavior( QAbstractItemView::SelectRows );
    TableWidget->setContextMenuPolicy( Qt::CustomContextMenu );
    TableWidget->horizontalHeader()->setSectionResizeMode( QHeaderView::ResizeMode::Stretch );
    TableWidget->horizontalHeader()->setStretchLastSection( true );
    TableWidget->verticalHeader()->setVisible( false );
    TableWidget->setFocusPolicy( Qt::NoFocus );
    
    TabWidget = new QTabWidget( Splitter );
    TabWidget->setObjectName( QString::fromUtf8( "TabWidget" ) );

    Splitter->addWidget( TableWidget );
    Splitter->addWidget( TabWidget );

    gridLayout->addWidget( ButtonNewListener, 0, 0, 1, 1 );
    gridLayout->addItem( horizontalSpacer, 0, 1, 1, 1 );
    gridLayout->addWidget( ActiveLabel, 0, 2, 1, 1 );
    gridLayout->addWidget( Splitter, 1, 0, 1, 3 );

    retranslateUi();

    QObject::connect( ButtonNewListener, &QPushButton::clicked, this, &HavocPageListener::buttonAddListener );

    QMetaObject::connectSlotsByName( this );
}

auto HavocPageListener::retranslateUi() -> void {
    setStyleSheet( Havoc->getStyleSheet() );

    setWindowTitle( "PageListener" );
    ActiveLabel->setText( "Active: 0" );
    ButtonNewListener->setText( "Add Listener" );
}

auto HavocPageListener::buttonAddListener() -> void {
    auto Dialog = new HavocListener;

    /* add registered protocols
     * to the dialog listener */
    for ( auto& p : Protocols ) {
        Dialog->addProtocol( p );
    }

    Dialog->start();

    delete Dialog;
}

auto HavocPageListener::addListener(
    const json& data
) -> void {
    auto name   = QString();
    auto type   = QString();
    auto host   = QString();
    auto port   = QString();
    auto status = QString();
    auto item   = new Listener();

    if ( data.contains( "name" ) ) {
        if ( data[ "name" ].is_string() ) {
            name = data[ "name" ].get<std::string>().c_str();
        } else {
            spdlog::error( "invalid listener: \"name\" is not string" );
            return;
        }
    } else {
        spdlog::error( "invalid listener: \"name\" is not found" );
        return;
    }

    if ( data.contains( "protocol" ) ) {
        if ( data[ "protocol" ].is_string() ) {
            type = data[ "protocol" ].get<std::string>().c_str();
        } else {
            spdlog::error( "invalid listener: \"protocol\" is not string" );
            return;
        }
    } else {
        spdlog::error( "invalid listener: \"protocol\" is not found" );
        return;
    }

    if ( data.contains( "host" ) ) {
        if ( data[ "host" ].is_string() ) {
            host = data[ "host" ].get<std::string>().c_str();
        } else {
            spdlog::error( "invalid listener: \"host\" is not string" );
            return;
        }
    } else {
        spdlog::error( "invalid listener: \"host\" is not found" );
        return;
    }

    if ( data.contains( "port" ) ) {
        if ( data[ "port" ].is_string() ) {
            port = data[ "port" ].get<std::string>().c_str();
        } else {
            spdlog::error( "invalid listener: \"port\" is not string" );
            return;
        }
    } else {
        spdlog::error( "invalid listener: \"port\" is not found" );
        return;
    }

    if ( data.contains( "status" ) ) {
        if ( data[ "status" ].is_string() ) {
            status = data[ "status" ].get<std::string>().c_str();
        } else {
            spdlog::error( "invalid listener: \"status\" is not string" );
            return;
        }
    } else {
        spdlog::error( "invalid listener: \"status\" is not found" );
        return;
    }

    item->Name   = new QTableWidgetItem( name );
    item->Type   = new QTableWidgetItem( type );
    item->Host   = new QTableWidgetItem( host );
    item->Port   = new QTableWidgetItem( port );
    item->Status = new QTableWidgetItem( status );

    item->Name->setFlags( item->Name->flags() ^ Qt::ItemIsEditable );
    item->Type->setFlags( item->Type->flags() ^ Qt::ItemIsEditable );
    item->Host->setFlags( item->Host->flags() ^ Qt::ItemIsEditable );
    item->Port->setFlags( item->Port->flags() ^ Qt::ItemIsEditable );
    item->Status->setFlags( item->Status->flags() ^ Qt::ItemIsEditable );

    if ( TableWidget->rowCount() < 1 ) {
        TableWidget->setRowCount( 1 );
    } else {
        TableWidget->setRowCount( TableWidget->rowCount() + 1 );
    }

    const bool isSortingEnabled = TableWidget->isSortingEnabled();
    TableWidget->setSortingEnabled( false );

    TableWidget->setItem( TableWidget->rowCount() - 1, 0, item->Name );
    TableWidget->setItem( TableWidget->rowCount() - 1, 1, item->Type );
    TableWidget->setItem( TableWidget->rowCount() - 1, 2, item->Host );
    TableWidget->setItem( TableWidget->rowCount() - 1, 3, item->Port );
    TableWidget->setItem( TableWidget->rowCount() - 1, 4, item->Status );

    TableWidget->setSortingEnabled( isSortingEnabled );

    TableEntries.push_back( item );

    /* increase the number of listeners */
    ListenersRunning++;

    updateListenersRunningLabel( ListenersRunning );
}

auto HavocPageListener::updateListenersRunningLabel(
    int value
) const -> void {
    ActiveLabel->setText( QString( "Active: %1" ).arg( value ) );
}
