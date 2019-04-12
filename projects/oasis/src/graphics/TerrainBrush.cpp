#include "../Precompiled.h"

TerrainBrush::TerrainBrush() {
}


TerrainBrush::~TerrainBrush() {
}

void TerrainBrush::init(std::shared_ptr<Renderer> renderer) {
    this->renderer = renderer;
    
    float hw = width / 2.0f;

	vertexes.push_back(Vertex(glm::fvec3( -hw, -hw, 0.0f )));
	vertexes.push_back(Vertex(glm::fvec3( hw, hw, 0.0f )));
	vertexes.push_back(Vertex(glm::fvec3( -hw, hw, 0.0f )));

	vertexes.push_back(Vertex(glm::fvec3( -hw, -hw, 0.0f )));
	vertexes.push_back(Vertex(glm::fvec3( hw, -hw, 0.0f )));
	vertexes.push_back(Vertex(glm::fvec3( hw, hw, 0.0f )));
    
    glProgram = renderer->createProgram("data/terrainBrush.vert", "data/terrainBrush.frag");
    glModelMatrixUniform = renderer->getParamFromProgram(glProgram, "modelMatrix");
    glCameraMatricesUbo = renderer->createUbo(glProgram, "cameraMatrices", sizeof(glm::mat4) * 2);
    glVbo = renderer->createVbo(vertexes.data(), vertexes.size() * sizeof(Vertex));
    glVao = renderer->createVao(glVbo, 3, 0, 0, 0, 0, sizeof(float));
    renderer->updateView(glCameraMatricesUbo);
}

void TerrainBrush::destroy() {
    renderer->destroyBuffer(glCameraMatricesUbo);
    renderer->destroyProgram(glProgram);
    renderer->destroyBuffer(glVao);
    renderer->destroyBuffer(glVbo);
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
    orientationMatrix = glm::translate(glm::mat4(1.0f), glm::fvec3(posX, posY, posZ));
    //orientationMatrix = glm::rotate(orientationMatrix, 0.0f, glm::fvec3(0, 0, 0));
    
    glUseProgram(glProgram);
    renderer->updateView(glCameraMatricesUbo);
    glUniformMatrix4fv(glModelMatrixUniform, 1, GL_FALSE, glm::value_ptr(orientationMatrix));
    glBindBuffer(GL_ARRAY_BUFFER, glVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexes.size() * sizeof(Vertex), vertexes.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(glVao);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, vertexes.size());
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void TerrainBrush::onMessage(IMessage *message) {
    if (message->getKeyPressed() == "motion") {
        glm::fvec2 pos = message->getMousePosition();
        RayVector camRay = renderer->camera->getVectorRay(pos.x, pos.y);
		glm::fvec3 intersection = renderer->terrain->getTerrainIntersection(camRay);
        setX(intersection.x);
		setY(intersection.y);
    }
    if (message->getKeyPressed() == "a") {
        
    }
    if (message->getKeyPressed() == "s") {
        
    }
}
