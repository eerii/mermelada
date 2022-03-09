//project fresa, 2017-2022
//by jose pazos perez
//licensed under GPLv3 uwu

#pragma once

#include "types.h"
#include "reflection.h"

//: Follow the instructions on fresa/ecs/ecs.h to create a file like this
//: Both member definitions are equivalent

namespace Fresa::Component
{
    struct Position : Reflection::Members<"x", "y"> {
        int x;
        int y;
    };
    
    struct Something {
        Members(Something, a, b, c, d, e)
        float a;
        str b;
        Vec2<int> c;
        Rect2<float> d;
        std::vector<str> e;
    };
    
    using ComponentType = std::variant<Position, Something>;
}
