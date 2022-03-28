#version 450

layout(binding = 0) uniform ObjectBuffer {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 camera_pos;
    vec3 color;
} ubo;

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

layout(location = 0) out vec3 camera_pos;
layout(location = 1) out vec3 frag_normal;
layout(location = 2) out vec3 frag_color;
layout(location = 3) out vec3 frag_pos;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(in_pos, 1.0);
    
    camera_pos = ubo.camera_pos;
    
    frag_normal = mat3(transpose(inverse(ubo.model))) * in_normal; //: Avoid inverse in the future
    frag_color = ubo.color;
    frag_pos = vec3(ubo.model * vec4(in_pos, 1.0));
}
