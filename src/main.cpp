#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "World.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

World world;

int main()
{
    // Callbacks
    GLFWwindow* window = world.GetRenderer().window;
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

	glm::vec3 gravity = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	std::vector<glm::vec3> initialActingForces = std::vector<glm::vec3>();
	glm::vec3 rotationAxis = glm::vec3(0.0f, 0.0f, 0.0f);//glm::vec3(0.5f, 1.0f, 0.7f);
	float angle = 50.0f;
	float mass = 50.f;
	float faceSize = 1.f;
	PhysicsObject tempObj = PhysicsObject(position, velocity, initialActingForces, rotationAxis, angle, mass, gravity, false, faceSize);
	world.AddObject(tempObj);

	//initialActingForces.push_back(glm::vec3(0.f, -1.f, 0.f));
	position = glm::vec3(2.5f, 5.5f, -4.5f);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	rotationAxis = glm::vec3(0.0f, 0.0f, 0.0f);
	angle = 30.0f;
	mass = 100.0f;
	tempObj = PhysicsObject(position, velocity, initialActingForces, rotationAxis, angle, mass, gravity, false, faceSize);
	tempObj.AddInstantaneousForce(glm::vec3(-70.f, -140.f, 140.f));

	world.AddObject(tempObj);

    while (!glfwWindowShouldClose(window))
    {
        world.Update();
    }

    world.GetRenderer().Cleanup();

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    world.camera.ProcessMouseMovement(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    world.ProcessMouseScroll(yoffset);
}