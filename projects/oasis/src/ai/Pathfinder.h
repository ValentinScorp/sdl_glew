#pragma once

class PfNode {
public:
    PfNode();
    ~PfNode();
    
    float startNodeDitance = 0;
    float endNodeDistance = 0;
    float fullDistance = 0;
    bool closed = false;
    
    glm::ivec3 position;
    size_t originNode = 0;
    std::vector<size_t> neighbors;
};

class Pathfinder {
public:
    Pathfinder();
    virtual ~Pathfinder();
    
    void init(Uint16 width, Uint16 height, Uint16 nodeWidth);
    void getPath(glm::fvec3 begin, glm::fvec3 end, std::vector<glm::fvec3> &path);
    size_t getNode(glm::fvec3 pos);
    
    std::vector<PfNode> nodes;
    
    Uint16 nodeRows = 0;
    Uint16 nodeCols = 0;
    Uint16 nodeWidth = 0;
};

