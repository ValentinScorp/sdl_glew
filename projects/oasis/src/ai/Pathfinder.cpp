#include "../Precompiled.h"

PfNode::PfNode() {
    
}

PfNode::~PfNode() {
    
}

Pathfinder::Pathfinder() {
    
}

Pathfinder::~Pathfinder()
{
}

void Pathfinder::init(Uint16 width, Uint16 height, Uint16 nodeWidth) {
    this->nodeWidth = nodeWidth;
    this->nodeRows = width;
    this->nodeCols = height;
    
    for (Uint16 h = 0; h < height; h++) {
        for (Uint16 w = 0; w < width; w++) {
            PfNode node;
            
            Sint16 x = w * nodeWidth + nodeWidth / 2;
            Sint16 y = h * nodeWidth + nodeWidth / 2;
            node.position = glm::ivec3(x, y, 0); // todo terrain height
                        
            size_t index = h * w;
            
            size_t topLeft = index + width - 1;
            size_t topCent = index + width;
            size_t topRight = index + width + 1;
            size_t left = index - 1;
            size_t right = index + 1;
            size_t botLeft = index - width + 1;
            size_t botCent = index - width;
            size_t botRight = index - width - 1;
            
            Uint16 result = index % width;
            Uint16 remainder = index % width;
            
            bool bottomRow = (result == 0);
            bool topRow = (result == (height - 1));
            bool leftCol = (remainder == 0);
            bool rightCol = (remainder == (width - 1));
            
            if (!leftCol && !topRow)     node.neighbors.push_back(topLeft);
            if (!topRow)                 node.neighbors.push_back(topCent);
            if (!rightCol && !topRow)    node.neighbors.push_back(topRight);
            if (!rightCol)               node.neighbors.push_back(right);
            if (!rightCol && !bottomRow) node.neighbors.push_back(botRight);
            if (!bottomRow)              node.neighbors.push_back(botCent);
            if (!leftCol && !bottomRow)  node.neighbors.push_back(botLeft);
            if (!leftCol)                node.neighbors.push_back(left);
            
            nodes.push_back(node);
        }
    }
}

void Pathfinder::getPath(glm::fvec3 begin, glm::fvec3 end, std::vector<glm::fvec3> &path) {
    path.push_back(begin);
    path.push_back(end);
}

size_t Pathfinder::getNode(glm::fvec3 pos) {
    Uint16 xOffs = ((Uint16) (pos.x / nodeWidth));
    Uint16 yOffs = ((Uint16) (pos.y / nodeWidth));
    
    return xOffs + yOffs * nodeRows;
}