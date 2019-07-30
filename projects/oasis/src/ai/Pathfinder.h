#pragma once

class RenderQuad;

class Pathfinder {
public:
    Pathfinder();
    virtual ~Pathfinder();
    
    void init(std::shared_ptr<Renderer> renderer, Uint16 width, Uint16 height, Uint16 nodeWidth);
    void render();
    void destroy();
    
    void getPath(glm::fvec2 begin, glm::fvec2 end, std::vector<glm::fvec2> &path);
    size_t getNodeIndex(glm::fvec2 pos);
    glm::fvec2 getNodePosition(size_t index);
    glm::fvec2 getObstaclePosition(glm::fvec2 pos);
    
    void setStaticObstacle(glm::fvec2 position);
    void removeStaticObstacle(glm::fvec2 position);
    
    void setDynamicObstacle(glm::fvec2 position);
    void removeDynamicObstacle(glm::fvec2 position);
    
    bool isObstacle(glm::fvec2 position);
    
    void placeBlocks(glm::fvec2 a, glm::fvec2 b);
    
    AiMap aiMap;
    
    Uint16 nodeRows = 0;
    Uint16 nodeCols = 0;
    Uint16 nodeWidth = 0;
    
    RenderQuad *renderQuad = nullptr;
    
    std::shared_ptr<Renderer> renderer = nullptr;
};

