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
        obj->update();
    }
}

void World::render() {
    terrain->render();
    for (auto &obj: worldObjects) {
        obj->render();
    }
}

void World::addObject(std::string objectName, glm::fvec3 position) {
    if (!worldObjectsDescription->isSectionExist(objectName)) {
        return;
    }
    
    if (!aiContainer->pathfinder.isObstacle(position)) {
        glm::fvec3 adjusedPosition = aiContainer->pathfinder.getObstaclePosition(position);
        auto worldObject = std::make_shared<WorldObject>();
        if (worldObject->init(objectName, renderer, aiContainer, worldObjectsDescription, adjusedPosition)) {
            worldObjects.push_back(std::move(worldObject));
        }
    }
}

void World::onMessage(IMessage *message) {
    if (message) {
        if (message->getMessage() == "List box item changed") {
            currentObjectSelected = message->getSenderId();
        }
        if (message->getKeyPressed() == "left_mouse_button_pressed") {
            auto mousePos = message->getMousePosition();
            addObject(currentObjectSelected, terrain->getTerrainPoint(mousePos));
        }
    }
}