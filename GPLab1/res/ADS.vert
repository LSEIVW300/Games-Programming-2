#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 transform;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
    FragPos = vec3(model * vec4(VertexPosition, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = VertexTexCoord;

    gl_Position = transform * vec4(VertexPosition, 1.0);
}