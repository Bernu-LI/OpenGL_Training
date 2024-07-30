#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"

#include <sstream>
#include <fstream>
#include <iostream>

/* Find the path to the resource files directory */
ResourceManager::ResourceManager(const std::string& executablePath) {
    size_t foundLastSlash = executablePath.find_last_of("/\\");
    m_path = executablePath.substr(0, foundLastSlash);
}

/* Get a string from the file */
std::string ResourceManager::getFileString(const std::string relativeFilePath) const {
    std::ifstream f;    // Create input file stream object
    f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in);  // Open file for reading

    /* Check file opening for success */
    if (!f.is_open()) {     
        std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
        return std::string{};
    }

    std::stringstream buffer;   // Create an object to read and write strings
    buffer << f.rdbuf();    // Copy file content to std::stringstream buffer
    return buffer.str();    // Get buffer content as std::string
}

/* Load shaders source code and create a shader program */
std::shared_ptr <Renderer::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderProgramName, const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    // Get vertex shader source code from the file
    std::string vertexShaderSource = getFileString(vertexShaderPath);
    /* Check getting the vertex shader source code for success */
    if (vertexShaderSource.empty()) {
        std::cerr << "No vertex shader" << std::endl;
        return nullptr;
    }

    // Get fragment shader source code from the file
    std::string fragmentShaderSource = getFileString(fragmentShaderPath);
    /* Check getting the fragment shader source code for success */
    if (fragmentShaderSource.empty()) {
        std::cerr << "No fragment shader" << std::endl;
        return nullptr;
    }
    
    /* Create a shader program and emplace it in the map */
    std::shared_ptr <Renderer::ShaderProgram>& newShaderProgram = m_shaderPrograms.emplace(shaderProgramName, std::make_shared<Renderer::ShaderProgram>(vertexShaderSource.c_str(), fragmentShaderSource.c_str())).first->second;
    /* Check shader program compilation for success */
    if (!newShaderProgram->isCompiled()) {
        std::cerr << "Can not load Shader Program:\n"
            << "Vertex: " << vertexShaderPath << "\n"
            << "Fragment: " << fragmentShaderPath << std::endl;
            return nullptr;
    }
    return newShaderProgram;
}

/* Get shader program by its name */
std::shared_ptr <Renderer::ShaderProgram> ResourceManager::getShaderProgram(const std::string shaderProgramName) const {
    ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderProgramName);
    /* Check the existence of the shader program */
    if (it == m_shaderPrograms.end()) {
        std::cerr << "Can not find the Shader Prpgram: " << shaderProgramName << std::endl;
        return nullptr;
    }
    return it->second;
}