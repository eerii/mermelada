#version 450

layout(binding = 0) uniform sampler2D attachment;

layout(location = 0) in vec2 frag_uv;

layout(location = 0) out vec4 out_color;

void main() {
    vec4 attachment_color = texture(attachment, frag_uv);
    out_color = vec4(attachment_color.rgb, 1.0);
}
