#pragma once

#include <vector>
#include <glm/glm/glm.hpp>
#include <limits>
#include <algorithm>

// A structure to represent a simplex (point, line, triangle, tetrahedron)
struct Simplex {
    std::vector<glm::vec3> points;

    void add(const glm::vec3& point) {
        points.push_back(point);
    }

    void clear() {
        points.clear();
    }

    size_t size() const {
        return points.size();
    }

    const glm::vec3& operator[](size_t index) const {
        return points[index];
    }
};


class GJKAlgorithm {
public:
    // Compute the support point for a shape in a given direction
    static glm::vec3 Support(const std::vector<glm::vec3>& verticesA, const std::vector<glm::vec3>& verticesB, const glm::vec3& direction) {
        glm::vec3 farthestA = verticesA[0];
        float maxDotA = glm::dot(farthestA, direction);
        for (const auto& vertex : verticesA) {
            float dotProduct = glm::dot(vertex, direction);
            if (dotProduct > maxDotA) {
                maxDotA = dotProduct;
                farthestA = vertex;
            }
        }

        glm::vec3 farthestB = verticesB[0];
        float maxDotB = glm::dot(farthestB, -direction);
        for (const auto& vertex : verticesB) {
            float dotProduct = glm::dot(vertex, -direction);
            if (dotProduct > maxDotB) {
                maxDotB = dotProduct;
                farthestB = vertex;
            }
        }

        return farthestA - farthestB;
    }

    static bool PointInTriangle(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
        glm::vec3 v0 = b - a;
        glm::vec3 v1 = c - a;
        glm::vec3 v2 = p - a;

        float dot00 = glm::dot(v0, v0);
        float dot01 = glm::dot(v0, v1);
        float dot02 = glm::dot(v0, v2);
        float dot11 = glm::dot(v1, v1);
        float dot12 = glm::dot(v1, v2);

        float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
        float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
        float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

        return (u >= 0) && (v >= 0) && (u + v <= 1);
    }

    static bool PointInTetrahedron(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d) {
        glm::vec3 n1 = glm::cross(b - a, c - a);
        glm::vec3 n2 = glm::cross(c - b, d - b);
        glm::vec3 n3 = glm::cross(d - c, a - c);
        glm::vec3 n4 = glm::cross(a - d, b - d);

        bool b1 = glm::dot(n1, p - a) >= 0;
        bool b2 = glm::dot(n2, p - b) >= 0;
        bool b3 = glm::dot(n3, p - c) >= 0;
        bool b4 = glm::dot(n4, p - d) >= 0;

        return (b1 && b2 && b3 && b4) || (!b1 && !b2 && !b3 && !b4);
    }

    static bool IsOriginInsideSimplex(const Simplex& simplex) {
        if (simplex.size() == 1) {
            return false; // Point
        }
        if (simplex.size() == 2) {
            // Line
            const glm::vec3& a = simplex[0];
            const glm::vec3& b = simplex[1];
            glm::vec3 ab = b - a;
            glm::vec3 ao = -a;
            return glm::dot(ab, ao) >= 0 && glm::dot(ab, b - ao) >= 0;
        }
        if (simplex.size() == 3) {
            // Triangle
            const glm::vec3& a = simplex[0];
            const glm::vec3& b = simplex[1];
            const glm::vec3& c = simplex[2];
            return PointInTriangle(glm::vec3(0, 0, 0), a, b, c);
        }
        if (simplex.size() == 4) {
            // Tetrahedron
            const glm::vec3& a = simplex[0];
            const glm::vec3& b = simplex[1];
            const glm::vec3& c = simplex[2];
            const glm::vec3& d = simplex[3];
            return PointInTetrahedron(glm::vec3(0, 0, 0), a, b, c, d);
        }
        return false; // If the simplex has more than 4 points, it's not handled
    }

    static glm::vec3 UpdateDirection(const Simplex& simplex) {
        switch (simplex.size()) {
        case 1: {
            // Point: No direction update required
            return glm::vec3(1, 0, 0); // Placeholder direction
        }
        case 2: {
            // Line: Perpendicular to the line segment
            const glm::vec3& a = simplex[0];
            const glm::vec3& b = simplex[1];
            glm::vec3 ab = b - a;
            return glm::normalize(glm::cross(glm::vec3(0, 0, 1), ab)); // Perpendicular direction (simple choice)
        }
        case 3: {
            // Triangle: Normal of the triangle's plane
            const glm::vec3& a = simplex[0];
            const glm::vec3& b = simplex[1];
            const glm::vec3& c = simplex[2];
            glm::vec3 ab = b - a;
            glm::vec3 ac = c - a;
            glm::vec3 normal = glm::cross(ab, ac);
            return glm::normalize(glm::dot(normal, a) < 0 ? normal : -normal);
        }
        case 4: {
            // Tetrahedron: Direction based on the closest face to the origin
            const glm::vec3& a = simplex[0];
            const glm::vec3& b = simplex[1];
            const glm::vec3& c = simplex[2];
            const glm::vec3& d = simplex[3];

            glm::vec3 n1 = glm::cross(b - a, c - a); // Face (a, b, c)
            glm::vec3 n2 = glm::cross(c - b, d - b); // Face (b, c, d)
            glm::vec3 n3 = glm::cross(d - c, a - c); // Face (c, d, a)
            glm::vec3 n4 = glm::cross(a - d, b - d); // Face (d, a, b)

            // Compute dot products with each vertex to find the closest face
            float dot1 = glm::dot(n1, a);
            float dot2 = glm::dot(n2, b);
            float dot3 = glm::dot(n3, c);
            float dot4 = glm::dot(n4, d);

            float maxDot = std::max({ dot1, dot2, dot3, dot4 });

            if (maxDot == dot1) return glm::normalize(n1);
            if (maxDot == dot2) return glm::normalize(n2);
            if (maxDot == dot3) return glm::normalize(n3);
            return glm::normalize(n4);
        }
        default: {
            // Handle unexpected simplex sizes
            return glm::vec3(1, 0, 0); // Placeholder direction
        }
        }
    }


    static bool GJK(const std::vector<glm::vec3>& verticesA, const std::vector<glm::vec3>& verticesB) {
        glm::vec3 direction(1, 0, 0); // Initial direction
        Simplex simplex;

        // Get the initial point of the Minkowski Difference
        glm::vec3 point = Support(verticesA, verticesB, direction);
        simplex.add(point);
        direction = -point;

        while (true) {
            // Get the farthest point in the current direction
            point = Support(verticesA, verticesB, direction);

            if (glm::dot(point, direction) <= 0) {
                // The origin is not within the Minkowski Difference
                return false;
            }

            simplex.add(point);

            if (IsOriginInsideSimplex(simplex)) {
                // The origin is within the Minkowski Difference
                return true;
            }

            direction = UpdateDirection(simplex);
        }
    }

};