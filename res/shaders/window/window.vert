#version 450

layout(binding = 0) uniform WindowTransform {
    mat4 model;
    mat4 proj;
} transform;

layout(location = 0) in vec2 in_pos;

layout(location = 0) out vec2 frag_uv;

void main() {
    gl_Position = transform.proj * transform.model * vec4(in_pos, 0.0, 1.0);
    frag_uv = vec2(clamp(in_pos.x, 0, 1), clamp(in_pos.y, 0, 1));
}
