#include "Model.h"

using namespace std;

Vertex::Vertex() {
}
Vertex::~Vertex() {
}
Vertex::Vertex(glm::vec4 p, glm::vec2 t) {
    pos = p;
    tex = t;
}
Vertex::Vertex(Vertex const& v) {
    pos = v.pos;
    tex = v.tex;
}
Vertex::Vertex(float x, float y, float z, float w, float s, float t) {
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    pos[3] = w;
    tex[0] = s;
    tex[1] = t;
}

SmaVertex::SmaVertex() {
}
SmaVertex::~SmaVertex() {
}
SmaVertex::SmaVertex(glm::vec3 p, glm::vec3 n, glm::vec2 t) {
    pos = p;
    nor = n;
    tex = t;
}
SmaVertex::SmaVertex(SmaVertex const& v) {
    pos = v.pos;
    nor = v.nor;
    tex = v.tex;
}
SmaVertex::SmaVertex(float x, float y, float z, float nx, float ny, float nz, float s, float t) {
    pos[0] = x; pos[1] = y; pos[2] = z;
    nor[0] = nx; nor[1] = ny; nor[2] = nz;
    tex[0] = s; tex[1] = t;
}


Model::Model() {
}

Model::~Model() {
    if (amesh != 0) {
        delete amesh;
    }
}

void Model::loadMesh(std::string fileName) {
    ifstream file(fileName);
    vector<string> fileContent;
    if (file.is_open()) {
        while (file.good()) {
            string line;
            getline(file, line);
            fileContent.push_back(line);
        }
        file.close();
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to open file -> %s\n", fileName.c_str());
    }
    vector<glm::vec4> pos;
    vector<glm::vec2> tex;
    vector<glm::ivec2> indexes;
    for (auto line: fileContent) {
        vector<string> words;
        stringstream ssLine(line);
        string tmp;
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
                stringstream a(words[1]);
                string tmpa;
                glm::ivec2 inda;
                getline(a, tmpa, '/');
                inda.x = std::stoi(tmpa);
                getline(a, tmpa, '/');
                inda.y = std::stoi(tmpa);
                indexes.push_back(inda-1);
                
                stringstream b(words[2]);
                string tmpb;
                glm::ivec2 indb;
                getline(b, tmpb, '/');
                indb.x = std::stoi(tmpb);
                getline(b, tmpb, '/');
                indb.y = std::stoi(tmpb);
                indexes.push_back(indb-1);
                
                stringstream c(words[3]);
                string tmpc;
                glm::ivec2 indc;
                getline(c, tmpc, '/');
                indc.x = std::stoi(tmpc);
                getline(c, tmpc, '/');
                indc.y = std::stoi(tmpc);
                indexes.push_back(indc-1);
            }
        }
    }
    for (auto i: indexes) {
        Vertex v(pos[i.x], tex[i.y]);
        vertexes.push_back(v);
    }
}
GLsizeiptr Model::getVertexBufferSize() {
    if (amesh == 0)
        return 0;
    return (amesh->vertexes.size() * sizeof(SmaVertex));
}
GLvoid* Model::getVertexBufferData() {
    if (amesh == 0)
        return 0;
    //std::cout << amesh->vertexes[0].pos.x << std::endl;
    return amesh->vertexes.data();
    //return vertexes.data();
}

GLvoid* Model::getVertexBufferAnimData() {
    if (amesh == 0)
        return 0;
    return amesh->vertexes.data();
    //return vertexes.data();
}

GLsizei Model::getVertexLen() {
    return smaVerts.size();
}
void Model::loadSmaMesh(std::string fileName) {
    
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
	amesh = new AnimateModel();
	//amesh->SetTexture(texName);

	unsigned short numBones = *(unsigned short*)data_iterator;
	data_iterator += sizeof(unsigned short);
	
	//auto animations = new Animations();

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
		
		amesh->AddBone(pos, rot);
	}
	
	// vertex weights
	unsigned short numVertWeights = *(unsigned short*)data_iterator;
	data_iterator += sizeof(unsigned short);

	for (int i = 0; i < numVertWeights; i++) {
		unsigned short numWeights = *(unsigned short*)data_iterator;
		data_iterator += sizeof(unsigned short);

		std::vector<AnimateModel::Weight> vertexWeights;
		for (int j = 0; j < numWeights; j++) {
			short boneIndex = *(unsigned short*)data_iterator;
			data_iterator += sizeof(unsigned short);

			float weight = *(float*)data_iterator;
			data_iterator += sizeof(float);

			
			AnimateModel::Bone *bone = nullptr;
			if (boneIndex >= 0 && boneIndex < amesh->GetBonesNum()) {
				bone = amesh->GetBone(boneIndex);
			}
			AnimateModel::Weight w(bone, weight);
			vertexWeights.push_back(w);
		}
		amesh->AddVertexWeights(vertexWeights);	
	}
	// animations

	unsigned short numAnimations = *(unsigned short*)data_iterator;
	data_iterator += sizeof(unsigned short);
	for (int i = 0; i < numAnimations; i++)	{
		
		char animName[64] = "None";
		memcpy(animName, data_iterator, sizeof(char) * 64);
		data_iterator += sizeof(char) * 64;

		auto *animation = new AnimateModel::Animation(animName);
        std::cout << "reading animation " << animName << std::endl;
        
		unsigned short numKeyframes = *(unsigned short*)data_iterator;
		data_iterator += sizeof(unsigned short);
		
		for (int j = 0; j < numKeyframes; j++) {
			AnimateModel::KeyFrame kf;
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
				kf.bone.push_back(amesh->GetBone(k));
			}
			animation->AddKeyframe(kf);
		}
		amesh->AddAnimation(animation);
	}
	amesh->BeginAnimation("Walk");
    
	delete[] data;
	data_iterator = nullptr;
    
    for (int i = 0; i < positions.size(); i++) {
        SmaVertex vert(positions[i], normals[i], texcoords[i]);
        smaVerts.push_back(vert);
        amesh->vertexes.push_back(vert);
    }
}

void Model::update(float time) {
    amesh->UpdateAnimation(time, smaVerts);
}
