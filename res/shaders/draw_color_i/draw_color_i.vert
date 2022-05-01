#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(std430, binding = 1) buffer TestBuffer {
    vec4 positions[];
} test;

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_color;

layout(location = 0) out vec3 frag_color;

void main() {
    gl_Position = ubo.proj * ubo.view * (ubo.model * vec4(vertex_pos, 1.0) + vec4(test.positions[gl_InstanceIndex].xyz, 0.0));
    frag_color = vec3(test.positions[gl_InstanceIndex].w);
}
