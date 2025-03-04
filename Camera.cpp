﻿#include "Camera.hpp"

namespace gps {

    float yaw, pitch;
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraUpDirection = cameraUp;
        cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
        cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, cameraUpDirection));
        yaw = glm::degrees(atan2(cameraFrontDirection.z, cameraFrontDirection.x)); 
        pitch = glm::degrees(asin(cameraFrontDirection.y)); 
    }
    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(cameraPosition, cameraPosition + cameraFrontDirection, cameraUpDirection);
    }
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        if (direction == MOVE_FORWARD) {
            cameraPosition += cameraFrontDirection * speed;
        }
        if (direction == MOVE_BACKWARD) {
            cameraPosition -= cameraFrontDirection * speed;
        }
        if (direction == MOVE_RIGHT) {
            cameraPosition += cameraRightDirection * speed;
        }
        if (direction == MOVE_LEFT) {
            cameraPosition -= cameraRightDirection * speed;
        }
        if (direction == MOVE_UP) {
            cameraPosition += cameraUpDirection * speed;
        }
        if (direction == MOVE_DOWN) {
            cameraPosition -= cameraUpDirection * speed;
        }
    }
    void Camera::rotate(float pitchChange, float yawChange) {
        yaw += yawChange;
        pitch += pitchChange;
        if (pitch > 89.0f) {
            pitch = 89.0f;
        }
        if (pitch < -89.0f) {
            pitch = -89.0f;
        }
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFrontDirection = glm::normalize(front);
        cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
        cameraUpDirection = glm::normalize(glm::cross(cameraRightDirection, cameraFrontDirection));
    }
    glm::vec3 Camera::getPosition() {
        return cameraPosition;
    }
    glm::vec3 Camera::getTarget() {
        return cameraTarget;
    }
    void Camera::setPosition(glm::vec3 newPosition) {
        cameraPosition = newPosition;
        cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
        cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, cameraUpDirection));
    }

    void Camera::setTarget(glm::vec3 newTarget) {
        cameraTarget = newTarget;
        cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
        cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, cameraUpDirection));
    }
}
