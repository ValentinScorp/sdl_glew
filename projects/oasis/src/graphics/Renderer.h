#pragma once

#include "../Precompiled.h"

class Camera;
class Configuration;

class Renderer
{
public:
    Renderer();
    virtual ~Renderer();
    
    void init(Configuration *config);
    
    GLuint loadTexture(std::string fileName);
    GLuint createProgram(std::string vertexShader, std::string fragmentShader);
    GLuint getParamFromProgram(GLuint program, std::string paramName);
    GLuint createUbo(GLuint program, std::string paramName, GLsizeiptr size);
    GLuint createVbo(const GLvoid *data, GLsizeiptr size);
    GLuint createVao(GLuint glVbo, GLsizeiptr attribNum1, GLsizeiptr attribNum2, GLsizeiptr attribNum3, GLsizeiptr componentSize);
    
    void updateView(GLuint glUboMatricesInShader);
private:
    GLuint createProgram(GLuint vertexShader, GLuint fragmentShader);
    Camera *camera = nullptr;
};

