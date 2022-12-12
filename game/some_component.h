//* sample component
//      an example on how to declare components and use reflection on them
#pragma once

#include "ecs.h"

namespace fresa
{
    //* a component is just a struct with some fields
    struct SomeComponent {
        int some_field;
        float another_field;
        std::vector<str> some_vector;
    };
}