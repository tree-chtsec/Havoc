#include <ui/DialogListener.h>

HavocListener::HavocListener() {
    if ( objectName().isEmpty() ) {
        setObjectName( QString::fromUtf8( "HavocListener" ) );
    }

    resize( 606, 649 );

    gridLayout = new QGridLayout( this );
    gridLayout->setObjectName( QString::fromUtf8( "gridLayout" ) );

    LabelListenerName = new QLabel( this );
    LabelListenerName->setObjectName( QString::fromUtf8( "LabelListenerName" ) );

    InputListenerName = new QLineEdit( this );
    InputListenerName->setObjectName( QString::fromUtf8( "InputListenerName" ) );

    LabelProtocol = new QLabel( this );
    LabelProtocol->setObjectName( QString::fromUtf8( "LabelProtocol" ) );

    ComboProtocol = new QComboBox( this );
    ComboProtocol->setObjectName( QString::fromUtf8( "ComboProtocol" ) );

    GroupOptions = new QGroupBox( this );
    GroupOptions->setObjectName( QString::fromUtf8( "GroupOptions" ) );

    gridLayout->addWidget( LabelListenerName, 0, 0, 1, 1 );
    gridLayout->addWidget( InputListenerName, 0, 1, 1, 1 );
    gridLayout->addWidget( LabelProtocol,     1, 0, 1, 1 );
    gridLayout->addWidget( ComboProtocol,     1, 1, 1, 1 );
    gridLayout->addWidget( GroupOptions,      2, 0, 1, 2 );

    retranslateUi();

    QMetaObject::connectSlotsByName( this );
}

auto HavocListener::retranslateUi() -> void {
    setWindowTitle( "Listener" );
    LabelListenerName->setText( "Listener Name:" );
    LabelProtocol->setText( "Protocol:" );
    GroupOptions->setTitle( QString() );
}

auto HavocListener::addProtocol(
    const json& data
) -> void {
    spdlog::info( "add protocol: {}", data.dump() );
}

auto HavocListener::getOptions() -> json {
    return {};
}
