#pragma once
#include <vector>
#include "Module.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Recorder.h"
#include "NFD/include/nfd.h"

class RecordModule : public Module {
private: 
    unsigned int* frameCount;

    int frameRate = 30;

    char outputNameBuffer[128] = "";
    char outputPathBuffer[128] = "../Recordings/";
public:

    void HandleData(unsigned int* data) {
        frameCount = data;
    }

    void RenderWindow() override {
        ImGuiWindowFlags windowFlag = GlobalData::inFocus ? ImGuiWindowFlags_NoInputs : 0;
        ImGui::Begin("Recording Module", nullptr, windowFlag);

        std::string buttonName = GlobalData::shouldRecord ? "End Recording" : "Start Recording";

        ImGui::Text("Name: ");  
        ImGui::SameLine();  
        ImGui::InputText("##", outputNameBuffer, IM_ARRAYSIZE(outputNameBuffer)); 
        ImGui::SameLine();
        ImGui::Text(".mp4");

        if (ImGui::Button(buttonName.c_str())) {
            GlobalData::shouldRecord = !GlobalData::shouldRecord;
            if (GlobalData::recordingUnpauses && GlobalData::paused) {
                GlobalData::paused = false;
            }
        }

        ImGui::SameLine();

        ImGui::Checkbox("Include GUI", &GlobalData::includeGUI);
        ImGui::SameLine();
        ImGui::Checkbox("Unpause", &GlobalData::recordingUnpauses);

        if (ImGui::Button("Choose Output Path")) {
            nfdchar_t *outPath = NULL;
            nfdresult_t result = NFD_PickFolder(NULL, &outPath);

            if (result == NFD_OKAY) {
                strncpy_s(outputPathBuffer, sizeof(outputPathBuffer), outPath, _TRUNCATE);
                free(outPath);
            }
        }
        if (outputPathBuffer[0] != '\0') {
            ImGui::SameLine();
            std::string outputPath = std::string(outputPathBuffer);
            ImGui::Text("Path: %s", outputPath.c_str());
        }

        // Cannot change frame rate during recording
        if (GlobalData::shouldRecord) {
            ImGui::Text("Frame Rate: %d", frameRate);
        }
        else {
            ImGui::SliderInt("Frame Rate: ", &frameRate, 1, 60);
        }

        if (frameCount != nullptr) {
            float videoLength = *frameCount / (float)frameRate;
            ImGui::Text("Video Length: %.2f", videoLength);

            if (!GlobalData::shouldRecord && *frameCount > 0) {
                GlobalData::paused = true;
                std::cout << std::string(outputPathBuffer) << std::endl;
                Recorder::encodeVideo(frameRate, *frameCount, outputNameBuffer, outputPathBuffer);
                (*frameCount) = 0;
            }
        }

        ImGui::End();
    }
};
