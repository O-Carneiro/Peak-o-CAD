#pragma once

#include "primitive.h"

class Mesh
{
private:

	std::vector<GLfloat> vertices;
	std::vector<GLfloat> colors;
	std::vector<GLubyte> indices;

	std::vector<GLfloat> position;
	std::vector<GLfloat> rotation;
	std::vector<GLfloat> scale;

public:

	GLdouble modelView[16], projection[16];
	GLint viewport[4];

	Mesh(Primitive* primitive)
	{
		for(size_t i = 0; i < primitive->getVertexCount(); i++)
		{
			this->vertices.push_back(primitive->getVertices()[i]);
			this->colors.push_back(primitive->getColors()[i]);
		}

		for(size_t i = 0; i < primitive->getIndexCount(); i++)
		{
			this->indices.push_back(primitive->getIndices()[i]);
		}

		for(size_t i = 0; i < 3; i++)
		{
			this->position.push_back(0.0f);
			this->scale.push_back(1.0f);
		}

		for(size_t i = 0; i < 4; i++)
		{
			this->rotation.push_back(0.0f);
		}
	}
	
	Mesh(Primitive* primitive, GLfloat* position, GLfloat* rotation, GLfloat* scale)
	{
		for(size_t i = 0; i < primitive->getVertexCount(); i++)
		{
			this->vertices.push_back(primitive->getVertices()[i]);
			this->colors.push_back(primitive->getColors()[i]);
		}

		for(size_t i = 0; i < primitive->getIndexCount(); i++)
		{
			this->indices.push_back(primitive->getIndices()[i]);
		}

		for(size_t i = 0; i < 3; i++)
		{
			this->position.push_back(position[i]);
			this->scale.push_back(scale[i]);
		}

		for(size_t i = 0; i < 4; i++)
		{
			this->rotation.push_back(rotation[i]);
		}
	}

	~Mesh() {}

	inline GLfloat* getVertices() { return this->vertices.data(); }
	inline GLfloat* getColors() { return this->colors.data(); }
	inline GLubyte* getIndices() { return this->indices.data(); }
	inline GLfloat* getPosition() { return this->position.data(); }
	inline GLfloat* getRotation() { return this->rotation.data(); }
	inline GLfloat* getScale() { return this->scale.data(); }

	inline const unsigned getVertexCount() { return this->vertices.size(); }
	inline const unsigned getIndexCount() { return this->indices.size(); }

	void move(GLfloat* newPosition)
	{
		for(size_t i = 0; i < 3; i++)
		{
			this->position[i] = newPosition[i];
		}
	}

	void render()
	{
		glMatrixMode(GL_MODELVIEW);

        glTranslatef(position[0], position[1], position[2]);
        glRotatef(rotation[0], rotation[1], rotation[2], rotation[3]);
        glScalef(scale[0], scale[1], scale[2]);

        glVertexPointer(3, GL_FLOAT, 0, this->vertices.data());
  		glColorPointer(3, GL_FLOAT, 0, this->colors.data());

  		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_BYTE, this->indices.data());

  		glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	    glGetDoublev(GL_PROJECTION_MATRIX, projection);
	    glGetIntegerv(GL_VIEWPORT, viewport);
	}

};