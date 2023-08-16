#include <iostream>         // cout, cerr
#include <cstdlib>          // EXIT_FAILURE

#include <vector>
#include "UGLProp.hpp"
#include "MeshGenerator.hpp"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <map>


#pragma once
#define GLSL_vec3 alignas(16) glm::vec3
using namespace std; // Uses the standard namespace

#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

// Unnamed namespace
namespace
{
    const char* const WINDOW_TITLE = "Project 1"; // Macro for window title

    // Variables for window width and height
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

    GLFWwindow* gWindow = nullptr;
    
    // Camera
    Camera gCamera(glm::vec3(0.0f, 5.0f, 20.0f));
    float gLastX = WINDOW_WIDTH / 2.0f;
    float gLastY = WINDOW_HEIGHT / 2.0f;
    bool gFirstMouse = true;
    float cameraSpeed = 1.0f;

    // Timing variables
    float gDeltaTime = 0.0f;
    float gLastFrame = 0.0f;

    // Projections
    glm::mat4 perspective = glm::perspective(glm::radians(gCamera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 ortho = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
    glm::mat4 currentProjection;
    double lastProjectionChange = glfwGetTime();

    // namespace-scoped variables

    // Scene Lights
    vector<RichWerks::Light> lightingVector;
    
    // Scene Props
    vector<RichWerks::UGLProp> propVector;

    map<const char*, unsigned int> textureCache;
}


/* User-defined Function prototypes to:
 * initialize the program, set the window size,
 * redraw graphics on the window when resized,
 * and render graphics on the screen
 */
bool UInitialize(int, char*[], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
bool UCreateShaderProgram(Shader& t_shader, const char* t_vsPath, const char* t_fsPath);
void UDestroyShaderProgram(GLuint programId);
void URender();
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void USetLighting();
unsigned int ULoadTexture(const char* texFile);

void UGLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);


// main function. Entry point to the OpenGL program
int main(int argc, char* argv[])
{
    if (!UInitialize(argc, argv, &gWindow))
        return EXIT_FAILURE;
    // Enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(UGLErrorCallback, 0);
    //if (!UCreateShaderProgram(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE, gProgramId))
    //    return EXIT_FAILURE;

    Shader phongShader;

    if (!UCreateShaderProgram(phongShader, "shaders/phong_shader.vs", "shaders/phong_shader2.fs")) {
        return EXIT_FAILURE;
    }
    
    USetLighting();
    // Create meshes for the objects that make up our candle holder and candle.
    
    RichWerks::UGLProp woodBase;
    woodBase.AttachShader(phongShader);
    RichWerks::Material woodBaseMaterial;
    woodBaseMaterial.texture = ULoadTexture("textures/wood1.jpg");
    woodBaseMaterial.shininess = 1;
    woodBase.SetMaterial(woodBaseMaterial);
    woodBase.AddMesh(generateCube(10.0f, 10.0f, 1.0f));
    woodBase.BindMesh();
    woodBase.Rotate(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    propVector.push_back(woodBase);

    RichWerks::UGLProp glassCandle;
    glassCandle.AttachShader(phongShader);
    RichWerks::Material glassCandleMaterial;
    glassCandleMaterial.texture = ULoadTexture("textures/ceramic.jpg"); // <a href="https://www.freepik.com/free-photo/close-up-white-marble-textured-background_3472368.htm#query=white%20ceramic%20texture&position=28&from_view=keyword&track=ais">Image by rawpixel.com</a> on Freepik
    glassCandleMaterial.shininess = 64;
    glassCandle.SetMaterial(glassCandleMaterial);
    glassCandle.AddMesh(generateCylinder(2.5f, 5.0f, 30));
    glassCandle.BindMesh();
    glassCandle.Translate(woodBase.GetPosition() + glm::vec3(0.0f, 0.5f, 0.0f));
    propVector.push_back(glassCandle);
    
    RichWerks::UGLProp candleStick;
    candleStick.AttachShader(phongShader);
    RichWerks::Material candleStickMaterial;
    candleStickMaterial.texture = ULoadTexture("textures/distressed_wood.jpg");
    candleStickMaterial.shininess = 1;
    candleStick.SetMaterial(candleStickMaterial);
    // Base of candle stick
    candleStick.AddMesh(generateCube(4.0f, 4.0f, 1.0f));
    // Shaft of Candlestick
    RichWerks::Mesh candleStickShaft = generateCube(2.0f, 2.0f, 5.0f);
    translateMesh(candleStickShaft, glm::vec3(0.0f, 1.0f, 0.0f) * 1.0f);
    candleStick.AddMesh(candleStickShaft);
    // Candlestick Platform
    RichWerks::Mesh candleStickShaft2 = generateCube(5.0f, 5.0f, 1.0f);
    translateMesh(candleStickShaft2, glm::vec3(0.0f, 1.0f, 0.0f) * 6.0f);
    candleStick.AddMesh(candleStickShaft2);
    candleStick.BindMesh();
    candleStick.Rotate(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    candleStick.Translate(glm::vec3(-1.0f, 0.0f, 1.0f) * 5.5f);
    propVector.push_back(candleStick);

    // Candle
    RichWerks::UGLProp candle;
    RichWerks::Material candleMaterial;
    candleMaterial.texture = ULoadTexture("textures/candle2.jpg");
    candleMaterial.shininess = 8;
    candleMaterial.emission = glm::vec3(0.81f, 0.73f, 0.43f);
    candleMaterial.materialAlpha = 0.3f;
    candleMaterial.materialScatterG = -0.5;
    candle.SetMaterial(candleMaterial);
    candle.AttachShader(phongShader);
    // Base of candle stick
    candle.AddMesh(generateCylinder(2.0f, 5.0f, 30));
    candle.BindMesh();
    candle.Translate(glm::vec3(candleStick.GetPosition().x, 7.0f, candleStick.GetPosition().z));
    propVector.push_back(candle);

    RichWerks::UGLProp candleStick2;
    candleStick2.AttachShader(phongShader);
    RichWerks::Material candleStick2Material;
    candleStick2Material.texture = ULoadTexture("textures/distressed_wood.jpg");
    candleStick2Material.shininess = 1;
    candleStick2.SetMaterial(candleStick2Material);
    // Base of candle stick
    candleStick2.AddMesh(generateCube(4.0f, 4.0f, 1.0f));
    // Shaft of Candlestick
    RichWerks::Mesh candleStick2Shaft = generateCube(2.0f, 2.0f, 5.0f);
    translateMesh(candleStick2Shaft, glm::vec3(0.0f, 1.0f, 0.0f) * 1.0f);
    candleStick2.AddMesh(candleStick2Shaft);
    // Candlestick Platform
    RichWerks::Mesh candleStick2Shaft2 = generateCube(5.0f, 5.0f, 1.0f);
    translateMesh(candleStick2Shaft2, glm::vec3(0.0f, 1.0f, 0.0f) * 6.0f);
    candleStick2.AddMesh(candleStick2Shaft2);
    candleStick2.BindMesh();
    candleStick2.Rotate(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    candleStick2.Translate(glm::vec3(1.0f, 0.0f, 1.0f) * 5.5f);
    propVector.push_back(candleStick2);

    // Candle2
    RichWerks::UGLProp candle2;
    RichWerks::Material candle2Material;
    candle2Material.texture = ULoadTexture("textures/candle2.jpg");
    candle2Material.shininess = 8;
    candle2Material.emission = glm::vec3(0.81f, 0.73f, 0.43f);
    candleMaterial.materialAlpha = 0.3f;
    candleMaterial.materialScatterG = -0.5;
    candle2.SetMaterial(candle2Material);
    candle2.AttachShader(phongShader);
    // Base of candle stick
    candle2.AddMesh(generateCylinder(2.0f, 5.0f, 30));
    candle2.BindMesh();
    candle2.Translate(glm::vec3(candleStick2.GetPosition().x, 7.0f, candleStick2.GetPosition().z));
    propVector.push_back(candle2);

    // Floor
    RichWerks::UGLProp floor;
    RichWerks::Material floorMaterial;
    floorMaterial.texture = ULoadTexture("textures/granite.jpg");
    floorMaterial.shininess = 16;
    floor.SetMaterial(floorMaterial);
    floor.AttachShader(phongShader);
    // Base of candle stick
    floor.AddMesh(generatePlane(40.0f, 40.0f));
    floor.BindMesh();
    propVector.push_back(floor);

    RichWerks::UGLProp lampPost;
    RichWerks::Material lampPostMaterial;
    lampPostMaterial.texture = ULoadTexture("textures/brass.jpg");
    lampPostMaterial.shininess = 32;
    lampPost.SetMaterial(lampPostMaterial);
    lampPost.AttachShader(phongShader);
    lampPost.AddMesh(generateCylinder(0.5f, 12.0f, 20));
    RichWerks::Mesh lampMesh2 = generateCylinder(0.5f, 3.5f, 20);
    rotateMesh(lampMesh2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    translateMesh(lampMesh2, glm::vec3(0.0f, 12.0f, 0.0f));
    lampPost.AddMesh(lampMesh2);
    RichWerks::Mesh lampMesh3 = generateCylinder(0.5f, 3.0f, 20);
    translateMesh(lampMesh3, glm::vec3(0.0f, 12.0f - 3.0f, 3.5f));
    lampPost.AddMesh(lampMesh3);
    lampPost.BindMesh();
    lampPost.Translate(glm::vec3(0.0f, 0.0f, -3.5f));
    propVector.push_back(lampPost);

    RichWerks::UGLProp lampHead;
    RichWerks::Material lampHeadMaterial;
    lampHeadMaterial.texture = ULoadTexture("textures/pink_glass.jpg");
    lampHeadMaterial.shininess = 64;
    candleMaterial.emission = glm::vec3(1.0f, 0.34f, 0.2f);
    candleMaterial.materialAlpha = 0.01f;
    candleMaterial.materialScatterG = -0.5;
    lampHead.SetMaterial(lampHeadMaterial);
    lampHead.AttachShader(phongShader);
    RichWerks::Mesh lampHeadMesh = generatePyramid(4.0f, 5.0f);
    translateMesh(lampHeadMesh, glm::vec3(0.0f, 12.0f - 3.0f - 2.0f, 0.0f));
    lampHead.AddMesh(lampHeadMesh);
    lampHead.BindMesh();
    propVector.push_back(lampHead);


    // Sets the background color of the window to black (it will be implicitely used by glClear)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(gWindow))
    {
        float currentFrame = glfwGetTime();
        gDeltaTime = currentFrame - gLastFrame;
        gLastFrame = currentFrame;
        // input
        // -----
        UProcessInput(gWindow);

        URender();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwPollEvents();
    }
    
    UDestroyShaderProgram(phongShader.ID);

    exit(EXIT_SUCCESS); // Terminates the program successfully
}


// Initialize GLFW, GLEW, and create a window
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
    gCamera.Position = glm::vec3(0.0f, 5.0f, 20.0f);
    currentProjection = perspective;
    // GLFW: initialize and configure (specify desired OpenGL version)
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW: window creation
    // ---------------------
    *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (*window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);
    glfwSetCursorPosCallback(*window, UMousePositionCallback);
    glfwSetScrollCallback(*window, UMouseScrollCallback);
    glfwSetMouseButtonCallback(*window, UMouseButtonCallback);

    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLEW: initialize
    // ----------------
	// Note: if using GLEW version 1.13 or earlier
	glewExperimental = GL_TRUE;
	GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
        std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
        return false;
    }

    // Displays GPU OpenGL version
    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

    return true;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void UProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        gCamera.ProcessKeyboard(LEFT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        gCamera.ProcessKeyboard(RIGHT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_SPACE))
        gCamera.Position += gCamera.Up * gCamera.MovementSpeed * gDeltaTime;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
        gCamera.Position -= gCamera.Up * gCamera.MovementSpeed * gDeltaTime;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && (glfwGetTime() - lastProjectionChange) > 0.5)
    {
        currentProjection = currentProjection == perspective ? ortho : perspective;
        lastProjectionChange = glfwGetTime();
    }
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId) {
    // Compilation and linkage error reporting
    int success = 0;
    char infoLog[512];

    // Create a Shader program object.
    programId = glCreateProgram();

    // Create the vertex and fragment shader objects
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    // Retrive the shader source
    glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
    glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

    // Compile the vertex shader, and print compilation errors (if any)
    glCompileShader(vertexShaderId); // compile the vertex shader
    // check for shader compile errors
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glCompileShader(fragmentShaderId); // compile the fragment shader
    // check for shader compile errors
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    // Attached compiled shaders to the shader program
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);   // links the shader program
    // check for linking errors
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glUseProgram(programId);    // Uses the shader program

    return true;
}

bool UCreateShaderProgram(Shader& t_shader,const char* t_vsPath, const char* t_fsPath) {
    t_shader = Shader(t_vsPath, t_fsPath);

    return t_shader.success;

}

void UDestroyShaderProgram(GLuint programId) {
    glDeleteProgram(programId);
}

void URender() {
    
    glEnable(GL_DEPTH_TEST);

    // Clear the background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (RichWerks::UGLProp& prop : propVector) {
        prop.Render(gCamera, currentProjection, lightingVector.size());
    }
        
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(gWindow);    // Flips the the back buffer with the front buffer every frame.
}

// Bind camera movement to mouse cursor.
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos) {
    if (gFirstMouse) {
        gLastX = xpos;
        gLastY = ypos;
        gFirstMouse = false;
    }

    float xoffset = xpos - gLastX;
    float yoffset = gLastY - ypos;

    gLastX = xpos;
    gLastY = ypos;
    //cout << "Mouse(x:y) = " << xpos << ":" << ypos << endl;
    gCamera.ProcessMouseMovement(xoffset, yoffset);
}

void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    gCamera.MovementSpeed = max(0.2f, min(20.0f, gCamera.MovementSpeed + (float)yoffset));
}

void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        if (action == GLFW_PRESS) {
            cout << "Left mouse button pressed" << endl;
        }
        break;
    }
}

unsigned int ULoadTexture(const char* texFile) {
    if (textureCache.find(texFile) != textureCache.end()) {
        return textureCache.find(texFile)->second;
    }
    
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(texFile, &width, &height, &nrChannels, 0);
    if (0 == nrChannels || 4 < nrChannels) { throw std::runtime_error("invalid depth"); }
    GLenum const format[] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, format[nrChannels - 1], width, height, 0, format[nrChannels - 1], GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free(data);
    textureCache.insert({ texFile, texture });
    return texture;
}

void USetLighting() {
    // create and add lighting for scene
    
    RichWerks::Light light;
    light.color = glm::vec3(0.97f, 0.98f, 0.75f);
    light.ambientIntensity = 0.2f;
    light.specularIntensity = 1.0f;
    light.type = RichWerks::LightingType::SPOT_LIGHT;
    light.position = glm::vec3(0.0f, 17.0f, 0.0f);
    light.direction = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - light.position);
    light.innerCutoff = glm::cos(glm::radians(15.0f));
    light.outerCutoff = glm::cos(glm::radians(20.0f));
    light.strength = 10.0f;
    light.lightConstant = 1.0f;
    light.lightLinear = 0.01f;
    light.debug = 0;

    lightingVector.push_back(light);
    
    light.color = glm::vec3(0.97f, 0.98f, 0.75f);
    light.ambientIntensity = 0.2f;
    light.specularIntensity = 1.0f;
    light.type = RichWerks::LightingType::POINT_LIGHT;
    light.position = glm::vec3(-1.0, 0.0f, 1.0) * 5.5f;
    light.position += glm::vec3(0.0f, 13.5f, 0.0f);
    light.direction = glm::vec3(0.0f, 0.0f, 0.0f);
    light.strength = 10.0f;
    light.lightLinear = 0.01;
    light.lightConstant = 1.0;
    light.debug = 0;

    lightingVector.push_back(light);
    
    light.color = glm::vec3(0.97f, 0.98f, 0.75f);
    light.ambientIntensity = 0.05f;
    light.specularIntensity = 0.5f;
    light.type = RichWerks::LightingType::POINT_LIGHT;
    light.position = glm::vec3(1.0, 0.0f, 1.0) * 5.5f;
    light.position += glm::vec3(0.0f, 13.5f, 0.0f);
    light.direction = glm::vec3(0.0f, 0.0f, 0.0f);
    light.strength = 5.0f;
    light.lightLinear = 0.01;
    light.lightConstant = 1.0;
    light.debug = 0;
    
    lightingVector.push_back(light);

    light.color = glm::vec3(0.97f, 0.98f, 0.75f);
    light.ambientIntensity = 0.1f;
    light.specularIntensity = 0.1f;
    light.position = glm::vec3(-10.0f, 13.0f, 0.0f);
    light.direction = light.position - glm::vec3(0.0f, 0.0f, 0.0f);
    light.type = RichWerks::LightingType::DIRECTIONAL_LIGHT;
    light.strength = 0.1f;

    lightingVector.push_back(light);

    GLuint buffer;
    GLuint sharedBindingPoint = 0;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, lightingVector.size() * sizeof(lightingVector[0]), lightingVector.data(), GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, sharedBindingPoint, buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

// Prints GLSL operation error to the screen.
void UGLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    cout << "GL ERROR: " << source << message << endl;
}