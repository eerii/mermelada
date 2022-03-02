//project fresa, 2017-2022
//by jose pazos perez
//licensed under GPLv3

//: Optional file to define new vertex definitions for shaders
//: It is included in "r_dtypes.h"

#pragma once

#include "reflection.h"

struct VertexExample {
    Serialize(VertexExample, pos);
    glm::vec3 pos;
};
    
using CustomVertexType = std::variant<VertexExample>;
