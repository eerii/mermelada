#version 450

//: Camera uniform
layout(binding = 0) uniform CameraView {
    mat4 view;
    mat4 proj;
} camera;

//: Transform model matrices
layout(std430, binding = 1) buffer TransformBuffer {
    mat4 transform[];
};

//: Material data
struct MaterialData {
    vec4 color;
};
layout(std430, binding = 2) buffer MaterialBuffer {
    MaterialData material[];
};

//: Instance indices for the data
struct InstanceData {
    uint transform;
    uint material;
};
layout(std430, binding = 3) buffer InstanceBuffer {
    InstanceData instance[];
};

//: Vertex attributes
layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

//: Outputs
layout(location = 0) out vec2 frag_uv;
layout(location = 1) out vec3 frag_normal;
layout(location = 2) out vec3 frag_color;

//: Hash pseudo-random functions
uint hash_i(uint x) {
    x ^= x >> 16;
    x *= 0x7feb352dU;
    x ^= x >> 15;
    x *= 0x846ca68bU;
    x ^= x >> 16;
    return x;
}
float hash(uint x) {
    return float(hash_i(x)) / float(0xffffffffU);
}

//: Vertex shader
void main() {
    uint t_id = instance[gl_InstanceIndex].transform;
    uint m_id = instance[gl_InstanceIndex].material;
    
    gl_Position = camera.proj * camera.view * transform[t_id] * vec4(in_pos, 1.0);
    
    frag_uv = in_uv;
    frag_normal = mat3(transpose(inverse(transform[t_id]))) * in_normal;
    frag_color = material[m_id].color.xyz + vec3(hash(gl_InstanceIndex), hash(gl_InstanceIndex + 1000), hash(gl_InstanceIndex + 1000)) * 0.4 - 0.2;
}
