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

class Camera {
public:
    Camera();
    virtual ~Camera();
    
    void init(Configuration *cfg);
    
    glm::mat4 getViewMatrix();
    glm::mat4 getCamMatrix();
    
    void moveUp();
    void moveDown();

    glm::vec3 position = {0.0f, 0.0f, 0.0f};;
    glm::vec3 upVector = {0.0f, 0.0f, 0.0f};;
    glm::vec3 lookTarget = {0.0f, 0.0f, 0.0f};;
    
    float aspectRatio = 0.0f;
    float moveSpeed = 0.0f;
};


