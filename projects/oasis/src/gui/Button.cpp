#include "../Precompiled.h"

Button::Button()
{
}

Button::~Button()
{
}

void Button::init(std::shared_ptr<Renderer> renderer, glm::fvec2 position, glm::fvec2 dimension, std::string text) {
    this->renderer = renderer;
    this->position = position;
    this->dimension = dimension;
    
    Vertex leftTop = Vertex(glm::fvec2(position.x, position.y + dimension.y));
    Vertex leftBot = Vertex(glm::fvec2(position));
    Vertex rightTop = Vertex(glm::fvec2(position.x + dimension.x, position.y + dimension.y));
    Vertex rightBot = Vertex(glm::fvec2(position.x + dimension.x, position.y));

	vertices.push_back(leftTop);
	vertices.push_back(leftBot);
	vertices.push_back(rightBot);

	vertices.push_back(leftTop);
	vertices.push_back(rightBot);
	vertices.push_back(rightTop);
    
    glProgram = renderer->createProgram("data/guiButton.vert", "data/guiButton.frag");
        
    GLuint glProjectionMatrix = renderer->getParamFromProgram(glProgram, "projectionMatrix");
    glm::mat4 projection = renderer->camera->getProjectionMatrix();
    
    glColorUniformLocation = renderer->getParamFromProgram(glProgram, "buttonColor");

    glUseProgram(glProgram);
    glUniformMatrix4fv(glProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projection));
    
    glVbo = renderer->createVbo(vertices.data(), vertices.size() * sizeof(Vertex));
    glVao = renderer->createVao(glVbo, 2, 0, 0, 0, 0, sizeof(float));
    
    this->text = text;
    Console::getInstance().convertUtf8ToUtf16(text, u16text);
}

void Button::destroy() {
    renderer->destroyProgram(glProgram);
    renderer->destroyVertexArray(glVao);
    renderer->destroyBuffer("Button glVbo", glVbo);
}

void Button::render() {
    glUseProgram(glProgram);
    
    glBindBuffer(GL_ARRAY_BUFFER, glVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(glVao);
    
    glProgramUniform4fv(glProgram, glColorUniformLocation, 1, glm::value_ptr(color));
    
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    glUseProgram(0);
    
    float scale = dimension.y / Console::getInstance().fontHeight / 1.5;
    Console::getInstance().renderAt(position.x + 5, position.y + dimension.y / 4, scale, u16text);
}

bool Button::isPointOver(glm::fvec2 point) {
    if (point.x > position.x && point.x < position.x + dimension.x &&
        point.y > position.y && point.y < position.y + dimension.y) {
            return true;
        }
    return false;
}

void Button::onMessage(IMessage *message) {
    if (message == nullptr) {
        return;
    }
    if (message->getKeyPressed() == "motion") {
        glm::fvec2 pos = renderer->camera->convertMouseToScreen(message->getMousePosition());
        if (isPointOver(pos)) {
            color = colorOver;
            message->destroyed = true;
        } else {
            color = colorBase;
        }
    }
    if (message->getKeyPressed() == "left_mouse_button_pressed") {
        if (isPointOver(renderer->camera->convertMouseToScreen(message->getMousePosition()))) {
            color = colorPress;
            message->destroyed = true;
        }
    }
    if (message->getKeyPressed() == "left_mouse_button_released") {
        if (isPointOver(renderer->camera->convertMouseToScreen(message->getMousePosition()))) {
            if (color == colorPress) {
                SMessageManager::getInstance().invokeMessage(new GuiButtonMessage(text));
            }
            message->destroyed = true;
            color = colorOver;
        } else {
            color = colorBase;
        }
    }
}
