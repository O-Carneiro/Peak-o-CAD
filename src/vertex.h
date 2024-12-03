#ifndef VERTEX_H
#define VERTEX_H

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <GL/glu.h>
#include <iostream>
#include <cmath>
#include <vector>


namespace Graphics {
  struct Vertex {
    GLfloat x, y, z, r, g, b;
    int id;

    Vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r = 1.0f, GLfloat g = 1.0f, GLfloat b = 1.0f, int id = 0)
      : x(x), y(y), z(z), r(r), g(g), b(b), id(id) {}
  };
}

// forward declaring Solid struct here because of circular dependency problems
struct Solid;

class VertexManager {
  private:
    std::vector<Graphics::Vertex> vertices;
    // stores the ID of the vertex which is currently selected
    int selectedVertexId = -1;
    // threshold to select a vertex by clicking on it
    float selectionThreshold = 0.5f;
    bool redraw = false;

  public:

    void addVertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r = 1.0f, GLfloat g = 1.0f, GLfloat b = 1.0f);

    void addSolid(const Solid& solid);

    // get vertex positions as a raw pointer for OpenGL
    std::vector<GLfloat> getVertexPositions() const;

    bool selectVertex(const sf::Vector2i &mousePos,
                      const sf::RenderWindow &window);

    void dragVertex(const sf::Vector2i &mousePos, const sf::RenderWindow &window);

    void deselectVertex();

    void clear();

    void handleEvent(const sf::Event &event, const sf::RenderWindow &window);

    std::vector<Graphics::Vertex> getVertices();

    bool getRedraw();

    void setRedraw(bool needsRedraw);
};

#endif // VERTEX_H
