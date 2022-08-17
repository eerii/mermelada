//* sample system
//      an example on how to declare and register a system. this file must be included in a source file
#pragma once

#include "system.h"
#include "some_component.h"

namespace fresa
{
    struct SomeSystem {
        inline static System<SomeSystem> system;

        static void init() {
            log::debug("initializing system");

            //* example on how to add components
            ecs::Scene scene;
            auto entity = scene.add(SomeComponent{1, 3.14f, {"hey", "there"}});

            //* iterate over all entities with some component
            for (auto [entity, component] : ecs::View<SomeComponent>(scene)()) {
                log::debug("entity {} has the component {}", entity, component);
            }
        }

        static void update() {
            log::debug("updating system");
        }

        static void stop() {
            log::debug("stopping system");
        }
    };
}