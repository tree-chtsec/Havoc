#include <Havoc.h>
#include <core/Helper.h>
#include <ui/Connect.h>

HavocConnect::HavocConnect() {
    if ( objectName().isEmpty() ) {
        setObjectName( QString::fromUtf8( "HavocConnect" ) );
    }

    resize( 600, 261 );
    setMinimumSize( QSize( 600, 0 ) );
    setMaximumSize( QSize( 800, 300 ) );

    horizontalLayout = new QHBoxLayout( this );
    ConnectionWidget = new QWidget( this );
    gridLayout       = new QGridLayout( ConnectionWidget );
    LabelHavoc       = new QLabel( ConnectionWidget );
    InputProfileName = new QLineEdit( ConnectionWidget );
    InputHost        = new QLineEdit( ConnectionWidget );
    InputPort        = new QLineEdit( ConnectionWidget );
    InputUsername    = new QLineEdit( ConnectionWidget );
    InputPassword    = new QLineEdit( ConnectionWidget );
    ButtonConnect    = new QPushButton( ConnectionWidget );
    ButtonAdd        = new QPushButton( ConnectionWidget );
    ListConnection   = new QListWidget( this );

    horizontalLayout->setObjectName( QString::fromUtf8( "horizontalLayout" ) );
    ConnectionWidget->setObjectName( QString::fromUtf8( "ConnectionWidget" ) );

    gridLayout->setObjectName( QString::fromUtf8( "gridLayout" ) );
    LabelHavoc->setObjectName( QString::fromUtf8( "LabelHavoc" ) );
    LabelHavoc->setMaximumSize( QSize( 200, 40 ) );

    InputProfileName->setObjectName( QString::fromUtf8( "InputProfileName" ) );
    InputProfileName->setMinimumSize( QSize( 0, 30 ) );

    InputHost->setObjectName( QString::fromUtf8( "InputHost" ) );
    InputHost->setMinimumSize( QSize( 0, 30 ) );

    InputPort->setObjectName( QString::fromUtf8( "InputPort" ) );
    InputPort->setMinimumSize( QSize( 0, 30 ) );
    InputPort->setValidator( new QRegularExpressionValidator( QRegularExpression( "[0-9]*" ), InputPort ) );

    InputUsername->setObjectName( QString::fromUtf8( "InputUsername" ) );
    InputUsername->setMinimumSize( QSize( 0, 30 ) );

    InputPassword->setObjectName( QString::fromUtf8( "InputPassword" ) );
    InputPassword->setMinimumSize( QSize( 0, 30 ) );

    ButtonConnect->setObjectName( QString::fromUtf8( "ButtonConnect" ) );
    ButtonConnect->setMinimumSize( QSize( 0, 30 ) );
    ButtonConnect->setProperty( "HcButton", "true" );

    ButtonAdd->setObjectName( QString::fromUtf8( "ButtonAdd" ) );
    ButtonAdd->setMinimumSize( QSize( 0, 30 ) );
    ButtonAdd->setProperty( "HcButton", "true" );

    ListConnection->setObjectName( QString::fromUtf8( "ListConnection" ) );
    ListConnection->setMaximumSize( QSize( 240, 16777215 ) );

    gridLayout->addWidget( LabelHavoc, 0, 0, 1, 2 );
    gridLayout->addWidget( InputProfileName, 1, 0, 1, 2 );
    gridLayout->addWidget( InputHost, 2, 0, 1, 1 );
    gridLayout->addWidget( InputPort, 2, 1, 1, 1 );
    gridLayout->addWidget( InputUsername, 3, 0, 1, 2 );
    gridLayout->addWidget( InputPassword, 4, 0, 1, 2 );
    gridLayout->addWidget( ButtonConnect, 5, 0, 1, 1 );
    gridLayout->addWidget( ButtonAdd, 5, 1, 1, 1 );

    horizontalLayout->addWidget( ConnectionWidget );
    horizontalLayout->addWidget( ListConnection );

    retranslateUi();

    /* add events to buttons */
    /* event when the "Connect" button is pressed/clicked.
    * is going to close the Connection dialog and set the "Connected" bool to true */
    QObject::connect( ButtonConnect, &QPushButton::clicked, this, [&] {
        /* check if every input is "correct" etc. */
        if ( sanityCheckInput() ) {
            PressedConnect = true;
            close();
        }
    } );

    QObject::connect( ButtonAdd, &QPushButton::clicked, this, [&] {
        InputProfileName->setText( "Server" );
        InputHost->setText( "127.0.0.1" );
        InputPort->setText( "40056" );
        InputUsername->setText( "5pider" );
        InputPassword->setText( "password1234" );
    } );

    QMetaObject::connectSlotsByName( this );
}

HavocConnect::~HavocConnect() {
    delete horizontalLayout;
    delete ConnectionWidget;
    delete gridLayout;
    delete LabelHavoc;
    delete InputProfileName;
    delete InputHost;
    delete InputPort;
    delete InputUsername;
    delete InputPassword;
    delete ButtonConnect;
    delete ButtonAdd;
    delete ListConnection;
}

void HavocConnect::retranslateUi() {
    setWindowTitle( QCoreApplication::translate( "HavocConnect", "Havoc Connect", nullptr ) );
    setStyleSheet( Havoc->getStyleSheet() );
    LabelHavoc->setText( QCoreApplication::translate( "HavocConnect", "<html><head/><body><p><span style=\" font-size:12pt;\">Havoc [ welcome back ]</span></p></body></html>", nullptr ) );
    InputProfileName->setPlaceholderText( QCoreApplication::translate( "HavocConnect", "Profile Name", nullptr ) );
    InputHost->setPlaceholderText( QCoreApplication::translate( "HavocConnect", "Host", nullptr ) );
    InputPort->setPlaceholderText( QCoreApplication::translate( "HavocConnect", "Port", nullptr ) );
    InputUsername->setPlaceholderText( QCoreApplication::translate( "HavocConnect", "Username", nullptr ) );
    InputPassword->setPlaceholderText( QCoreApplication::translate( "HavocConnect", "Password", nullptr ) );
    ButtonConnect->setText( QCoreApplication::translate( "HavocConnect", "CONNECT", nullptr ) );
    ButtonAdd->setText( QCoreApplication::translate( "HavocConnect", "ADD", nullptr ) );
}

auto HavocConnect::start() -> json {
    exec();

    if ( InputProfileName->text().isEmpty() ||
         InputHost->text().isEmpty()        ||
         InputPort->text().isEmpty()        ||
         InputUsername->text().isEmpty()    ||
         InputPassword->text().isEmpty()    ||
         ! PressedConnect
    ) {
        return {};
    }

    return {
        { "name",     InputProfileName->text().toStdString() },
        { "host",     InputHost->text().toStdString()        },
        { "port",     InputPort->text().toStdString()        },
        { "username", InputUsername->text().toStdString()    },
        { "password", InputPassword->text().toStdString()    },
    };
}

auto HavocConnect::sanityCheckInput() -> bool {

    if ( InputProfileName->text().isEmpty() ) {
        Helper::MessageBox( QMessageBox::Critical, "Profile Error", "Profile field is emtpy." );
        return false;
    }

    if ( InputHost->text().isEmpty() ) {
        Helper::MessageBox( QMessageBox::Critical, "Profile Error", "Host field is emtpy." );
        return false;
    }

    if ( InputPort->text().isEmpty() ) {
        Helper::MessageBox( QMessageBox::Critical, "Profile Error", "Port field is emtpy." );
        return false;
    }

    if ( InputUsername->text().isEmpty() ) {
        Helper::MessageBox( QMessageBox::Critical, "Profile Error", "Username field is emtpy." );
        return false;
    }

    if ( InputPassword->text().isEmpty() ) {
        Helper::MessageBox( QMessageBox::Critical, "Profile Error", "Password field is emtpy." );
        return false;
    }

    return true;
}

auto HavocConnect::pressedConnect() const -> bool { return PressedConnect; }



