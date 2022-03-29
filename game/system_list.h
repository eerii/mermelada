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
    
    inline DrawDescription draw_cube;
    inline DrawDescription draw_plane;
    inline DrawDescription draw_light;
    
    inline Clock::time_point start_time = time();
    
    inline const std::vector<Projection> projections = {
        PROJECTION_ORTHOGRAPHIC,
        Projection(PROJECTION_ORTHOGRAPHIC | PROJECTION_SCALED),
        PROJECTION_PERSPECTIVE,
        Projection(PROJECTION_PERSPECTIVE | PROJECTION_SCALED),
    };
    inline int proj_i = 2;
    
    inline float phi = -1.57f;
    inline float theta = 0.0f;
    inline const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    inline Vec2<int> previous_mouse_pos;
    inline float mouse_sensitivity = 0.003f;
    
    inline bool paused = false;
    
    struct ObjectBuffer { //: TODO: FIX PADDING IN UNIFORMS
        Members(ObjectBuffer, model, view, proj, camera_pos, _, color)
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
        glm::vec3 camera_pos; float _;
        glm::vec3 color;
    };
    
    struct LightBuffer {
        Members(LightBuffer, directional_light, point_lights, point_light_colors, point_light_count)
        glm::vec4 directional_light;
        std::array<glm::vec4, 16> point_lights;
        std::array<glm::vec4, 16> point_light_colors;
        int point_light_count;
    };
    
    struct SomeSystem : SystemInit<SomeSystem>, RenderUpdate<SomeSystem> {
        inline static void init() {
            auto [vertices, indices] = Serialization::loadOBJ("test");
            draw_cube = getDrawDescription<ObjectBuffer, LightBuffer>(vertices, indices, "draw_light");
            draw_plane = getDrawDescription<ObjectBuffer, LightBuffer>(vertices, indices, "draw_light");
            draw_light = getDrawDescription<UniformBufferObject>(vertices, indices, "draw_obj");
            
            /*//: Instancing
            std::vector<VertexExample> per_instance(1000);
            draw_i = getDrawDescriptionI<UniformBufferObject>(Vertices::cube_color, per_instance, Indices::cube, "draw_color_i");
            
            //: Compute test
            API::updateBufferFromCompute<VertexExample>(api, API::instanced_buffer_data.at(draw_i.instance).instance_buffer, 1000, "compute_test", [](){
                std::vector<VertexExample> per_instance(1000);
                for (auto &i : per_instance) {
                    float r = std::sqrt((float)(rand() % 1000) / 1000.0f) * 120.0f;
                    float theta = (float)(rand() % 314) / 100.0f;
                    float phi = (float)(rand() % 628) / 100.0f;
                    i.pos = glm::vec3(std::cos(theta) * std::sin(phi), std::sin(theta) * std::sin(phi), std::cos(phi)) * r;
                    i.something = (float)(rand() % 300) / 100.0f;
                }
                return per_instance;
            });*/
        }
        
        inline static void render() {
            float t = sec(time() - start_time);
            
            setGlobalUniform<ObjectBuffer, "view">(camera.view);
            setGlobalUniform<ObjectBuffer, "proj">(camera.proj);
            setGlobalUniform<ObjectBuffer, "camera_pos">(camera.pos);
            
            setGlobalUniform<LightBuffer, "directional_light">(glm::vec4(1.0f, 0.5f, -0.3f, 1.0f));
            setGlobalUniform<LightBuffer, "point_light_count">(1);
            
            /*std::array<glm::vec4, 16> point_lights;
            point_lights[0] = glm::vec4(0.0f, -30.0f, 250.0f, 500.0f);
            setGlobalUniform<LightBuffer, "point_lights">(point_lights);
            
            std::array<glm::vec4, 16> point_light_colors;
            point_light_colors[0] = glm::vec4(1.0f, 0.6f, 0.8f, 1.0f);
            setGlobalUniform<LightBuffer, "point_light_colors">(point_light_colors);*/
            
            LightBuffer light{};
            
            ObjectBuffer ubo{};
            ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -50.0f, 0.0f));
            ubo.model = glm::scale(ubo.model, glm::vec3(80.0f));
            ubo.model = glm::rotate(ubo.model, -t * 1.571f, glm::vec3(0.0f, 1.0f, 0.0f));
            ubo.color = glm::vec3(0.5, 0.5, 1.0);
            draw(draw_cube, ubo, light);
            
            ObjectBuffer ubo2{};
            ubo2.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 100.0f, 0.0f));
            ubo2.model = glm::scale(ubo2.model, glm::vec3(300.0f, 10.0f, 300.0f));
            ubo2.color = glm::vec3(1.0, 0.5, 0.5);
            draw(draw_plane, ubo2, light);
            
            setGlobalUniform<UniformBufferObject, "view">(camera.view);
            setGlobalUniform<UniformBufferObject, "proj">(camera.proj);
            
            UniformBufferObject ubo3{};
            ubo3.model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.5f, -0.3f) * (-300.0f));
            ubo3.model = glm::scale(ubo3.model, glm::vec3(10.0f));
            draw(draw_light, ubo3);
        }
    };
    
    struct CameraSystem : SystemInit<CameraSystem>, PhysicsUpdate<CameraSystem, PRIORITY_MOVEMENT> {
        inline static void init() {
            camera.pos = glm::vec3(-340.f, -40.f, 680.f);
            camera.direction = glm::vec3(0.47f, 0.03f, -0.87f);
            //camera.direction = glm::normalize(glm::vec3(std::cos(phi) * std::cos(theta), std::sin(theta), std::sin(phi) * std::cos(theta)));
            camera.proj_type = projections.at(proj_i);
        }
        
        inline static void update() {
            if (Input::key_pressed(SDLK_ESCAPE))
                paused = not paused;
            
            if (Input::mouse_pressed(Input::MouseButton::Left)) {
                previous_mouse_pos = Input::mouse.position;
            }
            
            if (Input::mouse_down(Input::MouseButton::Left)) {
                Vec2<int> mouse_delta = Input::mouse.position - previous_mouse_pos;
                previous_mouse_pos = Input::mouse.position;
                
                phi -= mouse_delta.x * mouse_sensitivity;
                theta -= mouse_delta.y * mouse_sensitivity;
                theta = std::clamp(theta, -1.0f, 1.0f);
                
                camera.direction = glm::normalize(glm::vec3(std::cos(phi) * std::cos(theta), std::sin(theta), std::sin(phi) * std::cos(theta)));
            }
            
            if (not paused) {
                glm::vec3 camera_right = glm::normalize(glm::cross(up, camera.direction));
                glm::vec3 camera_up = glm::cross(camera.direction, camera_right);
                
                float camera_speed = 500.0f * Time::physics_delta;
                
                if (camera.proj_type & PROJECTION_PERSPECTIVE) {
                    if (Input::key_down(SDLK_w))
                        camera.pos += camera_speed * glm::vec3(camera.direction.x, 0, camera.direction.z);
                    if (Input::key_down(SDLK_s))
                        camera.pos -= camera_speed * glm::vec3(camera.direction.x, 0, camera.direction.z);
                    
                    if (Input::key_down(SDLK_d))
                        camera.pos += camera_speed * glm::normalize(glm::cross(camera.direction, camera_up));
                    if (Input::key_down(SDLK_a))
                        camera.pos -= camera_speed * glm::normalize(glm::cross(camera.direction, camera_up));
                    
                    if (Input::key_down(SDLK_q))
                        camera.pos.y += camera_speed;
                    if (Input::key_down(SDLK_e))
                        camera.pos.y -= camera_speed;
                } else {
                    if (Input::key_down(SDLK_w))
                        camera.pos.y -= camera_speed;
                    if (Input::key_down(SDLK_s))
                        camera.pos.y += camera_speed;
                    
                    if (Input::key_down(SDLK_d))
                        camera.pos.x += camera_speed;
                    if (Input::key_down(SDLK_a))
                        camera.pos.x -= camera_speed;
                }
                
                camera.view = glm::lookAt(camera.pos, camera.pos + camera.direction, camera_up);
            }
            
            if (Input::key_pressed(SDLK_TAB)) {
                proj_i = (proj_i + 1) % projections.size();
                camera.proj_type = projections.at(proj_i);
                updateCameraProjection(camera);
                win.scaled_ubo = (camera.proj_type & PROJECTION_SCALED) ?
                                  API::getScaledWindowUBO(win) : UniformBufferObject{glm::mat4(1.0f),glm::mat4(1.0f),glm::mat4(1.0f)};
            }
        }
    };
}
