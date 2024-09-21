#pragma once

#include "Simplex.h"
#include "ConvexShape.h"
#include <iostream> // Include for printing warnings

// GJK Algorithm class definition
class GJKAlgorithm {
public:

    // Static wrapper function for GJK collision detection with Overlap struct
    static bool GJK(ConvexShape shape1, ConvexShape shape2, Overlap& overlap, Simplex& simplex)
    {

        glm::vec3 penetrationDepth;
        glm::vec3 penetrationNormal;

        // Perform GJK collision detection
        bool collisionDetected = GJKCollisionDetection(shape1, shape2, penetrationDepth, penetrationNormal, simplex);

        if (collisionDetected) {
            // Update the Overlap struct with depth and axis (normal)
            overlap.depth = glm::length(penetrationDepth);
            overlap.axis = glm::normalize(penetrationNormal);
        }

        return collisionDetected;
    }

private:
    // Static function to perform GJK collision detection
    static bool GJKCollisionDetection(const ConvexShape& shape1, const ConvexShape& shape2, glm::vec3& penetrationDepth, glm::vec3& penetrationNormal, Simplex& simplex) {
        // Compute initial direction based on centroids
        glm::vec3 center1 = shape1.GetCentroid(); // Assume this method exists
        glm::vec3 center2 = shape2.GetCentroid();
        glm::vec3 initialDirection = glm::normalize(center2 - center1);

        glm::vec3 support = GetSupport(shape1, shape2, initialDirection);
        simplex.push_front(support);
        glm::vec3 direction = -support;

        // Perform GJK iterations
        while (true) {
            glm::vec3 support = GetSupport(shape1, shape2, direction);

            if (glm::dot(support, direction) <= 0.0f) {
                return false; // No collision
            }

            simplex.push_front(support);

            if (UpdateSimplex(simplex, direction)) {
                return true; // Collision detected
            }
        }
    }

    static glm::vec3 GetSupport(ConvexShape shape1, ConvexShape shape2, glm::vec3 direction) {
        return shape1.GetSupportPoint(direction) - shape2.GetSupportPoint(-direction);
    }

    static bool SameDirection(const glm::vec3& direction, const glm::vec3& ao) {
        return glm::dot(direction, ao) > 0;
    }

    // Static additional function prototypes and implementations
    static bool UpdateSimplex(Simplex& simplex, glm::vec3& direction) {
        switch (simplex.size()) {
        case 2: return UpdateLineSimplex(simplex, direction);
        case 3: return UpdateTriangleSimplex(simplex, direction);
        case 4: return UpdateTetrahedronSimplex(simplex, direction);
        }
        return false;
    }

    static bool UpdateLineSimplex(Simplex& simplex, glm::vec3& direction) {
        glm::vec3 a = simplex[0];
        glm::vec3 b = simplex[1];

        glm::vec3 ab = b - a;
        glm::vec3 ao = -a;

        if (SameDirection(ab, ao)) {
            direction = glm::cross(glm::cross(ab, ao), ab);
        }
        else {
            // Check for collinearity
            if (glm::length(glm::cross(ab, ao)) < 1e-6f) { // Collinear
                // Set direction to a default orthogonal direction
                direction = glm::vec3(0.0f, 1.0f, 0.0f); // Example direction
            }
            else {
                simplex = { a }; // Keep only point a
                direction = ao; // Update direction towards point a
            }
        }

        return false; // No collision detected yet
    }

    static bool UpdateTriangleSimplex(Simplex& simplex, glm::vec3& direction) {
        glm::vec3 a = simplex[0];
        glm::vec3 b = simplex[1];
        glm::vec3 c = simplex[2];

        glm::vec3 ab = b - a;
        glm::vec3 ac = c - a;
        glm::vec3 ao = -a;

        glm::vec3 abc = glm::cross(ab, ac);

        if (SameDirection(glm::cross(abc, ac), ao)) {
            if (SameDirection(ac, ao)) {
                simplex = { a, c };
                direction = glm::cross(glm::cross(ac, ao), ac);
            }
            else {
                // Check for collinearity
                if (glm::length(abc) < 1e-6f) { // Collinear
                    // Set direction to a default orthogonal direction
                    direction = glm::vec3(0.0f, 1.0f, 0.0f); // Example direction
                }
                else {
                    return UpdateLineSimplex(simplex = { a, b }, direction);
                }
            }
        }
        else {
            if (SameDirection(glm::cross(ab, abc), ao)) {
                return UpdateLineSimplex(simplex = { a, b }, direction);
            }
            else {
                if (SameDirection(abc, ao)) {
                    direction = abc;
                }
                else {
                    simplex = { a, c, b };
                    direction = -abc;
                }
            }
        }

        return false; // No collision detected yet
    }


    static bool UpdateTetrahedronSimplex(Simplex& simplex, glm::vec3& direction) {
        glm::vec3 a = simplex[0];
        glm::vec3 b = simplex[1];
        glm::vec3 c = simplex[2];
        glm::vec3 d = simplex[3];

        glm::vec3 ab = b - a;
        glm::vec3 ac = c - a;
        glm::vec3 ad = d - a;
        glm::vec3 ao = -a;

        glm::vec3 abc = glm::cross(ab, ac);
        glm::vec3 acd = glm::cross(ac, ad);
        glm::vec3 adb = glm::cross(ad, ab);

        if (SameDirection(abc, ao)) {
            return UpdateTriangleSimplex(simplex = { a, b, c }, direction);
        }

        if (SameDirection(acd, ao)) {
            return UpdateTriangleSimplex(simplex = { a, c, d }, direction);
        }

        if (SameDirection(adb, ao)) {
            return UpdateTriangleSimplex(simplex = { a, d, b }, direction);
        }

        return true;
    }
};

// Code adapted from https://winter.dev/
