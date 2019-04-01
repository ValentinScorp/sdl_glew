 #include "Precompiled.h"

static SDL_Window *window = NULL;
static SDL_GLContext gl_context;

const float plane1[] = {
    0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f,
    
    0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
};

const float plane2[] = {
    -1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f,
    -1.0f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f,
    
    -1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f,
};

glm::vec3 camPosition(0.0f, 0.0f, 1.0f);
glm::vec3 camUpVector(0.0f, 1.0f, 0.0f);
float camNearPlane = 0.1f;
float camFarPlane = 100.0f;

GLuint vao1;
GLuint vao2;
GLuint vao3;

GLuint matricesUbo;
GLuint matricesUbo2;
GLfloat rotationAngle = 0;

float fovy = 45.0f;

SDL_Surface* flipSdlSurfaceVertical(SDL_Surface* sfc) {
     SDL_Surface* result = SDL_CreateRGBSurface(sfc->flags, sfc->w, sfc->h,
         sfc->format->BytesPerPixel * 8, sfc->format->Rmask, sfc->format->Gmask,
         sfc->format->Bmask, sfc->format->Amask);
     const auto pitch = sfc->pitch;
     const auto pxlength = pitch*sfc->h;
     auto pixels = static_cast<Uint8*>(sfc->pixels) + pxlength;
     auto rpixels = static_cast<Uint8*>(result->pixels) ;
     for(auto line = 0; line < sfc->h; ++line) {
         memcpy(rpixels,pixels,pitch);
         pixels -= pitch;
         rpixels += pitch;
     }
     return result;
}

void loadFile(std::string fileName, std::string *fileContent) {
    std::ifstream file(fileName);
    if (file.is_open()) {
        std::string line;
        while (file.good()) {
            line.clear();
            std::getline(file, line);
            (*fileContent).append(line + '\n');
        }
        file.close();
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to open file -> %s\n", fileName.c_str());
    }  
}

void loadShader(std::string fileName, GLenum shaderType, GLuint *shaderHandler) {
    std::string stringShader;
    loadFile(fileName, &stringShader);
    
    (*shaderHandler) = glCreateShader(shaderType);
    const char *ptrStrVertexShader = stringShader.c_str();
    glShaderSource((*shaderHandler), 1, &ptrStrVertexShader, NULL);
    glCompileShader((*shaderHandler));
    GLint shaderCompileStatus;
    glGetShaderiv((*shaderHandler), GL_COMPILE_STATUS, &shaderCompileStatus);
    if (shaderCompileStatus == GL_FALSE) {
        GLint infoLogLength;
        glGetShaderiv((*shaderHandler), GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog((*shaderHandler), infoLogLength, NULL, strInfoLog);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Compile failure in vertex shader -> %s\n%s\n", fileName.c_str(), strInfoLog);
        delete[] strInfoLog;
    }
}

void unloadShader(GLuint *shaderHandler) {
    glDeleteShader(*shaderHandler);
    (*shaderHandler) = 0;
}

void createProgram(GLuint *program, GLuint vertexShader, GLuint fragmentShader) {
    (*program) = glCreateProgram();
    glAttachShader((*program), vertexShader);
    glAttachShader((*program), fragmentShader);
    glLinkProgram(*program);
    GLint linkProgramStatus = GL_FALSE;
    glGetProgramiv ((*program), GL_LINK_STATUS, &linkProgramStatus);
    if (linkProgramStatus == GL_FALSE) {
        GLint infoLogLength;
        glGetProgramiv((*program), GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog((*program), infoLogLength, NULL, strInfoLog);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Link shader program failure -> %s\n", strInfoLog);
        delete[] strInfoLog;
    }
    glDetachShader((*program), vertexShader);
    glDetachShader((*program), fragmentShader);
}

void getParamFromProgram(GLuint program, std::string paramName, GLuint *paramId) {
    (*paramId) = glGetUniformLocation(program, paramName.c_str());
}

void getUniformParamFromProgram(GLuint program, std::string paramName, GLsizeiptr size, GLuint *paramId) {
    int uniformBlockBinding = 0;
    GLuint uniformBlockIndex = glGetUniformBlockIndex(program, paramName.c_str());
    glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
    
    // generate matrices
    glGenBuffers(1, paramId);
    glBindBuffer(GL_UNIFORM_BUFFER, (*paramId));
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    glBindBufferRange(GL_UNIFORM_BUFFER, uniformBlockBinding, (*paramId), 0, size);
}

void genVbo(const GLvoid *data, GLsizeiptr size, GLuint *vboId) {
    glGenBuffers(1, vboId);
    glBindBuffer(GL_ARRAY_BUFFER, (*vboId));
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void genVao(GLuint vbo, GLsizeiptr attribNum1, GLsizeiptr attribNum2, GLsizeiptr attribNum3, GLsizeiptr componentSize, GLuint *vao) {
    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLsizeiptr vertexSize = (attribNum1 + attribNum2 + attribNum3) * componentSize;
    if (attribNum1) {
        glVertexAttribPointer(0, attribNum1, GL_FLOAT, GL_FALSE, vertexSize, 0);
    }
    if (attribNum2) {
        glVertexAttribPointer(1, attribNum2, GL_FLOAT, GL_FALSE, vertexSize, (void*)(attribNum1 * componentSize));
    }
    if (attribNum3) {
        glVertexAttribPointer(2, attribNum3, GL_FLOAT, GL_FALSE, vertexSize, (void*)((attribNum1 + attribNum2) * componentSize));
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void renderVao(GLuint vao, GLuint attrNum, GLuint tex, GLsizei vertNum) {
    glBindVertexArray(vao);
    if (attrNum > 0) {
        glEnableVertexAttribArray(0);
    }
    if (attrNum > 1) {
        glEnableVertexAttribArray(1);
    }
    if (attrNum > 2) {
        glEnableVertexAttribArray(2);
    }    
    glBindTexture(GL_TEXTURE_2D, tex);
    glDrawArrays(GL_TRIANGLES, 0, vertNum);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
}

void loadTexture(std::string fileName, GLuint *texture, bool flipImgVertical = 0) {
    // todo -> unify for all textures
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_image could'n init! SDL_image error -> %s\n", IMG_GetError());
    }
    // end
    SDL_Surface *surface = IMG_Load(fileName.c_str());
    if (surface == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load image %s! SDL error -> %s\n", fileName.c_str(), SDL_GetError());
    }
    int mode = 0;
    if (surface->format->BytesPerPixel == 3) {
        mode = GL_RGB;
    }
    if (surface->format->BytesPerPixel == 4) {
        mode = GL_RGBA;
    }
    
    SDL_Surface* flippedSurface = flipSdlSurfaceVertical(surface);
        
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, (*texture));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    
    // todo -> optimize this
    if (flipImgVertical == true) {
        glTexImage2D(GL_TEXTURE_2D, 0, mode, flippedSurface->w, flippedSurface->h, 0, mode, GL_UNSIGNED_BYTE, (GLvoid*)flippedSurface->pixels);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, (GLvoid*)surface->pixels);
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);
        
    SDL_FreeSurface(surface);
    SDL_FreeSurface(flippedSurface);
}

void updateCamera(Camera &cam, GLuint matricesIdInShader) {
    glm::mat4 viewMatrix = cam.getViewMatrix();
    glBindBuffer(GL_UNIFORM_BUFFER, matricesIdInShader);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    glm::mat4 camMatrix = cam.getCamMatrix();    
    glBindBuffer(GL_UNIFORM_BUFFER, matricesIdInShader);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

int main(int argc, char **argv)
{
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_ERROR);
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
    
    Configuration config("data/config.ini");
    
    SDL_version compiledVer;
    SDL_version linkedVer;

    SDL_VERSION(&compiledVer);
    SDL_GetVersion(&linkedVer);
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Compiled SDL version -> %d.%d.%d\n", compiledVer.major, compiledVer.minor, compiledVer.patch);
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Linking SDL version -> %d.%d.%d\n", linkedVer.major, linkedVer.minor, linkedVer.patch);    
 
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL -> %s", SDL_GetError());
        SDL_Quit();
        getch();
        return -1;
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    window = SDL_CreateWindow("Window title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, config.screenResolutionWidth, config.screenResolutionHeight, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL window -> %s", SDL_GetError());
        SDL_Quit();
        getch();
        return -1;
    }   
    
    gl_context = SDL_GL_CreateContext(window);
    if (gl_context == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create GL context -> %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        getch();
        return -1;        
    }
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "OpenGL version -> %s", glGetString(GL_VERSION));
    
    int ds;
    SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &ds);
    
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to init GLEW -> %s", glewGetErrorString(err));
        SDL_DestroyWindow(window);
        SDL_Quit();
        getch();
        return -1;
    }
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Using GLEW version -> %s\n", glewGetString(GLEW_VERSION));
  
    //scene objects
    // todo
    Camera camera;
    camera.init(&config);
    
    //GLuint textureId1 = loadTexture1();
    //GLuint textureId2 = loadTexture2();
   
    GLuint textureId1 = 0;
    GLuint textureId2 = 0;
    GLuint textureId3 = 0;
    loadTexture("img/castle.png", &textureId1, true);
    loadTexture("img/ukr.png", &textureId2, true);
    loadTexture("img/roman_tex_high.png", &textureId3, false);
   
    // model 1 ====================
    Model model;
    model.loadMesh("model/object.obj");
    GLuint vertexShader = 0;
    loadShader("data/shader.vert", GL_VERTEX_SHADER, &vertexShader);
    GLuint fragmentShader = 0;
    loadShader("data/shader.frag", GL_FRAGMENT_SHADER, &fragmentShader);
    GLuint program = 0;
    createProgram(&program, vertexShader, fragmentShader);
    unloadShader(&vertexShader);
    unloadShader(&fragmentShader);
   
    // model2 roman ================================
    Model model2;
    model2.loadSmaMesh("model/Cube.002.sma");
    GLuint smaVertexShader = 0;
    loadShader("data/smaShader.vert", GL_VERTEX_SHADER, &smaVertexShader);
    GLuint smaFragmentShader = 0;
    loadShader("data/smaShader.frag", GL_FRAGMENT_SHADER, &smaFragmentShader);
    GLuint smaProgram = 0;
    createProgram(&smaProgram, smaVertexShader, smaFragmentShader);
    unloadShader(&smaVertexShader);
    unloadShader(&smaFragmentShader);
    
    // get matrices from program
    // model1 =================================================    
    GLuint modelToWorldMatrixUnif = 0;
    getParamFromProgram(program, "modelToWorldMatrix", &modelToWorldMatrixUnif);    
    getUniformParamFromProgram(program, "GlobalMatrices", sizeof(glm::mat4) * 2, &matricesUbo);
    
    // model2 =================================================
    GLuint modelToWorldMatrixUnif2 = 0;
    getParamFromProgram(smaProgram, "modelToWorldMatrix", &modelToWorldMatrixUnif2);    
    getUniformParamFromProgram(smaProgram, "GlobalMatrices", sizeof(glm::mat4) * 2, &matricesUbo2);

   // generate VBO for objects and send vertex data to it
    // model1 =================================================    
    GLuint vbo1;
    genVbo(model.getVertexBufferData(), model.getVertexBufferSize(), &vbo1);      
    GLuint vbo2;
    genVbo(plane2, sizeof(plane2), &vbo2);
    
    // model2 roman =================================================    
    GLuint vbo3;
    genVbo(model2.getVertexBufferData(), model2.getVertexBufferSize(), &vbo3);      
    //genVbo(model2.smaVerts.data(), model2.smaVerts.size() * sizeof(SmaVertex), &vbo3);
    
    // generate VAO and link VBO and it's internal data layout to it    
    // model1 =================================================    
    genVao(vbo1, 4, 2, 0, sizeof(float), &vao1);
    genVao(vbo2, 4, 2, 0, sizeof(float), &vao2);
    
    // model2 =================================================    
    genVao(vbo3, 3, 3, 2, sizeof(float), &vao3);    
       
    SDL_GL_SetSwapInterval(1);
        
    Uint32 currentTime = SDL_GetTicks();
    Uint32 lastTime = currentTime;
    Uint32 deltaTime = currentTime - lastTime;
    
    updateCamera(camera, matricesUbo);
    updateCamera(camera, matricesUbo2);
    
    // depth stuff    
   // glAlphaFunc(GL_GREATER, 0.0);
    //glEnable(GL_ALPHA_TEST);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    
    glEnable(GL_DEPTH_TEST);    
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glDepthRange(0.1f, 100.0f);    
    
    glm::mat4 objectPositionMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    
    // init gl error check
    GLenum err1;
    while ((err1 = glGetError()) != GL_NO_ERROR) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "OpenGL init error -> %d\n", err1);
    }
    
    bool runMainLoop = true;
    while (runMainLoop) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT: 
                    runMainLoop = false;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_q) {
                        if (event.key.keysym.mod & KMOD_SHIFT) {
                            // shift modifier
                            //printf("Q\n");
                        } else {
                            //printf("q\n");
                            fovy += 1.0;
                            printf("%f\n", fovy);
                            // fill view + cam matrices
                            glm::mat4 viewMatrix = glm::perspective(glm::radians(fovy), config.getScreenAspectRatio(), camNearPlane, camFarPlane);
                                                        
                            glBindBuffer(GL_UNIFORM_BUFFER, matricesUbo);
                            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
                            glBindBuffer(GL_UNIFORM_BUFFER, 0);
                        }
                    }
                    if (event.key.keysym.sym == SDLK_e) {
                        fovy -= 1.0;
                        glm::mat4 viewMatrix = glm::perspective(glm::radians(fovy), config.getScreenAspectRatio(), camNearPlane, camFarPlane);
                        glBindBuffer(GL_UNIFORM_BUFFER, matricesUbo);
                        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
                        glBindBuffer(GL_UNIFORM_BUFFER, 0);
                        printf("%f\n", fovy);
                    }
                    if (event.key.keysym.sym == SDLK_w) {
                        camera.moveUp();
                        updateCamera(camera, matricesUbo);
                    }
                    if (event.key.keysym.sym == SDLK_s) {
                        camera.moveDown();
                        updateCamera(camera, matricesUbo);
                    }
                    break;
                default:
                    break;
            }
        }
        
        currentTime = SDL_GetTicks(); 
        deltaTime = currentTime - lastTime;       
        lastTime = currentTime;        
        //SDL_Log("%d - %d = %d\n", currentTime, lastTime, deltaTime);        
        
        SDL_GL_MakeCurrent(window, gl_context);

        glClearColor(0.0f, 0.8f, 0.8f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                
        // model 1 object 2 (coub)
        glUseProgram(program);
        rotationAngle += 0.01;
        objectPositionMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
        objectPositionMatrix = glm::rotate(objectPositionMatrix, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(objectPositionMatrix));        
        renderVao(vao1, 2, textureId2, model.getVertexLen());        
        glUseProgram(0);
        
        // model2 (roman) =======================
        model2.update(rotationAngle);
        glUseProgram(smaProgram);
        objectPositionMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -20.0f));        
        objectPositionMatrix = glm::rotate(objectPositionMatrix, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        objectPositionMatrix = glm::rotate(objectPositionMatrix, 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        
        glUniformMatrix4fv(modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(objectPositionMatrix));
        
        //model2.smaVerts.data(), model2.smaVerts.size() * sizeof(SmaVertex), &vbo3
        glBindBuffer(GL_ARRAY_BUFFER, vbo3);
        glBufferSubData(GL_ARRAY_BUFFER, 0, model2.getVertexBufferSize(), model2.getVertexBufferData());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        renderVao(vao3, 3, textureId3, model2.smaVerts.size());
        glUseProgram(0);
        
        // model 1 object 1 (castle) =====================
        glUseProgram(program);        
        objectPositionMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
        glUniformMatrix4fv(modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(objectPositionMatrix));        
        renderVao(vao2, 2, textureId1, 6);        
        glUseProgram(0);

        SDL_GL_SwapWindow(window);

        SDL_Delay(2);
        
        GLenum err2;
        while ((err2 = glGetError()) != GL_NO_ERROR) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "OpenGL cycle error -> %d\n", err2);
        }
    }
        
    SDL_DestroyWindow(window); 
    SDL_Quit();
    
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "To quit application press any key ...\n");
    
    getch();
    return 0;
}
