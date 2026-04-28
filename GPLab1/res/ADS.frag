#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform sampler2D diffuse;

out vec4 FragColor;

void main()
{
    vec3 textureColor = texture(diffuse, TexCoord).rgb;

    float ambientStrength = 0.45;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseLight = diff * lightColor * 0.9;

    float specularStrength = 1.2;
    float shininess = 24.0;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuseLight) * textureColor * objectColor + specular;

    FragColor = vec4(result, 1.0);
}