#pragma once

class Configuration
{
    class Parameter {
    private:
        
        void errorDescr() {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Configuraion file error! Parameter -> %s ... using default value!\n", value.c_str());
        }
    public:
        std::string value;
            
        float toFloat() {
            float var = 0;
            try {
                var = std::stof(value);
            } catch (std::exception &e) {
                errorDescr();
            }
            return var;
        }
        glm::fvec3 toFvec3() {
            glm::fvec3 vec(0.0f);
            try {
                std::string::size_type firstComaPos = value.find(',');
                std::string::size_type secondComaPos = value.find(',', firstComaPos + 1);
                if (firstComaPos == std::string::npos || secondComaPos == std::string::npos) {
                    throw std::exception();
                }
                std::string x = value.substr(0, firstComaPos);
                std::string y = value.substr(firstComaPos + 1, (secondComaPos - firstComaPos - 1));
                std::string z = value.substr(secondComaPos + 1, (value.size()- secondComaPos));
                vec.x = std::stof(x);
                vec.y = std::stof(y);
                vec.z = std::stof(z);
            } catch (std::exception &e) {
                errorDescr();
            }
            return vec;
        }
        Parameter() {}
        Parameter(std::string str) : value(str) {}
        ~Parameter() {}
    };
    
    class Section {
    public:
        Section() {
        }
        ~Section() {
        }
        std::string name;
        std::vector<std::pair<std::string, Parameter>> parameters;
    };
    
public:
    Configuration(std::string fileName);
    virtual ~Configuration();
    
    void print();
    void convertStrToVec3(std::string paramName, std::string paramValue, glm::vec3 *vec);
    void convertStrToFloat(std::string paramName, std::string paramValue, float *var);
    
    float getScreenAspectRatio();
    
    Parameter getParameter(std::string sectionName, std::string parameter);
                
    std::vector<Section> sections;
};

