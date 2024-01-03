#include <Common.h>
#include <Havoc.h>
#include <api/HcCore.h>

auto HcServerApiSend(
    const std::string& endpoint,
    const json&        data
) -> json {
    auto result = httplib::Result();

    result = Havoc->ApiSend(
        endpoint,
        data
    );

    return json {
        { "version", result->version },
        { "status",  result->status  },
        { "reason",  result->reason  },
        { "body",    result->body    },
    };
}

auto HcListenerProtocolData(
    const std::string& protocol
) -> json {

    for ( auto& p : Havoc->Gui->PageListener->Protocols ) {
        if ( p.contains( "data" ) ) {
            if ( p[ "data" ].contains( "protocol" ) ) {
                if ( p[ "data" ][ "protocol" ] == protocol ) {
                    if ( p[ "data" ].contains( "data" ) ) {
                        return p[ "data" ][ "data" ];
                    }
                }
            }
        }
    }

    return {};
}