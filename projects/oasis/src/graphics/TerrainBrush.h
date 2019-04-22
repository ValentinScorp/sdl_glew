#pragma once

class Shader;
class Camera;
class Renderer;

class TerrainBrush : public IMessageRecipient {
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
	TerrainBrush();
	~TerrainBrush();
    
    void init(std::shared_ptr<Renderer> renderer);
	void render();
    void destroy();
    
    virtual void onMessage(IMessage *message);
    
	float GetWidth();
	float GetX();
	float GetY();

	void setX(float x);
	void setY(float y);

    float posX = 0;
	float posY = 0;
    float posZ = 0.01;
    
private:
	std::shared_ptr<Renderer> renderer;
	std::vector<Vertex> vertexes;
    
    GLuint glProgram = 0;
    GLuint glModelMatrixUniform = 0;
    GLuint glCameraMatricesUbo = 0;
    GLuint glVbo = 0;
    GLuint glVao = 0;
    
    glm::fmat4 orientationMatrix;
    

	float width = 4.0f;
    
    GLuint glTexture;
    std::string textureName;
};

