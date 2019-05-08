#include "../Precompiled.h"

AiContainer::AiContainer() {
}

AiContainer::~AiContainer() {
}

void AiContainer::init(Camera *cam) {
    camera = cam;
}

void AiContainer::destroy() {
    for (auto& a: agents) {
        if (a != nullptr) {
            delete a;
        }
    }
}

void AiContainer::update() {
    for (auto& a: agents) {
        if (a != nullptr) {
            a->update();
        }
    }
}

size_t AiContainer::createAgent() {
    auto agent = new AiAgent();
    agent->init(camera);
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