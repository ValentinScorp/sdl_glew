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

Model::Model() {
}

Model::~Model() {
    if (vertexArray != nullptr) {
        delete [] vertexArray;
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
        SDL_Log("Unable to open file -> %s\n", fileName.c_str());
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
                SDL_Log("Loaded object -> %s\n", words[1].c_str());
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
    
    vertexArray = new float[vertexes.size() * 6];
    for (int i = 0; i < vertexes.size(); i++) {
        vertexArray[i*6]      = vertexes[i].pos.x;
        vertexArray[i*6+1]    = vertexes[i].pos.y;
        vertexArray[i*6+2]    = vertexes[i].pos.z;
        vertexArray[i*6+3]    = vertexes[i].pos.w;
        vertexArray[i*6+4]    = vertexes[i].tex.s;
        vertexArray[i*6+5]    = vertexes[i].tex.t;
    }
}
GLsizeiptr Model::getVertexBufferSize() {
    return (vertexes.size() * sizeof(Vertex));
}
GLvoid* Model::getVertexBufferData() {
    return vertexes.data();
}
GLsizei Model::getVertexLen() {
    return vertexes.size();
}
