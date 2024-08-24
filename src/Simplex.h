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

    glm::vec3& operator[](int i) { return m_points[i]; }
    size_t size() const { return m_size; }

    auto begin() const { return m_points.begin(); }
    auto end() const { return m_points.begin() + m_size; }
};


// Code adapted from https://winter.dev/