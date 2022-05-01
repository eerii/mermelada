//project fresa, 2017-2022
//by jose pazos perez
//licensed under GPLv3

#include "system_list.h"

using namespace Fresa;
using namespace Graphics;
using namespace System;

namespace {
    DrawDescription draw_instanced;
    
    Clock::time_point start_time = time();
    
    const std::vector<CameraProjection> projections = {
        PROJECTION_ORTHOGRAPHIC,
        CameraProjection(PROJECTION_ORTHOGRAPHIC | PROJECTION_SCALED),
        PROJECTION_PERSPECTIVE,
        CameraProjection(PROJECTION_PERSPECTIVE | PROJECTION_SCALED),
    };
    int proj_i = 2;
    
    Vec2<int> previous_mouse_pos;
    float mouse_sensitivity = 0.003f;
    
    bool paused = false;
    
    /*struct ObjectBuffer { //: TODO: FIX PADDING IN UNIFORMS
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
    };*/
}

void SomeSystem::init() {
    //: Instancing
    std::vector<VertexExample> per_instance(1000);
    for (auto &i : per_instance) {
        float r = std::sqrt((float)(rand() % 1000) / 1000.0f) * 250.0f;
        float theta = (float)(rand() % 314) / 100.0f;
        float phi = (float)(rand() % 628) / 100.0f;
        i.pos = glm::vec3(std::cos(theta) * std::sin(phi), std::sin(theta) * std::sin(phi), std::cos(phi)) * r;
        i.something = (float)(rand() % 500) / 500.0f;
    }
    draw_instanced = getDrawDescription<UniformBufferObject>(Vertices::cube_color, per_instance, Indices::cube, "draw_color_i");
    
    //: Compute test
    /*updateBufferFromCompute<VertexExample>(api.instanced_buffer_data.at(draw_instanced.instance).instance_buffer, 1000, "compute_test", [](){
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

void SomeSystem::render() {
    float t = sec(time() - start_time);
    
    CameraTransform camera_transform = Camera::getTransform(); //: TODO: Improve how this is handled
    setGlobalUniform<UniformBufferObject, "view">(camera_transform.view);
    setGlobalUniform<UniformBufferObject, "proj">(camera_transform.proj);
    
    UniformBufferObject ubo{};
    ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(-300.0f, std::sin(1.571f * t + 1.571f) * 50.0f, -450.0f));
    ubo.model = glm::scale(ubo.model, glm::vec3(10.0f * (1.0f + 0.3f * std::sin(1.571f * t))));
    ubo.model = glm::rotate(ubo.model, -1.571f * t, glm::vec3(0.0f, 1.0f, 0.0f));
    draw(draw_instanced, ubo);
}

void CameraSystem::init() {
    camera = Camera::create(projections.at(proj_i), glm::vec3(-160.f, -225.f, 315.f), 4.55f, 0.24f);
}

void CameraSystem::update() {
    if (Input::key_pressed(SDLK_ESCAPE))
        paused = not paused;
    
    if (Input::mouse_pressed(Input::MouseButton::Left)) {
        previous_mouse_pos = Input::mouse.position;
    }
    
    if (Input::mouse_down(Input::MouseButton::Left)) {
        Vec2<int> mouse_delta = Input::mouse.position - previous_mouse_pos;
        previous_mouse_pos = Input::mouse.position;
        
        camera.phi -= mouse_delta.x * mouse_sensitivity;
        camera.theta -= mouse_delta.y * mouse_sensitivity;
        camera.theta = std::clamp(camera.theta, -1.0f, 1.0f);
    }
    
    if (not paused) {
        float camera_speed = 500.0f * Time::physics_delta;
        
        if (camera.projection & PROJECTION_PERSPECTIVE) {
            glm::vec3 direction = Camera::getDirection();
            glm::vec3 camera_right = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), direction));
            glm::vec3 camera_up = glm::cross(direction, camera_right);
            
            if (Input::key_down(SDLK_w))
                camera.pos += camera_speed * glm::vec3(direction.x, 0, direction.z);
            if (Input::key_down(SDLK_s))
                camera.pos -= camera_speed * glm::vec3(direction.x, 0, direction.z);
            
            if (Input::key_down(SDLK_d))
                camera.pos += camera_speed * glm::normalize(glm::cross(direction, camera_up));
            if (Input::key_down(SDLK_a))
                camera.pos -= camera_speed * glm::normalize(glm::cross(direction, camera_up));
            
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
    }
    
    if (Input::key_pressed(SDLK_TAB)) {
        proj_i = (proj_i + 1) % projections.size();
        camera.projection = projections.at(proj_i);
    }
}
