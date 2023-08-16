#include "MeshGenerator.hpp"

namespace std {
       
    RichWerks::Mesh generateCylinder(const float radius, const float height, const int segments) {
        RichWerks::Mesh cylinder;
        cylinder.size = glm::vec3(radius, height, radius);

        // Constants for generating the cylinder mesh.
        const int FLOATS_PER_POS = 3;
        const int FLOATS_PER_NORM = 3;
        const int FLOATS_PER_UV = 2;
        const int FLOATS_PER_VERT = FLOATS_PER_POS + FLOATS_PER_NORM + FLOATS_PER_UV;
        const float RADS_PER_SEG = (2 * M_PI) / segments * 1.0f;

        int firstIndex, nextIndex;

        // Begin building the bottom cap of the cylinder.
        firstIndex = cylinder.vertexData.size() / FLOATS_PER_VERT;
        // Initial values for the center vertex of the bottom cap.
        float x = 0.0f, y = 0.0f, z = 0.0f, nX = 0.0f, nY = -1.0f, nZ = 0.0f;
        float u = 0.5f, v = 0.5f; // Initial texture coordinates for center vertex.

        // Add center vertex data for bottom cap.
        cylinder.vertexData.insert(cylinder.vertexData.end(), { x, y, z, nX, nY, nZ, u, v });

        // Create the circular vertices for the bottom cap.
        for (int i = 0; i < segments; ++i) {
            float currAngle = RADS_PER_SEG * i;
            x = cos(currAngle) * radius;
            z = sin(currAngle) * radius;
            u = cos(currAngle) * 0.5f + 0.5f;
            v = sin(currAngle) * 0.5f + 0.5f;
            // Add the vertex data.
            cylinder.vertexData.insert(cylinder.vertexData.end(), { x, y, z, nX, nY, nZ, u, v });
        }

        // Index the bottom cap.
        nextIndex = firstIndex;
        for (int i = 0; i < segments; ++i) {
            cylinder.indexData.push_back(firstIndex);
            cylinder.indexData.push_back(nextIndex + 1);
            cylinder.indexData.push_back((i + 1 == segments ? firstIndex + 1 : nextIndex + 2));
            nextIndex += 1;
        }

        // Begin building the top cap of the cylinder.
        firstIndex = cylinder.vertexData.size() / FLOATS_PER_VERT;
        y = height;  // Adjust y to the height for the top cap.
        nY = 1.0f;   // Adjust normal Y direction for top cap.
        cylinder.vertexData.insert(cylinder.vertexData.end(), { x, y, z, nX, nY, nZ, u, v });

        // Create the circular vertices for the top cap.
        for (int i = 0; i < segments; ++i) {
            float currAngle = RADS_PER_SEG * i;
            x = cos(currAngle) * radius;
            z = sin(currAngle) * radius;
            u = cos(currAngle) * 0.5f + 0.5f;
            v = sin(currAngle) * 0.5f + 0.5f;
            cylinder.vertexData.insert(cylinder.vertexData.end(), { x, y, z, nX, nY, nZ, u, v });
        }

        // Index the top cap.
        nextIndex = firstIndex;
        for (int i = 0; i < segments; ++i) {
            cylinder.indexData.push_back(firstIndex);
            cylinder.indexData.push_back(nextIndex + 1);
            cylinder.indexData.push_back((i + 1 == segments ? firstIndex + 1 : nextIndex + 2));
            nextIndex += 1;
        }

        // Begin building the body of the cylinder.
        firstIndex = cylinder.vertexData.size() / FLOATS_PER_VERT;
        for (int i = 0; i <= segments + 1; ++i) {
            float currAngle = RADS_PER_SEG * i;
            x = cos(currAngle) * radius;
            z = sin(currAngle) * radius;
            nX = cos(currAngle);
            nZ = sin(currAngle);
            u = static_cast<float>(i * 1.0f / segments);
            v = 0.0f;
            cylinder.vertexData.insert(cylinder.vertexData.end(), { x, 0, z, nX, 0, nZ, u, v });
            v = 1.0f;
            cylinder.vertexData.insert(cylinder.vertexData.end(), { x, height, z, nX, 0, nZ, u, v });
        }

        // Index the body of the cylinder.
        nextIndex = firstIndex;
        for (int i = 0; i <= segments; ++i) {
            GLshort leftTriangle[3] = { nextIndex, nextIndex + 1, nextIndex + 3 };
            GLshort rightTriangle[3] = { nextIndex + 3, nextIndex + 2, nextIndex };
            cylinder.indexData.insert(cylinder.indexData.end(), { leftTriangle[0], leftTriangle[1], leftTriangle[2], rightTriangle[0], rightTriangle[1], rightTriangle[2] });
            nextIndex += 2;
        }

        return cylinder;
    }


    // This function is identical, but adds pos.x,y,z to vertex.x,y,z to place the cube in world space.
    RichWerks::Mesh generateCube(const float length, const float width, const float height) {
        RichWerks::Mesh cube;
        cube.size = glm::vec3(width, height, length);
        int i = 0;
        float x, y, z, nX, nY, nZ, u, v;

        //Front square
        // Bottom Left
        x = 0.0f - (width / 2.0f);
        y = 0.0f;
        z = 0.0f + (length / 2.0f);
        nX = 0.0f;
        nY = 0.0f;
        nZ = 1.0f;
        u = 0.0f;
        v = 0.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Bottom Right
        x = 0.0f + (width / 2.0f);
        y = 0.0f;
        z = 0.0f + (length / 2.0f);
        nX = 0.0f;
        nY = 0.0f;
        nZ = 1.0f;
        u = 1.0f;
        v = 0.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Top Right
        x = 0.0f + (width / 2.0f);
        y = 0.0f + height;
        z = 0.0f + (length / 2.0f);
        nX = 0.0f;
        nY = 0.0f;
        nZ = 1.0f;
        u = 1.0f;
        v = 1.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Top Left
        x = 0.0f - (width / 2.0f);
        y = 0.0f + height;
        z = 0.0f + (length / 2.0f);
        nX = 0.0f;
        nY = 0.0f;
        nZ = 1.0f;
        u = 0.0f;
        v = 1.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Add the indexData for this square
        // left triangle
        cube.indexData.push_back(i);
        cube.indexData.push_back(i + 1);
        cube.indexData.push_back(i + 3);
        
        // right triangle
        cube.indexData.push_back(i +1);
        cube.indexData.push_back(i + 2);
        cube.indexData.push_back(i + 3);

        i += 4;

        //Right square
        // Bottom Left
        x = 0.0f + (width / 2.0f);
        y = 0.0f;
        z = 0.0f + (length / 2.0f);
        nX = 1.0f;
        nY = 0.0f;
        nZ = 0.0f;
        u = 0.0f;
        v = 0.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Bottom Right
        x = 0.0f + (width / 2.0f);
        y = 0.0f;
        z = 0.0f - (length / 2.0f);
        nX = 1.0f;
        nY = 0.0f;
        nZ = 0.0f;
        u = 1.0f;
        v = 0.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Top Right
        x = 0.0f + (width / 2.0f);
        y = 0.0f + height;
        z = 0.0f - (length / 2.0f);
        nX = 1.0f;
        nY = 0.0f;
        nZ = 0.0f;
        u = 1.0f;
        v = 1.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Top Left
        x = 0.0f + (width / 2.0f);
        y = 0.0f + height;
        z = 0.0f + (length / 2.0f);
        nX = 1.0f;
        nY = 0.0f;
        nZ = 0.0f;
        u = 0.0f;
        v = 1.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Add the indexData for this square
        // left triangle
        cube.indexData.push_back(i);
        cube.indexData.push_back(i + 1);
        cube.indexData.push_back(i + 3);

        // right triangle
        cube.indexData.push_back(i + 1);
        cube.indexData.push_back(i + 2);
        cube.indexData.push_back(i + 3);

        i += 4;

        //Back square
        // Bottom Left
        x = 0.0f + (width / 2.0f);
        y = 0.0f;
        z = 0.0f - (length / 2.0f);
        nX = 0.0f;
        nY = 0.0f;
        nZ = -1.0f;
        u = 0.0f;
        v = 0.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Bottom Right
        x = 0.0f - (width / 2.0f);
        y = 0.0f;
        z = 0.0f - (length / 2.0f);
        nX = 0.0f;
        nY = 0.0f;
        nZ = -1.0f;
        u = 1.0f;
        v = 0.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Top Right
        x = 0.0f - (width / 2.0f);
        y = 0.0f + height;
        z = 0.0f - (length / 2.0f);
        nX = 0.0f;
        nY = 0.0f;
        nZ = -1.0f;
        u = 1.0f;
        v = 1.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Top Left
        x = 0.0f + (width / 2.0f);
        y = 0.0f + height;
        z = 0.0f - (length / 2.0f);
        nX = 0.0f;
        nY = 0.0f;
        nZ = -1.0f;
        u = 0.0f;
        v = 1.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Add the indexData for this square
        // left triangle
        cube.indexData.push_back(i);
        cube.indexData.push_back(i + 1);
        cube.indexData.push_back(i + 3);

        // right triangle
        cube.indexData.push_back(i + 1);
        cube.indexData.push_back(i + 2);
        cube.indexData.push_back(i + 3);

        i += 4;

        //Left square
        // Bottom Left
        x = 0.0f - (width / 2.0f);
        y = 0.0f;
        z = 0.0f - (length / 2.0f);
        nX = -1.0f;
        nY = 0.0f;
        nZ = 0.0f;
        u = 0.0f;
        v = 0.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Bottom Right
        x = 0.0f - (width / 2.0f);
        y = 0.0f;
        z = 0.0f + (length / 2.0f);
        nX = -1.0f;
        nY = 0.0f;
        nZ = 0.0f;
        u = 1.0f;
        v = 0.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Top Right
        x = 0.0f - (width / 2.0f);
        y = 0.0f + height;
        z = 0.0f + (length / 2.0f);
        nX = -1.0f;
        nY = 0.0f;
        nZ = 0.0f;
        u = 1.0f;
        v = 1.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Top Left
        x = 0.0f - (width / 2.0f);
        y = 0.0f + height;
        z = 0.0f - (length / 2.0f);
        nX = -1.0f;
        nY = 0.0f;
        nZ = 0.0f;
        u = 0.0f;
        v = 1.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Add the indexData for this square
        // left triangle
        cube.indexData.push_back(i);
        cube.indexData.push_back(i + 1);
        cube.indexData.push_back(i + 3);

        // right triangle
        cube.indexData.push_back(i + 1);
        cube.indexData.push_back(i + 2);
        cube.indexData.push_back(i + 3);

        i += 4;

        //Top square
        // Bottom Left
        x = 0.0f - (width / 2.0f);
        y = 0.0f + height;
        z = 0.0f + (length / 2.0f);
        nX = 0.0f;
        nY = 1.0f;
        nZ = 0.0f;
        u = 0.0f;
        v = 0.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Bottom Right
        x = 0.0f + (width / 2.0f);
        y = 0.0f + height;
        z = 0.0f + (length / 2.0f);
        nX = 0.0f;
        nY = 1.0f;
        nZ = 0.0f;
        u = 1.0f;
        v = 0.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Top Right
        x = 0.0f + (width / 2.0f);
        y = 0.0f + height;
        z = 0.0f - (length / 2.0f);
        nX = 0.0f;
        nY = 1.0f;
        nZ = 0.0f;
        u = 1.0f;
        v = 1.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Top Left
        x = 0.0f - (width / 2.0f);
        y = 0.0f + height;
        z = 0.0f - (length / 2.0f);
        nX = 0.0f;
        nY = 1.0f;
        nZ = 0.0f;
        u = 0.0f;
        v = 1.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Add the indexData for this square
        // left triangle
        cube.indexData.push_back(i);
        cube.indexData.push_back(i + 1);
        cube.indexData.push_back(i + 3);

        // right triangle
        cube.indexData.push_back(i + 1);
        cube.indexData.push_back(i + 2);
        cube.indexData.push_back(i + 3);

        i += 4;

        //Bottom square
        // Bottom Left
        x = 0.0f - (width / 2.0f);
        y = 0.0f;
        z = 0.0f + (length / 2.0f);
        nX = 0.0f;
        nY = -1.0f;
        nZ = 0.0f;
        u = 0.0f;
        v = 0.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Bottom Right
        x = 0.0f + (width / 2.0f);
        y = 0.0f;
        z = 0.0f + (length / 2.0f);
        nX = 0.0f;
        nY = -1.0f;
        nZ = 0.0f;
        u = 1.0f;
        v = 0.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Top Right
        x = 0.0f + (width / 2.0f);
        y = 0.0f;
        z = 0.0f - (length / 2.0f);
        nX = 0.0f;
        nY = -1.0f;
        nZ = 0.0f;
        u = 1.0f;
        v = 1.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Top Left
        x = 0.0f - (width / 2.0f);
        y = 0.0f;
        z = 0.0f - (length / 2.0f);
        nX = 0.0f;
        nY = -1.0f;
        nZ = 0.0f;
        u = 0.0f;
        v = 1.0f;
        
        // add the vertex
        cube.vertexData.push_back(x);
        cube.vertexData.push_back(y);
        cube.vertexData.push_back(z);
        cube.vertexData.push_back(nX);
        cube.vertexData.push_back(nY);
        cube.vertexData.push_back(nZ);
        cube.vertexData.push_back(u);
        cube.vertexData.push_back(v);

        // Add the indexData for this square
        // left triangle
        cube.indexData.push_back(i);
        cube.indexData.push_back(i + 1);
        cube.indexData.push_back(i + 3);

        // right triangle
        cube.indexData.push_back(i + 1);
        cube.indexData.push_back(i + 2);
        cube.indexData.push_back(i + 3);

        return cube;
    }

    RichWerks::Mesh generatePlane(const float length, const float width) {
        RichWerks::Mesh plane;

        // Set the dimensions of the plane.
        plane.size = glm::vec3(width, 0.0f, length);

        float x, y, z, nX, nY, nZ, u, v;

        // Each section below defines the vertex properties for a corner of the plane.

        // Top Right vertex properties [0]
        x = width / 2.0f;
        y = 0.0f;
        z = -length / 2.0f;
        nX = 0.0f;
        nY = 1.0f; // Normal pointing upwards for plane.
        nZ = 0.0f;
        u = 1.0f;  // Texture coordinate for top right corner.
        v = 1.0f;
        plane.vertexData.insert(plane.vertexData.end(), { x, y, z, nX, nY, nZ, u, v });

        // Bottom Right vertex properties [1]
        x = width / 2.0f;
        z = length / 2.0f;
        u = 1.0f;  // Texture coordinate for bottom right corner.
        v = 0.0f;
        plane.vertexData.insert(plane.vertexData.end(), { x, y, z, nX, nY, nZ, u, v });

        // Bottom Left vertex properties [2]
        x = -width / 2.0f;
        u = 0.0f;  // Texture coordinate for bottom left corner.
        v = 0.0f;
        plane.vertexData.insert(plane.vertexData.end(), { x, y, z, nX, nY, nZ, u, v });

        // Top Left vertex properties
        z = -length / 2.0f;
        u = 0.0f;  // Texture coordinate for top left corner.
        v = 1.0f;
        plane.vertexData.insert(plane.vertexData.end(), { x, y, z, nX, nY, nZ, u, v });

        // Define the indices that make up the two triangles of the plane.
        // The plane is divided into 2 triangles: 
        // 1. TopRight -> BottomLeft -> BottomRight
        // 2. TopRight -> BottomLeft -> TopLeft
        plane.indexData = {
            0, 2, 1,
            0, 2, 3
        };

        return plane;
    }


    // generate pyramid. Add texture and color parameters
    RichWerks::Mesh generatePyramid(const float baseLength, const float height) {
        RichWerks::Mesh pyramid;
        pyramid.size = glm::vec3(baseLength, height, baseLength);
        float x, y, z, nX, nY, nZ, u, v;

        // Triangle vertices that make up the pyramid (4 triangles make up the sides, 2 triangles make up the square base)
        int i = -1;
        // Trangle Front
        // Top
        x = 0.0;
        y = 0.0f + height;
        z = 0.0f;
        nX = 0.0f;
        nY = 0.0f;
        nZ = 1.0f;
        u = 0.5f;
        v = 1.0f;

        // add the vertex
        pyramid.vertexData.push_back(x);
        pyramid.vertexData.push_back(y);
        pyramid.vertexData.push_back(z);
        pyramid.vertexData.push_back(nX);
        pyramid.vertexData.push_back(nY);
        pyramid.vertexData.push_back(nZ);
        pyramid.vertexData.push_back(u);
        pyramid.vertexData.push_back(v);

        pyramid.indexData.push_back(++i);

        // Bottom left
        x = 0.0 - (baseLength / 2.0f);
        y = 0.0f;
        z = 0.0f + (baseLength / 2.0f);
        nX = 0.0f;
        nY = 0.0f;
        nZ = 1.0f;
        u = 0.0f;
        v = 0.0f;

        // add the vertex
        pyramid.vertexData.push_back(x);
        pyramid.vertexData.push_back(y);
        pyramid.vertexData.push_back(z);
        pyramid.vertexData.push_back(nX);
        pyramid.vertexData.push_back(nY);
        pyramid.vertexData.push_back(nZ);
        pyramid.vertexData.push_back(u);
        pyramid.vertexData.push_back(v);

        pyramid.indexData.push_back(++i);


        // Bottom bottom right
        x = 0.0 + (baseLength / 2.0f);
        y = 0.0f;
        z = 0.0f + (baseLength / 2.0f);
        nX = 0.0f;
        nY = 0.0f;
        nZ = 1.0f;
        u = 1.0f;
        v = 0.0f;

        // add the vertex
        pyramid.vertexData.push_back(x);
        pyramid.vertexData.push_back(y);
        pyramid.vertexData.push_back(z);
        pyramid.vertexData.push_back(nX);
        pyramid.vertexData.push_back(nY);
        pyramid.vertexData.push_back(nZ);
        pyramid.vertexData.push_back(u);
        pyramid.vertexData.push_back(v);

        pyramid.indexData.push_back(++i);

        // Right triangle
       // Top
        x = 0.0;
        y = 0.0f + height;
        z = 0.0f;
        nX = 1.0f;
        nY = 0.0f;
        nZ = 0.0f;
        u = 0.5f;
        v = 1.0f;

        // add the vertex
        pyramid.vertexData.push_back(x);
        pyramid.vertexData.push_back(y);
        pyramid.vertexData.push_back(z);
        pyramid.vertexData.push_back(nX);
        pyramid.vertexData.push_back(nY);
        pyramid.vertexData.push_back(nZ);
        pyramid.vertexData.push_back(u);
        pyramid.vertexData.push_back(v);

        pyramid.indexData.push_back(++i);

        // Bottom left
        x = 0.0f + (baseLength / 2.0f);
        y = 0.0f;
        z = 0.0f + (baseLength / 2.0f);
        nX = 1.0f;
        nY = 0.0f;
        nZ = 0.0f;
        u = 0.0f;
        v = 0.0f;

        // add the vertex
        pyramid.vertexData.push_back(x);
        pyramid.vertexData.push_back(y);
        pyramid.vertexData.push_back(z);
        pyramid.vertexData.push_back(nX);
        pyramid.vertexData.push_back(nY);
        pyramid.vertexData.push_back(nZ);
        pyramid.vertexData.push_back(u);
        pyramid.vertexData.push_back(v);

        pyramid.indexData.push_back(++i);

        // Bottom bottom right
        x = 0.0f + (baseLength / 2.0f);
        y = 0.0f;
        z = 0.0f - (baseLength / 2.0f);
        nX = 1.0f;
        nY = 0.0f;
        nZ = 0.0f;
        u = 1.0f;
        v = 0.0f;

        // add the vertex
        pyramid.vertexData.push_back(x);
        pyramid.vertexData.push_back(y);
        pyramid.vertexData.push_back(z);
        pyramid.vertexData.push_back(nX);
        pyramid.vertexData.push_back(nY);
        pyramid.vertexData.push_back(nZ);
        pyramid.vertexData.push_back(u);
        pyramid.vertexData.push_back(v);

        pyramid.indexData.push_back(++i);

        // Back triangle
       // Top
        x = 0.0f;
        y = 0.0f + height;
        z = 0.0f;
        nX = 0.0f;
        nY = 0.0f;
        nZ = -1.0f;
        u = 0.5f;
        v = 1.0f;

        // add the vertex
        pyramid.vertexData.push_back(x);
        pyramid.vertexData.push_back(y);
        pyramid.vertexData.push_back(z);
        pyramid.vertexData.push_back(nX);
        pyramid.vertexData.push_back(nY);
        pyramid.vertexData.push_back(nZ);
        pyramid.vertexData.push_back(u);
        pyramid.vertexData.push_back(v);

        pyramid.indexData.push_back(++i);

        // Bottom left
        x = 0.0f + (baseLength / 2.0f);
        y = 0.0f;
        z = 0.0f - (baseLength / 2.0f);
        nX = 0.0f;
        nY = 0.0f;
        nZ = -1.0f;
        u = 0.0f;
        v = 0.0f;

        // add the vertex
        pyramid.vertexData.push_back(x);
        pyramid.vertexData.push_back(y);
        pyramid.vertexData.push_back(z);
        pyramid.vertexData.push_back(nX);
        pyramid.vertexData.push_back(nY);
        pyramid.vertexData.push_back(nZ);
        pyramid.vertexData.push_back(u);
        pyramid.vertexData.push_back(v);

        pyramid.indexData.push_back(++i);

        // Bottom bottom right
        x = 0.0f - (baseLength / 2.0f);
        y = 0.0f;
        z = 0.0f - (baseLength / 2.0f);
        nX = 0.0f;
        nY = 0.0f;
        nZ = -1.0f;
        u = 1.0f;
        v = 0.0f;

        // add the vertex
        pyramid.vertexData.push_back(x);
        pyramid.vertexData.push_back(y);
        pyramid.vertexData.push_back(z);
        pyramid.vertexData.push_back(nX);
        pyramid.vertexData.push_back(nY);
        pyramid.vertexData.push_back(nZ);
        pyramid.vertexData.push_back(u);
        pyramid.vertexData.push_back(v);

        pyramid.indexData.push_back(++i);

        // Left triangle
       // Top
        x = 0.0f;
        y = 0.0f + height;
        z = 0.0f;
        nX = -1.0f;
        nY = 0.0f;
        nZ = 0.0f;
        u = 0.5f;
        v = 1.0f;

        // add the vertex
        pyramid.vertexData.push_back(x);
        pyramid.vertexData.push_back(y);
        pyramid.vertexData.push_back(z);
        pyramid.vertexData.push_back(nX);
        pyramid.vertexData.push_back(nY);
        pyramid.vertexData.push_back(nZ);
        pyramid.vertexData.push_back(u);
        pyramid.vertexData.push_back(v);

        pyramid.indexData.push_back(++i);

        // Bottom left
        x = 0.0 - (baseLength / 2.0f);
        y = 0.0f;
        z = 0.0f - (baseLength / 2.0f);
        nX = -1.0f;
        nY = 0.0f;
        nZ = 0.0f;
        u = 0.0f;
        v = 0.0f;

        // add the vertex
        pyramid.vertexData.push_back(x);
        pyramid.vertexData.push_back(y);
        pyramid.vertexData.push_back(z);
        pyramid.vertexData.push_back(nX);
        pyramid.vertexData.push_back(nY);
        pyramid.vertexData.push_back(nZ);
        pyramid.vertexData.push_back(u);
        pyramid.vertexData.push_back(v);

        pyramid.indexData.push_back(++i);

        // Bottom  right
        x = 0.0 - (baseLength / 2.0f);
        y = 0.0f;
        z = 0.0f + (baseLength / 2.0f);
        nX = -1.0f;
        nY = 0.0f;
        nZ = 0.0f;
        u = 1.0f;
        v = 0.0f;

        // add the vertex
        pyramid.vertexData.push_back(x);
        pyramid.vertexData.push_back(y);
        pyramid.vertexData.push_back(z);
        pyramid.vertexData.push_back(nX);
        pyramid.vertexData.push_back(nY);
        pyramid.vertexData.push_back(nZ);
        pyramid.vertexData.push_back(u);
        pyramid.vertexData.push_back(v);

        pyramid.indexData.push_back(++i);

        // Base of pyramid
        // Front-left
        x = 0.0f - (baseLength / 2.0f);
        y = 0.0f;
        z = 0.0f + (baseLength / 2.0f);
        nX = 0.0f;
        nY = -1.0f;
        nZ = 0.0f;
        u = 0.0f;
        v = 0.0f;

        // add the vertex
        pyramid.vertexData.push_back(x);
        pyramid.vertexData.push_back(y);
        pyramid.vertexData.push_back(z);
        pyramid.vertexData.push_back(nX);
        pyramid.vertexData.push_back(nY);
        pyramid.vertexData.push_back(nZ);
        pyramid.vertexData.push_back(u);
        pyramid.vertexData.push_back(v);

        // Base of pyramid
        // Front-right
        x = 0.0 + (baseLength / 2.0f);
        y = 0.0f;
        z = 0.0f + (baseLength / 2.0f);
        nX = 0.0f;
        nY = -1.0f;
        nZ = 0.0f;
        u = 1.0f;
        v = 0.0f;

        // add the vertex
        pyramid.vertexData.push_back(x);
        pyramid.vertexData.push_back(y);
        pyramid.vertexData.push_back(z);
        pyramid.vertexData.push_back(nX);
        pyramid.vertexData.push_back(nY);
        pyramid.vertexData.push_back(nZ);
        pyramid.vertexData.push_back(u);
        pyramid.vertexData.push_back(v);

        // Base of pyramid
        // Back-right
        x = 0.0 + (baseLength / 2.0f);
        y = 0.0f;
        z = 0.0f - (baseLength / 2.0f);
        nX = 0.0f;
        nY = -1.0f;
        nZ = 0.0f;
        u = 1.0f;
        v = 1.0f;

        // add the vertex
        pyramid.vertexData.push_back(x);
        pyramid.vertexData.push_back(y);
        pyramid.vertexData.push_back(z);
        pyramid.vertexData.push_back(nX);
        pyramid.vertexData.push_back(nY);
        pyramid.vertexData.push_back(nZ);
        pyramid.vertexData.push_back(u);
        pyramid.vertexData.push_back(v);

        // Base of pyramid
        // Back-left
        x = 0.0 - (baseLength / 2.0f);
        y = 0.0f;
        z = 0.0f - (baseLength / 2.0f);
        nX = 0.0f;
        nY = -1.0f;
        nZ = 0.0f;
        u = 0.0f;
        v = 1.0f;

        // add the vertex
        pyramid.vertexData.push_back(x);
        pyramid.vertexData.push_back(y);
        pyramid.vertexData.push_back(z);
        pyramid.vertexData.push_back(nX);
        pyramid.vertexData.push_back(nY);
        pyramid.vertexData.push_back(nZ);
        pyramid.vertexData.push_back(u);
        pyramid.vertexData.push_back(v);

        ++i;
        pyramid.indexData.push_back(i);
        pyramid.indexData.push_back(i + 1);
        pyramid.indexData.push_back(i + 3);

        pyramid.indexData.push_back(i + 1);
        pyramid.indexData.push_back(i + 2);
        pyramid.indexData.push_back(i + 3);

        return pyramid;
    }

    RichWerks::Mesh generateSphere(const float radius, const int divisions) {
        RichWerks::Mesh sphere;
        const int rings = divisions;
        const int sectors = divisions;
        // Calculate the number of vertices and indices needed
        //const int numVertices = (rings + 1) * (sectors + 1);
        //const int numIndices = rings * sectors * 6; // 2 triangles per sector, 3 indices per triangle

        // Reserve space for vertices and indices to avoid reallocations
        //sphere.vertexData.reserve(numVertices * 8); // Each vertex has 8 floats (3 pos, 3 norm, 2 tex)
        //sphere.indexData.reserve(numIndices);

        // Calculate angles
        const float RADS_PER_RING = M_PI / rings;
        const float RADS_PER_SECTOR = 2.0f * M_PI / sectors;

        for (int ring = 0; ring <= rings; ++ring) {
            for (int sector = 0; sector <= sectors; ++sector) {
                const float theta = ring * RADS_PER_RING;
                const float phi = sector * RADS_PER_SECTOR;

                const float x = radius * sin(theta) * cos(phi);
                const float y = radius * cos(theta);
                const float z = radius * sin(theta) * sin(phi);

                const float u = static_cast<float>(sector) / sectors;
                const float v = static_cast<float>(ring) / rings;

                sphere.vertexData.push_back(x);
                sphere.vertexData.push_back(y);
                sphere.vertexData.push_back(z);

                const glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));
                sphere.vertexData.push_back(normal.x);
                sphere.vertexData.push_back(normal.y);
                sphere.vertexData.push_back(normal.z);

                sphere.vertexData.push_back(u);
                sphere.vertexData.push_back(v);
            }
        }

        for (int ring = 0; ring < rings; ++ring) {
            for (int sector = 0; sector < sectors; ++sector) {
                const int current = ring * (sectors + 1) + sector;
                const int next = current + sectors + 1;

                sphere.indexData.push_back(current);
                sphere.indexData.push_back(next);
                sphere.indexData.push_back(current + 1);

                sphere.indexData.push_back(current + 1);
                sphere.indexData.push_back(next);
                sphere.indexData.push_back(next + 1);
            }
        }

        return sphere;
    }

    RichWerks::Mesh generateTorus(const float outerRadius, const float innerRadius, const int segments) {
        RichWerks::Mesh torus;
        const int majorSegments = segments;
        const int minorSegments = segments;
        float x, y, z, nX, nY, nZ, u, v;
        int indexCounter = 0;

        const float majorRadsPerSeg = (2 * glm::pi<float>()) / majorSegments;
        const float minorRadsPerSeg = (2 * glm::pi<float>()) / minorSegments;

        for (int i = 0; i < majorSegments; ++i) {
            for (int j = 0; j < minorSegments; ++j) {
                float majorCurrAngle = majorRadsPerSeg * i;
                float minorCurrAngle = minorRadsPerSeg * j;

                // Calculate vertex position
                x = (outerRadius + innerRadius * cos(minorCurrAngle)) * cos(majorCurrAngle);
                y = innerRadius * sin(minorCurrAngle);
                z = (outerRadius + innerRadius * cos(minorCurrAngle)) * sin(majorCurrAngle);

                // Calculate vertex normal
                nX = cos(majorCurrAngle) * cos(minorCurrAngle);
                nY = sin(minorCurrAngle);
                nZ = sin(majorCurrAngle) * cos(minorCurrAngle);

                // Calculate texture coordinates
                u = static_cast<float>(i) / majorSegments;
                v = static_cast<float>(j) / minorSegments;

                // Add the vertex
                torus.vertexData.push_back(x);
                torus.vertexData.push_back(y);
                torus.vertexData.push_back(z);
                torus.vertexData.push_back(nX);
                torus.vertexData.push_back(nY);
                torus.vertexData.push_back(nZ);
                torus.vertexData.push_back(u);
                torus.vertexData.push_back(v);

                // Calculate indices for the triangles
                int nextRow = (j + 1) % minorSegments;
                int nextCol = (i + 1) % majorSegments;

                int currentIndex = j * majorSegments + i;
                int nextRowIndex = nextRow * majorSegments + i;
                int nextColIndex = j * majorSegments + nextCol;
                int nextRowNextColIndex = nextRow * majorSegments + nextCol;

                // Add indices for the two triangles
                torus.indexData.push_back(currentIndex);
                torus.indexData.push_back(nextRowIndex);
                torus.indexData.push_back(nextColIndex);

                torus.indexData.push_back(nextRowIndex);
                torus.indexData.push_back(nextRowNextColIndex);
                torus.indexData.push_back(nextColIndex);

                indexCounter += 6; // 6 indices were added for the two triangles
            }
        }

        return torus;
    }

    void rotateMesh(RichWerks::Mesh& mesh, const float radians, const glm::vec3 axes) {
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), radians, axes);

        for (int i = 0; i < mesh.vertexData.size(); i += 8) {
            glm::vec4 vertPosition(mesh.vertexData[i], mesh.vertexData[i + 1], mesh.vertexData[i + 2], 1.0f);
            glm::vec4 newPosition = rotationMatrix * vertPosition;

            mesh.vertexData[i] = newPosition.x;
            mesh.vertexData[i + 1] = newPosition.y;
            mesh.vertexData[i + 2] = newPosition.z;

            // Rotate normals using the same rotation matrix
            glm::vec4 vertNormal(mesh.vertexData[i + 3], mesh.vertexData[i + 4], mesh.vertexData[i + 5], 0.0f);
            glm::vec4 newNormal = rotationMatrix * vertNormal;

            mesh.vertexData[i + 3] = newNormal.x;
            mesh.vertexData[i + 4] = newNormal.y;
            mesh.vertexData[i + 5] = newNormal.z;
        }
    }

    void translateMesh(RichWerks::Mesh& mesh, const glm::vec3 t_position){
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), t_position);

        for (int i = 0; i < mesh.vertexData.size(); i += 8) {
            glm::vec4 originalPos(glm::vec3(mesh.vertexData[i], mesh.vertexData[i + 1], mesh.vertexData[i + 2]), 1.0f);
            glm::vec4 newPosition = translationMatrix * originalPos;

            mesh.vertexData[i] = newPosition.x;
            mesh.vertexData[i + 1] = newPosition.y;
            mesh.vertexData[i + 2] = newPosition.z;
        }
    }
}