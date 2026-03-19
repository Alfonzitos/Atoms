#include "camera.h"

Camera::Camera(glm::vec3 initalEye, glm::vec3 initalCenter, glm::vec3 initialUp,
               float initalX, float initalY, float initalSensitivity)
{
    eye = initalEye;
    // center = initalCenter;
    viewDir = initalCenter;
    up = initialUp;
    lastX = initalX;
    lastY = initalY;
    sensitivity = initalSensitivity;

}

void Camera::mouseCallback(GLFWwindow *, double newXpos, double newYpos)
{
    if (!activeMouse)
        return;

    float xOffset = newXpos - lastX;
    float yOffset = lastY - newYpos; // reversed due to screen Y increasing downward

    lastX = newXpos;
    lastY = newYpos;

    yaw += static_cast<float>(xOffset * sensitivity);
    pitch += static_cast<float>(yOffset * sensitivity);

    // Clamp pitch and yaw
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    if (yaw > 360.f)
        yaw -= 360.0f;
    if (yaw < -360.0f)
        yaw += 360.f;

    viewDir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    viewDir.y = sin(glm::radians(pitch));
    viewDir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    viewDir = glm::normalize(viewDir);

    movementDir.x = cos(glm::radians(yaw));
    movementDir.y = 0.0f;
    movementDir.z = sin(glm::radians(yaw));

    movementDir = glm::normalize(movementDir);
}

void Camera::processKeyboard(GLFWwindow *window, float deltaTime)
{
    
 // Keep movementDir in sync with yaw even before any mouse input
    movementDir = glm::normalize(glm::vec3(
        cos(glm::radians(yaw)), 0.0f, sin(glm::radians(yaw))
    ));

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    bool mKeyCurrentlyPressed = glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS;
    if (!mKeyPreviouslyPressed && mKeyCurrentlyPressed)
    {
        activeMouse = !activeMouse;
        if (activeMouse){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            // // Reset lastX/lastY to current cursor to avoid jump
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            lastX = static_cast<float>(xpos);
            lastY = static_cast<float>(ypos);
        }
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    mKeyPreviouslyPressed = mKeyCurrentlyPressed;

    const float cameraSpeed = 70.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        eye += cameraSpeed * movementDir;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        eye -= cameraSpeed * movementDir;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        eye -= glm::normalize(glm::cross(movementDir, up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        eye += glm::normalize(glm::cross(movementDir, up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE))
        eye += cameraSpeed * up;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
        eye -= cameraSpeed * up;
}

glm::mat4 Camera::getView(void)
{
    view = glm::lookAt(eye, eye + viewDir, up);
    return view;
}

void Camera::updateMouseCaptured(bool isCaptured)
{
    activeMouse = isCaptured;
    return;
}