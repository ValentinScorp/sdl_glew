#pragma once

class Camera;
class Pathfinder;
class WorldObject;

class AiObstacle {
public:
    AiObstacle();
    virtual ~AiObstacle();
    AiObstacle(const AiObstacle &) = default;
    
    void init(Camera* cam, Pathfinder *pf, WorldObject* wo, std::shared_ptr<Renderer> r);
    void destroy();
    void render();
    void createRenderWall();
   
    Camera* camera = nullptr;
    WorldObject* worldObject = nullptr;
    Pathfinder* pathfinder = nullptr;
    
    glm::fvec2 positionA;
    glm::fvec2 positionB;
    
    RenderLine* renderLine = nullptr;
    std::shared_ptr<Renderer> renderer = nullptr;
};

