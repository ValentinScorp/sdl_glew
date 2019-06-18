#include "../Precompiled.h"

AiObstacle::AiObstacle() {

}

AiObstacle::~AiObstacle() {
}

void AiObstacle::placeLink(glm::fvec2 pos) {
    positions.push_back(pos);
}

void AiObstacle::init(Camera* cam, Pathfinder *pf, WorldObject* wo) {
    camera = cam;
    pathfinder = pf;
    worldObject = wo;
}