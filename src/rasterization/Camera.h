#ifndef CAMERA_H
#define CAMERA_H

#include "../../glm/glm.hpp"
#include "../../glm/ext/matrix_transform.hpp"

using namespace glm;

enum Camera_Movement{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera {
private:
    //camera attributes
    vec3 cameraPosition;
    vec3 cameraFront;
    vec3 cameraUp;
    vec3 cameraRight;
    vec3 worldUp;
    //Eurel angles
    float yaw;
    float pitch;
    //camera option
    float cameraSpeed;
    float mouseSensitivity;
    float zoom;
public:
    Camera(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f)) : cameraFront(vec3(0.0f, 0.0f, -1.0f)),
    cameraSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
    {
        cameraPosition = position;
        worldUp = up;
        yaw = YAW;
        pitch = PITCH;
        updateCameraVectors();
    }

    mat4 getViewMatrix() {
        return lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = cameraSpeed * deltaTime;
        if (direction == FORWARD)
            cameraPosition += cameraFront * velocity;
        if (direction == BACKWARD)
            cameraPosition -= cameraFront * velocity;
        if (direction == LEFT)
            cameraPosition -= cameraRight * velocity;
        if (direction == RIGHT)
            cameraPosition += cameraRight * velocity;
    }

    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw   += xoffset;
        pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch) {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        //update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset) {
        zoom -= (float)yoffset;
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;
    }

    void updateCameraVectors() {
        vec3 direction;

        direction.x = cos(radians(yaw)) * cos(radians(pitch));
        direction.y = sin(radians(pitch));
        direction.z = sin(radians(yaw)) * cos(radians(pitch));
        cameraFront = normalize(direction);

        cameraRight = normalize(cross(cameraFront, worldUp));
        cameraUp    = normalize(cross(cameraRight, cameraFront));
    }

    float getFov() const {return zoom;}

    vec3 getPos() const {return cameraPosition; }
};

#endif //CAMERA_H
