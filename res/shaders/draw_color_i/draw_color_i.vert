#version 450

layout(binding = 0) uniform CameraView {
    mat4 view;
    mat4 proj;
} camera;

layout(std430, binding = 1) buffer InstanceBuffer {
    mat4 model[];
} instance;

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_color;

layout(location = 0) out vec3 frag_color;

void main() {
    gl_Position = camera.proj * camera.view * instance.model[gl_InstanceIndex] * vec4(vertex_pos, 1.0);
    frag_color = instance.model[gl_InstanceIndex][3].rgb / 150.0 +  vec3(0.4);
}
