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

void AiMapNode::calcDistances(AiMapNode *nodes, Sint16 distanceFromStart, Sint16 endNode) {
    startNodeDitance = distanceFromStart + 10;
    endNodeDistance = glm::abs(nodes[endNode].selfX - selfX) + glm::abs(nodes[endNode].selfY - selfY);
    fullDistance = startNodeDitance + endNodeDistance;
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

bool AiMap::getPath(std::vector<Sint16> &path, Sint16 start, Sint16 end) {
    Sint16 currentNode = start;
    nodes[start].closed = 1;
        
    while (currentNode != end) {
        std::cout << "current node " << currentNode << std::endl;
        // calc distances for neib nodes
        for (Sint16 i = 0; i < 4; i++) {
            Sint16 neib = nodes[currentNode].neibours[i];
            if (neib >= 0 && nodes[neib].closed == false) {
                nodes[neib].opened = true;
                nodes[neib].calcDistances(nodes, nodes[currentNode].startNodeDitance, end);
            }
        }
        
        // select random not closed neib node
        Sint16 closedCounter = 0;
        Sint16 newNode = 0;
        for (Sint16 i = 0; i < 4; i++) {
            newNode = nodes[currentNode].neibours[i];
            if (newNode >= 0) {
                if (nodes[newNode].closed == false) {
                    closedCounter++;
                    break;
                }
            }
        }
        
        // compare selected node distance with others neib nodes
        for (Sint16 i = 0; i < 4; i++) {
            if (nodes[newNode].closed == true) {
                closedCounter++;
            } else {
                Sint16 neib = nodes[currentNode].neibours[i];
                if (neib >= 0 && nodes[neib].closed == false) {
                    if (nodes[neib].fullDistance < nodes[newNode].fullDistance) {
                        newNode = neib;
                    } else {
                        if (nodes[neib].fullDistance == nodes[newNode].fullDistance) {
                            if (nodes[neib].endNodeDistance < nodes[newNode].endNodeDistance) {
                                newNode = neib;
                            } else {
                                if (nodes[neib].endNodeDistance < nodes[newNode].startNodeDitance) {
                                    newNode = neib;
                                }
                            }
                        }
                    }
                }
            }
        }
        if (closedCounter >= 4) {
            break;
        }
        
        nodes[newNode].closed = 1;
        currentNode = newNode;
    }
    std::cout << "current node " << currentNode << std::endl;
    
    path.push_back(currentNode);
    while (currentNode != start) {
        //std::cout << "current node out " << currentNode << std::endl;
        Sint16 nextNode = nodes[currentNode].neibours[0];
        for (Sint16 i = 0; i < 4; i++) {
            Sint16 neibNode = nodes[currentNode].neibours[i];
            if (neibNode >= 0 && nodes[neibNode].closed == true) {
                nextNode = neibNode;
            }
        }
        for (Sint16 i = 0; i < 4; i++) {
            Sint16 neibNode = nodes[currentNode].neibours[i];
            if (neibNode >= 0 && nodes[neibNode].closed == true) {
                if (nodes[neibNode].fullDistance < nodes[nextNode].fullDistance) {
                    nextNode = neibNode;
                }
                std::cout << nodes[nextNode].fullDistance << std::endl;
            }
        }
        currentNode = nextNode;
        path.push_back(currentNode);
    }
    return true;
}