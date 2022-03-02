//project fresa, 2017-2022
//by jose pazos perez
//licensed under GPLv3 uwu

#pragma once

#include "types.h"
#include "reflection.h"

//: Follow the instructions on fresa/ecs/ecs.h to create a file like this

namespace Fresa::Component
{
    struct Test {
        Serialize(Test, a, b)
        int a;
        str b;
    };
    
    using ComponentType = std::variant<Test>;
}
