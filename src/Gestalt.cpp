#include "Gestalt.h"

World* Gestalt::world = nullptr;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
	if (Gestalt::world->cursorCaptured) {
		Gestalt::world->camera.ProcessMouseMovement(xpos, ypos);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	Gestalt::world->ProcessMouseScroll(yoffset);
}

void Gestalt::Initialize() {
	// Callbacks
	window = world->GetRenderer().window;
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
}

void Gestalt::Run() {
	while (!glfwWindowShouldClose(window))
	{
		world->Update();
	}

	world->GetRenderer().Cleanup();
}