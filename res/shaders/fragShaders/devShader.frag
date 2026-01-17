#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// Material colors
uniform vec3 Albedo;          
uniform vec3 AmbientColor;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 EmissiveColor;
uniform float Shininess;
uniform float IOR;            // Index of Refraction
uniform float Opacity;        // Alpha control (0.0-1.0)

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
        norm = normalize(normalTex);       // tangent-space not implemented
    }

    // ------------------------
    // 2. Compute lighting
    // ------------------------
    vec3 lightDir = normalize(-sunDirection);

    // Ambient
    vec3 ambient = AmbientColor * 0.4; 
    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = DiffuseColor * diff * sunColor;
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
    vec3 specular = SpecularColor * 0.3 * spec * sunColor;

    // ------------------------
    // 3. Clamp IOR and Opacity
    // ------------------------
    float IOR_safe = max(IOR, 1.0);           // Prevent IOR < 1
    float Opacity_safe = clamp(Opacity, 0.0, 1.0); // Clamp alpha 0..1

    // ------------------------
    // 4. Apply Fresnel using clamped IOR
    // ------------------------
    float cosTheta = clamp(dot(viewDir, norm), 0.0, 1.0);
    float F0 = pow((IOR_safe - 1.0)/(IOR_safe + 1.0), 2.0); 
    float fresnel = F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
    specular *= fresnel; 

    // ------------------------
    // 5. Apply textures
    // ------------------------
    vec3 result = Albedo;

    // Diffuse map
    if (hasDiffuseMap) {
        result *= texture(diffuseMap, TexCoords).rgb;
    }

    // Specular map
    if (hasSpecularMap) {
        float specIntensity = texture(specularMap, TexCoords).r;
        specular *= specIntensity;
    }

    // Emissive map
    vec3 emissive = EmissiveColor;
    if (hasEmissiveMap) {
        emissive += texture(emissiveMap, TexCoords).rgb;
    }

    // Alpha
    float alpha = Opacity_safe; // start with clamped opacity
    if (hasAlphaMap) {
        alpha *= texture(alphaMap, TexCoords).r; // multiply by texture if present
    }
    alpha = clamp(alpha, 0.0, 1.0); // final clamp to ensure safety

    // ------------------------
    // 6. Final color
    // ------------------------
    FragColor = vec4(result * (ambient + diffuse) + specular + emissive, alpha);
}
