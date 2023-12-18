#include <Havoc.h>
#include <api/HcScriptManager.h>

auto HcScriptManagerConsoleStdOut(
    const std::string& text
) -> void {
    Havoc->MainWindows->PageScripts->PyConsole->appendConsole( text.c_str() );
}