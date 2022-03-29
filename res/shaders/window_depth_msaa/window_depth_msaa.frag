#version 450

layout(binding = 1) uniform sampler2DMS attachment;

layout(location = 0) in vec2 frag_uv;

layout(location = 0) out vec4 out_color;

void main() {
    ivec2 tex_size = textureSize(attachment);
    ivec2 tex_coord = ivec2(frag_uv * tex_size);
    
    float depth = texelFetch(attachment, tex_coord, 1).r;
    depth = pow(depth, 128);
    
    out_color = vec4(vec3(depth), 1.0);
}
