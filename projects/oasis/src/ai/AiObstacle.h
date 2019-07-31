#pragma once

class Camera;
class Pathfinder;
class WorldObject;

class AiObstacle {
public:
    AiObstacle();
    ~AiObstacle();
    AiObstacle(const AiObstacle &) = default;
    
    void init(Camera* cam, Pathfinder *pf, std::shared_ptr<Renderer> r, glm::fvec2 pos, float rad);
    void destroy();
    void render();
    void create(glm::fvec2 pos);
   
    Camera* camera = nullptr;
    WorldObject* worldObject = nullptr;
    Pathfinder* pathfinder = nullptr;
    
    glm::fvec2 position;
    float radius;
        
    std::shared_ptr<RenderObject> renderObject = nullptr;
    std::shared_ptr<Renderer> renderer = nullptr;
};

