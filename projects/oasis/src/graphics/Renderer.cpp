#include "../Precompiled.h"

Renderer::Renderer()
{
}

Renderer::~Renderer() {
    if (camera)
        delete camera;
}

void Renderer::init(std::shared_ptr<IniFile> config) {
    camera = new Camera(shared_from_this());
    camera->init(config);
    
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_image could'n init! SDL_image error -> %s\n", IMG_GetError());
    }
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glDepthRange(camera->nearPlane, camera->farPlane);
}

SDL_Surface* flipSdlSurfaceVertical(SDL_Surface* sfc) {
    SDL_Surface* result = SDL_CreateRGBSurface(sfc->flags, sfc->w, sfc->h,
        sfc->format->BytesPerPixel * 8, sfc->format->Rmask, sfc->format->Gmask,
        sfc->format->Bmask, sfc->format->Amask);
    const auto pitch = sfc->pitch;
    const auto pxlength = pitch * sfc->h;
    auto pixels = static_cast<Uint8*>(sfc->pixels) + pxlength;
    auto rpixels = static_cast<Uint8*>(result->pixels) ;
    for(auto line = 0; line < sfc->h; ++line) {
        memcpy(rpixels, pixels, pitch);
        pixels -= pitch;
        rpixels += pitch;
    }
    SDL_FreeSurface(sfc);
    sfc = result;
    return result;
}

GLuint Renderer::createTexture(GLsizei width, GLsizei height, GLenum internalFormat, GLenum format, GLvoid *data) {
    GLuint glTexture = 0;
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
      
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, (GLvoid*)data);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return glTexture;
}

GLuint Renderer::loadTexture(std::string fileName, bool flipVertical) {
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
    SDL_Surface * surf;
    if (flipVertical) {
        surf = flipSdlSurfaceVertical(surface);
    } else {
        surf = surface;
    }
    GLuint glTexture = createTexture(surf->w, surf->h, mode, mode, (GLvoid*)surf->pixels);
    
    SDL_FreeSurface(surf);
     
    return glTexture;
}

void Renderer::unloadTexture(GLuint glTexture) {
    glDeleteTextures(1, &glTexture);
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

GLuint loadShader(std::string fileName, GLenum shaderType) {
    
    std::string stringShader;
    loadFile(fileName, &stringShader);
    
    GLuint glShader = glCreateShader(shaderType);
    const char *ptrStrVertexShader = stringShader.c_str();
    glShaderSource(glShader, 1, &ptrStrVertexShader, NULL);
    glCompileShader(glShader);
    GLint shaderCompileStatus;
    glGetShaderiv(glShader, GL_COMPILE_STATUS, &shaderCompileStatus);
    if (shaderCompileStatus == GL_FALSE) {
        GLint infoLogLength;
        glGetShaderiv(glShader, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(glShader, infoLogLength, NULL, strInfoLog);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Compile failure in vertex shader -> %s\n%s\n", fileName.c_str(), strInfoLog);
        delete[] strInfoLog;
    }
    return glShader;
}

GLuint Renderer::createProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint glProgram = glCreateProgram();
    glAttachShader(glProgram, vertexShader);
    glAttachShader(glProgram, fragmentShader);
    glLinkProgram(glProgram);
    GLint linkProgramStatus = GL_FALSE;
    glGetProgramiv (glProgram, GL_LINK_STATUS, &linkProgramStatus);
    if (linkProgramStatus == GL_FALSE) {
        GLint infoLogLength;
        glGetProgramiv(glProgram, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(glProgram, infoLogLength, NULL, strInfoLog);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Link shader program failure -> %s\n", strInfoLog);
        delete[] strInfoLog;
    }
    glDetachShader(glProgram, vertexShader);
    glDetachShader(glProgram, fragmentShader);
    
    return glProgram;
}

GLuint Renderer::createProgram(std::string vertexShaderFile, std::string fragmentShaderFile) {
        
    GLuint vertexShader = loadShader(vertexShaderFile, GL_VERTEX_SHADER);
    GLuint fragmentShader = loadShader(fragmentShaderFile, GL_FRAGMENT_SHADER);
    
    GLuint glProgram = createProgram(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return glProgram;
}

void Renderer::destroyProgram(GLuint glProgram) {
    glDeleteProgram(glProgram);
}

GLuint Renderer::getParamFromProgram(GLuint program, std::string paramName) {
    return glGetUniformLocation(program, paramName.c_str());
}

GLuint Renderer::createUbo(GLuint program, std::string paramName, GLsizeiptr size) {
    
    GLuint uniformBlockIndex = glGetUniformBlockIndex(program, paramName.c_str());
    glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBindingCounter);
        
    GLuint glUniformBufferObject = 0;
    glGenBuffers(1, &glUniformBufferObject);
    glBindBuffer(GL_UNIFORM_BUFFER, glUniformBufferObject);
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, uniformBlockBindingCounter, glUniformBufferObject, 0, size);
    uniformBlockBindingCounter++;
    return glUniformBufferObject;
}

GLuint Renderer::createVbo(const GLvoid *data, GLsizeiptr size) {
    GLuint glVbo = 0;
    glGenBuffers(1, &glVbo);
    glBindBuffer(GL_ARRAY_BUFFER, glVbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    return glVbo;
}

void Renderer::destroyBuffer(GLuint buffer) {
    glDeleteBuffers(1, &buffer);
}

GLuint Renderer::createVao(GLuint glVbo, GLsizeiptr attribNum1, GLsizeiptr attribNum2, GLsizeiptr attribNum3, GLsizeiptr attribNum4, GLsizeiptr attribNum5, GLsizeiptr componentSize) {
    GLuint glVao = 0;
    glGenVertexArrays(1, &glVao);
    glBindVertexArray(glVao);
    glBindBuffer(GL_ARRAY_BUFFER, glVbo);
    GLsizeiptr vertexSize = (attribNum1 + attribNum2 + attribNum3 + attribNum4 + attribNum5) * componentSize;
    if (attribNum1) {
        glVertexAttribPointer(0, attribNum1, GL_FLOAT, GL_FALSE, vertexSize, 0);
    }
    if (attribNum2) {
        glVertexAttribPointer(1, attribNum2, GL_FLOAT, GL_FALSE, vertexSize, (void*)(attribNum1 * componentSize));
    }
    if (attribNum3) {
        glVertexAttribPointer(2, attribNum3, GL_FLOAT, GL_FALSE, vertexSize, (void*)((attribNum1 + attribNum2) * componentSize));
    }
    if (attribNum4) {
        glVertexAttribPointer(3, attribNum4, GL_FLOAT, GL_FALSE, vertexSize, (void*)((attribNum1 + attribNum2  + attribNum3) * componentSize));
    }
    if (attribNum5) {
        glVertexAttribPointer(4, attribNum5, GL_FLOAT, GL_FALSE, vertexSize, (void*)((attribNum1 + attribNum2  + attribNum3  + attribNum4) * componentSize));
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    return glVao;
}

void Renderer::sendSubDataToVbo(GLuint glVbo, GLintptr offset, void* data, GLsizeiptr size) {
    glBindBuffer(GL_ARRAY_BUFFER, glVbo);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::sendTexture(Uint16 texNum, GLuint glTexture, GLuint glProgram, std::string textureNameInProgram) {
    GLenum textureUnit = GL_TEXTURE0;
    switch(texNum) {
        case 0: textureUnit = GL_TEXTURE0; break;
        case 1: textureUnit = GL_TEXTURE1; break;
        case 2: textureUnit = GL_TEXTURE2; break;
        case 3: textureUnit = GL_TEXTURE3; break;
        case 4: textureUnit = GL_TEXTURE4; break;
        case 5: textureUnit = GL_TEXTURE5; break;
        case 6: textureUnit = GL_TEXTURE6; break;
        case 7: textureUnit = GL_TEXTURE7; break;
        default: break;
    }
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glUniform1i(glGetUniformLocation(glProgram, textureNameInProgram.c_str()), texNum);
}

void Renderer::undindTexture(Uint16 texNum) {
    switch(texNum) {
        case 0: glActiveTexture(GL_TEXTURE0); break;
        case 1: glActiveTexture(GL_TEXTURE1); break;
        case 2: glActiveTexture(GL_TEXTURE2); break;
        case 3: glActiveTexture(GL_TEXTURE3); break;
        case 4: glActiveTexture(GL_TEXTURE4); break;
        case 5: glActiveTexture(GL_TEXTURE5); break;
        case 6: glActiveTexture(GL_TEXTURE6); break;
        case 7: glActiveTexture(GL_TEXTURE7); break;
        default: break;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}


void Renderer::destroyVertexArrays(GLuint vertexArray) {
    glDeleteVertexArrays(1, &vertexArray);
}

void Renderer::updateView(GLuint glUboMatricesInShader) {
    if (camera) {
        glm::mat4 viewMatrix = camera->getViewMatrix();
        glBindBuffer(GL_UNIFORM_BUFFER, glUboMatricesInShader);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        
        glm::mat4 camMatrix = camera->getCamMatrix();
        glBindBuffer(GL_UNIFORM_BUFFER, glUboMatricesInShader);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camMatrix));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}