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
private:
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    float speed = 0.0f;
    float multiplier = 10.0f;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
public:
    Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up):pos(pos), front(front), up(up) {}

    // returns the world space position of the camera
    glm::vec3 getPos() {
        return pos;
    }

    // returns the view matrix by the camera
    glm::mat4 getViewMatrix() {
        return glm::lookAt(pos, pos + front, up);
    }

    // update time it takes to render the last frame
    void updateDelta(float currentFrame) {
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }

    // moves the camera position based on keyboard input
    void processKeyboardInput(keyboard_input key) {
        speed = multiplier * deltaTime;
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