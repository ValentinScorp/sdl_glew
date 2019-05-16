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
    
    std::vector<Sint16> pathIndexes;
    aiMap.getPath(pathIndexes, startIdx, finishIdx);
    
    path.push_back(begin);
    for (auto& i: pathIndexes) {
        glm::fvec3 pos = getNodePosition(i);
        path.push_back(pos);
    }
    path.push_back(end);
}

glm::fvec3 Pathfinder::getNodePosition(size_t index) {
    glm::fvec3 position;
    
    Sint16 x = index - (index / nodeRows) * nodeRows;
    Sint16 y = index / nodeRows;
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
    if (nodeIndex >= 0)
        aiMap.setObstacle(nodeIndex);
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