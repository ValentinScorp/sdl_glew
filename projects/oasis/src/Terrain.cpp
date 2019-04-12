#include "Precompiled.h"

/*
2 ---- 3	B ---C			 B
|      |	|  /		  /	 |
1------4	A			A----C
*/

Terrain::Tile::Tile(Vertex point1, Vertex point2, Vertex point3, Vertex point4)
{
	triangle1.A.pos = point1.pos;
	triangle1.B.pos = point3.pos;
	triangle1.C.pos = point2.pos;

	triangle2.A.pos = point1.pos;
	triangle2.B.pos = point4.pos;
	triangle2.C.pos = point3.pos;

	triangle2.C.nor.x = 0;
	triangle2.C.nor.y = 0;
	triangle2.C.nor.z = 1;
	triangle1.A.nor = triangle1.B.nor = triangle1.C.nor = triangle2.A.nor = triangle2.B.nor = triangle2.C.nor;

	triangle1.A.tex1.x = 0; triangle1.A.tex1.y = 0;
	triangle1.B.tex1.x = 1; triangle1.B.tex1.y = 1;
	triangle1.C.tex1.x = 0; triangle1.C.tex1.y = 1;

	triangle2.A.tex1.x = 0; triangle2.A.tex1.y = 0;
	triangle2.B.tex1.x = 1; triangle2.B.tex1.y = 0;
	triangle2.C.tex1.x = 1; triangle2.C.tex1.y = 1;

	triangle1.A.tex0 = point1.tex0;
	triangle1.B.tex0 = point3.tex0;
	triangle1.C.tex0 = point2.tex0;

	triangle2.A.tex0 = point1.tex0;
	triangle2.B.tex0 = point4.tex0;
	triangle2.C.tex0 = point3.tex0;
    
    vertexes.push_back(triangle1.A);
	vertexes.push_back(triangle1.B);
	vertexes.push_back(triangle1.C);

	vertexes.push_back(triangle2.A);
	vertexes.push_back(triangle2.B);
	vertexes.push_back(triangle2.C);
}

Terrain::Tile::~Tile()
{
}

bool Terrain::Tile::intersection(RayVector ray, glm::fvec3 & intersectionVertex) {
	return intersectRayTriangle(ray, triangle1, intersectionVertex) | intersectRayTriangle(ray, triangle2, intersectionVertex);
}

std::vector<Terrain::Vertex>& Terrain::Tile::getVertexes()
{
	return vertexes;
}

void Terrain::Tile::ClearPoints()
{
	vertexes.clear();
}

void Terrain::Tile::setTexture(Uint16 textureNum, GLuint glTexture) {
    glTextures[textureNum] = glTexture;
}

bool Terrain::Tile::intersectRayTriangle(RayVector ray, Triangle triangle, glm::fvec3 &intersectionVertex)
{
	// находим вектора сторон треугольника
	glm::fvec3 u = triangle.B.pos - triangle.A.pos;
	glm::fvec3 v = triangle.C.pos - triangle.A.pos;

	// находим нормаль к треугольнику
	glm::fvec3 n = glm::cross(u, v);

	if (n.x == 0 && n.y == 0 && n.z == 0) {
		return 0;                       // неверные параметры треугольника (либо точки на одной прямой, либо все в одной точке)
	}

	// вектор направления луча
	glm::fvec3 dir = ray.end - ray.begin;
	// векор от точки на треугольнике до начала луча
	glm::fvec3 w0 = ray.begin - triangle.A.pos;

    float a = -glm::dot(n, w0);
    float b = glm::dot(n, dir);

	if (fabs(b) < 0.0001) {       // луч паралельный плоскости треугольника
		if (a == 0) { return 0; }   // луч лежит на плоскости треугольника
		else { return 0; }   // луч не на плоскости треугольника
	}
	// найдем точку пересечения луча с треугольником
	float r = a / b;
	if (r < 0.0) {
		return 0;                    // луч идет мимо треугольника
	}
	// для сегмента проверить также (r > 1.0) => нет пересечения

	intersectionVertex = ray.begin + dir * r;           // точка пересечения луча и плоскости
														// лежит ли точка в треугольнике
       
	float    uu, uv, vv, wu, wv, D;
	uu = glm::dot(u, u);
	uv = glm::dot(u, v);
	vv = glm::dot(v, v);
	glm::fvec3 w = intersectionVertex - triangle.A.pos;
	wu = glm::dot(w, u);
	wv = glm::dot(w, v);
	D = uv * uv - uu * vv;

	// найдем и проверим параметрические координаты
	float s = (uv * wv - vv * wu) / D;
	if (s < 0.0 || s > 1.0) {
		return 0;                   // точка вне треугольника
	}
	float t = (uv * wu - uu * wv) / D;
	if (t < 0.0 || (s + t) > 1.0) {
		return 0;                   // точка вне треугольника
	}

	return 1;
}
/*
    tile order:
    3 -- 2
    |    |
    4 -- 1
  
    patch order:
    16  15  14  13
    12  11  10  9
    8   7   6   5
    4   3   2   1
*/

Terrain::Patch::Patch(int x, int y, int tilesDim, float tileSize)
{
	float xPos = x * tileSize * tilesDim;
	float yPos = y * tileSize * tilesDim;

	float k = 1.0f / tilesDim;

	for (int i = 0; i < tilesDim; i++) {
		for (int j = 0; j < tilesDim; j++) {
			
			Vertex tp1, tp2, tp3, tp4;

			tp1.pos = { xPos + j * tileSize, yPos + i * tileSize, 0.0f };
			tp2.pos = { xPos + j * tileSize, yPos + (i + 1) * tileSize, 0.0f };
			tp3.pos = { xPos + (j + 1) * tileSize, yPos + (i + 1) * tileSize, 0.0f };
			tp4.pos = { xPos + (j + 1) * tileSize, yPos + i * tileSize, 0.0f };

			tp1.tex0.x = j * k;
			tp1.tex0.y = i * k;

			tp2.tex0.x = j * k;
			tp2.tex0.y = (i * k + k);

			tp3.tex0.x = (j * k + k);
			tp3.tex0.y = (i * k + k);

			tp4.tex0.x = (j * k + k);
			tp4.tex0.y = i * k;
            
			Tile t(tp1, tp2, tp3, tp4);
            
			tiles.push_back(t);
		}
	}
}

Terrain::Terrain() {
}

Terrain::~Terrain() {
}

void Terrain::createCanvasMesh() {
    int width = 16;
    int height = 16;
    float tileWidth = 4;
        
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Patch patch(i, j, 4, tileWidth);
			tiles.insert(tiles.end(), patch.tiles.begin(), patch.tiles.end());
		}
	}
    
    for (Terrain::Tile &t: tiles) {
        t.setTexture(0, glGrassTex);
        t.setTexture(1, glGrassTex);
        t.setTexture(2, glGrassTex);
        t.setTexture(3, glGrassTex);
        t.setTexture(4, glGrassTex);
        t.setTexture(5, glAlphaCornerNew);
    }
}

void Terrain::init(std::shared_ptr<Renderer> renderer, Configuration *cfg) {
    mRenderer = renderer;
    renderer->terrain = this;
    xPatchesNum = cfg->getParameter("Terrain", "xPatchesNum").toInt();
    yPatchesNum = cfg->getParameter("Terrain", "yPatchesNum").toInt();
    patchDimension = cfg->getParameter("Terrain", "patchDimension").toInt();
    tileDimension = cfg->getParameter("Terrain", "tileDimension").toFloat();
    
    glSandTex = renderer->loadTexture("img/terrainSand.png");
    glGrassTex = renderer->loadTexture("img/terrainGrass.png");
    glRockTex = renderer->loadTexture("img/terrainRock.png");
    glAlphaSide = renderer->loadTexture("img/alphaSide.png");
    glAlphaCorner = renderer->loadTexture("img/alphaCornerNew.png");
    glAlphaCornerNew = renderer->loadTexture("img/alphaCornerNew.png");
    glAlphaFull = renderer->loadTexture("img/alphaFull.png");
    glAlphaDiag = renderer->loadTexture("img/alphaDiag.png");
    
    glProgram = renderer->createProgram("data/" + cfg->getParameter("Terrain", "vertexShader").value, 
                                        "data/" + cfg->getParameter("Terrain", "fragmentShader").value);
    
    createCanvasMesh();
    
    glModelMatrixUniform = renderer->getParamFromProgram(glProgram, "modelMatrix");
    glCameraMatricesUbo = renderer->createUbo(glProgram, "cameraMatrices", sizeof(glm::mat4) * 2);

    glVbo = renderer->createVbo(0, tiles.size() * 6 * sizeof(Vertex));
    glVao = renderer->createVao(glVbo, 3, 3, 2, 2, 0, sizeof(float));
    
    renderer->updateView(glCameraMatricesUbo);
    
    orientationMatrix = glm::mat4(1.0f);
}

void Terrain::destroy() {
    mRenderer->unloadTexture(glSandTex);
    mRenderer->unloadTexture(glGrassTex);
    mRenderer->unloadTexture(glRockTex);
    mRenderer->unloadTexture(glAlphaSide);
    mRenderer->unloadTexture(glAlphaCorner);
    mRenderer->unloadTexture(glAlphaCornerNew);
    mRenderer->unloadTexture(glAlphaFull);
    mRenderer->unloadTexture(glAlphaDiag);
     
    mRenderer->destroyBuffer(glCameraMatricesUbo);
    mRenderer->destroyProgram(glProgram);
    mRenderer->destroyBuffer(glVao);
    mRenderer->destroyBuffer(glVbo);
}

void Terrain::update() {
    
}

void Terrain::Tile::render(std::shared_ptr<Renderer> renderer, GLuint glVbo, GLintptr offset, GLuint glProgram) {
    renderer->sendSubDataToVbo(glVbo, offset * sizeof(Vertex) * 6, vertexes.data(), vertexes.size() * sizeof(Vertex));
       
    renderer->sendTexture(0, glTextures[0], glProgram, "texture0");
    renderer->sendTexture(1, glTextures[1], glProgram, "texture1");
    renderer->sendTexture(2, glTextures[2], glProgram, "texture2");
    renderer->sendTexture(3, glTextures[3], glProgram, "texture3");
    renderer->sendTexture(4, glTextures[4], glProgram, "texture4");
    renderer->sendTexture(5, glTextures[5], glProgram, "textureAlpha");
    
    glDrawArrays(GL_TRIANGLES, offset * 6, vertexes.size());
    
    renderer->undindTexture(5);
    renderer->undindTexture(4);
    renderer->undindTexture(3);
    renderer->undindTexture(2);
    renderer->undindTexture(1);
    renderer->undindTexture(0);
}

void Terrain::render() {
    glUseProgram(glProgram);
    mRenderer->updateView(glCameraMatricesUbo);
    glUniformMatrix4fv(glModelMatrixUniform, 1, GL_FALSE, glm::value_ptr(orientationMatrix));
    
    glBindVertexArray(glVao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    
    int tileCounter = 0;
    for(auto& t: tiles) {
        t.render(mRenderer, glVbo, tileCounter, glProgram);
        tileCounter++;
    }
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    
    glBindVertexArray(0);
    glUseProgram(0);
}

glm::fvec3 Terrain::getTerrainIntersection(RayVector rv) {
	glm::fvec3 intersection = { 0.0f, 0.0f, 0.0f };

	for (auto t : tiles) {
		if (t.intersection(rv, intersection)) {
			return intersection;
		}
	}

	return glm::fvec3(0.0f, 0.0f, 0.0f);
}

bool Terrain::Tile::isPointOnTile(glm::fvec3 p) {
    if ((p.x > triangle1.A.pos.x && p.x < triangle1.B.pos.x) &&
			(p.y > triangle1.A.pos.y && p.y < triangle1.B.pos.y) ) {
		return 1;
	}
	return 0;
}

void Terrain::onMessage(IMessage *message) {
    if (message->getKeyPressed() == "left_mouse_button_pressed") {
        glm::fvec2 pos = message->getMousePosition();
        RayVector camRay = mRenderer->camera->getVectorRay(pos.x, pos.y);
		glm::fvec3 intersection = mRenderer->terrain->getTerrainIntersection(camRay);
        
        glm::fvec3 point2(intersection.x + 2.0f, intersection.y - 2.0f, 0.0);
        glm::fvec3 point1(intersection.x - 2.0f, intersection.y - 2.0f, 0.0);
        glm::fvec3 point3(intersection.x - 2.0f, intersection.y + 2.0f, 0.0);
        glm::fvec3 point4(intersection.x + 2.0f, intersection.y + 2.0f, 0.0);

        for (Tile &tile : tiles) {
            if (tile.isPointOnTile(point1))
                tile.setTexture(4, glSandTex);
            if (tile.isPointOnTile(point2))
                tile.setTexture(1, glSandTex);
            if (tile.isPointOnTile(point3))
                tile.setTexture(3, glSandTex);
            if (tile.isPointOnTile(point4))
                tile.setTexture(2, glSandTex);
        }
    }
}