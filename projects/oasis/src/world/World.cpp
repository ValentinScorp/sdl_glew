#include "../Precompiled.h"

World::World() {

}

World::~World() {
}

void World::init(std::shared_ptr<Renderer> renderer, std::shared_ptr<AiContainer> aiContainer) {
    this->renderer = renderer;
    this->aiContainer = aiContainer;
    
    worldObjectsDescription = std::make_shared<IniFile>("data/worldObjects.ini");
    
    terrain = std::make_shared<Terrain>();
    terrain->init(renderer);
}

void World::destroy() {
    for (auto &obj: worldObjects) {
        obj->destroy();
    }
    terrain->destroy();
}

void World::update() {
    terrain->update();
    for (auto &obj: worldObjects) {
        obj->update(1.0f);
    }
}

void World::render() {
    terrain->render();
    for (auto &obj: worldObjects) {
        obj->render();
    }
}

void World::addObject(std::string objectName, glm::fvec3 position) {
    auto worldObject = std::make_shared<RenderObject>();
    auto id = aiContainer->createAgent();
    auto agent = aiContainer->getAgent(id);
    agent->collisionRadius = worldObjectsDescription->getParameter(objectName, "collisionRadius").toFloat();
    
    agent->setPosition(position);
    if (worldObjectsDescription->getParameter(objectName, "staticPosition").toInt()) {
        agent->setObstacleOnAiMap();
    }
    worldObject->init(renderer, worldObjectsDescription, objectName, agent);
    worldObjects.push_back(std::move(worldObject));
}

void World::onMessage(IMessage *message) {
    if (message->getKeyPressed() == "left_mouse_button_pressed") {

        if (currentObjectSelected == "Tree") {
//            addObject(currentObjectSelected, )
        }
    }
    
    
}