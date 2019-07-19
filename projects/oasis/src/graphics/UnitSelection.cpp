#include "../Precompiled.h"

UnitSelection::UnitSelection()
{
}

UnitSelection::~UnitSelection()
{
}

void UnitSelection::init(std::shared_ptr<Renderer> renderer, float size) {
    mRenderer = renderer;
    
    float hw = size / 2.0f;

	vertices.push_back(Vertex(glm::fvec3( -hw, -hw, 0.0f ), glm::fvec2(0.0f, 0.0f)));
	vertices.push_back(Vertex(glm::fvec3( hw, hw, 0.0f ), glm::fvec2(1.0f, 1.0f)));
	vertices.push_back(Vertex(glm::fvec3( -hw, hw, 0.0f ), glm::fvec2(0.0f, 1.0f)));

	vertices.push_back(Vertex(glm::fvec3( -hw, -hw, 0.0f ), glm::fvec2(0.0f, 0.0f)));
	vertices.push_back(Vertex(glm::fvec3( hw, -hw, 0.0f ), glm::fvec2(1.0f, 0.0f)));
	vertices.push_back(Vertex(glm::fvec3( hw, hw, 0.0f ), glm::fvec2(1.0f, 1.0f)));
    
    glTexture = mRenderer->loadTexture("img/selectionCircle.png");
    glProgram = mRenderer->createProgram("data/selectionCircle.vert", "data/selectionCircle.frag");
    
    glModelMatrixUniform = mRenderer->getParamFromProgram(glProgram, "modelMatrix");   
    glGetUniformLocation(glProgram, "modelMatrix"); 
    
    GLuint bindingPoint = 0;
    glCameraMatricesUbo = mRenderer->createUbo(glProgram, "cameraMatrices", sizeof(glm::mat4) * 2, bindingPoint);
 
    glVbo = mRenderer->createVbo(vertices.data(), vertices.size() * sizeof(Vertex));
    glVao = mRenderer->createVao(glVbo, 3, 2, 0, 0, 0, sizeof(float));
        
    mRenderer->updateView(glCameraMatricesUbo);
}

void UnitSelection::render() {
    orientationMatrix = glm::translate(glm::mat4(1.0f), position);
    
    glUseProgram(glProgram);
    mRenderer->updateView(glCameraMatricesUbo);
    glUniformMatrix4fv(glModelMatrixUniform, 1, GL_FALSE, glm::value_ptr(orientationMatrix));
    glBindBuffer(GL_ARRAY_BUFFER, glVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(glVao);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glUniform1i(glGetUniformLocation(glProgram, "texture0"), 0);
    
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
    glUseProgram(0);
}

void UnitSelection::destroy() {
    mRenderer->unloadTexture(glTexture);
    mRenderer->destroyBuffer("UnitSelection glCameraMatricesUbo", glCameraMatricesUbo);
    mRenderer->destroyProgram(glProgram);
    mRenderer->destroyVertexArray(glVao);
    mRenderer->destroyBuffer("UnitSelection glVbo", glVbo);
}