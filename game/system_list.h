//project verse, 2017-2022
//by jose pazos perez
//licensed under GPLv3 uwu

#pragma once

#include "scene.h"

#include "log.h"
#include "input.h"
#include "r_graphics.h"
#include "f_time.h"

namespace Fresa::System
{
    //: Example of a hardcoded render system
    //: This is very basic and not elegant, it will be reworked in the future

    inline Graphics::TextureID test_texture_data;
    inline Graphics::DrawID test_draw_id;
    inline Graphics::DrawID test_draw_id_2;
    
    inline Clock::time_point start_time = time();
    
    struct SomeSystem : PhysicsUpdate<SomeSystem, PRIORITY_MOVEMENT>, RenderUpdate<SomeSystem> {
        inline static void update() {
            /*Scene& s = scene_list.at(active_scene);
            for (EntityID e : SceneView<Component::Test>(s)) {
                log::info("%d %s", e, s.getName(e).c_str());
            }*/
            
            //: Sample camera update
            if (Input::key_down(SDLK_w))
                Graphics::camera.pos.y += 1.0f * Time::physics_delta;
            if (Input::key_down(SDLK_s))
                Graphics::camera.pos.y -= 1.0f * Time::physics_delta;
            if (Input::key_down(SDLK_d))
                Graphics::camera.pos.x += 1.0f * Time::physics_delta;
            if (Input::key_down(SDLK_a))
                Graphics::camera.pos.x -= 1.0f * Time::physics_delta;
        }
        
        inline static void render() {
            float t = sec(time() - start_time);
            
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f + 0.8f * std::sin(t * 1.570796f), 0.0f));
            model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
            model = glm::rotate(model, -1.570796f, glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, t * 1.570796f, glm::vec3(0.0f, 0.0f, 1.0f));
            Graphics::draw(test_draw_id, model);
            
            glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f + 0.8f * std::sin(t * 1.570796f + 1.570796f), 0.0f));
            model2 = glm::scale(model2, glm::vec3(0.5f, 0.5f, 0.5f));
            model2 = glm::rotate(model2, 1.570796f, glm::vec3(1.0f, 0.0f, 0.0f));
            model2 = glm::rotate(model2, -t * 1.570796f, glm::vec3(0.0f, 0.0f, 1.0f));
            Graphics::draw(test_draw_id_2, model2);
        }
    };
}
