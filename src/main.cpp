#include <bits/stdc++.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <cmath>
#include "camera.h"
#include "solid_selector.h"

#define MAX_VERTICES 100000

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

// Define indices for each face of the pyramid
GLubyte pyramidIndices[] = {
    // Side faces
    0, 1, 2,  // Apex, Base-left, Base-right
    0, 2, 3,  // Apex, Base-right, Base-front
    0, 3, 1,  // Apex, Base-front, Base-left

    // Base face
    1, 3, 2   // Base-left, Base-front, Base-right (counterclockwise for back face culling)
};

void drawSolid(GLfloat vertices[], GLfloat colors[], GLubyte indices[], GLsizei indexCount) {
  // Set vertex and color pointers
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glColorPointer(3, GL_FLOAT, 0, colors);

  // Draw the cube using index array
  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_BYTE, indices);
}

void drawOption(std::string solid)
{
    if(solid == "Cube") drawSolid(cubeVertices, cubeColors, cubeIndices, 36);
    else if(solid == "Pyramid") drawSolid(pyramidVertices, pyramidColors, pyramidIndices, 18);
}

int main()
{
    int window_width = 800, window_height = 600;
// Create the window using sf::RenderWindow to support drawing
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "OpenGL Spinning Cube", sf::Style::Default, sf::ContextSettings(32));
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
                //glViewport(0, 0, event.size.width, event.size.height);
                window_width = event.size.width;
                window_height = event.size.height;
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
        // Draw the solid using index array

        // - Top-right (free camera)
        glViewport(window_width/2, window_height/2, window_width/2, window_height/2);
        drawOption(selectedSolid);

        // - Bottom-right (front view)
        glViewport(window_width/2, 0, window_width/2, window_height/2);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -1.5f);
        drawOption(selectedSolid);

        // - Top-left (up view)
        glViewport(0, window_height/2, window_width/2, window_height/2);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -1.5f);
        glRotatef(90, 1.0f, 0.0f, 0.0f);
        drawOption(selectedSolid);

        // - Bottom-left (side view)
        glViewport(0, 0, window_width/2, window_height/2);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -1.5f);
        glRotatef(90, 0.0f, 1.0f, 0.0f);
        drawOption(selectedSolid);
        
        // Disable vertex and color arrays
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);

        // Draw the dropdown menu on top of OpenGL content
        window.pushGLStates();          // Save the OpenGL state
        solidSelector.draw(window);     // Draw the SFML dropdown
        window.popGLStates();            // Restore the OpenGL state

        // End the current frame
        window.display();
    }

    return 0;
}
