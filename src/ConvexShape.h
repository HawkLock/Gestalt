#pragma once

#include <vector>
#include <array>
#include <initializer_list>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

// Struct to represent collision overlap
struct Overlap {
    float depth;
    glm::vec3 axis;
    glm::vec3 collisionPoint;
};

// ConvexShape class definition
class ConvexShape {
public:
    ConvexShape(const std::vector<glm::vec3>& vertices, glm::quat orientation, glm::vec3 pos) : vertices(vertices), orientation(orientation), pos(pos){
        // Initialize the shape with vertices
    }

    glm::vec3 GetSupportPoint(const glm::vec3& direction) const {
        glm::vec3 supportPoint = vertices[0];
        float maxDot = glm::dot(supportPoint, direction);

        for (const auto& vertex : vertices) {
            float dotProduct = glm::dot(vertex, direction);
            if (dotProduct > maxDot) {
                maxDot = dotProduct;
                supportPoint = vertex;
            }
        }

        return supportPoint;
    }

    glm::vec3 GetCentroid() const {
        glm::vec3 centroid(0.0f);
        for (const auto& vertex : vertices) {
            centroid += vertex;
        }
        return centroid / static_cast<float>(vertices.size());
    }

    glm::quat GetOrientation() {
        return orientation;
    }

    glm::vec3 GetPos() {
        return pos;
    }

    glm::vec3 GetMinBounds() const {
        if (vertices.empty()) return glm::vec3(0.0f); // Handle empty case

        glm::vec3 minBounds = vertices[0];
        for (const auto& vertex : vertices) {
            minBounds = glm::min(minBounds, vertex); // Component-wise min
        }
        return minBounds;
    }

    glm::vec3 GetMaxBounds() const {
        if (vertices.empty()) return glm::vec3(0.0f); // Handle empty case

        glm::vec3 maxBounds = vertices[0];
        for (const auto& vertex : vertices) {
            maxBounds = glm::max(maxBounds, vertex); // Component-wise max
        }
        return maxBounds;
    }


private:
    std::vector<glm::vec3> vertices;
    glm::quat orientation;
    glm::vec3 pos;
};

// Code adapted from https://winter.dev/
