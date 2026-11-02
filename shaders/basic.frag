#version 330 core
in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightDir;
uniform vec3 objectColor;

out vec4 FragColor;

void main()
{
    vec3 n = normalize(Normal);
    float diff = max(dot(n, normalize(-lightDir)), 0.0);
    vec3 color = (0.2 + diff) * objectColor;
    FragColor = vec4(color, 1.0);
}