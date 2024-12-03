#pragma once

#include <vector>
#include <SFML/OpenGL.hpp>

class Primitive
{
private:

	std::vector<GLfloat> vertices;
	std::vector<GLfloat> colors;
	std::vector<GLubyte> indices;

public:

	Primitive() {};
	virtual ~Primitive() {};

	void set(
		const GLfloat* vertices, 
		const GLfloat* colors, 
		const GLsizei vertexCount, 
		const GLubyte* indices, 
		const GLsizei indexCount
		)
	{
		for(size_t i = 0; i < vertexCount*3; i++)
		{
			this->vertices.push_back(vertices[i]);
			this->colors.push_back(colors[i]);
		}

		for(size_t i = 0; i < indexCount; i++)
		{
			this->indices.push_back(indices[i]);
		}
	}

	inline GLfloat* getVertices() { return this->vertices.data(); }
	inline GLfloat* getColors() { return this->colors.data(); }
	inline GLubyte* getIndices() { return this->indices.data(); }

	inline const unsigned getVertexCount() { return this->vertices.size(); }
	inline const unsigned getIndexCount() { return this->indices.size(); }
};