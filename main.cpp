//project verse, 2017-2022
//by jose pazos perez
//licensed under GPLv3 uwu

#include "log.h"
#include "game.h"
#include "scene.h"
#include "file.h"

#include "system_list.h"

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
    SceneID scene = registerScene();
    active_scene = scene;
    
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
