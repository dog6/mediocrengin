#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D diffuseMap;
uniform vec3 baseColor;

void main()
{
    vec4 texColor = texture(diffuseMap, TexCoord);
    FragColor = texColor * vec4(baseColor, 1.0);
}