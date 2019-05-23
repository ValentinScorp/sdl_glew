#include "Precompiled.h"

IniFile::IniFile(std::string fileName) {
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
    std::string sectionName, sectionName_old;
    Section sect;
    for (auto line: fileContent) {
        if (line.size() > 1) {
            if (line[0] == '[') {
                sectionName = line.substr(1, (line.size() - 2));
            } else {
                if (line[0] != ';' && line[0] != '#') {
                    if (sectionName != sectionName_old) {
                        if (sectionName_old != "") {
                            sections.push_back(sect);
                        }
                        sectionName_old = sectionName;
                        sect.name = sectionName;
                        sect.parameters.clear();
                    }
                    size_t delimPos = line.find('=');
                    std::string paramName = line.substr(0, delimPos);
                    std::string paramValue = line.substr(delimPos + 1, line.size());
                    std::pair<std::string, Parameter> param;
                    param.first = paramName;
                    param.second = paramValue;
                    sect.parameters.push_back(param);
                }
            }
        }
    }
    sections.push_back(sect);
}

IniFile::~IniFile()
{
}

void IniFile::print() {
    for (auto s: sections) {
        std::cout << "[" << s.name << "]" << std::endl;
        for (auto p: s.parameters) {
            std::cout << p.first << "=" << p.second.value << std::endl;
        }
    }
}

void IniFile::convertStrToFloat(std::string paramName, std::string paramValue, float *var) {
    try {
        (*var) = std::stof(paramValue);
    } catch (std::exception &e) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Configuraion file error in -> %s ... using default value!\n", paramName.c_str());
    }
}

void IniFile::convertStrToVec3(std::string paramName, std::string paramValue, glm::vec3 *vec) {
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

float IniFile::getScreenAspectRatio() {
    float w = getParameter("Window", "width").toFloat();
    float h = getParameter("Window", "height").toFloat();
    if (w != 0 && h != 0) {
        return w / h;
    }
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in IniFile::getScreenAspectRatio()\n");
    return 1;
}

IniFile::Parameter IniFile::getParameter(std::string sectionName, std::string parameter) {
    for (auto s: sections) {
        if (s.name == sectionName) {
            for (auto p: s.parameters) {
                if (p.first == parameter) {
                    return p.second;
                }
            }
        }
    }
    return Parameter("Error");
}
