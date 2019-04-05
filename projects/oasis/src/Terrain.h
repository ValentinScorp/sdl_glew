#pragma once

class Renderer;
class Configuration;

class Terrain
{
    class Vertex {
    public:
        Vertex() {}
        Vertex(Vertex const& v) {
            pos = v.pos; nor = v.nor; tex0 = v.tex0; tex1 = v.tex1;
        }
        Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 t0, glm::vec2 t1) {
            pos = p; nor = n; tex0 = t0; tex1 = t1;
        }
        Vertex(float x, float y, float z, float nx, float ny, float nz, float s0, float t0, float s1, float t1) {
            pos[0] = x; pos[1] = y; pos[2] = z;
            nor[0] = nx; nor[1] = ny; nor[2] = nz;
            tex0[0] = s0; tex0[1] = t0; tex1[0] = s1; tex1[1] = t1;
        }
        ~Vertex() {}
        Vertex& operator=(Vertex const& other) {
            if (this != &other) {
                pos = other.pos;
                nor = other.nor;
                tex0 = other.tex0;
                tex1 = other.tex1;
            }
            return *this;
        }
        void print() {
            SDL_Log("%f %f %f   %f %f %f   %f %f  %f %f\n", pos.x, pos.y, pos.z, nor.x, nor.y, nor.z, tex0.s, tex0.t, tex1.s, tex1.t);
        }
        
        glm::fvec3 pos;
        glm::fvec3 nor;
        glm::fvec2 tex0;
        glm::fvec2 tex1;
    };
    
    class Tile {
    private:
        struct Triangle {
            Vertex A;
            Vertex B;
            Vertex C;
        };

        std::vector <Vertex> points;

        Triangle triangle1;
        Triangle triangle2;

        size_t textureFront;
        size_t textureBack;
        size_t textureAlpha;
        int alphaRotation;
            
    public:
        Tile(glm::fvec3 point1, glm::fvec3 point2, glm::fvec3 point3, glm::fvec3 point4);
        Tile(Vertex point1, Vertex point2, Vertex point3, Vertex point4);
        ~Tile();

        bool Intersection(RayVector ray, glm::fvec3 &intersectionVertex);

        std::vector <Vertex> & GetPoints();
        void ClearPoints();

        void SetTexFront(size_t textureIndex);
        void SetTexBack(size_t textureIndex);
        void SetTexAlpha(size_t texIndex);
        void SetAlphaRotaion(int arot);

        size_t GetTexFront();
        size_t GetTexBack();
        size_t GetTexAlpha();
        int GetAlphaRotation();

        void RotateAlpha(int rotateNum);
        
    private:
        bool intersectRayTriangle(RayVector ray, Triangle triangle, glm::fvec3 &intersectionVertex);
    };
        
    class Patch {
    public:
        std::vector <Tile> tiles;
        
        Patch(int x, int y, int tilesDim, float tileSize);
        ~Patch() {}
    };
    
    
    std::vector<Tile> tiles;
    std::vector<Vertex> vertexes;
    
    Uint16 xPatchesNum = 0;
    Uint16 yPatchesNum = 0;
    Uint16 patchDimension = 0;
    float tileDimension = 0;
    
    GLuint glSandTex = 0;
	GLuint glGrassTex = 0;
	GLuint glRockTex = 0;
	
	GLuint glAlphaSide = 0;
	GLuint glAlphaCorner = 0;
	GLuint glAlphaCornerNew = 0;
	GLuint glAlphaFull = 0;
	GLuint glAlphaDiag = 0;
    
    GLuint glProgram = 0;
    GLuint glModelMatrixUniform = 0;
    GLuint glCameraMatricesUbo = 0;
    GLuint glVbo = 0;
    GLuint glVao = 0;
    
    glm::mat4 orientationMatrix;
    
    std::shared_ptr<Renderer> mRenderer = nullptr;
public:
    Terrain();
    virtual ~Terrain();
    
    void init(std::shared_ptr<Renderer> renderer, Configuration *cfg);
    void destroy();
    void update();
    void render();
    
    void createCanvasMesh();
};

