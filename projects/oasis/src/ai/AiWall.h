#pragma once

class Camera;
class Pathfinder;
class WorldObject;

class AiWall {
public:
    AiWall();
    ~AiWall();
    AiWall(const AiWall &) = default;
    
    void init(Camera* cam, Pathfinder *pf, std::shared_ptr<Renderer> r);
    void destroy();
    void render();
    void createRenderWall();
   
    Camera* camera = nullptr;
    Pathfinder* pathfinder = nullptr;
    
    glm::fvec2 positionA;
    glm::fvec2 positionB;
    
    RenderLine* renderLine = nullptr;
    std::shared_ptr<Renderer> renderer = nullptr;
};

