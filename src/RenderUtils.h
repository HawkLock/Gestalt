#pragma once
#include <glm/glm/glm.hpp>

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <functional>
#include <cstdio>

#include "PhysicsUtility.h"
#include "Mesh.h"

class RenderUtils {

public:

	static void RenderMesh(const Shader& shader, GLuint textureID, Mesh& Model, glm::vec3 pos, glm::quat rot) {
		// Compute the model matrix
		glm::mat4 modelMatrix = glm::mat4(1.0f); // Identity matrix
		modelMatrix = glm::translate(modelMatrix, pos); // Apply translation
		modelMatrix *= glm::mat4_cast(rot); // Apply rotation

		// Optionally apply scale
		// modelMatrix = glm::scale(modelMatrix, scale);

		// Bind the VAO associated with the mesh
		Model.Bind();

		// Bind the texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Bind the shader and set uniforms
		shader.use();
		shader.setMat4("model", modelMatrix);
		shader.setInt("texture1", 0);  // Ensure the uniform sampler is set to the correct texture unit

		// Draw elements using indices
		glDrawElements(GL_TRIANGLES, Model.indices.size(), GL_UNSIGNED_INT, 0);

		// Unbind the VAO (optional, but good practice)
		glBindVertexArray(0);
	}

	/*
	Call with: 
		RenderUtils::PrintExtent(ArrowModel, [](const Vertex& v) { return v.ATTRIBUTE; });
	*/
	static void PrintExtent(Mesh& model, std::function<float(const Vertex&)> accessor) {
		if (model.vertices.empty()) {
			printf("The mesh is empty.\n");
			return;
		}

		auto minmax = std::minmax_element(model.vertices.begin(), model.vertices.end(),
			[&accessor](const Vertex& a, const Vertex& b) {
				return accessor(a) < accessor(b);
			});

		float minExtent = accessor(*minmax.first);
		float maxExtent = accessor(*minmax.second);

		// Output results.
		printf("Min extent: %f\n", minExtent);
		printf("Max extent: %f\n", maxExtent);
	}

	static float CalculateExtent(Mesh& model, std::function<float(const Vertex&)> accessor) {
		if (model.vertices.empty()) {
			printf("The mesh is empty.\n");
			return -1;
		}

		auto minmax = std::minmax_element(model.vertices.begin(), model.vertices.end(),
			[&accessor](const Vertex& a, const Vertex& b) {
				return accessor(a) < accessor(b);
			});

		float minExtent = accessor(*minmax.first);
		float maxExtent = accessor(*minmax.second);

		// Output results.
		return maxExtent - minExtent;
	}


};