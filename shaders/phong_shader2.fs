#version 440 core

// Define the structure for light data
struct GLLight {
    vec3 color;
    float padding_1;
    vec3 position;
    float lightLinear;
    vec3 direction;
    float lightConstant;
    int type;
    float ambientIntensity;
    float specularIntensity;
    float strength;
    float innerCutoff;
    float outerCutoff;
    int debug;
    float padding_2;
};

// Declare a buffer to hold light data
layout(std430, binding = 0) buffer DataBuffer{
    GLLight lightData[99]; // Array of light structures
};

const float PI = 3.14159265358979323846;

// Light types
const int DIRECTIONAL_LIGHT = 0;
const int POINT_LIGHT = 1;
const int SPOT_LIGHT = 2;

// Inputs from the vertex and fragment shaders
in vec3 vertexNormal;
in vec3 vertexFragmentPos;
in vec2 TexCoord;

// Output color of the fragment shader
out vec4 fragmentColor;

// Uniform variables
uniform vec3 v3color;
uniform float strength;
uniform sampler2D texSample;
uniform vec3 cameraPosition;
uniform int materialShininess;
uniform vec3 materialEmission;
uniform float materialScatterG;
uniform float materialAlpha;
uniform int num_lights;

// This is a calculation for light scattering using the Henyey-Greenstein phase function.
float calculateScatter(float g, float cosTheta){
    float g_squared = g * g;
    float denom = 1.0 + g_squared - 2.0 * g * cosTheta;
    return 1.0 / (4.0 * PI * denom * sqrt(denom));
}

// Function to calculate attenuation for light
float calculateAttenuation(GLLight light){
    float quadratic = 0.04;
    float distance = length(light.position - vertexFragmentPos);
    float attenuation = 1.0 / (light.lightConstant + light.lightLinear * distance + quadratic * distance * distance);
    attenuation *= light.strength;
    return max(attenuation, 0.0);
}

// Calculate lighting for a point light
vec3 calculatePointLighting(GLLight light){
    // Calculate point light ambient lighting
    vec3 ambient = light.color * light.ambientIntensity;

    // Calculate diffuse light component
    vec3 norm = normalize(vertexNormal);
    vec3 lightDirection = normalize(light.position - vertexFragmentPos);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * light.color;

    // Calculate simple subsurface light scattering
    vec3 viewDir = normalize(cameraPosition - vertexFragmentPos);
    float cosTheta = dot(viewDir, norm);
    float scatter = calculateScatter(materialScatterG, cosTheta);

    // Apply subsurface scatter to diffuse component
    diffuse *= scatter;

    // Calculate specular light component
    vec3 reflection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflection), 0.0), materialShininess);
    vec3 specular = light.specularIntensity * spec * light.color;

    // Combine all lighting components
    vec3 phong = ambient + diffuse + specular;

    // Apply bloom effect
    float bloomIntensity = 0.25;
    vec3 bloom = materialEmission * bloomIntensity;
    phong += bloom;

    // Apply material alpha
    phong *= (1.0 - materialAlpha);

    return phong;
}

// Calculate lighting for a spotlight
vec3 calculateSpotLighting(GLLight light){
    // Calculate coefficient of spotlight effect from the light's direction
    vec3 lightDirection = normalize(light.position - vertexFragmentPos);
    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = (light.innerCutoff - light.outerCutoff);
    float spot = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    // Calculate diffuse light component
    vec3 norm = normalize(vertexNormal);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * light.color;

    // Calculate specular light component
    vec3 viewDir = normalize(cameraPosition - vertexFragmentPos);
    vec3 reflection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflection), 0.0), materialShininess);
    vec3 specular = light.specularIntensity * spec * light.color;

    // Combine and attenuate lighting components
    return ((diffuse + spec) * spot * calculateAttenuation(light));
}

// Calculate lighting for a directional light
vec3 calculateDirectionalLighting(GLLight light){
    vec3 norm = normalize(vertexNormal);
    vec3 lightDir = normalize(-light.direction); // Invert for directional light
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color;

    vec3 viewDir = normalize(cameraPosition - vertexFragmentPos); // Assumed eye at (0, 0, 0)
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    vec3 specular = light.specularIntensity * spec * light.color;

    return (diffuse + specular) * light.strength;
}

void main(){
    if (lightData[0].debug == 1){
        // Debug mode: Only show spotlight lighting for light index 0
        fragmentColor = vec4(calculateSpotLighting(lightData[0]), 1.0);
    }
    else{
        // Calculate lighting for all lights and accumulate results
        vec4 textureColor = texture(texSample, TexCoord);
        vec3 phong = vec3(0.0, 0.0, 0.0);
        for (int i = 0; i < num_lights; ++i){
            if (lightData[i].type == POINT_LIGHT){
                phong += calculatePointLighting(lightData[i]);
            }
            else if (lightData[i].type == SPOT_LIGHT){
                phong += calculateSpotLighting(lightData[i]);
            }
            else if(lightData[i].type == DIRECTIONAL_LIGHT){
                phong += calculateDirectionalLighting(lightData[i]);
            }
        }

        // Apply final shading and texture
        fragmentColor = vec4(phong * textureColor.xyz, 0.1);
    }
}
