#pragma once

class Renderer;
class Mesh;
class IMessage;

class RenderObject : public IMessageRecipient {
public:
    RenderObject();
    virtual ~RenderObject();
    
    virtual void onMessage(IMessage *message);
    
    void init(Renderer *renderer, Configuration *cfg, std::string name);
    void update(float time);
    void render();

    void setOrientation(glm::fvec3 p, glm::fvec3 r);
    
    std::string name;
    
    Mesh *mesh = nullptr;
    GLuint glTexture = 0;
    GLuint glProgram = 0;
    GLuint glModelMatrixUniform = 0;
    GLuint glCameraMatricesUbo = 0;
    GLuint glVbo = 0;
    GLuint glVao = 0;
    
    glm::fmat4 orientationMatrix;
};

