//* game
//      main user configurable game file, defines engine callbacks, such as initialization and stop
#pragma once

#include "engine.h"
#include "log.h"

namespace fresa
{
    //* engine callbacks
    constexpr inline struct _EngineCallback : EngineCallback {
        void on_init() const override {
            log::debug("this will be executed when the engine starts");
        }

        void on_stop() const override {}
    } engine_callback;
}