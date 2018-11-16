#ifndef CAMERA_H
#define CAMERA_H

#include <includes/glm/glm.hpp>

// enumeration of keyboard inputs
enum keyboard_input {
    key_w,
    key_a,
    key_s,
    key_d
};

// class to abstract a camera object
class Camera {
public:
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    float speed = 0.05f;

    Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up):pos(pos), front(front), up(up) {}

    // returns the view matrix by the camera
    glm::mat4 getViewMatrix() {
        return glm::lookAt(pos, pos + front, up);
    }

    // moves the camera position based on keyboard input
    void processKeyboardInput(keyboard_input key) {
        if (key == key_w) {
            pos += speed * front;
        } else if (key == key_a) {
            // normalize the "right" vector to ensure constant speed movement
            pos -= glm::normalize(glm::cross(front, up)) * speed;
        } else if (key == key_s) {
            pos -= speed * front;
        } else if (key == key_d) {
            pos += glm::normalize(glm::cross(front, up)) * speed;
        }
    }
};

#endif