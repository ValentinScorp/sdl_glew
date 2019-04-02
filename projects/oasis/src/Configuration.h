#pragma once

#include "Precompiled.h"

class Configuration
{
    
    class Parameter {
    public:
        virtual float floatValue() = 0;
        virtual glm::fvec3 fvec3Value() = 0;
        virtual ~Parameter() = 0;
    };
    class Float : public Parameter {
    private:
        float v;
    public:
        float floatValue() { return v; }
        virtual glm::fvec3 fvec3Value() { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to get fvec3 value from float\n"); }
        Float(float iv) : v(iv) {}
        ~Float() {}
    };
    class Vec3 : public Parameter {
    private:
        glm::fvec3 v;
    public:
        float floatValue() { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to get float value from fvec3 -> %s\n"); }
        virtual glm::fvec3 fvec3Value() { return v; }
        Vec3(glm::fvec3 iv) : v(iv) {}
        ~Vec3() {}
    };
    class Section {
    public:
        Section() {
        }
        ~Section() {
        }
        std::string name;
        std::vector<std::pair<std::string, std::string>> parameters;
    };
public:
    Configuration(std::string fileName);
    virtual ~Configuration();
    
    void convertStrToVec3(std::string paramName, std::string paramValue, glm::vec3 *vec);
    void convertStrToFloat(std::string paramName, std::string paramValue, float *var);
    
    float getScreenAspectRatio();
    
    void getParameter(std::string sectionName, std::string parameter);
    
    float screenResolutionWidth = 640;
    float screenResolutionHeight = 480;
    
    glm::vec3 camPosition = {0.0f, 0.0f, 0.0f};
    glm::vec3 camUpVec = {0.0f, 0.0f, 0.0f};
    glm::vec3 camLookAt = {0.0f, 0.0f, 0.0f};
    float camNearPlane = 0;
    float camFarPlane = 0;
    
    std::vector<Section> sections;
};

