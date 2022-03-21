#version 450

layout(location = 0) in vec2 frag_uv;
layout(location = 1) in vec3 frag_normal;

layout(location = 0) out vec4 out_color;

void main() {
    out_color = vec4(0.5 + frag_normal.yzx * 0.5, 1.0);
}
