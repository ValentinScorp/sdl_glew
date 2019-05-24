#pragma once

class RenderQuad;

class Pathfinder {
public:
    Pathfinder();
    virtual ~Pathfinder();
    
    void init(std::shared_ptr<Renderer> renderer, Uint16 width, Uint16 height, Uint16 nodeWidth);
    void render();
    void destroy();
    
    void getPath(glm::fvec3 begin, glm::fvec3 end, std::vector<glm::fvec3> &path);
    size_t getNodeIndex(glm::fvec3 pos);
    glm::fvec3 getNodePosition(size_t index);
    glm::fvec3 getObstaclePosition(glm::fvec3 pos);
    
    void setStaticObstacle(glm::fvec3 position);
    void removeStaticObstacle(glm::fvec3 position);
    bool isObstacle(glm::fvec3 position);
    
    AiMap aiMap;
    
    Uint16 nodeRows = 0;
    Uint16 nodeCols = 0;
    Uint16 nodeWidth = 0;
    
    RenderQuad *renderQuad = nullptr;
    
    std::shared_ptr<Renderer> renderer = nullptr;
};

