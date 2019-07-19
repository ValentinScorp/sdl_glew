#include "../Precompiled.h"

RenderQuad::RenderQuad() {
}


RenderQuad::~RenderQuad() {
}

void RenderQuad::init(std::shared_ptr<Renderer> renderer, float width) {
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
    GLuint bindingPoint = 0;
    glCameraMatricesUbo = renderer->createUbo(glProgram, "cameraMatrices", sizeof(glm::mat4) * 2, bindingPoint);
    glVbo = renderer->createVbo(vertexes.data(), vertexes.size() * sizeof(Vertex));
    glVao = renderer->createVao(glVbo, 3, 0, 0, 0, 0, sizeof(float));
    
    renderer->updateView(glCameraMatricesUbo);
}

void RenderQuad::destroy() {
    renderer->destroyBuffer("RenderQuad glCameraMatricesUbo", glCameraMatricesUbo);
    renderer->destroyProgram(glProgram);
    renderer->destroyVertexArray(glVao);
    renderer->destroyBuffer("RenderQuad glVbo", glVbo);
}

void RenderQuad::renderAt(glm::fvec3 position) {
    orientationMatrix = glm::translate(glm::mat4(1.0f), position);
    
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
