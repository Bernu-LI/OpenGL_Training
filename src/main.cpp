#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"

/* Array of vertex coordinates (already normalized) */
GLfloat vertices[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f, 
   -0.5f, -0.5f, 0.0f
};

/* Array of texture-to-vertex coordinate mapping */
GLfloat texCoordinates[] = {
    0.5f, 1.0f,
    1.0f, 0.0f, 
    0.0f, 0.0f  
}; 

/* Global variables for window size */
int gWindowSizeX = 640;
int gWindowSizeY = 480;

/* Callback function for resize window */
void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    gWindowSizeX = width;
    gWindowSizeY = height;
    /* Set the rendering area inside the window */
    glViewport(0, 0, gWindowSizeX, gWindowSizeY);
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
    GLFWwindow* pWindow = glfwCreateWindow(gWindowSizeX, gWindowSizeY, "OpenGL_Training ", nullptr, nullptr);
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
            std::cerr << "Can not create Shader Program: " << "DefaultShaderProgram" << std::endl;
            return -1;
        }

        /* Load a texture */
        auto pDefaultTexture = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");

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

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            /* Render an object */
            pDefaultShaderProgram->use();   // Activate shader program (make it current)
            glBindVertexArray(vao);     // Make the vertex array current
            pDefaultTexture->bind();    // Make the texture bound to the texture unit current
            glDrawArrays(GL_TRIANGLES, 0, 3);   // Render an object

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    
    glfwTerminate();
    return 0;
}