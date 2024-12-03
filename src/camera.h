#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#define VIEWPORT_MAX_ZOOM 1.5
#define VIEWPORT_MIN_ZOOM 5.0
#define SIDEVIEW_MAX_ZOOM 0.5
#define SIDEVIEW_MIN_ZOOM 3.0


class Camera
{
    public:
    float rotation_angle_h = 0;
    float rotation_angle_v = 0;
    double zoom_distance_viewport = 2;
    double zoom_distance_front = 0.5;
    double zoom_distance_up = 0.5;
    double zoom_distance_side = 0.5;


    void handleInput() {
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

    
    void handleEvent(const sf::Event &event, const sf::Window &window){
        if(event.type == sf::Event::MouseWheelMoved) {
            // Zoom in/out
            double window_width = window.getSize().x;
            double window_height = window.getSize().y;

            if(sf::Mouse::getPosition(window).y < window_height/2){
                if(sf::Mouse::getPosition(window).x < window_width/2){
                    zoom_distance_up += -event.mouseWheel.delta*0.1;
                    zoom_distance_up = std::clamp(zoom_distance_up, SIDEVIEW_MAX_ZOOM, SIDEVIEW_MIN_ZOOM);
                }
                else{
                    zoom_distance_viewport += -event.mouseWheel.delta*0.1;
                    zoom_distance_viewport = std::clamp(zoom_distance_viewport, VIEWPORT_MAX_ZOOM, VIEWPORT_MIN_ZOOM);
                }
            }
            else{
                if(sf::Mouse::getPosition(window).x > window_width/2){
                    zoom_distance_front += -event.mouseWheel.delta*0.1;
                    zoom_distance_front = std::clamp(zoom_distance_front, SIDEVIEW_MAX_ZOOM, VIEWPORT_MIN_ZOOM);
                }
                else{
                    zoom_distance_side += -event.mouseWheel.delta*0.1;
                    zoom_distance_side = std::clamp(zoom_distance_side, SIDEVIEW_MAX_ZOOM, VIEWPORT_MIN_ZOOM);
                }
            }
        }
    }


    void setDistance(float distance){
        glTranslatef(0.0f, 0.0f, -distance);  // Move camera backwards
    }


    void setRotation(float rotation_angle_h, float rotation_angle_v) {
        glRotatef(rotation_angle_v, 1.0f, 0.0f, 0.0f);  // Rotate camera vertically
        glRotatef(rotation_angle_h, 0.0f, 1.0f, 0.0f);  // Rotate camera horizontally
    }

    void setRotation() {
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