#include "../Precompiled.h"

AiAgent::AiAgent() {
    location = glm::fvec3(0.0f);
    velocity = glm::fvec3(0.0f);
    acceleration = glm::fvec3(0.0f);
}

AiAgent::~AiAgent()
{
}

void AiAgent::init(Camera* cam, Pathfinder *pf, WorldObject* wo){
    camera = cam;
    pathfinder = pf;
    worldObject = wo;
    
    position = glm::fvec2(0.0f, 0.0f);
   //position = glm::fvec3(0.0f, 0.0f, 0.0f);
    //upVector = glm::fvec3(0.0f, 0.0f, 1.0f);
   // rotationMatrix = glm::fmat4(1.0f);
    
   // setPosition(position);
    //makeFinalMatrix();
}

void AiAgent::createSelectionBox(float unitWidth, float unitHeight) {
    float hw = unitWidth / 2;
    aux::vertex bot_a(glm::fvec3(-hw, -hw, 0.0f), glm::fvec3(0.0f, 0.0f, 0.0f));
    aux::vertex bot_b(glm::fvec3( hw, -hw, 0.0f), glm::fvec3(0.0f, 0.0f, 0.0f));
    aux::vertex bot_c(glm::fvec3( hw,  hw, 0.0f), glm::fvec3(0.0f, 0.0f, 0.0f));
    aux::vertex bot_d(glm::fvec3(-hw,  hw, 0.0f), glm::fvec3(0.0f, 0.0f, 0.0f));
    
    aux::vertex top_a(glm::fvec3(-hw, -hw, 0.0f + unitHeight), glm::fvec3(0.0f, 0.0f, 0.0f));
    aux::vertex top_b(glm::fvec3( hw, -hw, 0.0f + unitHeight), glm::fvec3(0.0f, 0.0f, 0.0f));
    aux::vertex top_c(glm::fvec3( hw,  hw, 0.0f + unitHeight), glm::fvec3(0.0f, 0.0f, 0.0f));
    aux::vertex top_d(glm::fvec3(-hw,  hw, 0.0f + unitHeight), glm::fvec3(0.0f, 0.0f, 0.0f));
    
    selectionBox.vertices.push_back(bot_a); // 0
    selectionBox.vertices.push_back(bot_b); // 1
    selectionBox.vertices.push_back(bot_c); // 2
    selectionBox.vertices.push_back(bot_d); // 3
    
    selectionBox.vertices.push_back(top_a); // 4
    selectionBox.vertices.push_back(top_b); // 5
    selectionBox.vertices.push_back(top_c); // 6
    selectionBox.vertices.push_back(top_d); // 7
    
    selectionBox.createTriangle(0, 1, 2);
    selectionBox.createTriangle(0, 2, 3);
    selectionBox.createTriangle(4, 5, 6);
    selectionBox.createTriangle(4, 6, 7);
    
    selectionBox.createTriangle(0, 1, 4);
    selectionBox.createTriangle(0, 4, 7);
    selectionBox.createTriangle(2, 3, 7);
    selectionBox.createTriangle(2, 7, 6);
    
    selectionBox.createTriangle(1, 2, 6);
    selectionBox.createTriangle(1, 6, 5);
    selectionBox.createTriangle(3, 0, 4);
    selectionBox.createTriangle(3, 4, 7);
}

bool AiAgent::lineIntersetsCircle(glm::fvec3 p1, glm::fvec3 p2, glm::fvec3 center, float radius) {
    return (glm::length(center - p1) <= radius) || (glm::length(center - p2) <= radius);
}
/*
void AiAgent::avoidObstacle(AiAgent *obstacle) {
    const float MAX_SEE_AHEAD = collisionRadius;
    glm::fvec3 ahead = position + glm::normalize(movementDirection) * movementSpeed * MAX_SEE_AHEAD;
    glm::fvec3 ahead2 = position + glm::normalize(movementDirection) * movementSpeed * MAX_SEE_AHEAD * 0.5f;
    
    glm::fvec3 avoidance_force(0.0f, 0.0f, 0.0f);
            
    const float MAX_AVOID_FORCE = movementSpeed * 1.0;
    if (lineIntersetsCircle(ahead, ahead2, obstacle->position, obstacle->collisionRadius)) {
        avoidance_force = glm::normalize(ahead - obstacle->position);
    }
            
    float dotSpeed = glm::abs(glm::dot(avoidance_force, movementDirection));
    
    glm::fvec3 newDirection = glm::normalize(avoidance_force * dotSpeed + movementDirection);
    rotateToward(position + newDirection * movementSpeed);
    
    setPosition(position + newDirection * movementSpeed);
    movementDirection = glm::normalize(movementTarget - position);
}
*/

void AiAgent::tryMove(glm::fvec2 newPos) {
    pathfinder->removeStaticObstacle(position);
    if (pathfinder->isObstacle(newPos) == false) {
        pathfinder->setStaticObstacle(newPos);
        setPosition(newPos);
        
        float distance = glm::length(movementPath[currentPath + 1] - newPos);
      
        if (glm::abs(glm::length(movementPath[currentPath + 1] - newPos) <= movementSpeed)) {
            currentPath++;
            if (currentPath >= (movementPath.size() - 1)) {
                moving = false;
                worldObject->stopAnimation();
            }
        }
    } else {
        pathfinder->setStaticObstacle(position);
        moving = false;
        currentPath = 0;
        
        std::cout << "obstacle found at " << newPos.x << " x " << newPos.y << std::endl;
        std::cout << "path is: \n";
        for (auto& p: movementPath) {
            std::cout << p.x << " x " << p.y << std::endl;
        }
            
        movementPath.clear();
        auto index = pathfinder->getNodeIndex(newPos);
        worldObject->stopAnimation();
    }
}

void AiAgent::move() {
    if (movementPath.size() > 1) {
        location = position;
        
        glm::fvec2 desiredVector = glm::normalize(movementPath[1] - location);
        desiredVector *= MAX_SPEED;
        glm::fvec2 steerForce = desiredVector - velocity;
        if (steerForce.x > MAX_FORCE) {
            steerForce.x = MAX_FORCE;
        }
        if (steerForce.y > MAX_FORCE) {
            steerForce.y = MAX_FORCE;
        }

        acceleration += steerForce;
                
        velocity += acceleration;
        if (velocity.x > MAX_SPEED) {
            velocity.x = MAX_SPEED;
        }
        if (velocity.y > MAX_SPEED) {
            velocity.y = MAX_SPEED;
        }

        location += velocity;
        acceleration = glm::fvec2(0.0f, 0.0f);
        
        position = location;
    }
    /*
    if (currentPath < (movementPath.size() - 1)) {
        if (position != movementPath[currentPath + 1]) {
            movementDirection = glm::normalize(movementPath[currentPath + 1] - position);
        }
        glm::fvec2 newPosition = position + movementDirection * movementSpeed;
        
        tryMove(newPosition);
    }*/
}

void AiAgent::updateColisions(AiAgent *obstacle) {
    if (moving) {
        //avoidObstacle(obstacle);
    }
}
void AiAgent::update() {
    
    if (moving) {
        move();
    }
    pathfinder->setStaticObstacle(position);
}

void AiAgent::setPosition(glm::fvec2 pos) {
    position = pos;
    selectionBox.position = glm::fvec3(pos.x, pos.y, 0.0); // todo send this to WorldObject class
}

void AiAgent::setObstacleOnAiMap() {
    pathfinder->setStaticObstacle(position);
}

void AiAgent::createPath(glm::fvec2 destination) {
    movementPath.clear();
    movementPath.push_back(position);
    movementPath.push_back(destination);
    //pathfinder->removeStaticObstacle(position);
    //pathfinder->getPath(position, glm::fvec2(destination.x, destination.y), movementPath);
    //pathfinder->setStaticObstacle(position);
}

void AiAgent::startMove(glm::fvec3 destination) {
    currentPath = 0;
    createPath(destination);

    if(movementPath.size() > 1) {
        currentPath = 0;
        movementTarget = movementPath[1];
        moving = true;
        worldObject->beginAnimation("Walk");
        if (movementTarget != position)
            movementDirection = glm::normalize(movementTarget - position);
    }
}

void AiAgent::onMessage(IMessage *message) {
    if (message == nullptr) {
        return;
    }
    if (message->getKeyPressed() == "e") {
    }
    if (message->getKeyPressed() == "a") {
    }
    if (message->getKeyPressed() == "s") {
    }
    if (message->getKeyPressed() == "left_mouse_button_pressed") {
        if (selectable && message && camera) {
            glm::fvec2 pos = message->getMousePosition();
            RayVector camRay = camera->getVectorRay(pos.x, pos.y);
            aux::ray ray;
            ray.begin = camRay.begin;
            ray.end = camRay.end;
            selected = selectionBox.isIntersected(ray);
        }
    }
    if (message->getMessage() == "unit_walk") {
        if (selected) {
            startMove(message->getPosition());
        }
    }
}