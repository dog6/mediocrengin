#version 330

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main() {
    // Transform position to world space
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Transform normal to world space (using normal matrix)
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    TexCoords = aTexCoord;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}