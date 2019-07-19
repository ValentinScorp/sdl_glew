#include "../Precompiled.h"

AiObstacle::AiObstacle() {

}

AiObstacle::~AiObstacle() {
}

void AiObstacle::init(Camera* cam, Pathfinder *pf, WorldObject* wo, std::shared_ptr<Renderer> r) {
    camera = cam;
    pathfinder = pf;
    worldObject = wo;
    renderer = r;
}

void AiObstacle::destroy() {
    if (renderLine) {
        renderLine->destroy();
        delete renderLine;
        renderLine = nullptr;
    }
}

void AiObstacle::render() {
    if (renderLine) {
        renderLine->renderAt();
    }
}

void AiObstacle::createRenderWall() {
    renderLine = new RenderLine();
    renderLine->initWall(renderer, 
                         glm::fvec3(positionA.x, positionA.y, 0.0f), 
                         glm::fvec3(positionB.x, positionB.y, 0.0f), 
                         5.0f);
}