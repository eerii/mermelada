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
    
    inline DrawDescription draw_test;
    inline DrawDescription draw_i;
    
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
            
            auto [vertices, indices] = Serialization::loadOBJ("test");
            draw_test = getDrawDescription<UniformBufferObject>(vertices, indices, "draw_obj");
            
            std::vector<VertexExample> per_instance(1000);
            for (auto &i : per_instance) {
                float r = std::sqrt((float)(rand() % 100) / 100.0f) * 200.0f;
                float theta = (float)(rand() % 628) / 100.0f - 3.14f;
                float phi = (float)(rand() % 628) / 100.0f - 3.14f;
                i.pos = glm::vec3(std::cos(theta) * std::sin(phi), std::sin(theta) * std::sin(phi), std::cos(phi)) * r;
            }
            draw_i = getDrawDescriptionI<UniformBufferObject>(Vertices::cube_color, per_instance, Indices::cube, "draw_color_i");
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
            
            setGlobalUniform<UniformBufferObject, "view">(camera.view);
            setGlobalUniform<UniformBufferObject, "proj">(camera.proj);
                
            UniformBufferObject ubo{};
            
            ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(150.0f, 40.0f * std::sin(t * 1.571f), -100.0f));
            ubo.model = glm::scale(ubo.model, glm::vec3(1.0f) * 80.0f);
            ubo.model = glm::rotate(ubo.model, -t * 1.571f, glm::vec3(0.0f, 1.0f, 0.0f));
            draw(draw_test, ubo);
            
            UniformBufferObject ubo2{};
            
            ubo2.model = glm::translate(glm::mat4(1.0f), glm::vec3(-250.0f, 40.0f * std::sin(t * 1.571f), -100.0f));
            ubo2.model = glm::scale(ubo2.model, glm::vec3(1.0f) * 10.0f);
            ubo2.model = glm::rotate(ubo2.model, t * 1.571f, glm::vec3(0.0f, 1.0f, 0.0f));
            draw(draw_i, ubo2);
        }
    };
}
