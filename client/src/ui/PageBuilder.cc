#include <Common.h>
#include <ui/PageBuilder.h>

HavocPageBuilder::HavocPageBuilder() {
    if ( objectName().isEmpty() )
        setObjectName( "PageBuilder" );

    gridLayout = new QGridLayout( this );
    gridLayout->setObjectName( "gridLayout" );

    LabelPayload = new QLabel( this );
    LabelPayload->setObjectName( "LabelPayload" );
    LabelPayload->setProperty( "HcLabelDisplay", "true" );

    ComboPayload = new QComboBox( this );
    ComboPayload->setObjectName(QString::fromUtf8("ComboPayload"));

    Spacer = new QSpacerItem( 834, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );

    StackedWidget = new QStackedWidget( this );
    StackedWidget->setObjectName( "stackedWidget" );

    gridLayout->addWidget( LabelPayload,  0, 0, 1, 1);
    gridLayout->addWidget( ComboPayload,  0, 1, 1, 1 );
    gridLayout->addItem( Spacer,          0, 2, 1, 1 );
    gridLayout->addWidget( StackedWidget, 1, 0, 1, 3 );

    retranslateUi();

    QMetaObject::connectSlotsByName( this );
}

auto HavocPageBuilder::retranslateUi() -> void {
    setWindowTitle( "PageBuilder" );
    LabelPayload->setText( "Payload: " );
}