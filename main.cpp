//project verse, 2017-2022
//by jose pazos perez
//licensed under GPLv3 uwu

#include "log.h"
#include "game.h"
#include "scene.h"

#include "system_list.h"

using namespace Fresa;

int main(int argc, const char * argv[]) {
    log::debug("Starting main");
    
    //: Initialize game
    bool running = Game::init();
    
    //: Load scene
    SceneID scene = registerScene();
    active_scene = scene;
    
    //: Temporary test of systems and components
    EntityID e = scene_list.at(scene).createEntity();
    Component::Test* c = scene_list.at(scene).addComponent<Component::Test>(e);
    
    System::test_texture_data = Graphics::getTextureID("res/graphics/texture.png");
    System::test_draw_id = Graphics::getDrawID_Rect(Graphics::SHADER_DRAW_TEX);
    Graphics::bindTexture(System::test_draw_id, System::test_texture_data);
    System::test_draw_id_2 = Graphics::getDrawID_Cube(Graphics::SHADER_DRAW_COLOR);
    
    //: Update loop
    while (running)
        running = Game::update();
    
    //: Cleanup
    Game::stop();
    
    return EXIT_SUCCESS;
}
