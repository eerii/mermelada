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
    } engine_config;

    //* run config
    //      run time, modify default values using the constructor
    inline RunConfig config{};

    //* debug config
    //      run time, only on debug builds, same structure as run config
    #ifdef DEBUG
    inline DebugConfig debug_config{};
    #endif
}