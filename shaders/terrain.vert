#version 330 core

in vec3  position;
in vec3  normal;
in float color;

out vec3  Normal;
out vec3  LightDirection;
out vec3  FragPosition;
out float Height;
out float Color;

vec3 light_direction = normalize(vec3(0.2, 0.3, 1.0));

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    Color          = color;
    Normal         = vec3(mat3(view * model) * normal);
    Height         = position.z;
    LightDirection = mat3(view) * light_direction;
    FragPosition   = vec3(view * model * vec4(position, 1.0));
    gl_Position    = proj * view * model * vec4(position, 1.0);
}
