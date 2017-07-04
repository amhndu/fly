#version 330 core

in vec3 position;
in vec3 normal;
in vec2 texcoords;

out vec2  Texcoords;
out vec3  Normal;
out float Height;
out vec3  LightDirection;
out vec3  FragPosition;

vec3 light_direction = normalize(vec3(0.2, 0.3, 1.0));

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    Texcoords      = texcoords;
    Normal         = vec3(mat3(view * model) * normal);
    Height         = position.z;
    LightDirection = mat3(view) * light_direction;
    FragPosition   = vec3(view * model * vec4(position, 1.0));
    gl_Position    = proj * view * model * vec4(position, 1.0);
}
