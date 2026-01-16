#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// Material colors
uniform vec3 Albedo;

// Texture maps
uniform sampler2D diffuseMap;
uniform bool hasDiffuseMap;

uniform sampler2D specularMap;
uniform bool hasSpecularMap;

uniform sampler2D normalMap;
uniform bool hasNormalMap;

uniform sampler2D emissiveMap;
uniform bool hasEmissiveMap;

uniform sampler2D alphaMap;
uniform bool hasAlphaMap;

// Lighting
uniform vec3 sunDirection;
uniform vec3 sunColor;
uniform vec3 viewPos;

void main()
{
    // Base normal
    vec3 norm = normalize(Normal);

    // If a normal map exists, modify the normal (basic tangent-space not implemented here yet)
    if (hasNormalMap) {
        vec3 normalTex = texture(normalMap, TexCoords).rgb;
        normalTex = normalTex * 2.0 - 1.0; // convert from [0,1] to [-1,1]
        norm = normalize(normalTex);       // simple replacement; tangent-space needed for proper normals
    }

    // Light calculations
    vec3 lightDir = normalize(-sunDirection);
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * sunColor;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * sunColor;

    float specularStrength = 0.3;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * spec * sunColor;

    vec3 lighting = ambient + diffuse + specular;

    // Start with base color
    vec3 result = Albedo;

    // Apply diffuse texture
    if (hasDiffuseMap) {
        result *= texture(diffuseMap, TexCoords).rgb;
    }

    // Apply specular intensity modulation
    if (hasSpecularMap) {
        float specIntensity = texture(specularMap, TexCoords).r; // usually stored in red channel
        specular *= specIntensity;
    }

    // Apply emissive texture
    if (hasEmissiveMap) {
        vec3 emissive = texture(emissiveMap, TexCoords).rgb;
        result += emissive; // add light directly
    }

    // Apply alpha mask
    float alpha = 1.0;
    if (hasAlphaMap) {
        alpha = texture(alphaMap, TexCoords).r;
    }

    FragColor = vec4(result * lighting + specular, alpha);
}
