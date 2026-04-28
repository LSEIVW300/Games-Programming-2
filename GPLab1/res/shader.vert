#version 400 core

layout (location = 0) in vec3 position;  
layout (location = 1) in vec2 texCoord;

out vec2 texCoord0;  // 'varying' is replaced with 'out'
out vec3 worldPos;

uniform mat4 transform;
uniform mat4 model;
uniform float time;

void main()
{
    vec3 pos = position;

    pos.y += sin((position.x * 3.0) + time * 2.0) * 0.03;
    pos.y += cos((position.z * 3.0) + time * 1.5) * 0.03;

    vec4 worldPosition = model * vec4(pos, 1.0);
    worldPos = worldPosition.xyz;

    gl_Position = transform * vec4(pos, 1.0);

    texCoord0 = texCoord * 6.0;  // Pass the texture coordinates to the fragment shader
}
