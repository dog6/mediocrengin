#version 330 core
out vec4 FragColor;

uniform vec3 baseColor;

void main() {
    FragColor = vec4(baseColor, 1); 
}
