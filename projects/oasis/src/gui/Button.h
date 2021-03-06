#pragma once

class Renderer;

class Button : public IMessageRecipient {
private:
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
    Button();
    virtual ~Button();
    
    void init(std::shared_ptr<Renderer> renderer, glm::fvec2 position, glm::fvec2 dimension, std::string text);
    void destroy();
    void render();
    bool isPointOver(glm::fvec2 point);
    
    void onMessage(IMessage *message);
    
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
};

