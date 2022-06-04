//: fresa by jose pazos perez, licensed under GPLv3
#include "log.h"
#include "game.h"
#include "scene.h"
#include "serialization.h"
#include "audio.h"
#include "game_config.h"

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
    active_scene = registerScene("");
    
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
