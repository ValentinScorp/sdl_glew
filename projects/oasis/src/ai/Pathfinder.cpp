#include "../Precompiled.h"

Pathfinder::Pathfinder() {
    
}

Pathfinder::~Pathfinder()
{
}

void Pathfinder::init(std::shared_ptr<Renderer> renderer, Uint16 width, Uint16 height, Uint16 nodeWidth) {
    this->renderer = renderer;
    this->nodeWidth = nodeWidth;
    this->nodeRows = width;
    this->nodeCols = height;
  
    aiMap.init(width, height);
    
    renderQuad = new RenderQuad();
    renderQuad->init(renderer, nodeWidth);
}

void Pathfinder::render() {
    aiMap.render(renderQuad, nodeWidth);
}

void Pathfinder::destroy() {
    renderQuad->destroy();
    delete renderQuad;
}

void Pathfinder::getPath(glm::fvec2 begin, glm::fvec2 end, std::vector<glm::fvec2> &path) {
    auto startIdx = getNodeIndex(begin);
    auto finishIdx = getNodeIndex(end);
    
   // std::cout << std::endl << std::endl;
  //  std::cout << "begin " << begin.x << " x " << begin.y << std::endl;
  //  std::cout << "end   " << end.x << " x " << end.y << std::endl;
  //  std::cout << "indexes  " << startIdx << " x " << finishIdx << std::endl;
    
   // std::cout << "begin real " << aiMap.nodes[startIdx].selfX << " x " << aiMap.nodes[startIdx].selfY << std::endl;
   // std::cout << "end real " << aiMap.nodes[finishIdx].selfX << " x " << aiMap.nodes[finishIdx].selfY << std::endl;    
   // std::cout << std::endl;
    
    std::vector<Sint16> pathIndexes;
    Timer findPath;
    findPath.play();
    aiMap.getPath(pathIndexes, startIdx, finishIdx);
    findPath.pause();
    findPath.print("Path found in");
    
  //  std::cout << std::endl;
    path.push_back(begin);
    //for (Sint16 i = 1; i < pathIndexes.size() - 1; i++) {
    for (Sint16 i = 0; i < pathIndexes.size(); i++) {
        glm::fvec2 pos = getNodePosition(pathIndexes[i]);
        path.push_back(pos);
    }
    path.push_back(end);
    for (Sint16 i = 0; i < pathIndexes.size(); i++) {
        glm::fvec2 pos = getNodePosition(pathIndexes[i]);
        std::cout << "found path " << pos.x << " x " << pos.y << std::endl;
    }
}

glm::fvec2 Pathfinder::getNodePosition(size_t index) {
    glm::fvec2 position(0.0f);
    if (index != -1) {
    
        Sint16 x = index - (index / nodeRows) * nodeRows;
        Sint16 y = index / nodeRows;
       // std::cout << "index " << index << std::endl;
       // std::cout << "path nodes " << x << " x " << y << std::endl;
        position.x = x * nodeWidth + nodeWidth / 2;
        position.y = y * nodeWidth + nodeWidth / 2;
    }
    return position;
}
glm::fvec2 Pathfinder::getObstaclePosition(glm::fvec2 pos) {
    return getNodePosition(getNodeIndex(pos));
}

size_t Pathfinder::getNodeIndex(glm::fvec2 pos) {
    Sint16 xOffs = ((Uint16) (pos.x / nodeWidth));
    Sint16 yOffs = ((Uint16) (pos.y / nodeWidth));
    
    Sint16 index = xOffs + yOffs * nodeRows;
    
    if (index >= 0 && index < aiMap.getNodesCount()) {
        return index;
    }
    
    return 0;
}

void Pathfinder::setStaticObstacle(glm::fvec2 position) {
    Sint16 nodeIndex = getNodeIndex(position);
    if (nodeIndex >= 0) {
        //std::cout << "setting obstacle at index  " << nodeIndex << std::endl;
        //std::cout << "setting obstacle at node pos  " << aiMap.nodes[nodeIndex].selfX << " x " << aiMap.nodes[nodeIndex].selfY << std::endl;
        aiMap.setObstacle(nodeIndex);
        
    }
}

void Pathfinder::removeStaticObstacle(glm::fvec2 position) {
    Sint16 nodeIndex = getNodeIndex(position);
    if (nodeIndex >= 0)
        aiMap.clearObstacle(nodeIndex);
}

bool Pathfinder::isObstacle(glm::fvec2 position) {
    Sint16 nodeIndex = getNodeIndex(position);
    if (nodeIndex >= 0)
        return aiMap.isObstacle(nodeIndex);
}