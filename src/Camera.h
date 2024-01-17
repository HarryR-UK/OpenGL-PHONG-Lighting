#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef glm::vec3 vec3;
typedef glm::mat4 mat4;

// HEAVILY inspired by the camera class example in learnopengl.com

enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

const float DEFAULT_YAW = 90.0f;
const float DEFAULT_PITCH = 0.0f;
const float DEFAULT_SPEED = 2.5f;
const float DEFAULT_SENSITIVITY = 0.1f;
const float DEFAULT_FOV = 70.0f;
const float DEFAULT_SPRINT_SPEED = 2.0f;

class Camera
{
    private:

    private:
        void updateCameraVectors()
        {
            this->front = glm::normalize(
                vec3(
                    cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                    sin(glm::radians(pitch)),
                    sin(glm::radians(yaw)) * cos(glm::radians(pitch))
                )
            );

            this->forwards = glm::normalize(
                vec3(
                    cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                    0.0f,
                    sin(glm::radians(yaw)) * cos(glm::radians(pitch))
                )
            );

            this->right = glm::normalize(glm::cross(this->front, this->worldUp));
            this->up = glm::normalize(glm::cross(this->right, this->front));

        }
    public:
        vec3 position;
        vec3 front;
        vec3 up;
        vec3 right;
        vec3 worldUp;
        vec3 forwards;

        float yaw;
        float pitch;

        float moveSpeed;
        float sprintSpeed;
        float mouseSensitivity;
        float fov;

        bool isSprinting;

    public:
        Camera(vec3 posL = vec3(0.0f, 0.0f, 0.0f), vec3 upL = vec3(0.0f, 1.0f, 0.0f), float yawL = DEFAULT_YAW, float pitchL = DEFAULT_PITCH)
        : front  (vec3(0.0f, 0.0f, -1.0f))
        , moveSpeed (DEFAULT_SPEED)
        , mouseSensitivity (DEFAULT_SENSITIVITY)
        , fov (DEFAULT_FOV)
        , sprintSpeed (DEFAULT_SPRINT_SPEED)
        {
            this->position = posL;
            this->worldUp = upL;
            this->yaw = yawL;
            this->pitch = pitchL;
            updateCameraVectors();
        }

        void setSprint(bool value) { this->isSprinting = value; }

        mat4 getViewMatrix()
        {
             return glm::lookAt(position, position + front, up);
        }

        void processKeyboardInput(CameraMovement direction, float deltaTime)
        {
            vec3 moveDir = vec3(0.0f);
            float speed = moveSpeed * deltaTime;
            if(isSprinting) 
                speed *= sprintSpeed;
            
            switch (direction)
            {
                case FORWARD:
                    moveDir += forwards;
                    //position += forwards * speed;
                    break;
                case BACKWARD:
                    moveDir -= forwards;
                    //position -= forwards * speed;
                    break;
                case LEFT:
                    moveDir -= right;
                    //position -= right * speed;
                    break;
                case RIGHT:
                    moveDir += right;
                    //position += right * speed;
                    break;
                case UP:
                    moveDir += worldUp;
                    //position += worldUp * speed;
                    break;
                case DOWN:
                    moveDir -= worldUp;
                    //position -= worldUp * speed;
                    break;
                default:
                    break;

            }
            position += glm::normalize(moveDir) * speed;

        }

        void processMouseMovement(float xOffset, float yOffset)
        {
            xOffset *= mouseSensitivity;
            yOffset *= mouseSensitivity;

            yaw += xOffset;
            pitch += yOffset;

            if(pitch > 89.0f)
                pitch = 89.0f;
            if(pitch < -89.0f)
                pitch = -89.0f;

            updateCameraVectors();
        }

};


#endif // !CAMERA_H

