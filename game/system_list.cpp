//project fresa, 2017-2022
//by jose pazos perez
//licensed under GPLv3

#include "system_list.h"

using namespace Fresa;
using namespace Graphics;
using namespace System;

namespace {
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
    
    MeshID mesh_cube;
    inline std::vector<glm::vec3> objects(2000);
    DrawID draw_test;
    DrawID draw_test_2;
    
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
    for (auto &i : objects) {
        float r = std::sqrt((float)(rand() % 1000) / 1000.0f) * 250.0f;
        float theta = (float)(rand() % 314) / 100.0f;
        float phi = (float)(rand() % 628) / 100.0f;
        i = glm::vec3(r, theta, phi);
    }
    
    mesh_cube = Draw::registerMesh(Vertices::cube_color, Indices::cube);
    
    draw_test = Draw::registerDrawID(mesh_cube, DrawBatchType(DRAW_INSTANCES | DRAW_DYNAMIC), 1000);
    draw_test_2 = Draw::registerDrawID(mesh_cube, DrawBatchType(DRAW_SINGLE_OBJECT | DRAW_DYNAMIC), 1);
}

void SomeSystem::render() {
    float t = sec(time() - start_time);
    
    CameraTransform camera_transform = Camera::getTransform();
    Shader::updateGlobalUniform(ShaderID{"draw_color_i"}, "CameraView", camera_transform);
    
    auto *instance = Draw::getInstanceData(draw_test);
    for (int i = 0; i < 1000; i++) {
        float r = objects.at(i).x, theta = objects.at(i).y, phi = objects.at(i).z;
        glm::vec3 pos = glm::vec3(std::cos(theta) * std::sin(phi), std::sin(theta) * std::sin(phi), std::cos(phi)) * r;
        instance[i].model = glm::translate(glm::mat4(1.0f), pos + glm::vec3(0.0f, std::pow(std::sin(t + phi), 2) * (r / 10.0f), 0.0f));
        instance[i].model = glm::scale(instance[i].model, glm::vec3(10.0f));
    }
    
    instance = Draw::getInstanceData(draw_test_2);
    instance->model = glm::translate(glm::mat4(1.0f), glm::vec3(500.0f, 0.0f, 0.0f));
    instance->model = glm::scale(instance->model, glm::vec3(100.0f));
    
    Draw::draw(ShaderID{"draw_color_i"}, draw_test);
    Draw::draw(ShaderID{"draw_color_i"}, draw_test_2);
}

void CameraSystem::init() {
    camera = Camera::create(projections.at(proj_i), glm::vec3(400.f, -25.f, 670.f), 4.18f, 0.03f);
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
