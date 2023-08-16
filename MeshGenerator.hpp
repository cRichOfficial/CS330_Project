/*
 * Author:        Christopher Richards
 * email:         christopher.richards@snhu.edu
 * Date:          07/30/2023
 * File:          MeshGenerator.hpp
 * Description:   This is a library I created to generate the vertex and index
 *                data for generating primitive shapes as our modules go on.
 *                I am certain there are already libraries built for this, but
 *                I wanted to see if I could do it myself.
 */

 // Includes
#include <vector>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <glm/glm.hpp>     // Include the GLFW library
#include "UGLProp.hpp"     // Include the UGLProp header
#pragma once

// This library will be part of the namespace "std"
namespace std {
    // Function declarations for generating primitive shapes.

    // Generate vertex and index data for a cylinder.
    RichWerks::Mesh generateCylinder(const float radius, const float height, const int segments);

    // Generate vertex and index data for a cube.
    RichWerks::Mesh generateCube(const float length, const float width, const float height);

    // Generate vertex and index data for a plane.
    RichWerks::Mesh generatePlane(const float length, const float width);

    // Generate vertex and index data for a pyramid.
    RichWerks::Mesh generatePyramid(const float baseLength, const float height);

    // Generate vertex and index data for a sphere.
    RichWerks::Mesh generateSphere(const float radius, const int divisions);

    // Generate vertex and index data for a torus.
    RichWerks::Mesh generateTorus(const float outerRadius, const float innerRadius, const int segments);

    // Rotate the vertices of a mesh around a given axis.
    void rotateMesh(RichWerks::Mesh& mesh, const float radians, const glm::vec3 axes);

    // Translate the vertices of a mesh by a given position.
    void translateMesh(RichWerks::Mesh& mesh, const glm::vec3 t_position);
}
