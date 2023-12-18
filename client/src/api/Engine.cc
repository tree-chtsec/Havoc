#include <Havoc.h>
#include <core/Helper.h>
#include <api/Engine.h>

PYBIND11_EMBEDDED_MODULE( _pyhavoc, m ) {
    m.doc() = "python api for havoc framework";

    /* havoc client core api */
    {
        auto core = m.def_submodule(
            "core",
            "havoc client core api"
        );

        core.def( "HcScriptManagerConsoleStdOut", HcScriptManagerConsoleStdOut, py11::call_guard<py11::gil_scoped_release>() );
    }
}

HavocPyEngine::HavocPyEngine() {
    guard = new py11::scoped_interpreter;
}

HavocPyEngine::~HavocPyEngine() = default;

auto HavocPyEngine::run() -> void {
    auto exception = std::string();

    try {
        py11::module_::import( "python.pyhavoc" );
    } catch ( py11::error_already_set &eas ) {
        exception = std::string( eas.what() );
    } catch ( const std::exception &e ) {
        exception = std::string( e.what() );
    }

    if ( ! exception.empty() ) {
        spdlog::error( "failed to import \"python.pyhavoc\": \n{}", exception );
        exit( 0 );
    }
}

