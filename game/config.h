//* game_config
//      sample configuration for a game, extends fresa_config.h
#pragma once

#include "fresa_config.h"

namespace fresa
{
    //* engine config
    //      compile time, modify default values using overrides of the constexpr virtual functions
    constexpr inline struct _EngineConfig : EngineConfig {
        constexpr str_view name() const override { return "fresa"; }

        #ifdef FRESA_DEBUG
        constexpr str_view run_tests() const override { return ""; }
        constexpr ui32 log_level() const override { return 0b0111111; }
        #endif
    } engine_config;

    //* run config
    //      run time, modify default values using the constructor
    inline RunConfig config{};

    //* debug config
    //      run time, only on debug builds, same structure as run config
    #ifdef FRESA_DEBUG
    inline DebugConfig debug_config{};
    #endif
}