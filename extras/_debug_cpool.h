#pragma once

#include <iostream>
#include "ecs.h"

using namespace fresa;

void debug_cpool(const ecs::Scene& scene, int n = 10) {
    std::cout << "[SPARSE]: ";
    for (auto &[page, sparse] : scene.cpool<int>().sparse)
        for (auto &i : sparse | rv::take(n))
            ecs::index(i).value == ui16(-1) ?
            std::cout << "- " :
            (std::cout << ecs::index(i).value) << ":" << ecs::version(i).value << " ";
    std::cout << std::endl;
    
    std::cout << "[DENSE]:  ";
    for (auto &i : scene.cpool<int>().dense)
        std::cout << (i.value == ui16(-1) ? -1 : i.value) << " ";
    std::cout << std::endl;

    std::cout << "[DATA]:   ";
    for (auto &i : scene.cpool<int>().data)
        std::cout << i << " ";
    std::cout << std::endl;
}