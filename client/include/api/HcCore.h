#ifndef HAVOCCLIENT_API_HCCORE_H
#define HAVOCCLIENT_API_HCCORE_H

auto HcServerApiSend(
    const std::string& endpoint,
    const json&        data
) -> json;

#endif //HAVOCCLIENT_API_HCCORE_H
