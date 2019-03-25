#pragma once

#include "Precompiled.h"

class Model
{
public:
    Model();
    virtual ~Model();
    
    void loadMesh(std::string fileName);
    
    GLuint arrayBufferId = NULL;
};

