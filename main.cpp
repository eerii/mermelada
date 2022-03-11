//project fresa, 2017-2022
//by jose pazos perez
//licensed under GPLv3 uwu

#include "log.h"
#include "game_config.h"
#include "game.h"
#include "scene.h"
#include "file.h"

#include "system_list.h"

#include "serialization.h"

using namespace Fresa;

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
void game_loop_emscripten() { Game::update(); }
#endif

int main (int argv, char** args) {
    log::debug("Starting main");
    
    //: Initialize game
    bool running = Game::init();
    
    //: Load scene
    SceneID scene = Serialization::loadScene("test_scene");
    active_scene = scene;
    
    for (EntityID e : SceneView<>(scene_list.at(scene))) {
        log::info(scene_list.at(scene).getName(e));
        for_<Component::ComponentType>([&](auto i){
            using C = std::variant_alternative_t<i.value, Component::ComponentType>;
            C* c = scene_list.at(scene).getComponent<C>(e);
            if (c != nullptr) {
                for_<Reflection::as_type_list<C>>([&](auto i){
                    using M = std::variant_alternative_t<i.value, Reflection::as_type_list<C>>;
                    M* m = Reflection::get_member_i<i.value, C>(c);
                    log::info("%s:", C::member_names.at(i.value));
                    log::info(*m);
                });
            }
        });
    }
    
    //: Update loop
    #ifdef __EMSCRIPTEN__
    while (true)
        emscripten_set_main_loop(game_loop_emscripten, 0, 1);
    #else
    while (running)
        running = Game::update();
    #endif
    
    //: Cleanup
    Game::stop();
    
    return EXIT_SUCCESS;
}
