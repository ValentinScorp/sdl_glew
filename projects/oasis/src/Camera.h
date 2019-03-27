#pragma once

#include "Precompiled.h"

class Configuration;
class Camera
{
public:
    Camera();
    Camera(float ar, glm::vec3 position, glm::vec3 upVector, glm::vec3 lookAt);
    virtual ~Camera();
    
    void init(Configuration *cfg);
    
    glm::mat4 getViewMatrix();
    glm::mat4 getCamMatrix();
    
    void moveUp();
    void moveDown();

    glm::vec3 position;
    glm::vec3 upVector;
    glm::vec3 lookTarget;
    
    float aspectRatio = 1;
    float moveSpeed = 0.01;
};


