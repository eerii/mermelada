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
            log::info("hey");
        }

        void on_stop() const override {}
    } engine_callback;
}