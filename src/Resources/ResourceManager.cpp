#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"

#include <sstream>
#include <fstream>
#include <iostream>

#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/* Find the path to the resource files directory */
ResourceManager::ResourceManager(const std::string& executablePath) {
    size_t foundLastSlash = executablePath.find_last_of("/\\");
    m_path = executablePath.substr(0, foundLastSlash);
}

/* Get a string from the file */
std::string ResourceManager::getFileString(const std::string relativeFilePath) const {
    std::ifstream f;    // Create input file stream object
    f.open(m_path + "/" + relativeFilePath, std::ios::in);  // Open file for reading

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

/* Load a texture */
std::shared_ptr <Renderer::Texture2D> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath) {
    int width = 0;      // Image width
    int height = 0;     // Image height
    int channels = 0;   // Number of image channels (RGBA)
    /*
    stb_image library loads the image from the top left corner, however, OpenGL draws image from the bottom left corner.
    This command flips the image vertically when loading, so that the first pixel corresponds to the bottom left
    */
    stbi_set_flip_vertically_on_load(true);
    /* Load an image and return a pointer to an array of its pixels */
    unsigned char* pixels = stbi_load((m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0);

    /* Check image loading for success */
    if(!pixels) {
        std::cerr << "Can not load image: " << texturePath << std::endl;
        return nullptr;
    }
    
    /* Create a texture and emplace it in the map */
    std::shared_ptr <Renderer::Texture2D> newTexture = m_textures.emplace(textureName, std::make_shared<Renderer::Texture2D>(width, height, pixels, channels, GL_LINEAR, GL_CLAMP_TO_EDGE)).first->second;

    /* Free the memory allocated for storing the image */
    stbi_image_free(pixels);

    return newTexture;
}

/* Get texture by its name */
std::shared_ptr <Renderer::Texture2D> ResourceManager::getTexture(const std::string& textureName) const {
    TexturesMap::const_iterator it = m_textures.find(textureName);
    /* Check the existence of the texture */
    if (it == m_textures.end()) {
        std::cerr << "Can not find the texture: " << textureName << std::endl;
        return nullptr;
    }
    return it->second;
}