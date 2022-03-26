#version 450

layout(input_attachment_index = 0, binding = 0) uniform subpassInput input_color;

layout(location = 0) out vec4 out_color;

void main() {
    vec3 color = subpassLoad(input_color).rgb;
    out_color = vec4(color.x * 1.1, color.g * 0.95, color.b * 0.9, 1.0);
}
