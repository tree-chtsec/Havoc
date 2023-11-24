#include <Havoc.h>
#include "ui/MainWindow.h"

/* Havoc side button hover over event. */
class Hover : public QObject
{
    QString* TabName = nullptr;

    void handleButtonSideEvent( const QString& Name, QPushButton* button, QObject* watched, QEvent* event ) {
        if ( watched == button ) {
            if ( event->type() == QEvent::HoverEnter ) {
                button->setIcon( QIcon( ":/icons/32px-" + Name + "-white" ) );
                button->setIconSize( QSize( 32, 32 ) );
            } else if ( event->type() == QEvent::HoverLeave ) {
                /* only apply icon if it's not used */
                if ( ! ( ( HavocButton* ) button )->currentlyUsed() ) {
                    button->setIcon( GrayScaleIcon( QImage( ":/icons/32px-" + Name + "-white" ) ) );
                    button->setIconSize( QSize( 32, 32 ) );
                }
            }
        }
    }

public:
    bool eventFilter( QObject* watched, QEvent* event ) override {
        handleButtonSideEvent( "connection", Havoc->MainWindows->ButtonAgents,    watched, event );
        handleButtonSideEvent( "listener",   Havoc->MainWindows->ButtonListeners, watched, event );
        handleButtonSideEvent( "payload",    Havoc->MainWindows->ButtonPayload,   watched, event );
        handleButtonSideEvent( "hosting",    Havoc->MainWindows->ButtonServer,    watched, event );
        handleButtonSideEvent( "script",     Havoc->MainWindows->ButtonScripts,   watched, event );
        handleButtonSideEvent( "settings",   Havoc->MainWindows->ButtonSettings,  watched, event );

        return false;
    }
};

HavocMainWindow::HavocMainWindow() {
    auto HoverEvent = new Hover;

    if ( objectName().isEmpty() ) {
        setObjectName( QString::fromUtf8( "HavocMainWindow" ) );
    }

    resize( 1124, 659 );

    MainWidget = new QWidget( this );
    MainWidget->setObjectName( QString::fromUtf8( "MainWidget" ) );

    gridLayout = new QGridLayout( MainWidget );
    gridLayout->setObjectName( QString::fromUtf8("gridLayout" ) );

    SideBarWidget = new QWidget( MainWidget );
    SideBarWidget->setObjectName( QString::fromUtf8( "SideBarWidget" ) );

    gridLayout_2 = new QGridLayout( SideBarWidget );
    gridLayout_2->setObjectName( QString::fromUtf8( "gridLayout_2" ) );

    ButtonHavoc = new HavocButton( SideBarWidget );
    ButtonHavoc->setObjectName( QString::fromUtf8( "SideHavoc" ) );
    ButtonHavoc->setIcon( QIcon( ":/icons/havoc-white" ) );
    ButtonHavoc->setIconSize( QSize( 40, 40 ) );
    ButtonHavoc->setMinimumSize( QSize( 0, 50 ) );

    ButtonAgents = new HavocButton( SideBarWidget );
    ButtonAgents->setObjectName( QString::fromUtf8( "SideButtonAgents" ) );
    ButtonAgents->setMinimumSize( QSize( 0, 50 ) );

    ButtonListeners = new HavocButton( SideBarWidget );
    ButtonListeners->setObjectName( QString::fromUtf8( "SideButtonListeners" ) );
    ButtonListeners->setMinimumSize( QSize( 0, 50 ) );

    ButtonPayload = new HavocButton( SideBarWidget );
    ButtonPayload->setObjectName( QString::fromUtf8( "SideButtonPayload" ) );
    ButtonPayload->setMinimumSize( QSize( 0, 50 ) );

    ButtonServer = new HavocButton( SideBarWidget );
    ButtonServer->setObjectName( QString::fromUtf8( "SideButtonServer" ) );
    ButtonServer->setMinimumSize( QSize( 0, 50 ) );

    ButtonScripts = new HavocButton( SideBarWidget );
    ButtonScripts->setObjectName( QString::fromUtf8( "SideButtonScripts" ) );
    ButtonScripts->setMinimumSize( QSize( 0, 50 ) );

    ButtonSettings  = new HavocButton( SideBarWidget );
    ButtonSettings->setObjectName( QString::fromUtf8( "SideButtonSettings" ) );
    ButtonSettings->setMinimumSize( QSize( 0, 50 ) );
    ButtonSettings->setStyleSheet( "margin-bottom: 5px" );

    Spacer = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );

    StackedWidget = new QStackedWidget( MainWidget );
    StackedWidget->setObjectName( QString::fromUtf8( "StackedWidget" ) );

    gridLayout_2->addWidget( ButtonHavoc,     0, 0, 1, 1 );
    gridLayout_2->addWidget( ButtonAgents,    1, 0, 1, 1 );
    gridLayout_2->addWidget( ButtonListeners, 2, 0, 1, 1 );
    gridLayout_2->addWidget( ButtonPayload,   3, 0, 1, 1 );
    gridLayout_2->addWidget( ButtonServer,    4, 0, 1, 1 );
    gridLayout_2->addWidget( ButtonScripts,   5, 0, 1, 1 );
    gridLayout_2->addItem( Spacer, 6, 0, 1, 1 );
    gridLayout_2->addWidget( ButtonSettings,  7, 0, 1, 1 );
    gridLayout_2->setContentsMargins( 0, 0, 0, 0 );

    gridLayout->addWidget( SideBarWidget, 0, 0, 1, 1 );
    gridLayout->addWidget( StackedWidget, 0, 1, 1, 1 );
    gridLayout->setContentsMargins( 0, 0, 0, 0 );

    /* create page objects */
    PageAgent    = new HavocPageAgent;
    PageListener = new HavocPageListener;
    PagePayload  = new QWidget;
    PageServer   = new QWidget;
    PageScripts  = new QWidget;
    PageSettings = new QWidget;

    /* add stacked pages to the GUI */
    StackedWidget->addWidget( PageAgent );
    StackedWidget->addWidget( PageListener );
    StackedWidget->addWidget( PagePayload );
    StackedWidget->addWidget( PageServer );
    StackedWidget->addWidget( PageScripts );
    StackedWidget->addWidget( PageSettings );

    setCentralWidget( MainWidget );

    connect( ButtonAgents, &QPushButton::clicked, this, [&] () {
        switchPageAgent();
    } );

    connect( ButtonListeners, &QPushButton::clicked, this, [&] () {
        switchPageListener();
    } );

    connect( ButtonPayload, &QPushButton::clicked, this, [&] () {
        switchPagePayload();
    } );

    connect( ButtonServer, &QPushButton::clicked, this, [&] () {
        switchPageServer();
    } );

    connect( ButtonScripts, &QPushButton::clicked, this, [&] () {
        switchPageScripts();
    } );

    connect( ButtonSettings, &QPushButton::clicked, this, [&] () {
        switchPageSettings();
    } );

    retranslateUi();
    unusedSideButtons();

    /* default button is the Agent */
    useSideButton( ButtonAgents, "connection" );

    /* Side panel stuff */
    ButtonAgents->installEventFilter( HoverEvent );
    ButtonListeners->installEventFilter( HoverEvent );
    ButtonPayload->installEventFilter( HoverEvent );
    ButtonServer->installEventFilter( HoverEvent );
    ButtonScripts->installEventFilter( HoverEvent );
    ButtonSettings->installEventFilter( HoverEvent );

    QMetaObject::connectSlotsByName( this );
}

auto HavocMainWindow::retranslateUi() -> void {
    setWindowTitle( QString( "Havoc [Version: %1 %2]" ).arg( HAVOC_VERSION ).arg( HAVOC_CODENAME ) );

}

auto HavocMainWindow::renderWindow() -> void {
    show();
}

auto HavocMainWindow::unusedSideButtons() -> void {
    ButtonAgents->setIcon( GrayScaleIcon( QImage( ":/icons/32px-connection-white" ) ) );
    ButtonAgents->setIconSize( QSize( 32, 32 ) );
    ButtonAgents->setToolTip( "Agents" );
    ButtonAgents->setUsed( false );

    ButtonListeners->setIcon( GrayScaleIcon( QImage( ":/icons/32px-listener-white" ) ) );
    ButtonListeners->setIconSize( QSize( 32, 32 ) );
    ButtonListeners->setToolTip( "Listeners" );
    ButtonListeners->setUsed( false );

    ButtonPayload->setIcon( GrayScaleIcon( QImage( ":/icons/32px-payload-white" ) ) );
    ButtonPayload->setIconSize( QSize( 32, 32 ) );
    ButtonPayload->setToolTip( "Payload" );
    ButtonPayload->setUsed( false );

    ButtonServer->setIcon( GrayScaleIcon( QImage( ":/icons/32px-hosting-white" ) ) );
    ButtonServer->setIconSize( QSize( 32, 32 ) );
    ButtonServer->setToolTip( "Server" );
    ButtonServer->setUsed( false );

    ButtonScripts->setIcon( GrayScaleIcon( QImage( ":/icons/32px-script-white" ) ) );
    ButtonScripts->setIconSize( QSize( 32, 32 ) );
    ButtonScripts->setToolTip( "Scripts" );
    ButtonScripts->setUsed( false );

    ButtonSettings->setIcon( GrayScaleIcon( QImage( ":/icons/32px-settings-white" ) ) );
    ButtonSettings->setIconSize( QSize( 32, 32 ) );
    ButtonSettings->setToolTip( "Settings" );
    ButtonSettings->setUsed( false );
}

auto HavocMainWindow::useSideButton(
    HavocButton*   button,
    const QString& name
) -> void {
    unusedSideButtons();

    button->setUsed( true );
    button->setIcon( QIcon( ":/icons/32px-" + name + "-white" ) );
    button->setIconSize( QSize( 32, 32 ) );
}

auto HavocMainWindow::switchPageAgent() -> void {
    useSideButton( ButtonAgents, "connection" );
    StackedWidget->setCurrentIndex( 0 );
}

auto HavocMainWindow::switchPageListener() -> void {
    useSideButton( ButtonListeners, "listener" );
    StackedWidget->setCurrentIndex( 1 );
}

auto HavocMainWindow::switchPagePayload() -> void {
    useSideButton( ButtonPayload, "payload" );
    StackedWidget->setCurrentIndex( 2 );
}

auto HavocMainWindow::switchPageServer() -> void {
    useSideButton( ButtonServer, "hosting" );
    StackedWidget->setCurrentIndex( 3 );
}

auto HavocMainWindow::switchPageScripts() -> void {
    useSideButton( ButtonScripts, "script" );
    StackedWidget->setCurrentIndex( 4 );
}

auto HavocMainWindow::switchPageSettings() -> void {
    useSideButton( ButtonSettings, "settings" );
    StackedWidget->setCurrentIndex( 5 );
}

HavocButton::HavocButton(
    QWidget *w
) : QPushButton( w ) {}

auto HavocButton::setUsed(
    bool state
) -> void {
    buttonUsed = state;
    setProperty( "HavocButtonUsed", buttonUsed ? "true" : "false" );
    style()->unpolish( this );
    style()->polish( this );
}

auto HavocButton::currentlyUsed() const -> bool { return buttonUsed; }

