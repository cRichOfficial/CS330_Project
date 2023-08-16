#include "UGLObject.hpp"         // Include the class header
using namespace RichWerks;

// Default constructor: Initializes position and direction to zero vectors
UGLObject::UGLObject() {
	position = glm::vec3(0.0f);
	direction = glm::vec3(0.0f);
}

// Parameterized constructor: Initializes position and direction with provided values
UGLObject::UGLObject(glm::vec3 t_position, glm::vec3 t_direction) {
	position = t_position;
	direction = t_direction;
}

// Move constructor: Moves the position and direction from the provided object
UGLObject::UGLObject(UGLObject&& object) noexcept
	: position(std::move(object.position)),
	direction(std::move(object.direction)) {
	// No need to do anything else, as position and direction have been moved
}

// Move assignment operator: Moves the position and direction from the provided object
UGLObject& UGLObject::operator=(UGLObject&& object) noexcept {
	if (this != &object) {
		position = std::move(object.position);
		direction = std::move(object.direction);
		// No need to do anything else, as position and direction have been moved
	}
	return *this;
}

// Getter for object position
glm::vec3 UGLObject::getPosition() {
	return position;
}

// Setter for object position
void UGLObject::setPosition(glm::vec3 t_position) {
	position = t_position;
}

// Setter for object position with individual components
void UGLObject::setPosition(GLfloat t_x, GLfloat t_y, GLfloat t_z) {
	position = glm::vec3(t_x, t_y, t_z);
}

// Getter for object direction
glm::vec3 UGLObject::getDirection() {
	return direction;
}

// Setter for object direction
void UGLObject::setDirection(glm::vec3 t_direction) {
	direction = t_direction;
}

// Setter for object direction with individual components
void UGLObject::setDirection(GLfloat t_x, GLfloat t_y, GLfloat t_z) {
	direction = glm::vec3(t_x, t_y, t_z);
}
