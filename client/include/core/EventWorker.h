#ifndef HAVOCCLIENT_EVENTWORKER_H
#define HAVOCCLIENT_EVENTWORKER_H

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

    void test();

public slots:
    auto connected() -> void;
    auto closed() -> void;
    auto receivedEvent( const QByteArray &message ) -> void;

signals:
    /* dispatch json event */
    auto availableEvent( const QByteArray& event ) -> void;
    auto socketClosed() -> void;
};

#endif //HAVOCCLIENT_EVENTWORKER_H
