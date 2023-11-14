#ifndef HAVOCCLIENT_HAVOC_H
#define HAVOCCLIENT_HAVOC_H

#include <Common.h>

/* Havoc include */
#include <core/Helper.h>
#include <core/Events.h>

#include <ui/MainWindow.h>
#include <ui/HavocConnect.h>

#define HAVOC_VERSION  "0.7"
#define HAVOC_CODENAME "Bite The Dust"

class HavocClient : public QWidget {

    /* current connection info */
    struct {
        std::string Name;
        std::string Host;
        std::string Port;
        std::string User;
        std::string Pass;

        /* login token to interact
         * with the api endpoints */
        std::string Token;
    } Profile;

    struct {
        QThread*     Thread;
        EventWorker* Worker;
    } Events;

public:
    /* havoc client constructor and destructor */
    explicit HavocClient();
    ~HavocClient();

    /* client entrypoint */
    auto Main(
        int    argc,
        char** argv
    ) -> void;

    /* exit application and free resources */
    auto Exit() -> void;

    /* get server address */
    auto getServer() -> std::string;

    /* get server connection token */
    auto getServerToken() -> std::string;

    auto eventHandle( const QByteArray& event ) -> void;
    auto eventClosed() -> void;

    /* send request to api endpoint */
    auto ApiSend(
        const std::string endpoint,
        const json&       body
    ) -> httplib::Result;
};

/* a global Havoc app instance */
extern HavocClient* Havoc;

#endif //HAVOCCLIENT_HAVOC_H
