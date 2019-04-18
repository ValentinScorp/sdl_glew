#pragma once

namespace aux {

class vertex {
public:
    vertex() {}
    vertex(glm::fvec3 p, glm::fvec3 n) {
        pos = p;
        nor = n;
    }
    ~vertex() {}
    
    void calcNormal() {
        for (auto& t: triangles) {
            // todo
        }
    }
    
    glm::fvec3 pos = { 0.0f, 0.0f, 0.0f };
    glm::fvec3 nor = { 0.0f, 0.0f, 0.0f };
    
    std::vector<std::shared_ptr<triangle>> triangles;
};
    
class triangle {
public:
    triangle(std::shared_ptr<vertex> a, std::shared_ptr<vertex> b,std::shared_ptr<vertex> c) {
        this->a = a;
        this->b = b;
        this->c = c;
    }
    ~triangle() {}
    
    void calcNormal() {
        nor = glm::normalize(glm::cross(b->pos - a->pos, c->pos - a->pos));
    }
    std::shared_ptr<vertex> getVetrtexByIdx(size_t idx) {
        switch (idx) {
            case 0: return a; break;
            case 1: return b; break;
            case 2: return c; break;
            default: break;
        }
        return 0;
    }
    glm::fvec2 getTextureByIdx(size_t idx) {
        switch (idx) {
            case 0: return a_tex; break;
            case 1: return b_tex; break;
            case 2: return c_tex; break;
            default: break;
        }
        return 0;
    }
        
    std::shared_ptr<vertex> a;
    std::shared_ptr<vertex> b;
    std::shared_ptr<vertex> c;
    
    glm::fvec2 a_tex;
    glm::fvec2 b_tex;
    glm::fvec2 c_tex;
    
    glm::fvec3 nor = { 0.0f, 0.0f, 0.0f };
};

class surface {
    surface() {}
    ~surface() {}
    
    void create(glm::fvec2 offs, Uint16 w, Uint16 h, float step) {
        width = w;
        height = h;
        
        for (Uint16 y = 0; y < h; y++) {
            for (Uint16 x = 0; x < w; x++) {
                auto v = std::make_shared<vertex>(glm::fvec3(offs.x + x * step, offs.y + y * step, 0.0f));
                vertices.push_back(v);
            }
        }
        for (Uint16 y = 0; y < h; y++) {
            for (Uint16 x = 0; x < w; x++) {
                size_t a_idx = x + y * h;
                size_t b_idx = x + y * h + 1;
                size_t c_idx = x + y * (h + 1) + 1;
                size_t d_idx = x + y * (h + 1);
                
                float w_step = 1.0f / w;
                float h_step = 1.0f / h;
                
                glm::fvec2 a_tex(w_step * x,        h_step * h);
                glm::fvec2 b_tex(w_step * (x + 1),  h_step * h);
                glm::fvec2 c_tex(w_step * (x + 1),  h_step * (h + 1));
                glm::fvec2 d_tex(w_step * x,        h_step * (h + 1));
                
                auto tri1 = std::make_shared<triangle>(vertices[a_idx], vertices[b_idx], vertices[c_idx]);
                vertices[a_idx].triangles.push_back(tri1);
                vertices[b_idx].triangles.push_back(tri1);
                vertices[c_idx].triangles.push_back(tri1);
                
                tri1.a_tex = a_tex;
                tri1.b_tex = b_tex;
                tri1.c_tex = c_tex;
                
                triangles.push_back(tri1);
                
                auto tri2 = std::make_shared<triangle>(vertices[a_idx], vertices[c_idx], vertices[d_idx]);
                vertices[a_idx].triangles.push_back(tri2);
                vertices[c_idx].triangles.push_back(tri2);
                vertices[d_idx].triangles.push_back(tri2);
                
                tri1.a_tex = a_tex;
                tri1.b_tex = c_tex;
                tri1.c_tex = d_tex;
                
                triangles.push_back(tri2);
            }
        }
    }
    
    size_t getRenderVertexSize() {
        return triangles * 3;
    }
    
    vertex& getVertex(size_t idx) {
        size_t triangleIdx = idx / 3;
        return triangles[triangleIdx]->getVertexByIdx(idx - triangleIdx * 3);
    }
    glm::fvec2 getTextureByIdx(size_t idx) {
        size_t triangleIdx = idx / 3;
        return triangles[triangleIdx]->getTextureByIdx(idx - triangleIdx * 3);
    }
    
    glm::fvec3 getVertexPos(size_t idx) {
        return getVertex(idx)->pos;
    }
    glm::fvec3 getVertexNor(size_t idx) {
        return getVertex(idx)->nor;
    }
    glm::fvec3 getVertexTex(size_t idx) {
        return getTextureByIdx(idx)->nor;
    }
    
    std::vector<std::shared_ptr<vertex>> vertices;
    std::vector<std::shared_ptr<triangle>> triangles;
    
    size_t width = 0;
    size_t height = 0;
}


};