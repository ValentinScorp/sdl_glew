#include "../Precompiled.h"

AiContainer::AiContainer() {

}

AiContainer::~AiContainer() {
}

void AiContainer::init(std::shared_ptr<Renderer> renderer, Camera *cam, std::shared_ptr<IniFile> cfg) {
    camera = cam;
    
    Uint16 wPatchesNum = cfg->getParameter("Terrain", "wPatchesNum").toInt();
    Uint16 hPatchesNum = cfg->getParameter("Terrain", "hPatchesNum").toInt();
    Uint16 wTilesInPatch = cfg->getParameter("Terrain", "wTilesInPatch").toInt();
    Uint16 hTilesInPatch = cfg->getParameter("Terrain", "hTilesInPatch").toInt();
    float tileSize = cfg->getParameter("Terrain", "tileSize").toFloat();
    
    this->renderer = renderer;
    pathfinder.init(renderer, wPatchesNum * wTilesInPatch * 2, hPatchesNum * hTilesInPatch * 2, tileSize / 2);
}

void AiContainer::destroy() {
    for (auto &a: agentsPtr) {
        a->destroy();
    }
    agentsPtr.clear();
    
    for (auto &w: walls) {
        w->destroy();
    }
    walls.clear();
}

void AiContainer::update() {
    for (AiAgent* a : agentsPtr) {
        a->update(agentsPtr);
    }
    pathfinder.aiMap.update();
}

void AiContainer::render() {
    pathfinder.render();
    for (AiAgent* a : agentsPtr) {
        a->render();
    }
    for (auto &w: walls) {
        w->render();
    }
}

size_t AiContainer::createAgent(WorldObject *wo) {
    auto agent = new AiAgent();
    agent->init(camera, &pathfinder, wo, renderer);
    agentsPtr.push_back(agent);
    
    return (agentsPtr.size() - 1);
}

AiWall* AiContainer::createObstacle(WorldObject *wo) {
    AiWall* wall = new AiWall();
    wall->init(camera, &pathfinder, wo, renderer);
    walls.push_back(wall);
    return wall;
}

void AiContainer::calcBlockNodes(AiWall *wall) {
    pathfinder.placeBlocks(wall->positionA, wall->positionB);
}

AiAgent* AiContainer::getAgent(size_t id) {
    return agentsPtr[id];
}

void AiContainer::onMessage(IMessage *message) {
    for (auto& a: agentsPtr) {
        a->onMessage(message);
    }
}