#pragma once
#include <vector>
#include "Module.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Recorder.h"

class RecordModule : public Module {
private: 
    unsigned int* frameCount;

    int frameRate = 30;

    char outputNameBuffer[128] = "";
public:

    void HandleData(unsigned int* data) {
        frameCount = data;
    }

    void RenderWindow() override {
        ImGui::Begin("Recording Module");

        std::string buttonName = GlobalData::shouldRecord ? "End Recording" : "Start Recording";

        ImGui::Text("Name: ");  
        ImGui::SameLine();  
        ImGui::InputText("##", outputNameBuffer, IM_ARRAYSIZE(outputNameBuffer)); 

        // Cannot change frame rate during recording
        if (GlobalData::shouldRecord) {
            ImGui::Text("Frame Rate: %d", frameRate);
        }
        else {
            ImGui::SliderInt("Frame Rate: ", &frameRate, 1, 60);
        }

        if (ImGui::Button(buttonName.c_str())) {
            GlobalData::shouldRecord = !GlobalData::shouldRecord;
        }

        if (frameCount != nullptr) {
            float videoLength = *frameCount / (float)frameRate;
            ImGui::Text("Video Length: %.2f", videoLength);

            if (!GlobalData::shouldRecord && *frameCount > 0) {
                GlobalData::paused = true;
                Recorder::encodeVideo(frameRate, *frameCount, outputNameBuffer);
                (*frameCount) = 0;
            }
        }

        ImGui::End();
    }
};
