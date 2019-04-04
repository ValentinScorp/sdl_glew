 #include "Precompiled.h"

static SDL_Window *window = NULL;
static SDL_GLContext gl_context;

GLfloat rotationAngle = 0;

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
    
    float w = config.getParameter("Window", "width").toFloat();
    float h = config.getParameter("Window", "height").toFloat();
    window = SDL_CreateWindow("Window title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_OPENGL);
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
    
    if (SDL_GL_SetSwapInterval(1) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to set swap interval -> %s", SDL_GetError());
    }
    
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to init GLEW -> %s", glewGetErrorString(err));
        SDL_DestroyWindow(window);
        SDL_Quit();
        getch();
        return -1;
    }
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Using GLEW version -> %s\n", glewGetString(GLEW_VERSION));
  
    Renderer renderer;
    renderer.init(&config);
    
    //Mesh mesh();
    //mesh.loadSmaMesh("model/Cube.002.sma");
    
    RenderObject objectRoman;
    objectRoman.init(&renderer, &config, "Roman"); 
    objectRoman.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    objectRoman.mesh->BeginAnimation("Walk");

    Uint32 currentTime = SDL_GetTicks();
    Uint32 lastTime = currentTime;
    Uint32 deltaTime = currentTime - lastTime;
    
    glm::mat4 objectPositionMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        
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
                        }
                    }
                    if (event.key.keysym.sym == SDLK_e) {
                        
                    }
                    if (event.key.keysym.sym == SDLK_w) {
                    }
                    if (event.key.keysym.sym == SDLK_s) {
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
    
        objectRoman.update(rotationAngle);
        objectRoman.render();
    
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
