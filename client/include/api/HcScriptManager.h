#ifndef HAVOCCLIENT_API_HCSCRIPTMANAGER_H
#define HAVOCCLIENT_API_HCSCRIPTMANAGER_H

#include <Common.h>
#include <api/Engine.h>

auto HcScriptManagerConsoleStdOut(
    const std::string& text
) -> void;

auto HcScriptManagerConsoleStdIn(
    const py11::object& callback
) -> void;

#endif //HAVOCCLIENT_API_HCSCRIPTMANAGER_H
