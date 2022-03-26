#version 450

layout(binding = 1) uniform sampler2D attachment;

layout(location = 0) in vec2 frag_uv;

layout(location = 0) out vec4 out_color;

void main() {
    float depth = texture(attachment, frag_uv).r;
    depth = pow(depth, 128);
    out_color = vec4(vec3(depth), 1.0);
}
