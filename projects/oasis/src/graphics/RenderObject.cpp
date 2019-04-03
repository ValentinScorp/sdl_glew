#include "RenderObject.h"

RenderObject::RenderObject() {
}

RenderObject::~RenderObject() {
    delete model;
}

void RenderObject::init(Renderer *renderer, Configuration *cfg, std::string objectName) {
    model = new Model();
    
    glTexture = renderer->loadTexture("img/" + cfg->getParameter(objectName, "texture").value);
    model->loadSmaMesh("model/" + cfg->getParameter(objectName, "meshFile").value);
    
    glProgram = renderer->createProgram("data/" + cfg->getParameter(objectName, "vertexShader").value, 
                                        "data/" + cfg->getParameter(objectName, "fragmentShader").value);
    
    glModelMatrixUniform = renderer->getParamFromProgram(glProgram, "modelMatrix");
    glCameraMatricesUbo = renderer->createUbo(glProgram, "cameraMatrices", sizeof(glm::mat4) * 2);
    
    glVbo = renderer->createVbo(model->getVertexBufferData(), model->getVertexBufferSize());
    glVao = renderer->createVao(glVbo, 3, 3, 2, sizeof(float));
    
    renderer->updateView(glCameraMatricesUbo);
}

void RenderObject::setOrientation(glm::fvec3 p, glm::fvec3 r) {
    orientationMatrix = glm::translate(glm::mat4(1.0f), p);
    orientationMatrix = glm::rotate(orientationMatrix, 0.0f, r);
}

void RenderObject::update(float time) {
    model->update(time);
}

void RenderObject::render() {
    glUseProgram(glProgram);
    glUniformMatrix4fv(glModelMatrixUniform, 1, GL_FALSE, glm::value_ptr(orientationMatrix));
    glBindBuffer(GL_ARRAY_BUFFER, glVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, model->getVertexBufferSize(), model->getVertexBufferData());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(glVao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glDrawArrays(GL_TRIANGLES, 0, model->smaVerts.size());
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
    glUseProgram(0);
}
