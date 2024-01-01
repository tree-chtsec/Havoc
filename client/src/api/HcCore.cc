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
