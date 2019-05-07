#pragma once

class Listbox;

class ListboxItem : public IMessageRecipient {
     class Vertex {
    public:
        glm::fvec2 pos;
    public:
        Vertex() {}
        ~Vertex() {}
        Vertex(glm::fvec2 v) {
            pos = v;
        }
        Vertex(Vertex const& v) {
            pos = v.pos;
        }
        Vertex& operator=(Vertex const& other) {
            if (this != &other) {
                pos = other.pos;
            }
            return *this;
        }
    };
public:
    ListboxItem();
    virtual ~ListboxItem();

    void init(std::shared_ptr<Renderer> renderer, glm::fvec2 position, glm::fvec2 dimension, std::string text, Listbox *parent);
    void destroy();
    void render();
    bool isPointOver(glm::fvec2 point);
    
    void onMessage(IMessage *message);
    
    bool opened = false;
    Listbox* parent;
    
 private:   
    glm::fvec2 position = {0.0f, 0.0f};
    glm::fvec2 dimension = {0.0f, 0.0f};
    std::string text;
    std::u16string u16text;
    
    const glm::fvec4 colorBase = { 0.5f, 0.5f, 0.5f, 0.7f };
    const glm::fvec4 colorOver = { 0.6f, 0.6f, 0.6f, 0.7f };
    const glm::fvec4 colorPress = { 0.8f, 0.8f, 0.8f, 0.7f };
    
    glm::fvec4 color = colorBase;
	
    std::shared_ptr<Renderer> renderer;
    std::vector<Vertex> vertices;
    
    GLuint glProgram = 0;
    GLuint glVbo = 0;
    GLuint glVao = 0;
    
    GLuint glColorUniformLocation = 0;
    GLuint glUniformPosition = 0;
    
};

class Listbox  : public IMessageRecipient {
    class Vertex {
    public:
        glm::fvec2 pos;
    public:
        Vertex() {}
        ~Vertex() {}
        Vertex(glm::fvec2 v) {
            pos = v;
        }
        Vertex(Vertex const& v) {
            pos = v.pos;
        }
        Vertex& operator=(Vertex const& other) {
            if (this != &other) {
                pos = other.pos;
            }
            return *this;
        }
    };
public:
    Listbox();
    virtual ~Listbox();

    void init(std::shared_ptr<Renderer> renderer, glm::fvec2 position, glm::fvec2 dimension, std::string text);
    void addItem(std::string itemText);
    void destroy();
    void render();
    bool isPointOver(glm::fvec2 point);
    
    void onMessage(IMessage *message);
    void hideChildren();
    
    std::string text;
    bool hidden = false;
    
private:
    glm::fvec2 position = {0.0f, 0.0f};
    glm::fvec2 dimension = {0.0f, 0.0f};
    
    std::string id;
    
    std::u16string u16text;
    
    const glm::fvec4 colorBase = { 0.5f, 0.5f, 0.5f, 0.7f };
    const glm::fvec4 colorOver = { 0.6f, 0.6f, 0.6f, 0.7f };
    const glm::fvec4 colorPress = { 0.8f, 0.8f, 0.8f, 0.7f };
    
    glm::fvec4 color = colorBase;
	
    std::shared_ptr<Renderer> renderer;
    std::vector<Vertex> vertices;
    
    GLuint glProgram = 0;
    GLuint glVbo = 0;
    GLuint glVao = 0;
    
    GLuint glColorUniformLocation = 0;
    GLuint glUniformPosition = 0;
    
    // listbox specific
    std::vector<std::unique_ptr<ListboxItem>> items;
    size_t selectedItemIndex = 0;
};

