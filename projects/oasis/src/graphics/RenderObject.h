#pragma once

class Renderer;
class Mesh;
class IMessage;

class RenderObject : public IMessageRecipient {

public:
    RenderObject();
    virtual ~RenderObject();
    
    virtual void onMessage(IMessage *message);
    
    void init(std::shared_ptr<Renderer> renderer, Configuration *cfg, std::string name);
    void update(float time);
    void render();
    void destroy();

    void setPosition(glm::fvec3 pos);
    void rotateToward(glm::fvec3 direction);
    void adjustRotation();
    
    void makeFinalMatrix();
    
    std::string name;
    
    Mesh *mesh = nullptr;
    GLuint glTexture = 0;
    GLuint glProgram = 0;
    GLuint glModelMatrixUniform = 0;
    GLuint glCameraMatricesUbo = 0;
    GLuint glVbo = 0;
    GLuint glVao = 0;
    
    glm::fmat4 orientationMatrix;
    
    std::shared_ptr<Renderer> mRenderer = nullptr;
    UnitSelection *unitSelection = nullptr;
    glm::fvec3 position = { 0.0f, 0.0f, 0.0f };
    glm::fvec3 upVector = { 0.0f, 0.0f, 0.0f };
    //glm::fvec3 rotation = { 0.0f, 0.0f, 0.0f };
    glm::fmat4 rotationMatrix;
    
    aux::surface selectionBox;
    bool selected = false;
    bool moving = false;
    glm::fvec3 movementDirection = { 0.0f, 0.0f, 0.0f };
    float movementSpeed = 0.5;
    glm::fvec3 movementTarget = { 0.0f, 0.0f, 0.0f };
    
    bool selectable = false;
};

