#version 330

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 Albedo;

uniform sampler2D diffuseMap;
uniform bool hasDiffuseMap;

uniform vec3 sunDirection;   // Direction the sun is shining
uniform vec3 sunColor;       // Color of sunlight (usually warm white/yellow)
uniform vec3 viewPos;        // Camera position for specular

void main()
{

    // Normalize the normal vector
    vec3 norm = normalize(Normal);
    
    // Sun is a directional light (all rays parallel)
    vec3 lightDir = normalize(-sunDirection);  // -sunDirection because we want direction TO light
    
    // Ambient lighting
    // Minimum light level (prevents completely black shadows)
    float ambientStrength = 0.4;  // Minecraft-style has fairly bright ambient
    vec3 ambient = ambientStrength * sunColor;
    
    // Diffuse lighting
    // Surfaces facing the sun are brighter
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * sunColor;
    
    // Shiny highlights where sun reflects into camera
    float specularStrength = 0.3;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);  // Lower shininess for blocks
    vec3 specular = specularStrength * spec * sunColor;
    
    // Combine lighting
    vec3 lighting = ambient + diffuse + specular;
    
    // Apply to texture/color

    vec3 result;
    if (hasDiffuseMap) {
        vec4 texColor = texture(diffuseMap, TexCoords);
        result = lighting * Albedo * texColor.rgb;
    }else {
        result = lighting * Albedo;
    }
    
    FragColor = vec4(result, 1.0);
    
}