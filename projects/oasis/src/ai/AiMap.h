#pragma once

class RenderQuad;

class AiMapNode {
public:
    AiMapNode() {}
    ~AiMapNode() {}
    
    void init(size_t index, Sint16 x, Sint16 y, Uint16 width, Uint16 height);
    void render(RenderQuad *renderQuad, float nodeWidth);
    
    Sint16 getFullDistance();
    
    Sint16 selfX = -1;
    Sint16 selfY = -1;
    Sint16 selfIndex = -1;
    Sint16 neibours[4];
    
    Sint16 blockLevel = 0;
    
    Sint16 startNodeDistance = 0;
    Sint16 endNodeDistance = 0;
    
    bool opened = false;
    bool closed = false;
        
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
    Sint16 getDistance(Sint16 nodeA, Sint16 nodeB);
    
    bool getPath(std::vector<Sint16> &path, Sint16 start, Sint16 end);
private:
    Uint16 width = 0;
    Uint16 height = 0;
    AiMapNode *nodes = nullptr;
   
};

