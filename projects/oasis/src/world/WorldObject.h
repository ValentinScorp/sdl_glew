#pragma once

class Renderer;
class World;
class AiAgent;
class RenderObject;

class WorldObject : private IMessageRecipient {

public:
    WorldObject();
    virtual ~WorldObject();
    
    bool init(std::string name, 
              World *w,
              std::shared_ptr<Renderer> renderer, 
              std::shared_ptr<AiContainer> aiContainer, 
              std::shared_ptr<IniFile> iniFile, 
              glm::fvec3 position);
    void update();
    void render();
    void destroy();
    void beginAnimation(std::string animName);
    void stopAnimation();
    glm::fmat4 getRotationMatrix(glm::fvec3 direction, glm::fvec3 upVector);
    void adjustRotation(glm::fmat4 &rotationMatrix);
    glm::fmat4 makeFinalMatrix(glm::fvec3 position, glm::fmat4 rotationMatrix);
    
    void onMessage(IMessage *message);
    
    std::string name;
    std::shared_ptr<RenderObject> renderObject = nullptr;
    std::shared_ptr<UnitSelection> unitSelection = nullptr;
    //std::shared_ptr<Renderer> renderer = nullptr;
    //AiAgent *aiAgent = nullptr;
    std::shared_ptr<AiContainer> aiContainer = nullptr;
    size_t aiAgentId = 0;
    World *world = nullptr;
    // animation
    Mesh::Animation *currentAnimation = nullptr;
    size_t currentFrame = 0;
    size_t animCounter = 0;
};

