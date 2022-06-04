//: fresa by jose pazos perez, licensed under GPLv3
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
    bool orbiting = true;
    
    std::map<str, MeshID> mesh_ids{};
    std::map<str, DrawID> draw_ids{};
    std::map<str, MaterialID> material_ids{};
}

void SomeSystem::init() {
    //: Meshes
    for (str obj : {"cube", "sphere", "torus"}) {
        auto [v, i] = Serialization::loadOBJ(obj);
        mesh_ids[obj] = Draw::registerMesh(v, i);
    }
    
    //: Materials
    material_ids["yellow"] = Draw::registerMaterial(glm::vec4(0.9f, 0.8f, 0.4f, 1.0f));
    material_ids["purple"] = Draw::registerMaterial(glm::vec4(0.7f, 0.3f, 0.9f, 1.0f));
    material_ids["blue"] = Draw::registerMaterial(glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
    
    //: Draws
    draw_ids["cube"] = Draw::registerDrawID(mesh_ids["cube"], material_ids["yellow"], {"draw_obj"}, DRAW_DYNAMIC, 1);
    draw_ids["small_cubes"] = Draw::registerDrawID(mesh_ids["cube"], material_ids["blue"], {"draw_obj"}, DRAW_DYNAMIC, 5);
    draw_ids["torus"] = Draw::registerDrawID(mesh_ids["torus"], material_ids["purple"], {"draw_obj"}, DRAW_DYNAMIC, 1);
    draw_ids["sphere"] = Draw::registerDrawID(mesh_ids["sphere"], material_ids["purple"], {"draw_obj"}, DRAW_DYNAMIC, 1);
    draw_ids["floor"] = Draw::registerDrawID(mesh_ids["cube"], material_ids["blue"], {"draw_obj"}, DRAW_STATIC, 1);
    
    //: Floor static position
    auto instance = Draw::getInstanceData(draw_ids["floor"]);
    instance->model = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 200.f, 0.f));
    instance->model = glm::scale(instance->model, glm::vec3(500.f, 20.f, 500.f));
}

void SomeSystem::render() {
    float t = sec(time() - start_time);
    DrawTransformData* instance = nullptr;
    
    CameraTransform camera_transform = Camera::getTransform();
    Shader::updateGlobalUniform("draw_obj", "CameraView", camera_transform);
    
    //: Cube
    instance = Draw::getInstanceData(draw_ids["cube"]);
    instance->model = glm::translate(glm::mat4(1.f), glm::vec3(200.f, std::sin(t) * 80.f, 0.f));
    instance->model = glm::scale(instance->model, glm::vec3(100.f));
    instance->model = glm::rotate(instance->model, t, glm::vec3(0.f, 1.f, 0.f));
    
    //: Small cubes
    instance = Draw::getInstanceData(draw_ids["small_cubes"]);
    for (int i = 0; i < 5; i++) {
        instance[i].model = glm::translate(glm::mat4(1.f), glm::vec3(std::cos(2.f * t - std::pow(0.2f * i, 1.2f) - 0.15f) * 500.f, 50.f, std::sin(2.f * t - std::pow(0.2f * i, 1.2f) - 0.15f) * 500.f));
        instance[i].model = glm::scale(instance[i].model, glm::vec3(10.f - i));
        instance[i].model = glm::rotate(instance[i].model, (1.f + 0.5f * i) * t, glm::vec3(i * 1.f, -i * 1.f, 10.f - i));
    }
    
    //: Torus
    instance = Draw::getInstanceData(draw_ids["torus"]);
    instance->model = glm::translate(glm::mat4(1.f), glm::vec3(-200.f, 0.f, 0.f));
    instance->model = glm::scale(instance->model, glm::vec3(70.f));
    
    //: Sphere
    instance = Draw::getInstanceData(draw_ids["sphere"]);
    instance->model = glm::translate(glm::mat4(1.f), glm::vec3(std::cos(2.f * t) * 500.f, 50.f, std::sin(2.f * t) * 500.f));
    instance->model = glm::scale(instance->model, glm::vec3(30.f));
    
    //: Draw
    for (auto &[name, object] : draw_ids)
        Draw::draw(object);
}

void CameraSystem::init() {
    camera = Camera::create(projections.at(proj_i), glm::vec3(0.f, 0.f, 0.f), 0.f, 0.2f);
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
        //: Orbiting camera
        if (orbiting) {
            float t = sec(time() - start_time);
            camera.pos = glm::vec3(std::cos(t * 0.3f) * 800.f, -140.f, std::sin(t * 0.3f) * 800.f);
            camera.phi = t * 0.3f + 3.14f;
            
            if (Input::key_down(SDLK_w) or Input::key_down(SDLK_s) or
                Input::key_down(SDLK_d) or Input::key_down(SDLK_a) or
                Input::mouse_down(Input::MouseButton::Left)) { orbiting = false; }
        }
        //: Movement
        else {
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
            
            if (Input::key_down(SDLK_o))
                orbiting = true;
        }
    }
    
    if (Input::key_pressed(SDLK_TAB)) {
        proj_i = (proj_i + 1) % projections.size();
        camera.projection = projections.at(proj_i);
        Graphics::resize();
    }
}

/*
 
 //: PLANET WITH RINGS
 
 for (auto &i : objects) {
     float r = std::sqrt((float)(rand() % 1000) / 1000.0f) * 350.0f;
     float theta = (float)(rand() % 314) / 100.0f;
     float phi = (float)(rand() % 628) / 100.0f;
     i = glm::vec3(r, theta, phi);
 }
 
 camera = Camera::create(projections.at(proj_i), glm::vec3(-1030.f, -275.f, 780.f), -0.65f, 0.22f);
 
 //---
 
 auto *instance_cube = Draw::getInstanceData(draw_cube);
 auto *instance_sphere = Draw::getInstanceData(draw_sphere);
 auto *instance_torus = Draw::getInstanceData(draw_torus);
 for (int i = 0; i < objects.size(); i++) {
     DrawTransformData* instance = nullptr;
     if (i < objects.size() * sphere_n)
         instance = &instance_cube[i];
     else if (i < objects.size() * (sphere_n + cube_n))
         instance = &instance_sphere[i - int(objects.size() * sphere_n)];
     else
         instance = &instance_torus[i - int(objects.size() * (sphere_n + cube_n))];
     
     float r = objects.at(i).x, theta = objects.at(i).y, phi = objects.at(i).z;
     float scale = 10.0f + i * 0.01f;
     float rot = i % 50 + t;
     
     float model[16] = {
         scale * std::cos(rot), 0.0f, scale * std::sin(rot), 0.0f,
         0.0f, scale, 0.0f, 0.0f,
         -scale * std::sin(rot), 0.0f, scale * std::cos(rot), 0.0f,
         
         std::cos(theta) * std::sin(phi) * r,
         std::sin(theta) * std::sin(phi) * r + std::sin(i % 200 + t * 2.0f) * 10.0f,
         std::cos(phi) * r,
         1.0f
     };
     memcpy(&instance->model, model, 16 * sizeof(float));
 }
 
 instance_sphere = Draw::getInstanceData(draw_ring);
 for (int i = 0; i < 100; i++) {
     float r = (objects.at(i).x / 350.0f) * 30.0f + 150.0f, phi = objects.at(i).z + t * (200-i) * 0.003f;
     while (phi > 6.28f)
         phi -= 6.28f;
     float scale = 3.0f + i * 0.05f;
     
     float model[16] = {
         scale, 0.0f, 0.0f, 0.0f,
         0.0f, scale, 0.0f, 0.0f,
         0.0f, 0.0f, scale, 0.0f,
         
         std::sin(phi) * r * 3.0f,
         std::sin(1.5f * t) * 200.0f * std::cos(phi + 2.0f),
         std::cos(phi) * r * 3.0f,
         1.0f
     };
     memcpy(&instance_sphere[i].model, model, 16 * sizeof(float));
 }
 
 instance_torus = Draw::getInstanceData(draw_ring_outer);
 for (int i = 0; i < 100; i++) {
     float r = (objects.at(i + 100).x / 350.0f) * 30.0f + 200.0f, phi = objects.at(i + 100).z + t * (110-i) * 0.001f;
     while (phi > 6.28f)
         phi -= 6.28f;
     float scale = 5.0f + i * 0.2f;
     float rot = i % 50 + i / 30.0f * t;
     
     float model[16] = {
         scale * std::cos(rot), 0.0f, scale * std::sin(rot), 0.0f,
         0.0f, scale, 0.0f, 0.0f,
         -scale * std::sin(rot), 0.0f, scale * std::cos(rot), 0.0f,
         
         std::sin(phi) * r * 3.0f,
         std::sin(t) * 200.0f * std::cos(phi),
         std::cos(phi) * r * 3.0f,
         1.0f
     };
     memcpy(&instance_torus[i].model, model, 16 * sizeof(float));
 }
 
 Draw::draw("draw_obj", draw_cube);
 Draw::draw("draw_obj", draw_sphere);
 Draw::draw("draw_obj", draw_torus);
 Draw::draw("draw_obj", draw_ring);
 Draw::draw("draw_obj", draw_ring_outer);
 
 */
