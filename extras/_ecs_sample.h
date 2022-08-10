#pragma once

#include "ecs.h"
#include "system.h"
#include "fresa_math.h"

using namespace fresa;

struct ComponentA {
    int x;
    float y;
};

struct ComponentB {
    str name;
    Vec3<float> color;
};

inline ecs::Scene scene;

struct SomeSystem {
    static void init() {
        for (auto [e, a] : ecs::View<ComponentA>(scene)()) { a.x *= 2; }
        log::info("the first entity with component a has an x value of {}", ecs::View<ComponentA>(scene)().front().second.x);
    }

    static void stop() {}
};

void ecs_sample() {
    auto e1 = scene.add(ComponentA{4, 2.0f});
    auto e2 = scene.add(ComponentB{"hello", Vec3<float>{0.5f, 1.0f, 0.8f}});
    auto e3 = scene.add(ComponentA{3, 4.0f}, ComponentB{"world", Vec3<float>{0.4f, 0.3f, 0.7f}});
    for (int i : rv::iota(0, 5)) { scene.add(ComponentA{i, i * 2.0f}); }

    log::info("the scene has {} entities with component a and {} entities with component b", scene.cpool<ComponentA>().size(), scene.cpool<ComponentB>().size());

    system::add(SomeSystem());
}