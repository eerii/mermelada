#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

layout(location = 0) out vec2 frag_uv;
layout(location = 1) out vec3 frag_normal;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(in_pos, 1.0);
    frag_uv = in_uv;
    frag_normal = normalize((ubo.model * vec4(in_normal, 1.0)).xyz);
}
