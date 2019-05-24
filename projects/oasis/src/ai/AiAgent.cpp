#include "../Precompiled.h"

AiAgent::AiAgent() {
    
}

AiAgent::~AiAgent()
{
}

void AiAgent::init(Camera* cam, Pathfinder *pf){
    camera = cam;
    pathfinder = pf;
    
    position = glm::fvec3(0.0f, 0.0f, 0.0f);
    upVector = glm::fvec3(0.0f, 0.0f, 1.0f);
    rotationMatrix = glm::fmat4(1.0f);
    
    setPosition(position);
    makeFinalMatrix();
}

void AiAgent::createSelectionBox(float unitWidth, float unitHeight) {
    float hw = unitWidth / 2;
    aux::vertex bot_a(glm::fvec3(-hw, -hw, position.z), glm::fvec3(0.0f, 0.0f, 0.0f));
    aux::vertex bot_b(glm::fvec3( hw, -hw, position.z), glm::fvec3(0.0f, 0.0f, 0.0f));
    aux::vertex bot_c(glm::fvec3( hw,  hw, position.z), glm::fvec3(0.0f, 0.0f, 0.0f));
    aux::vertex bot_d(glm::fvec3(-hw,  hw, position.z), glm::fvec3(0.0f, 0.0f, 0.0f));
    
    aux::vertex top_a(glm::fvec3(-hw, -hw, position.z + unitHeight), glm::fvec3(0.0f, 0.0f, 0.0f));
    aux::vertex top_b(glm::fvec3( hw, -hw, position.z + unitHeight), glm::fvec3(0.0f, 0.0f, 0.0f));
    aux::vertex top_c(glm::fvec3( hw,  hw, position.z + unitHeight), glm::fvec3(0.0f, 0.0f, 0.0f));
    aux::vertex top_d(glm::fvec3(-hw,  hw, position.z + unitHeight), glm::fvec3(0.0f, 0.0f, 0.0f));
    
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

void AiAgent::move() {
    if (currentPath < (movementPath.size() - 1)) {
        //std::cout << movementDirection.x << " x " << movementDirection.y << " x " << movementDirection.z << std::endl;
        if (position != movementPath[currentPath + 1])
            movementDirection = glm::normalize(movementPath[currentPath + 1] - position);
        //std::cout << movementDirection.x << " x " << movementDirection.y << " x " << movementDirection.z << std::endl;
        glm::fvec3 newPosition = position + movementDirection * movementSpeed;
        pathfinder->removeStaticObstacle(position);
        if (pathfinder->isObstacle(newPosition) == false) {
            pathfinder->setStaticObstacle(newPosition);
            rotateToward(newPosition);
            setPosition(newPosition);
            adjustRotation();
            
            float distance = glm::length(movementPath[currentPath + 1] - newPosition);
          //  std::cout << distance << std::endl;
            if (glm::abs(glm::length(movementPath[currentPath + 1] - newPosition) <= movementSpeed)) {
                currentPath++;
                //std::cout << "currentPath " << currentPath << std::endl;
                if (currentPath >= (movementPath.size() - 1)) {
                    moving = false;
                    //std::cout << "no path left \n";
                }
            }
            
        } else {
            pathfinder->setStaticObstacle(position);
            moving = false;
            currentPath = 0;
            movementPath.clear();
            std::cout << "obstacle found at " << newPosition.x << " x " << newPosition.y << std::endl;
            auto index = pathfinder->getNodeIndex(newPosition);
         //   std::cout << "index is " << index  << std::endl;
        }
    }
}

void AiAgent::update(AiAgent *obstacle) {
    
    if (moving) {
        //
        //avoidObstacle(obstacle);
        move();
        
        makeFinalMatrix();
    }
    pathfinder->setStaticObstacle(position);
}

void AiAgent::setPosition(glm::fvec3 pos) {
    position = pos;
    selectionBox.position = pos;
}

void AiAgent::setObstacleOnAiMap() {
    pathfinder->setStaticObstacle(position);
}

void AiAgent::rotateToward(glm::fvec3 direction) {
    glm::fvec3 dir = glm::normalize(direction - position);
    rotationMatrix = glm::inverse(glm::lookAt(glm::fvec3(0.0f), dir, upVector)); // LH version
}

void AiAgent::adjustRotation() {
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-90.0f), glm::fvec3(0.0f, 1.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-90.0f), glm::fvec3(1.0f, 0.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-90.0f), glm::fvec3(0.0f, 0.0f, 1.0f));
}

void AiAgent::makeFinalMatrix() {
    orientationMatrix =  glm::translate(glm::mat4(1.0f), position) * rotationMatrix;
}

void AiAgent::onMessage(IMessage *message) {
    if (message == nullptr) {
        return;
    }
    if (message->getKeyPressed() == "e") {
    }
    if (message->getKeyPressed() == "a") {
        //mesh->beginAnimation("Walk");
    }
    if (message->getKeyPressed() == "s") {
        //mesh->stopAnimation();
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
            glm::fvec3 dest = message->getPosition();
            
            currentPath = 0;
            movementPath.clear();
            pathfinder->removeStaticObstacle(position);
            auto t1 = std::chrono::high_resolution_clock::now();
            pathfinder->getPath(position, dest, movementPath);
            auto t2 = std::chrono::high_resolution_clock::now();
           // std::cout << "delta " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << std::endl;
            pathfinder->setStaticObstacle(position);
          //  std::cout << std::endl;
          //  std::cout << "Path found " << std::endl;
           // for (auto &p: movementPath) {
           //     std::cout << p.x << " x " << p.y << " x " << p.z << std::endl;
           // }
            if(movementPath.size() > 1) {
                rotateToward(movementPath[1]);
                adjustRotation();
                makeFinalMatrix();
                
                currentPath = 0;
                movementTarget = movementPath[1];
                moving = true;
                if (movementTarget != position)
                    movementDirection = glm::normalize(movementTarget - position);
            }
        }
    }
}