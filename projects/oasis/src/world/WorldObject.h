#pragma once

class Renderer;
class AiAgent;
class Mesh::Animation;
class RenderObject;

class WorldObject {

public:
    WorldObject();
    virtual ~WorldObject();
    
    bool init(std::string name, 
              std::shared_ptr<Renderer> renderer, 
              std::shared_ptr<AiContainer> aiContainer, 
              std::shared_ptr<IniFile> iniFile, 
              glm::fvec3 position);
    void update();
    void render();
    void destroy();
    void startAnimation(std::string animName);
    void stopAnimation();
    
    std::string name;
    std::shared_ptr<RenderObject> renderObject = nullptr;
    std::shared_ptr<UnitSelection> unitSelection = nullptr;
    //std::shared_ptr<Renderer> renderer = nullptr;
    AiAgent *aiAgent = nullptr;
    
    // animation
    Mesh::Animation *currentAnimation = nullptr;
    size_t currentFrame = 0;
    size_t animCounter = 0;
};

