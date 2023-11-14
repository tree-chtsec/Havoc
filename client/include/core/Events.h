#ifndef HAVOCCLIENT_EVENTS_H
#define HAVOCCLIENT_EVENTS_H

#include <Common.h>

#include <QThread>
#include <QString>
#include <QWebSocket>

class EventWorker : public QThread
{
Q_OBJECT
    QWebSocket* WebSocket = nullptr;
    bool        shutdown  = false;

public:
    explicit EventWorker();
    ~EventWorker();

    /* run event thread */
    void run();

public slots:
    auto connected() -> void;
    auto closed() -> void;
    auto receivedEvent( const QByteArray &message ) -> void;

signals:
    /* dispatch json event */
    auto availableEvent( const QByteArray& event ) -> void;
    auto socketClosed() -> void;
};

namespace Event {

    namespace Operator {
        static std::string connect = "operator::connect";
        static std::string login   = "operator::login";
        static std::string logout  = "operator::logout";
        static std::string chat    = "operator::chat";
    }

    namespace Agent {
        static std::string initialize = "agent::initialize";
        static std::string callback   = "agent::callback";
        static std::string console    = "agent::console";
        static std::string input      = "agent::input";
        static std::string remove     = "agent::remove";
    }

    namespace Listener {
        static std::string add    = "listener::add";
        static std::string edit   = "listener::edit";
        static std::string status = "listener::status";
    }
};

#endif //HAVOCCLIENT_EVENTS_H
