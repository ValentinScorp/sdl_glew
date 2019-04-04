#pragma once

class Configuration;

class Camera {
public:
    Camera();
    virtual ~Camera();
    
    void init(Configuration *cfg);
    
    glm::mat4 getViewMatrix();
    glm::mat4 getCamMatrix();
    
    void moveUp();
    void moveDown();

    glm::vec3 position = {0.0f, 0.0f, 0.0f};;
    glm::vec3 upVector = {0.0f, 0.0f, 0.0f};;
    glm::vec3 lookTarget = {0.0f, 0.0f, 0.0f};;
    
    float aspectRatio = 0.0f;
    float moveSpeed = 0.0f;
};


