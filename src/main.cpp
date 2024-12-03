#include <GL/gl.h>
#include <bits/stdc++.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <cmath>
#include <vector>
#include "camera.h"
#include "solid_selector.h"
#include "vertex.h"
#include "solids.h"

#define MAX_VERTICES 100000

VertexManager vertexManager;


void drawSolid(const std::vector<Graphics::Vertex> vertices, const std::vector<GLubyte>& indices) {
    // enable vertex and color arrays
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    std::vector<GLfloat> positions;
    std::vector<GLfloat> colors;
    for (const auto& vertex : vertices) {
        positions.push_back(vertex.x);
        positions.push_back(vertex.y);
        positions.push_back(vertex.z);

        colors.push_back(vertex.r);
        colors.push_back(vertex.g);
        colors.push_back(vertex.b);
    }

    // pass vertex and color data to OpenGL
    glVertexPointer(3, GL_FLOAT, 0, positions.data());
    glColorPointer(3, GL_FLOAT, 0, colors.data());

    // draw elements
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_BYTE, indices.data());

    // disable vertex and color arrays
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}


int main() {
// Create the window using sf::RenderWindow to support drawing
    sf::RenderWindow window(sf::VideoMode(800, 600), "OpenGL Spinning Cube", sf::Style::Default, sf::ContextSettings(32));
    Camera camera;
    window.setVerticalSyncEnabled(true);

    // Create solid selector dropdown menu
    std::vector<std::string> items = { "Cube", "Pyramid"};
    SolidSelector solidSelector(10, 10, 150, 30, items);
    std::string selectedSolid = "";

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable back-face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Set up perspective projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float fov = 90.0f * 3.14159265f / 180.0f;  // Field of view in radians
    float aspect = static_cast<float>(800) / 600; // Aspect ratio
    float near = 0.1f;
    float far = 100.0f;
    camera.setFrustum(fov, aspect, near, far);  // Replace gluPerspective

    // Run the main loop
    bool running = true;
    float rotation_angle_h = 0;
    float rotation_angle_v = 0;

    while (running) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                running = false;
            } else if (event.type == sf::Event::Resized) {
                // Adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                float aspect = static_cast<float>(event.size.width) / event.size.height;
                camera.setFrustum(fov, aspect, near, far);  // Update frustum for the new aspect ratio
            }
            solidSelector.handleEvent(event, window);
        }

        // Handle camera input
        camera.handleInput(rotation_angle_h, rotation_angle_v);

        // Clear the color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set model-view matrix
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        camera.setDistance(1.5f);
        camera.setRotation(rotation_angle_h, rotation_angle_v);

        // Enable vertex and color arrays
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        selectedSolid = solidSelector.getSelectedText().getString().toAnsiString();
        if (selectedSolid == "Cube") {
            drawSolid(cubeVertices, cubeColors, cubeIndices, 36);
        }
        else if (selectedSolid == "Pyramid") {
            drawSolid(pyramidVertices, pyramidColors, pyramidIndices, 18);
        }

        // Draw the dropdown menu on top of OpenGL content
        window.pushGLStates();          // Save the OpenGL state
        solidSelector.draw(window);     // Draw the SFML dropdown
        window.popGLStates();            // Restore the OpenGL state

        // End the current frame
        window.display();
    }

    return 0;
}
