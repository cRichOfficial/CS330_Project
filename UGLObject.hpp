#ifndef __OPEN_GL_LIBRARY__
#define __OPEN_GL_LIBRARY__

#include <GL/glew.h>               // GLEW library
#include <GLFW/glfw3.h>            // GLFW library
#include <glm/glm.hpp>             // glm library
#include <glm/gtx/transform.hpp>   // glm library
#include <glm/gtc/type_ptr.hpp>    // glm library

#endif

#include "shader.h"                 // Include the "shader.h" header
#include <vector>                  // Include the vector library

#pragma once
namespace RichWerks {

	// Enumeration to define different types of lighting
	enum struct LightingType : GLint { DIRECTIONAL_LIGHT, POINT_LIGHT, SPOT_LIGHT };

	// Structure to represent a light source
	struct Light {
		glm::vec3 color;              // Color of the light
		GLfloat padding_1;            // Padding to satisfy layout in GLSL
		glm::vec3 position;           // Position of the light source
		GLfloat lightLinear;          // Linear attenuation factor of the light
		glm::vec3 direction;          // Direction of the light (for spotlights)
		GLfloat lightConstant;        // Constant attenuation factor of the light
		LightingType type;            // Type of the light (directional, point, spot)
		GLfloat ambientIntensity;     // Ambient intensity of the light
		GLfloat specularIntensity;    // Specular intensity of the light
		GLfloat strength;             // Strength of the light
		GLfloat innerCutoff;          // Inner cutoff angle for spotlights
		GLfloat outerCutoff;          // Outer cutoff angle for spotlights
		GLint debug;                  // Debug flag for the light
		GLfloat padding_2;            // Padding to satisfy layout in GLSL
	};

	// Base class for OpenGL objects with position and direction
	class UGLObject
	{

	public:
		// Constructors
		UGLObject();
		UGLObject(glm::vec3 t_position, glm::vec3 t_direction);

		// Move constructor
		UGLObject(UGLObject&& object) noexcept;

		// Destructor
		~UGLObject() {};

		// Move assignment operator
		UGLObject& operator=(UGLObject&& object) noexcept;

		// Getter for object position
		glm::vec3 getPosition();

		// Setter for object position
		void setPosition(glm::vec3 t_position);
		void setPosition(GLfloat t_x, GLfloat t_y, GLfloat t_z);

		// Getter for object direction
		glm::vec3 getDirection();

		// Setter for object direction
		void setDirection(glm::vec3 t_direction);
		void setDirection(GLfloat t_x, GLfloat t_y, GLfloat t_z);

	protected:
		glm::vec3 position;         // Position of the object
		glm::vec3 direction;        // Direction of the object
	};

} // End of namespace RichWerks
