#pragma once

class Shader;
class Camera;
class Renderer;
class RenderQuad;

class TerrainBrush : public IMessageRecipient {
public:
	TerrainBrush();
	~TerrainBrush();
    
    void init(std::shared_ptr<Renderer> renderer, float width);
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
    
	float width = 0.0f;
    
    RenderQuad *renderQuad = nullptr;
};

