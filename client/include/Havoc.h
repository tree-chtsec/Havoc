#ifndef HAVOCCLIENT_HAVOC_H
#define HAVOCCLIENT_HAVOC_H

#include <Common.h>
#include <Events.h>

/* Havoc include */
#include <core/Helper.h>
#include <core/EventWorker.h>

#include <ui/HcPageAgent.h>
#include <ui/HcPageListener.h>
#include <ui/HcPageBuilder.h>
#include <ui/HcPageScript.h>

#include <ui/HcListenerDialog.h>
#include <ui/HcMainWindow.h>
#include <ui/HcConnectDialog.h>

#include <api/Engine.h>

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
    HcMainWindow* Gui    = nullptr;
    HcPyEngine*   Python = nullptr;

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

    auto getStyleSheet() -> QByteArray;

    auto setupThreads() -> void;

    /* send request to api endpoint */
    auto ApiSend(
        const std::string& endpoint,
        const json&        body
    ) const -> httplib::Result;

Q_SIGNALS:
    /* signals */
    auto eventHandle(
        const QByteArray& request
    ) -> void;

    auto eventDispatch(
        const json& event
    ) -> void;

    auto eventClosed() -> void;
};

/* a global Havoc app instance */
extern HavocClient* Havoc;

#endif //HAVOCCLIENT_HAVOC_H
