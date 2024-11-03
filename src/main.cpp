#include <bits/stdc++.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <cmath>
#include "camera.h"
#include "solid_selector.h"

// Cube vertices and color data
GLfloat vertices[] = {
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

GLfloat colors[] = {
    // Front face (red)
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    // Back face (green)
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
};

GLubyte indices[] = {
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


int main()
{
// Create the window using sf::RenderWindow to support drawing
    sf::RenderWindow window(sf::VideoMode(800, 600), "OpenGL Spinning Cube", sf::Style::Default, sf::ContextSettings(32));
    Camera camera;
    window.setVerticalSyncEnabled(true);

    // Create solid selector dropdown menu
    std::vector<std::string> items = { "Cube", "Pyramid", "Sphere" };
    SolidSelector solid_selector(10, 10, 150, 30, items);

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
            solid_selector.handleEvent(event, window);
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

        // Set vertex and color pointers (assuming vertices, colors, indices are defined elsewhere)
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glColorPointer(3, GL_FLOAT, 0, colors);

        // Draw the cube using index array
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

        // Disable vertex and color arrays
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);

        // Draw the dropdown menu on top of OpenGL content
        window.pushGLStates();          // Save the OpenGL state
        solid_selector.draw(window);     // Draw the SFML dropdown
        window.popGLStates();            // Restore the OpenGL state

        // End the current frame
        window.display();
    }

    return 0;
}
