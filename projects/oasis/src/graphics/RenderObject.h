#pragma once

class Renderer;
class Mesh;
class IMessage;

class RenderObject {

public:
    RenderObject();
    virtual ~RenderObject();
    
    void init(std::shared_ptr<Renderer> renderer, Configuration *cfg, std::string name, AiAgent *agent);
    void update(float time);
    void render();
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
    UnitSelection *unitSelection = nullptr;
    
    AiAgent *aiAgent = nullptr;
};

