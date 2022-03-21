#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_color;

layout(location = 2) in vec3 instance_pos;
layout(location = 3) in float instance_color;

layout(location = 0) out vec3 frag_color;

void main() {
    gl_Position = ubo.proj * ubo.view * (ubo.model * vec4(vertex_pos, 1.0) + vec4(instance_pos, 0.0));
    frag_color = vertex_color;
    if (instance_color < 1.0)
        frag_color.r += instance_color * 0.5;
    else if (instance_color < 2.0)
        frag_color.g += (instance_color - 1.0) * 0.5;
    else
        frag_color.b += (instance_color - 2.0) * 0.5;
}
