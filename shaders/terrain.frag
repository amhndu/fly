#version 330 core

in vec2  Texcoords;
in vec3  Normal;
in float Height;
in vec3  LightDirection;
in vec3  FragPosition;

layout (location = 0) out vec4 outColor;

uniform sampler2D tex;

vec3 light_color = vec3( 1.0,  1.0,  1.0);

void main()
{
    vec3 color = mix(vec3(0, 135.0 / 255.0, 49.0 / 255.0), vec3(0, 168.0 / 255, 62.0 / 255.0), Height);
    color *= vec3(texture(tex, Texcoords)) / 3 + vec3(0.6, 0.6, 0.6);

    vec3 norm = normalize(Normal);

    // ambient
    float ambient_strength = 0.4;
    vec3 ambient = ambient_strength * light_color * color;  // ambient strength = 0.1

    // diffuse
    float diffuse_strength = 0.8;
    vec3 diffuse = diffuse_strength * max(dot(norm, LightDirection), 0.0) * light_color * color;

    vec3 final_color = ambient + diffuse;

    float alpha = 1.0;
    float distance = length(FragPosition);
    if (distance >= 13.0)
        alpha = 1.0 / (distance - 12.0);

    outColor = vec4(mix(vec3(0.39, 0.98, 1.0), final_color, alpha), 1.0);
}
