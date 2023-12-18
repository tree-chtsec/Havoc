#ifndef HAVOCCLIENT_API_ENGINE_H
#define HAVOCCLIENT_API_ENGINE_H

#include <Common.h>

#include <api/HcCore.h>
#include <api/HcScriptManager.h>

class HavocPyEngine : public QThread {
public:
    py11::scoped_interpreter* guard{};

    explicit HavocPyEngine();
    ~HavocPyEngine();

    auto run() -> void;
};

#endif
