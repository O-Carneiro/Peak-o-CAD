#pragma once

#include <SFML/Graphics.hpp>
#include <GL/glu.h>
#include <iostream>
#include "mesh.h"

class VertexDragger
{
private:

	Mesh* selectedMesh = NULL;
	int selectedVertex = -1;
	float currentDistance = 0.5f;

public:

	bool isSelected = false;

	VertexDragger(){}
	~VertexDragger(){}

	// void updateMeshList(std::vector<Mesh*> meshes)
	// {
	// 	this->meshes.clear();

	// 	for(auto* i : meshes)
	// 	{
	// 		this->meshes.push_back(i);
	// 	}
	// }

	// inline Mesh** getMeshes() { return this->meshes.data(); }

	void reset()
	{
		currentDistance = 0.5f;
		selectedMesh = NULL;
		selectedVertex = -1;
	}

	void selectVertex(const sf::Vector2i& mousePos, const sf::RenderWindow& window, Mesh* mesh) 
	{
		sf::Vector2u windowSize = window.getSize();

	    // read depth from depth buffer
	    GLfloat depth;
	    glReadPixels(mousePos.x, windowSize.y - mousePos.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

	    // get current matrices
	    GLdouble modelView[16], projection[16];
	    GLint viewport[4];
	    glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	    glGetDoublev(GL_PROJECTION_MATRIX, projection);
	    glGetIntegerv(GL_VIEWPORT, viewport);

	    // unproject the 2D point (mouse position) to 3D space
	    GLdouble worldX, worldY, worldZ;
	    gluUnProject(mousePos.x, windowSize.y - mousePos.y, depth, modelView, projection, viewport, &worldX, &worldY, &worldZ);

	    for(size_t i = 0; i < mesh->getVertexCount(); i += 3)
	    {
	    	float distance = std::pow(mesh->getVertices()[i] - worldX, 2) + std::pow(mesh->getVertices()[i + 1] - worldY, 2) + std::pow(mesh->getVertices()[i + 2] - worldZ, 2);

	        if (distance < currentDistance) {
	            selectedVertex = i;
	            selectedMesh = mesh;
	            currentDistance = distance;
	        }
	    }

	    // std::cout << "vertex selected?? " << selectedVertex << " " << isSelected << std::endl;
	}

	void dragVertex(const sf::Vector2i& mousePos, const sf::RenderWindow& window)
	{
		if (selectedVertex == -1) return;

	    // update the position of the selected vertex based on mouse drag
	    sf::Vector2u windowSize = window.getSize();

	    GLfloat depth;
	    glReadPixels(mousePos.x, windowSize.y - mousePos.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

	    // Tentando guardar essas matrizes na própria mesh (ainda n consertou :( )

	    // GLdouble modelView[16], projection[16];
	    // GLint viewport[4];
	    // glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	    // glGetDoublev(GL_PROJECTION_MATRIX, projection);
	    // glGetIntegerv(GL_VIEWPORT, viewport);

	    GLdouble worldX, worldY, worldZ;
	    gluUnProject(mousePos.x, (windowSize.y - mousePos.y), depth, selectedMesh->modelView, selectedMesh->projection, selectedMesh->viewport, &worldX, &worldY, &worldZ);
	    std::cout << worldX << " " << worldY << " " << worldZ << "\n";

	    if(mousePos.x < windowSize.x/2 && windowSize.y - mousePos.y < windowSize.y/2)
	    {
	    	selectedMesh->getVertices()[selectedVertex + 1] = worldY;
	    	selectedMesh->getVertices()[selectedVertex + 2] = worldZ;
	    }
	    else if(mousePos.x >= windowSize.x/2 && windowSize.y - mousePos.y < windowSize.y/2)
	    {
	    	selectedMesh->getVertices()[selectedVertex] = worldX;
	    	selectedMesh->getVertices()[selectedVertex + 1] = worldY;
	    }
	    else if(mousePos.x < windowSize.x/2 && windowSize.y - mousePos.y >= windowSize.y/2)
	    {
	    	selectedMesh->getVertices()[selectedVertex] = worldX;
	    	selectedMesh->getVertices()[selectedVertex + 2] = worldZ;
	    }

	    // selectedMesh->getVertices()[selectedVertex] = worldX;
	    // selectedMesh->getVertices()[selectedVertex + 1] = worldY;
	    // selectedMesh->getVertices()[selectedVertex + 2] = worldZ;
	}

	void handleEvent(const sf::Event &event, const sf::RenderWindow &window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
            //selectVertex(mousePos, window);
            this->isSelected = true;
        }
    } 
    else if (event.type == sf::Event::MouseButtonReleased) {
    	if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
            //selectVertex(mousePos, window);
            this->isSelected = false;
        }
    }
    else if (event.type == sf::Event::MouseMoved) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i mousePos(sf::Mouse::getPosition(window));
            dragVertex(mousePos, window);
        }
    }
}
};