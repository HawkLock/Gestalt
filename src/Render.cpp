#include "Render.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);


Renderer::Renderer()
{
    modules = std::vector<Module*>();
    Initialize();
    shader = Shader("../Shaders/VertexShader.vert", "../Shaders/FragmentShader.frag");
    arrowShader = Shader("../Shaders/VertexShader.vert", "../Shaders/ArrowFragmentShader.frag");
    crosshairShader = Shader("../Shaders/CrosshairVertexShader.vert", "../Shaders/CrosshairFragmentShader.frag");
    gridShader = Shader("../Shaders/GridVertexShader.vert", "../Shaders/GridFragmentShader.frag");
}

void Renderer::CreateDefaultModules() {
    ObjectModule* objModule = new ObjectModule();
    modules.push_back(objModule);

    SettingsModule* settModule = new SettingsModule();
    modules.push_back(settModule);

    FocusModule* focusModule = new FocusModule();
    modules.push_back(focusModule);

    ScenarioModule* sceneModule = new ScenarioModule();
    modules.push_back(sceneModule);

}

void Renderer::GenerateTexture(std::string path, unsigned int& texture, bool includeAlpha) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        if (includeAlpha) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void Renderer::InitImGUI(GLFWwindow *window) {
    // Set-up ImGUI
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io; // Getting IO object
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // Setup ImGui style
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.FrameRounding = 4.0f;
    style.FrameBorderSize = 1.0f;
    style.ItemSpacing = ImVec2(8, 6);

    ImFontConfig fontConfig;
    fontConfig.OversampleH = 3; // Horizontal oversampling
    fontConfig.OversampleV = 3; // Vertical oversampling
    fontConfig.PixelSnapH = false; // Prevent snapping for better smoothing
    io.Fonts->AddFontFromFileTTF("../Fonts/JetBrains-Mono.ttf", 16.0f);
    io.FontGlobalScale = 1.0f; // Adjust global scaling if needed

    // Setup platform/renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void Renderer::InitTextures() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    GenerateTexture("../Textures/CrateTexture.jpg", texture1, false);
    GenerateTexture("../Textures/sus.png", texture2, true);

    GenerateTexture("../Textures/red.png", redTexture, true);
    GenerateTexture("../Textures/blue.png", blueTexture, true);

    GenerateTexture("../Textures/blue.png", gridTexture, true);
}

void Renderer::InitCrosshair() {
    glGenVertexArrays(1, &crosshairVAO);
    glGenBuffers(1, &crosshairVBO);
    glBindVertexArray(crosshairVAO);

    glBindBuffer(GL_ARRAY_BUFFER, crosshairVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::InitGrid() {
    float gridVertices[] = {
        0.0f, -100.0f, 0.0f,  // Line starts
        0.0f,  100.0f, 0.0f   // Line ends
    };

    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);

    glBindVertexArray(gridVAO);

    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gridVertices), gridVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::Initialize()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Gestalt", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glEnable(GL_DEPTH_TEST);

    InitImGUI(window);
    InitTextures();
    InitCrosshair();
    InitGrid();

    shader.use();

    // Windows initialize
    CreateDefaultModules();


}

void Renderer::Cleanup()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

// For Debugging Energy Currently
void Renderer::RenderObjectTable(std::vector<PhysicsObject*> objects) {
    if (ImGui::BeginTable("Objects", 4)) { // 2 columns
        ImGui::TableSetupColumn("Object Index");
        ImGui::TableSetupColumn("Energy");
        ImGui::TableSetupColumn("Lin");
        ImGui::TableSetupColumn("Rot");
        ImGui::TableHeadersRow();

        float totalEnergy = 0.f;
        for (size_t i = 0; i < objects.size(); ++i) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%zu", i);
            ImGui::TableNextColumn();
            float energy = objects[i]->CalculateTotalEnergy();
            totalEnergy += energy;
            ImGui::Text("%.2f J", energy);
            ImGui::TableNextColumn();
            ImGui::Text("%.2f J", objects[i]->CalculateTranslationalEnergy());
            ImGui::TableNextColumn();
            ImGui::Text("%.2f J", objects[i]->CalculateRotationalEnergy());
        }

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Total");
        ImGui::TableNextColumn();
        ImGui::Text("%.2f J", totalEnergy);

        ImGui::EndTable();
    }
}

glm::vec2 Renderer::WorldToScreen(glm::vec3 worldPos, const glm::mat4& view, const glm::mat4& projection, int screenWidth, int screenHeight) {
    glm::vec4 clipSpacePos = projection * view * glm::vec4(worldPos, 1.0f);

    // Perspective divide to get normalized device coordinates
    glm::vec3 ndc = glm::vec3(clipSpacePos) / clipSpacePos.w;

    // Convert to screen coordinates
    glm::vec2 screenPos;
    screenPos.x = (ndc.x * 0.5f + 0.5f) * screenWidth;
    screenPos.y = (1.0f - (ndc.y * 0.5f + 0.5f)) * screenHeight; // Invert Y for ImGui

    return screenPos;
}

void Renderer::RenderArrowLabels(PhysicsObject* obj, const glm::mat4& view, const glm::mat4& projection, int screenWidth, int screenHeight) {
    glm::vec3 velocityTip = obj->pos + obj->velocity;
    glm::vec3 accelerationTip = obj->pos + obj->acceleration;

    glm::vec2 velocityScrPos = WorldToScreen(velocityTip, view, projection, screenWidth, screenHeight);
    glm::vec2 accelerationScrPos = WorldToScreen(accelerationTip, view, projection, screenWidth, screenHeight);

    ImGui::SetNextWindowPos(ImVec2(velocityScrPos.x, velocityScrPos.y), ImGuiCond_Always);
    std::string velLabelTextHead = "VelocityLabel";
    std::string labelID = std::to_string(reinterpret_cast<uintptr_t>(& obj));
    std::string velLabel = velLabelTextHead + labelID;
    ImGui::Begin(velLabel.c_str(), nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    std::string velTextHead = "Vel: ";
    std::string velLength = std::to_string(glm::length(obj->velocity));
    std::string velText = velTextHead + velLength;
    ImGui::Text(velText.c_str());
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(accelerationScrPos.x, accelerationScrPos.y), ImGuiCond_Always);
    std::string acelLabelTextHead = "VelocityLabel";
    std::string acelLabel = acelLabelTextHead + labelID;
    ImGui::Begin(acelLabel.c_str(), nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    std::string acelTextHead = "Acel: ";
    std::string acelLength = std::to_string(glm::length(obj->acceleration));
    std::string acelText = acelTextHead + acelLength;
    ImGui::Text(acelText.c_str());
    ImGui::End();

}

void Renderer::RenderGrid(int gridSize, float gridSpacing, const glm::mat4& view, const glm::mat4& projection, Camera* camera) {
    // Grid lines
    std::vector<float> gridVertices;
    std::vector<float> xAxisVertices;
    std::vector<float> zAxisVertices;

    int xOffset = round(camera->cameraPos.x / gridSpacing) * gridSpacing;
    int zOffset = round(camera->cameraPos.z / gridSpacing) * gridSpacing;

    xAxisVertices.push_back(-gridSize * gridSpacing + xOffset);
    xAxisVertices.push_back(0.0f);
    xAxisVertices.push_back(0.0f);

    xAxisVertices.push_back(gridSize * gridSpacing + xOffset);
    xAxisVertices.push_back(0.0f);
    xAxisVertices.push_back(0.0f);

    // Vertical (z-axis) line
    zAxisVertices.push_back(0.0f);
    zAxisVertices.push_back(0.0f);
    zAxisVertices.push_back(-gridSize * gridSpacing + zOffset);

    zAxisVertices.push_back(0.0f);
    zAxisVertices.push_back(0.0f);
    zAxisVertices.push_back(gridSize * gridSpacing + zOffset);

    for (int i = -gridSize; i <= gridSize; i++) {

            // Horizontal lines
            gridVertices.push_back(-gridSize * gridSpacing + xOffset);
            gridVertices.push_back(0.0f);
            gridVertices.push_back(i * gridSpacing + zOffset);

            gridVertices.push_back(gridSize * gridSpacing + xOffset);
            gridVertices.push_back(0.0f);
            gridVertices.push_back(i * gridSpacing + zOffset);

            // Vertical lines
            gridVertices.push_back(i * gridSpacing + xOffset);
            gridVertices.push_back(0.0f);
            gridVertices.push_back(-gridSize * gridSpacing + zOffset);

            gridVertices.push_back(i * gridSpacing + xOffset);
            gridVertices.push_back(0.0f);
            gridVertices.push_back(gridSize * gridSpacing + zOffset);
    }

    // Generate and bind VAOs/VBOs for the grid, x-axis, and z-axis
    unsigned int gridVAO, gridVBO, xAxisVAO, xAxisVBO, zAxisVAO, zAxisVBO;

    // Grid lines
    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);
    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // X-axis lines
    glGenVertexArrays(1, &xAxisVAO);
    glGenBuffers(1, &xAxisVBO);
    glBindVertexArray(xAxisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, xAxisVBO);
    glBufferData(GL_ARRAY_BUFFER, xAxisVertices.size() * sizeof(float), xAxisVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Z-axis lines
    glGenVertexArrays(1, &zAxisVAO);
    glGenBuffers(1, &zAxisVBO);
    glBindVertexArray(zAxisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, zAxisVBO);
    glBufferData(GL_ARRAY_BUFFER, zAxisVertices.size() * sizeof(float), zAxisVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Use shader
    gridShader.use();
    gridShader.setMat4("view", view);
    gridShader.setMat4("projection", projection);

    // Render grid lines
    gridShader.setVec4("lineColor", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)); // Gray for regular grid
    glBindVertexArray(gridVAO);
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(gridVertices.size() / 3));
    glBindVertexArray(0);

    glDisable(GL_DEPTH_TEST);


    // Render x-axis lines
    glm::vec3 xCol = TextureUtils::getRGB(xColor);
    gridShader.setVec4("lineColor", glm::vec4(xCol.x, xCol.y, xCol.z, 1.0f)); // Red for x-axis
    glBindVertexArray(xAxisVAO);
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(xAxisVertices.size() / 3));
    glBindVertexArray(0);

    // Render z-axis lines
    glm::vec3 zCol = TextureUtils::getRGB(zColor);
    gridShader.setVec4("lineColor", glm::vec4(zCol.x, zCol.y, zCol.z, 1.0f)); // Blue for z-axis
    glBindVertexArray(zAxisVAO);
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(zAxisVertices.size() / 3));
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);


    // Cleanup
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);
    glDeleteVertexArrays(1, &xAxisVAO);
    glDeleteBuffers(1, &xAxisVBO);
    glDeleteVertexArrays(1, &zAxisVAO);
    glDeleteBuffers(1, &zAxisVBO);
}

void Renderer::RenderLoop(Camera* camera, std::vector<PhysicsObject*> RenderObjectsP, std::vector<TriggerObject*> RenderObjectsT, SettingsBus settingsBus)
{
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    timeAccumulator += deltaTime;    


    // Start the ImGui frame
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 clearColor = TextureUtils::getRGB(backgroundColor);
    glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    shader.use();
    shader.setBool("isPaused", GlobalData::paused);

    glm::mat4 view = camera->GetCameraView();
    glm::mat4 projection = glm::perspective(glm::radians(camera->fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    shader.setMat4("projection", projection); 
    shader.setMat4("view", view);

    // Render the grid
    RenderGrid(GRID_SIZE, GRID_SPACING, view, projection, camera);

    for (auto& object : RenderObjectsT)
    {
        object->RenderMesh(shader, texture2);
    }

    for (auto& object : RenderObjectsP)
    {
        object->RenderMesh(shader);
    }

    arrowShader.use();
    arrowShader.setMat4("projection", projection);
    arrowShader.setMat4("view", view);
    if (*settingsBus.renderArrows == true) {
        for (auto& object : RenderObjectsP)
        {
                if (*settingsBus.renderArrowsOnTop) {
                    glDisable(GL_DEPTH_TEST);
                    object->RenderArrows(arrowShader, redTexture, blueTexture, *settingsBus.decomposeArrows);
                    glEnable(GL_DEPTH_TEST);
                }
                else {
                    object->RenderArrows(arrowShader, redTexture, blueTexture, *settingsBus.decomposeArrows);
                }
                if (*settingsBus.renderArrowLabels) {
                    RenderArrowLabels(object, view, projection, SCR_WIDTH, SCR_HEIGHT);
                }
        }
    }

    // Render the Crosshair
    crosshairShader.use();
    glBindVertexArray(crosshairVAO);  // Set the VAO for the crosshair
    glDrawArrays(GL_LINES, 0, 4);  // Draw the two lines that make the crosshair


    PhysicsObject* obj = RenderObjectsP[0];

    // ImGUI
    ImGui::Begin("Real-Time Variables");
    ImGui::Text("Move");
    //ImGui::SetWindowSize(ImVec2(200, 400));
    //ImGui::SetWindowPos(ImVec2(0, 0));
    ImGui::SliderFloat("Obj X", &obj->velocity.x, 0.0f, 1.0f);
    ImGui::SliderFloat("Obj Y", &obj->velocity.y, 0.0f, 1.0f);
    ImGui::SliderFloat("Obj Z", &obj->velocity.z, 0.0f, 1.0f);
    RenderObjectTable(RenderObjectsP);
    ImGui::End();


    // Objects Module
    modules[0]->UpdateData(RenderObjectsP);
    modules[0]->RenderWindow();

    // Settings Module (requires more nuanced control because I implemented it with templates for more flexibility)
    std::pair<std::string, bool*> pause("Paused", &GlobalData::paused);
    std::pair<std::string, bool*> arrows("Render Arrows", settingsBus.renderArrows);
    std::pair<std::string, bool*> decompose("Decompose Velocity/Acceleration", settingsBus.decomposeArrows);
    std::pair<std::string, bool*> onTop("Arrows On Top", settingsBus.renderArrowsOnTop);
    std::pair<std::string, bool*> labels("Arrows Labels", settingsBus.renderArrowLabels);
    ImGui::Begin("Settings");
    modules[1]->UpdateData(pause);
    modules[1]->UpdateData(arrows);
    modules[1]->UpdateData(decompose);
    modules[1]->UpdateData(onTop);
    modules[1]->UpdateData(labels);
    ImGui::End();

    // Focus Module
    if (settingsBus.focusObject != nullptr) {
        std::pair<std::string, bool*> follow("Follow With Camera", settingsBus.followFocusedObject);
        ImGui::Begin("Focus Object");
        modules[2]->UpdateData(follow);
        modules[2]->UpdateData(settingsBus.focusObject);
        modules[2]->RenderWindowBody();
        ImGui::End();
    }

    // Scenario Module
    modules[3]->RenderWindow();

    //ImGui::End();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


    glfwSwapBuffers(window);
    glfwPollEvents();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
