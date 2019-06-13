#include "../Precompiled.h"

AiObstacle::AiObstacle() {

}

AiObstacle::~AiObstacle()
{
}

void AiObstacle::init(Camera* cam, Pathfinder *pf, WorldObject* wo) {
    camera = cam;
    pathfinder = pf;
    worldObject = wo;
    
    position = glm::fvec2(0.0f, 0.0f);
}