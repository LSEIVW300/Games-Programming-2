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

    vec3 water
    FragColor = texture(diffuse, texCoord0);  // Use 'texture' instead of 'texture2D'
}
