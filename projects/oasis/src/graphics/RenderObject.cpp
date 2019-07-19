#include "../Precompiled.h"

RenderObject::RenderObject() {
    
}

RenderObject::~RenderObject() {
    delete mesh;
}

void RenderObject::init(std::shared_ptr<Renderer> renderer, std::shared_ptr<IniFile> cfg, std::string objectName) {
    mRenderer = renderer;
    mesh = new Mesh();
    
    name = objectName;
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
    GLuint bindingPoint = 0;
    glCameraMatricesUbo = renderer->createUbo(glProgram, "cameraMatrices", sizeof(glm::mat4) * 2, bindingPoint);
    
    glVbo = renderer->createVbo(mesh->getVertexBufferData(), mesh->getVertexBufferSize());
    glVao = renderer->createVao(glVbo, 3, 3, 2, 0, 0, sizeof(float));
    
    renderer->updateView(glCameraMatricesUbo);
    
    initialized = true;
}

void RenderObject::destroy() {
    mRenderer->unloadTexture(glTexture);
    mRenderer->destroyBuffer("RenderObject glCameraMatricesUbo " + name, glCameraMatricesUbo);
    mRenderer->destroyProgram(glProgram);
    mRenderer->destroyVertexArray(glVao);
    mRenderer->destroyBuffer("RenderObject glVbo " + name, glVbo);
    
    initialized = false;
}

void RenderObject::render(glm::fmat4 orientationMatrix) {
    if (initialized == false) {
        return;
    }
    glUseProgram(glProgram);
       
    mRenderer->updateView(glCameraMatricesUbo);
    glUniformMatrix4fv(glModelMatrixUniform, 1, GL_FALSE, glm::value_ptr(orientationMatrix));
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
}

