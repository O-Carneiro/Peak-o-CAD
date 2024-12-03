#include "vertex.h"
#include "solids.h"

void VertexManager::addVertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b) {
    // assign a unique ID
    int id = vertices.size();
    // constructs vertex object and push_back() it
    vertices.emplace_back(x, y, z, r, g, b, id);
}

void VertexManager::addSolid(const Solid& solid) {
    for (const auto& vertex: solid.vertices) {
        addVertex(vertex.x, vertex.y, vertex.z, vertex.r, vertex.g, vertex.b);
    }
}

std::vector<GLfloat> VertexManager::getVertexPositions() const {
    std::vector<GLfloat> data;
    for (const auto& vertex : vertices) {
        data.push_back(vertex.x);
        data.push_back(vertex.y);
        data.push_back(vertex.z);
        data.push_back(vertex.r);
        data.push_back(vertex.g);
        data.push_back(vertex.b);
    }
    return data;
}

bool VertexManager::selectVertex(const sf::Vector2i& mousePos, const sf::RenderWindow& window) {
    // convert mouse coordinates to normalized device coordinates, which OpenGL understands
    // it is defined in the interval (-1, 1)
    sf::Vector2u windowSize = window.getSize();
    GLfloat ndcX = 2.0f * mousePos.x / windowSize.x - 1.0f;
    GLfloat ndcY = 1.0f - 2.0f * mousePos.y / windowSize.y;

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

    float minDistance = selectionThreshold;
    selectedVertexId = -1;

    // loops through all vertices and find out which is closest
    for (const auto& vertex : vertices) {
        float distance = std::sqrt(
            std::pow(vertex.x - worldX, 2) +
            std::pow(vertex.y - worldY, 2) +
            std::pow(vertex.z - worldZ, 2)
        );

        if (distance < minDistance) {
            selectedVertexId = vertex.id;
            minDistance = distance;
        }
    }

    std::cout << "vertex selected?? " << selectedVertexId << std::endl;
    return selectedVertexId != -1;
}

void VertexManager::dragVertex(const sf::Vector2i& mousePos, const sf::RenderWindow& window) {
    if (selectedVertexId == -1) return;

    // update the position of the selected vertex based on mouse drag
    sf::Vector2u windowSize = window.getSize();

    GLfloat depth;
    glReadPixels(mousePos.x, windowSize.y - mousePos.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

    GLdouble modelView[16], projection[16];
    GLint viewport[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLdouble worldX, worldY, worldZ;
    gluUnProject(mousePos.x, windowSize.y - mousePos.y, depth, modelView, projection, viewport, &worldX, &worldY, &worldZ);

    vertices[selectedVertexId].x = worldX;
    vertices[selectedVertexId].y = worldY;
    vertices[selectedVertexId].z = worldZ;

    redraw = true;

    std::cout << "vvv VERTICES NOW vvv" << std::endl;
    for (const auto& vertex : vertices) {
        std::cout << vertex.x << ", " << vertex.y << ", " << vertex.z << " - " << vertex.r << ", " << vertex.g << ", " << vertex.b << std::endl;
    }
}

void VertexManager::deselectVertex() {
    selectedVertexId = -1;
}

void VertexManager::clear() {
    vertices.clear();
}

void VertexManager::handleEvent(const sf::Event &event, const sf::RenderWindow &window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
            selectVertex(mousePos, window);
        }
    } else if (event.type == sf::Event::MouseMoved) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i mousePos(sf::Mouse::getPosition(window));
            dragVertex(mousePos, window);
        }
    }
}

std::vector<Graphics::Vertex> VertexManager::getVertices() {
    return vertices;
}

bool VertexManager::getRedraw() {
    return redraw;
}

void VertexManager::setRedraw(bool needsRedraw) {
    redraw = needsRedraw;
}
