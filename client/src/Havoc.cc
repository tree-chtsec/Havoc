#include <Havoc.h>
#include <QTimer>
#include <QtCore5Compat/QTextCodec>

HavocClient::HavocClient() {
    /* initialize logger */
    spdlog::set_pattern( "[%T %^%l%$] %v" );
    spdlog::info( "Havoc Framework [{} :: {}]", HAVOC_VERSION, HAVOC_CODENAME );

    /* enabled debug messages */
    spdlog::set_level( spdlog::level::debug );

    /* TODO: read this from the config file */
    const auto family = "Monospace";
    const auto size   = 9;

    QTextCodec::setCodecForLocale( QTextCodec::codecForName( "UTF-8" ) );
    QApplication::setFont( QFont( family, size ) );
    QTimer::singleShot( 10, [&]() {
        QApplication::setFont( QFont( family, size ) );
    } );
}

HavocClient::~HavocClient() = default;

/*!
 * @brief
 *  this entrypoint executes the connector dialog
 *  and tries to connect and login to the teamserver
 *
 *  after connecting it is going to start an event thread
 *  and starting the Havoc MainWindow.
 */
auto HavocClient::Main(
    int    argc,
    char** argv
) -> void {
    auto Connector = new HcConnectDialog();
    auto Result    = httplib::Result();
    auto Response  = json{};
    auto Error     = std::string( "Failed to send login request: " );

    /* get provided creds */
    auto data = Connector->start();
    if ( data.empty() || ! Connector->pressedConnect() ) {
        return;
    }

    /* create http client */
    auto Http = httplib::Client( "https://" + data[ "host" ].get<std::string>() + ":" + data[ "port" ].get<std::string>() );
    Http.enable_server_certificate_verification( false );

    /* send request */
    Result = Http.Post( "/api/login", data.dump(), "application/json" );

    switch ( Result.error() ) {
        case httplib::Error::Unknown:
            spdlog::error( Error + "Unknown" );
            return;

        case httplib::Error::Connection:
            spdlog::error( Error + "Connection" );
            return;

        case httplib::Error::BindIPAddress:
            spdlog::error( Error + "BindIPAddress" );
            return;

        case httplib::Error::Read:
            spdlog::error( Error + "Read" );
            return;

        case httplib::Error::Write:
            spdlog::error( Error + "Write" );
            return;

        case httplib::Error::ExceedRedirectCount:
            spdlog::error( Error + "ExceedRedirectCount" );
            return;

        case httplib::Error::Canceled:
            spdlog::error( Error + "Canceled" );
            return;

        case httplib::Error::SSLConnection:
            spdlog::error( Error + "SSLConnection" );
            return;

        case httplib::Error::SSLLoadingCerts:
            spdlog::error( Error + "SSLLoadingCerts" );
            return;

        case httplib::Error::SSLServerVerification:
            spdlog::error( Error + "SSLServerVerification" );
            return;

        case httplib::Error::UnsupportedMultipartBoundaryChars:
            spdlog::error( Error + "UnsupportedMultipartBoundaryChars" );
            return;

        case httplib::Error::Compression:
            spdlog::error( Error + "Compression" );
            return;

        case httplib::Error::ConnectionTimeout:
            spdlog::error( Error + "ConnectionTimeout" );
            return;

        case httplib::Error::ProxyConnection:
            spdlog::error( Error + "ProxyConnection" );
            return;

        case httplib::Error::SSLPeerCouldBeClosed_:
            spdlog::error( Error + "SSLPeerCouldBeClosed_" );
            return;

        default: break;
    }

    /* 401 Unauthorized: Failed to log in */
    if ( Result->status == 401 ) {

        if ( Result->body.empty() ) {
            Helper::MessageBox(
                QMessageBox::Critical,
                "Login failure",
                "Failed to login: Unauthorized"
            );
        } else {

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
                "Login failure",
                QString( "Failed to login: %1" ).arg( data[ "error" ].get<std::string>().c_str() ).toStdString()
            );

            return;
        }

    } else if ( Result->status != 200 ) {
        Helper::MessageBox(
            QMessageBox::Critical,
            "Login failure",
            QString( "Unexpected response: Http status code %1" ).arg( Result->status ).toStdString()
        );
        return;
    }

    spdlog::debug( "Result: {}", Result->body );

    Profile.Name = data[ "name" ].get<std::string>();
    Profile.Host = data[ "host" ].get<std::string>();
    Profile.Port = data[ "port" ].get<std::string>();
    Profile.User = data[ "username" ].get<std::string>();
    Profile.Pass = data[ "password" ].get<std::string>();

    if ( ( data = json::parse( Result->body ) ).is_discarded() ) {
        goto InvalidServerResponseError;
    }

    if ( ! data.contains( "token" ) ) {
        goto InvalidServerResponseError;
    }

    if ( ! data[ "token" ].is_string() ) {
        goto InvalidServerResponseError;
    }

    Profile.Token = data[ "token" ].get<std::string>();

    /* create main window */
    Gui = new HcMainWindow;
    Gui->renderWindow();
    Gui->setStyleSheet( getStyleSheet() );

    setupThreads();

    Python = new HcPyEngine();
    Python->run();

    QApplication::exec();

    return;

InvalidServerResponseError:
    Helper::MessageBox(
        QMessageBox::Critical,
        "Login failure",
        "Failed to login: Invalid response from the server"
    );
}

auto HavocClient::Exit() -> void {
    QApplication::exit( 0 );
}

auto HavocClient::ApiSend(
    const std::string& endpoint,
    const json&        body
) const -> httplib::Result {
    /* create http client */
    auto Http   = httplib::Client( "https://" + Profile.Host + ":" + Profile.Port );
    auto Result = httplib::Result();
    auto Error  = std::string( "Failed to send api request: " );

    Http.enable_server_certificate_verification( false );

    /* send request */
    Result = Http.Post( endpoint, body.dump(), "application/json" );

    switch ( Result.error() ) {
        case httplib::Error::Unknown:
            spdlog::error( Error + "Unknown" );
            goto END;

        case httplib::Error::Connection:
            spdlog::error( Error + "Connection" );
            goto END;

        case httplib::Error::BindIPAddress:
            spdlog::error( Error + "BindIPAddress" );
            goto END;

        case httplib::Error::Read:
            spdlog::error( Error + "Read" );
            goto END;

        case httplib::Error::Write:
            spdlog::error( Error + "Write" );
            goto END;

        case httplib::Error::ExceedRedirectCount:
            spdlog::error( Error + "ExceedRedirectCount" );
            goto END;

        case httplib::Error::Canceled:
            spdlog::error( Error + "Canceled" );
            goto END;

        case httplib::Error::SSLConnection:
            spdlog::error( Error + "SSLConnection" );
            goto END;

        case httplib::Error::SSLLoadingCerts:
            spdlog::error( Error + "SSLLoadingCerts" );
            goto END;

        case httplib::Error::SSLServerVerification:
            spdlog::error( Error + "SSLServerVerification" );
            goto END;

        case httplib::Error::UnsupportedMultipartBoundaryChars:
            spdlog::error( Error + "UnsupportedMultipartBoundaryChars" );
            goto END;

        case httplib::Error::Compression:
            spdlog::error( Error + "Compression" );
            goto END;

        case httplib::Error::ConnectionTimeout:
            spdlog::error( Error + "ConnectionTimeout" );
            goto END;

        case httplib::Error::ProxyConnection:
            spdlog::error( Error + "ProxyConnection" );
            goto END;

        case httplib::Error::SSLPeerCouldBeClosed_:
            spdlog::error( Error + "SSLPeerCouldBeClosed_" );
            goto END;

        default: break;
    }

END:
    return Result;
}

auto HavocClient::eventClosed() -> void {
    spdlog::error( "websocket closed" );
    Exit();
}

auto HavocClient::getServer()      -> std::string { return Profile.Host + ":" + Profile.Port; }
auto HavocClient::getServerToken() -> std::string { return Profile.Token; }

auto HavocClient::eventHandle(
    const QByteArray& request
) -> void {
    auto event = json::parse( request.toStdString() );

    /* check if we managed to parse the json event
     * if yes then dispatch it but if not then dismiss it */
    if ( ! event.is_discarded() ) {
        eventDispatch( event );
    } else {
        spdlog::error( "failed to parse event" );
        /* what now ?
         * I guess ignore since its not valid event
         * or debug print it I guess */
    }
}

auto HavocClient::getStyleSheet(
    void
) -> QByteArray {
    if ( QFile::exists( "theme.css" ) ) {
        return Helper::FileRead( "theme.css" );
    }

    return Helper::FileRead( ":/style/default" );
}

auto HavocClient::addListener(
    const std::string&  name,
    const py11::object& listener
) -> void {
    protocols.push_back( Listener{
        .name   = name,
        .object = listener
    } );
}

auto HavocClient::listener(
    const std::string& name
) -> std::optional<py11::object> {
    for ( auto& listener : protocols ) {
        if ( listener.name == name ) {
            return listener.object;
        }
    }

    return std::nullopt;
}

auto HavocClient::listeners() -> std::vector<std::string> {
    auto names = std::vector<std::string>();

    for ( auto& listener : protocols ) {
        names.push_back( listener.name );
    }

    return names;
}

auto HavocClient::setupThreads() -> void {
    /* now set up the event thread and dispatcher */
    Events.Thread = new QThread;
    Events.Worker = new EventWorker;
    Events.Worker->moveToThread( Events.Thread );

    /* connect events */
    QObject::connect( Events.Thread, &QThread::started, Events.Worker, &EventWorker::run );
    QObject::connect( Events.Worker, &EventWorker::availableEvent, this, &HavocClient::eventHandle );
    QObject::connect( Events.Worker, &EventWorker::socketClosed, this, &HavocClient::eventClosed );

    /* fire up the even thread that is going to
     * process events and emit signals to the main gui thread */
    Events.Thread->start();
}
