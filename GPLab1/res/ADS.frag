#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform sampler2D diffuse;

out vec4 FragColor;

void main()
{
    vec3 textureColor = texture(diffuse, TexCoord).rgb;

    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseLight = diff * lightColor;

    vec3 result = (ambient + diffuseLight) * textureColor * objectColor;

    FragColor = vec4(result, 1.0);
}