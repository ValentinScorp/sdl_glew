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
    
    pathfinder.init(renderer, wPatchesNum * wTilesInPatch * 2, hPatchesNum * hTilesInPatch * 2, tileSize / 2);
}

void AiContainer::destroy() {
    agents.clear();
}

void AiContainer::update() {
    for (auto &a : agents) {
        a.update(agents);
    }
    pathfinder.aiMap.update();
}

void AiContainer::render() {
    pathfinder.render();
}

size_t AiContainer::createAgent(WorldObject *wo) {
    AiAgent agent;
    agent.init(camera, &pathfinder, wo);
    agents.push_back(agent);
    return (agents.size() - 1);
}

/*
AiAgent* AiContainer::createAgent(WorldObject *wo) {
    AiAgent agent;
    agent.init(camera, &pathfinder, wo);
    agents.push_back(agent);
    SMessageManager::getInstance().registerRecipient(&agents.back());
    return &agents.back();
}*/
/*
AiAgent* AiContainer::getAgent(size_t id) {
    std::cout << "getting agent id pointer " << id << std::endl;    
    //AiAgent &agent = agents.at(id);
    AiAgent* firstElement = agents.data();
    for (size_t i = 0; i < agents.size(); i++) {
        if (i == id) {
            return firstElement;
        }
        firstElement++;
    }
    std::cout << "element not found" << std::endl;
    return nullptr;
}
*/

AiAgent& AiContainer::getAgent(size_t id) {
    return agents[id];
}

void AiContainer::onMessage(IMessage *message) {
    for (auto& a: agents) {
        a.onMessage(message);
    }
}