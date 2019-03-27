#include "Configuration.h"

Configuration::Configuration(std::string fileName) {
    std::ifstream file(fileName);
    std::vector<std::string> fileContent;
    if (file.is_open()) {
        while (file.good()) {
            std::string line;
            std::getline(file, line);
            fileContent.push_back(line);
        }
        file.close();
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to open file -> %s\n", fileName.c_str());
    }
    std::string sectionName;
    for (auto line: fileContent) {
        if (line.size() > 1) {
            if (line[0] == '[') {
                sectionName = line.substr(1, (line.size() - 2));
            } else {
                if (line[0] != ';' && line[0] != '#') {
                    size_t delimPos = line.find('=');
                    std::string paramName = line.substr(0, delimPos);
                    std::string paramValue = line.substr(delimPos + 1, line.size());
                    if (sectionName == "Window") {
                        if (paramName == "width") {
                            screenResolutionWidth = std::atoi(paramValue.c_str());
                        }
                        if (paramName == "height") {
                            screenResolutionHeight = std::atoi(paramValue.c_str());
                        }
                    }
                    if (sectionName == "Camera") {
                        //position=0.0, 0.0, 1.0
                        //upVector=0.0, 1.0, 0.0
                        //nearPlane=0.1
                        //farPlane=100.0
                        if (paramName == "position") {
                            convertStrToVec3("[" + sectionName + "] " + paramName, paramValue, &camPosition);                            
                        }
                    }
                }
            }
        }
    }
}

Configuration::~Configuration()
{
}

float Configuration::getScreenAspectRatio() {
    return (screenResolutionWidth / screenResolutionHeight);
}

void Configuration::convertStrToVec3(std::string paramName, std::string paramValue, glm::vec3 *vec) {
    try {
        std::string::size_type firstComaPos = paramValue.find(',');
        std::string::size_type secondComaPos = paramValue.find(',', firstComaPos + 1);
        if (firstComaPos == std::string::npos || secondComaPos == std::string::npos) {
            throw std::exception();
        }
        std::string x = paramValue.substr(0, firstComaPos);
        std::string y = paramValue.substr(firstComaPos + 1, (secondComaPos - firstComaPos - 1));
        std::string z = paramValue.substr(secondComaPos + 1, (paramValue.size()- secondComaPos));
        (*vec).x = std::stof(x);
        (*vec).y = std::stof(y);
        (*vec).z = std::stof(z);
    } catch (std::exception &e) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Configuraion file error in -> %s ... using default value!\n", paramName.c_str());
    }
}

