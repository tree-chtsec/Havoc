#include <Havoc.h>
#include <api/HcScriptManager.h>

auto HcScriptManagerConsoleStdOut(
    const std::string& text
) -> void {
    Havoc->Gui->PageScripts->PyConsole->appendConsole( text.c_str() );
}

auto HcScriptManagerLoadScriptCallback(
    const py11::object& callback
) -> void {
    Havoc->Gui->PageScripts->LoadCallback = callback;
}