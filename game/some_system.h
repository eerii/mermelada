//* sample system
//      an example on how to declare and register a system. this file must be included in a source file
#pragma once

#include "system.h"
#include "log.h"

namespace fresa
{
    struct SomeSystem {
        inline static System<SomeSystem> system;

        static void init() {
            log::debug("initializing system");
        }

        static void update() {
            log::debug("updating system");
        }

        static void stop() {
            log::debug("stopping system");
        }
    };
}