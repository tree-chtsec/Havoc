#ifndef HAVOCCLIENT_API_ENGINE_H
#define HAVOCCLIENT_API_ENGINE_H

#include <Common.h>

class HavocPyEngine {
private:
    py11::scoped_interpreter* guard = nullptr;

    explicit HavocPyEngine();

    auto InvokePath(
        const std::string& path
    ) -> std::string;

    auto Invoke(
        const std::string& code
    ) -> std::string;
};

#endif
