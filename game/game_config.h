//project fresa, 2017-2022
//by jose pazos perez
//licensed under GPLv3

//: Config definitons
//      You need to initialize all the specific configurations for your project (marked as const in the config file)
//      as well as any value you would like to change
//      An example would be using a config file such as this one

#pragma once

#ifndef GAME_CONFIG
#define GAME_CONFIG

#include "config.h"
using namespace Fresa;

const str Config::name = "fresa";
const ui8 Config::version[3] = {0, 3, 12};
const Vec2<ui32> Config::window_size = {1024, 720};
const Vec2<ui32> Config::resolution = {256, 180};

const float Config::timestep = 10.0f;
float Config::game_speed = 1.0f;

str Config::renderer_description_path = "render/renderer_description";
bool Config::draw_indirect = false;

#endif
