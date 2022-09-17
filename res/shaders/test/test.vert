#version 450

layout(location = 0) in vec2 in_pos;

layout(binding = 0) uniform hello_world_ubo {
    vec4 color;
};

layout(set = 1, binding = 1) uniform sampler2D tex;

layout(binding = 3) buffer hey {
    float a;
    vec3 b;
    int c[];
};

void main() {
    gl_Position = vec4(in_pos, 0.0, 1.0);
}