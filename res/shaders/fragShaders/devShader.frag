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

// Material properties
uniform float Shininess;
uniform float IOR;
uniform float Opacity;
uniform float Metallic;

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

// Texture tiling and offset for each map
uniform vec2 diffuseMapTiling;
uniform vec2 diffuseMapOffset;
uniform vec2 specularMapTiling;
uniform vec2 specularMapOffset;
uniform vec2 normalMapTiling;
uniform vec2 normalMapOffset;
uniform vec2 emissiveMapTiling;
uniform vec2 emissiveMapOffset;
uniform vec2 alphaMapTiling;
uniform vec2 alphaMapOffset;
uniform vec2 metallicMapTiling;
uniform vec2 metallicMapOffset;

// Lighting
uniform vec3 sunDirection;
uniform vec3 sunColor;
uniform vec3 viewPos;

vec2 TransformUV(vec2 uv, vec2 tiling, vec2 offset)
{
    return uv * tiling + offset;
}

// Builds a normal from the normal map.
// This method needs no tangent vertex data. It uses screen-space derivatives.
vec3 GetMappedNormal(vec3 baseNormal)
{
    vec2 uv = TransformUV(TexCoords, normalMapTiling, normalMapOffset);

    vec3 tangentNormal = texture(normalMap, uv).rgb * 2.0 - 1.0;

    vec3 posDX = dFdx(FragPos);
    vec3 posDY = dFdy(FragPos);
    vec2 uvDX = dFdx(uv);
    vec2 uvDY = dFdy(uv);

    vec3 N = normalize(baseNormal);
    vec3 T = normalize(posDX * uvDY.t - posDY * uvDX.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{
    vec3 norm = normalize(Normal);

    if (hasNormalMap) {
        norm = GetMappedNormal(norm);
    }

    vec3 lightDir = normalize(-sunDirection);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Base color
    vec3 baseColor = Albedo;
    if (hasDiffuseMap) {
        baseColor *= texture(diffuseMap,
            TransformUV(TexCoords, diffuseMapTiling, diffuseMapOffset)).rgb;
    }

    // Metallic value
    float metallic = Metallic;
    if (hasMetallicMap) {
        metallic *= texture(metallicMap,
            TransformUV(TexCoords, metallicMapTiling, metallicMapOffset)).r;
    }
    metallic = clamp(metallic, 0.0, 1.0);

    float IOR_safe = max(IOR, 1.0);

    float F0_dielectric = pow((IOR_safe - 1.0) / (IOR_safe + 1.0), 2.0);

    vec3 F0 = mix(vec3(F0_dielectric), baseColor, metallic);

    vec3 diffuseContribution = mix(baseColor, vec3(0.0), metallic);

    // Metal surfaces use F0 for ambient tint.
    vec3 ambientTint = mix(diffuseContribution, F0, metallic);
    vec3 ambient = AmbientColor * 0.4 * ambientTint;

    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = DiffuseColor * diff * sunColor * diffuseContribution;

    // Map Shininess (0 to 1) to a real exponent for pow().
    float specExponent = mix(2.0, 256.0, Shininess);

    // Specular with Fresnel
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specExponent);

    float cosTheta = clamp(dot(viewDir, norm), 0.0, 1.0);
    vec3 fresnel = F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);

    vec3 specular = SpecularColor * spec * sunColor * fresnel;

    if (hasSpecularMap) {
        float specIntensity = texture(specularMap,
            TransformUV(TexCoords, specularMapTiling, specularMapOffset)).r;
        specular *= specIntensity;
    }

    // Emissive
    vec3 emissive = EmissiveColor;
    if (hasEmissiveMap) {
        emissive *= texture(emissiveMap,
            TransformUV(TexCoords, emissiveMapTiling, emissiveMapOffset)).rgb;
    }

    // Alpha
    float alpha = clamp(Opacity, 0.0, 1.0);
    if (hasAlphaMap) {
        alpha *= texture(alphaMap,
            TransformUV(TexCoords, alphaMapTiling, alphaMapOffset)).r;
    }
    alpha = clamp(alpha, 0.0, 1.0);

    vec3 finalColor = ambient + diffuse + specular + emissive;
    FragColor = vec4(finalColor, alpha);
}