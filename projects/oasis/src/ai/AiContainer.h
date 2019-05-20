#pragma once

class AiContainer
{
public:
    AiContainer();
    virtual ~AiContainer();
    
    void init(std::shared_ptr<Renderer> renderer, Camera *cam, Configuration *cfg);
    void destroy();
    void update();
    void render();
    
    size_t createAgent();
    AiAgent* getAgent(size_t id);
    void destroyAgent(size_t id);
    
private:
    std::vector<AiAgent*> agents;
    Camera *camera = nullptr;
    
    Pathfinder pathfinder;
};
