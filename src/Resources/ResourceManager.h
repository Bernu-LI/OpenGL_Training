#pragma once

#include <string>
#include <memory>
#include <map>

namespace Renderer {
    class ShaderProgram;
}

class ResourceManager {
public:
    /* Find the path to the resource files directory */
    ResourceManager(const std::string& executablePath);

    ~ResourceManager() = default;

    /* Prohibit any copying or moving of resource manager objects */
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(const ResourceManager&&) = delete;
    ResourceManager& operator = (const ResourceManager&) = delete;
    ResourceManager& operator = (const ResourceManager&&) = delete;
    
    /* Load shaders source code and create a shader program */
    std::shared_ptr <Renderer::ShaderProgram> loadShaders(const std::string& shaderProgramName, const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    /* Get shader program by its name */
    std::shared_ptr <Renderer::ShaderProgram> getShaderProgram(const std::string shaderProgramName) const;

private:
    /* Get a string from the file */
    std::string getFileString(const std::string relativeFilePath) const; 

    typedef std::map <const std::string, std::shared_ptr <Renderer::ShaderProgram>> ShaderProgramsMap;
    ShaderProgramsMap m_shaderPrograms;

    std::string m_path;
};  