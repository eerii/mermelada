//project fresa, 2017-2022
//by jose pazos perez
//licensed under GPLv3

//: Optional file to define new vertex definitions for shaders
//: It is included in "r_types.h"

#pragma once

#include "reflection.h"

struct VertexModel {
    Members(VertexModel, model)
    glm::mat4 model;
};
    
using CustomVertexType = std::variant<VertexModel>;
