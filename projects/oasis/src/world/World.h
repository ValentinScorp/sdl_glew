#pragma once

class Renderer;
class AiContainer;
class IniFile;
class RenderObject;
class WorldObject;

class World : public IMessageRecipient {
public:
    World();
    virtual ~World();
    
    void init(std::shared_ptr<Renderer> renderer, std::shared_ptr<AiContainer> aiContainer);
    void destroy();
    void update();
    void render();
    
    void addObject(std::string objectName, glm::fvec3 position);
    void addObstacle(glm::fvec3 position);
    
    virtual void onMessage(IMessage *message);
    
    std::shared_ptr<Terrain> terrain = nullptr;
    
private:
    std::shared_ptr<Renderer> renderer = nullptr;
    std::shared_ptr<AiContainer> aiContainer = nullptr;
    std::shared_ptr<IniFile> worldObjectsDescription = nullptr;
    
    std::vector<std::shared_ptr<WorldObject>> worldObjects;
    
    std::string currentObjectSelected;
};

