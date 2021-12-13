//project verse, 2017-2022
//by jose pazos perez
//licensed under GPLv3

#pragma once

#include "scene.h"
#include "log.h"

namespace Fresa::System
{
    struct SomeSystem : PhysicsUpdate<SomeSystem, PRIORITY_MOVEMENT>, RenderUpdate<SomeSystem> {
        inline static void update() {
            Scene& s = scene_list.at(active_scene);
            for (EntityID e : SceneView<Component::Test>(s)) {
                //log::info("%d %s", e, s.getName(e).c_str());
            }
        }
        
        inline static void render() {
            
        }
    };
}
