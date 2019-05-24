#include "../Precompiled.h"

void AiMapNode::reset() {
    startNodeDistance = 0;
    endNodeDistance = 0;
    opened = false;
    closed = false;
    viewSightWatched = false;
    parentNode = -1;
}

void AiMapNode::resetWatch() {
    viewSightWatched = false;
}


void AiMapNode::init(size_t index, Sint16 x, Sint16 y, Uint16 width, Uint16 height) {
    selfIndex = index;
    selfX = x;
    selfY = y;
    selfPos.x = selfX;
    selfPos.y = selfY;
    
    Sint16 topLeft = index + width - 1;
    Sint16 topCent = index + width;
    Sint16 topRight = index + width + 1;
    Sint16 left = index - 1;
    Sint16 right = index + 1;
    Sint16 botRight = index - width + 1;
    Sint16 botCent = index - width;
    Sint16 botLeft = index - width - 1;
    
    Sint16 result = index / width;
    Sint16 remainder = index % width;
    
    bool bottomRow = (result == 0);
    bool topRow = (result == (height - 1));
    bool leftCol = (remainder == 0);
    bool rightCol = (remainder == (width - 1));
    
    neibours[0] = (!leftCol && !topRow)     ? topLeft : -1;
    neibours[1] = (!topRow)                 ? topCent : -1;
    neibours[2] = (!rightCol && !topRow)    ? topRight : -1;
    neibours[3] = (!rightCol)               ? right : -1;
    neibours[4] = (!rightCol && !bottomRow) ? botRight : -1;
    neibours[5] = (!bottomRow)              ? botCent : -1;
    neibours[6] = (!leftCol && !bottomRow)  ? botLeft : -1;
    neibours[7] = (!leftCol)                ? left : -1;
    
    for (Sint16 i = 0; i < 8; i++) {
  //      std::cout << "neib " << i << " index " << neibours[i] << std::endl;
        
    }
        
    /*neibours[0] = (!topRow)                 ? topCent : -1;
    neibours[1] = (!rightCol)               ? right : -1;
    neibours[2] = (!bottomRow)              ? botCent : -1;
    neibours[3] = (!leftCol)                ? left : -1;
     * */
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
          //  if ((x + y * width) > 256) {
         //       break;
          //  }
          //  std::cout<< "initing node " << x + y * width << std::endl;
            nodes[x + y * width].init(x + y * width, x, y, width, height);
        }
    }
}

void AiMap::destroy() {
    delete [] nodes;
}

void AiMap::update() {
    for (Sint16 i = 0; i < width* height; i++) {
        if (nodes[i].blockLevel > 0) {
            //std::cout << "block at " << i << std::endl;
        }
    }
   
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

float AiMap::getDistance(Sint16 nodeA, Sint16 nodeB) {
    glm::fvec2 a(nodes[nodeA].selfX * 10, nodes[nodeA].selfY * 10);
    glm::fvec2 b(nodes[nodeB].selfX * 10, nodes[nodeB].selfY * 10);
    return glm::length(a - b);
    //return glm::abs(nodes[nodeA].selfX - nodes[nodeB].selfX) + glm::abs(nodes[nodeA].selfY - nodes[nodeB].selfY);
}

bool AiMap::isCornerNeibsWalkable(Sint16 neib1Index, Sint16 neib2Index) {
    if (neib1Index != -1 && neib2Index != -1) {
      //  std::cout << "neib1Index " << neib1Index << " -> block level " << nodes[neib1Index].blockLevel << std::endl;
     //   std::cout << "neib2Index " << neib2Index << " -> block level " << nodes[neib2Index].blockLevel << std::endl;
        return (nodes[neib1Index].blockLevel == 0 && nodes[neib2Index].blockLevel == 0);
    }
    return false;
}

bool AiMap::neibIsCornerAndHasNeibBlock(Sint16 parentIndex, Sint16 neibIndex) {
    if (neibIndex == 0) {
       return !isCornerNeibsWalkable(nodes[parentIndex].neibours[7], nodes[parentIndex].neibours[1]);
    }
    if (neibIndex == 2) {
        return !isCornerNeibsWalkable(nodes[parentIndex].neibours[1], nodes[parentIndex].neibours[3]);
    }
    if (neibIndex == 4) {
        return !isCornerNeibsWalkable(nodes[parentIndex].neibours[3], nodes[parentIndex].neibours[5]);
    }
    if (neibIndex == 6) {
        return !isCornerNeibsWalkable(nodes[parentIndex].neibours[5], nodes[parentIndex].neibours[7]);
    }
    return false;
}

float AiMap::getPathLen(std::vector<Sint16> &path) {
    float len = 0;
    for (Sint16 i = 1; i < path.size(); i++) {
        glm::fvec2 p1(nodes[path[i]].selfX, nodes[path[i]].selfY);
        glm::fvec2 p2(nodes[path[i-1]].selfX, nodes[path[i-1]].selfY);
        len += glm::length(p1 - p2);
    }
    return len;
}

bool AiMap::getPath(std::vector<Sint16> &path, Sint16 start, Sint16 end) {
    for (size_t i = 0; i < width * height; i++) {
        nodes[i].reset();
    }
    if (nodes[end].blockLevel != 0) {
        Sint16 closestNode = -1;
        for (size_t i = 0; i < AiMapNode::NEIB_MAX; i++) {
            Sint16 neibIndex = nodes[end].neibours[i];
            if (neibIndex != -1) {
                if (nodes[neibIndex].blockLevel == 0) {
                    if (closestNode == -1) {
                        closestNode = neibIndex;
                    } else {
                        float newLen = getDistance(start, neibIndex);
                        float len = getDistance(start, closestNode);
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
            reversePath.push_back(itNode);
            
            if (reversePath.size() == 0) {
                return false;
            }
            for (Sint16 i = (reversePath.size() - 1); i >= 0; i--) {
                path.push_back(reversePath[i]);
            }

 //           std::cout << "path found " << std::endl;
  //          for (Sint16 i = 0; i < path.size(); i++) {
  //              std::cout << nodes[path[i]].selfX << " x " << nodes[path[i]].selfY << std::endl;
  //          }

 //           std::cout << "smoothing path " << std::endl;
            std::vector<Sint16> tmpPath;
            for (Sint16 i = 1; i < path.size() - 1; i++) {
                tmpPath.push_back(path[i]);
            }
            //smoothPath(reversePath);
            reverseAndSmoothPath(reversePath);
            //smoothPath(tmpPath);
            //path.clear();
            //path = tmpPath;
            path = reversePath;

 //           for (Sint16 i = 0; i < path.size(); i++) {
      //          std::cout << nodes[path[i]].selfX << " x " << nodes[path[i]].selfY << std::endl;
    //        }
          //  std::cout << std::endl;
            return true;
        }
        
        for (Sint16 i = 0; i < AiMapNode::NEIB_MAX; i++) {
            Sint16 neib = nodes[currentNode].neibours[i];
            if (neib >= 0) {
                auto it = std::find(closedSet.begin(), closedSet.end(), neib);
                if (currentNode == 130 && i == 4) {
                 //   std::cout << "checking neib of -> "<< 131 << "neib index " << neib  <<  "walkable neib " << i << " -> " << neibIsCornerAndHasNeibBlock(currentNode, i) << std::endl;
                }
                if (isObstacle(neib) || it != closedSet.end() || neibIsCornerAndHasNeibBlock(currentNode, i)) {
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

void AiMap::removeIntermediate(std::vector<Sint16> &path) {
    for (size_t i = 0; i < width * height; i++) {
        nodes[i].reset();
    }
    size_t pathIndex = 0;
    for (auto &p: path) {
    //    std::cout << p << std::endl;
    }
   // std::cout << "path size " << path.size() << std::endl;
    std::vector<Sint16> newPath;
    if (path.size() > 2) {
        newPath.push_back(path[pathIndex]);
        while (pathIndex < (path.size() - 2)) {
            size_t stepIndex = pathIndex + 1;
            Sint16 firstPointIndex = path[pathIndex];
            Sint16 secondPointIndex = path[stepIndex];
            glm::ivec2 firstDirection = nodes[path[stepIndex]].selfPos - nodes[path[pathIndex]].selfPos;
            glm::ivec2 secondDirection = nodes[path[stepIndex + 1]].selfPos - nodes[path[stepIndex]].selfPos;
           // std::cout << "firstDirection " << firstDirection.x << " x " << firstDirection.y  << std::endl;
            //std::cout << "secondDirection " << secondDirection.x << " x " << secondDirection.y  << std::endl;
            while (firstDirection == secondDirection) {
                stepIndex++;
                if (stepIndex >= (path.size() - 1)) {
                    break;
                } else {
                    secondDirection = nodes[path[stepIndex + 1]].selfPos - nodes[path[stepIndex]].selfPos;
                    secondPointIndex = path[stepIndex];
              //      std::cout << "secondDirection " << secondDirection.x << " x " << secondDirection.y  << std::endl;
                }
            }
            pathIndex = stepIndex;
            //std::cout << "Pushing PathIndex " << nodes[path[pathIndex]].selfPos.x << " x " << nodes[path[pathIndex]].selfPos.y << std::endl;
            newPath.push_back(path[pathIndex]);
        }
    } else {
        newPath = path;
    }
    path.clear();
    path = newPath;
}

Uint16 AiMap::calcSimplifiedDistance(Sint16 pA, Sint16 pB) {
    if (pA == -1 || pB == -1) {
        return 0;
    }
    return 1000 * glm::sqrt((glm::pow((float)(nodes[pA].selfPos.x - nodes[pB].selfPos.x), 2) + glm::pow((float)(nodes[pA].selfPos.y - nodes[pB].selfPos.y), 2)));
}

Uint16 AiMap::calcSimplifiedDistance(Sint16 pMid, Sint16 pA, Sint16 pB) {
    if (pMid == -1 || pA == -1 || pB == -1) {
        return 0;
    }
    return glm::pow((nodes[pMid].selfPos.x - nodes[pA].selfPos.x), 2) + glm::pow((nodes[pMid].selfPos.y - nodes[pA].selfPos.y), 2) +
           glm::pow((nodes[pMid].selfPos.x - nodes[pB].selfPos.x), 2) + glm::pow((nodes[pMid].selfPos.y - nodes[pB].selfPos.y), 2);
   // return glm::sqrt(glm::pow((nodes[pMid].selfPos.x - nodes[pA].selfPos.x), 2) + glm::pow((nodes[pMid].selfPos.y - nodes[pA].selfPos.y), 2)) +
   //        glm::sqrt(glm::pow((nodes[pMid].selfPos.x - nodes[pB].selfPos.x), 2) + glm::pow((nodes[pMid].selfPos.y - nodes[pB].selfPos.y), 2));
}

bool AiMap::isLineOfSight(Sint16 nodeIndexA, Sint16 nodeIndexB) {
    if (nodeIndexA == nodeIndexB) {
        return true;
    }
    if (nodeIndexA == -1 || nodeIndexB == -1) {
        return false;
    }
    for (Sint16 i = 0; i < width * height; i++) {
        nodes[i].resetWatch();
    }
//    std::cout << "watch sight from " << nodes[nodeIndexA].selfX << " x " << nodes[nodeIndexA].selfY << std::endl;
 //   std::cout << "            to   " << nodes[nodeIndexB].selfX << " x " << nodes[nodeIndexB].selfY << std::endl;
    Sint16 currentNodeIndex = nodeIndexA;
    while (currentNodeIndex != nodeIndexB) {
        Sint16 minNeibIndex = -1;
        Sint16 minNeibIndexSecond = -1;
        Uint16 minDistance = 0;
        Uint16 minDistanceB = 0;
//        std::cout << std::endl << std::endl;
//       std::cout << "checking node pos " << nodes[currentNodeIndex].selfX << " x " << nodes[currentNodeIndex].selfY << std::endl;
        for (Sint16 i = 1; i < AiMapNode::NEIB_MAX; i+=2) {
            Sint16 neibIndex = nodes[currentNodeIndex].neibours[i];
//            std::cout << std::endl;
//            std::cout << "neib " << i << "   index " << neibIndex << std::endl;
            if (neibIndex != -1) {
           //     std::cout << "neib was watched " << nodes[neibIndex].viewSightWatched << std::endl;
                if (neibIndex == nodeIndexB) {
                    return true;
                }
                if (nodes[neibIndex].viewSightWatched == false) {
 //                   std::cout << "neib node pos " << nodes[neibIndex].selfX << " x " << nodes[neibIndex].selfY << std::endl;
//                    std::cout << "neib node pos " << nodes[neibIndex].selfPos.x << " x " << nodes[neibIndex].selfPos.y << std::endl;
 //                   std::cout << "start  " << nodes[nodeIndexA].selfX << " x " << nodes[nodeIndexA].selfY << std::endl;
//                    std::cout << "end    " << nodes[nodeIndexB].selfX << " x " << nodes[nodeIndexB].selfY << std::endl;
                    Uint16 newDistanceA = calcSimplifiedDistance(neibIndex, nodeIndexA);
                    Uint16 newDistanceB = calcSimplifiedDistance(neibIndex, nodeIndexB);
                    Uint16 newDistance = newDistanceA + newDistanceB;
                  //  std::cout << "newDistance " << newDistance << " minDistance " << minDistance << std::endl;
                    if (minDistance == 0 || newDistance < minDistance) {
                        minDistance = newDistance;
                        minDistanceB = newDistanceB;
                        minNeibIndex = neibIndex;
                        minNeibIndexSecond = -1;
                      //  std::cout << "new min dist " << minDistance << " index " << minNeibIndex << std::endl;
                    } else {
                        if (newDistance == minDistance) {
                            if (newDistanceB < minDistanceB) {
                            ///   std::cout << "distances equal but new distance to B is less " << neibIndex << std::endl;
                                minDistance = newDistance;
                                minDistanceB = newDistanceB;
                                minNeibIndex = neibIndex;
                                minNeibIndexSecond = -1;
                            } else {
                             //   std::cout << "second found " << neibIndex << std::endl;
                                minNeibIndexSecond = neibIndex;
                            }
                        }
                    }
                }
            }
        }
        if (minNeibIndex == -1) {
            return false;
        }
        if (minNeibIndex == nodeIndexB) {
            return true;
        }
        if (nodes[minNeibIndex].blockLevel > 0) {
            return false;
        }
        if (minNeibIndexSecond != -1) {
            if (nodes[minNeibIndexSecond].blockLevel > 0)
                return false;
        }
        nodes[currentNodeIndex].viewSightWatched = true;
        currentNodeIndex = minNeibIndex;
    }
    return false;
}

Sint16 AiMap::findClosestVisible(std::vector<Sint16> &path, Sint16 startIndex) {
    if (startIndex >= path.size()) {
        return -1;
    }
    if (startIndex >= (path.size() - 2)) {
        return path.size() - 1;
    }
    Sint16 nextIndex = startIndex + 2;
    while (nextIndex < path.size()) {
        if (!isLineOfSight(path[startIndex], path[nextIndex])) {
            return nextIndex - 1;
        } else {
            nextIndex++;
        }
    }
    return nextIndex - 1;
}

void AiMap::reverseAndSmoothPath(std::vector<Sint16> &revPath) {
    if (revPath.size() <= 2) {
        return;
    }
    std::vector<Sint16> newPath;
    
    // smooth from end
    Sint16 closestToEnd = findClosestVisible(revPath, 0);
    if (closestToEnd == -1) {
        std::cout << "Unable to smooth path 1 - iteration" << std::endl;
        return;
    }
    newPath.push_back(revPath[0]);
    for (Sint16 i = closestToEnd; i < revPath.size(); i++) {
        newPath.push_back(revPath[i]);
    }
    //
    // reverse path to make it in right order
    revPath.clear();
    for (Sint16 i = (newPath.size() - 1); i >= 0; i--) {
        revPath.push_back(newPath[i]);
    }
    if (revPath.size() <= 2) {
        return;
    }
    
    Sint16 closestToStart = findClosestVisible(revPath, 0);
    if (closestToStart == -1) {
        std::cout << "Unable to smooth path 2 - iteration" << std::endl;
        return;
    }
    std::vector<Sint16> path;
    path.push_back(revPath[0]);
    path.push_back(revPath[closestToStart]);
    for (Sint16 i = closestToStart; i < revPath.size(); i++) {
        path.push_back(revPath[i]);
    }
    // smooth the rest - todo need optimize to not include first and last points
    smoothPath(path);
    revPath.clear();
    revPath = path;
}

void AiMap::smoothPath(std::vector<Sint16> &path) {
    if (path.size() <= 2) {
        return;
    }
    std::vector<Sint16> newPath;
    
    Sint16 firstPathIndex = 0;
    Sint16 lastPathIndex = firstPathIndex + 2;
    
    while (lastPathIndex <= path.size()) {
        Sint16 firstNodeIndex = path[firstPathIndex];
        newPath.push_back(firstNodeIndex);
        
        for (Sint16 i = lastPathIndex; i < path.size(); i++) {
            Sint16 lastNodeIndex = path[i];
            if (isLineOfSight(firstNodeIndex, lastNodeIndex)) {
                lastPathIndex = i;
            } else {
                lastPathIndex = i;
                break;
            }
        }
        firstPathIndex = lastPathIndex - 1;
        lastPathIndex += 1;
        if (lastPathIndex == (path.size() - 1)) {
            newPath.push_back(path[firstPathIndex]);
            newPath.push_back(path[lastPathIndex]);
            break;
        }
        if (lastPathIndex == path.size()) {
            newPath.push_back(path[firstPathIndex]);
            newPath.push_back(path[firstPathIndex + 1]);
            break;
        }
        if (lastPathIndex == (path.size() + 1)) {
            newPath.push_back(path[firstPathIndex]);
            break;
        }
    }
    path.clear();
    path = newPath;
}
