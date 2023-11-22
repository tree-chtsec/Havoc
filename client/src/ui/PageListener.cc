#include <Havoc.h>

HavocPageListener::HavocPageListener() {
    if ( objectName().isEmpty() ) {
        setObjectName( QString::fromUtf8( "PageListener" ) );
    }

    setStyleSheet( Havoc->getStyleSheet() );

    gridLayout = new QGridLayout( this );
    gridLayout->setObjectName( QString::fromUtf8( "gridLayout" ) );

    ActiveLabel = new QLabel( this );
    ActiveLabel->setObjectName( QString::fromUtf8("LabelDisplayListenerActive" ) );

    horizontalSpacer  = new QSpacerItem( 1157, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    ButtonNewListener = new QPushButton( this );
    ButtonNewListener->setObjectName( QString::fromUtf8( "ButtonNewListener" ) );
    ButtonNewListener->setProperty( "ClickButton", "true" );

    Splitter = new QSplitter( this );
    Splitter->setObjectName( QString::fromUtf8( "Splitter" ) );
    Splitter->setOrientation( Qt::Vertical );

    TableWidget = new QTableWidget( Splitter );
    TableWidget->setObjectName( QString::fromUtf8( "TableWidget" ) );

    TabWidget = new QTabWidget( Splitter );
    TabWidget->setObjectName( QString::fromUtf8( "TabWidget" ) );

    Splitter->addWidget( TableWidget );
    Splitter->addWidget( TabWidget );

    gridLayout->addWidget( ActiveLabel, 0, 0, 1, 1 );
    gridLayout->addItem( horizontalSpacer, 0, 1, 1, 1 );
    gridLayout->addWidget( ButtonNewListener, 0, 2, 1, 1 );
    gridLayout->addWidget( Splitter, 1, 0, 1, 3 );

    retranslateUi();

    QMetaObject::connectSlotsByName( this );
}

auto HavocPageListener::retranslateUi() -> void {
    setWindowTitle( "PageListener" );
    ActiveLabel->setText( "Active: 0" );
    ButtonNewListener->setText( "New Listener" );
}