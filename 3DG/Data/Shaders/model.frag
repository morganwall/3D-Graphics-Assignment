#version 460 core

struct Material
{
    sampler2D albedo;
    sampler2D normal;
    sampler2D roughness;
    sampler2D metallic;
    sampler2D ao;
    sampler2D emissive;
    sampler2D height;
    sampler2D opacity;

    bool hasAmbedo;
    bool hasNormal;
    bool hasRoughness;
    bool hasMetallic;
    bool hasAO;
    bool hasEmissive;
    bool hasHeight;
    bool hasOpacity;

    vec3 baseAlbedo;
    float baseRoughness;
    float baseMetallic;
    float baseEmissive;
    float baseOpacity;
};

struct Light
{
    bool enabled;
    vec3 position;
    float intensity;
    int type;
    vec3 colour;
    float innerCone;
    float outerCone;
    vec3 direction;
    float attenuation;
    float maxRange;
};

uniform Light light; // Current light.
uniform float ambient; // Ambient light intensity.
uniform Material material; // Material object.
uniform vec3 viewPosition; // Camera's position.

in vec3 vertPos; // World space position.
in vec2 vertUV; // Texture coordinates.
in vec3 vertNormals; // Normals vector.
in mat3 vertTBN;

out vec4 fragColor; // Output colour.

void main()
{
    // Check if the Light is Enabled.
    if (!light.enabled)
    {
        // Output Unlit.
        if (hasTexture) // Check if we have a Texture.
            fragColor = texture(dTexture, vertUV);
        else
            fragColor = vec4(baseColour, 1.0f);

        return;
    }

    // Get Object Colour.
    vec3 albedo;
    if (hasTexture) // Check if the Object has a Texture.
        albedo = texture(dTexture, vertUV).rgb;
    else
        albedo = baseColour;

    // Calculate Attenuation, Light Direction, and Ambient Light.
    float attenuation = 0.0f;
    vec3 curLightDirection;
    if (light.type == 1 || light.type == 2) // Check if the light is a point or spot light.
    {
        // Check Range.
        float distance = length(light.position - vertPos);
        if (distance > light.maxRange) // Check if what we're trying to light is outside of the maximum light range.
        {
            fragColor = vec4(albedo * ambient, 1.0f);
            return;
        }

        attenuation = 1.0f / (1.0f + light.attenuation * length(light.position - vertPos));
        curLightDirection = normalize(light.position - vertPos);
    }
    else // Directional Light.
    {
        attenuation = 1.0f;
        curLightDirection = normalize(-light.direction);
    }

    // Calculate Spot Intensity.
    float spotIntensity = 1.0f;
    if (light.type == 2) // Check if the light is a spot light.
    {
        vec3 spotDirection = normalize(light.direction); // Our light's direction.
        vec3 viewDirection = normalize(vertPos - light.position); // From light to fragment
        float alignment = dot(viewDirection, spotDirection); // How aligned they are.
        
        float innerConeCos = cos(radians(light.innerCone));
        float outerConeCos = cos(radians(light.outerCone));
        spotIntensity = clamp((alignment - outerConeCos) / (innerConeCos - outerConeCos), 0.0f, 1.0f);
    }

    vec3 vertNormalsNormalised = normalize(vertNormals);

    // Calculate Diffuse.
    float diffuse = max(dot(vertNormalsNormalised, curLightDirection), 0.0f);

    // Calculate Final Colour.
    vec3 finalColour = albedo * (ambient + attenuation * spotIntensity * diffuse * light.intensity) * light.colour;

    fragColor = vec4(finalColour, 1.0f);
}