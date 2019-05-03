#include "../Precompiled.h"

RenderObject::RenderObject() {
    position = glm::fvec3(10.0f, 10.0f, 0.0f);
    upVector = glm::fvec3(0.0f, 0.0f, 1.0f);
    rotationMatrix = glm::fmat4(1.0f);
    
    setPosition(position);
    makeFinalMatrix();
}

RenderObject::~RenderObject() {
    delete mesh;
}

void RenderObject::init(std::shared_ptr<Renderer> renderer, Configuration *cfg, std::string objectName) {
    mRenderer = renderer;
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
    unitSelection->position = position;
    unitSelection->position.z += 0.1;
    
    float hw = unitWidth / 2;
    aux::vertex bot_a(glm::fvec3(-hw, -hw, position.z), glm::fvec3(0.0f, 0.0f, 0.0f));
    aux::vertex bot_b(glm::fvec3( hw, -hw, position.z), glm::fvec3(0.0f, 0.0f, 0.0f));
    aux::vertex bot_c(glm::fvec3( hw,  hw, position.z), glm::fvec3(0.0f, 0.0f, 0.0f));
    aux::vertex bot_d(glm::fvec3(-hw,  hw, position.z), glm::fvec3(0.0f, 0.0f, 0.0f));
    
    aux::vertex top_a(glm::fvec3(-hw, -hw, position.z + unitHeight), glm::fvec3(0.0f, 0.0f, 0.0f));
    aux::vertex top_b(glm::fvec3( hw, -hw, position.z + unitHeight), glm::fvec3(0.0f, 0.0f, 0.0f));
    aux::vertex top_c(glm::fvec3( hw,  hw, position.z + unitHeight), glm::fvec3(0.0f, 0.0f, 0.0f));
    aux::vertex top_d(glm::fvec3(-hw,  hw, position.z + unitHeight), glm::fvec3(0.0f, 0.0f, 0.0f));
    
    selectionBox.vertices.push_back(bot_a); // 0
    selectionBox.vertices.push_back(bot_b); // 1
    selectionBox.vertices.push_back(bot_c); // 2
    selectionBox.vertices.push_back(bot_d); // 3
    
    selectionBox.vertices.push_back(top_a); // 4
    selectionBox.vertices.push_back(top_b); // 5
    selectionBox.vertices.push_back(top_c); // 6
    selectionBox.vertices.push_back(top_d); // 7
    
    selectionBox.createTriangle(0, 1, 2);
    selectionBox.createTriangle(0, 2, 3);
    selectionBox.createTriangle(4, 5, 6);
    selectionBox.createTriangle(4, 6, 7);
    
    selectionBox.createTriangle(0, 1, 4);
    selectionBox.createTriangle(0, 4, 7);
    selectionBox.createTriangle(2, 3, 7);
    selectionBox.createTriangle(2, 7, 6);
    
    selectionBox.createTriangle(1, 2, 6);
    selectionBox.createTriangle(1, 6, 5);
    selectionBox.createTriangle(3, 0, 4);
    selectionBox.createTriangle(3, 4, 7);
}

void RenderObject::destroy() {
    unitSelection->destroy();
    
    mRenderer->unloadTexture(glTexture);
    mRenderer->destroyBuffer(glCameraMatricesUbo);
    mRenderer->destroyProgram(glProgram);
    mRenderer->destroyBuffer(glVao);
    mRenderer->destroyBuffer(glVbo);
}

void RenderObject::setPosition(glm::fvec3 pos) {
    position = pos;
    selectionBox.position = pos;
    if (unitSelection) {
        unitSelection->position = position;
        unitSelection->position.z += 0.1;
    }
}

void RenderObject::rotateToward(glm::fvec3 direction) {
    glm::fvec3 dir = glm::normalize(direction - position);
    rotationMatrix = glm::inverse(glm::lookAt(glm::fvec3(0.0f), dir, upVector)); // LH version
}

void RenderObject::adjustRotation() {
    
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-90.0f), glm::fvec3(0.0f, 1.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-90.0f), glm::fvec3(1.0f, 0.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-90.0f), glm::fvec3(0.0f, 0.0f, 1.0f));
}
    

void RenderObject::makeFinalMatrix() {
    orientationMatrix =  glm::translate(glm::mat4(1.0f), position) * rotationMatrix;
}

void RenderObject::update(float time) {
    mesh->update(time);
    
    if (moving) {
        setPosition(position + movementDirection * movementSpeed);
        makeFinalMatrix();
        
        if (glm::abs(glm::length(movementTarget - position)) < movementSpeed) {
            moving = false;
        }
    }
}

void RenderObject::render() {
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
    
    if (selected) {
        unitSelection->render();
    }
}

void RenderObject::onMessage(IMessage *message) {
    if (message->getKeyPressed() == "e") {
    }
    if (message->getKeyPressed() == "a") {
        mesh->beginAnimation("Walk");
    }
    if (message->getKeyPressed() == "s") {
        mesh->stopAnimation();
    }
    if (message->getKeyPressed() == "left_mouse_button_pressed") {
        if (selectable) {
            glm::fvec2 pos = message->getMousePosition();
            RayVector camRay = mRenderer->camera->getVectorRay(pos.x, pos.y);
            aux::ray ray;
            ray.begin = camRay.begin;
            ray.end = camRay.end;
            selected = selectionBox.isIntersected(ray);
        }
    }
    if (message->getMessage() == "unit_walk") {
        if (selected) {
            glm::fvec3 dest = message->getPosition();
            rotateToward(dest);
            adjustRotation();
            makeFinalMatrix();
            
            movementTarget = dest;
            moving = true;
            movementDirection = glm::normalize(movementTarget - position);
        }
    }
}