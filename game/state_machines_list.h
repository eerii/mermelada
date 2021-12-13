//project verse, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include <variant>

#include "state_machine.h"
#include "static_str.h"

namespace Fresa
{
    using StateType = std::variant<>;
}

namespace Fresa::State
{
    template <typename State>
    str getCurrentState(State state) {
        return CURR_STATE(state);
    }
}

