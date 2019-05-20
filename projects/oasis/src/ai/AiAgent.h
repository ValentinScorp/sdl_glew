#pragma once

class Camera;
class Pathfinder;

class AiAgent: public IMessageRecipient {
public:
    AiAgent();
    virtual ~AiAgent();
    
    void createSelectionBox(float unitWidth, float unitHeight);
    
    void init(Camera* cam, Pathfinder *pf);
    void update(AiAgent *obstacle);
    void avoidObstacle(AiAgent *obstacle);
    void move();
    void setPosition(glm::fvec3 pos);
    void setObstacleOnAiMap();
    void rotateToward(glm::fvec3 direction);
    void adjustRotation(); 
    void makeFinalMatrix();
    bool lineIntersetsCircle(glm::fvec3 p1, glm::fvec3 p2, glm::fvec3 center, float radius);
    
    virtual void onMessage(IMessage *message);
    
    Camera* camera = nullptr;
        
    glm::fvec3 position;
    glm::fvec3 upVector;
    glm::fmat4 rotationMatrix;
    
    glm::fmat4 orientationMatrix;
    
    bool selected = false;
    bool moving = false;
    
    glm::fvec3 movementDirection = { 0.0f, 0.0f, 0.0f };
    float movementSpeed = 0.3;
    glm::fvec3 movementTarget = { 0.0f, 0.0f, 0.0f };
    
    bool selectable = false;
    bool dynamic = false;
    float collisionRadius = 3;
    
    aux::surface selectionBox;
    Pathfinder* pathfinder = nullptr;
    std::vector<glm::fvec3> movementPath;
    size_t currentPath = 0;
};
