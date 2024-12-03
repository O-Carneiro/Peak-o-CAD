#ifndef SOLIDS_H
#define SOLIDS_H

#include <vector>
#include "vertex.h"

// Structure to represent a solid with vertices, colors, and indices
class Solid {
public:
    std::vector<Graphics::Vertex> vertices;
    std::vector<GLubyte> indices;

Solid(const std::vector<Graphics::Vertex>& vertices,
      const std::vector<GLubyte>& indices)
    : vertices(vertices), indices(indices) {}
};

class Cube : public Solid {
public:
Cube() : Solid(getCubeVertices(), getCubeIndices()) {}

    // Cube-specific vertices
    static std::vector<Graphics::Vertex> getCubeVertices() {
        return {
        // Front face (blue)
        { -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f },  // Bottom-left
        {  0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f },  // Bottom-right
        {  0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f },  // Top-right
        { -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f },  // Top-left

        // Back face (green)
        { -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f },  // Bottom-left
        {  0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f },  // Bottom-right
        {  0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f },  // Top-right
        { -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f }   // Top-left
    };
    }

    // Cube-specific indices
    static std::vector<GLubyte> getCubeIndices() {
        return {
        0, 1, 2, 2, 3, 0,   // Front face
        4, 7, 6, 6, 5, 4,   // Back face
            0, 3, 7, 7, 4, 0,   // Left face
            1, 5, 6, 6, 2, 1,   // Right face
            3, 2, 6, 6, 7, 3,   // Top face
            0, 4, 5, 5, 1, 0    // Bottom face
    };
    }
};

// Pyramid definition
class Pyramid : public Solid {
public:
Pyramid() : Solid(getPyramidVertices(), getPyramidIndices()) {}

    // Pyramid-specific vertices
    static std::vector<Graphics::Vertex> getPyramidVertices() {
        return {
        {  0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f },  // Apex (red)
        { -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f },  // Base - left (green)
        {  0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f },  // Base - right (green)
        {  0.0f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f }   // Base - front (green)
    };
    }

    // Pyramid-specific indices
    static std::vector<GLubyte> getPyramidIndices() {
        return {
        0, 1, 2,  // Side face 1
        0, 2, 3,  // Side face 2
        0, 3, 1,  // Side face 3
        1, 3, 2   // Base
    };
    }
};

#endif // SOLIDS_H
