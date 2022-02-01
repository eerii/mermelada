#version 450

layout(input_attachment_index = 0, binding = 0) uniform subpassInput input_color;

layout(location = 0) out vec4 out_color;

void main() {
    vec3 color = subpassLoad(input_color).rgb;
    if (color == vec3(0.0))
        discard;
    out_color = vec4(color.r * 1.2, 0.0, 0.0, 1.0); //color.g * 0.8, color.b, 1.0);
    //out_color = vec4(0.0f, color.r, 1.0f, 1.0f);
    //out_color = vec4(color, 1.0f);
}
