#pragma once

#include "Precompiled.h"

class Configuration
{
public:
    Configuration(std::string fileName);
    virtual ~Configuration();
        
    float getScreenAspectRatio();
    void convertStrToVec3(std::string paramName, std::string paramValue, glm::vec3 *vec);
    
    float screenResolutionWidth = 640;
    float screenResolutionHeight = 480;
    
    glm::vec3 camPosition = {0.0f, 0.0f, 1.0f};
};

