#include "../Precompiled.h"

glm::mat4 Mesh::Bone::GetLocalToWorldMatrix() {
	glm::mat4 FinalMat = glm::mat4(1.0f);
	glm::mat4 MatTemp = glm::mat4(1.0f);
	glm::mat4 MatRot = glm::mat4(1.0f);

    FinalMat = glm::translate(FinalMat, glm::fvec3(position.x, position.y, position.z));
    
    MatTemp = glm::rotate(MatTemp, rotation.x, glm::fvec3(1.0f, 0.0f, 0.0f));
    MatRot =  MatTemp * MatRot;
    MatTemp = glm::mat4(1.0);
    MatTemp = glm::rotate(MatTemp, rotation.y, glm::fvec3(0.0f, 1.0f, 0.0f));
    MatRot =  MatTemp * MatRot;
    MatTemp = glm::mat4(1.0);
    MatTemp = glm::rotate(MatTemp, rotation.z, glm::fvec3(0.0f, 0.0f, 1.0f));
    MatRot =  MatTemp * MatRot;
    
	FinalMat = FinalMat * MatRot;

	return FinalMat;
}

Mesh::~Mesh() {
    weights.clear();

	for (auto a : animations)
		if (a != nullptr)
			delete a;

	for (auto b : bones)
		if (b != nullptr)
			delete b;
}

void Mesh::loadObjMesh(std::string fileName) {
    std::ifstream file(fileName);
    std::vector<std::string> fileContent;
    if (file.is_open()) {
        while (file.good()) {
            std::string line;
            std::getline(file, line);
            fileContent.push_back(line);
        }
        file.close();
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to open file -> %s\n", fileName.c_str());
    }
    std::vector<glm::vec4> pos;
    std::vector<glm::vec2> tex;
    std::vector<glm::ivec2> indexes;
    for (auto line: fileContent) {
        std::vector<std::string> words;
        std::stringstream ssLine(line);
        std::string tmp;
        while (getline(ssLine, tmp, ' ')) {
            words.push_back(tmp);
        }
        if (words.size() > 1) {
            if (words[0] == "o") {
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Loaded object -> %s\n", words[1].c_str());
            }
            if (words[0] == "v") {
                glm::vec4 v(stof(words[1]), stof(words[2]), stof(words[3]), 1.0f);
                pos.push_back(v);
            }
            if (words[0] == "vt") {
                glm::vec2 t(stof(words[1]), stof(words[2]));
                tex.push_back(t);
            }
            if (words[0] == "f") {
                std::stringstream a(words[1]);
                std::string tmpa;
                glm::ivec2 inda;
                std::getline(a, tmpa, '/');
                inda.x = std::stoi(tmpa);
                std::getline(a, tmpa, '/');
                inda.y = std::stoi(tmpa);
                indexes.push_back(inda-1);
                
                std::stringstream b(words[2]);
                std::string tmpb;
                glm::ivec2 indb;
                std::getline(b, tmpb, '/');
                indb.x = std::stoi(tmpb);
                std::getline(b, tmpb, '/');
                indb.y = std::stoi(tmpb);
                indexes.push_back(indb-1);
                
                std::stringstream c(words[3]);
                std::string tmpc;
                glm::ivec2 indc;
                std::getline(c, tmpc, '/');
                indc.x = std::stoi(tmpc);
                std::getline(c, tmpc, '/');
                indc.y = std::stoi(tmpc);
                indexes.push_back(indc-1);
            }
        }
    }
    for (auto i: indexes) {
        Vertex v(pos[i.x], glm::fvec3(0.0f), tex[i.y]);
        vertexes.push_back(v);
    }
}
GLsizeiptr Mesh::getVertexBufferSize() {
    return (vertexes.size() * sizeof(Vertex));
}
GLvoid* Mesh::getVertexBufferData() {
    return vertexes.data();
}

GLvoid* Mesh::getVertexBufferAnimData() {
    return animVertexes.data();
}

GLsizei Mesh::getVertexLen() {
    return vertexes.size();
}

void Mesh::loadSmaMesh(std::string fileName) {
    
	std::ifstream inputFile(fileName, std::ios::in | std::ios::binary);
	if (inputFile.fail()) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to open file -> %s\n", fileName.c_str());
        return;
	}

	inputFile.seekg(0, std::ios::end);
	long fileSize = inputFile.tellg();
	inputFile.seekg(0, std::ios::beg);
	
	unsigned char *data = new unsigned char[fileSize];
	inputFile.read((char*)data, fileSize);
	inputFile.close();
	unsigned char *data_iterator = data;

	char fileFormat[8] = "";
	char fileFormatSma[8] = "SMA";
	memcpy(fileFormat, data_iterator, sizeof(char) * 8);
	if (strcmp(fileFormat, fileFormatSma) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "This is not sma file -> %s\n", fileName.c_str());
		delete[] data;
		return;
	}
	data_iterator += sizeof(char) * 8;
		
	unsigned short version = *(unsigned short*)data_iterator;
	data_iterator += sizeof(unsigned short);
	
	char meshName[32] = " ";
	memcpy(meshName, data_iterator, sizeof(char) * 32);
	data_iterator += sizeof(char) * 32;
	
	// vertexes
	unsigned short posTotal = *(unsigned short*)data_iterator;
	data_iterator += sizeof(unsigned short);
	std::vector<glm::vec3> positions;
	if (posTotal) {
		for (int i = 0; i < (posTotal / 3); i++) {

			float *x = (float*)data_iterator; data_iterator += sizeof(float);
			float *y = (float*)data_iterator; data_iterator += sizeof(float);
			float *z = (float*)data_iterator; data_iterator += sizeof(float);

			glm::vec3 p((*x), *y, *z);

			positions.push_back(p);
		}
	}

	// normals
	unsigned short normalsTotal = *(unsigned short*)data_iterator;
	data_iterator += sizeof(unsigned short);
	std::vector<glm::vec3> normals;
	if (normalsTotal) {
		for (int i = 0; i < (normalsTotal / 3); i++) {

			float *x = (float*)data_iterator; data_iterator += sizeof(float);
			float *y = (float*)data_iterator; data_iterator += sizeof(float);
			float *z = (float*)data_iterator; data_iterator += sizeof(float);

			glm::vec3 n((*x), *y, *z);

			normals.push_back(n);
		}
	}
	// texture coordinates
	unsigned short uvTotal = *(unsigned short*)data_iterator;
	data_iterator += sizeof(unsigned short);
	std::vector<glm::vec2> texcoords;
	if (uvTotal > 0 && uvTotal == (posTotal / 3) * 2) {
		for (int i = 0; i < (uvTotal / 2); i++) {

			float *s = (float*)data_iterator; data_iterator += sizeof(float);
			float *t = (float*)data_iterator; data_iterator += sizeof(float);

			glm::vec2 uv(*s, *t);
			texcoords.push_back(uv);
		}
	}

	// textures
	std::string texName;

	unsigned short numTextures = *(unsigned short*)data_iterator;
	data_iterator += sizeof(unsigned short);
		
	for (int i = 0; i < numTextures; i++) {
		char textureName[64] = "img/default.png";
		memcpy(textureName, data_iterator, sizeof(char) * 64);
		data_iterator += sizeof(char) * 64;
		
		texName = textureName;
		
		unsigned short numTriangleIndexes = *(unsigned short*)data_iterator;
		data_iterator += sizeof(unsigned short);

		for (int j = 0; j < numTriangleIndexes; j++) {
			unsigned short index = *(unsigned short*)data_iterator;
			// todo save indexes
			data_iterator += sizeof(unsigned short);
		}
	}
    
    // smaVertexes
    //================================ animations =========================================
	// skeleton
	unsigned short numBones = *(unsigned short*)data_iterator;
	data_iterator += sizeof(unsigned short);
	
	for (int i = 0; i < numBones; i++) {

		short parentIdx = *(unsigned short*)data_iterator;
		data_iterator += sizeof(unsigned short);
		
		glm::fvec3 rot, pos;
		rot.x = (*(float*)data_iterator); data_iterator += sizeof(float);
		rot.y = *(float*)data_iterator; data_iterator += sizeof(float);
		rot.z = *(float*)data_iterator; data_iterator += sizeof(float);

		pos.x = (*(float*)data_iterator); data_iterator += sizeof(float);
		pos.y = *(float*)data_iterator; data_iterator += sizeof(float);
		pos.z = *(float*)data_iterator; data_iterator += sizeof(float);
		
		AddBone(pos, rot);
	}
	
	// vertex weights
	unsigned short numVertWeights = *(unsigned short*)data_iterator;
	data_iterator += sizeof(unsigned short);

	for (int i = 0; i < numVertWeights; i++) {
		unsigned short numWeights = *(unsigned short*)data_iterator;
		data_iterator += sizeof(unsigned short);

		std::vector<Weight> vertexWeights;
		for (int j = 0; j < numWeights; j++) {
			short boneIndex = *(unsigned short*)data_iterator;
			data_iterator += sizeof(unsigned short);

			float weight = *(float*)data_iterator;
			data_iterator += sizeof(float);
			
			Bone *bone = nullptr;
			if (boneIndex >= 0 && boneIndex < GetBonesNum()) {
				bone = GetBone(boneIndex);
			}
			Weight w(bone, weight);
			vertexWeights.push_back(w);
		}
		AddVertexWeights(vertexWeights);
	}
    
	// animations
	unsigned short numAnimations = *(unsigned short*)data_iterator;
	data_iterator += sizeof(unsigned short);
	for (int i = 0; i < numAnimations; i++) {
		
		char animName[64] = "None";
		memcpy(animName, data_iterator, sizeof(char) * 64);
		data_iterator += sizeof(char) * 64;

		auto *animation = new Animation(animName);
        
		unsigned short numKeyframes = *(unsigned short*)data_iterator;
		data_iterator += sizeof(unsigned short);
		
		for (int j = 0; j < numKeyframes; j++) {
			KeyFrame kf;
			unsigned short keyframeIndex = *(unsigned short*)data_iterator;
			data_iterator += sizeof(unsigned short);

			kf.index = keyframeIndex;
			for (int k = 0; k < numBones; k++) {
				glm::fvec3 rotation;
				glm::fvec3 position;

				rotation.x = (*(float*)data_iterator); data_iterator += sizeof(float);
				rotation.y = *(float*)data_iterator; data_iterator += sizeof(float);
				rotation.z = *(float*)data_iterator; data_iterator += sizeof(float);
				position.x = (*(float*)data_iterator); data_iterator += sizeof(float);
				position.y = *(float*)data_iterator; data_iterator += sizeof(float);
				position.z = *(float*)data_iterator; data_iterator += sizeof(float);

				kf.rotations.push_back(rotation);
				kf.positions.push_back(position);
				kf.bone.push_back(GetBone(k));
			}
			animation->AddKeyframe(kf);
		}
		AddAnimation(animation);
	}
    
	delete[] data;
	data_iterator = nullptr;
    
    for (int i = 0; i < positions.size(); i++) {
        Vertex vert(positions[i], normals[i], texcoords[i]);
        vertexes.push_back(vert);
        animVertexes.push_back(vert);
    }
}

void Mesh::update(float time) {
    UpdateAnimation(time, vertexes);
}

void Mesh::AddBone(glm::fvec3 p, glm::fvec3 r) {
	auto b = new Bone(p, r);
	bones.push_back(b);
}

Mesh::Bone * Mesh::GetBone(size_t id) {
	if (id < bones.size())
		return bones[id];
	return nullptr;
}

size_t Mesh::GetBonesNum() {
	return bones.size();
}

void Mesh::AddVertexWeights(std::vector<Weight> w) {
	weights.push_back(w);
}

void Mesh::AddAnimation(Animation * anim) {
	animations.push_back(anim);
}

void Mesh::addVertex(glm::fvec3 pos, glm::fvec3 nor, glm::fvec2 uv) {
	Vertex v;
	v.pos = pos;
	v.nor = nor;
	v.tex = uv;
	vertexes.push_back(v);

	vertexes.push_back(Vertex(pos, nor, uv));
}

void Mesh::addVertex(Vertex ver) {
	vertexes.push_back(ver);
}

void Mesh::UpdateAnimation(float dt, std::vector<Vertex> &smaVerts) {
	if (currentAnimation == nullptr) {
		return;
	}
	std::vector<Bone> bones;

	KeyFrame *kf = currentAnimation->GetKeyframe(currentFrame);

	animCounter += 1;
	if (animCounter >= 1) {
		animCounter = 0;
		currentFrame++;
	}
	if (currentFrame >= currentAnimation->GetKeyframesNum()) {
		currentFrame = 0;
	}
	for (int i = 0; i < vertexes.size(); i++) {
		glm::fvec3 finalVecPositin(0.0f, 0.0f, 0.0f);

		for (int j = 0; j < weights[i].size(); j++) {
			Bone* init_bone = weights[i][j].bone;
			float weight = weights[i][j].weight;

			if (init_bone != nullptr) {
				Bone deform_bone(*(kf->GetPosition(init_bone)), *(kf->GetRotation(init_bone)));
				glm::mat4 bonemat = glm::inverse(init_bone->GetLocalToWorldMatrix());
				glm::mat4 deformbonemat = deform_bone.GetLocalToWorldMatrix();

				glm::fvec3 vertPos(smaVerts[i].pos);
				vertPos = bonemat * glm::vec4(vertPos, 1.0f);
                vertPos = deformbonemat * glm::vec4(vertPos, 1.0f);
				vertPos *= weight;

				finalVecPositin += vertPos;
			}
		}

		animVertexes[i].pos.x = finalVecPositin.x;
		animVertexes[i].pos.y = finalVecPositin.y;
		animVertexes[i].pos.z = finalVecPositin.z;
	}
}

void Mesh::BeginAnimation(std::string aname) {
	for (auto a : animations) {
		if (a->GetName() == aname) {
			currentAnimation = a;
		}
	}
}

void Mesh::StopAnimation() {
	for (int i = 0; i < vertexes.size(); i++) {
		animVertexes[i] = vertexes[i];
	}
	currentAnimation = nullptr;
}

Mesh::Animation * Mesh::getAnimation(std::string aname) {
	for (auto a : animations) {
		if (a->GetName() == aname) {
			return a;
		}
	}
	return nullptr;
}

size_t Mesh::GetVertexesNum() {
	return vertexes.size();
}

size_t Mesh::GetVertexSize() {
	return sizeof(Vertex);
}

Mesh::Vertex* Mesh::GetVertexes() {
	return &vertexes[0];
}
