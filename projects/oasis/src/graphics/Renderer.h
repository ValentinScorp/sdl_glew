#pragma once

class Camera;
class IniFile;
class Terrain;

class Renderer : public std::enable_shared_from_this<Renderer>
{
public:
    Renderer();
    virtual ~Renderer();
    
    void init(std::shared_ptr<IniFile> config);
    void loadObjects(std::string iniFile);
    void unloadObjects();
    std::shared_ptr<RenderObject> getRenderObject(std::string name);
    GLuint createTexture(GLsizei width, GLsizei height, GLenum internalFormat, GLenum format, GLvoid *data);
    GLuint loadTexture(std::string fileName, bool flipVertical = false);
    void unloadTexture(GLuint texture);
    
    GLuint createProgram(std::string vertexShader, std::string fragmentShader);
    void destroyProgram(GLuint program);
    
    GLuint getParamFromProgram(GLuint program, std::string paramName);
    GLuint createUbo(GLuint program, std::string paramName, GLsizeiptr size, GLuint &bindingPoint);
    GLuint createVbo(const GLvoid *data, GLsizeiptr size);
    void destroyBuffer(std::string bufferName, GLuint buffer);
    
    GLuint createVao(GLuint glVbo, GLsizeiptr attribNum1, GLsizeiptr attribNum2, GLsizeiptr attribNum3, GLsizeiptr attribNum4, GLsizeiptr attribNum5, GLsizeiptr componentSize);
    void destroyVertexArray(GLuint vao);
    void sendSubDataToVbo(GLuint glVbo, GLintptr offset, void* data, GLsizeiptr size);
    void sendTexture(Uint16 texNum, GLuint glTexture, GLuint glProgram, std::string textureNameInProgram);
    void unbindTexture(Uint16 texNum);
    
    void destroyVertexArrays(GLuint vertexArray);
    void updateView(GLuint glUboMatricesInShader);
    
    void showError(std::string descr);
    
    Camera *camera = nullptr;
    Terrain *terrain = nullptr;
    
    
private:
    GLuint createProgram(GLuint vertexShader, GLuint fragmentShader);
    GLuint uniformBlockBindingCounter = 0;
    
    std::vector<std::shared_ptr<RenderObject>> renderObjects;
};

