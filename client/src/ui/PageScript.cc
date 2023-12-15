#include <ui/PageScript.h>

HavocPageScript::HavocPageScript() {
    if ( objectName().isEmpty() ) {
        setObjectName( "ScriptPage" );
    }

    gridLayout = new QGridLayout( this );
    gridLayout->setObjectName( "gridLayout" );

    Spacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );

    Splitter = new QSplitter( this );
    Splitter->setObjectName( "Splitter" );
    Splitter->setOrientation( Qt::Vertical );

    ScriptTableWidget = new QTableWidget( Splitter );
    ScriptTableWidget->setColumnCount( 1 );
    ScriptTableWidget->setHorizontalHeaderItem( 0, new QTableWidgetItem( "Path" ) );
    ScriptTableWidget->setObjectName( "ScriptTableWidget" );
    ScriptTableWidget->horizontalHeader()->setStretchLastSection( true );

    ScriptTabWidget = new QTabWidget( Splitter );
    ScriptTabWidget->setObjectName( "ScriptTabWidget" );
    ScriptTabWidget->addTab( TabScriptInterpreter, "Interpreter" );

    TabScriptInterpreter = new QWidget();
    TabScriptInterpreter->setObjectName( "TabScriptInterpreter" );

    Splitter->addWidget( ScriptTableWidget );
    Splitter->addWidget( ScriptTabWidget );

    ButtonLoadScript = new QPushButton( this );
    ButtonLoadScript->setObjectName( "ButtonLoadScript" );
    ButtonLoadScript->setProperty( "HxButton", "true" );

    label = new QLabel( this );
    label->setObjectName( "label" );
    label->setProperty( "HxLabelDisplay", "true" );

    gridLayout->addItem( Spacer, 0, 2, 1, 1 );
    gridLayout->addWidget( Splitter, 1, 0, 1, 4 );
    gridLayout->addWidget( label, 0, 3, 1, 1 );
    gridLayout->addWidget( ButtonLoadScript, 0, 0, 1, 2 );

    retranslateUi();

    QMetaObject::connectSlotsByName( this );
}

auto HavocPageScript::retranslateUi() -> void {
    setWindowTitle( "ScriptPage" );
    ButtonLoadScript->setText( "Load Script" );
    label->setText( "Scripts loaded: 0" );
}