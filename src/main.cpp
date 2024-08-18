#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite.h"

/* Array of vertex coordinates in local space */
GLfloat vertices[] = {
    0.0f, 50.f, 0.0f,
    50.f, -50.f, 0.0f, 
   -50.f, -50.f, 0.0f
};

/* Array of texture-to-vertex coordinate mapping */
GLfloat texCoordinates[] = {
    0.5f, 1.0f,
    1.0f, 0.0f, 
    0.0f, 0.0f  
}; 

/* Global variable for window size */
glm::ivec2 gWindowSize(640, 480);

/* Callback function for resize window */
void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    gWindowSize.x = width;
    gWindowSize.y = height;
    /* Set the rendering area inside the window */
    glViewport(0, 0, width, height);
}

/* Callback function for handling keyboard events */
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE and action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(int argc, char** argv)
{   
    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "glfwInit failed" << std::endl;
        return -1;
    }

    /* Set the minimum required version of OpenGL */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(gWindowSize.x, gWindowSize.y, "OpenGL_Training ", nullptr, nullptr);
    if (!pWindow)
    {
        std::cout << "glfwCreateWindow failed" << std::endl; 
        glfwTerminate();
        return -1;
    }

    /* Set a function that is called when the window is resized */
    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    /* Set a function that is called on every keyboard event  */
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    /* Check GLAD initialization for success */
    if (!gladLoadGL()) {
        std::cout << "Can not load GLAD" << std::endl;
        return -1;
    }
    
    /* Output renderer and OpenGL version   */
    std::cout << "Renderer: " <<  glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(0, 1, 0, 1);

    /* 
    Resource Manager contains OpenGL context that should be deleted before finishing work with OpenGL (glfwTerminate() command).
    Therefore, all work with Resource Manager is allocated to a separate scope
     */
    {
        /* Create a resource manager object */
        ResourceManager resourceManager(argv[0]);

        /* Load shaders source code and create a shader program */
        auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShaderProgram", "res/shaders/vertex_shader.txt", "res/shaders/fragment_shader.txt");
        /* Check creation of the shader program for success */
        if (!pDefaultShaderProgram) {
            std::cerr << "Can not create shader program: " << "DefaultShaderProgram" << std::endl;
            return -1;
        }

        /* Load sprite shaders source code and create a shader program for sprites */
        auto pSpriteShaderProgram = resourceManager.loadShaders("SpriteShaderProgram", "res/shaders/vSprite_shader.txt", "res/shaders/fSprite_shader.txt");
        if (!pDefaultShaderProgram) {
            std::cerr << "Can not create shader program: " << "SpriteShaderProgram" << std::endl;
            return -1;
        }

        /* Load a texture */
        auto pDefaultTexture = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");

        /* Load a texture atlas */
        std::vector <std::string> subTextureNames = { "brick", "topBrick", "bottomBrick", "leftBrick", "rightBrick", "topLeftBrick", "topRightBrick", "bottomLeftBrick", "bottomRightBrick", "concrete" };
        auto pTextureAtlas = resourceManager.loadTextureAtlas("DefaultTextureAtlas", "res/textures/map_16x16.png", std::move(subTextureNames), 16, 16);

        /* Load a sprite */
        auto pSprite = resourceManager.loadSprite("Sprite", "DefaultTextureAtlas", "SpriteShaderProgram", 100, 100, "brick");

        /* Set sprite position */
        pSprite->setPosition(glm::vec2(300, 100));

        /* Create a Vertex Buffer Object with vertex coordinate data in video card memory */
        GLuint vertices_vbo = 0;    
        glGenBuffers(1, &vertices_vbo);     // Generate and return one unique identifier for a buffer
        glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo); // Create a buffer of GL_ARRAY_BUFFER type, bind it to the ID & make current
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // Fill the buffer with data

        /* Create a Vertex Buffer Object with texture-to-vertex coordinate mapping data in video card memory */
        GLuint texCoordinates_vbo = 0;
        glGenBuffers(1, &texCoordinates_vbo);   // Generate and return one unique identifier for a buffer
        glBindBuffer(GL_ARRAY_BUFFER, texCoordinates_vbo);  // Create a buffer of GL_ARRAY_BUFFER type, bind it to the ID & make current
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoordinates), texCoordinates, GL_STATIC_DRAW);  // Fill the buffer with data

        /* Create a Vertex Array Object for vertex attribute state */
        GLuint vao = 0;
        glGenVertexArrays(1, &vao);     // Generate and return one unique identifier for a vertex array
        glBindVertexArray(vao);     // Create a vertex array object, bind it to the ID & make current

        /* Configure the vertex shader to work with coordinates */
        glEnableVertexAttribArray(0);   // Enable use of vertex attribute with index 0 in the vertex array
        glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo); // Make the buffer current
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr); // Configure how the vertex shader interprets data

        /* Configure the vertex shader to work with texture */
        glEnableVertexAttribArray(1);   // Enable use of vertex attribute with index 2 in the vertex array
        glBindBuffer(GL_ARRAY_BUFFER, texCoordinates_vbo);  // Make the buffer current
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr); // Configure how the vertex shader interprets data

        /* Link a texture to the shader program*/
        pDefaultShaderProgram->use();   // Activate shader program (make it current)
        pDefaultShaderProgram->setTexture("tex", 0);    // Link the texture stored in texture unit 0 to the shader program

        /* Link a texture to the sprite shader program */
        pSpriteShaderProgram->use();    // Activate shader program (make it current)
        pSpriteShaderProgram->setTexture("tex", 0);     // Link the texture stored in texture unit 0 to the shader program 

        /*  
        Create model matrices for transformation coordinates from local space to world space.
        Model matrix determines where the shape is located in OpenGL window
        */
        glm::mat4x4 modelMatrix_1(1.f);     // Create a 4x4 identity matrix
        modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 50.f, 0.f)); // Apply translation to the model matrix

        glm::mat4x4 modelMatrix_2(1.f);     // Create a 4x4 identity matrix
        modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(540.f, 50.f, 0.f)); // Apply translation to the model matrix

        /*  
        Create a projection matrix for transormation coordinates from world space to clip space.
        Projection matrix defines visible area(frustrum) in the space
        */
        glm::mat4x4 projectionMatrix = glm::ortho(0.f, static_cast<float>(gWindowSize.x), 0.f, static_cast<float>(gWindowSize.y), -100.f, 100.f);
        
        /* Link the projection matrix to the shader program */
        pDefaultShaderProgram->use();
        pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        /* Link the projection matrix to the sprite shader program */
        pSpriteShaderProgram->use();
        pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            /* Render an object */
            pDefaultShaderProgram->use();   // Activate shader program (make it current)
            glBindVertexArray(vao);     // Make the vertex array current
            pDefaultTexture->bind();    // Make the texture bound to the texture unit current

            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_1);   // Link the model matrix to the shader program
            glDrawArrays(GL_TRIANGLES, 0, 3);   // Render an object

            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_2);   // Link the model matrix to the shader program
            glDrawArrays(GL_TRIANGLES, 0, 3);   // Rener an object

            /* Render a sprite */
            pSprite->render();

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    
    glfwTerminate();
    return 0;
}