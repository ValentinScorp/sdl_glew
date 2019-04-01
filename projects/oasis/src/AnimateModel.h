#pragma once

#include "Precompiled.h"

class SmaVertex;
class AnimateModel
{
public:
    class Bone {
    public:
        Bone(glm::fvec3 p, glm::fvec3 r)
			:position(p), rotation(r) {}
		~Bone() {}

        glm::mat4 GetLocalToWorldMatrix();
        
        glm::fvec3 position;
        glm::fvec3 rotation;
    };
    class Weight {
    public:
        Weight(Bone *b, float w) : bone(b), weight(w) {}
		~Weight() {}
        
        Bone *bone;
        float weight;
    };
    class KeyFrame {
    public:
        unsigned int index;
    
        std::vector<Bone*> bone;
        std::vector<glm::fvec3> rotations;
        std::vector<glm::fvec3> positions;
        
        glm::fvec3* GetRotation(Bone *b) {
			for (size_t i = 0; i < bone.size(); i++) {
				if (bone[i] == b)
					return &rotations[i];
			}
			return nullptr;
		}
		glm::fvec3* GetPosition(Bone *b) {
			for (size_t i = 0; i < bone.size(); i++) {
				if (bone[i] == b)
					return &positions[i];
			}
			return nullptr;
		}

    };
    class Animation {
    public:
        Animation(std::string n)
			: name(n) {}
        ~Animation() {}
            
        std::string name;
        std::vector<KeyFrame> keyFrames;
        void AddKeyframe(KeyFrame kf) {
			keyFrames.push_back(kf);
		}
		std::string GetName() {
			return name;
		}
		KeyFrame * GetKeyframe(size_t id) {
			return &keyFrames[id];
		}

		size_t GetKeyframesNum() {
			return keyFrames.size();
		}
    };
public:
    AnimateModel();
    virtual ~AnimateModel();
    
    void AddBone(glm::fvec3 p, glm::fvec3 r);
	Bone* GetBone(size_t id);
	size_t GetBonesNum();
	void AddVertexWeights(std::vector<Weight> w);
	void AddAnimation(Animation *anim);	
	
	virtual void addVertex(glm::fvec3 pos, glm::fvec3 nor, glm::fvec2 uv);
	virtual void addVertex(SmaVertex ver);
	void UpdateAnimation(float dt, std::vector<SmaVertex> &smaVerts);
	void BeginAnimation(std::string aname);
	void StopAnimation();

	size_t GetVertexesNum();
	size_t GetVertexSize();
	SmaVertex* GetVertexes();
    Animation* getAnimation(std::string aname);

    std::vector<SmaVertex> vertexes;
    std::vector<Bone*> bones;
    std::vector<std::vector<Weight>> weights;
    
    std::vector<Animation*> animations;
    Animation *currentAnimation;
    
    size_t currentFrame;
    size_t animCounter;
};

