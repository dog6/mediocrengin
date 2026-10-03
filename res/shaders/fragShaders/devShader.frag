#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// Material colors
uniform vec3 Albedo;          // Reflectivity
uniform vec3 AmbientColor;    // Indirect light color
uniform vec3 DiffuseColor;    // Base color
uniform vec3 SpecularColor;   // Glare color
uniform vec3 EmissiveColor;   // Glow color

// Material properties
uniform float Shininess;      // 0 = matte, 1 = reflective
uniform float IOR;            // amount light refracts (bends)
uniform float Opacity;        // Alpha control (0.0-1.0)
uniform float Metallic;       // How metallic the surface is

// Material texture maps
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

uniform sampler2D metallicMap;
uniform bool hasMetallicMap;

// Lighting
uniform vec3 sunDirection;
uniform vec3 sunColor;
uniform vec3 viewPos;

void main()
{
    // Compute normal
    vec3 norm = normalize(Normal);

    if (hasNormalMap) {
        vec3 normalTex = texture(normalMap, TexCoords).rgb;
        normalTex = normalTex * 2.0 - 1.0; // [0,1] -> [-1,1]
        norm = normalize(normalTex);       // tangent-space not implemented
    }

    // Compute lighting
    vec3 lightDir = normalize(-sunDirection);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Get base color
    vec3 baseColor = Albedo;
    if (hasDiffuseMap) {
        baseColor *= texture(diffuseMap, TexCoords).rgb;
    }

    // Get metallic value
    float metallic = Metallic;
    if (hasMetallicMap) {
        metallic *= texture(metallicMap, TexCoords).r;
    }
    metallic = clamp(metallic, 0.0, 1.0);

    // Clamp IOR
    float IOR_safe = max(IOR, 1.0);

    // Calculate F0 from IOR for dielectrics
    float F0_dielectric = pow((IOR_safe - 1.0) / (IOR_safe + 1.0), 2.0);
    
    // Metallic workflow: metals have colored specular, dielectrics use IOR-based F0
    vec3 F0 = mix(vec3(F0_dielectric), baseColor, metallic);
    
    // For metals, diffuse is absorbed (black), for dielectrics it's the base color
    vec3 diffuseContribution = mix(baseColor, vec3(0.0), metallic);

    // Ambient
    vec3 ambient = AmbientColor * 0.4 * diffuseContribution;

    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = DiffuseColor * diff * sunColor * diffuseContribution;

    // Specular with Fresnel
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
    
    // Fresnel-Schlick approximation
    float cosTheta = clamp(dot(viewDir, norm), 0.0, 1.0);
    vec3 fresnel = F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
    
    vec3 specular = SpecularColor * spec * sunColor * fresnel;

    // Apply specular map
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
    float Opacity_safe = clamp(Opacity, 0.0, 1.0);
    float alpha = Opacity_safe;
    if (hasAlphaMap) {
        alpha *= texture(alphaMap, TexCoords).r;
    }
    alpha = clamp(alpha, 0.0, 1.0);

    // Final color
    vec3 finalColor = ambient + diffuse + specular + emissive;
    FragColor = vec4(finalColor, alpha);
}