//project verse, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include "ecs.h"
#include <map>

namespace Fresa
{
    static std::map<str, std::function<bool(EntityID eid)>> controllers = {};
    
    static std::map<str, std::function<bool(EntityID eid)>> damage_controllers = {};
    
    static std::map<str, std::function<bool(EntityID eid)>> camera_controllers = {};
}
