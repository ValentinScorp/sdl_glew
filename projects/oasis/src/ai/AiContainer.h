#pragma once

class AiContainer : public IMessageRecipient
{
public:
    AiContainer();
    virtual ~AiContainer();
    
    void init(std::shared_ptr<Renderer> renderer, Camera *cam, std::shared_ptr<IniFile> cfg);
    void destroy();
    void update();
    void render();
    
    size_t createAgent(WorldObject *wo);
    AiWall* createWall();
    AiObstacle* createObstacle(glm::fvec2 pos, float radius);
    
    void calcBlockNodes(AiWall *obstacle);
    
    AiAgent* getAgent(size_t id);
    void destroyAgent(size_t id);
    
    void onMessage(IMessage *message);
    
    Pathfinder pathfinder;
    
private:
    std::vector<AiAgent*> agentsPtr;
    std::vector<AiWall*> walls;
    std::vector<AiObstacle*> obstacles;
    Camera *camera = nullptr;
    
    std::shared_ptr<Renderer> renderer = nullptr;
};

