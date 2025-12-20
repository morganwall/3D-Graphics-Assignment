#version 460 core

// ><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><
//      ><>< Constants ><><

#define MAX_LIGHTS 32

// Light Types.
#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

//      ><>< Constants ><><
// ><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><

// --------------------------------------------------------------------------------------

// ><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><
//      ><>< Structs ><><

struct Light
{
    bool enabled; // Is the light enabled.
    vec3 position; // The light's world space position.
    float intensity; // The light's intensity.
    int type; // The light type (0 = Directional, 1 = Point, 2 = Spot).
    vec3 colour; // The light's colour.
    float innerCone; // Inner cone angle for spot lights.
    float outerCone; // Outer cone angle for spot lights.
    vec3 direction; // The light's direction.
    float attenuation; // The light's attenuation factor.
    float maxRange; // The maximum range of the light.
};

//      ><>< Structs ><><
// ><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><

// --------------------------------------------------------------------------------------

// ><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><
//      ><>< Uniforms ><><

uniform Light light;
uniform Light lights[MAX_LIGHTS]; // All Lights.
uniform int numLights; // Number of lights.
uniform float ambient; // Ambient Light Intensity.
uniform vec3 baseColour; // Base colour.
uniform bool hasTexture; // Does the object have a texture.
uniform sampler2D dTexture; // The texture sampler.

//      ><>< Uniforms ><><
// ><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><

// --------------------------------------------------------------------------------------

// ><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><
//      ><>< Inputs ><><

in vec3 vertPos; // World space position.
in vec2 vertUV; // Texture coordinates.
in vec3 vertNormals; // Normals vector.

//      ><>< Inputs ><><
// ><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><

// --------------------------------------------------------------------------------------

// ><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><
//      ><>< Outputs ><><

out vec4 fragColor; // Output colour.

//      ><>< Outputs ><><
// ><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><

vec3 GetSpecular(vec3 lightDirection, vec3 normal, vec3 viewDirection, float intensity, vec3 lightColour)
{
    vec3 reflectDir = reflect(-lightDirection, normal); // Reflect light direction around normal.
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0f), 32); // Shininess hardcoded.
    return lightColour * (intensity * spec);
}

vec3 GetDirectionalLighting(Light light, vec3 albedo, vec3 normal)
{
    vec3 lightDir = normalize(-light.direction); // Light's direction.
    float diffuse = max(dot(lightDir, normal), 0.0f); // Diffuse factor.
    vec3 specular = vec3(0.0f);
    if (diffuse > 0.0f)
        specular = GetSpecular(lightDir, normal, normalize(-vertPos), light.intensity, light.colour);
    return (albedo * light.colour * (light.intensity * diffuse)) + specular;
}

vec3 GetPointLighting(Light light, vec3 albedo, vec3 normal)
{
    vec3 lightDir = normalize(light.position - vertPos); // Light's direction.
    float diffuse = max(dot(lightDir, normal), 0.0f); // Diffuse factor.

    float attenuation = 0.0f; // Attenuation factor.
    float distance = length(light.position - vertPos); // Distance from light to fragment.
    if (distance > light.maxRange) // Check if what we're trying to light is outside of the maximum light range.
        attenuation = 0.0f;

    attenuation = 1.0f / (1.0f + light.attenuation * distance); // Calculate attenuation.

    return albedo * light.colour * (attenuation * light.intensity * diffuse);
}

vec3 GetSpotLighting(Light light, vec3 albedo, vec3 normal)
{
    vec3 lightDir = normalize(light.position - vertPos); // Light's direction.
    float diffuse = max(dot(lightDir, normal), 0.0f); // Diffuse factor.

    float attenuation = 0.0f; // Attenuation factor.
    float distance = length(light.position - vertPos); // Distance from light to fragment.
    if (distance > light.maxRange) // Check if what we're trying to light is outside of the maximum light range.
		attenuation = 0.0f;

    attenuation = 1.0f / (1.0f + light.attenuation * distance); // Calculate attenuation.

    float spotIntensity = 0.0f;
    vec3 spotDirection = normalize(light.direction); // Our light's direction.
    vec3 viewDirection = normalize(vertPos - light.position); // From light to fragment
    float alignment = dot(viewDirection, spotDirection); // How aligned they are.
        
    float innerConeCos = cos(radians(light.innerCone));
    float outerConeCos = cos(radians(light.outerCone));
    spotIntensity = clamp((alignment - outerConeCos) / (innerConeCos - outerConeCos), 0.0f, 1.0f);

    return albedo * light.colour * (attenuation * spotIntensity * light.intensity * diffuse);
}

vec3 GetLighting(Light light, vec3 albedo, vec3 normal)
{
    // Check if the Light is Enabled.
    if (!light.enabled)
		return vec3(0.0f, 0.0f, 0.0f);

    // Loop through Light Types.
	switch (light.type)
    {
        case LIGHT_TYPE_DIRECTIONAL:
            return GetDirectionalLighting(light, albedo, normal);
        case LIGHT_TYPE_POINT:
            return GetPointLighting(light, albedo, normal);
        case LIGHT_TYPE_SPOT:
            return GetSpotLighting(light, albedo, normal);
        default:
			return vec3(0.0f, 0.0f, 0.0f);
    }
}

void main()
{
    vec3 albedo = vec3(0.0f);
    if (hasTexture) // Check if the Object has a Texture.
        albedo = texture(dTexture, vertUV).rgb;
    else
        albedo = baseColour;

    vec3 normal = normalize(vertNormals);

    vec3 finalLighting = albedo * ambient; // Start with ambient light.

    //for (int i = 0; i < MAX_LIGHTS && i < numLights; i++)
    //{
        //finalLighting += GetLighting(lights[i], albedo, normal);
	//}

    finalLighting += GetLighting(light, albedo, normal);

    fragColor = vec4(finalLighting, 1.0f);
}