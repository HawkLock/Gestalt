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
};

// ConvexShape class definition
class ConvexShape {
public:
    ConvexShape(const std::vector<glm::vec3>& vertices) : vertices(vertices) {
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

private:
    std::vector<glm::vec3> vertices;
};

// Code adapted from https://winter.dev/