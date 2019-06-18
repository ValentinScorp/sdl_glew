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
    void placeLink(glm::fvec2 pos);
   
    Camera* camera = nullptr;
    WorldObject* worldObject = nullptr;
    Pathfinder* pathfinder = nullptr;
    
    std::vector<glm::fvec2> positions;
};

