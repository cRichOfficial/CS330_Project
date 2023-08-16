#include "UGLProp.hpp"
using namespace RichWerks;

// Default constructor
UGLProp::UGLProp() {
    UGLObject(); // Call UGLObject constructor
    model = glm::mat4(1.0f);
    rotation = glm::rotate(glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
    translation = glm::translate(position);
    model = translation * rotation * scale;
}

// Constructor with position and direction parameters
UGLProp::UGLProp(glm::vec3 t_position, glm::vec3 t_direction) {
    UGLObject(t_position, t_direction); // Call UGLObject constructor with parameters
    model = glm::mat4(1.0f);
    rotation = glm::rotate(glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
    translation = glm::translate(position);
    model = translation * rotation * scale;
}

// Constructor with mesh and material parameters
UGLProp::UGLProp(Mesh t_mesh, Material t_material) {
    UGLObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Call UGLObject constructor with default position and direction
    meshVector.push_back(t_mesh);
    materialVector.push_back(t_material);
    model = glm::mat4(1.0f);
    rotation = glm::rotate(glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
    translation = glm::translate(position);
    model = translation * rotation * scale;
}

// Constructor with position, direction, mesh, and material parameters
UGLProp::UGLProp(glm::vec3 t_position, glm::vec3 t_direction, Mesh t_mesh, Material t_material) {
    UGLObject(t_position, t_direction); // Call UGLObject constructor with parameters
    materialVector.push_back(t_material);
    meshVector.push_back(t_mesh);
    model = glm::mat4(1.0f);
    rotation = glm::rotate(glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
    translation = glm::translate(position);
    model = translation * rotation * scale;
}

// Copy constructor
UGLProp::UGLProp(const UGLProp& prop) {
    Copy(prop); // Call Copy function to copy data
}

// Move constructor
UGLProp::UGLProp(UGLProp&& prop) noexcept
    : UGLObject(std::move(prop)), // Call base class move constructor
    materialVector(std::move(prop.materialVector)),
    meshVector(std::move(prop.meshVector)),
    shader(prop.shader),
    model(prop.model),
    scale(prop.scale),
    rotation(prop.rotation),
    translation(prop.translation) {
    // Set the moved-from object's shader to nullptr
    // prop.shader = nullptr;
}

// Destructor
UGLProp::~UGLProp() {
    DestroyMeshVector(); // Call function to release mesh resources
}

// Copy assignment operator
UGLProp& UGLProp::operator=(const UGLProp& prop) {
    if (this != &prop) {
        DestroyMeshVector(); // Release current resources
        Copy(prop); // Copy data from prop
    }
    return *this;
}

// Move assignment operator
UGLProp& UGLProp::operator=(UGLProp&& prop) noexcept {
    if (this != &prop) {
        // Call base class move assignment operator
        UGLObject::operator=(std::move(prop));

        // Release resources from the current object
        DestroyMeshVector();

        // Move resources from the other object
        materialVector = std::move(prop.materialVector);
        meshVector = std::move(prop.meshVector);
        shader = prop.shader;
        model = prop.model;
        scale = prop.scale;
        rotation = prop.rotation;
        translation = prop.translation;

        // Set the moved-from object's shader to nullptr
        prop.shader = nullptr;
    }
    return *this;
}

// Copy function to copy data from another object
void UGLProp::Copy(const UGLProp& prop) {
    materialVector = prop.materialVector;
    meshVector = prop.meshVector;
    shader = prop.shader;
    model = prop.model;
    scale = prop.scale;
    rotation = prop.rotation;
    translation = prop.translation;
}

// Attach a shader to the object
void UGLProp::AttachShader(Shader& t_shader) {
    shader = &t_shader;
}

// Get the shader ID
GLint UGLProp::GetShaderID() {
    return shader->ID;
}

// Use the attached shader
void UGLProp::UseShader() {
    shader->use();
}

// Set shader uniform using different data types
template <typename T>
void UGLProp::SetShaderUniform(T const& t_data, const char* t_name) {
    if constexpr (std::is_same_v<T, bool>) {
        shader->setBool(t_name, t_data);
    }
    else if constexpr (std::is_same_v<T, float> || std::is_same_v<T, GLfloat>) {
        shader->setFloat(t_name, t_data);
    }
    else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, GLint>) {
        shader->setInt(t_name, t_data);
    }
    else if constexpr (std::is_same_v<T, glm::vec3>) {
        shader->setVec3(t_name, t_data);
    }
    else if constexpr (std::is_same_v<T, glm::mat4>) {
        shader->setMatrix4fv(t_name, t_data);
    }
}

// Add a mesh to the mesh vector
void UGLProp::AddMesh(Mesh t_mesh) {
    meshVector.push_back(t_mesh);
}

// Get a reference to the mesh vector
std::vector<Mesh>& UGLProp::GetMeshVectorReference() {
    return meshVector;
}

// Set the material for the object
void UGLProp::SetMaterial(Material t_material) {
    materialVector.push_back(t_material);
}

// Translate the object
void UGLProp::Translate(glm::vec3 t_position) {
    translation = glm::translate(glm::mat4(1.0f), t_position);
    position = t_position;
    updateModel();
}

// Rotate the object
void UGLProp::Rotate(GLfloat t_radians, glm::vec3 t_axes) {
    rotation = glm::rotate(glm::mat4(1.0f), t_radians, t_axes);
    updateModel();
}

// Scale the object
void UGLProp::Scale(glm::vec3 t_scale) {
    scale = glm::scale(t_scale);
    updateModel();
}

// Bind the mesh data
void UGLProp::BindMesh() {
    const GLuint FLOATS_PER_VERTEX = 3;
    const GLuint FLOATS_PER_NORMAL = 3;
    const GLuint FLOATS_PER_TEX_COORD = 2;
    for (Mesh& mesh : meshVector) {
        glGenVertexArrays(1, &mesh.vao);
        glBindVertexArray(mesh.vao);

        glGenBuffers(2, mesh.vbos); // Creates 2 buffers
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
        glBufferData(GL_ARRAY_BUFFER, mesh.vertexData.size() * sizeof(mesh.vertexData[0]), &mesh.vertexData.front(), GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indexData.size() * sizeof(mesh.indexData[0]), &mesh.indexData.front(), GL_STATIC_DRAW);

        // Stride of each vertex coordinate in array
        GLint stride = sizeof(float) * (FLOATS_PER_VERTEX + FLOATS_PER_NORMAL + FLOATS_PER_TEX_COORD);

        // Send vertex coordinates to GPU
        glVertexAttribPointer(0, FLOATS_PER_VERTEX, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, FLOATS_PER_NORMAL, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(GLfloat) * FLOATS_PER_VERTEX));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, FLOATS_PER_TEX_COORD, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(GLfloat) * (FLOATS_PER_NORMAL + FLOATS_PER_VERTEX)));
        glEnableVertexAttribArray(2);
    }
}

// Destroy mesh data and release resources
void UGLProp::DestroyMeshVector() {
    for (Mesh mesh : meshVector) {
        glDeleteVertexArrays(1, &mesh.vao);
        glDeleteBuffers(2, mesh.vbos);
    }
}

// Render the object
void UGLProp::Render(Camera t_camera, glm::mat4 t_projection, int num_lights) {
    GLint activeShader;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&activeShader);
    if (activeShader != shader->ID) {
        shader->use();
    }
    model = translation * rotation * scale;
    glm::mat4 view = t_camera.GetViewMatrix();
    glm::mat4 projection = t_projection;
    SetShaderUniform(num_lights, "num_lights");
    SetShaderUniform(t_camera.Position, "cameraPosition");
    SetShaderUniform(model, "model");
    SetShaderUniform(view, "view");
    SetShaderUniform(projection, "projection");

    Material currentMaterial;
    int i = 0;
    for (const RichWerks::Mesh& mesh : meshVector) {
        glBindVertexArray(mesh.vao);
        if (i < materialVector.size()) {
            currentMaterial = materialVector[i];
            SetShaderUniform(currentMaterial.shininess, "materialShininess");
            SetShaderUniform(currentMaterial.emission, "materialEmission");
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, currentMaterial.texture);
            i++;
        }
        glDrawElements(GL_TRIANGLES, mesh.indexData.size() * sizeof(GLshort), GL_UNSIGNED_SHORT, NULL);

        glBindVertexArray(0);
    }
}

// Update the model matrix
void UGLProp::updateModel() {
    model = translation * rotation * scale;
}

// Get the position of the object
glm::vec3 UGLProp::GetPosition() {
    return position;
}

// Get the mesh at a specific index
Mesh UGLProp::GetMesh(int idx) {
    if (idx < meshVector.size()) {
        return meshVector[idx];
    }
    else {
        return meshVector.back();
    }
}

// Get the count of meshes
int UGLProp::GetMeshCount() {
    return meshVector.size();
}
