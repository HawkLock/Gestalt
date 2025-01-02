#pragma once

#include <vector>
#include <array>
#include <initializer_list>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

struct Simplex {
private:
    std::array<glm::vec3, 4> m_points;
    int m_size;

public:
    Simplex()
        : m_size(0)
    {}

    Simplex& operator=(std::initializer_list<glm::vec3> list)
    {
        m_size = 0;

        for (const glm::vec3& point : list)
            m_points[m_size++] = point;

        return *this;
    }

    void push_front(const glm::vec3& point)
    {
        m_points = { point, m_points[0], m_points[1], m_points[2] };
        m_size = std::min(m_size + 1, 4);
    }

    bool containsOrigin() {
        // This function checks if the origin is inside the simplex
        // A simple way is to check if the origin is contained within a triangle or tetrahedron.

        // For a 2D triangle (you can extend for higher dimensions)
        if (m_size == 3) {
            glm::vec3 a = m_points[0];
            glm::vec3 b = m_points[1];
            glm::vec3 c = m_points[2];

            glm::vec3 ab = b - a;
            glm::vec3 ac = c - a;
            glm::vec3 ao = -a;

            glm::vec3 n = glm::cross(ab, ac);

            // Check if the origin is on the same side of each triangle edge
            return glm::dot(glm::cross(ab, n), ao) > 0 &&
                glm::dot(glm::cross(ac, n), ao) > 0 &&
                glm::dot(glm::cross(b - c, n), ao) > 0;
        }

        // Extend for other simplex shapes (tetrahedron, etc.)
        return false; // Default for more complex shapes
    }

    glm::vec3 getNextSearchDirection() {
        // For simplicity, assume we're working in 2D with a triangle simplex

        if (m_size == 3) {
            glm::vec3 a = m_points[0];
            glm::vec3 b = m_points[1];
            glm::vec3 c = m_points[2];

            // Get the direction to the furthest point from the origin
            glm::vec3 ab = b - a;
            glm::vec3 ac = c - a;
            glm::vec3 ao = -a;

            // Determine which edge to continue along based on the origin
            glm::vec3 n = glm::cross(ab, ac);
            if (glm::dot(n, ao) > 0) {
                return glm::cross(ab, n);  // Direction to continue searching
            }
            else {
                return glm::cross(ac, n);  // Direction to continue searching
            }
        }

        // Extend for more complex simplexes (tetrahedron, etc.)
        return glm::vec3(0.0f); // Default
    }


    glm::vec3& operator[](int i) { return m_points[i]; }
    size_t size() const { return m_size; }

    auto begin() const { return m_points.begin(); }
    auto end() const { return m_points.begin() + m_size; }
};


// Code adapted from https://winter.dev/