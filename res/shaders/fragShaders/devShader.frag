#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// Material colors
uniform vec3 Albedo;          // Base color
uniform vec3 AmbientColor;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 EmissiveColor;
uniform float Shininess;

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
    // ------------------------
    // 1. Compute normal
    // ------------------------
    vec3 norm = normalize(Normal);

    if (hasNormalMap) {
        vec3 normalTex = texture(normalMap, TexCoords).rgb;
        normalTex = normalTex * 2.0 - 1.0; // [0,1] -> [-1,1]
        norm = normalize(normalTex);       // note: tangent-space not implemented yet
    }

    // ------------------------
    // 2. Compute lighting
    // ------------------------
    vec3 lightDir = normalize(-sunDirection);

    // Ambient
    vec3 ambient = AmbientColor * 0.4; // ambient strength, can be uniform if needed
    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = DiffuseColor * diff * sunColor;
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
    vec3 specular = SpecularColor * 0.3 * spec * sunColor;

    vec3 lighting = ambient + diffuse;

    // ------------------------
    // 3. Apply textures
    // ------------------------
    vec3 result = Albedo;

    // Diffuse map
    if (hasDiffuseMap) {
        result *= texture(diffuseMap, TexCoords).rgb;
    }

    // Specular map (modulates specular intensity)
    if (hasSpecularMap) {
        float specIntensity = texture(specularMap, TexCoords).r;
        specular *= specIntensity;
    }

    // Emissive map
    vec3 emissive = EmissiveColor;
    if (hasEmissiveMap) {
        emissive += texture(emissiveMap, TexCoords).rgb;
    }

    // Alpha map
    float alpha = 1.0;
    if (hasAlphaMap) {
        alpha = texture(alphaMap, TexCoords).r;
    }

    // ------------------------
    // 4. Final color
    // ------------------------
    FragColor = vec4(result * lighting + specular + emissive, alpha);
}
