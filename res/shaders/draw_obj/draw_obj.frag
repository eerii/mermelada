#version 450

layout(location = 0) in vec2 frag_uv;
layout(location = 1) in vec3 frag_normal;
layout(location = 2) in vec3 frag_color;

layout(location = 0) out vec4 out_color;

vec3 light_dir = vec3(0.0, 0.5, -1.0);

vec3 diffuse(vec3 light, float intensity, vec3 normal) {
    //: Diffuse
    vec3 light_dir = normalize(-light);
    float diffuse = max(dot(normal, light_dir), 0.0);
    diffuse *= intensity;
    
    return vec3(diffuse);
}

void main() {
    //: Ambient
    vec3 ambient = vec3(0.1);
    
    //: Helpers
    vec3 normal = normalize(frag_normal);
    
    //: Directional light
    vec3 light = diffuse(light_dir, 1.0, normal);
    
    //: Final
    vec3 color = (ambient + light) * frag_color;
    out_color = vec4(color, 1.0);
}
