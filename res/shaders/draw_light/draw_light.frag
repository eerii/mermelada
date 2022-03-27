#version 450

layout(location = 0) in vec3 directional_light;
layout(location = 1) in vec3 camera_pos;
layout(location = 2) in vec3 frag_normal;
layout(location = 3) in vec3 frag_color;
layout(location = 4) in vec3 frag_pos;

layout(location = 0) out vec4 out_color;

float specular_intensity = 0.5;
int shininess = 8;

void main() {
    //: Ambient
    vec3 ambient = vec3(0.1);
    
    //: Diffuse
    vec3 normal = normalize(frag_normal);
    vec3 light_dir = normalize(-directional_light);
    float diffuse_light = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = vec3(diffuse_light);
    
    //: Specular
    vec3 view_dir = normalize(camera_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float specular_light = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
    vec3 specular = vec3(specular_light * specular_intensity);
    
    //: Final
    vec3 light = (ambient + diffuse + specular) * frag_color;
    out_color = vec4(light, 1.0);
}
