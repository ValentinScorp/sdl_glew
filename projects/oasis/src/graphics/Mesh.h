#pragma once

#include "../Precompiled.h"

class Mesh
{
public:
    Mesh();
    virtual ~Mesh();

    void load(std::string fileName);
};

