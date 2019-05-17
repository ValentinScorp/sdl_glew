#include "../Precompiled.h"

void AiMapNode::init(size_t index, Sint16 x, Sint16 y, Uint16 width, Uint16 height) {
    selfIndex = index;
    selfX = x;
    selfY = y;
    
    Sint16 topLeft = index + width - 1;
    Sint16 topCent = index + width;
    Sint16 topRight = index + width + 1;
    Sint16 left = index - 1;
    Sint16 right = index + 1;
    Sint16 botLeft = index - width + 1;
    Sint16 botCent = index - width;
    Sint16 botRight = index - width - 1;
    
    Sint16 result = index % width;
    Sint16 remainder = index % width;
    
    bool bottomRow = (result == 0);
    bool topRow = (result == (height - 1));
    bool leftCol = (remainder == 0);
    bool rightCol = (remainder == (width - 1));
    /*
    neibours[0] = (!leftCol && !topRow)     ? topLeft : -1;
    neibours[1] = (!topRow)                 ? topCent : -1;
    neibours[2] = (!rightCol && !topRow)    ? topRight : -1;
    neibours[3] = (!rightCol)               ? right : -1;
    neibours[4] = (!rightCol && !bottomRow) ? botRight : -1;
    neibours[5] = (!bottomRow)              ? botCent : -1;
    neibours[6] = (!leftCol && !bottomRow)  ? botLeft : -1;
    neibours[7] = (!leftCol)                ? left : -1;
     */
    
    neibours[0] = (!topRow)                 ? topCent : -1;
    neibours[1] = (!rightCol)               ? right : -1;
    neibours[2] = (!bottomRow)              ? botCent : -1;
    neibours[3] = (!leftCol)                ? left : -1;
}

void AiMapNode::render(RenderQuad *renderQuad, float nodeWidth) {
    renderQuad->renderAt(glm::fvec3(selfX * nodeWidth + nodeWidth / 2, selfY * nodeWidth + nodeWidth / 2, 0.2f));
}

Sint16 AiMapNode::getFullDistance() {
    return startNodeDistance + endNodeDistance;
}

AiMap::AiMap() {

}

AiMap::~AiMap() {
}

void AiMap::init(Uint16 width, Uint16 height) {
    this->width = width;
    this->height = height;
    nodes = new AiMapNode[width * height];
    
    for (Sint16 y = 0; y < height; y++) {
        for (Sint16 x = 0; x < width; x++) {
            nodes[x + y * width].init(x + y * width, x, y, width, height);
        }
    }
}

void AiMap::destroy() {
    delete [] nodes;
}

void AiMap::update() {
   
}

Sint16 AiMap::getNodesCount() {
    return width * height;
}

void AiMap::setObstacle(Sint16 nodeIndex) {
    nodes[nodeIndex].blockLevel = 1;
}

void AiMap::clearObstacle(Sint16 nodeIndex) {
    nodes[nodeIndex].blockLevel = 0;
}

void AiMap::render(RenderQuad *renderQuad, float nodeWidth) {
    for (Sint16 i = 0; i < getNodesCount(); i++) {
        if (nodes[i].blockLevel != 0) {
            nodes[i].render(renderQuad, nodeWidth);
        }
    }
}

bool AiMap::isObstacle(Sint16 nodeIndex) {
    return (nodes[nodeIndex].blockLevel != 0);
}

Sint16 AiMap::getDistance(Sint16 nodeA, Sint16 nodeB) {
    glm::fvec2 a(nodes[nodeA].selfX, nodes[nodeA].selfY);
    glm::fvec2 b(nodes[nodeB].selfX, nodes[nodeB].selfY);
    return glm::length(a - b);
    //return glm::abs(nodes[nodeA].selfX - nodes[nodeB].selfX) + glm::abs(nodes[nodeA].selfY - nodes[nodeB].selfY);
}



bool AiMap::getPath(std::vector<Sint16> &path, Sint16 start, Sint16 end) {
    
    if (nodes[end].blockLevel != 0) {
        Sint16 closestNode = -1;
        for (size_t i = 0; i < 4; i++) {
            Sint16 neibIndex = nodes[end].neibours[i];
            if (neibIndex != -1) {
                if (nodes[neibIndex].blockLevel == 0) {
                    if (closestNode == -1) {
                        closestNode = neibIndex;
                    } else {
                        Sint16 newLen = getDistance(start, neibIndex);
                        Sint16 len = getDistance(start, closestNode);
                        if (newLen < len) {
                            closestNode = neibIndex;
                        }
                    }
                }
            }
        }
        if (closestNode != -1) {
            end = closestNode;
        } else {
            return false;
            std::cout << "Couldnot find end node\n";
        }
    }
    
    std::list<Sint16> openSet;
    std::list<Sint16> closedSet;
    
    openSet.push_front(start);
    
    while (openSet.size() > 0) {
        Sint16 currentNode = openSet.front();
        for (auto &i: openSet) {
            if (nodes[i].getFullDistance() < nodes[currentNode].getFullDistance()) {
                currentNode = i;
            } else {
                if (nodes[i].getFullDistance() == nodes[currentNode].getFullDistance()) {
                    if (nodes[i].endNodeDistance < nodes[currentNode].endNodeDistance) {
                        currentNode = i;
                    } 
                } 
            }
        }
        openSet.remove(currentNode);
        closedSet.push_front(currentNode);
        
        if (currentNode == end) {
            std::vector<Sint16> reversePath;
            Sint16 itNode = end;
            while (itNode != start) {
                reversePath.push_back(itNode);
                itNode = nodes[itNode].parentNode;
                if (itNode == -1) {
                    std::cout << "path error: invalid node in path" << std::endl;
                    return false;
                }
            }
            if (reversePath.size() == 0) {
                std::cout << "path error: start == end" << std::endl;
                return false;
            }
            for (size_t i = (reversePath.size() - 1); i > 0; i--) {
                path.push_back(reversePath[i]);
            }
            return true;
        }
        
        for (Sint16 i = 0; i < 4; i++) {
            Sint16 neib = nodes[currentNode].neibours[i];
            if (neib >= 0) {
                auto it = std::find(closedSet.begin(), closedSet.end(), neib);
                if (isObstacle(neib) || it != closedSet.end()) {
                    continue;
                } else {
                    Sint16 newNeibStartDistanse = nodes[currentNode].startNodeDistance + getDistance(currentNode, neib);
                    auto it = std::find(openSet.begin(), openSet.end(), neib);
                    if (newNeibStartDistanse < nodes[neib].startNodeDistance || it == openSet.end()) {
                        nodes[neib].startNodeDistance = newNeibStartDistanse;
                        nodes[neib].endNodeDistance = getDistance(neib, end);
                        nodes[neib].parentNode = currentNode;
                        
                        if (it == openSet.end()) {
                            openSet.push_front(neib);
                        }
                    }
                }
            }
        }
    }
    
    return false;
}