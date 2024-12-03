#include <bits/stdc++.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <cmath>
#include "camera.h"
#include "solid_selector.h"
#include "solids.h"
#include "mesh.h"

#define MAX_VERTICES 100000

// void drawSolid(GLfloat vertices[], GLfloat colors[], GLubyte indices[], GLsizei indexCount) {
//   // Set vertex and color pointers
//   glVertexPointer(3, GL_FLOAT, 0, vertices);
//   glColorPointer(3, GL_FLOAT, 0, colors);

//   // Draw the cube using index array
//   glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_BYTE, indices);
// }

// void drawOption(std::string solid)
// {
//     if(solid == "Cube") drawSolid(cubeVertices, cubeColors, cubeIndices, 36);
//     else if(solid == "Pyramid") drawSolid(pyramidVertices, pyramidColors, pyramidIndices, 18);
// }

int main()
{
    // Default solids
    Cube* cube = new Cube();
    Pyramid* pyr = new Pyramid();

    // Example meshes
    Mesh* mesh = new Mesh(cube);
    Mesh* mesh2 = new Mesh(cube);
    Mesh* mesh3 = new Mesh(pyr);
    GLfloat pos[3] = {1.0f, 0.0f, 0.0f};
    GLfloat pos1[3] = {-1.0f, 0.0f, 0.0f};
    GLfloat pos2[3] = {0.0f, 0.0f, -1.0f};
    mesh->move(pos);
    mesh2->move(pos1);
    mesh3->move(pos2);
    std::vector<Mesh*> meshes;
    meshes.push_back(mesh); meshes.push_back(mesh2);meshes.push_back(mesh3);

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
                glViewport(0, 0, event.size.width, event.size.height);
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

        // Enable vertex and color arrays
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        // - Top-right (free camera)
        glViewport(window_width/2, window_height/2, window_width/2, window_height/2);
        for(auto* i : meshes)
        {
            glLoadIdentity();
            camera.setDistance(2.5f);
            camera.setRotation(rotation_angle_h, rotation_angle_v); 
            i->render();  
        }

        // - Bottom-right (front view)
        glViewport(window_width/2, 0, window_width/2, window_height/2);
        for(auto* i : meshes)
        {
            glLoadIdentity();
            camera.setDistance(2.5f);
            i->render();  
        }

        // - Top-left (up view)
        glViewport(0, window_height/2, window_width/2, window_height/2);
        for(auto* i : meshes)
        {
            glLoadIdentity();
            camera.setDistance(2.5f);
            camera.setRotation(0, 90); 
            i->render();  
        }


        // - Bottom-left (side view)
        glViewport(0, 0, window_width/2, window_height/2);
        for(auto* i : meshes)
        {
            glLoadIdentity();
            camera.setDistance(2.5f);
            camera.setRotation(90, 0); 
            i->render();  
        }
        
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

    delete cube;
    delete mesh;

    return 0;
}
