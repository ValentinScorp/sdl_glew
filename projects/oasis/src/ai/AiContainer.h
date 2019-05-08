#pragma once

class AiContainer
{
public:
    AiContainer();
    virtual ~AiContainer();
    
    void init(Camera *cam);
    void destroy();
    void update();
    
    size_t createAgent();
    AiAgent* getAgent(size_t id);
    void destroyAgent(size_t id);
    
private:
    std::vector<AiAgent*> agents;
    Camera *camera = nullptr;
};

