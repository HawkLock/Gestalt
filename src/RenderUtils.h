#pragma once
#include <glm/glm/glm.hpp>
#include <iostream>
#include <vector>
#include <stack>

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


};