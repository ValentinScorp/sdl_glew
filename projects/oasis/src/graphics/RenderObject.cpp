#include "../Precompiled.h"

RenderObject::RenderObject() {
    
}

RenderObject::~RenderObject() {
    delete mesh;
}

void RenderObject::init(std::shared_ptr<Renderer> renderer, Configuration *cfg, std::string objectName, AiAgent *agent) {
    mRenderer = renderer;
    aiAgent = agent;
    mesh = new Mesh();
    
    std::string fileFormat = cfg->getParameter(objectName, "fileFormat").value;
    if (fileFormat == "sma") {
        glTexture = renderer->loadTexture("img/" + cfg->getParameter(objectName, "texture").value);
        mesh->loadSmaMesh("model/" + cfg->getParameter(objectName, "meshFile").value);
    }
    if (fileFormat == "obj") {
        glTexture = renderer->loadTexture("img/" + cfg->getParameter(objectName, "texture").value, true);
        mesh->loadObjMesh("model/" + cfg->getParameter(objectName, "meshFile").value);
    }
    
    glProgram = renderer->createProgram("data/" + cfg->getParameter(objectName, "vertexShader").value, 
                                        "data/" + cfg->getParameter(objectName, "fragmentShader").value);
    
    glModelMatrixUniform = renderer->getParamFromProgram(glProgram, "modelMatrix");
    glCameraMatricesUbo = renderer->createUbo(glProgram, "cameraMatrices", sizeof(glm::mat4) * 2);
    
    glVbo = renderer->createVbo(mesh->getVertexBufferData(), mesh->getVertexBufferSize());
    glVao = renderer->createVao(glVbo, 3, 3, 2, 0, 0, sizeof(float));
    
    renderer->updateView(glCameraMatricesUbo);
    
    float unitWidth = 4.0;
    float unitHeight = 12.0;
    
    unitSelection = new UnitSelection();
    unitSelection->init(renderer, unitWidth);
    unitSelection->position = aiAgent->position;
    unitSelection->position.z += 0.1;
        
    aiAgent->createSelectionBox(unitWidth, unitHeight);
}

void RenderObject::destroy() {
    unitSelection->destroy();
    
    mRenderer->unloadTexture(glTexture);
    mRenderer->destroyBuffer(glCameraMatricesUbo);
    mRenderer->destroyProgram(glProgram);
    mRenderer->destroyBuffer(glVao);
    mRenderer->destroyBuffer(glVbo);
}

void RenderObject::update(float time) {
    mesh->update(time);
    unitSelection->position = aiAgent->position;
    unitSelection->position.z += 0.1;
}

void RenderObject::render() {
    glUseProgram(glProgram);
    
    mRenderer->updateView(glCameraMatricesUbo);
    glUniformMatrix4fv(glModelMatrixUniform, 1, GL_FALSE, glm::value_ptr(aiAgent->orientationMatrix));
    glBindBuffer(GL_ARRAY_BUFFER, glVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->getVertexBufferSize(), mesh->getVertexBufferAnimData());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(glVao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glUniform1i(glGetUniformLocation(glProgram, "colorTexture"), 0);
    
    glDrawArrays(GL_TRIANGLES, 0, mesh->vertexes.size());
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
    glUseProgram(0);
        
    if (aiAgent->selected) {
        unitSelection->render();
    }
}

