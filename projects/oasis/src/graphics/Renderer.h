#pragma once

class Camera;
class Configuration;
class Terrain;

class Renderer : public std::enable_shared_from_this<Renderer>
{
public:
    Renderer();
    virtual ~Renderer();
    
    void init(Configuration *config);
    
    GLuint createTexture(GLsizei width, GLsizei height, GLenum internalFormat, GLenum format, GLvoid *data);
    GLuint loadTexture(std::string fileName, bool flipVertical = false);
    void unloadTexture(GLuint texture);
    
    GLuint createProgram(std::string vertexShader, std::string fragmentShader);
    void destroyProgram(GLuint program);
    
    GLuint getParamFromProgram(GLuint program, std::string paramName);
    GLuint createUbo(GLuint program, std::string paramName, GLsizeiptr size);
    GLuint createVbo(const GLvoid *data, GLsizeiptr size);
    void destroyBuffer(GLuint buffer);
    
    GLuint createVao(GLuint glVbo, GLsizeiptr attribNum1, GLsizeiptr attribNum2, GLsizeiptr attribNum3, GLsizeiptr attribNum4, GLsizeiptr attribNum5, GLsizeiptr componentSize);
    void sendSubDataToVbo(GLuint glVbo, GLintptr offset, void* data, GLsizeiptr size);
    void sendTexture(Uint16 texNum, GLuint glTexture, GLuint glProgram, std::string textureNameInProgram);
    void undindTexture(Uint16 texNum);
    
    void destroyVertexArrays(GLuint vertexArray);
    
    
    void updateView(GLuint glUboMatricesInShader);
    
    
    Camera *camera = nullptr;
    Terrain *terrain = nullptr;
private:
    GLuint createProgram(GLuint vertexShader, GLuint fragmentShader);
    GLuint uniformBlockBindingCounter = 0;
};

