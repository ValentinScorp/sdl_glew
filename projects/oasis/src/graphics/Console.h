#pragma once

class Renderer;

class Console
{
    struct Character {
        GLuint glTexture;
        glm::ivec2 size;
        glm::ivec2 bearing;
        FT_Pos advance;
    };
private:
    Console() {}
    Console(Console const&) = delete;
    Console& operator=(Console const&) = delete;
public:
    static Console& getInstance() {
        static Console instance;
        return instance;
    }
    void init(std::shared_ptr<Renderer> renderer, Configuration *cfg);
    void loadFont(std::string path);
    
    void render();
    void destroy();
    
private:
    std::shared_ptr<Renderer> mRenderer = nullptr;
    GLuint glFontVbo = 0;
    GLuint glFontVao = 0;
    GLuint glFontProgram = 0;
    GLuint glFontProjectionMatrix = 0;
    std::map<GLchar, Character> characters;
};

