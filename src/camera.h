#include <glad/glad.h>
#include <glfw3.h>
#include <glfw3native.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:

    glm::mat4 view;
    Camera(glm::vec3 initalEye, glm::vec3 initalCenter, glm::vec3 initialUp,
           float initalX, float initalY, float initalSensitivity);
    void mouseCallback(GLFWwindow *, double newXpos, double newYpos);
    void processKeyboard(GLFWwindow * window, float deltaTime);

    void updateMouseCaptured(bool isCaptured);
    glm::mat4 getView(void);

private:
    bool activeMouse = false;
    bool mKeyPreviouslyPressed = false;
    float sensitivity;
    
    glm::vec3 eye;
    glm::vec3 center;
    glm::vec3 up;

    float lastX;
    float lastY;

    glm::vec3 viewDir;
    glm::vec3 movementDir;

    float yaw = -90.f;
    float pitch = 0.0f;
};