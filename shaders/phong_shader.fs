#version 440 core

// Define the structure for light data
struct GLLight {
    vec3 color;
    float padding_01;
    vec3 position;
    float padding_02;
    vec3 direction;
    float padding_03;
    int type;
    float ambientIntensity;
    float specularIntensity;
    float strength;
};

// Declare a buffer to hold light data
layout(std430, binding = 0) buffer DataBuffer{
    GLLight lightData[99]; // Array of light structures
};

// Light types
const int DIRECTIONAL_LIGHT = 0;
const int POINT_LIGHT = 1;

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
uniform int num_lights;

// Function to calculate attenuation for light
float calcAttenuation(vec3 position, int type, float intensity){
    float attenuationCoefficient = 1.0;
    if(type != POINT_LIGHT){
        return 1.0;
    }
    float distance = length(position - vertexFragmentPos);
    //const float constant = 1.0;
    //const float linear = 0.045;
    //const float quadratic = 0.0075;
    //return (1.0 / (constant + linear * distance + quadratic * (distance * distance)));
    return (intensity * exp(-attenuationCoefficient * distance));
}

// Function to calculate ambient light
vec3 calcAmbient(in float intensity, in vec3 color){
    return (color * intensity);
}

// Function to calculate diffuse light
vec3 calcDiffuse(in vec3 color, in vec3 position, in int type, in vec3 direction){
    vec3 norm = normalize(vertexNormal);
    vec3 lightDir;
    if(type == DIRECTIONAL_LIGHT){
        lightDir = normalize(-direction);
    }
    else{
        lightDir = normalize(position - vertexFragmentPos);
    } 
    float diff = max(dot(norm, lightDir), 0.0);
    return (diff * color);
}

// Function to calculate specular light
vec3 calcSpecular(in float intensity, in vec3 color, in vec3 position, in int type, in vec3 direction){
    vec3 norm = normalize(vertexNormal);
    vec3 viewDir = normalize(cameraPosition - vertexFragmentPos);
    vec3 lightDir;
    if(type == DIRECTIONAL_LIGHT){
        lightDir = normalize(-direction);
    }
    else{
        lightDir = normalize(position - vertexFragmentPos);
    }
    vec3 reflection = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflection), 0.0), 32);
    return (intensity * spec * color);
}

void main(){
    vec4 textureColor = texture(texSample, TexCoord);
    vec3 phong = textureColor.xyz;
    vec3 ambient = vec3(0.0, 0.0, 0.0);
    //vec3 ambient = vec3(1.0, 1.0, 1.0);
    vec3 diffuse = vec3(0.0, 0.0, 0.0);
    vec3 specular = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < num_lights; ++i){
        // Calculate ambient, diffuse, and specular components for each light
        if(lightData[i].type != DIRECTIONAL_LIGHT){
            ambient += calcAmbient(lightData[i].ambientIntensity, lightData[i].color) * calcAttenuation(lightData[i].position, lightData[i].type, lightData[i].strength);
        }
        diffuse += calcDiffuse(lightData[i].color, lightData[i].position, lightData[i].type, lightData[i].direction) * calcAttenuation(lightData[i].position, lightData[i].type, lightData[i].strength);
        specular += calcSpecular(lightData[i].specularIntensity, lightData[i].color, lightData[i].position, lightData[i].type, lightData[i].position) * calcAttenuation(lightData[i].position, lightData[i].type, lightData[i].strength);
    }
    //Combine all lighting components and assign to the output color
    fragmentColor = vec4(phong * (ambient + diffuse + specular), 1.0);
    //fragmentColor = vec4(phong, 1.0);
    
}