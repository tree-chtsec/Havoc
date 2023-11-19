#ifndef HAVOCCLIENT_AGENTCONSOLE_H
#define HAVOCCLIENT_AGENTCONSOLE_H

#include <Havoc.h>

class AgentConsole : public QWidget {

    json data;

public:
    explicit AgentConsole( json agent );
    ~AgentConsole();

    auto invokeCommand(
        std::string command
    ) -> bool;
};

#endif //HAVOCCLIENT_AGENTCONSOLE_H
