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

//* add reflection
//      to add reflection information, include the file "reflection_<filename>.h"
//      this will trigger the automatic generator when building the code
//      the include has to be at the end of the file
#if __has_include("reflection_some_component.h")
#include "reflection_some_component.h"
#endif