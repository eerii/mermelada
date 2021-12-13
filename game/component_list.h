//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include <variant>

namespace Fresa::Component
{
    struct Test {
        Serialize(Test, i, j);
        int i;
        float j;
    };

    struct Other {
        Serialize(Other, a, b);
        int a;
        float b;
    };
    
    using ComponentType = std::variant<Test, Other>;
}
