#include <Havoc.h>
#include <ui/HcListenerDialog.h>

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

class HcWidgetFile : public QWidget {
    QString      FilePath      = {};

public:
    QGridLayout* gridLayout    = nullptr;
    QLabel*      LabelFilePath = nullptr;
    QPushButton* ButtonAdd     = nullptr;
    QPushButton* ButtonRemove  = nullptr;

    explicit HcWidgetFile( QWidget* parent ) : QWidget( parent )
    {
        auto buttonSizeMax = QSize( 70, 16777215 );
        auto buttonSizeMin = QSize( 70, 0 );

        setContentsMargins( 0, 0, 0, 0 );

        gridLayout = new QGridLayout( this );
        gridLayout->setObjectName( "gridLayout" );
        gridLayout->setContentsMargins( 0, 0, 0, 0 );

        LabelFilePath = new QLabel( this );
        LabelFilePath->setObjectName( "LabelFilePath" );
        LabelFilePath->setProperty( "HcLabelDisplay", "true" );

        ButtonAdd = new QPushButton( this );
        ButtonAdd->setObjectName( "ButtonAdd" );
        ButtonAdd->setMaximumSize( buttonSizeMax );
        ButtonAdd->setMinimumSize( buttonSizeMin );
        ButtonAdd->setProperty( "HcButton", "true" );

        ButtonRemove = new QPushButton( this );
        ButtonRemove->setObjectName( "ButtonRemove" );
        ButtonRemove->setMaximumSize( buttonSizeMax );
        ButtonRemove->setMinimumSize( buttonSizeMin );
        ButtonRemove->setProperty( "HcButton", "true" );

        gridLayout->addWidget( LabelFilePath, 0, 0, 1, 1 );
        gridLayout->addWidget( ButtonAdd,     0, 1, 1, 1 );
        gridLayout->addWidget( ButtonRemove,  0, 2, 1, 1 );

        retranslateUi();

        connect( ButtonAdd,    &QPushButton::clicked, this, &HcWidgetFile::buttonAdd    );
        connect( ButtonRemove, &QPushButton::clicked, this, &HcWidgetFile::buttonRemove );

        QMetaObject::connectSlotsByName( this );
    }

    auto buttonAdd() -> void {
        auto FileDialog = new QFileDialog;

        FileDialog->setStyleSheet( Havoc->getStyleSheet() );
        FileDialog->setAcceptMode( QFileDialog::AcceptOpen );

        if ( FileDialog->exec() == QFileDialog::Accepted ) {
            FilePath = FileDialog->selectedFiles().value( 0 );
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

class HcWidgetList : public QWidget
{
public:
    QGridLayout*            gridLayout  = nullptr;
    QPushButton*            ButtonAdd   = nullptr;
    QPushButton*            ButtonClear = nullptr;
    QGroupBox*              ListWidget  = nullptr;
    QFormLayout*            FormLayout  = nullptr;
    std::vector<QLineEdit*> InputList   = {};

    explicit HcWidgetList( QWidget* parent ) : QWidget( parent ) {

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
        ButtonAdd->setProperty( "HcButton", "true" );

        ButtonClear = new QPushButton( this );
        ButtonClear->setObjectName( "ButtonClear" );
        ButtonClear->setMaximumSize( buttonSize );
        ButtonClear->setMinimumSize( buttonSize );
        ButtonClear->setProperty( "HcButton", "true" );

        gridLayout->addWidget( ListWidget,  0, 0, 3, 1 );
        gridLayout->addWidget( ButtonAdd,   0, 1, 1, 1 );
        gridLayout->addWidget( ButtonClear, 1, 1, 1, 1 );

        retranslateUi( );

        connect( ButtonAdd,   &QPushButton::clicked, this, &HcWidgetList::buttonAdd   );
        connect( ButtonClear, &QPushButton::clicked, this, &HcWidgetList::buttonClear );

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

HcListenerDialog::HcListenerDialog() {
    if ( objectName().isEmpty() ) {
        setObjectName( QString::fromUtf8( "HcListenerDialog" ) );
    }

    gridLayout = new QGridLayout( this );
    gridLayout->setObjectName( "gridLayout" );

    InputName = new HcLineEdit( this );
    InputName->Input->setObjectName( QString::fromUtf8( "InputName" ) );

    ComboProtocol = new HcComboBox;
    ComboProtocol->Combo->setObjectName( QString::fromUtf8( "ComboProtocol" ) );

    StackedProtocols = new QStackedWidget( this );
    StackedProtocols->setObjectName( QString::fromUtf8( "HcListenerDialog.StackedProtocols" ) );

    ButtonSave = new QPushButton( this );
    ButtonSave->setObjectName( "ButtonClose" );
    ButtonSave->setProperty( "HcButton", "true" );

    ButtonClose = new QPushButton( this );
    ButtonClose->setObjectName( "ButtonClose" );
    ButtonClose->setProperty( "HcButton", "true" );

    gridLayout->addWidget( InputName,        0, 0, 1, 6 );
    gridLayout->addWidget( ComboProtocol,    1, 0, 1, 6 );
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

    connect( ComboProtocol->Combo, &QComboBox::currentTextChanged, this, &HcListenerDialog::changeProtocol );
    connect( ButtonSave,  &QPushButton::clicked, this, &HcListenerDialog::save );
    connect( ButtonClose, &QPushButton::clicked, this, [&]() {
        State = Closed;
        close();
    } );

    retranslateUi();

    for ( auto& name : Havoc->listeners() ) {
        if ( Havoc->listener( name ).has_value() ) {
            addProtocol( name, Havoc->listener( name ).value() );
        }
    }

    QMetaObject::connectSlotsByName( this );
}

auto HcListenerDialog::retranslateUi() -> void {
    setStyleSheet( Havoc->getStyleSheet() );
    setWindowTitle( "Listener" );

    ComboProtocol->Combo->clear();

    InputName->setLabelText( "Name:      " );
    ComboProtocol->setLabelText( "Protocol:  " );
    ButtonSave->setText( "Save" );
    ButtonClose->setText( "Close" );
}

auto HcListenerDialog::changeProtocol(
    const QString &text
) -> void {
    spdlog::debug( "changeProtocol( \"{}\" );", text.toStdString() );

    for ( int i = 0; i < Protocols.size(); i++ ) {
        if ( Protocols[ i ].name == text.toStdString() ) {
            StackedProtocols->setCurrentIndex( i );
            break;
        }
    }
}

auto HcListenerDialog::getCloseState() -> ListenerState { return State; }

auto HcListenerDialog::start() -> void {
    if ( Havoc->listeners().empty() ) {
        Helper::MessageBox(
                QMessageBox::Icon::Warning,
                "Listener error",
                "No protocols available"
        );
        return;
    }

    ComboProtocol->Combo->setCurrentIndex( 0 );
    exec();
}

auto HcListenerDialog::save() -> void {
    auto Result = httplib::Result();
    auto data   = json();

    /*if ( ! sanityCheckOptions() ) {
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
    }*/

InvalidServerResponseError:
    Helper::MessageBox(
        QMessageBox::Critical,
        "Listener failure",
        "Failed to start listener: Invalid response from the server"
    );

    close();
}

auto HcListenerDialog::getCurrentProtocol() -> Protocol* { return & Protocols[ StackedProtocols->currentIndex() ]; }

auto HcListenerDialog::addProtocol(
    const std::string&  name,
    const py11::object& object
) -> void {
    auto protocol = Protocol {
        .name   = name,
        .widget = new QWidget
    };

    protocol.widget->setObjectName( "HcListenerDialog.Protocol." + QString( name.c_str() ) );

    try {
        auto instance = object();
        instance.attr( "_hc_set_name" )( name );
        instance.attr( "_hc_main" )();
    } catch ( py11::error_already_set &eas ) {
        Helper::MessageBox(
            QMessageBox::Icon::Critical,
            "Listener loading error",
            std::string( eas.what() )
        );
        return;
    } catch ( const std::exception &e ) {
        Helper::MessageBox(
            QMessageBox::Icon::Critical,
            "Listener loading error",
            std::string( e.what() )
        );
        return;
    }

    ComboProtocol->Combo->addItem( protocol.name.c_str() );
    StackedProtocols->addWidget( protocol.widget );

    Protocols.push_back( protocol );
}
