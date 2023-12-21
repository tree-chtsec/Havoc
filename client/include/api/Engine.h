#ifndef HAVOCCLIENT_API_ENGINE_H
#define HAVOCCLIENT_API_ENGINE_H

#include <Common.h>

#include <api/HcCore.h>
#include <api/HcScriptManager.h>

class HcPyEngine : public QThread {
public:
    py11::scoped_interpreter* guard{};

    explicit HcPyEngine();
    ~HcPyEngine();

    auto run() -> void;
};

#endif
