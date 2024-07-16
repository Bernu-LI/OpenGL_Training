#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

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

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}