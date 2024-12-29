#include "Render.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);


Renderer::Renderer()
{
    modules = std::vector<Module*>();
    Initialize();
    shader = Shader("../Shaders/VertexShader.vert", "../Shaders/FragmentShader.frag");
}

void Renderer::CreateDefaultWindows() {
    ObjectModule* objModule = new ObjectModule();
    modules.push_back(objModule);

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


void Renderer::Initialize()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // Set-up ImGUI
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io; // Getting IO object
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  


    // Setup ImGui style
    ImGui::StyleColorsDark();

    // Setup platform/renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");



    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glEnable(GL_DEPTH_TEST);

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

    // texture 1
    // ---------
    GenerateTexture("../Textures/CrateTexture.jpg", texture1, false);
    // "../Textures/CrateTexture.jpg"
    // texture 2
    // ---------
    GenerateTexture("../Textures/sus.png", texture2, true);
    //"../Textures/sus.png"

    GenerateTexture("../Textures/red.png", redTexture, true);
    GenerateTexture("../Textures/blue.png", blueTexture, true);

    shader.use();
    //shader.setInt("texture1", 0);
    //shader.setInt("texture2", 1);


    // Windows initialize
    CreateDefaultWindows();
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

void Renderer::RenderLoop(Camera* camera, std::vector<PhysicsObject*> RenderObjectsP, std::vector<TriggerObject*> RenderObjectsT, bool renderArrows)
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

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    shader.use();

    glm::mat4 view = camera->GetCameraView();
    glm::mat4 projection = glm::perspective(glm::radians(camera->fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    // pass transformation matrices to the shader
    shader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    shader.setMat4("view", view);

    // glBindVertexArray(VAO);

    for (auto& object : RenderObjectsP)
    {
        object->RenderMesh(shader, texture1);

        if (renderArrows) {
            object->RenderArrows(shader, redTexture, blueTexture);
        }
    }

    for (auto& object : RenderObjectsT)
    {
        object->RenderMesh(shader, texture2);
    }


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

    for (auto& mod : modules) {
        if (mod == nullptr) {
            std::cout << "Module is null" << std::endl;
            continue;
        }
        mod->UpdateObjects(RenderObjectsP);
        mod->RenderWindow();
    }

    ImGui::End();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


    glfwSwapBuffers(window);
    glfwPollEvents();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
