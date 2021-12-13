//project verse, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#include "log.h"
#include "game.h"
#include "scene.h"
#include "f_time.h"
#include "r_graphics.h"

#include "system_list.h"

using namespace Fresa;

#ifdef __EMSCRIPTEN__
void game_loop_emscripten() {
    Game::update(config);
}
#endif

int main(int argc, const char * argv[]) {
    log::debug("Starting main");
    
    //: Initialize game
    bool running = Game::init();
    
    //: Load scene
    SceneID scene = registerScene();
    active_scene = scene;
    
    //: Draw test
    Graphics::TextureID test_texture_data = Graphics::getTextureID("res/graphics/texture.png");
    Graphics::DrawID test_draw_id = Graphics::getDrawID_Rect(Graphics::SHADER_DRAW_TEX);
    Graphics::bindTexture(test_draw_id, test_texture_data);
    Graphics::DrawID test_draw_id_2 = Graphics::getDrawID_Cube(Graphics::SHADER_DRAW_COLOR);
    
    Clock::time_point start_time = time();
    
    //DELETE
    EntityID e = scene_list.at(scene).createEntity();
    Component::Test* c = scene_list.at(scene).addComponent<Component::Test>(e);
    
    Component::getID<Component::Other>();
    
    //: Update loop
#ifdef __EMSCRIPTEN__
    while (true)
        emscripten_set_main_loop(game_loop_emscripten, 0, 1);
#else
    while (running) {
        float t = sec(time() - start_time);
        
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f + 0.8f * std::sin(t * 1.570796f)));
        model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
        model = glm::rotate(model, t * 1.570796f, glm::vec3(0.0f, 0.0f, 1.0f));
        Graphics::draw(test_draw_id, model);
        
        glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f + 0.8f * std::sin(t * 1.570796f + 1.570796f)));
        model2 = glm::scale(model2, glm::vec3(0.5f, 0.5f, 0.5f));
        model2 = glm::rotate(model2, -t * 1.570796f, glm::vec3(0.0f, 0.0f, 1.0f));
        Graphics::draw(test_draw_id_2, model2);
        
        running = Game::update();
    }
#endif
    
    //: Cleanup
    Game::stop();
    
    return EXIT_SUCCESS;
}
