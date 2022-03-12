//project fresa, 2017-2022
//by jose pazos perez
//licensed under GPLv3 uwu

#pragma once

#include "scene.h"

#include "log.h"
#include "file.h"
#include "input.h"
#include "r_graphics.h"
#include "f_time.h"
#include "load_obj.h"

using namespace Fresa::Graphics;

namespace Fresa::System
{
    //: Example of a hardcoded render system
    //: This is very basic and not elegant, it will be reworked in the future
    
    inline DrawID draw_id_test;
    
    inline Clock::time_point start_time = time();
    
    inline const std::vector<Projection> projections = {
        PROJECTION_ORTHOGRAPHIC,
        Projection(PROJECTION_ORTHOGRAPHIC | PROJECTION_SCALED),
        PROJECTION_PERSPECTIVE,
        Projection(PROJECTION_PERSPECTIVE | PROJECTION_SCALED),
    };
    inline int proj_i = 2;
    
    struct SomeSystem : SystemInit<SomeSystem>, PhysicsUpdate<SomeSystem, PRIORITY_MOVEMENT>, RenderUpdate<SomeSystem> {
        inline static void init() {
            camera.pos.z = 500.0f;
            camera.proj_type = projections.at(proj_i);
            
            auto vertices = Serialization::loadOBJ("test");
            draw_id_test = getDrawID(Vertices::cube_color, Indices::cube, "draw_color");
        }
        
        inline static void update() {
            //: Sample camera update
            if (Input::key_down(SDLK_d))
                camera.pos.x += 300.0f * Time::physics_delta;
            if (Input::key_down(SDLK_a))
                camera.pos.x -= 300.0f * Time::physics_delta;
            
            if (camera.proj_type & PROJECTION_PERSPECTIVE) {
                if (Input::key_down(SDLK_q))
                    camera.pos.y += 300.0f * Time::physics_delta;
                if (Input::key_down(SDLK_e))
                    camera.pos.y -= 300.0f * Time::physics_delta;
                if (Input::key_down(SDLK_w))
                    camera.pos.z -= 300.0f * Time::physics_delta;
                if (Input::key_down(SDLK_s))
                    camera.pos.z += 300.0f * Time::physics_delta;
            } else {
                if (Input::key_down(SDLK_w))
                    camera.pos.y -= 300.0f * Time::physics_delta;
                if (Input::key_down(SDLK_s))
                    camera.pos.y += 300.0f * Time::physics_delta;
            }
            
            if (Input::key_pressed(SDLK_TAB)) {
                proj_i = (proj_i + 1) % projections.size();
                camera.proj_type = projections.at(proj_i);
                updateCameraProjection(camera);
                win.scaled_ubo = (camera.proj_type & PROJECTION_SCALED) ?
                                            API::getScaledWindowUBO(win) :
                                            UniformBufferObject{glm::mat4(1.0f),glm::mat4(1.0f),glm::mat4(1.0f)};
            }
        }
        
        inline static void render() {
            float t = sec(time() - start_time);
            
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 40.0f * std::sin(t * 1.571f), -100.0f));
            model = glm::scale(model, glm::vec3(1.0f) * 100.0f);
            model = glm::rotate(model, t * 1.571f, glm::vec3(0.0f, 1.0f, 0.0f));
            draw(draw_id_test, model);
        }
    };
}
