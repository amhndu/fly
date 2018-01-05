#version 330 core

in vec3  Normal;
in vec3  LightDirection;
in vec3  FragPosition;
in vec4  FragPosLightSpace;
in float Height;
in float Color;

layout (location = 0) out vec4 outColor;

uniform sampler2D tex;
uniform sampler2D ShadowMap;

float underShadow()
{
    if (length(FragPosition) > 3.f)
        return 1.f;

    // perform perspective divide
    // redundant because we use orthographic projection to create the depth map
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if(projCoords.z > 1.0)
        return 1.0;
    float closestDepth = texture(ShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = 0;//0.00003;//max(0.005 * (1.0 - dot(Normal, LightDirection)), 0.0005); //0.0005;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(ShadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 0.0 : 1.0;
        }
    }
    shadow /= 9.0;
    return shadow;
}

vec3 light_color = vec3( 1.0,  1.0,  1.0);

void main()
{
    vec3 color = vec3(texture(tex, vec2(1.0 - (Height / 1.5), Color)));

    vec3 norm = normalize(Normal);

    // ambient
    float ambient_strength = 0.2;
    vec3 ambient = ambient_strength * light_color * color;  // ambient strength = 0.1

    // diffuse
    float diffuse_strength = 0.8;
    vec3 diffuse = diffuse_strength * max(dot(norm, LightDirection), 0.0) * light_color * color;

    float alpha = 1.0;
    float distance = length(FragPosition);
    const float fog_begin = 7.0;
    if (distance >= fog_begin)
        alpha = exp((fog_begin - distance) / 8.0);

    vec3 perspective_color = vec3(181 / 255.0, 208 / 255.0, 212 / 255.0);

    vec3 final_color = mix(perspective_color, ambient + underShadow() * diffuse, alpha);
    outColor = vec4(vec3(final_color), 1.0);
}
