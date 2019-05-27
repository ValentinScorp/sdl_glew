#pragma once

class Mesh
{
private:
    class Vertex {
    public:
        Vertex() {}
        Vertex(Vertex const& v) {
            pos = v.pos; nor = v.nor; tex = v.tex;
        }
        Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 t) {
            pos = p; nor = n; tex = t;
        }
        Vertex(float x, float y, float z, float nx, float ny, float nz, float s, float t) {
            pos[0] = x; pos[1] = y; pos[2] = z;
            nor[0] = nx; nor[1] = ny; nor[2] = nz;
            tex[0] = s; tex[1] = t;
        }
        ~Vertex() {}
        Vertex& operator=(Vertex const& other) {
            if (this != &other) {
                pos = other.pos;
                nor = other.nor;
                tex = other.tex;
            }
            return *this;
        }
        void print() {
            SDL_Log("%f %f %f   %f %f %f   %f %f \n", pos.x, pos.y, pos.z, nor.x, nor.y, nor.z, tex.s, tex.t);
        }
        
        glm::fvec3 pos;
        glm::fvec3 nor;
        glm::fvec2 tex;
    };
    
    class Bone {
    public:
        Bone() {}
        Bone(glm::fvec3 p, glm::fvec3 r) :position(p), rotation(r) {}
		~Bone() {}
        
        glm::mat4 GetLocalToWorldMatrix();
        
        glm::fvec3 position;
        glm::fvec3 rotation;
        bool matrixCalculated = false;
        glm::fmat4 finalMatrix;
        glm::fmat4 finalMatrixInverted;
    };
    
    class Weight {
    public:
        Weight(Bone *b, size_t boneId, float w) : bone(b), boneIndex(boneId), weight(w) {}
		~Weight() {}
        
        Bone *bone;
        size_t boneIndex = 0;
        float weight;
    };
    
    class KeyFrame {
    public:
        KeyFrame() {}
        ~KeyFrame(){}
    
        unsigned int index;
    
        std::vector<Bone*> bones;
        std::vector<glm::fvec3> rotations;
        std::vector<glm::fvec3> positions;
        std::vector<glm::fmat4> initMatrix;
        std::vector<glm::fmat4> deformMatrix;
        
        glm::fvec3* GetRotation(Bone *b) {
			for (size_t i = 0; i < bones.size(); i++) {
				if (bones[i] == b)
					return &rotations[i];
			}
			return nullptr;
		}
		glm::fvec3* GetPosition(Bone *b) {
			for (size_t i = 0; i < bones.size(); i++) {
				if (bones[i] == b)
					return &positions[i];
			}
			return nullptr;
		}
        void resize(size_t bonesMax) {
            bones.resize(bonesMax);
            positions.resize(bonesMax);
            rotations.resize(bonesMax);
            initMatrix.resize(bonesMax);
            deformMatrix.resize(bonesMax);
        }
        KeyFrame& operator=(const KeyFrame& other) {
            if (this != &other) {
                resize(other.bones.size());
                for (size_t i = 0; i < other.bones.size(); i++) {
                    bones[i] = other.bones[i];
                    positions[i] = other.positions[i];
                    rotations[i] = other.rotations[i];
                    initMatrix[i] = other.initMatrix[i];
                    deformMatrix[i] = other.deformMatrix[i];
                }
            }
            return *this;
        }
        void generateMatrices();
    };
    
public:
    class Animation {
    public:
        Animation(std::string n) : name(n) {}
        ~Animation() {}
                
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
        
        std::string name;
        std::vector<KeyFrame> keyFrames;
    };
    
public:
    Mesh();
    virtual ~Mesh();

    void print() {
        for (auto v: vertexes) {
            v.print();
        }
        for (auto v: animVertexes) {
            v.print();
        }
    }
    
    void loadObjMesh(std::string fileName);
    void loadSmaMesh(std::string fileName);

    GLsizeiptr getVertexBufferSize();
    GLvoid* getVertexBufferData();
    GLvoid* getVertexBufferAnimData();
    GLsizei getVertexLen();
    
    void AddBone(glm::fvec3 p, glm::fvec3 r);
	Bone* GetBone(size_t id);
	size_t GetBonesNum();
	void AddVertexWeights(std::vector<Weight> w);
	void AddAnimation(Animation *anim);
	
	virtual void addVertex(glm::fvec3 pos, glm::fvec3 nor, glm::fvec2 uv);
	virtual void addVertex(Vertex ver);
	void updateAnimation(Animation *currentAnimation, size_t *currentFrame, size_t *animCounter, float dt);	
    void getFrame(std::vector<Vertex> &frameVertexes, Animation *animationPtr, Sint16 frameIndex);
    void setAnimationData(Sint16 frameIndex);
	void initilizeMesh();
    void processWalkAnimation();

	size_t GetVertexesNum();
	size_t GetVertexSize();
	Vertex* GetVertexes();
    Animation* getAnimation(std::string aname);
    
    std::vector<Vertex> vertexes;
    std::vector<Vertex> animVertexes;
    
    std::vector<std::vector<Vertex>> walkAnimationVertexes;
    
    std::vector<Bone*> bones;
    std::vector<std::vector<Weight>> weights;
    
    std::vector<Animation*> animations;
    
   // Animation *currentAnimation;
   // size_t currentFrame;
   // size_t animCounter;
    
    std::string name;
};

