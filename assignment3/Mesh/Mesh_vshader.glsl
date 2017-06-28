#version 330 core 
in vec3 vpoint;
in vec3 vnormal;

out vec2 uv;

///--- Uploaded by Eigen in C++
uniform mat4 MODEL;
uniform mat4 VIEW;
uniform mat4 PROJ;

uniform sampler2D height_map;


void main() {

     uv = (vpoint.xz + vec2(1.0f,1.0f)) / 2.0f;

    float displacement = texture(height_map, uv).r;
    vec4 world = MODEL * vec4(vpoint + vec3(0, displacement, 0), 1.0);
    gl_Position = PROJ * VIEW * world;

}
