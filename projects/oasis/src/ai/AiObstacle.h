#pragma once

class Camera;
class Pathfinder;
class WorldObject;

class AiObstacle {
public:
    AiObstacle();
    virtual ~AiObstacle();
    AiObstacle(const AiObstacle &) = default;
    
    void init(Camera* cam, Pathfinder *pf, WorldObject* wo);
   
    Camera* camera = nullptr;
    WorldObject* worldObject = nullptr;
    Pathfinder* pathfinder = nullptr;
    
    glm::fvec2 position;
};

