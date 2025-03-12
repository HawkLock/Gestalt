#pragma once

#include "Mesh.h"
#include "PhysicsObject.h"
#include <iostream> 

class Raycast {
public:
    struct Ray {
        glm::vec3 origin;
        glm::vec3 direction;
        
    };

    static PhysicsObject* CheckRayIntersection(const Ray& ray, const std::vector<PhysicsObject*>& objects) {
        float closestIntersection = FLT_MAX;
        PhysicsObject* closestObject = nullptr;
        glm::vec3 closestIntersectionPoint;

        for (auto& object : objects) {
            std::vector<glm::vec3> vertices = object->GetVertices();
            for (size_t i = 0; i < vertices.size()-2; i += 3) {
                glm::vec3 v0 = vertices[i];
                glm::vec3 v1 = vertices[i + 1];
                glm::vec3 v2 = vertices[i + 2];

                float t = 0.0f;
                if (RayIntersectsTriangle(ray, v0, v1, v2, t)) {
                    if (t < closestIntersection) {
                        closestIntersection = t;
                        closestObject = object;
                        closestIntersectionPoint = ray.origin + ray.direction * t;
                    }
                }
            }
        }

        return closestObject;

    }



private:

    static bool RayIntersectsTriangle(const Ray& ray, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, float& t) {
        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 h = glm::cross(ray.direction, edge2);

        float EPSILON = 1e-6;
        float a = glm::dot(edge1, h);

        if (a > -EPSILON && a < EPSILON) {
            return false;  // The ray is parallel to the triangle
        }

        float f = 1.0f / a;
        glm::vec3 s = ray.origin - v0;
        float u = f * glm::dot(s, h);
        if (u < 0.0f || u > 1.0f) {
            return false;
        }

        glm::vec3 q = glm::cross(s, edge1);
        float v = f * glm::dot(ray.direction, q);
        if (v < 0.0f || u + v > 1.0f) {
            return false;
        }

        t = f * glm::dot(edge2, q);
        return t > EPSILON;  // t is the intersection distance along the ray
    }


};