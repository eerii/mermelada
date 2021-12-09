#version 450

layout(binding = 1) uniform sampler2D tex_sampler;

layout(location = 0) in vec2 frag_uv;

layout(location = 0) out vec4 out_color;

void main() {
    vec4 tex_color = texture(tex_sampler, frag_uv);
    out_color = vec4(tex_color.rgb, 1.0);
}
