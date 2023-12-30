#include <Havoc.h>
#include <core/Helper.h>
#include <api/Engine.h>
#include <filesystem>

PYBIND11_EMBEDDED_MODULE( _pyhavoc, m ) {
    m.doc() = "python api for havoc framework";

    /* havoc client core api */
    {
        auto core = m.def_submodule(
            "core",
            "havoc client core api"
        );

        //
        // Havoc Script Manager api functions
        //
        core.def( "HcScriptManagerConsoleStdOut",      HcScriptManagerConsoleStdOut );
        core.def( "HcScriptManagerLoadScriptCallback", HcScriptManagerLoadScriptCallback );

    }

    /* havoc client ui api */
    {
        auto ui = m.def_submodule(
            "ui",
            "havoc client ui api"
        );

        //
        // Havoc Ui functions and utilities
        //

        ui.def( "HcUiPayloadBuilderObjName", [] () -> py11::str {
            return ( Havoc->Gui->PagePayload->objectName().toStdString() );
        } );

        ui.def( "HcUiListenerObjName", []() -> py11::str {
            return ( "HcListenerDialog.StackedProtocols" );
        } );

        ui.def( "HcUiListenerRegisterView", [](
            const std::string&  name,
            const py11::object& object
        ) {
            Havoc->addListener( name, object );
        } );

        ui.def( "HcUiGetStyleSheet", []() -> py11::str {
            return ( Havoc->getStyleSheet().toStdString() );
        } );
    }
}

HcPyEngine::HcPyEngine()  = default;
HcPyEngine::~HcPyEngine() = default;

auto HcPyEngine::run() -> void {
    auto exception = std::string();

    guard = new py11::scoped_interpreter;

    try {
        py11::module_::import( "sys" )
            .attr( "path" )
            .attr( "append" )( "python" );

        py11::module_::import( "pyhavoc" );
    } catch ( py11::error_already_set &eas ) {
        exception = std::string( eas.what() );
    } catch ( const std::exception &e ) {
        exception = std::string( e.what() );
    }

    if ( ! exception.empty() ) {
        spdlog::error( "failed to import \"python.pyhavoc\": \n{}", exception );
    }
}

