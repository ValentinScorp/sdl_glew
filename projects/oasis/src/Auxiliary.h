#pragma once

namespace aux {

class triangle;
    
class vertex {
public:
    vertex() {}
    vertex(vertex const& v) {
        pos = v.pos;
        nor = v.nor;
    }
    vertex(glm::fvec3 p, glm::fvec3 n) {
        pos = p;
        nor = n;
    }
    ~vertex() {}
    
    void calcNormal();
    
    glm::fvec3 pos = { 0.0f, 0.0f, 0.0f };
    glm::fvec3 nor = { 0.0f, 0.0f, 0.0f };
    Uint8 terrainType = 0;
    
    std::vector<size_t> triangles;
};
    
class triangle {
public:
    triangle(size_t a, size_t b, size_t c) {
        a_idx = a;
        b_idx = b;
        c_idx = c;
    }
    ~triangle() {}
    
    void calcNormal(std::vector<vertex> &v) {
        nor = glm::normalize(glm::cross(v[b_idx].pos - v[a_idx].pos, v[c_idx].pos - v[a_idx].pos));
    }
    size_t getVertexIdx(size_t idx) {
        switch (idx) {
            case 0: return a_idx; break;
            case 1: return b_idx; break;
            case 2: return c_idx; break;
            default: break;
        }
        return 0;
    }
    glm::fvec2 getTexture0(size_t idx) {
        switch (idx) {
            case 0: return a_tex0; break;
            case 1: return b_tex0; break;
            case 2: return c_tex0; break;
            default: break;
        }
        return a_tex0;
    }
    glm::fvec2 getTexture1(size_t idx) {
        switch (idx) {
            case 0: return a_tex1; break;
            case 1: return b_tex1; break;
            case 2: return c_tex1; break;
            default: break;
        }
        return a_tex1;
    }
        
    size_t a_idx;
    size_t b_idx;
    size_t c_idx;
        
    glm::fvec2 a_tex0, a_tex1;
    glm::fvec2 b_tex0, b_tex1;
    glm::fvec2 c_tex0, c_tex1;
    
    glm::fvec3 nor = { 0.0f, 0.0f, 0.0f };
};

class ray {
public:
    ray() {}
    ~ray() {}
    
    void shift(glm::fvec3 offs) {
        begin += offs;
        end += offs;
    }

    glm::fvec3 begin;
    glm::fvec3 end;
};

class surface {
public:
    surface() {}
    ~surface() {}

    void create(Uint16 patch_w, Uint16 patch_h, Uint16 w, Uint16 h, float tileStep);
    bool intersectRayTriangle(aux::ray ray, size_t tIdx, glm::fvec3 &intersectionVertex);
    float sign(glm::fvec2 p1, glm::fvec2 p2, glm::fvec2 p3);
    bool isPointOnTriangle(glm::fvec2 point, size_t tIdx);
    void setHeight(glm::fvec2 coord, float height);
    size_t findTriangle(glm::fvec2 coord);
    
    size_t getRenderVertexSize() {
        return triangles.size() * 3;
    }
    
    vertex& getVertex(size_t idx) {
        size_t triangleIdx = idx / 3;
        size_t vertexIdx = triangles[triangleIdx].getVertexIdx(idx - triangleIdx * 3);
        return vertices[vertexIdx];
    }
     glm::fvec2 getTexture0(size_t idx) {
        size_t triangleIdx = idx / 3;
        return triangles[triangleIdx].getTexture0(idx - triangleIdx * 3);
    }
    glm::fvec2 getTexture1(size_t idx) {
        size_t triangleIdx = idx / 3;
        return triangles[triangleIdx].getTexture1(idx - triangleIdx * 3);
    }
    
    glm::fvec3 getVertexPos(size_t idx) {
        return getVertex(idx).pos;
    }
    glm::fvec3 getVertexNor(size_t idx) {
        return getVertex(idx).nor;
    }
    glm::fvec3 getTriangleNor(size_t idx) {
        size_t triangleIdx = idx / 3;
        return triangles[triangleIdx].nor;
    }
    glm::fvec2 getVertexTex0(size_t idx) {
        return getTexture0(idx);
    }
    glm::fvec2 getVertexTex1(size_t idx) {
        return getTexture1(idx);
    }
    
    void getAreaIndexes(glm::fvec2 center, float radius, std::vector<size_t> &indexes);
    void createTriangle(size_t idx_a, size_t idx_b, size_t idx_c);
    bool isIntersected(aux::ray ray);
        
    size_t getClosestPoint(size_t tIdx, glm::fvec3 point);
    void recalcTriangleNormals();
    void recalcVertexNormals();
        
    std::vector<vertex> vertices;
    std::vector<triangle> triangles;
    
    size_t width = 0;
    size_t height = 0;
    float step = 0;
    
    glm::fvec3 position = { 0.0f, 0.0f, 0.0f };
};


};