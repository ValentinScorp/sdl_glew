#include "../Precompiled.h"

AiContainer::AiContainer() {

}

AiContainer::~AiContainer() {
}

void AiContainer::init(std::shared_ptr<Renderer> renderer, Camera *cam, Configuration *cfg) {
    camera = cam;
    
    Uint16 wPatchesNum = cfg->getParameter("Terrain", "wPatchesNum").toInt();
    Uint16 hPatchesNum = cfg->getParameter("Terrain", "hPatchesNum").toInt();
    Uint16 wTilesInPatch = cfg->getParameter("Terrain", "wTilesInPatch").toInt();
    Uint16 hTilesInPatch = cfg->getParameter("Terrain", "hTilesInPatch").toInt();
    float tileSize = cfg->getParameter("Terrain", "tileSize").toFloat();
    
    pathfinder.init(renderer, wPatchesNum * wTilesInPatch * 2, hPatchesNum * hTilesInPatch * 2, tileSize / 2);
}

void AiContainer::destroy() {
    for (auto& a: agents) {
        if (a != nullptr) {
            delete a;
        }
    }
}

void AiContainer::update() {
    for (auto &a : agents) {
        for (auto &obstacles: agents) {
            a->update(obstacles);
        }
    }
}

void AiContainer::render() {
    pathfinder.render();
}

size_t AiContainer::createAgent() {
    auto agent = new AiAgent();
    agent->init(camera, &pathfinder);
    agents.push_back(agent);
    return (agents.size() - 1);
}

AiAgent* AiContainer::getAgent(size_t id){
    return agents[id];
}

void AiContainer::destroyAgent(size_t id) {
    if (agents[id] != nullptr){
        delete agents[id];
        agents[id] = nullptr;
    }
}