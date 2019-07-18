#pragma once

class Camera;
class Pathfinder;
class WorldObject;

typedef std::vector<glm::fvec3> Path;

// todo when agents removed messeges sending error occured
class AiAgent {
public:
    AiAgent();
    virtual ~AiAgent();
    AiAgent(const AiAgent &) = default;
    
    void createSelectionBox(float unitWidth, float unitHeight);
    
    void init(Camera* cam, Pathfinder *pf, WorldObject* wo, std::shared_ptr<Renderer> r);
    void updateColisions(AiAgent *obstacle);
    void update(std::vector<AiAgent> &agents);
    void render();
    bool lineIntersetsCircle(glm::fvec3 p1, glm::fvec3 p2, glm::fvec3 center, float radius);
    void avoidObstacle(AiAgent *obstacle);
    void move(std::vector<AiAgent> &agents);
    void setPosition(glm::fvec2 pos);
    void setObstacleOnAiMap();
    void tryMove(glm::fvec2 newPos);
    void startMove(glm::fvec3 destination);
    void createPath(glm::fvec2 destination);
    glm::fvec2 calcSteerForce(glm::fvec2 pos, glm::fvec2 dest, glm::fvec2 vel);
    glm::fvec2 calcSeparationForce(std::vector<AiAgent> &agents, glm::fvec2 pos, glm::fvec2 vel);
    void setCollisionRadius(float radius);
    void calcNextPosition(std::vector<AiAgent> &agents, glm::fvec2 &nextPos);
    void destroyRenderLine();
    
    void onMessage(IMessage *message);
    
    Camera* camera = nullptr;
    WorldObject* worldObject = nullptr;
      
    glm::fvec2 position;
   
    glm::fvec2 location;
    glm::fvec2 velocity;
    glm::fvec2 acceleration;
    float MAX_SPEED = 0.2;
    float MAX_STEER_FORCE = 0.1;
    float MAX_SEPAR_FORCE = 0.1;
    
    bool selected = false;
    bool moving = false;
    
    glm::fvec2 movementDirection = { 0.0f, -1.0f };
    float movementSpeed = 0.3;
    glm::fvec2 movementTarget = { 0.0f, 0.0f };
    
    bool lockSelectable = false;
    bool selectable = false;
    bool dynamic = false;
    float collisionRadius = 3;
    
    aux::surface selectionBox;
    Pathfinder* pathfinder = nullptr;
    std::vector<glm::fvec2> movementPath;
    size_t currentPath = 0;
    
    RenderLine *renderLine = nullptr;
    std::shared_ptr<Renderer> renderer = nullptr;
};

