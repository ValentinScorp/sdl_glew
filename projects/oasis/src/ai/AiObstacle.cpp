#include "../Precompiled.h"

AiObstacle::AiObstacle() {

}

AiObstacle::~AiObstacle() {
}

void AiObstacle::init(Camera* cam, Pathfinder *pf, std::shared_ptr<Renderer> r, glm::fvec2 pos, float rad) {
    camera = cam;
    pathfinder = pf;
    renderer = r;
    
    renderObject = renderer->getRenderObject("Banner");
    position = pos;
    radius = rad;
}

void AiObstacle::destroy() {
}

void AiObstacle::render() {
    
    
    glm::fvec3 pos(position.x, position.y, 0.0f);
    glm::fvec3 direction(0.0f, 1.0f, 0.0f);
    glm::fvec3 upVector(glm::fvec3(0.0f, 0.0f, 1.0f));
    
    glm::fmat4 rotationMatrix = glm::inverse(glm::lookAt(glm::fvec3(0.0f), glm::normalize(direction), upVector));
    
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-90.0f), glm::fvec3(0.0f, 1.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-90.0f), glm::fvec3(1.0f, 0.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-90.0f), glm::fvec3(0.0f, 0.0f, 1.0f));
    
    glm::fmat4 finalMatrix = glm::translate(glm::mat4(1.0f), pos) * rotationMatrix;

    renderObject->mesh->updateAnimation(nullptr, 0, 0, 1.0f);
    
    renderObject->render(finalMatrix);
}
