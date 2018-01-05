#version 330 core

layout (location = 0) in vec3 position;
in vec3 normal;
// in vec2 texcoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec3 light_direction;

out vec3 Normal;
out vec3 FragPosition;
out vec3 LightDirection;

//vec3 light_direction = normalize(vec3(-0.6, 0.3, 0.9));

void main()
{
    mat4 view_model = view * model;
    LightDirection  = mat3(view) * light_direction;
    FragPosition    = vec3(view_model * vec4(position, 1.0));
    Normal          = vec3(mat3(view_model) * normal);  // NOTE: This doesn't work with non-uniform scaling
    // Normal = mat3(transpose(inverse(model))) * normal; // works with non-uniform scaling
    gl_Position     = proj * view_model * vec4(position, 1.0);
}
