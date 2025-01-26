#pragma once
#include <iostream>
#include <vector>
#include <string>

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

#include "SettingsBus.h"

class Renderer {

public:
    unsigned int SCR_WIDTH = 1000;
    unsigned int SCR_HEIGHT = 750;

    glm::vec3 backgroundColor = glm::vec3(43, 43, 68);
    glm::vec3 gridColor = glm::vec3(78, 133, 120);
    glm::vec3 xColor = glm::vec3(255, 99, 71);
    glm::vec3 yColor = glm::vec3(50, 205, 50);
    glm::vec3 zColor = glm::vec3(30, 144, 255);

    GLFWwindow* window;
    Shader shader;
    Shader gridShader;
    Shader crosshairShader;
    unsigned int VBO, VAO;
    unsigned int crosshairVAO, crosshairVBO;
    unsigned int gridVAO, gridVBO;

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
    unsigned int gridTexture;

    const float fixedTimeStep = 0.02f;
    float timeAccumulator = 0.0f;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    std::vector<Module*> modules; // effectively GUI windows

    float crosshairVertices[16] = {
        // Horizontal line
        -0.02f,  0.0f,
         0.02f,  0.0f,

         // Vertical line
          0.0f,  0.02f,
          0.0f, -0.02f,
    };

    int GRID_SIZE = 40;
    float GRID_SPACING = 2;

	Renderer();

    void CreateDefaultModules();

    static void GenerateTexture(std::string path, unsigned int& texture, bool includeAlpha);

    void RenderObjectTable(std::vector<PhysicsObject*> RenderObjects);
    void RenderGrid(int gridSize, float gridSpacing, const glm::mat4& view, const glm::mat4& projection);
    void RenderLoop(Camera* camera, std::vector<PhysicsObject*> RenderObjectsP, std::vector<TriggerObject*> RenderObjectsT, SettingsBus settingsBus);
    void InitImGUI(GLFWwindow *window);
    void InitTextures();
    void InitCrosshair();
    void InitGrid();
    void Initialize();
    void Cleanup();
 
    GLFWwindow* GetWindow() { return window; }
    float GetDeltaTime() { return deltaTime; }

};
