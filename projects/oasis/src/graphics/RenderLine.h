#pragma once

class Shader;
class Camera;
class Renderer;

class RenderLine {
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
	RenderLine();
	~RenderLine();
    
    void init(std::shared_ptr<Renderer> renderer, glm::fvec3 pointA, glm::fvec3 pointB, float width);
    void initWall(std::shared_ptr<Renderer> renderer, glm::fvec3 pointA, glm::fvec3 pointB, float height);
	void renderAt(glm::fvec3 position = glm::fvec3(0.0f, 0.0f, 0.0f));
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
    
    bool initialized = false;
};

