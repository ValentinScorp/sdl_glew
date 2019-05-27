#include "../Precompiled.h"

WorldObject::WorldObject() {
    
}

WorldObject::~WorldObject() {
    
}

bool WorldObject::init( std::string name, 
                        World *w,
                        std::shared_ptr<Renderer> renderer, 
                        std::shared_ptr<AiContainer> aiContainer, 
                        std::shared_ptr<IniFile> iniFile, 
                        glm::fvec3 position) {
    
    if (iniFile->isSectionExist(name)) {
        world = w;
        renderObject = renderer->getRenderObject(name);
        if (renderObject == nullptr) {
            return false;
        }
        auto id = aiContainer->createAgent(this);
        auto agent = aiContainer->getAgent(id);
        agent->collisionRadius = iniFile->getParameter(name, "collisionRadius").toFloat();
        agent->selectable = iniFile->getParameter(name, "selectable").toInt();
        agent->setPosition(position);
        if (iniFile->getParameter(name, "staticPosition").toInt()) {
            agent->setObstacleOnAiMap();
        }
        
        float unitWidth = 4.0;
        float unitHeight = 12.0;
        
        unitSelection = std::make_shared<UnitSelection>();
        unitSelection->init(renderer, unitWidth);
        unitSelection->position.x = agent->position.x;
        unitSelection->position.y = agent->position.y;
        unitSelection->position.z += 0.1;
            
        agent->createSelectionBox(unitWidth, unitHeight);
        aiAgent = agent;
    
        return true;
    }
    
    return false;
}

void WorldObject::destroy() {
    if (unitSelection)
        unitSelection->destroy();
     
    if (renderObject)
        renderObject->destroy();
}

void WorldObject::update() {
    if (unitSelection) {
        unitSelection->position.x = aiAgent->position.x;
        unitSelection->position.y = aiAgent->position.y;
        unitSelection->position.z = 0.1;
    }
}

glm::fmat4 WorldObject::getRotationMatrix(glm::fvec3 direction, glm::fvec3 upVector) {
    return glm::inverse(glm::lookAt(glm::fvec3(0.0f), glm::normalize(direction), upVector)); // LH version
}

void WorldObject::adjustRotation(glm::fmat4 &rotationMatrix) {
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-90.0f), glm::fvec3(0.0f, 1.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-90.0f), glm::fvec3(1.0f, 0.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-90.0f), glm::fvec3(0.0f, 0.0f, 1.0f));
}

glm::fmat4 WorldObject::makeFinalMatrix(glm::fvec3 position, glm::fmat4 rotationMatrix) {
    return glm::translate(glm::mat4(1.0f), position) * rotationMatrix;
}

void WorldObject::render() {
    glm::fvec3 unitWorldPosition(aiAgent->position.x, 
                                 aiAgent->position.y,
                                 world->terrain->getHeight(glm::fvec2(aiAgent->position.x, aiAgent->position.y)));
    glm::fvec3 unitWorldDirection(aiAgent->movementDirection.x,
                                  aiAgent->movementDirection.y,
                                  0.0f);
    
    glm::fmat4 rotationMatrix = getRotationMatrix(unitWorldDirection, glm::fvec3(0.0f, 0.0f, 1.0f));
    adjustRotation(rotationMatrix);
    glm::fmat4 finalMatrix = makeFinalMatrix(unitWorldPosition, rotationMatrix);
    renderObject->mesh->updateAnimation(currentAnimation, &currentFrame, &animCounter, 1.0f);
    renderObject->render(finalMatrix);
    
    if (aiAgent->selected) {
        unitSelection->render();
    }
}

void WorldObject::beginAnimation(std::string animName) {
    currentAnimation = renderObject->mesh->getAnimation(animName);
}

void WorldObject::stopAnimation() {
    currentAnimation = nullptr;
}

void WorldObject::onMessage(IMessage *message) {
    
}

