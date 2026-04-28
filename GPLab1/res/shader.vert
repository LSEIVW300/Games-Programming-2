#version 400 core

layout (location = 0) in vec3 position;  
layout (location = 1) in vec2 texCoord;

out vec2 texCoord0;  // 'varying' is replaced with 'out'
out vec3 worldPos;


uniform mat4 transform;

void main()
{
    worldPos = position;
    gl_Position = transform * vec4(position, 1.0);

    texCoord0 = texCoord * 8.0;  // Pass the texture coordinates to the fragment shader
}
