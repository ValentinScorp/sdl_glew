#pragma once

class Camera;
class Configuration;

class Renderer
{
public:
    Renderer();
    virtual ~Renderer();
    
    void init(Configuration *config);
    
    GLuint loadTexture(std::string fileName);
    void unloadTexture(GLuint texture);
    
    GLuint createProgram(std::string vertexShader, std::string fragmentShader);
    void destroyProgram(GLuint program);
    
    GLuint getParamFromProgram(GLuint program, std::string paramName);
    GLuint createUbo(GLuint program, std::string paramName, GLsizeiptr size);
    GLuint createVbo(const GLvoid *data, GLsizeiptr size);
    void destroyBuffer(GLuint buffer);
    
    GLuint createVao(GLuint glVbo, GLsizeiptr attribNum1, GLsizeiptr attribNum2, GLsizeiptr attribNum3, GLsizeiptr attribNum4, GLsizeiptr componentSize);
    void destroyVertexArrays(GLuint vertexArray);
    
    void updateView(GLuint glUboMatricesInShader);
private:
    GLuint createProgram(GLuint vertexShader, GLuint fragmentShader);
    Camera *camera = nullptr;
};

