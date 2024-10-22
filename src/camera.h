#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>


class Camera
{
    public:
    
    void handleInput(float &rotation_angle_h, float &rotation_angle_v) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
            rotation_angle_h --;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            rotation_angle_h ++;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            rotation_angle_v ++;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
            rotation_angle_v --;
        }
        rotation_angle_v = std::clamp(rotation_angle_v, -90.0f, 90.0f);
    }


    void setDistance(float distance){
        glTranslatef(0.0f, 0.0f, -distance);  // Move camera backwards
    }


    void setRotation(float rotation_angle_h, float rotation_angle_v) {
        glRotatef(rotation_angle_v, 1.0f, 0.0f, 0.0f);  // Rotate camera vertically
        glRotatef(rotation_angle_h, 0.0f, 1.0f, 0.0f);  // Rotate camera horizontally
    }


    void setFrustum(float fov, float aspect, float near, float far) {
        float top = near * tan(fov / 2.0f);
        float bottom = -top;
        float right = top * aspect;
        float left = -right;
        // Set the projection matrix using glFrustum
        glFrustum(left, right, bottom, top, near, far);
    }
};


//Camera::Camera(){
//}
//
//Camera::~Camera(){
//}