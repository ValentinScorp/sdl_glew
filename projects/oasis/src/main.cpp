 #include "Precompiled.h"

#define SCREEN_RESOLUTION_W 800
#define SCREEN_RESOLUTION_H 600

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
    -1.0f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f,
    
    -1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
};

glm::vec3 camPosition(0.0f, 0.0f, 1.0f);
glm::vec3 camUpVector(0.0f, 1.0f, 0.0f);
float camNearPlane = 0.1f;
float camFarPlane = 100.0f;
float screenAspectRatio = (float)SCREEN_RESOLUTION_W / (float)SCREEN_RESOLUTION_H;

GLuint positionBufferObject1;
GLuint positionBufferObject2;
GLuint vao;

GLuint matricesUbo;
GLfloat rotationAngle = 0;

const std::string strVertexShader(
    "#version 330\n"
    "layout(location = 0) in vec4 position;\n"
    "layout(location = 1) in vec2 texCoord;\n"
    "out vec2 colorCoord;"
    "layout(std140) uniform GlobalMatrices\n"
    "{\n"
    "   mat4 cameraToClipMatrix;\n"
    "   mat4 worldToCameraMatrix;\n"
    "};\n"
    "uniform mat4 modelToWorldMatrix;\n"
    "void main()\n"
    "{\n"
    //"   vec4 temp = modelToWorldMatrix * position;\n"
    //"   temp = worldToCameraMatrix * temp;\n"
    //"   gl_Position = cameraToClipMatrix * temp;\n"
    "   gl_Position = cameraToClipMatrix * worldToCameraMatrix * modelToWorldMatrix * position;\n"
    "   colorCoord = texCoord;\n"
    "}\n"
);

const std::string strFragmentShader(
    "#version 330\n"
    "in vec2 colorCoord;\n"
    "uniform sampler2D colorTexture;\n"    
    "out vec4 outputColor;\n"    
    "void main()\n"
    "{\n"
    "   outputColor = texture(colorTexture, colorCoord);\n"
    "}\n"
);

static const int g_iGlobalMatricesBindingIndex = 0;
float fovy = 45.0f;

SDL_Surface* flipVertical(SDL_Surface* sfc) {
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

GLuint loadTexture1() {     
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        SDL_Log("SDL_image could'n init! SDL_image error -> %s\n", IMG_GetError());
    }    
    SDL_Surface *surface = IMG_Load("img/castle.png");
    if (surface == NULL) {
        SDL_Log("Unable to load image! SDL error -> %s\n", SDL_GetError());
    }
    int mode = 0;
    if (surface->format->BytesPerPixel == 3) {
        mode = GL_RGB;
    }
    if (surface->format->BytesPerPixel == 4) {
        mode = GL_RGBA;
    }
    
    SDL_Surface* flippedSurface = flipVertical(surface);   
    
    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
       
    glTexImage2D(GL_TEXTURE_2D, 0, mode, flippedSurface->w, flippedSurface->h, 0, mode, GL_UNSIGNED_BYTE, (GLvoid*)flippedSurface->pixels);
    
    glBindTexture(GL_TEXTURE_2D, 0);
        
    SDL_FreeSurface(surface);
    SDL_FreeSurface(flippedSurface);
    
    return textureId;
}

GLuint loadTexture2() {     
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        SDL_Log("SDL_image could'n init! SDL_image error -> %s\n", IMG_GetError());
    }    
    SDL_Surface *surface = IMG_Load("img/ukr.png");
    if (surface == NULL) {
        SDL_Log("Unable to load image! SDL error -> %s\n", SDL_GetError());
    }
    int mode = 0;
    if (surface->format->BytesPerPixel == 3) {
        mode = GL_RGB;
    }
    if (surface->format->BytesPerPixel == 4) {
        mode = GL_RGBA;
    }
    
    SDL_Surface* flippedSurface = flipVertical(surface);   
    
    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
       
    glTexImage2D(GL_TEXTURE_2D, 0, mode, flippedSurface->w, flippedSurface->h, 0, mode, GL_UNSIGNED_BYTE, (GLvoid*)flippedSurface->pixels);
    
    glBindTexture(GL_TEXTURE_2D, 0);
        
    SDL_FreeSurface(surface);
    SDL_FreeSurface(flippedSurface);
    
    return textureId;
}

int main(int argc, char **argv)
{    
    SDL_version compiledVer;
    SDL_version linkedVer;

    SDL_VERSION(&compiledVer);
    SDL_GetVersion(&linkedVer);
    SDL_Log("Compiled SDL version -> %d.%d.%d\n", compiledVer.major, compiledVer.minor, compiledVer.patch);
    SDL_Log("Linking SDL version -> %d.%d.%d\n", linkedVer.major, linkedVer.minor, linkedVer.patch);
 
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) != 0) {
        SDL_Log("Failed to initialize SDL -> %s", SDL_GetError());
        SDL_Quit();
        getch();
        return -1;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);    
    
    window = SDL_CreateWindow("Window title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_RESOLUTION_W, SCREEN_RESOLUTION_H, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        SDL_Log("Failed to create SDL window -> %s", SDL_GetError());
        SDL_Quit();
        getch();
        return -1;
    }   
    
    gl_context = SDL_GL_CreateContext(window);
    if (gl_context == NULL) {
        SDL_Log("Failed to create GL context -> %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        getch();
        return -1;        
    }
    SDL_Log("OpenGL version -> %s", glGetString(GL_VERSION));
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
    
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        SDL_Log("Failed to init GLEW -> %s", glewGetErrorString(err));
        SDL_DestroyWindow(window);
        SDL_Quit();
        getch();
        return -1;
    }
    SDL_Log("Using GLEW version -> %s\n", glewGetString(GLEW_VERSION));
      
    GLuint textureId1 = loadTexture1();
    GLuint textureId2 = loadTexture2();
    
    Model model;
    model.loadMesh("model/object.obj");
    
    // shader load
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *ptrStrVertexShader = strVertexShader.c_str();
    glShaderSource(vertexShader, 1, &ptrStrVertexShader, NULL);
    glCompileShader(vertexShader);    
    GLint vertexShaderCompileStatus;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderCompileStatus);
    if (vertexShaderCompileStatus == GL_FALSE) {
        GLint infoLogLength;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(vertexShader, infoLogLength, NULL, strInfoLog);
        SDL_Log("Compile failure in vertex shader -> \n%s\n", strInfoLog);
        delete[] strInfoLog;
    }
    
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *ptrStrFragmentShader = strFragmentShader.c_str();
    glShaderSource(fragmentShader, 1, &ptrStrFragmentShader, NULL);
    glCompileShader(fragmentShader);    
    GLint fragmentShaderCompileStatus;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderCompileStatus);
    if (fragmentShaderCompileStatus == GL_FALSE) {
        GLint infoLogLength;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(fragmentShader, infoLogLength, NULL, strInfoLog);
        SDL_Log("Compile failure in fragment shader -> \n%s\n", strInfoLog);
        delete[] strInfoLog;
    }
    
    // create and link shader program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    GLint linkProgramStatus = GL_FALSE;
    glGetProgramiv (program, GL_LINK_STATUS, &linkProgramStatus);
    if (linkProgramStatus == GL_FALSE) {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        SDL_Log("Link shader program failure -> %s\n", strInfoLog);
        delete[] strInfoLog;
    }
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
   
    // get matrices from program
    GLuint modelToWorldMatrixUnif = glGetUniformLocation(program, "modelToWorldMatrix");
    GLuint globalUniformBlockIndex = glGetUniformBlockIndex(program, "GlobalMatrices");    
    glUniformBlockBinding(program, globalUniformBlockIndex, g_iGlobalMatricesBindingIndex);
    
    // generate matrices    
    glGenBuffers(1, &matricesUbo);
    glBindBuffer(GL_UNIFORM_BUFFER, matricesUbo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    glBindBufferRange(GL_UNIFORM_BUFFER, g_iGlobalMatricesBindingIndex, matricesUbo, 0, sizeof(glm::mat4) * 2);
    
    // init vertex buffer
    glGenBuffers(1, &model.glBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, model.glBufferId);
    glBufferData(GL_ARRAY_BUFFER, model.getVertexBufferSize(), model.getVertexBufferData(), GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &positionBufferObject2);
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane2), plane2, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    SDL_GL_SetSwapInterval(1);
        
    Uint32 currentTime = SDL_GetTicks();
    Uint32 lastTime = currentTime;
    Uint32 deltaTime = currentTime - lastTime;
    
    // fill view + cam matrices
    glm::mat4 viewMatrix = glm::perspective(glm::radians(45.0f), screenAspectRatio, camNearPlane, camFarPlane);
    glBindBuffer(GL_UNIFORM_BUFFER, matricesUbo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    glm::mat4 camMatrix = glm::lookAt(camPosition, glm::vec3(0.0, 0.0, 0.0), camUpVector);
    glBindBuffer(GL_UNIFORM_BUFFER, matricesUbo);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    // depth stuff
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glDepthRange(0.1f, 100.0);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glm::mat4 objectPositionMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,0.0f));
    
    // init gl error check
    GLenum err1;
    while ((err1 = glGetError()) != GL_NO_ERROR) {
        SDL_Log("OpenGL init error -> %d\n", err1);
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
                            glm::mat4 viewMatrix = glm::perspective(glm::radians(fovy), screenAspectRatio, camNearPlane, camFarPlane);
                                                        
                            glBindBuffer(GL_UNIFORM_BUFFER, matricesUbo);
                            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
                            glBindBuffer(GL_UNIFORM_BUFFER, 0);
                        }
                    }
                    if (event.key.keysym.sym == SDLK_e) {
                        fovy -= 1.0;
                        glm::mat4 viewMatrix = glm::perspective(glm::radians(fovy), screenAspectRatio, camNearPlane, camFarPlane);
                        glBindBuffer(GL_UNIFORM_BUFFER, matricesUbo);
                        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
                        glBindBuffer(GL_UNIFORM_BUFFER, 0);
                        printf("%f\n", fovy);
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
                
        //glViewport(0, 0, (GLsizei)SCREEN_RESOLUTION_W, (GLsizei)SCREEN_RESOLUTION_H);
        
        
        // object 1
        glUseProgram(program);        
        
        glBindTexture(GL_TEXTURE_2D, textureId2);        
        rotationAngle += 0.01;
        objectPositionMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
        objectPositionMatrix = glm::rotate(objectPositionMatrix, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(objectPositionMatrix));
        
        glBindBuffer(GL_ARRAY_BUFFER, model.glBufferId);
        
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float)));
        glEnableVertexAttribArray(1);
     
        glDrawArrays(GL_TRIANGLES, 0, model.getVertexLen());
        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glUseProgram(0);
        
        // object 2
        glUseProgram(program);        
        
        glBindTexture(GL_TEXTURE_2D, textureId1);        
        
        objectPositionMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
        glUniformMatrix4fv(modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(objectPositionMatrix));
        
        glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject2);        
        
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float)));
        glEnableVertexAttribArray(1);
     
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glUseProgram(0);
        

        SDL_GL_SwapWindow(window);

        SDL_Delay(2);
        
        GLenum err2;
        while ((err2 = glGetError()) != GL_NO_ERROR) {
            SDL_Log("OpenGL cycle error -> %d\n", err2);
        }  
    }
        
    SDL_DestroyWindow(window); 
    SDL_Quit();
    
    SDL_Log("To quit application press any key ...\n");
    
    getch();
    return 0;
}
