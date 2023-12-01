#include <Havoc.h>
#include <ui/DialogListener.h>

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <QtWidgets/QFormLayout>

QT_BEGIN_NAMESPACE

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

        ButtonClear = new QPushButton( this );
        ButtonClear->setObjectName( "ButtonClear" );
        ButtonClear->setMaximumSize( buttonSize );
        ButtonClear->setMinimumSize( buttonSize );

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

    gridLayout->addWidget( LabelName,        0, 0, 1, 1 );
    gridLayout->addWidget( InputName,        0, 1, 1, 1 );
    gridLayout->addWidget( LabelProtocol,    1, 0, 1, 1 );
    gridLayout->addWidget( ComboProtocol,    1, 1, 1, 1 );
    gridLayout->addWidget( StackedProtocols, 2, 0, 1, 2 );

    retranslateUi();

    QMetaObject::connectSlotsByName( this );

    connect( ComboProtocol, &QComboBox::currentTextChanged, this, &HavocListener::changeProtocol );

    retranslateUi();

    QMetaObject::connectSlotsByName( this );
}

auto HavocListener::retranslateUi() -> void {
    setStyleSheet( Havoc->getStyleSheet() );
    setWindowTitle( "Listener" );

    LabelName->setText( "Name:      " );
    LabelProtocol->setText( "Protocol:  " );
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

        addOption( tab, option );

        tab.options.push_back( option );
    }

    protocol.tabWidget->addTab( tab.tab, tab.name.c_str() );
    protocol.tabs.push_back( tab );
}

auto HavocListener::getOptions() -> json {
    return {};
}

auto HavocListener::addOption(
    ProtclTabOption& tab,
    ProtclOption&    option
) -> void {
    spdlog::debug( "option: [name: {}] [option: {}]", option.name, option.option.dump() );

    if ( option.type == "label" ) {
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
    } else if ( option.type == "list" ) {
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
    } else if ( option.type == "toggle" ) {

    } else if ( option.type == "input" ) {
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
    } else if ( option.type == "combo" ) {
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
    } else if ( option.type == "text" ) {

    } else if ( option.type == "spacer" ) {
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
    } else {
        spdlog::debug( "[error] option.type \"{}\" not found", option.type );
    }

}

auto HavocListener::changeProtocol(
    const QString &text
) -> void {

    auto size = std::vector<int>();

    spdlog::debug( "changeProtocol( \"{}\" );", text.toStdString() );

    for ( int i = 0; i < Protocols.size(); i++ ) {
        if ( Protocols[ i ].type.compare( text ) == 0 ) {
            resize( Protocols[ i ].resize );
            StackedProtocols->setCurrentIndex( i );
            break;
        }
    }

    /*if ( Previous ) {
        for ( auto& option : Previous->options ) {
            delete option.widget;
        }
    }

    for ( auto & protocol : Protocols ) {
        if ( protocol.type.compare( text ) == 0 ) {

            if ( protocol.data.contains( "resize" ) ) {
                if ( protocol.data[ "resize" ].is_array() ) {
                    size = protocol.data[ "resize" ].get<std::vector<int>>();

                    spdlog::debug( "size: {} {}", size[0], size[1] );

                    resize( size[ 0 ], size[ 1 ] );
                }
            }

            //
            // add widgets fields to the group box
            //
            for ( auto & option : protocol.options ) {
                addOption( option );
            }

            Previous = & protocol;
            break;
        }
    }*/

}

auto HavocListener::start() -> void
{
    ComboProtocol->setCurrentIndex( 0 );

    exec();
}




