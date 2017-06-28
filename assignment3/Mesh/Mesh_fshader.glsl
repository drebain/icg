#version 330 core
out vec3 color;

in vec2 uv;

uniform sampler2D height_map;
uniform sampler2D diffuse_map;

void main() {

    float tiling_amount = 5;

    float height_center = texture(height_map, uv).r;
    float height_plus_u = 0; // TODO: get offset height values
    float height_plus_v = 0;
    
    vec3 N = vec3(0, 1, 0); // Calculate normal from height differences

    vec3 light = normalize(vec3(1,3,0));

    vec3 ambient = vec3(0.1, 0.1, 0.2);
    vec3 diffuse = texture(diffuse_map, uv * tiling_amount).rgb * clamp(dot(N, light), 0, 1);

    // Optional TODO: add specular term
    // Hint: you will need the world space position of each pixel.
    // You can easily get this from the vertex shader where it is
    // already calculated
        
    color = ambient + diffuse;

}
