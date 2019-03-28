#pragma once

#include "Precompiled.h"

class Configuration
{
public:
    Configuration(std::string fileName);
    virtual ~Configuration();
    
    void convertStrToVec3(std::string paramName, std::string paramValue, glm::vec3 *vec);
    void convertStrToFloat(std::string paramName, std::string paramValue, float *var);
    
    float getScreenAspectRatio();
    
    float screenResolutionWidth = 640;
    float screenResolutionHeight = 480;    
    
    glm::vec3 camPosition = {0.0f, 0.0f, 0.0f};
    glm::vec3 camUpVec = {0.0f, 0.0f, 0.0f};
    glm::vec3 camLookAt = {0.0f, 0.0f, 0.0f};
    float camNearPlane = 0;
    float camFarPlane = 0;
};

