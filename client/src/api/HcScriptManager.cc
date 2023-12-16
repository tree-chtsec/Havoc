#include <Havoc.h>
#include <api/HcScriptManager.h>

auto HcScriptManagerConsoleStdOut(
    const std::string& text
) -> void {
    Havoc->MainWindows->PageScripts->PyConsole->appendConsole( text.c_str() );
}

auto HcScriptManagerConsoleStdIn(
    const py11::function& callback
) -> void {
    Havoc->MainWindows->PageScripts->PyConsole->InputCallbacks.push_back( callback );
}