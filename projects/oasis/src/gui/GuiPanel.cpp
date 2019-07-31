#include "../Precompiled.h"

GuiPanel::GuiPanel() {
}

GuiPanel::~GuiPanel() {
}

void GuiPanel::init(std::shared_ptr<Renderer> renderer) {
    this->renderer = renderer;
    
    std::unique_ptr<Button> buttonSave(new Button);
    buttonSave->init(renderer, glm::fvec2(500, 100), glm::fvec2(200, 40), "Save Map");
    buttons.push_back(std::move(buttonSave));
    
    std::unique_ptr<Button> buttonLoad(new Button);
    buttonLoad->init(renderer, glm::fvec2(500, 50), glm::fvec2(200, 40), "Load Map");
    buttons.push_back(std::move(buttonLoad));
    
    std::unique_ptr<Button> buttonGen(new Button);
    buttonGen->init(renderer, glm::fvec2(500, 150), glm::fvec2(200, 40), "Generate");
    buttons.push_back(std::move(buttonGen));
    
    std::unique_ptr<Listbox> testlistbox(new Listbox);
    testlistbox->init(renderer, glm::fvec2(40, 550), glm::fvec2(200, 40), "List box");
    
    testlistbox->addItem("None");
    testlistbox->addItem("Tree");
    testlistbox->addItem("Roman");
    testlistbox->addItem("Wall");
    testlistbox->addItem("Obstacle");
    listboxes.push_back(std::move(testlistbox));
}

void GuiPanel::destroy() {
    for (auto& b: buttons) {
        b->destroy();
    }
    for (auto& lb: listboxes) {
        lb->destroy();
    }
}

void GuiPanel::render() {
    for (auto& b: buttons) {
        b->render();
    }
    for (auto& lb: listboxes) {
        lb->render();
    }
}