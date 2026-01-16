#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Tangent;
in vec3 Bitangent;

out vec4 FragColor;

// Material properties
uniform vec3 Albedo;
uniform vec3 AmbientColor;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 EmissiveColor;
uniform float Shininess;

// Texture maps
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D emissiveMap;
uniform sampler2D normalMap;
uniform sampler2D alphaMap;

// Booleans for map presence
uniform bool hasDiffuseMap;
uniform bool hasSpecularMap;
uniform bool hasEmissiveMap;
uniform bool hasNormalMap;
uniform bool hasAlphaMap;

// Lighting
uniform vec3 sunDirection;
uniform vec3 sunColor;
uniform vec3 viewPos;

void main()
{
    // Base colors
    // vec3 albedo = Albedo;
    // if (hasDiffuseMap)
    //     albedo *= texture(diffuseMap, TexCoord).rgb;

    vec3 albedo = Albedo;
    if (hasDiffuseMap)
        albedo = texture(diffuseMap, TexCoord).rgb;

    // vec3 specularCol = SpecularColor;
    // if (hasSpecularMap)
    //     specularCol *= texture(specularMap, TexCoord).rgb;

    // vec3 emissive = EmissiveColor;
    // if (hasEmissiveMap)
    //     emissive *= texture(emissiveMap, TexCoord).rgb;

    // float alpha = 1.0;
    // if (hasAlphaMap)
    //     alpha = texture(alphaMap, TexCoord).r;

    // // Normal mapping
    // vec3 norm = normalize(Normal);
    // if (hasNormalMap)
    // {
    //     vec3 tangentNormal = texture(normalMap, TexCoord).rgb;
    //     tangentNormal = tangentNormal * 2.0 - 1.0; // convert [0,1] to [-1,1]

    //     mat3 TBN = mat3(normalize(Tangent), normalize(Bitangent), normalize(Normal));
    //     norm = normalize(TBN * tangentNormal);
    // }

    // // Directional light calculations
    vec3 lightDir = normalize(-sunDirection);

    // // Ambient
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * AmbientColor * sunColor;

    // // Diffuse
    // float diff = max(dot(norm, lightDir), 0.0);
    // vec3 diffuse = diff * DiffuseColor * sunColor;

    // // Specular
    // vec3 viewDir = normalize(viewPos - FragPos);
    // vec3 reflectDir = reflect(-lightDir, norm);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
    // vec3 specular = spec * specularCol * sunColor;

    // Combine lighting + emissive
    vec3 lighting = ambient + diffuse + specular + emissive;

    // Apply to albedo
    vec3 result = lighting * albedo;

    FragColor = vec4(result, alpha);
}
