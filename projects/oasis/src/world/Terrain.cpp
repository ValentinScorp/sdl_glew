#include "../Precompiled.h"

Terrain::Terrain() {
}

Terrain::~Terrain() {
}

void Terrain::createCanvasMesh(std::shared_ptr<IniFile> cfg) {
    Uint16 wPatchesNum = cfg->getParameter("Terrain", "wPatchesNum").toInt();
    Uint16 hPatchesNum = cfg->getParameter("Terrain", "hPatchesNum").toInt();
    Uint16 wTilesInPatch = cfg->getParameter("Terrain", "wTilesInPatch").toInt();
    Uint16 hTilesInPatch = cfg->getParameter("Terrain", "hTilesInPatch").toInt();
    float tileSize = cfg->getParameter("Terrain", "tileSize").toFloat();

    surface.create(wTilesInPatch, hTilesInPatch, wPatchesNum, hPatchesNum, tileSize);
    surface.recalcTriangleNormals();
    surface.recalcVertexNormals();
}

void Terrain::getDataFromSurface() {
    
    vertexes.clear();
    size_t vertexesTotal = surface.getRenderVertexSize();
    for (size_t i = 0; i < vertexesTotal; i++) {
        Vertex v;
        v.pos = surface.getVertexPos(i);
        //v.nor = surface.getVertexNor(i);
        v.nor = surface.getTriangleNor(i);
        v.tex0 = surface.getVertexTex0(i);
        v.tex1 = surface.getVertexTex1(i);
        vertexes.push_back(v);
    }
}

void Terrain::init(std::shared_ptr<Renderer> renderer) {
    mRenderer = renderer;
    renderer->terrain = this;
    
    glSandTex = renderer->loadTexture("img/terrainSand.png");
    glGrassTex = renderer->loadTexture("img/terrainGrass.png");
    glRockTex = renderer->loadTexture("img/terrainRock.png");
    glAlphaSide = renderer->loadTexture("img/alphaSide.png");
    glAlphaCorner = renderer->loadTexture("img/alphaCornerNew.png");
    glAlphaCornerNew = renderer->loadTexture("img/alphaCornerNew.png");
    glAlphaFull = renderer->loadTexture("img/alphaFull.png");
    glAlphaDiag = renderer->loadTexture("img/alphaDiag.png");
    
    auto cfg = std::make_shared<IniFile>("data/config.ini");
    
    glProgram = renderer->createProgram("data/" + cfg->getParameter("Terrain", "vertexShader").value, 
                                        "data/" + cfg->getParameter("Terrain", "fragmentShader").value);
    
    createCanvasMesh(cfg);
    getDataFromSurface();
    
    glModelMatrixUniform = renderer->getParamFromProgram(glProgram, "modelMatrix");
    glUniform_ligthDirection = renderer->getParamFromProgram(glProgram, "ligthDirection");
    glUniform_lightIntensity = renderer->getParamFromProgram(glProgram, "lightIntensity");
    glUniform_ambientIntensity = renderer->getParamFromProgram(glProgram, "ambientIntensity");
    
    glCameraMatricesUbo = renderer->createUbo(glProgram, "cameraMatrices", sizeof(glm::mat4) * 2);
        
    glVbo = renderer->createVbo(vertexes.data(), vertexes.size() * sizeof(Vertex));
    glVao = renderer->createVao(glVbo, 3, 3, 2, 2, 0, sizeof(float));
    
    renderer->updateView(glCameraMatricesUbo);
    
    orientationMatrix = glm::mat4(1.0f);
    
    terrainBrush = std::make_unique<TerrainBrush>();
    float tileSize = cfg->getParameter("Terrain", "tileSize").toFloat();
    terrainBrush->init(renderer, tileSize);
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
    
    terrainBrush->destroy();
    terrainBrush.reset();
}

void Terrain::update() {
    
}

void Terrain::render() {
        
    if (gridView) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    glUseProgram(glProgram);
    mRenderer->updateView(glCameraMatricesUbo);
    
    glUniformMatrix4fv(glModelMatrixUniform, 1, GL_FALSE, glm::value_ptr(orientationMatrix));
        
    float intense = 0.2;
    float ambient = 1.0;
    glUniform3f(glUniform_ligthDirection, 1.0, 1.0, 1.0);
    glUniform4f(glUniform_lightIntensity, intense, intense, intense, 1.0f);
    glUniform4f(glUniform_ambientIntensity, ambient, ambient, ambient, 1.0f);
        
    glBindVertexArray(glVao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    
    //renderer->sendSubDataToVbo(glVbo, offset * sizeof(Vertex) * 6, vertexes.data(), vertexes.size() * sizeof(Vertex));
    size_t tilesMax = vertexes.size() / 6;
    
    for (size_t i = 0; i < tilesMax; i++) {
        
        size_t a_idx = surface.triangles[i * 2].getVertexIdx(0);
        size_t b_idx = surface.triangles[i * 2].getVertexIdx(1);
        size_t c_idx = surface.triangles[i * 2].getVertexIdx(2);
        size_t d_idx = surface.triangles[i * 2 + 1].getVertexIdx(2);
        
        mRenderer->sendTexture(0, glGrassTex, glProgram, "texture0");
        mRenderer->sendTexture(1, getGlTexture(surface.vertices[d_idx].terrainType), glProgram, "texture1");
        mRenderer->sendTexture(2, getGlTexture(surface.vertices[a_idx].terrainType), glProgram, "texture2");
        mRenderer->sendTexture(3, getGlTexture(surface.vertices[b_idx].terrainType), glProgram, "texture3");
        mRenderer->sendTexture(4, getGlTexture(surface.vertices[c_idx].terrainType), glProgram, "texture4");
        mRenderer->sendTexture(5, glAlphaCornerNew, glProgram, "textureAlpha");
        
        glDrawArrays(GL_TRIANGLES, i * 6, 6);
        
        mRenderer->undindTexture(5);
        mRenderer->undindTexture(4);
        mRenderer->undindTexture(3);
        mRenderer->undindTexture(2);
        mRenderer->undindTexture(1);
        mRenderer->undindTexture(0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    
    glBindVertexArray(0);
    glUseProgram(0);
    
    if (gridView) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    terrainBrush->render();
}

Uint8 Terrain::convertTextureName(std::string textureName) {
    if (textureName == "grass") {
        return 0;
    }
    if (textureName == "sand") {
        return 1;
    }
    if (textureName == "rock") {
        return 2;
    }
    if (textureName == "alpha") {
        return 3;
    }
    return 0;
}

std::string Terrain::convertTextureId(Uint8 id) {
    switch (id) {
        case 0: return "grass"; break;
        case 1: return "sand"; break;
        case 2: return "rock"; break;
        case 3: return "alpha"; break;
        default: break;
    }
    return "grass";
}


void Terrain::saveMap(std::string fileName)
{
    std::ofstream outFile(fileName.c_str(), std::ios::out);
    if (outFile) {
        for (auto& v: surface.vertices) {
            outFile << std::to_string(v.terrainType) << "," << std::to_string(v.pos.z) << "\n";
        }
        outFile.close();
    } else {
        std::cerr << "Unable to save file: " << fileName << std::endl;
    }
}

GLuint Terrain::getGlTexture(Uint8 id) {
    switch (id) {
        case 0: return glGrassTex; break;
        case 1: return glSandTex; break;
        case 2: return glRockTex; break;
        case 3: return glAlphaCornerNew; break;
        default: break;
    }
    return glGrassTex;
}

void Terrain::loadMap(std::string fileName)
{
    std::ifstream file(fileName);
    std::vector<std::string> fileContent;
    if (file.is_open()) {
        std::string line;
        while (file.good()) {
            line.clear();
            std::getline(file, line);
            fileContent.push_back(line);
        }
        file.close();
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to open file -> %s\n", fileName.c_str());
    }
    
    for (auto& l: fileContent) {
        std::stringstream line(l);
        std::string idStr;
        if (l.size() > 0) {
            auto i = &l - &fileContent[0];
            std::getline(line, idStr, ',');
            surface.vertices[i].terrainType = std::stoi(idStr);
            std::getline(line, idStr, '\n');
            surface.vertices[i].pos.z = std::stoi(idStr);
        }
    }
    
    updateRenderData();
}

size_t Terrain::getVertexIntersecIdx(glm::fvec2 mousePos) {
    RayVector camRay = mRenderer->camera->getVectorRay(mousePos.x, mousePos.y);
    
    for (auto &triangle: surface.triangles) {
        aux::ray ray;
        ray.begin = camRay.begin;
        ray.end = camRay.end;
        glm::fvec3 intersection;
        auto t = &triangle - &surface.triangles[0];
        if (surface.intersectRayTriangle(ray, t, intersection)) {
            return surface.getClosestPoint(t, intersection);
        }
    }
    return 0;
}

void Terrain::setSurfaceVertexTexure(glm::fvec2 mousePos, std::string texName) {
    size_t vIdx = getVertexIntersecIdx(mousePos);
    surface.vertices[vIdx].terrainType = 2;
}

void Terrain::setSurfaceVertexHeight(glm::fvec2 mousePos, float height) {
    size_t vIdx = getVertexIntersecIdx(mousePos);
    surface.vertices[vIdx].pos.z += height;
    
    updateRenderData();
}
void Terrain::updateRenderData() {
    surface.recalcTriangleNormals();
    surface.recalcVertexNormals();
    
    getDataFromSurface();
    
    glBindBuffer(GL_ARRAY_BUFFER, glVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexes.size() * sizeof(Vertex), vertexes.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Terrain::genCircle(glm::fvec2 center, float radius, float height) {
    for (float x = center.x - radius; x <= center.x + radius; x += 0.5) {
        float y1 = glm::sqrt(glm::pow(radius, 2) - glm::pow((x - center.x), 2)) + center.y;
        float y2 = center.y - (y1 - center.y);
        surface.setHeight(glm::fvec2(x, y1), height);
        surface.setHeight(glm::fvec2(x, y2), height);
    }
}

void Terrain::toggleGridView() {
    gridView = !gridView;
}

glm::fvec3 Terrain::getTerrainPoint(glm::fvec2 mousePos) {
    RayVector camRay = mRenderer->camera->getVectorRay(mousePos.x, mousePos.y);
    
    for (auto &triangle: surface.triangles) {
        aux::ray ray;
        ray.begin = camRay.begin;
        ray.end = camRay.end;
        glm::fvec3 intersection;
        auto t = &triangle - &surface.triangles[0];
        if (surface.intersectRayTriangle(ray, t, intersection)) {
            return intersection;
        }
    }
    return glm::fvec3(0.0f);
}

void Terrain::onMessage(IMessage *message) {
    if (message->getKeyPressed() == "left_mouse_button_pressed") {
        glm::fvec2 pos = message->getMousePosition();
     //   setSurfaceVertexTexure(pos, "sand");
     //   setSurfaceVertexHeight(pos, 1.0f);
    }
    if (message->getKeyPressed() == "w") {
        toggleGridView();
     //   setSurfaceVertexTexure(pos, "sand");
     //   setSurfaceVertexHeight(pos, 1.0f);
    }
    if (message->getMessage() == "Save Map") {
        saveMap("data/mapfile.txt");
    }
    if (message->getMessage() == "Load Map") {
        loadMap("data/mapfile.txt");
    }
    if (message->getKeyPressed() == "motion") {
        glm::fvec2 pos = message->getMousePosition();
        size_t vIdx = getVertexIntersecIdx(pos);
        if (vIdx != 0) {
       //     terrainBrush->posX = (surface.vertices[vIdx].pos.x);
       //    terrainBrush->posY = (surface.vertices[vIdx].pos.y);
       //     terrainBrush->posZ = (surface.vertices[vIdx].pos.z + 0.1f);
        }
    }
    if (message->getMessage() == "Generate") {
        glm::fvec2 center(surface.width * surface.step / 2, surface.height * surface.step / 2);
        std::vector<size_t> indexes;
        surface.getAreaIndexes(center, center.x / 2, indexes);
        for (auto &idx: indexes) {
            surface.vertices[idx].pos.z = -2;
        }
        updateRenderData();
    }
    
    if (message->getKeyPressed() == "right_mouse_button_pressed") {
        SMessageManager::getInstance().invokeMessage(new UnitMessage("unit_walk", getTerrainPoint(message->getMousePosition())));
        /*
        glm::fvec2 pos = message->getMousePosition();
        RayVector camRay = mRenderer->camera->getVectorRay(pos.x, pos.y);
    
        for (auto &triangle: surface.triangles) {
            aux::ray ray;
            ray.begin = camRay.begin;
            ray.end = camRay.end;
            glm::fvec3 intersection;
            auto t = &triangle - &surface.triangles[0];
            if (surface.intersectRayTriangle(ray, t, intersection)) {
                SMessageManager::getInstance().invokeMessage(new UnitMessage("unit_walk", intersection));
            }
        }*/
    }
}

