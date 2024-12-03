#include "primitive.h"

class Cube : public Primitive
{
public:

	Cube() : Primitive()
	{
		// Cube vertices and color data
		GLfloat cubeVertices[] = {
		    // Front face
		    -0.5f, -0.5f,  0.5f,  // Bottom-left
		     0.5f, -0.5f,  0.5f,  // Bottom-right
		     0.5f,  0.5f,  0.5f,  // Top-right
		    -0.5f,  0.5f,  0.5f,  // Top-left

		    // Back face
		    -0.5f, -0.5f, -0.5f,  // Bottom-left
		     0.5f, -0.5f, -0.5f,  // Bottom-right
		     0.5f,  0.5f, -0.5f,  // Top-right
		    -0.5f,  0.5f, -0.5f   // Top-left
		};

		GLfloat cubeColors[] = {
		    // Front face (red)
		    0.0f, 1.0f, 1.0f,
		    0.0f, 0.0f, 1.0f,
		    0.0f, 0.0f, 1.0f,
		    0.0f, 0.0f, 1.0f,

		    // Back face (green)
		    0.0f, 1.0f, 0.0f,
		    0.0f, 1.0f, 0.0f,
		    0.0f, 1.0f, 0.0f,
		    0.0f, 1.0f, 0.0f
		};

		GLsizei vertexCount = sizeof(cubeVertices) / (3 * sizeof(GLfloat));

		GLubyte cubeIndices[] = {
		    // Front face (counterclockwise)
		    0, 1, 2,
		    2, 3, 0,
		    
		    // Back face (counterclockwise)
		    4, 7, 6,
		    6, 5, 4,

		    // Left face (counterclockwise)
		    0, 3, 7,
		    7, 4, 0,

		    // Right face (counterclockwise)
		    1, 5, 6,
		    6, 2, 1,

		    // Top face (counterclockwise)
		    3, 2, 6,
		    6, 7, 3,

		    // Bottom face (counterclockwise)
		    0, 4, 5,
		    5, 1, 0
		};

		GLsizei indexCount = sizeof(cubeIndices) / sizeof(GLubyte);

		this->set(cubeVertices, cubeColors, vertexCount, cubeIndices, indexCount);
	}
};

class Pyramid : public Primitive
{
public:

	Pyramid() : Primitive()
	{
		// Pyramid vertices and color data
		GLfloat pyramidVertices[] = {
		    // Apex (top point of the pyramid)
		    0.0f, 0.5f, 0.0f,   // Vertex 0 (Apex)

		    // Base vertices
		    -0.5f, -0.5f, -0.5f, // Vertex 1 (Base - left)
		     0.5f, -0.5f, -0.5f, // Vertex 2 (Base - right)
		     0.0f, -0.5f,  0.5f  // Vertex 3 (Base - front)
		};

		// Define colors for each vertex (can be customized for each face)
		GLfloat pyramidColors[] = {
		    // Apex color (e.g., red)
		    1.0f, 0.0f, 0.0f,  // Apex

		    // Base vertices (e.g., green)
		    0.0f, 1.0f, 0.0f,  // Base - left
		    0.0f, 1.0f, 0.0f,  // Base - right
		    0.0f, 1.0f, 0.0f   // Base - front
		};

		GLsizei vertexCount = sizeof(pyramidVertices) / (3 * sizeof(GLfloat));

		// Define indices for each face of the pyramid
		GLubyte pyramidIndices[] = {
		    // Side faces
		    /*0, 1, 2,  // Apex, Base-left, Base-right
		    0, 2, 3,  // Apex, Base-right, Base-front
		    0, 3, 1,  // Apex, Base-front, Base-left*/
		    0, 2, 1,
		    0, 3, 2,
		    0, 1, 3,

		    // Base face
		    1, 2, 3   // Base-left, Base-front, Base-right (counterclockwise for back face culling)
		};

		GLsizei indexCount = sizeof(pyramidIndices) / sizeof(GLubyte);

		this->set(pyramidVertices, pyramidColors, vertexCount, pyramidIndices, indexCount);
	}
};