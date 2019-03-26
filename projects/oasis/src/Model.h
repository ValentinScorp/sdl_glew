#pragma once

#include "Precompiled.h"

class Vertex {
public:
    Vertex();
    Vertex(Vertex const& v);
    Vertex(glm::vec4 p, glm::vec2 t);
    Vertex(float x, float y, float z, float w, float s, float t);
    ~Vertex();
    Vertex& operator=(Vertex const& other) {
        if (this != &other) {
            pos = other.pos;
            tex = other.tex;
        }
        return *this;
    }
    void print() {
        SDL_Log("%f %f %f %f   %f %f \n", pos.x, pos.y, pos.z, pos.w, tex.s, tex.t);
    }
    
    glm::fvec4 pos;
    glm::fvec2 tex;
};

class Model {
public:
    Model();
    virtual ~Model();
    void print() {
        for (auto v: vertexes) {
            v.print();
        }
    }
    
    void loadMesh(std::string fileName);
    GLsizeiptr getVertexBufferSize();
    GLvoid* getVertexBufferData();
    GLsizei getVertexLen();
    
    GLuint glBufferId = 0;
    std::vector<Vertex> vertexes;
    std::string name;
    
    float *vertexArray = nullptr;
};

