#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer/ShaderProgram.h"

/* Array of vertex coordinates (already normalized) */
GLfloat vertices[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f, 
   -0.5f, -0.5f, 0.0f
};

/* Array of vertex colors */
GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

/* Code for the vertex shader */
const char* vertex_shader = 
"#version 330\n"    // GLSL version
"layout(location = 0) in vec3 vertex_position;"     // Declaration of input parameter
"layout(location = 1) in vec3 vertex_color;"        // Declaration of input parameter
"out vec3 color;"    // Declaration of output variable
"void main() {"     // Function with shader logic
"   color = vertex_color;"
"   gl_Position = vec4(vertex_position, 1.0f);" // Definition of vertex position
"}";

/* Code for the fragment shader */
const char* fragment_shader = 
"#version 330\n"    // GLSL version
"in vec3 color;"    // Take the value set in the vertex shader
"out vec4 fragment_color;"    //    Declaration of output variable (defines the fragment color)  
"void main() {"
"   fragment_color = vec4(color, 1.0f);" 
"}";

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

int main(void)
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

    /* Create a ready-to-use shader program object */
    Renderer::ShaderProgram shader_program(vertex_shader, fragment_shader);
    /* Check shader program compilation for success */
    if (!shader_program.isCompiled()) {
        std::cerr << "Can not create shader program" << std::endl;
    }

    /* Create a Vertex Buffer Object with vertex coordinate data in video card memory */
    GLuint vertices_vbo = 0;    
    glGenBuffers(1, &vertices_vbo);     // Create one buffer and return its unique identidier
    glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo); // Bind the buffer to GL_ARRAY_BUFFER type & make it current
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // Fill buffer with data

    /* Create a Vertex Buffer Object with color data in video card memory */
    GLuint colors_vbo = 0;
    glGenBuffers(1, &colors_vbo);     // Create one buffer and return its unique identidier
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);  // Bind the buffer to GL_ARRAY_BUFFER type & make it current
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);  // Fill buffer with data

    /* Create a Vertex Array Object for vertex attribute state */
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);     // Create one vertex array and return its unique identifier
    glBindVertexArray(vao);     // Make the vertex array current

    /* Configure the vertex shader to work with coordinates */
    glEnableVertexAttribArray(0);   // Enable use of vertex attribute with index 0 in the vertex shader
    glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo); // Bind the buffer to GL_ARRAY_BUFFER type & make it current
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr); // Configure how the driver interprets data

    /* Configure the vertex shader to work with colors */
    glEnableVertexAttribArray(1);   // Enable use of vertex attribute with index 1 in the vertex shader
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);  // Bind the buffer to GL_ARRAY_BUFFER type & make it current
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr); // Configure how the driver interprets data

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Render an object */
        shader_program.use();   // Activate shader program (make it current)
        glBindVertexArray(vao);     //  Make the vertex array current
        glDrawArrays(GL_TRIANGLES, 0, 3);   // Render an object

        /* Swap front and back buffers */
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}