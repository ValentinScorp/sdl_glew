#include "../Precompiled.h"

// Listbox item

ListboxItem::ListboxItem()
{
}

ListboxItem::~ListboxItem()
{
}

void ListboxItem::init(std::shared_ptr<Renderer> renderer, glm::fvec2 position, glm::fvec2 dimension, std::string text) {
    
    this->renderer = renderer;
    this->position = position;
    this->dimension = dimension;
    /* 
     *       A        A      D
     *       | /\      \     ^
     *      \/   \       \|  |
     * pos-> B  -> C         C
     */
    Vertex leftTop = Vertex(glm::fvec2(0, dimension.y));
    Vertex leftBot = Vertex(glm::fvec2(0.0f));
    Vertex rightTop = Vertex(glm::fvec2(dimension.x, dimension.y));
    Vertex rightBot = Vertex(glm::fvec2(dimension.x, 0));

	vertices.push_back(leftTop);
	vertices.push_back(leftBot);
	vertices.push_back(rightBot);

	vertices.push_back(leftTop);
	vertices.push_back(rightBot);
	vertices.push_back(rightTop);
    
    glProgram = renderer->createProgram("data/guiListbox.vert", "data/guiListbox.frag");
        
    glUniformPosition = renderer->getParamFromProgram(glProgram, "position");
    GLuint glProjectionMatrix = renderer->getParamFromProgram(glProgram, "projectionMatrix");
    glColorUniformLocation = renderer->getParamFromProgram(glProgram, "buttonColor");

    glUseProgram(glProgram);
    glm::mat4 projection = renderer->camera->getProjectionMatrix();
    glUniformMatrix4fv(glProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform2f(glUniformPosition, position.x, position.y);
    
    glVbo = renderer->createVbo(vertices.data(), vertices.size() * sizeof(Vertex));
    glVao = renderer->createVao(glVbo, 2, 0, 0, 0, 0, sizeof(float));
    
    this->text = text;
    Console::getInstance().convertUtf8ToUtf16(text, u16text);
}

void ListboxItem::destroy() {
    renderer->destroyProgram(glProgram);
    renderer->destroyBuffer(glVao);
    renderer->destroyBuffer(glVbo);
}

void ListboxItem::render() {
    glUseProgram(glProgram);
    
    glBindBuffer(GL_ARRAY_BUFFER, glVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(glVao);
    glProgramUniform4fv(glProgram, glColorUniformLocation, 1, glm::value_ptr(color));
    //glUniform2f(glUniformPosition, position.x, position.y);
    
    glEnableVertexAttribArray(0);
    
    glUniform2f(glUniformPosition, position.x, position.y);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
      
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    glUseProgram(0);
    
    float scale = dimension.y / Console::getInstance().fontHeight / 1.5;
    
    Console::getInstance().convertUtf8ToUtf16(text, u16text);
    Console::getInstance().renderAt(position.x + 5, position.y + dimension.y / 4, scale, u16text);
}

void ListboxItem::onMessage(IMessage *message) {
    if (message->getKeyPressed() == "motion") {
        glm::fvec2 pos = renderer->camera->convertMouseToScreen(message->getMousePosition());
        if (isPointOver(pos)) {
            color = colorOver;
        } else {
            color = colorBase;
        }
    }
    if (message->getKeyPressed() == "left_mouse_button_pressed") {
        if (isPointOver(renderer->camera->convertMouseToScreen(message->getMousePosition()))) {
            color = colorPress;
        }
    }
    if (message->getKeyPressed() == "left_mouse_button_released") {
        if (isPointOver(renderer->camera->convertMouseToScreen(message->getMousePosition()))) {
            if (color == colorPress) {
                SMessageManager::getInstance().invokeMessage(new GuiButtonMessage(text));
                opened = false;
            }
            color = colorOver;
        } else {
            color = colorBase;
        }
    }
}

bool ListboxItem::isPointOver(glm::fvec2 point) {
    if (point.x > position.x && point.x < position.x + dimension.x &&
        point.y > position.y && point.y < position.y + dimension.y) {
            return true;
        }
    return false;
}

// Listbox =================

Listbox::Listbox()
{
    
}

Listbox::~Listbox()
{
}

void Listbox::init(std::shared_ptr<Renderer> renderer, glm::fvec2 position, glm::fvec2 dimension, std::string text) {
    
    this->renderer = renderer;
    this->position = position;
    this->dimension = dimension;
    
    /* 
     *       A        A      D
     *       | /\      \     ^
     *      \/   \       \|  |
     * pos-> B  -> C         C
     */
    Vertex leftTop = Vertex(glm::fvec2(0, dimension.y));
    Vertex leftBot = Vertex(glm::fvec2(0.0f));
    Vertex rightTop = Vertex(glm::fvec2(dimension.x, dimension.y));
    Vertex rightBot = Vertex(glm::fvec2(dimension.x, 0));

	vertices.push_back(leftTop);
	vertices.push_back(leftBot);
	vertices.push_back(rightBot);

	vertices.push_back(leftTop);
	vertices.push_back(rightBot);
	vertices.push_back(rightTop);
    
    glProgram = renderer->createProgram("data/guiListbox.vert", "data/guiListbox.frag");
        
    glUniformPosition = renderer->getParamFromProgram(glProgram, "position");
    GLuint glProjectionMatrix = renderer->getParamFromProgram(glProgram, "projectionMatrix");
    glColorUniformLocation = renderer->getParamFromProgram(glProgram, "buttonColor");

    glUseProgram(glProgram);
    glm::mat4 projection = renderer->camera->getProjectionMatrix();
    glUniformMatrix4fv(glProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform2f(glUniformPosition, position.x, position.y);
    
    glVbo = renderer->createVbo(vertices.data(), vertices.size() * sizeof(Vertex));
    glVao = renderer->createVao(glVbo, 2, 0, 0, 0, 0, sizeof(float));
    
    this->text = text;
    Console::getInstance().convertUtf8ToUtf16(text, u16text);
    
    std::unique_ptr<ListboxItem> listboxItem1(new ListboxItem);
    listboxItem1->init(renderer, position, glm::fvec2(200, 40), "Item 1");
    items.push_back(std::move(listboxItem1));
    
    std::unique_ptr<ListboxItem> listboxItem2(new ListboxItem);
    listboxItem2->init(renderer, glm::fvec2(position.x, position.y - 42), glm::fvec2(200, 40), "Item 2");
    items.push_back(std::move(listboxItem2));
}

void Listbox::destroy() {
    for (auto &i: items){
        i->destroy();
    }
    
    renderer->destroyProgram(glProgram);
    renderer->destroyBuffer(glVao);
    renderer->destroyBuffer(glVbo);
}

void Listbox::render() {
    if (opened){
        for (auto& i: items) {
            i->render();
        }
    } else {
        glUseProgram(glProgram);
        
        glBindBuffer(GL_ARRAY_BUFFER, glVbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glBindVertexArray(glVao);
        glProgramUniform4fv(glProgram, glColorUniformLocation, 1, glm::value_ptr(color));
        glUniform2f(glUniformPosition, position.x, position.y);
        
        glEnableVertexAttribArray(0);
        
        glUniform2f(glUniformPosition, position.x, position.y);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
          
        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
        glUseProgram(0);
        
        float scale = dimension.y / Console::getInstance().fontHeight / 1.5;
           
        Console::getInstance().convertUtf8ToUtf16(text, u16text);
        Console::getInstance().renderAt(position.x + 5, position.y + dimension.y / 4, scale, u16text);
    }
}

void Listbox::onMessage(IMessage *message) {
    if (message->getKeyPressed() == "motion") {
        glm::fvec2 pos = renderer->camera->convertMouseToScreen(message->getMousePosition());
        if (isPointOver(pos)) {
            color = colorOver;
        } else {
            color = colorBase;
        }
    }
    if (message->getKeyPressed() == "left_mouse_button_pressed") {
        if (isPointOver(renderer->camera->convertMouseToScreen(message->getMousePosition()))) {
            color = colorPress;
        }
    }
    if (message->getKeyPressed() == "left_mouse_button_released") {
        if (isPointOver(renderer->camera->convertMouseToScreen(message->getMousePosition()))) {
            if (color == colorPress) {
                SMessageManager::getInstance().invokeMessage(new GuiButtonMessage(text));
                opened = true;
                for (auto &i: items) {
                    i->opened = true;
                }
            }
            color = colorOver;
        } else {
            color = colorBase;
        }
    }
    if (message->getMessage() == "Item 1") {
        if (opened == true) {
            for (auto &i: items) {
                i->opened = false;
            }
            text = "Item 1";
            opened = false;
        }
    }
    if (message->getMessage() == "Item 2") {
        if (opened == true) {
            for (auto &i: items) {
                i->opened = false;
            }
            text = "Item 2";
            opened = false;
        }
    }
}

bool Listbox::isPointOver(glm::fvec2 point) {
    if (point.x > position.x && point.x < position.x + dimension.x &&
        point.y > position.y && point.y < position.y + dimension.y) {
            return true;
        }
    return false;
}