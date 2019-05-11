#include "../Precompiled.h"

AiContainer::AiContainer() {

}

AiContainer::~AiContainer() {
}

void AiContainer::init(Camera *cam) {
    camera = cam;
    
    /* todo
     * [Terrain]
    wPatchesNum=8
    hPatchesNum=8
    wTilesInPatch=4
    hTilesInPatch=4
    tileSize=4.0
*/
   // pathfinder.init(4 * 4 * 2, 4 * 4 * 2, 4 / 2);
}

void AiContainer::destroy() {
    for (auto& a: agents) {
        if (a != nullptr) {
            delete a;
        }
    }
}

void AiContainer::update() {
    agents[0]->update(agents[1]);
    /*
    for (auto& a: agents) {
        if (a != nullptr) {
            a->update(agents);
        }
    }*/
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