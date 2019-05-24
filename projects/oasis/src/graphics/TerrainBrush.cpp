#include "../Precompiled.h"

TerrainBrush::TerrainBrush() {
}


TerrainBrush::~TerrainBrush() {
}

void TerrainBrush::init(std::shared_ptr<Renderer> renderer, float width) {
    this->width = width;
    
    renderQuad = new RenderQuad;
    renderQuad->init(renderer, width);
}

void TerrainBrush::destroy() {
    renderQuad->destroy();
    delete renderQuad;
}

float TerrainBrush::GetWidth() {
	return width;
}

float TerrainBrush::GetX() {
	return posX;
}

float TerrainBrush::GetY() {
	return posY;
}

void TerrainBrush::setX(float x) {
	int w = width;
	int newx = (((int)x + w / 2) / w) * w;

	posX = newx;
}

void TerrainBrush::setY(float y) {
	int w = width;
	int newy = (((int)y + w / 2) / w) * w;
	posY = newy;
}

void TerrainBrush::render() {
    renderQuad->renderAt(glm::fvec3(posX, posY, posZ));
}

void TerrainBrush::onMessage(IMessage *message) {
    if (message == nullptr) {
        return;
    }
    if (message->getKeyPressed() == "motion") {
        
    }
    if (message->getKeyPressed() == "a") {
    }
    if (message->getKeyPressed() == "s") {
    }
}
