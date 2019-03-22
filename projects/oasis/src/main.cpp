#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

#include <conio.h>
#include <stdio.h>
#include <vector>
#include <string>

#define SCREEN_RESOLUTION_W 800
#define SCREEN_RESOLUTION_H 600

static SDL_Window *window = NULL;
static SDL_GLContext gl_context;

const float plane[] = {
    0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
};

glm::vec3 camPosition(0.0f, 0.0f, 1.0f);
glm::vec3 camUpVector(0.0f, 1.0f, 0.0f);
float camNearPlane = 0.5f;
float camFarPlane = 10.0f;
float screenAspectRatio = SCREEN_RESOLUTION_W / SCREEN_RESOLUTION_H;

const float vertexPositions[] = {
    0.75f, 0.75f, 0.0f, 1.0f,
    0.75f, -0.75f, 0.0f, 1.0f,
    -0.75f, -0.75f, 0.0f, 1.0f,
};
GLuint positionBufferObject;
GLuint vao;

const std::string strVertexShader(
    "#version 330\n"
    "layout(location = 0) in vec4 position;\n"
    "layout(std140) uniform GlobalMatrices\n"
    "{\n"
    "   mat4 cameraToClipMatrix;\n"
    "   mat4 worldToCameraMatrix;\n"
    "};\n"
    "uniform mat4 modelToWorldMatrix;\n"
    "void main()\n"
    "{\n"
    "   vec4 temp = modelToWorldMatrix * position;\n"
    "   temp = worldToCameraMatrix * temp;\n"
    "   gl_Position = cameraToClipMatrix * temp;\n"
    "}\n"
);

const std::string strFragmentShader(
    "#version 330\n"
    "out vec4 outputColor;\n"
    "void main()\n"
    "{\n"
    "   outputColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\n"
);

static const int g_iGlobalMatricesBindingIndex = 0;

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
    
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        SDL_Log("Failed to init GLEW -> %s", glewGetErrorString(err));
        SDL_DestroyWindow(window);
        SDL_Quit();
        getch();
        return -1;
    }
    SDL_Log("Using GLEW version -> %s\n", glewGetString(GLEW_VERSION));
    
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
    GLuint matricesUbo;
    glGenBuffers(1, &matricesUbo);
    glBindBuffer(GL_UNIFORM_BUFFER, matricesUbo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    glBindBufferRange(GL_UNIFORM_BUFFER, g_iGlobalMatricesBindingIndex, matricesUbo, 0, sizeof(glm::mat4) * 2);
    
    // init vertex buffer
    glGenBuffers(1, &positionBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STREAM_DRAW);
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
    glDepthRange(0.0f, 10.0);
    
    glm::mat4 objectPositionMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,0.0f));    
    
    GLenum err1;
    while ((err1 = glGetError()) != GL_NO_ERROR) {
        SDL_Log("OpenGL init error -> %d\n", err1);
    }    
    
    bool runMainLoop = true;
    while (runMainLoop) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                runMainLoop = false;
            }
        }
        
        currentTime = SDL_GetTicks(); 
        deltaTime = currentTime - lastTime;       
        lastTime = currentTime;        
        //SDL_Log("%d - %d = %d\n", currentTime, lastTime, deltaTime);        
        
        SDL_GL_MakeCurrent(window, gl_context);

        glClearColor(0.0f, 0.8f, 0.8f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glViewport(0, 0, (GLsizei)SCREEN_RESOLUTION_W, (GLsizei)SCREEN_RESOLUTION_H);
        
        glUseProgram(program);
        
        glm::mat4 objectPositionMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
        glUniformMatrix4fv(modelToWorldMatrixUnif, 1, GL_FALSE, glm::value_ptr(objectPositionMatrix));
        
        glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(0);
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
    
    getch();
    return 0;
}
