#include <ui/AgentConsole.h>

AgentConsole::AgentConsole( json agent ) : data( agent ) {}
AgentConsole::~AgentConsole( ) = default;

/*!
 * @brief
 *  invokes an agent command
 *
 * @param command
 *  command to invoke
 *
 * @return
 *  if successfully invoked command
 */
auto AgentConsole::invokeCommand(
    std::string command
) -> bool {
    auto success = false;

    return success;
}

