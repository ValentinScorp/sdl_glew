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

void World::addObstacle(glm::fvec3 position) {
    if (!worldObjectsDescription->isSectionExist("Banner")) {
        return;
    }
    
    //auto worldObject = std::make_shared<WorldObject>();
    //if (worldObject->init("Banner", this, renderer, aiContainer, worldObjectsDescription, adjusedPosition)) {
    //    worldObjects.push_back(std::move(worldObject));
    //}
}

void World::addObject(std::string objectName, glm::fvec3 position) {
    if (!worldObjectsDescription->isSectionExist(objectName)) {
        return;
    }
    
    if (!aiContainer->pathfinder.isObstacle(glm::fvec2(position.x, position.y))) {
        glm::fvec2 obstaclePosition = aiContainer->pathfinder.getObstaclePosition(position);
        glm::fvec3 adjusedPosition(obstaclePosition.x, obstaclePosition.y, terrain->getHeight(obstaclePosition));
        auto worldObject = std::make_shared<WorldObject>();
        if (worldObject->init(objectName, this, renderer, aiContainer, worldObjectsDescription, adjusedPosition)) {
            worldObjects.push_back(std::move(worldObject));
        }
    }
}

void World::onMessage(IMessage *message) {
    if (message) {
        if (message->getMessage() == "List box item changed") {
            obstaclePlaceBegin = false;
            currentObjectSelected = message->getSenderId();
        }
        if (message->getKeyPressed() == "left_mouse_button_pressed") {
            auto mousePos = message->getMousePosition();
            if (currentObjectSelected != "Banner") {
                addObject(currentObjectSelected, terrain->getTerrainPoint(mousePos));
            } 
            if (currentObjectSelected == "Banner") {
                if (!obstaclePlaceBegin) {
                    obstaclePlaceBegin = true;
                    lastObstaclePoint = glm::fvec2(terrain->getTerrainPoint(mousePos).x,
                                                   terrain->getTerrainPoint(mousePos).y);
                } else {
                    currentObstacle = aiContainer->createObstacle(nullptr);
                    currentObstacle->positionA = lastObstaclePoint;
                    currentObstacle->positionB = glm::fvec2(terrain->getTerrainPoint(mousePos).x,
                                                            terrain->getTerrainPoint(mousePos).y);
                    aiContainer->calcBlockNodes(currentObstacle);
                    currentObstacle->createRenderWall();
                    lastObstaclePoint = glm::fvec2(terrain->getTerrainPoint(mousePos).x,
                                                   terrain->getTerrainPoint(mousePos).y);
                }
            }
        }
    }
}