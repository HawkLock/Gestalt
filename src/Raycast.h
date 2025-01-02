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
        PhysicsObject* hitObj = nullptr;
        float closestT = FLT_MAX;

        for (PhysicsObject* obj : objects) {
            Mesh* mesh = obj->GetMeshPtr();

            // Get the position and rotation of the object
            glm::vec3 position = obj->pos;
            glm::quat rotation = obj->rot;

            // Iterate over triangles (assuming the mesh has a list of indices and vertices)
            for (size_t i = 0; i < mesh->indices.size(); i += 3) {
                // Get the vertices of the triangle using indices
                unsigned int index0 = mesh->indices[i];
                unsigned int index1 = mesh->indices[i + 1];
                unsigned int index2 = mesh->indices[i + 2];

                // Get the original vertices
                glm::vec3 v0 = glm::vec3(mesh->vertices[index0].x, mesh->vertices[index0].y, mesh->vertices[index0].z);
                glm::vec3 v1 = glm::vec3(mesh->vertices[index1].x, mesh->vertices[index1].y, mesh->vertices[index1].z);
                glm::vec3 v2 = glm::vec3(mesh->vertices[index2].x, mesh->vertices[index2].y, mesh->vertices[index2].z);

                // Apply rotation and translation to the vertices
                v0 = position + rotation * v0;
                v1 = position + rotation * v1;
                v2 = position + rotation * v2;

                // Check intersection with the current triangle
                float t = 0;
                if (RayIntersectsTriangle(ray, v0, v1, v2, t)) {
                    // If the intersection is closer than previous ones, update the closest intersection
                    if (t < closestT) {
                        closestT = t;
                        hitObj = obj;  // Store the object that was hit
                    }
                }
            }
        }

        return hitObj;  // Return the first object hit, or nullptr if no object is hit
    }


private:

    static bool RayIntersectsTriangle(const Ray& ray, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, float& t) {
        const float EPSILON = 1e-8;
        glm::vec3 e1 = v1 - v0;
        glm::vec3 e2 = v2 - v0;
        glm::vec3 h = glm::cross(ray.direction, e2);
        float a = glm::dot(e1, h);

        if (a > -EPSILON && a < EPSILON)
            return false;

        float f = 1.0f / a;
        glm::vec3 s = ray.origin - v0;
        float u = f * glm::dot(s, h);

        if (u < 0.0f || u > 1.0f)
            return false;

        glm::vec3 q = glm::cross(s, e1);
        float v = f * glm::dot(ray.direction, q);

        if (v < 0.0f || u + v > 1.0f)
            return false;

        t = f * glm::dot(e2, q);

        return t > EPSILON; // Ray intersects the triangle if t is positive
    }

};