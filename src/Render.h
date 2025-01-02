#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Shader.h"

#include "Module.h"
#include "ObjectModule.h"
#include "SettingsModule.h"
#include "FocusModule.h"

#include "Camera.h"
#include "PhysicsObject.h"
#include "TriggerObject.h"

#include <iostream>
#include <vector>
#include <string>

class Renderer {

public:
    unsigned int SCR_WIDTH = 1000;
    unsigned int SCR_HEIGHT = 750;

    GLFWwindow* window;
    Shader shader;
    Shader crosshairShader;
    unsigned int VBO, VAO;
    unsigned int crosshairVAO, crosshairVBO;

    float vertices[180] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    unsigned int texture1, texture2, redTexture, blueTexture;

    const float fixedTimeStep = 0.02f;
    float timeAccumulator = 0.0f;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    std::vector<Module*> modules; // effectively GUI windows
    bool renderArrows = true;

    float crosshairVertices[16] = {
        // Horizontal line
        -0.02f,  0.0f,
         0.02f,  0.0f,

         // Vertical line
          0.0f,  0.02f,
          0.0f, -0.02f,
    };


	Renderer();

    void CreateDefaultModules();

    void GenerateTexture(std::string path, unsigned int& texture, bool includeAlpha);

    void RenderObjectTable(std::vector<PhysicsObject*> RenderObjects);
    void RenderLoop(Camera* camera, std::vector<PhysicsObject*> RenderObjectsP, std::vector<TriggerObject*> RenderObjectsT, PhysicsObject* focusObject);
    void InitImGUI(GLFWwindow *window);
    void InitTextures();
    void InitCrosshair();
    void Initialize();
    void Cleanup();
 
    GLFWwindow* GetWindow() { return window; }
    float GetDeltaTime() { return deltaTime; }

};
