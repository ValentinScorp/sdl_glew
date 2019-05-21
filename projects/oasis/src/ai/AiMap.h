#pragma once

class RenderQuad;

class AiMapNode {
public:
    AiMapNode() {}
    ~AiMapNode() {}
    
    void reset();
    void resetWatch();
    void init(size_t index, Sint16 x, Sint16 y, Uint16 width, Uint16 height);
    void render(RenderQuad *renderQuad, float nodeWidth);
    
    Sint16 getFullDistance();
        
    Sint16 selfX = -1;
    Sint16 selfY = -1;
    glm::ivec2 selfPos;
    Sint16 selfIndex = -1;
    static const Sint16 NEIB_MAX = 8;
    Sint16 neibours[NEIB_MAX];
    
    Sint16 blockLevel = 0;
    
    Sint16 startNodeDistance = 0;
    Sint16 endNodeDistance = 0;
    
    bool opened = false;
    bool closed = false;
    bool viewSightWatched = false;
        
    Sint16 parentNode = -1;
};

class AiMap
{
public:
    AiMap();
    virtual ~AiMap();
    
    void init(Uint16 width, Uint16 height);
    void destroy();
    void update();
    void render(RenderQuad *renderQuad, float nodeWidth);
    
    Sint16 getNodesCount();
    void setObstacle(Sint16 nodeIndex);
    void clearObstacle(Sint16 nodeIndex);
    bool isObstacle(Sint16 nodeIndex);
    float getDistance(Sint16 nodeA, Sint16 nodeB);
    
    bool getPath(std::vector<Sint16> &path, Sint16 start, Sint16 end);
    void removeIntermediate(std::vector<Sint16> &path);
    void smoothPath(std::vector<Sint16> &path);
    bool isLineOfSight(Sint16 nodeIndexA, Sint16 nodeIndexB);
    bool isCornerNeibsWalkable(Sint16 neib1Index, Sint16 neib2Index);
    bool neibIsCornerAndHasNeibBlock(Sint16 parentIndex, Sint16 neibIndex);
    float getPathLen(std::vector<Sint16> &path);

    Uint16 width = 0;
    Uint16 height = 0;
    AiMapNode *nodes = nullptr;
    
};

