#version 450

layout(binding = 1) uniform LightBuffer {
    vec4 directional; //: direction + intensity
    vec4 point[16]; //: position + radius
    vec4 point_color[16]; //: color + intensity
    int point_count;
} data;

layout(location = 0) in vec3 camera_pos;
layout(location = 1) in vec3 frag_normal;
layout(location = 2) in vec3 frag_color;
layout(location = 3) in vec3 frag_pos;

layout(location = 0) out vec4 out_color;

int shininess = 64;
float specular_intensity = 0.3;

vec3 directional(vec3 light, float intensity, vec3 normal, vec3 view_dir) {
    //: Diffuse
    vec3 light_dir = normalize(-light);
    float diffuse = max(dot(normal, light_dir), 0.0);
    diffuse *= intensity;
    
    //: Specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float specular = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
    specular *= specular_intensity * intensity;
    
    return vec3(diffuse + specular);
}

vec3 point(vec3 light, float radius, vec3 color, float intensity, vec3 normal, vec3 view_dir) {
    //: Diffuse
    vec3 light_dir = normalize(light - frag_pos);
    float diffuse = max(dot(normal, light_dir), 0.0);
    diffuse *= intensity;
    
    //: Specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float specular = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
    specular *= specular_intensity * intensity;
    
    //: Attenuation
    float d = length(light - frag_pos);
    float a = 2.0 / radius;
    float b = 1.0 / (radius*radius);
    float attenuation =  1.0 / (1.0 + a * d + b * (d*d));
    
    return vec3(diffuse + specular) * color * attenuation;
}

void main() {
    //: Ambient
    vec3 ambient = vec3(0.1);
    
    //: Helpers
    vec3 normal = normalize(frag_normal);
    vec3 view_dir = normalize(camera_pos - frag_pos);
    
    //: Directional light
    vec3 light = directional(data.directional.xyz, data.directional.w, normal, view_dir);
    
    //: Point light
    for (int i = 0; i < data.point_count; i++)
        light += point(data.point[i].xyz, data.point[i].w, data.point_color[i].xyz, data.point_color[i].w, normal, view_dir);
    
    //: Final
    vec3 color = (ambient + light) * frag_color;
    out_color = vec4(color, 1.0);
}
