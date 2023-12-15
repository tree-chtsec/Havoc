#include <ui/HxConsole.h>

HxConsole::HxConsole() {
    if ( objectName().isEmpty() ) {
        setObjectName( "HxConsole" );
    }

    gridLayout = new QGridLayout( this );
    gridLayout->setObjectName( "gridLayout" );

    LabelHeader = new QLabel( this );
    LabelHeader->setObjectName( "LabelHeader" );


    Console = new QTextEdit( this );
    Console->setObjectName( "Console" );

    LabelBottom = new QLabel( this );
    LabelBottom->setObjectName( "LabelBottom" );

    LabelInput = new QLabel( this );
    LabelInput->setObjectName( "LabelInput" );

    Input = new QLineEdit( this );
    Input->setObjectName( "Input" );

    gridLayout->addWidget( LabelHeader, 0, 0, 1, 2 );
    gridLayout->addWidget( Console,     1, 0, 1, 2 );
    gridLayout->addWidget( LabelBottom, 2, 0, 1, 2 );
    gridLayout->addWidget( LabelInput,  3, 0, 1, 1 );
    gridLayout->addWidget( Input,       3, 1, 1, 1 );

    QMetaObject::connectSlotsByName( this );
}

auto HxConsole::setHeaderLabel(
    const QString& text
) -> void {

}

auto HxConsole::setBottomLabel(
    const QString& text
) -> void {

}

auto HxConsole::setInputLabel(
    const QString& text
) -> void {

}