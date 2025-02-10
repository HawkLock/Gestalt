#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "World.h"
#include "PhysicsUtility.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

World world;

int main()
{
    // Callbacks
    GLFWwindow* window = world.GetRenderer().window;
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    while (!glfwWindowShouldClose(window))
    {
        world.Update();
    }

    world.GetRenderer().Cleanup();

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
	if (world.cursorCaptured) {
		world.camera.ProcessMouseMovement(xpos, ypos);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    world.ProcessMouseScroll(yoffset);
}