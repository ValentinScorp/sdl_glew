#pragma once

class Camera;
class Pathfinder;

// todo when agents removed messeges sending error occured
class AiAgent : public IMessageRecipient {
public:
    AiAgent();
    virtual ~AiAgent();
    
    void createSelectionBox(float unitWidth, float unitHeight);
    
    void init(Camera* cam, Pathfinder *pf, Mesh *mesh = nullptr);    
    void updateColisions(AiAgent *obstacle);
    void update();
    void avoidObstacle(AiAgent *obstacle);
    void move();
    void setPosition(glm::fvec3 pos);
    void setObstacleOnAiMap();
    void rotateToward(glm::fvec3 direction);
    void adjustRotation(); 
    void makeFinalMatrix();
    bool lineIntersetsCircle(glm::fvec3 p1, glm::fvec3 p2, glm::fvec3 center, float radius);
    
    void onMessage(IMessage *message);
    
    Camera* camera = nullptr;
    Mesh* mesh = nullptr;
    Mesh::Animation* currentAnimation = nullptr;
    size_t currentFrame = 0;
    size_t animCounter = 0;
        
    glm::fvec3 position;
    glm::fvec3 upVector;
    glm::fmat4 rotationMatrix;
    
    glm::fmat4 orientationMatrix;
    
    bool selected = false;
    bool moving = false;
    
    glm::fvec3 movementDirection = { 0.0f, -1.0f, 0.0f };
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

