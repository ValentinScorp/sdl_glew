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

class SmaVertex {
public:
    SmaVertex();
    SmaVertex(SmaVertex const& v);
    SmaVertex(glm::vec3 p, glm::vec3 n, glm::vec2 t);
    SmaVertex(float x, float y, float z, float nx, float ny, float nz, float s, float t);
    ~SmaVertex();
    SmaVertex& operator=(SmaVertex const& other) {
        if (this != &other) {
            pos = other.pos;
            nor = other.nor;
            tex = other.tex;
        }
        return *this;
    }
    void print() {
        SDL_Log("%f %f %f   %f %f %f   %f %f \n", pos.x, pos.y, pos.z, nor.x, nor.y, nor.z, tex.s, tex.t);
    }
    
    glm::fvec3 pos;
    glm::fvec3 nor;
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
    void loadSmaMesh(std::string fileName);
    
    GLsizeiptr getVertexBufferSize();
    GLvoid* getVertexBufferData();
    GLsizei getVertexLen();
        
    std::vector<Vertex> vertexes;
    std::vector<SmaVertex> smaVerts;
    std::string name;
};

