//
//  main.cpp
//  aguacate
//
//  Created by José on 9/12/21.
//

#include "log.h"
#include "game.h"
#include "config.h"
#include "ftime.h"
#include "r_graphics.h"

using namespace Fresa;

#ifdef __EMSCRIPTEN__
void game_loop_emscripten() {
    Game::update(config);
}
#endif

int main(int argc, const char * argv[]) {
    log::debug("Starting main");
    
    //TODO: REMOVE
    Config config = {
        .resolution = Vec2(256, 180),
        .window_size = Vec2(1024, 720),
        .render_scale = 4,
        
        .timestep = 10.0,
        .game_speed = 1.0f,
        
        .use_grayscale = false,
        .use_light = true,
        .palette_index = 1,
        .num_palettes = 9,
        .background_color = {0.0, 0.0, 0.0, 1.0},
        
        .enable_lookahead = true,
        .enable_smooth_panning = true,
        .use_subpixel_cam = true,
        
        .gravity = 800,
        .gravity_dir = Vec2<float>(0, 1),
        
        .player_loses_light = false,
    };
    
    //: Initialize game
    bool running = Game::init(config);
    
    //: Register components
    Component::registerComponents();
    
    //: Load scene
    Scene* scene = new Scene();
    config.active_scene = scene;
    
    //: Draw test
    Graphics::TextureID test_texture_data = Graphics::getTextureID("res/graphics/texture.png");
    Graphics::DrawID test_draw_id = Graphics::getDrawID_Rect(Graphics::SHADER_DRAW_TEX);
    Graphics::bindTexture(test_draw_id, test_texture_data);
    Graphics::DrawID test_draw_id_2 = Graphics::getDrawID_Cube(Graphics::SHADER_DRAW_COLOR);
    
    Clock::time_point start_time = time();
    
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
        
        running = Game::update(config);
    }
#endif
    
    //: Cleanup
    Game::stop();
    
    return EXIT_SUCCESS;
}
