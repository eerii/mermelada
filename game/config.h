//* game_config
//      sample configuration for a game, extends fresa_config.h
#pragma once

#include "fresa_config.h"

namespace fresa
{
    //* engine config
    //      compile time, modify default values using overrides of the constexpr virtual functions
    constexpr inline struct _EngineConfig : EngineConfig {
        consteval str_view name() const override { return "fresa"; }

        //: override some values only on debug builds
        #ifdef FRESA_DEBUG
        consteval str_view run_tests() const override { return ""; }
        consteval bool enable_assertions() const override { return true; }
        consteval ui32 log_level() const override { return 0b0111111; }
        consteval bool hot_reload() const override { return true; }
        #endif
    } engine_config;

    //* run config
    //      run time, modify default values using the constructor
    inline RunConfig config{
        .window_size = {800, 600}
    };

    //* debug config
    //      run time, only on debug builds, same structure as run config
    #ifdef FRESA_DEBUG
    inline DebugConfig debug_config{};
    #endif
}
