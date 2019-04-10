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
    
    void attachFrameTime(std::shared_ptr<float> ft);
    
    void renderFrameParameters();
    void renderAt(Uint16 x, Uint16 y, float scale, std::u16string text);
    
private:
    std::shared_ptr<Renderer> mRenderer = nullptr;
    GLuint glFontVbo = 0;
    GLuint glFontVao = 0;
    GLuint glFontProgram = 0;
    GLuint glFontProjectionMatrix = 0;
    std::map<FT_ULong, Character> characters;
    
    std::shared_ptr<float> frameTime;
    float screenWidth = 800;
    float screenHeight = 600;
};

