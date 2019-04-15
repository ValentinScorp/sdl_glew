#pragma once

class Renderer;
class Button;

class GuiPanel
{
public:
    GuiPanel();
    virtual ~GuiPanel();
    
    void init(std::shared_ptr<Renderer> renderer);
    void destroy();
    void render();
    
private:
    std::shared_ptr<Renderer> renderer;
    glm::fvec2 position = { 0.0f, 0.0f };
    glm::fvec2 dimension = { 0.0f, 0.0f };
    
    std::vector<std::unique_ptr<Button>> buttons;
};

