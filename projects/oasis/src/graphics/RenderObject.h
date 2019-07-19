#pragma once

class Renderer;
class RenderObject;
class IMessage;

class RenderObject {

public:
    RenderObject();
    virtual ~RenderObject();
    
    void init(std::shared_ptr<Renderer> renderer, std::shared_ptr<IniFile> cfg, std::string objectName);
    void render(glm::fmat4 orientationMatrix);
    void destroy();
    
    std::string name;
    
    Mesh *mesh = nullptr;
    GLuint glTexture = 0;
    GLuint glProgram = 0;
    GLuint glModelMatrixUniform = 0;
    GLuint glCameraMatricesUbo = 0;
    GLuint glVbo = 0;
    GLuint glVao = 0;
    
    std::shared_ptr<Renderer> mRenderer = nullptr;
    
    bool initialized = false;
};

