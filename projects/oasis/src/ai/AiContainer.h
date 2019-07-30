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
    AiObstacle* createObstacle(WorldObject *wo);
    
    void calcBlockNodes(AiObstacle *obstacle);
    
    AiAgent* getAgent(size_t id);
    void destroyAgent(size_t id);
    
    void onMessage(IMessage *message);
    
    Pathfinder pathfinder;
    
private:
    std::vector<AiAgent*> agentsPtr;
    std::vector<AiObstacle*> obstacles;
    Camera *camera = nullptr;
    
    std::shared_ptr<Renderer> renderer = nullptr;
};

