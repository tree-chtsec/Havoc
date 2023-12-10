#include <Havoc.h>
#include <ui/DialogListener.h>

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QTextEdit>
#include <QMetaMethod>
#include <QRegularExpressionValidator>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE

class HxWidgetFile : public QWidget {
    QString      FilePath      = {};

public:
    QGridLayout* gridLayout    = nullptr;
    QLabel*      LabelFilePath = nullptr;
    QPushButton* ButtonAdd     = nullptr;
    QPushButton* ButtonRemove  = nullptr;

    explicit HxWidgetFile( QWidget* parent ) : QWidget( parent )
    {
        auto buttonSizeMax = QSize( 70, 16777215 );
        auto buttonSizeMin = QSize( 70, 0 );

        setContentsMargins( 0, 0, 0, 0 );

        gridLayout = new QGridLayout( this );
        gridLayout->setObjectName( "gridLayout" );
        gridLayout->setContentsMargins( 0, 0, 0, 0 );

        LabelFilePath = new QLabel( this );
        LabelFilePath->setObjectName( "LabelFilePath" );
        LabelFilePath->setProperty( "labelDisplay", "true" );

        ButtonAdd = new QPushButton( this );
        ButtonAdd->setObjectName( "ButtonAdd" );
        ButtonAdd->setMaximumSize( buttonSizeMax );
        ButtonAdd->setMinimumSize( buttonSizeMin );
        ButtonAdd->setProperty( "ClickButton", "true" );

        ButtonRemove = new QPushButton( this );
        ButtonRemove->setObjectName( "ButtonRemove" );
        ButtonRemove->setMaximumSize( buttonSizeMax );
        ButtonRemove->setMinimumSize( buttonSizeMin );
        ButtonRemove->setProperty( "ClickButton", "true" );

        gridLayout->addWidget( LabelFilePath, 0, 0, 1, 1 );
        gridLayout->addWidget( ButtonAdd,     0, 1, 1, 1 );
        gridLayout->addWidget( ButtonRemove,  0, 2, 1, 1 );

        retranslateUi();

        connect( ButtonAdd,    &QPushButton::clicked, this, &HxWidgetFile::buttonAdd    );
        connect( ButtonRemove, &QPushButton::clicked, this, &HxWidgetFile::buttonRemove );

        QMetaObject::connectSlotsByName( this );
    }

    auto buttonAdd() -> void {
        auto FileDialog = new QFileDialog;

        FileDialog->setStyleSheet( Havoc->getStyleSheet() );
        FileDialog->setAcceptMode( QFileDialog::AcceptOpen );

        if ( FileDialog->exec() == QFileDialog::Accepted ) {
            FilePath = FileDialog->selectedUrls().value( 0 ).toLocalFile();
            LabelFilePath->setText( FilePath );
        }

        delete FileDialog;
    }

    auto buttonRemove() -> void {
        FilePath = "";
        LabelFilePath->setText( "(empty)" );
    }

    auto getFilePath() -> QString { return FilePath; }

    auto retranslateUi( ) -> void {
        setStyleSheet( Havoc->getStyleSheet() );
        LabelFilePath->setText( "(empty)" );
        ButtonAdd->setText( "Add" );
        ButtonRemove->setText( "Remove" );
    }
};

class HxWidgetList : public QWidget
{
public:
    QGridLayout*            gridLayout  = nullptr;
    QPushButton*            ButtonAdd   = nullptr;
    QPushButton*            ButtonClear = nullptr;
    QGroupBox*              ListWidget  = nullptr;
    QFormLayout*            FormLayout  = nullptr;
    std::vector<QLineEdit*> InputList   = {};

    explicit HxWidgetList( QWidget* parent ) : QWidget( parent ) {

        auto buttonSize = QSize( 100, 25 );

        setContentsMargins( 0, 0, 0, 0 );

        gridLayout = new QGridLayout( this );
        gridLayout->setObjectName( "gridLayout" );
        gridLayout->setContentsMargins( 0, 0, 0, 0 );

        ListWidget = new QGroupBox( this );
        ListWidget->setObjectName( "ListWidget" );
        FormLayout = new QFormLayout( ListWidget );

        ButtonAdd = new QPushButton( this );
        ButtonAdd->setObjectName( "ButtonAdd" );
        ButtonAdd->setMaximumSize( buttonSize );
        ButtonAdd->setMinimumSize( buttonSize );
        ButtonAdd->setStyleSheet( "margin-top: 2px" );
        ButtonAdd->setProperty( "ClickButton", "true" );

        ButtonClear = new QPushButton( this );
        ButtonClear->setObjectName( "ButtonClear" );
        ButtonClear->setMaximumSize( buttonSize );
        ButtonClear->setMinimumSize( buttonSize );
        ButtonClear->setProperty( "ClickButton", "true" );

        gridLayout->addWidget( ListWidget,  0, 0, 3, 1 );
        gridLayout->addWidget( ButtonAdd,   0, 1, 1, 1 );
        gridLayout->addWidget( ButtonClear, 1, 1, 1, 1 );

        retranslateUi( );

        connect( ButtonAdd,   &QPushButton::clicked, this, &HxWidgetList::buttonAdd   );
        connect( ButtonClear, &QPushButton::clicked, this, &HxWidgetList::buttonClear );

        QMetaObject::connectSlotsByName( this );
    }

    auto addListValue(
        const std::string& value
    ) {
        auto Item = new QLineEdit;

        Item->setFocus();
        Item->setText( value.c_str() );

        FormLayout->setWidget( InputList.size(), QFormLayout::FieldRole, Item );

        InputList.push_back( Item );
    }

    auto getListStrings() -> std::vector<std::string> {
        auto array = std::vector<std::string>();

        for ( auto& input : InputList ) {
            array.push_back( input->text().toStdString() );
        }

        return array;
    }

    auto retranslateUi() -> void {
        setStyleSheet( Havoc->getStyleSheet() );
        ButtonAdd->setText( "Add" );
        ButtonClear->setText( "Clear" );
    }

private:
    auto buttonAdd() -> void {
        auto Item = new QLineEdit;

        Item->setFocus();

        FormLayout->setWidget( InputList.size(), QFormLayout::FieldRole, Item );

        InputList.push_back( Item );
    }

    auto buttonClear() -> void {
        for ( auto& item : InputList ) {
            delete item;
        }

        InputList.clear();
    }

};

QT_END_NAMESPACE

HavocListener::HavocListener() {
    if ( objectName().isEmpty() ) {
        setObjectName( QString::fromUtf8( "HavocListener" ) );
    }

    gridLayout = new QGridLayout( this );
    gridLayout->setObjectName( "gridLayout" );

    LabelName = new QLabel( this );
    LabelName->setObjectName( "LabelName" );

    InputName = new QLineEdit( this );
    InputName->setObjectName( QString::fromUtf8( "InputName" ) );

    LabelProtocol = new QLabel( this );
    LabelProtocol->setObjectName( QString::fromUtf8( "LabelProtocol" ) );

    ComboProtocol = new QComboBox( this );
    ComboProtocol->setObjectName( QString::fromUtf8( "ComboProtocol" ) );

    StackedProtocols = new QStackedWidget( this );
    StackedProtocols->setObjectName( QString::fromUtf8( "StackedProtocols" ) );
    StackedProtocols->setProperty( "protocol", "true" );

    ButtonSave = new QPushButton( this );
    ButtonSave->setObjectName( "ButtonClose" );
    ButtonSave->setProperty( "ClickButton", "true" );

    ButtonClose = new QPushButton( this );
    ButtonClose->setObjectName( "ButtonClose" );
    ButtonClose->setProperty( "ClickButton", "true" );

    gridLayout->addWidget( LabelName,        0, 0, 1, 1 );
    gridLayout->addWidget( InputName,        0, 1, 1, 5 );
    gridLayout->addWidget( LabelProtocol,    1, 0, 1, 1 );
    gridLayout->addWidget( ComboProtocol,    1, 1, 1, 5 );
    gridLayout->addWidget( StackedProtocols, 2, 0, 1, 6 );

    horizontal[ 0 ] = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontal[ 1 ] = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontal[ 2 ] = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontal[ 3 ] = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );

    gridLayout->addWidget( ButtonSave,  3, 2, 1, 1 );
    gridLayout->addWidget( ButtonClose, 3, 3, 1, 1 );

    gridLayout->addItem( horizontal[ 0 ], 3, 0, 1, 1 );
    gridLayout->addItem( horizontal[ 2 ], 3, 1, 1, 1 );
    gridLayout->addItem( horizontal[ 3 ], 3, 4, 1, 1 );
    gridLayout->addItem( horizontal[ 1 ], 3, 5, 1, 1 );

    retranslateUi();

    QMetaObject::connectSlotsByName( this );

    connect( ComboProtocol, &QComboBox::currentTextChanged, this, &HavocListener::changeProtocol );
    connect( ButtonSave,  &QPushButton::clicked, this, &HavocListener::save );
    connect( ButtonClose, &QPushButton::clicked, this, [&]() {
        State = Closed;
        close();
    } );

    retranslateUi();

    QMetaObject::connectSlotsByName( this );
}

auto HavocListener::retranslateUi() -> void {
    setStyleSheet( Havoc->getStyleSheet() );
    setWindowTitle( "Listener" );

    LabelName->setText( "Name:      " );
    LabelProtocol->setText( "Protocol:  " );
    ButtonSave->setText( "Save" );
    ButtonClose->setText( "Close" );
}

auto HavocListener::addProtocol(
    const json& data
) -> void {
    auto protocol = Protocol();
    auto handler  = json();
    auto name     = std::string();
    auto type     = std::string();

    spdlog::debug( "add protocol: {}", data.dump() );

    if ( ! data.contains( "data" ) ) {
        spdlog::error( "failed to add protocol: \"data\" field not found" );
        return;
    }

    handler = data[ "data" ].get<json>();

    if ( ! handler.contains( "protocol" ) ) {
        spdlog::error( "failed to add protocol: \"protocol\" field not found" );
        return;
    }

    if ( ! handler[ "protocol" ].is_string() ) {
        spdlog::error( "failed to add protocol: \"protocol\" field is not string" );
        return;
    }

    protocol.type = QString( handler[ "protocol" ].get<std::string>().c_str() );
    protocol.data = handler;
    protocol.tabs = {};

    if ( ! handler.contains( "tabs" ) ) {
        spdlog::error( "failed to add protocol: \"tabs\" field not found" );
        return;
    }

    if ( ! handler[ "tabs" ].is_array() ) {
        spdlog::error( "failed to add protocol: \"tabs\" field is not array" );
        return;
    }

    if ( handler.contains( "resize" ) ) {
        if ( handler[ "resize" ].is_array() ) {
            auto array = handler[ "resize" ].get<std::vector<int>>();
            protocol.resize = QSize(
                array[ 0 ],
                array[ 1 ]
            );
        } else {
            spdlog::error( "failed to add protocol: \"resize\" field is not array" );
        }
    }

    insertPage( protocol );

    Protocols.push_back( protocol );

    ComboProtocol->addItem( protocol.type );
}

auto HavocListener::insertPage(
    Protocol& protocol
) -> void {

    protocol.page = new QWidget();
    protocol.page->setObjectName( "protocol.page" );

    protocol.layout = new QGridLayout( protocol.page );
    protocol.layout->setObjectName( "protocol.layout" );
    protocol.layout->setContentsMargins( 0, 0, 0, 0 );

    protocol.tabWidget = new QTabWidget( protocol.page );
    protocol.tabWidget->setObjectName( "protocol.tabWidget" );
    protocol.tabWidget->tabBar()->setProperty( "protocol", "true" );
    protocol.tabWidget->tabBar()->setExpanding( true );
    protocol.tabWidget->tabBar()->setDocumentMode( true );

    protocol.layout->addWidget( protocol.tabWidget, 0, 0, 1, 1 );

    StackedProtocols->addWidget( protocol.page );

    for ( auto& tab : protocol.data[ "tabs" ].get<std::vector<json>>() ) {
        auto name    = std::string();
        auto widgets = std::vector<json>();

        if ( tab.contains( "name" ) ) {
            if ( tab[ "name" ].is_string() ) {
                name = tab[ "name" ].get<std::string>();
            } else {
                spdlog::error( "failed to add protocol: \"protocol.tab.name\" field is not string" );
                return;
            }
        } else {
            spdlog::error( "failed to add protocol: \"protocol.tab.name\" field not found" );
            return;
        }

        if ( tab.contains( "widgets" ) ) {
            if ( tab[ "widgets" ].is_array() ) {
                widgets = tab[ "widgets" ].get<std::vector<json>>();
            } else {
                spdlog::error( "failed to add protocol: \"protocol.tab.widgets\" field is not array" );
                return;
            }
        }

        insertTab( protocol, name, widgets );
    }

}

auto HavocListener::insertTab(
    Protocol&          protocol,
    const std::string& name,
    std::vector<json>& widgets
) -> void {
    auto option = ProtclOption();
    auto tab    = ProtclTabOption();

    tab.name = name;
    tab.tab  = new QWidget;
    tab.tab->setProperty( "dark", "true" );
    tab.tab->setProperty( "custom-widget", "true" );

    tab.layout = new QGridLayout( tab.tab );

    for ( auto& widget : widgets ) {

        spdlog::debug( "widget -> {}", widget.dump() );

        option.option = widget;

        if ( widget.contains( "name" ) ) {
            if ( widget[ "name" ].is_string() ) {
                option.name = widget[ "name" ].get<std::string>();
            } else {
                spdlog::error( "failed to add protocol: \"name\" field is not string" );
                return;
            }
        } else {
            spdlog::error( "failed to add protocol: \"name\" field not found" );
            return;
        }

        if ( widget.contains( "type" ) ) {
            if ( widget[ "type" ].is_string() ) {
                option.type = widget[ "type" ].get<std::string>();
            } else {
                spdlog::error( "failed to add protocol: \"type\" field is not string" );
                return;
            }
        } else {
            spdlog::error( "failed to add protocol: \"type\" field not found" );
            return;
        }

        if ( widget.contains( "optional" ) ) {
            if ( widget[ "optional" ].is_boolean() ) {
                option.optional = widget[ "optional" ].get<bool>();
            } else {
                spdlog::error( "failed to add protocol: \"optional\" field is not string" );
                return;
            }
        }

        if ( widget.contains( "event" ) ) {
            if ( widget[ "event" ].is_number_integer() ) {
                option.event = widget[ "event" ].get<int>();
            } else {
                spdlog::error( "failed to add protocol: \"event\" field is not int" );
                return;
            }
        }

        addOption( tab, option );

        tab.options.push_back( option );
    }

    protocol.tabWidget->addTab( tab.tab, tab.name.c_str() );
    protocol.tabs.push_back( tab );
}

auto HavocListener::addOption(
    ProtclTabOption& tab,
    ProtclOption&    option
) -> void {
    spdlog::debug( "option: [name: {}] [option: {}]", option.name, option.option.dump() );

    if ( option.type == ListenerWidgetTypeLabel ) {
        auto label = new QLabel( this );
        auto place = std::vector<int>();

        if ( option.option.contains( "name" ) ) {
            if ( option.option[ "name" ].is_string() ) {
                label->setObjectName( option.option[ "name" ].get<std::string>().c_str() );
            } else {
                spdlog::error( "failed to add option: \"name\" field is not string" );
                return;
            }
        } else {
            spdlog::error( "failed to add option: \"name\" field not found" );
            return;
        }

        if ( option.option.contains( "text" ) ) {
            if ( option.option[ "text" ].is_string() ) {
                label->setText( option.option[ "text" ].get<std::string>().c_str() );
            } else {
                spdlog::error( "failed to add option: \"text\" field is not string" );
                return;
            }
        } else {
            spdlog::error( "failed to add option: \"text\" field not found" );
            return;
        }

        if ( option.option.contains( "css" ) ) {
            if ( option.option[ "css" ].is_string() ) {
                label->setStyleSheet( option.option[ "css" ].get<std::string>().c_str() );
            } else {
                spdlog::error( "failed to add option: \"css\" field is not string" );
                return;
            }
        }

        if ( option.option.contains( "place" ) ) {
            if ( option.option[ "place" ].is_array() ) {
                place = option.option[ "place" ].get<std::vector<int>>();
            } else {
                spdlog::error( "failed to add option: \"place\" field is not an array" );
                return;
            }
        } else {
            spdlog::error( "failed to add option: \"place\" field not found" );
            return;
        }

        option.widget = label;

        tab.layout->addWidget( label, place[ 0 ], place[ 1 ], place[ 2 ], place[ 3 ] );
    } else if ( option.type == ListenerWidgetTypeList ) {
        auto list  = new HxWidgetList( this );
        auto place = std::vector<int>();

        if ( option.option.contains( "name" ) ) {
            if ( option.option[ "name" ].is_string() ) {
                list->setObjectName( option.option[ "name" ].get<std::string>().c_str() );
            } else {
                spdlog::error( "failed to add option: \"name\" field is not string" );
                return;
            }
        } else {
            spdlog::error( "failed to add option: \"name\" field not found" );
            return;
        }

        if ( option.option.contains( "value" ) ) {
            if ( option.option[ "value" ].is_array() ) {
                for ( auto& value : option.option[ "value" ].get<std::vector<std::string>>() ) {
                    list->addListValue( value );
                }
            }
        }

        if ( option.option.contains( "css" ) ) {
            if ( option.option[ "css" ].is_string() ) {
                list->setStyleSheet( option.option[ "css" ].get<std::string>().c_str() );
            } else {
                spdlog::error( "failed to add option: \"css\" field is not string" );
                return;
            }
        }

        if ( option.option.contains( "place" ) ) {
            if ( option.option[ "place" ].is_array() ) {
                place = option.option[ "place" ].get<std::vector<int>>();
            } else {
                spdlog::error( "failed to add option: \"place\" field is not an array" );
                return;
            }
        } else {
            spdlog::error( "failed to add option: \"place\" field not found" );
            return;
        }

        option.widget = list;

        tab.layout->addWidget( list, place[ 0 ], place[ 1 ], place[ 2 ], place[ 3 ] );
    } else if ( option.type == ListenerWidgetTypeToggle ) {

    } else if ( option.type == ListenerWidgetTypeInput ) {
        auto input = new QLineEdit( this );
        auto place = std::vector<int>();

        if ( option.option.contains( "name" ) ) {
            if ( option.option[ "name" ].is_string() ) {
                input->setObjectName( option.option[ "name" ].get<std::string>().c_str() );
            } else {
                spdlog::error( "failed to add option: \"name\" field is not string" );
                return;
            }
        } else {
            spdlog::error( "failed to add option: \"name\" field not found" );
            return;
        }

        if ( option.option.contains( "value" ) ) {
            if ( option.option[ "value" ].is_string() ) {
                input->setText( option.option[ "value" ].get<std::string>().c_str() );
            } else {
                spdlog::error( "failed to add option: \"value\" field is not string" );
                return;
            }
        }

        if ( option.option.contains( "regex" ) ) {
            if ( option.option[ "regex" ].is_string() ) {
                input->setValidator( new QRegularExpressionValidator(
                    QRegularExpression( option.option[ "regex" ].get<std::string>().c_str() ),
                    input
                ) );
            } else {
                spdlog::error( "failed to add option: \"regex\" field is not string" );
                return;
            }
        }

        if ( option.option.contains( "css" ) ) {
            if ( option.option[ "css" ].is_string() ) {
                input->setStyleSheet( option.option[ "css" ].get<std::string>().c_str() );
            } else {
                spdlog::error( "failed to add option: \"css\" field is not string" );
                return;
            }
        }

        if ( option.option.contains( "place" ) ) {
            if ( option.option[ "place" ].is_array() ) {
                place = option.option[ "place" ].get<std::vector<int>>();
            } else {
                spdlog::error( "failed to add option: \"place\" field is not an array" );
                return;
            }
        } else {
            spdlog::error( "failed to add option: \"place\" field not found" );
            return;
        }

        option.widget = input;

        tab.layout->addWidget( input, place[ 0 ], place[ 1 ], place[ 2 ], place[ 3 ] );
    } else if ( option.type == ListenerWidgetTypeCombo ) {
        auto combo = new QComboBox( this );
        auto place = std::vector<int>();

        if ( option.option.contains( "name" ) ) {
            if ( option.option[ "name" ].is_string() ) {
                combo->setObjectName( option.option[ "name" ].get<std::string>().c_str() );
            } else {
                spdlog::error( "failed to add option: \"name\" field is not string" );
                return;
            }
        } else {
            spdlog::error( "failed to add option: \"name\" field not found" );
            return;
        }

        if ( option.option.contains( "items" ) ) {
            if ( option.option[ "items" ].is_array() ) {
                for ( auto& item : option.option[ "items" ].get<std::vector<std::string>>() ) {
                    combo->addItem( item.c_str() );
                }
            } else {
                spdlog::error( "failed to add option: \"items\" field is not string" );
                return;
            }
        }

        if ( option.option.contains( "css" ) ) {
            if ( option.option[ "css" ].is_string() ) {
                combo->setStyleSheet( option.option[ "css" ].get<std::string>().c_str() );
            } else {
                spdlog::error( "failed to add option: \"css\" field is not string" );
                return;
            }
        }

        if ( option.option.contains( "place" ) ) {
            if ( option.option[ "place" ].is_array() ) {
                place = option.option[ "place" ].get<std::vector<int>>();
            } else {
                spdlog::error( "failed to add option: \"place\" field is not an array" );
                return;
            }
        } else {
            spdlog::error( "failed to add option: \"place\" field not found" );
            return;
        }

        option.widget = combo;

        tab.layout->addWidget( combo, place[ 0 ], place[ 1 ], place[ 2 ], place[ 3 ] );
    } else if ( option.type == ListenerWidgetTypeText ) {
        auto text  = new QTextEdit;
        auto place = std::vector<int>();

        if ( option.option.contains( "name" ) ) {
            if ( option.option[ "name" ].is_string() ) {
                text->setObjectName( option.option[ "name" ].get<std::string>().c_str() );
            } else {
                spdlog::error( "failed to add option: \"name\" field is not string" );
                return;
            }
        } else {
            spdlog::error( "failed to add option: \"name\" field not found" );
            return;
        }

        if ( option.option.contains( "readonly" ) ) {
            if ( option.option[ "readonly" ].is_boolean() ) {
                text->setReadOnly( option.option[ "readonly" ].get<bool>() );
            } else {
                spdlog::error( "failed to add option: \"readonly\" field is not boolean" );
                return;
            }
        }

        if ( option.option.contains( "css" ) ) {
            if ( option.option[ "css" ].is_string() ) {
                text->setStyleSheet( option.option[ "css" ].get<std::string>().c_str() );
            } else {
                spdlog::error( "failed to add option: \"css\" field is not string" );
                return;
            }
        }

        if ( option.option.contains( "place" ) ) {
            if ( option.option[ "place" ].is_array() ) {
                place = option.option[ "place" ].get<std::vector<int>>();
            } else {
                spdlog::error( "failed to add option: \"place\" field is not an array" );
                return;
            }
        } else {
            spdlog::error( "failed to add option: \"place\" field not found" );
            return;
        }

        option.widget = text;

        tab.layout->addWidget( text, place[ 0 ], place[ 1 ], place[ 2 ], place[ 3 ] );
    } else if ( option.type == ListenerWidgetTypeSpacer ) {
        auto spacer   = ( QSpacerItem* ) nullptr;
        auto place    = std::vector<int>();
        auto vertical = false;

        if ( option.option.contains( "place" ) ) {
            if ( option.option[ "place" ].is_array() ) {
                place = option.option[ "place" ].get<std::vector<int>>();
            } else {
                spdlog::error( "failed to add option: \"place\" field is not an array" );
                return;
            }
        } else {
            spdlog::error( "failed to add option: \"place\" field not found" );
            return;
        }

        if ( option.option.contains( "vertical" ) ) {
            if ( option.option[ "vertical" ].is_boolean() ) {
                vertical = option.option[ "vertical" ].get<bool>();
            } else {
                spdlog::error( "failed to add option: \"vertical\" field is not a boolean" );
                return;
            }
        }

        if ( vertical ) {
            spacer = new QSpacerItem( 20, 125, QSizePolicy::Minimum, QSizePolicy::Expanding );
        } else {
            spacer = new QSpacerItem( 867, 0, QSizePolicy::Expanding, QSizePolicy::Minimum  );
        }

        option.widget = ( QWidget* ) spacer;

        tab.layout->addItem( spacer, place[ 0 ], place[ 1 ], place[ 2 ], place[ 3 ] );

    } else if ( option.type == ListenerWidgetTypeFile ) {
        auto file  = new HxWidgetFile( this );
        auto place = std::vector<int>();

        option.widget = file;

        if ( option.option.contains( "name" ) ) {
            if ( option.option[ "name" ].is_string() ) {
                file->setObjectName( option.option[ "name" ].get<std::string>().c_str() );
            } else {
                spdlog::error( "failed to add option: \"name\" field is not string" );
                return;
            }
        } else {
            spdlog::error( "failed to add option: \"name\" field not found" );
            return;
        }

        if ( option.option.contains( "css" ) ) {
            if ( option.option[ "css" ].is_string() ) {
                file->setStyleSheet( option.option[ "css" ].get<std::string>().c_str() );
            } else {
                spdlog::error( "failed to add option: \"css\" field is not string" );
                return;
            }
        }

        if ( option.option.contains( "place" ) ) {
            if ( option.option[ "place" ].is_array() ) {
                place = option.option[ "place" ].get<std::vector<int>>();
            } else {
                spdlog::error( "failed to add option: \"place\" field is not an array" );
                return;
            }
        } else {
            spdlog::error( "failed to add option: \"place\" field not found" );
            return;
        }

        tab.layout->addWidget( file, place[ 0 ], place[ 1 ], place[ 2 ], place[ 3 ] );
    } else if ( option.type == ListenerWidgetTypeButton ) {
        auto button  = new QPushButton( this );
        auto place   = std::vector<int>();
        auto objects = std::vector<std::string>();

        button->setProperty( "ClickButton", "true" );
        option.widget = button;

        if ( option.option.contains( "name" ) ) {
            if ( option.option[ "name" ].is_string() ) {
                button->setObjectName( option.option[ "name" ].get<std::string>().c_str() );
            } else {
                spdlog::error( "failed to add option: \"name\" field is not string" );
                return;
            }
        } else {
            spdlog::error( "failed to add option: \"name\" field not found" );
            return;
        }

        if ( option.option.contains( "text" ) ) {
            if ( option.option[ "text" ].is_string() ) {
                button->setText( option.option[ "text" ].get<std::string>().c_str() );
            } else {
                spdlog::error( "failed to add option: \"text\" field is not string" );
                return;
            }
        } else {
            spdlog::error( "failed to add option: \"text\" field not found" );
            return;
        }

        if ( option.option.contains( "css" ) ) {
            if ( option.option[ "css" ].is_string() ) {
                button->setStyleSheet( option.option[ "css" ].get<std::string>().c_str() );
            } else {
                spdlog::error( "failed to add option: \"css\" field is not string" );
                return;
            }
        }

        if ( option.option.contains( "place" ) ) {
            if ( option.option[ "place" ].is_array() ) {
                place = option.option[ "place" ].get<std::vector<int>>();
            } else {
                spdlog::error( "failed to add option: \"place\" field is not an array" );
                return;
            }
        } else {
            spdlog::error( "failed to add option: \"place\" field not found" );
            return;
        }

        if ( option.option.contains( "events" ) ) {
            if ( option.option[ "events" ].is_array() ) {
                connect( button, &QPushButton::clicked, this, [&]() {
                    auto option  = getOption( sender()->objectName().toStdString() );
                    auto widgets = json::array();

                    for ( auto& event : option->option[ "events" ].get<std::vector<std::string>>() ) {
                        auto v = json();
                        auto o = getOption( event );

                        if ( ! o ) continue;

                        if ( o->type == ListenerWidgetTypeLabel ) {
                            v = ( ( QLabel* ) o->widget )->text().toStdString();
                        } else if ( o->type == ListenerWidgetTypeInput ) {
                            v = ( ( QLineEdit* ) o->widget )->text().toStdString();
                        } else if ( o->type == ListenerWidgetTypeCombo ) {
                            v = ( ( QComboBox* ) o->widget )->currentText().toStdString();
                        } else if ( o->type == ListenerWidgetTypeList ) {
                            v = ( ( HxWidgetList* ) o->widget )->getListStrings();
                        } else if ( o->type == ListenerWidgetTypeToggle ) {
                            v = json();
                        } else if ( o->type == ListenerWidgetTypeFile ) {
                            v = ( ( HxWidgetFile* ) o->widget )->getFilePath().toStdString();
                        } else if ( o->type == ListenerWidgetTypeText ) {
                            v = ( ( QTextEdit* ) o->widget )->toPlainText().toStdString();
                        }

                        widgets.push_back( { o->name, v } );
                    }

                    eventProcess( json {
                        { option->name, widgets }
                    } );
                } );
            } else {
                spdlog::error( "failed to add option: \"events\" field is not an array" );
                return;
            }
        } else {
            spdlog::error( "failed to add option: \"events\" field not found" );
            return;
        }

        tab.layout->addWidget( button, place[ 0 ], place[ 1 ], place[ 2 ], place[ 3 ] );
    } else {
        spdlog::debug( "[error] option.type \"{}\" not found", option.type );
    }

}

auto HavocListener::getOption(
    const std::string& name
) -> ProtclOption* {
    auto index = StackedProtocols->currentIndex();

    if ( Protocols.size() <= index ) {
        return nullptr;
    }

    for ( auto& tab : Protocols[ index ].tabs ) {
        for ( auto & option : tab.options ) {
            if ( option.name == name ) {
                return & option;
            }
        }
    }

    return nullptr;
}

auto HavocListener::setOption(
    const ProtclOption* option,
    const json&         values
) -> void {
    if ( ! option ) {
        return;
    }

    if ( option->type == ListenerWidgetTypeLabel ) {
        ( ( QLabel* ) option->widget )->setText( values.get<std::string>().c_str() );
    } else if ( option->type == ListenerWidgetTypeInput ) {
        ( ( QLineEdit* ) option->widget )->setText( values.get<std::string>().c_str() );
    } else if ( option->type == ListenerWidgetTypeCombo ) {
        // TODO: handle it
    } else if ( option->type == ListenerWidgetTypeList ) {
        // TODO: handle it
    } else if ( option->type == ListenerWidgetTypeToggle ) {
        // TODO: handle it
    } else if ( option->type == ListenerWidgetTypeFile ) {
        // TODO: handle it
    } else if ( option->type == ListenerWidgetTypeText ) {
        ( ( QTextEdit* ) option->widget )->setText( values.get<std::string>().c_str() );
    }
}

auto HavocListener::getOptions() -> json {
    auto index    = StackedProtocols->currentIndex();
    auto data     = json();
    auto listener = json();

    if ( Protocols.size() <= index ) {
        return {};
    }

    for ( auto& tab : Protocols[ index ].tabs ) {
        data[ tab.name ] = json();

        for ( auto& option : tab.options ) {
            if ( option.type == ListenerWidgetTypeLabel ) {
                data[ tab.name ][ option.name ] = ( ( QLabel* ) option.widget )->text().toStdString();
            } else if ( option.type == ListenerWidgetTypeInput ) {
                data[ tab.name ][ option.name ] = ( ( QLineEdit* ) option.widget )->text().toStdString();
            } else if ( option.type == ListenerWidgetTypeCombo ) {
                data[ tab.name ][ option.name ] = ( ( QComboBox* ) option.widget )->currentText().toStdString();
            } else if ( option.type == ListenerWidgetTypeList ) {
                data[ tab.name ][ option.name ] = ( ( HxWidgetList* ) option.widget )->getListStrings();
            } else if ( option.type == ListenerWidgetTypeToggle ) {
                data[ tab.name ][ option.name ] = json();
            } else if ( option.type == ListenerWidgetTypeFile ) {
                if ( ! ( ( HxWidgetFile* ) option.widget )->getFilePath().isEmpty() ) {
                    data[ tab.name ][ option.name ] = Helper::FileRead( ( ( HxWidgetFile* ) option.widget )->getFilePath() ).toBase64().toStdString();
                }
            } else if ( option.type == ListenerWidgetTypeText ) {
                data[ tab.name ][ option.name ] = ( ( QTextEdit* ) option.widget )->toPlainText().toStdString();
            }
        }
    }

    listener[ "name" ]     = InputName->text().toStdString();
    listener[ "protocol" ] = ComboProtocol->currentText().toStdString();
    listener[ "data" ]     = data;

    return listener;
}

auto HavocListener::changeProtocol(
    const QString &text
) -> void {
    spdlog::debug( "changeProtocol( \"{}\" );", text.toStdString() );

    for ( int i = 0; i < Protocols.size(); i++ ) {
        if ( Protocols[ i ].type.compare( text ) == 0 ) {
            resize( Protocols[ i ].resize );
            StackedProtocols->setCurrentIndex( i );
            break;
        }
    }
}

auto HavocListener::getCloseState() -> ListenerState { return State; }
auto HavocListener::start()         -> void { ComboProtocol->setCurrentIndex( 0 ); exec(); }

auto HavocListener::save() -> void {
    auto Result = httplib::Result();
    auto data   = json();

    if ( ! sanityCheckOptions() ) {
        return;
    }

    spdlog::debug( "options -> {}", getOptions().dump() );

    State = Error;

    if ( ( Result = Havoc->ApiSend( "/api/listener/start", getOptions() ) ) ) {
        if ( Result->status != 200 ) {
            if ( ! Result->body.empty() ) {
                if ( ( data = json::parse( Result->body ) ).is_discarded() ) {
                    goto InvalidServerResponseError;
                }

                if ( ! data.contains( "error" ) ) {
                    goto InvalidServerResponseError;
                }

                if ( ! data[ "error" ].is_string() ) {
                    goto InvalidServerResponseError;
                }

                Helper::MessageBox(
                    QMessageBox::Critical,
                    "Listener failure",
                    QString( "Failed to start listener: %1" ).arg( data[ "error" ].get<std::string>().c_str() ).toStdString()
                );

                State = Error;
                close();
                return;
            } else {
                goto InvalidServerResponseError;
            }
        } else {
            State = Saved;
            close();
            return;
        }
    }


InvalidServerResponseError:
    Helper::MessageBox(
        QMessageBox::Critical,
        "Listener failure",
        "Failed to start listener: Invalid response from the server"
    );

    close();
}

auto HavocListener::sanityCheckOptions() -> bool {
    if ( InputName->text().isEmpty() ) {
        Helper::MessageBox(
            QMessageBox::Critical,
            "Listener failure",
            "Invalid option: Name is emtpy"
        );
        return false;
    }

    if ( ComboProtocol->currentText().isEmpty() && Protocols.empty() ) {
        Helper::MessageBox(
            QMessageBox::Critical,
            "Listener failure",
            "Invalid option: Protocol is emtpy"
        );
        return false;
    }

    if ( Protocols.size() <= StackedProtocols->currentIndex() ) {
        Helper::MessageBox(
            QMessageBox::Critical,
            "Listener failure",
            "Invalid option: Protocol is out of index"
        );
        return false;
    }

    for ( auto& tab : getCurrentProtocol()->tabs ) {
        for ( auto& option : tab.options ) {
            if ( option.type == ListenerWidgetTypeInput && ! option.optional ) {
                if ( ( ( QLineEdit* ) option.widget )->text().isEmpty() ) {
                    Helper::MessageBox(
                        QMessageBox::Critical,
                        "Listener failure",
                        QString( "Invalid option: %1 is empty" ).arg( option.name.c_str() ).toStdString()
                    );
                    return false;
                }
            } else if ( option.type == ListenerWidgetTypeCombo && ! option.optional ) {
                if ( ( ( QComboBox* ) option.widget )->currentText().isEmpty() ) {
                    Helper::MessageBox(
                        QMessageBox::Critical,
                        "Listener failure",
                        QString( "Invalid option: %1 is empty" ).arg( option.name.c_str() ).toStdString()
                    );
                    return false;
                }
            } else if ( option.type == ListenerWidgetTypeList && ! option.optional ) {
                if ( ( ( HxWidgetList* ) option.widget )->getListStrings().empty() ) {
                    Helper::MessageBox(
                        QMessageBox::Critical,
                        "Listener failure",
                        QString( "Invalid option: %1 is empty" ).arg( option.name.c_str() ).toStdString()
                    );
                    return false;
                }
            } if ( option.type == ListenerWidgetTypeFile && ! option.optional ) {
                if ( ( ( HxWidgetFile* ) option.widget )->getFilePath().isEmpty() ) {
                    Helper::MessageBox(
                        QMessageBox::Critical,
                        "Listener failure",
                        QString( "Invalid option: %1 is empty" ).arg( option.name.c_str() ).toStdString()
                    );
                    return false;
                }
            } else if ( option.type == ListenerWidgetTypeText && ! option.optional ) {
                if ( ( ( QTextEdit* ) option.widget )->toPlainText().isEmpty() ) {
                    Helper::MessageBox(
                        QMessageBox::Critical,
                        "Listener failure",
                        QString( "Invalid option: %1 is empty" ).arg( option.name.c_str() ).toStdString()
                    );
                    return false;
                }
            }
        }
    }

    return true;
}

auto HavocListener::eventProcess(
    const json& data
) -> void {
    auto result = httplib::Result();
    auto body   = json();

    result = Havoc->ApiSend( "/api/listener/event", json {
        { "protocol", getCurrentProtocol()->type.toStdString() },
        { "events",   data }
    } );

    spdlog::debug( "result : {} {}", result->status, result->body );

    if ( ( body = json::parse( result->body ) ).is_discarded() ) {
        spdlog::debug( "[error] failed to process event response: invalid server response" );
        return;
    }

    for ( auto& item : body.items() ) {
        setOption( getOption( item.key() ), item.value() );
    }
}

auto HavocListener::getOptionValue(
    const std::string& name
) -> json {
    for ( auto& tab : getCurrentProtocol()->tabs ) {
        for ( auto& option : tab.options ) {
            if ( option.type == ListenerWidgetTypeLabel ) {
                return ( ( QLabel* ) option.widget )->text().toStdString();
            } else if ( option.type == ListenerWidgetTypeInput ) {
                return ( ( QLineEdit* ) option.widget )->text().toStdString();
            } else if ( option.type == ListenerWidgetTypeCombo ) {
                return ( ( QComboBox* ) option.widget )->currentText().toStdString();
            } else if ( option.type == ListenerWidgetTypeList ) {
                return ( ( HxWidgetList* ) option.widget )->getListStrings();
            } else if ( option.type == ListenerWidgetTypeToggle ) {
                return json();
            } else if ( option.type == ListenerWidgetTypeFile ) {
                return ( ( HxWidgetFile* ) option.widget )->getFilePath().toStdString();
            } else if ( option.type == ListenerWidgetTypeText ) {
                return ( ( QTextEdit* ) option.widget )->toPlainText().toStdString();
            }
        }
    }
}

auto HavocListener::getCurrentProtocol() -> Protocol* { return & Protocols[ StackedProtocols->currentIndex() ]; }
