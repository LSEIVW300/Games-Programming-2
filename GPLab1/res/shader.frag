#version 400 core

in vec2 texCoord0;
in vec3 worldPos;

uniform sampler2D diffuse;
uniform float time;

out vec4 FragColor; // 'out' is used for fragment output

void main()
{
    vec2 uv1 = texCoord0;
    vec2 uv2 = texCoord0;

    uv1.x += time * 0.04;
    uv1.y += time * 0.02;

    uv2.x -= time * 0.025;
    uv2.y += time * 0.035;

    vec3 waterA = texture(diffuse, uv1).rgb;
    vec3 waterB = texture(diffuse, uv2).rgb;

    vec3 waterColour = mix(waterA, waterB, 0.5);

    float distanceFade = clamp(1.0 - (texCoord0.y / 8.0), 0.0, 1.0);
    vec3 deepWater = vec3(0.0, 0.18, 0.45);

    waterColour = mix(waterColour, deepWater, distanceFade * 0.45);

    FragColor = vec4(waterColour, 1.0);
}
