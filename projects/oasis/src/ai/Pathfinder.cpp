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

void Pathfinder::getPath(glm::fvec3 begin, glm::fvec3 end, std::vector<glm::fvec3> &path) {
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
    aiMap.getPath(pathIndexes, startIdx, finishIdx);
  //  std::cout << std::endl;
    path.push_back(begin);
    for (Sint16 i = 0; i < pathIndexes.size(); i++) {
        glm::fvec3 pos = getNodePosition(pathIndexes[i]);
        path.push_back(pos);
    }
    path.push_back(end);
}

glm::fvec3 Pathfinder::getNodePosition(size_t index) {
    glm::fvec3 position;
    
    Sint16 x = index - (index / nodeRows) * nodeRows;
    Sint16 y = index / nodeRows;
   // std::cout << "index " << index << std::endl;
   // std::cout << "path nodes " << x << " x " << y << std::endl;
    position.x = x * nodeWidth + nodeWidth / 2;
    position.y = y * nodeWidth + nodeWidth / 2;
    position.z = 0.0;
    return position;
}

size_t Pathfinder::getNodeIndex(glm::fvec3 pos) {
    Sint16 xOffs = ((Uint16) (pos.x / nodeWidth));
    Sint16 yOffs = ((Uint16) (pos.y / nodeWidth));
    
    Sint16 index = xOffs + yOffs * nodeRows;
    
    if (index >= 0 && index < aiMap.getNodesCount()) {
        return index;
    }
    
    return 0;
}

void Pathfinder::setStaticObstacle(glm::fvec3 position) {
    Sint16 nodeIndex = getNodeIndex(position);
    if (nodeIndex >= 0) {
        //std::cout << "setting obstacle at index  " << nodeIndex << std::endl;
        //std::cout << "setting obstacle at node pos  " << aiMap.nodes[nodeIndex].selfX << " x " << aiMap.nodes[nodeIndex].selfY << std::endl;
        aiMap.setObstacle(nodeIndex);
        
    }
}

void Pathfinder::removeStaticObstacle(glm::fvec3 position) {
    Sint16 nodeIndex = getNodeIndex(position);
    if (nodeIndex >= 0)
        aiMap.clearObstacle(nodeIndex);
}

bool Pathfinder::isObstacle(glm::fvec3 position) {
    Sint16 nodeIndex = getNodeIndex(position);
    if (nodeIndex >= 0)
        return aiMap.isObstacle(nodeIndex);
}