#pragma once

class UnitSelection
{
    class Vertex {
    private:
        glm::fvec3 pos;
        glm::fvec2 uv;
    public:
        Vertex() {}
        ~Vertex() {}
        Vertex(glm::fvec3 v, glm::fvec2 t) {
            pos = v;
            uv = t;
        }
        Vertex(Vertex const& v) {
            pos = v.pos;
            uv = v.uv;
        }
        Vertex& operator=(Vertex const& other) {
            if (this != &other) {
                pos = other.pos;
                uv = other.uv;
            }
            return *this;
        }
    };
public:
    UnitSelection();
    virtual ~UnitSelection();
    
    void init(std::shared_ptr<Renderer> renderer, float size);
    void render();
    void destroy();
    
    std::shared_ptr<Renderer> mRenderer;
    std::vector<Vertex> vertices;
    
    GLuint glTexture = 0;
    GLuint glProgram = 0;
    GLuint glModelMatrixUniform = 0;
    GLuint glCameraMatricesUbo = 0;
    GLuint glVbo = 0;
    GLuint glVao = 0;
    
    glm::fmat4 orientationMatrix;
    glm::fvec3 position;
};

