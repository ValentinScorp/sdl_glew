#include "../Precompiled.h"

RenderLine::RenderLine() {
}


RenderLine::~RenderLine() {
    if (initialized == true) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Forgot to destroy RenderLine\n");
    }
}

void RenderLine::init(std::shared_ptr<Renderer> renderer, glm::fvec3 pointA, glm::fvec3 pointB, float width) {
    if (initialized == true) {
        return;
    }
    this->renderer = renderer;
    
    float hw = width / 2.0f;
    
    glm::fvec3 vec1 = pointB - pointA;
    glm::fvec3 vec2 = pointA - pointB;
    
    glm::fvec3 point1 = pointA + glm::normalize(glm::cross(vec1, glm::fvec3(0.0f, 0.0f, 1.0f))) * hw;
    glm::fvec3 point2 = pointA + glm::normalize(glm::cross(glm::fvec3(0.0f, 0.0f, 1.0f), vec1)) * hw;
    
    glm::fvec3 point3 = pointB + glm::normalize(glm::cross(glm::fvec3(0.0f, 0.0f, 1.0f), vec2)) * hw;
    glm::fvec3 point4 = pointB + glm::normalize(glm::cross(vec2, glm::fvec3(0.0f, 0.0f, 1.0f))) * hw;

	vertexes.push_back(Vertex(point2));
	vertexes.push_back(Vertex(point3));
	vertexes.push_back(Vertex(point4));

	vertexes.push_back(Vertex(point2));
	vertexes.push_back(Vertex(point1));
	vertexes.push_back(Vertex(point3));
    
    glProgram = renderer->createProgram("data/terrainBrush.vert", "data/terrainBrush.frag");
    glModelMatrixUniform = renderer->getParamFromProgram(glProgram, "modelMatrix");
    GLuint bindingPoint = 0;
    glCameraMatricesUbo = renderer->createUbo(glProgram, "cameraMatrices", sizeof(glm::mat4) * 2, bindingPoint);
    glVbo = renderer->createVbo(vertexes.data(), vertexes.size() * sizeof(Vertex));
    glVao = renderer->createVao(glVbo, 3, 0, 0, 0, 0, sizeof(float));
    
    renderer->updateView(glCameraMatricesUbo);
    
    initialized = true;
}

void RenderLine::initWall(std::shared_ptr<Renderer> renderer, glm::fvec3 pointA, glm::fvec3 pointB, float height) {
    if (initialized == true) {
        return;
    }
    this->renderer = renderer;
        
    glm::fvec3 point1 = pointA;
    glm::fvec3 point2 = pointB;
    
    glm::fvec3 point3 = pointA + glm::fvec3(0.0f, 0.0f, 1.0f) * height;
    glm::fvec3 point4 = pointB + glm::fvec3(0.0f, 0.0f, 1.0f) * height;

	vertexes.push_back(Vertex(point2));
	vertexes.push_back(Vertex(point3));
	vertexes.push_back(Vertex(point4));

	vertexes.push_back(Vertex(point2));
	vertexes.push_back(Vertex(point1));
	vertexes.push_back(Vertex(point3));
    
    vertexes.push_back(Vertex(point2));
	vertexes.push_back(Vertex(point4));
	vertexes.push_back(Vertex(point3));

	vertexes.push_back(Vertex(point2));
	vertexes.push_back(Vertex(point3));
	vertexes.push_back(Vertex(point1));
    
    glProgram = renderer->createProgram("data/terrainBrush.vert", "data/terrainBrush.frag");
    glModelMatrixUniform = renderer->getParamFromProgram(glProgram, "modelMatrix");
    GLuint bindingPoint = 0;
    glCameraMatricesUbo = renderer->createUbo(glProgram, "cameraMatrices", sizeof(glm::mat4) * 2, bindingPoint);
    glVbo = renderer->createVbo(vertexes.data(), vertexes.size() * sizeof(Vertex));
    glVao = renderer->createVao(glVbo, 3, 0, 0, 0, 0, sizeof(float));
    
    renderer->updateView(glCameraMatricesUbo);
    
    initialized = true;
}

void RenderLine::destroy() {
    if (initialized == false) {
        return;
    }
    renderer->destroyBuffer("RenderLine glCameraMatricesUbo", glCameraMatricesUbo);
    renderer->destroyProgram(glProgram);
    renderer->destroyVertexArray(glVao);
    renderer->destroyBuffer("RenderLine glVbo", glVbo);
    
    glProgram = 0;
    glCameraMatricesUbo = 0;
    glModelMatrixUniform = 0;
    glVbo = 0;
    glVao = 0;
    
    initialized = false;
}

void RenderLine::renderAt(glm::fvec3 position) {
    if (initialized == false) {
        return;
    }
    
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
