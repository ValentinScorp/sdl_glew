#include "Precompiled.h"

/*
2 ---- 3	B ---C			 B
|      |	|  /		  /	 |
1------4	A			A----C
*/
Terrain::Tile::Tile(glm::fvec3 point1, glm::fvec3 point2, glm::fvec3 point3, glm::fvec3 point4)
{
	triangle1.A.pos = point1;
	triangle1.B.pos = point3;
	triangle1.C.pos = point2;

	triangle2.A.pos = point1;
	triangle2.B.pos = point4;
	triangle2.C.pos = point3;

	triangle2.C.nor.x = 0;
	triangle2.C.nor.y = 0;
	triangle2.C.nor.z = 1;
	triangle1.A.nor = triangle1.B.nor = triangle1.C.nor = triangle2.A.nor = triangle2.B.nor = triangle2.C.nor;

	triangle1.A.tex0.x = 0; triangle1.A.tex0.y = 0;
	triangle1.B.tex0.x = 1; triangle1.B.tex0.y = 1;
	triangle1.C.tex0.x = 0; triangle1.C.tex0.y = 1;

	triangle2.A.tex0.x = 0; triangle2.A.tex0.y = 0;
	triangle2.B.tex0.x = 1; triangle2.B.tex0.y = 0;
	triangle2.C.tex0.x = 1; triangle2.C.tex0.y = 1;

	triangle1.A.tex1 = triangle1.A.tex0;
	triangle1.B.tex1 = triangle1.B.tex0;
	triangle1.C.tex1 = triangle1.C.tex0;

	triangle2.A.tex1 = triangle2.A.tex0;
	triangle2.B.tex1 = triangle2.B.tex0;
	triangle2.C.tex1 = triangle2.C.tex0;

}

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

	textureFront = 0;
	textureBack = 0;
	alphaRotation = 0;
}

Terrain::Tile::~Tile()
{
}

bool Terrain::Tile::Intersection(RayVector ray, glm::fvec3 & intersectionVertex)
{
	return intersectRayTriangle(ray, triangle1, intersectionVertex) | intersectRayTriangle(ray, triangle2, intersectionVertex);
}

std::vector<Terrain::Vertex>& Terrain::Tile::GetPoints()
{
	points.push_back(triangle1.A);
	points.push_back(triangle1.B);
	points.push_back(triangle1.C);

	points.push_back(triangle2.A);
	points.push_back(triangle2.B);
	points.push_back(triangle2.C);

	return points;
}

void Terrain::Tile::ClearPoints()
{
	points.clear();
}

void Terrain::Tile::SetTexFront(size_t textureIndex)
{
	textureFront = textureIndex;
}

void Terrain::Tile::SetTexBack(size_t textureIndex)
{
	textureBack = textureIndex;
}

void Terrain::Tile::SetTexAlpha(size_t texIndex)
{
	textureAlpha = texIndex;
}

void Terrain::Tile::SetAlphaRotaion(int arot)
{
	alphaRotation = arot;
}

size_t Terrain::Tile::GetTexFront()
{
	return textureFront;
}

size_t Terrain::Tile::GetTexBack()
{
	return textureBack;
}

size_t Terrain::Tile::GetTexAlpha()
{
	return textureAlpha;
}

int Terrain::Tile::GetAlphaRotation()
{
	return alphaRotation;
}


void Terrain::Tile::RotateAlpha(int rotateNum)
{
	alphaRotation = rotateNum;
}

bool Terrain::Tile::intersectRayTriangle(RayVector ray, Triangle triangle, glm::fvec3 &intersectionVertex)
{
	// находим вектора сторон треугольника
	glm::fvec3 u = triangle.B.pos - triangle.A.pos;
	glm::fvec3 v = triangle.C.pos - triangle.A.pos;

	// находим нормаль к треугольнику
	glm::fvec3 n = glm::normalize(glm::cross(u, v));

	if (n.x == 0 && n.y == 0 && n.z == 0) {
		return 0;                       // неверные параметры треугольника (либо точки на одной прямой, либо все в одной точке)
	}

	// вектор направления луча
	glm::fvec3 dir = ray.end - ray.begin;
	// векор от точки на треугольнике до начала луча
	glm::fvec3 w0 = ray.begin - triangle.A.pos;

    float a = glm::dot(n, w0);
    float b = glm::dot(n, dir);
	//float a = -D3DXVec3Dot(&n, &w0);
	//float b = D3DXVec3Dot(&n, &dir);

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

Terrain::Patch::Patch(int x, int y, int tilesDim, float tileSize)
{
	float xPos = x * tileSize * tilesDim;
	float yPos = y * tileSize * tilesDim;

	float k = 1.0f / tilesDim;

	int alphaRot[] = {	3, 3,	3, 0,
						2, 0,	0, 0, 
						2, 0,	0, 0, 
						2, 1,	1, 1 };

	size_t alphaTex[] = {1, 0, 0, 1,
						0, 3, 2, 0,
						0, 2, 2, 0, 
						1, 0, 0, 1};

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

			// убрать потом
			t.SetTexFront(1);
			t.SetTexBack(0);
			t.SetTexAlpha(alphaTex[j + i * tilesDim]);
			t.SetAlphaRotaion(alphaRot[j + i * tilesDim]);
			

			// -------------

			tiles.push_back(t);
		}
	}
}

Terrain::Terrain() {
}

Terrain::~Terrain() {
}

void Terrain::createCanvasMesh() {
    /*float td = 4;
    Vertex va(0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    Vertex vb(0, td, 0,  0, 0, 1, 0, 1, 0, 1);
    Vertex vc(td, td, 0,  0, 0, 1, 1, 1, 1, 1);
    Vertex vd(td, 0, 0,  0, 0, 1, 1, 0, 1, 0);
    
    vertexes.push_back(va); vertexes.push_back(vc); vertexes.push_back(vb);
    vertexes.push_back(va); vertexes.push_back(vd); vertexes.push_back(vc);
    
    	*/
    int width = 16;
    int height = 16;
    float tile = 4;
        
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Patch patch(i, j, 4, tile);
			tiles.insert(tiles.end(), patch.tiles.begin(), patch.tiles.end());
		}
	}
    
    for (auto t : tiles) {
		auto points = t.GetPoints();
		for (auto p : points) {
			vertexes.push_back(p);
		}
		t.ClearPoints();
	}
}

void Terrain::init(std::shared_ptr<Renderer> renderer, Configuration *cfg) {
    mRenderer = renderer;
    xPatchesNum = cfg->getParameter("Terrain", "xPatchesNum").toInt();
    yPatchesNum = cfg->getParameter("Terrain", "yPatchesNum").toInt();
    patchDimension = cfg->getParameter("Terrain", "patchDimension").toInt();
    tileDimension = cfg->getParameter("Terrain", "tileDimension").toFloat();
    
    glSandTex = renderer->loadTexture("img/terrainSand.png");
    glGrassTex = renderer->loadTexture("img/terrainGrass.png");
    glRockTex = renderer->loadTexture("img/terrainRock.png");
    glAlphaSide = renderer->loadTexture("img/alphaSide.png");
    glAlphaCorner = renderer->loadTexture("img/alphaCorner.png");
    glAlphaCornerNew = renderer->loadTexture("img/alphaCornerNew.png");
    glAlphaFull = renderer->loadTexture("img/alphaFull.png");
    glAlphaDiag = renderer->loadTexture("img/alphaDiag.png");
    
    glProgram = renderer->createProgram("data/" + cfg->getParameter("Terrain", "vertexShader").value, 
                                        "data/" + cfg->getParameter("Terrain", "fragmentShader").value);
    
    createCanvasMesh();
    
    glModelMatrixUniform = renderer->getParamFromProgram(glProgram, "modelMatrix");
    glCameraMatricesUbo = renderer->createUbo(glProgram, "cameraMatrices", sizeof(glm::mat4) * 2);

    glVbo = renderer->createVbo(vertexes.data(), vertexes.size() * sizeof(Vertex));
    glVao = renderer->createVao(glVbo, 3, 3, 2, 2, sizeof(float));
    
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

void Terrain::render() {
    glUseProgram(glProgram);
    glUniformMatrix4fv(glModelMatrixUniform, 1, GL_FALSE, glm::value_ptr(orientationMatrix));
    glBindBuffer(GL_ARRAY_BUFFER, glVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexes.size() * sizeof(Vertex), vertexes.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(glVao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, glGrassTex);
    glUniform1i(glGetUniformLocation(glProgram, "texture0"), 0);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, glSandTex);
    glUniform1i(glGetUniformLocation(glProgram, "texture1"), 1);
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, glRockTex);
    glUniform1i(glGetUniformLocation(glProgram, "texture2"), 2);
    
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, glAlphaSide);
    glUniform1i(glGetUniformLocation(glProgram, "textureAlphaSide"), 3);
    
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, glAlphaCorner);
    glUniform1i(glGetUniformLocation(glProgram, "textureAlphaCorner"), 4);
    
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, glAlphaFull);
    glUniform1i(glGetUniformLocation(glProgram, "textureAlphaFull"), 5);
    
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, glAlphaDiag);
    glUniform1i(glGetUniformLocation(glProgram, "textureAlphaDiag"), 6);
    
    glDrawArrays(GL_TRIANGLES, 0, vertexes.size());
       
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glBindVertexArray(0);
    glUseProgram(0);
}