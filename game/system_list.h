//: fresa by jose pazos perez, licensed under GPLv3
#pragma once

#include "scene.h"

#include "log.h"
#include "file.h"
#include "input.h"
#include "r_graphics.h"
#include "f_time.h"
#include "load_obj.h"

using namespace Fresa::Graphics;

namespace Fresa::System
{
    //: Example of a hardcoded render system
    //: This is very basic and not elegant, it will be reworked in the future
    
    struct SomeSystem : SystemInit<SomeSystem>, RenderUpdate<SomeSystem, PRIORITY_FIRST> {
        static void init();
        static void render();
    };
    
    struct CameraSystem : SystemInit<CameraSystem>, PhysicsUpdate<CameraSystem, PRIORITY_MOVEMENT> {
        static void init();
        static void update();
    };
}
