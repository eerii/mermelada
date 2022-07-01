//* game_config
//      sample configuration for a game, extends fresa_config.h
//      works by overriding the pure constexpr virtual functions (a feature of c++20)
//      that way, all configuration is resolved at compile time but can be modified here
#pragma once

#include "fresa_config.h"

namespace fresa
{
    constexpr inline struct GameConfig : Config {
        //: change the project name to...
        constexpr str_view name() const override { return "fresa"; }
    } config;
}