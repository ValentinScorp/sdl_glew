#pragma once

class Configuration;

class RayVector {
public:
	RayVector() {
		begin = {0.0f, 0.0f, 0.0f};
		end = { 0.0f, 0.0f, 0.0f };
	}
	RayVector(glm::fvec3 b, glm::fvec3 e) {
		begin = { b.x, b.y, b.z };
		end = { e.x, e.y, e.z };
	}
	~RayVector() {}
	glm::fvec3 begin;
	glm::fvec3 end;
};

class Camera : public IMessageRecipient  {
public:
    Camera(std::shared_ptr<Renderer> renderer);
    virtual ~Camera();
    
    virtual void onMessage(IMessage *message);
    
    void init(Configuration *cfg);
    
    glm::mat4 getViewMatrix();
    glm::mat4 getCamMatrix();
    
    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();

    glm::vec3 position = {0.0f, 0.0f, 0.0f};;
    glm::vec3 upVector = {0.0f, 0.0f, 0.0f};;
    glm::vec3 lookTarget = {0.0f, 0.0f, 0.0f};;
    
    float aspectRatio = 0.0f;
    float moveSpeed = 1.0f;
    
    std::shared_ptr<Renderer> mRenderer;
};


