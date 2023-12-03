#ifndef HAVOCCLIENT_HAVOC_H
#define HAVOCCLIENT_HAVOC_H

#include <Common.h>
#include <Events.h>

/* Havoc include */
#include <core/Helper.h>
#include <core/EventWorker.h>

#include <ui/PageAgent.h>
#include <ui/PageListener.h>
#include <ui/DialogListener.h>
#include <ui/MainWindow.h>
#include <ui/Connect.h>

#define HAVOC_VERSION  "0.8"
#define HAVOC_CODENAME "Killer Queen"

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
    /* Main gui window */
    HavocMainWindow* MainWindows = nullptr;

    /* havoc client constructor and destructor */
    explicit HavocClient();
    ~HavocClient() override;

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

    /* signals */
    auto eventHandle(
        const QByteArray& request
    ) -> void;
    auto eventDispatch(
        const json& event
    ) -> void;
    auto eventClosed() -> void;

    /*
     * Ui methods
     */
    auto getStyleSheet() -> QByteArray;

    /* send request to api endpoint */
    auto ApiSend(
        const std::string& endpoint,
        const json&        body
    ) -> httplib::Result;
};

/* a global Havoc app instance */
extern HavocClient* Havoc;

#endif //HAVOCCLIENT_HAVOC_H
