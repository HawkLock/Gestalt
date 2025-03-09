#pragma once
#include <vector>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <regex>
#include <filesystem>

class Recorder {
private:

public:

    static void clearFrameFolder() {
        try {
            // Check if the directory exists
            std::string folderPath = "../Recordings/frames";
            if (std::filesystem::exists(folderPath) && std::filesystem::is_directory(folderPath)) {
                // Iterate through each file in the directory
                for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
                    // Remove each file or subdirectory
                    std::filesystem::remove_all(entry.path());
                }
            }
            else {
                std::cout << "Folder does not exist" << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    static void encodeVideo(int framerate = 30, unsigned int frameCount = 0, const char outputNameBuffer[] = "output", const char outputPathBuffer[] = "../Recordings/") {
        if (frameCount == 0) {
            std::cerr << "Video Encoding Error: Zero Frames" << std::endl;
            return;
        }

        std::string outputName;
        if (std::string(outputNameBuffer) == "") {
            // outputName = "../Recordings/output.mp4";
            std::string pathEnd = "/output.mp4";
            outputName = outputPathBuffer + pathEnd;
        }
        else {
            outputName = std::string(outputPathBuffer) + "/" + std::string(outputNameBuffer) + ".mp4";
        }

        std::string folderPath = "../Recordings/frames/";
        std::string ffmpegCmd = "ffmpeg -y -framerate " + std::to_string(framerate) +
            " -i " + folderPath + "frame_%04d.ppm -vcodec libx264 -pix_fmt yuv420p " + outputName +
            " -frames:v " + std::to_string(frameCount) +
            " > NUL 2>&1";

        system(ffmpegCmd.c_str());
        clearFrameFolder();
        std::cout << "\nEncoding complete! Video saved as " + outputName << std::endl;
    }

    static void saveFrame(std::vector<unsigned char>& frame, unsigned int WIDTH, unsigned int HEIGHT, unsigned int* frameCount) {
        std::ostringstream filename;
        filename << "../Recordings/frames/frame_" << std::setw(4) << std::setfill('0') << *frameCount << ".ppm";

        std::ofstream tempFile(filename.str(), std::ios::binary);
        if (!tempFile) {
            std::cerr << "Error: Could not open file " << filename.str() << " for writing." << std::endl;
            return;
        }

        // Flip image vertically
        std::vector<unsigned char> flippedFrame(WIDTH * HEIGHT * 3);
        for (unsigned int y = 0; y < HEIGHT; ++y) {
            memcpy(&flippedFrame[y * WIDTH * 3], &frame[(HEIGHT - 1 - y) * WIDTH * 3], WIDTH * 3);
        }

        // Write header
        tempFile << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";
        tempFile.write(reinterpret_cast<const char*>(flippedFrame.data()), flippedFrame.size());
        tempFile.close();

        (*frameCount)++;
    }

    
    static void captureFrame(unsigned int WIDTH, unsigned int HEIGHT, unsigned int* frameCount) {
        std::vector<unsigned char> pixels(WIDTH * HEIGHT * 3); // RGB
        glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
        saveFrame(pixels, WIDTH, HEIGHT, frameCount);
    }

};
