#include "UGLObject.hpp"
#include <learnOpengl/camera.h>

#ifndef _UGLProp_
#define _UGLProp_

#pragma once
namespace RichWerks {
    struct Mesh {
        std::vector<GLfloat> vertexData;
        std::vector<GLshort> indexData;
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 size;
        GLuint vao;
        GLuint vbos[2];
        GLfloat getHeight() {
            return size.y;
        }
        GLfloat getWidth() {
            return size.x;
        }
        GLfloat getLength() {
            return size.z;
        }
    };

    struct Material {
        int texture;
        int shininess = 0;
        glm::vec3 emission = glm::vec3(0.0f);
        GLfloat materialScatterG = 0.0f;
        GLfloat materialAlpha = 0.0f;
        glm::vec3 color;
    };

    class UGLProp :
        public UGLObject
    {
    public:
        // Constructors
        UGLProp();
        UGLProp(glm::vec3 t_position, glm::vec3 t_direction);
        UGLProp(Mesh t_mesh, Material t_material);
        UGLProp(glm::vec3 t_position, glm::vec3 t_direction, Mesh t_mesh, Material t_material);
        UGLProp(const UGLProp& prop);
        UGLProp(UGLProp&& prop) noexcept;
        ~UGLProp();

        // Assignment operators
        UGLProp& operator=(const UGLProp& prop);
        UGLProp& operator=(UGLProp&& prop) noexcept;

        // Mesh operations
        void AddMesh(Mesh t_mesh);
        void BindMesh();
        std::vector<Mesh>& GetMeshVectorReference();
        void SetMaterial(Material t_material);

        // Shader operations
        void AttachShader(Shader& t_shader);
        GLint GetShaderID();
        void UseShader();
        template <class T>
        void SetShaderUniform(T const& t_data, const char* name);

        // Transformation operations
        void Translate(glm::vec3 t_position);
        void Rotate(GLfloat t_radians, glm::vec3 t_axes);
        void Scale(glm::vec3 t_scale);

        // Information retrieval
        glm::vec3 GetPosition();
        Mesh GetMesh(int idx);
        int GetMeshCount();

        // Rendering
        void Render(Camera t_camera, glm::mat4 t_projection, int num_lights);

    protected:
        // Utility functions
        void DestroyMeshVector();
        void Copy(const UGLProp& prop);
        void updateModel();

        // Data members
        std::vector<Material> materialVector;
        std::vector<Mesh> meshVector;
        Shader* shader;
        glm::mat4 model;
        glm::mat4 scale;
        glm::mat4 rotation;
        glm::mat4 translation;
        glm::vec3 size;
    };

}
#endif // !_UGLProp_
