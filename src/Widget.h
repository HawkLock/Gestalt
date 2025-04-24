#pragma once
#include <vector>
#include <iostream>
#include <cstdio> 

#include "Module.h"

enum TrackerType {
    INT,
    FLOAT,
    BOOL,
    VEC
};
struct Tracker {
    TrackerType type;
    bool modifiable;
    std::string name;

    int* trackInt;
    float* trackFloat;
    bool* trackBool;
    glm::vec3* trackVec;

    // For ranges (only applicable for mutable trackers)
    float min;
    float max;
};

class Widget : public Module{

    std::vector<Tracker*> trackers;

    void RenderTrackerMutable(Tracker* tracker) {
        switch (tracker->type) {
        case INT:
            ImGui::SliderInt(tracker->name.c_str(), tracker->trackInt, tracker->min, tracker->max);
            break;
        case FLOAT:
            ImGui::SliderFloat(tracker->name.c_str(), tracker->trackFloat, tracker->min, tracker->max);
            break;
        case BOOL:
            ImGui::Checkbox(tracker->name.c_str(), tracker->trackBool);
            break;
        case VEC:
            GenerateVectorSubfolder(tracker->name.c_str(), tracker->trackVec, tracker->min, tracker->max);
            break;
        default:
            break;
        }
    }

    void RenderTrackerImmutable(Tracker* tracker) {
        switch (tracker->type) {
            case INT: 
                ImGui::Text("%s: %s", tracker->name.c_str(), std::to_string(*tracker->trackInt).c_str());
                break;
            case FLOAT:
                ImGui::Text("%s: %s", tracker->name.c_str(), std::to_string(*tracker->trackFloat).c_str());
                break;
            case BOOL:
                ImGui::Text("%s: %s", tracker->name.c_str(), *tracker->trackBool ? "True" : "False");
                break;
            case VEC:
                GenerateVectorSubfolderImmutable(tracker->name.c_str(), tracker->trackVec);
                break;
            default:
                break;
        }
    }

public:
    std::string name;

    void AddTracker(std::string name, int* track) {
        Tracker* tracker = new Tracker();
        tracker->name = name;
        tracker->trackInt = track;
        tracker->type = INT;
        tracker->modifiable = false;
        trackers.push_back(tracker);
    }

    void AddTracker(std::string name, int* track, float min, float max) {
        Tracker* tracker = new Tracker();
        tracker->name = name;
        tracker->trackInt = track;
        tracker->type = INT;
        tracker->modifiable = true;
        tracker->min = min;
        tracker->max = max;
        trackers.push_back(tracker);
    }

    void AddTracker(std::string name, float* track) {
        Tracker* tracker = new Tracker();
        tracker->name = name;
        tracker->trackFloat = track;
        tracker->type = FLOAT;
        tracker->modifiable = false;
        trackers.push_back(tracker);
    }

    void AddTracker(std::string name, float* track, float min, float max) {
        Tracker* tracker = new Tracker();
        tracker->name = name;
        tracker->trackFloat = track;
        tracker->type = FLOAT;
        tracker->modifiable = true;
        tracker->min = min;
        tracker->max = max;
        trackers.push_back(tracker);
    }

    void AddTracker(std::string name, bool* track) {
        Tracker* tracker = new Tracker();
        tracker->name = name;
        tracker->trackBool = track;
        tracker->type = BOOL;
        tracker->modifiable = false;
        trackers.push_back(tracker);
    }

    void AddTracker(std::string name, bool* track, bool defaultState) {
        Tracker* tracker = new Tracker();
        tracker->name = name;
        tracker->trackBool = track;
        tracker->type = BOOL;
        tracker->modifiable = false;
        trackers.push_back(tracker);
    }

    void AddTracker(std::string name, glm::vec3* track) {
        Tracker* tracker = new Tracker();
        tracker->name = name;
        tracker->trackVec = track;
        tracker->type = VEC;
        tracker->modifiable = false;
        trackers.push_back(tracker);
    }

    void AddTracker(std::string name, glm::vec3* track, float min, float max) {
        Tracker* tracker = new Tracker();
        tracker->name = name;
        tracker->trackVec = track;
        tracker->type = VEC;
        tracker->modifiable = true;
        tracker->min = min;
        tracker->max = max;
        trackers.push_back(tracker);
    }


    void RenderWindow() override {
        for (Tracker* tracker : trackers) {
            if (tracker->modifiable) {
                RenderTrackerMutable(tracker);
            }
            else {
                RenderTrackerImmutable(tracker);
            }
        }
    }

    Widget (std::string name) {
        this->name = name;
    }
    
};
