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

        //
        // Havoc Server api functions
        //
        core.def( "HcServerApiSend", HcServerApiSend );

        //
        // Havoc Listener api functions
        //
        core.def( "HcListenerProtocolData", HcListenerProtocolData );
        core.def( "HcListenerAll",          HcListenerAll );
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
            /*if ( Havoc->Protocols().empty() ) {
                Helper::MessageBox(
                    QMessageBox::Critical,
                    "Script Manager",
                    "Failed to register listener view: No protocols registered"
                );

                return;
            }

            if ( ! Havoc->ProtocolObject( name ).has_value() ) {
                Helper::MessageBox(
                    QMessageBox::Critical,
                    "Script Manager",
                    QString( "Failed to register listener view: No protocol with the name \"%1\" found" ).arg( name.c_str() ).toStdString()
                );

                return;
            }*/

            Havoc->AddProtocol( name, object );
        } );

        ui.def( "HcUiBuilderRegisterView", [](
            const std::string&  name,
            const py11::object& object
        ) {
            /*if ( Havoc->Builders().empty() ) {
                Helper::MessageBox(
                    QMessageBox::Critical,
                    "Script Manager",
                    "Failed to register builder view: No payloads registered"
                );

                return;
            }

            if ( ! Havoc->BuilderObject( name ).has_value() ) {
                Helper::MessageBox(
                    QMessageBox::Critical,
                    "Script Manager",
                    QString( "Failed to register builder view: No payload with the name \"%1\" found" ).arg( name.c_str() ).toStdString()
                );

                return;
            }*/

            Havoc->AddBuilder( name, object );
        } );

        ui.def( "HcUiGetStyleSheet", []() -> py11::str {
            return ( Havoc->getStyleSheet().toStdString() );
        } );

        ui.def( "HcUiMessageBox", [](
            const int          icon,
            const std::string& title,
            const std::string& text
        ) {
            Helper::MessageBox( ( QMessageBox::Icon ) icon, title, text );
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

