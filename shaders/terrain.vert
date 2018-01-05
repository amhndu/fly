#version 330 core

layout (location = 0) in vec3  position;
in vec3  normal;
in float color;

out vec3  Normal;
out vec3  LightDirection;
out vec3  FragPosition;
out vec4  FragPosLightSpace;
out float Height;
out float Color;

// vec3 light_direction = normalize(vec3(-0.6, 0.3, 0.9));

uniform vec3 light_direction;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 lightSpace;

void main()
{
    Color             = color;
    Normal            = vec3(mat3(view) * normal);
    Height            = position.z;
    LightDirection    = mat3(view) * light_direction;
    FragPosition      = vec3(view * vec4(position, 1.0));
    FragPosLightSpace = lightSpace * vec4(position, 1.0);
    gl_Position       = proj * view * vec4(position, 1.0);
}
