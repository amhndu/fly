#version 330 core

in vec3 position;

uniform mat4 view;
uniform mat4 proj;

out vec3 Position;

void main()
{
    Position    = vec3(position.xy, position.z);
    gl_Position = (proj * view * vec4(Position, 1.0)).xyww;
}
