#include "Precompiled.h"

class System : public IMessageRecipient  {
bool quit = false;
public:
    System() {}
    ~System() {}
    
    void onMessage(IMessage *message) {
        if (message) {
            quit = message->isQuit();
        }
    }
    bool isQuit() {
        return quit;
    }
};

class Time {
    private:
    Uint32 begin = 0;
public:
    Time() {
        begin = SDL_GetTicks();
    }
    ~Time() {}
    Uint32 getElapsed() {
        return SDL_GetTicks() - begin;
    }
    void restart() {
        begin = SDL_GetTicks();
    }
};


int main(int argc, char **argv)
{
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_ERROR);
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
    
    auto config = std::make_shared<IniFile>("data/config.ini");
    
    SDL_version compiledVer;
    SDL_version linkedVer;

    SDL_VERSION(&compiledVer);
    SDL_GetVersion(&linkedVer);
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Compiled SDL version -> %d.%d.%d\n", compiledVer.major, compiledVer.minor, compiledVer.patch);
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Linking SDL version -> %d.%d.%d\n", linkedVer.major, linkedVer.minor, linkedVer.patch);    
 
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL -> %s", SDL_GetError());
        SDL_Quit();    
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
    
    float w = config->getParameter("Window", "width").toFloat();
    float h = config->getParameter("Window", "height").toFloat();
    SDL_Window *window = SDL_CreateWindow("Window title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_OPENGL);
    
    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL window -> %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }
    
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (gl_context == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create GL context -> %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
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
        return -1;
    }
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Using GLEW version -> %s\n", glewGetString(GLEW_VERSION));
     
    System system;
        
    auto renderer = std::make_shared<Renderer>();
    renderer->init(config);
    
    GuiPanel guiPanel;
    guiPanel.init(renderer);

    auto aiContainer = std::make_shared<AiContainer>();
    
    Camera *cam = renderer->camera;
    aiContainer->init(renderer, cam, config);
    
    Console::getInstance().init(renderer, config);
    
    
    World world;
    world.init(renderer, aiContainer);
    //world.addObject("Tree", glm::fvec3(7.0f, 5.0f, 0.0f));
   // world.addObject("Tree", glm::fvec3(17.0f, 5.0f, 0.0f));
   // world.addObject("Tree", glm::fvec3(9.0f, 7.0f, 0.0f));
   // world.addObject("Tree", glm::fvec3(11.0f, 7.0f, 0.0f));
   // world.addObject("Tree", glm::fvec3(13.0f, 7.0f, 0.0f));
   // world.addObject("Tree", glm::fvec3(15.0f, 7.0f, 0.0f));
   // world.addObject("Tree", glm::fvec3(17.0f, 7.0f, 0.0f));
    
    
    GLenum err1;
    while ((err1 = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL init error -> " << err1 << std::endl;
    } 
    
    auto frameTime = std::make_shared<float>();
    Console::getInstance().attachFrameTime(frameTime);
    Time frame;
    
    bool runMainLoop = true;
    while (runMainLoop) {
        Uint32 elapsedTime = frame.getElapsed();
        frame.restart();
        (*frameTime) = (elapsedTime);
        
        sendEvents();
                
        if (system.isQuit() == true) {
            runMainLoop = false;
        }
        
        SDL_GL_MakeCurrent(window, gl_context);

        glClearColor(0.0f, 0.8f, 0.8f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                       
        aiContainer->update();
//std::cout << "updatein world start" << std::endl;
        
        world.update();
        //std::cout << "updatein world end" << std::endl;
      //  objectRoman.update(1.0f);
         
        world.render();
        aiContainer->render();
       // objectRoman.render();
        guiPanel.render();
        Console::getInstance().render();
        
        SDL_GL_SwapWindow(window);
       // SDL_Delay(2);
        
        GLenum err2;
        while ((err2 = glGetError()) != GL_NO_ERROR) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "OpenGL cycle error -> %d\n", err2);
        }
    }
    
    guiPanel.destroy();
    //objectRoman.destroy();    
    world.destroy();
    renderer->unloadObjects();
    Console::getInstance().destroy();
    aiContainer->destroy();
        
    GLenum errDelete;
    while ((errDelete = glGetError()) != GL_NO_ERROR) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "OpenGL delete error -> %d\n", errDelete);
    }
        
    SDL_DestroyWindow(window); 
    SDL_Quit();
    
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "To quit application press any key ...\n");
    
    return 0;
}
