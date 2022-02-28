//project verse, 2017-2022
//by jose pazos perez
//licensed under GPLv3 uwu

#pragma once

#include "scene.h"

#include "log.h"
#include "input.h"
#include "r_graphics.h"
#include "f_time.h"

using namespace Fresa::Graphics;

namespace Fresa::System
{
    //: Example of a hardcoded render system
    //: This is very basic and not elegant, it will be reworked in the future

    inline TextureID texture_data;
    inline DrawID draw_id_tex_a;
    inline DrawID draw_id_tex_b;
    inline std::array<DrawID, 20> draw_ids_cube{};
    inline std::array<DrawID, 16> draw_ids_floor{};
    
    inline Clock::time_point start_time = time();
    
    inline std::array<glm::vec3, 20> positions{};
    inline std::array<glm::vec3, 20> rotations{};
    inline std::array<glm::vec3, 16> positions_floor{};
    
    inline const std::vector<Projection> projections = {
        PROJECTION_ORTHOGRAPHIC,
        Projection(PROJECTION_ORTHOGRAPHIC | PROJECTION_SCALED),
        PROJECTION_PERSPECTIVE,
        Projection(PROJECTION_PERSPECTIVE | PROJECTION_SCALED),
    };
    inline int proj_i = 0;
    
    struct SomeSystem : PhysicsUpdate<SomeSystem, PRIORITY_MOVEMENT>, RenderUpdate<SomeSystem> {
        inline static void update() {
            /*Scene& s = scene_list.at(active_scene);
            for (EntityID e : SceneView<Component::Test>(s)) {
                log::info("%d %s", e, s.getName(e).c_str());
            }*/
            
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
            static bool init = false;
            if (not init) {
                texture_data = getTextureID(File::path("graphics/texture.png"));
                draw_id_tex_a = getDrawID(Vertices::rect3_tex, Indices::rect, "draw_tex");
                bindTexture(draw_id_tex_a, texture_data);
                draw_id_tex_b = getDrawID(Vertices::rect3_tex, Indices::rect, "draw_tex");
                bindTexture(draw_id_tex_b, texture_data);
                
                for (int i = 0; i < draw_ids_cube.size(); i++) {
                    draw_ids_cube[i] = getDrawID(Vertices::cube_color, Indices::cube, "draw_color");
                    float theta_p = float(rand() % 31415) / 10000.0f; //: 0, pi
                    float phi_p = float(rand() % 62829) / 10000.0f; //: 0, 2*pi
                    positions[i] = glm::vec3(std::cos(theta_p) * std::sin(phi_p), std::sin(theta_p) * std::sin(phi_p), std::cos(phi_p));
                    positions[i] *= std::sqrt(float(rand() % 1000) / 1000.0f) * 400.0f;
                    float theta_r = float(rand() % 31415) / 10000.0f; //: 0, pi
                    float phi_r = float(rand() % 62829) / 10000.0f; //: 0, 2*pi
                    rotations[i] = glm::vec3(std::cos(theta_r) * std::sin(phi_r), std::sin(theta_r) * std::sin(phi_r), std::cos(phi_r));
                }
                
                for (int i = -2; i < 2; i++) {
                    for (int j = -2; j < 2; j++) {
                        int k = (i+2) + 4*(j+2);
                        draw_ids_floor[k] = getDrawID(Vertices::cube_color, Indices::cube, "draw_color");
                        positions_floor[k] = glm::vec3(200.0f * i + 100.0f, 250.0f, 200.0f * j + 100.0f);
                    }
                }
                
                camera.pos.z = 500.0f;
                
                init = true;
            }
            
            float t = sec(time() - start_time);
            
            //: Texture
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 100.0f * std::sin(t * 1.570796f), -100.0f));
            model = glm::scale(model, glm::vec3(1.0f) * 100.0f);
            model = glm::rotate(model, t * 3.141592f, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
            draw(draw_id_tex_a, model);
            #ifndef USE_OPENGL
            model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 100.0f * std::sin(t * 1.570796f), -100.0f));
            model = glm::scale(model, glm::vec3(1.0f) * 100.0f);
            model = glm::rotate(model, (t + 1.0f) * 3.141592f, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
            draw(draw_id_tex_b, model); //Quick hack to avoid disabling culling
            #endif
            
            //: Cubes
            for (int i = 0; i < draw_ids_cube.size(); i++) {
                model = glm::translate(glm::mat4(1.0f), positions[i]);
                model = glm::scale(model, glm::vec3(1.0f) * 10.0f);
                model = glm::rotate(model, t * 1.570796f, rotations[i]);
                draw(draw_ids_cube[i], model);
            }
            
            //: Floor
            for (int i = 0; i < draw_ids_floor.size(); i++) {
                model = glm::translate(glm::mat4(1.0f), positions_floor[i]);
                model = glm::scale(model, glm::vec3(100.0f, 30.0f, 100.0f));
                draw(draw_ids_floor[i], model);
            }
        }
    };
}
