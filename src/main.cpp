#include <glad/glad.h>
#include <glfw3.h>
#include <glfw3native.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "probability_math.h"
#include "hydrogen.h"
#include "camera.h"

#include <iostream>


GLFWwindow *programInit();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void handleOrbitalSelection(GLFWwindow *window);
void changeOrbital(GLFWwindow* window,  int key);

Orbital currentOrbital = {0, 0, 0};
int currentIndex = 0;
bool orbitalNeedsUpdate = false;
bool keyPreviouslyPressed[10] = {false};

Camera camera(
    glm::vec3(0.0f, 0.0f, 3.0f),  // camera position
    glm::vec3(0.0f, 0.0f, -1.0f), // initial center/front (not really used)
    glm::vec3(0.0f, 1.0f, 0.0f),  // up vector
    800.0f / 2.0f,                // initial mouse X
    600.0f / 2.0f,                // initial mouse Y
    0.1f                          // sensitivity
);

Camera *cameraPtr = &camera;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float axis[] =
    {
        // positions         // colors
        -500.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // start  x
        500.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // end x

        0.0f, -500.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // start y
        0.0f, 500.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // end y

        0.0f, 0.0f, -500.0f, 0.0f, 0.0f, 1.0f, 1.0f, // start  z
        0.0f, 0.0f, 500.0f, 0.0f, 0.0f, 1.0f, 1.0f   // end z
};

int main()
{

    // Init
    GLFWwindow *window = programInit();
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Hydrogen hydrogen_atom = Hydrogen();
    std::vector<Vertex> verts;

    unsigned int VAO_electrons;
    glGenVertexArrays(1, &VAO_electrons);
    glBindVertexArray(VAO_electrons);

    unsigned int VBO_electrons;
    glGenBuffers(1, &VBO_electrons);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_electrons);

    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int VAO_axis;
    glGenVertexArrays(1, &VAO_axis);
    glBindVertexArray(VAO_axis);

    unsigned int VBO_axis;
    glGenBuffers(1, &VBO_axis);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_axis);

    glBufferData(GL_ARRAY_BUFFER, sizeof(axis), axis, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Binding
    // Can be done here or in event loop if different programs/VAO are used
    Shader pointShader("src/shaders/vertex.glsl", "src/shaders/fragmentPoint.glsl");
    Shader lineShader("src/shaders/vertex.glsl", "src/shaders/fragmentLine.glsl");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glLineWidth(5.0f);

    glfwSetCursorPosCallback(window, mouse_callback);

    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = static_cast<float>(glfwGetTime());

        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.processKeyboard(window, deltaTime);
        handleOrbitalSelection(window);

        if(orbitalNeedsUpdate)
        {

            verts = hydrogen_atom.compute_orbital_hydrogen(currentOrbital, 300'000, 500.0f, 10'000);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_electrons);
            glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_DYNAMIC_DRAW);
            orbitalNeedsUpdate = false;

        }

        glClearColor(0.2f, 0.2f, 0.2f, 0.2f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Perspective determines "how" the camera works, 
        // determening how the existing world is viewed, what is out of range
        // and what is too close, as well as field of view.
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            800.0f / 600.0f,
            0.1f,
            3000.0f);

        
        // View moves the world in relation to the cameras position
        // i.e where is the camera and what is it looking at.
        glm::mat4 view = camera.getView();


        // transformatinos are applied from right to left
        glm::mat4 mvp = projection * view;

        if(!verts.empty())
        {
            pointShader.use();
            pointShader.setUniMat4("view", view);
            pointShader.setUniMat4("uMVP", mvp);
            pointShader.setUniFloat("uPointSize", 10.0f);

            glBindVertexArray(VAO_electrons);
            glDrawArrays(GL_POINTS, 0, verts.size());
        }
       

        lineShader.use();
        lineShader.setUniMat4("view", view);
        lineShader.setUniMat4("uMVP", mvp);

        glBindVertexArray(VAO_axis);
        glDrawArrays(GL_LINES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

GLFWwindow *programInit()
{
    // GLFW & GLAD SETUP
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Press three numbers to enter n, l, m", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "GLFWWindow creation failed" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)))
    {
        std::cout << "GLAD initialization failed" << std::endl;
        exit(-1);
    }
    glViewport(0, 0, 800, 600);
    return window;
}

void framebuffer_size_callback(GLFWwindow * /*window*/, int width, int height)
{
    glViewport(0, 0, width, height);
}

void handleOrbitalSelection(GLFWwindow *window)
{
    for (int key = GLFW_KEY_0; key <= GLFW_KEY_9; ++key)
    {
        int index = key - GLFW_KEY_0;
        bool currentlyPressed = glfwGetKey(window, key) == GLFW_PRESS;

        if (!keyPreviouslyPressed[index] && currentlyPressed)
        {
            changeOrbital(window, key);
        }

        keyPreviouslyPressed[index] = currentlyPressed;
    }

}

void changeOrbital(GLFWwindow* window,  int key)
{
    int value = key - GLFW_KEY_0;
    switch (currentIndex)
    {
    case 0:
        if (value < 1) return;
        currentOrbital.n = value;
        break;
    case 1:
        if (value > currentOrbital.n - 1) return;
        currentOrbital.l = value;
        break;
    case 2:
        if(value > currentOrbital.l || value < -currentOrbital.l) return;
        currentOrbital.m = value;
        orbitalNeedsUpdate = true;
        break;
    }
    if (currentIndex < 2)
    {
        currentIndex += 1;
    }
    else
    {
        currentIndex = 0;
    }
    char title[100];
    sprintf(title, "n: %d  l: %d  m: %d", currentOrbital.n, currentOrbital.l, currentOrbital.m);
    glfwSetWindowTitle(window, title);
}

void mouse_callback(GLFWwindow *window, double newXpos, double newYpos)
{
    if (cameraPtr)
    {
        cameraPtr->mouseCallback(window, newXpos, newYpos);
    }
}
