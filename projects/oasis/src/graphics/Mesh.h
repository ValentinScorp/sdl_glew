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
        Bone(glm::fvec3 p, glm::fvec3 r) :position(p), rotation(r) {}
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
        Animation(std::string n) : name(n) {}
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
    Mesh() {
        currentAnimation = nullptr;
        animCounter = 0;
        currentFrame = 0;
    }
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
    
    void update(float time);
    
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
	void UpdateAnimation(float dt, std::vector<Vertex> &smaVerts);
	void beginAnimation(std::string aname);
	void stopAnimation();

	size_t GetVertexesNum();
	size_t GetVertexSize();
	Vertex* GetVertexes();
    Animation* getAnimation(std::string aname);
    
    std::vector<Vertex> vertexes;
    std::vector<Vertex> animVertexes;
    
    std::vector<Bone*> bones;
    std::vector<std::vector<Weight>> weights;
    
    std::vector<Animation*> animations;
    Animation *currentAnimation;
    
    size_t currentFrame;
    size_t animCounter;
    
    std::string name;
};

