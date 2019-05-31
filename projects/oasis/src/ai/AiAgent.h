#pragma once

class Camera;
class Pathfinder;
class WorldObject;

typedef std::vector<glm::fvec3> Path;

// todo when agents removed messeges sending error occured
class AiAgent : public IMessageRecipient {
public:
    AiAgent();
    virtual ~AiAgent();
    
    void createSelectionBox(float unitWidth, float unitHeight);
    
    void init(Camera* cam, Pathfinder *pf, WorldObject* wo);
    void updateColisions(AiAgent *obstacle);
    void update();
    bool lineIntersetsCircle(glm::fvec3 p1, glm::fvec3 p2, glm::fvec3 center, float radius);
    void avoidObstacle(AiAgent *obstacle);
    void move();
    void setPosition(glm::fvec2 pos);
    void setObstacleOnAiMap();
    void tryMove(glm::fvec2 newPos);
    void startMove(glm::fvec3 destination);
    void createPath(glm::fvec2 destination);
    
    void onMessage(IMessage *message);
    
    Camera* camera = nullptr;
    WorldObject* worldObject = nullptr;
      
    glm::fvec2 position;
    //glm::fvec3 position;
    //glm::fvec3 upVector;
    //glm::fmat4 rotationMatrix;
    
   // glm::fmat4 orientationMatrix;
    
    glm::fvec2 location;
    glm::fvec2 velocity;
    glm::fvec2 acceleration;
    float MAX_SPEED = 4;
    float MAX_FORCE = 0.01;
    
    
    bool selected = false;
    bool moving = false;
    
    glm::fvec2 movementDirection = { 0.0f, -1.0f};
    float movementSpeed = 0.3;
    glm::fvec2 movementTarget = { 0.0f, 0.0f };
    
    bool selectable = false;
    bool dynamic = false;
    float collisionRadius = 3;
    
    aux::surface selectionBox;
    Pathfinder* pathfinder = nullptr;
    std::vector<glm::fvec2> movementPath;
    size_t currentPath = 0;
};

