#include "../Precompiled.h"

WorldObject::WorldObject() {
    
}

WorldObject::~WorldObject() {
    
}

bool WorldObject::init( std::string name, 
                        std::shared_ptr<Renderer> renderer, 
                        std::shared_ptr<AiContainer> aiContainer, 
                        std::shared_ptr<IniFile> iniFile, 
                        glm::fvec3 position) {
    
    if (iniFile->isSectionExist(name)) {
        renderObject = renderer->getRenderObject(name);
        if (renderObject == nullptr) {
            return false;
        }
        auto id = aiContainer->createAgent();
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
        unitSelection->position = agent->position;
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
    if (renderObject)
        renderObject->update(currentAnimation, currentFrame, animCounter, 1.0f);
    if (unitSelection) {
        unitSelection->position = aiAgent->position;
        unitSelection->position.z += 0.1;
    }
}

void WorldObject::render() {
    aiAgent->makeFinalMatrix();
    renderObject->render(aiAgent->orientationMatrix);    
        
    if (aiAgent->selected) {
        unitSelection->render();
    }
}

void WorldObject::startAnimation(std::string animName) {
    currentAnimation = renderObject->getAnimation(animName);
}

void WorldObject::stopAnimation() {
    currentAnimation = 0;
}

void WorldObject::onMessage(IMessage *message) {
    
}

