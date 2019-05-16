#pragma once

class Shader;
class Camera;
class Renderer;

class RenderQuad {
private:
    class Vertex {
    private:
        glm::fvec3 pos;
    public:
        Vertex() {}
        ~Vertex() {}
        Vertex(glm::fvec3 v) {
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
	RenderQuad();
	~RenderQuad();
    
    void init(std::shared_ptr<Renderer> renderer, float width);
	void renderAt(glm::fvec3 position);
    void destroy();
private:
	std::shared_ptr<Renderer> renderer;
	std::vector<Vertex> vertexes;
    
    GLuint glProgram = 0;
    GLuint glModelMatrixUniform = 0;
    GLuint glCameraMatricesUbo = 0;
    GLuint glVbo = 0;
    GLuint glVao = 0;
    
    glm::fmat4 orientationMatrix;
};

