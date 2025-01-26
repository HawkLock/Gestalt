#include "TriggerObject.h"


TriggerObject::TriggerObject(glm::vec3 initialPosition, glm::quat initialRot, float faceSize, std::string& modelPath, std::string &texturePath)
	: pos(initialPosition),
	rot(initialRot),
	sideLength(faceSize),
	Model(initialPosition, faceSize, modelPath, texturePath) // Use parameterized constructor
{
	angularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	angularAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);

}

void TriggerObject::ScaleSize(float scale) {
	Model.ChangeSize(scale);
}


std::vector<Vertex> TriggerObject::extractVertices(const std::vector<float>& data) {
	std::vector<Vertex> vertices;
	for (size_t i = 0; i < data.size(); i += 5) {
		vertices.push_back({ data[i], data[i + 1], data[i + 2] });
	}
	return vertices;
}

std::vector<glm::vec3> TriggerObject::GetVertices()
{
	// Retrieves all the corner vertices from the mesh
	std::vector<glm::vec3> vertices;
	for (int i = 0; i < sizeof(Model.verticesU) / sizeof(Model.verticesU[0]);)
	{
		glm::vec3 vertex = glm::vec3(Model.verticesU[i], Model.verticesU[i + 1], Model.verticesU[i + 2]) + pos;
		vertices.push_back(vertex);
		i += 5;
	}
	// Gets indices of all duplicate vertices
	std::vector<float> duplicateVertexIndices;
	for (int i = 0; i < vertices.size(); i++)
	{
		for (int z = i + 1; z < vertices.size(); z++)
		{
			if (!std::count(duplicateVertexIndices.begin(), duplicateVertexIndices.end(), z))
			{
				if (vertices[i] == vertices[z])
				{
					duplicateVertexIndices.push_back(z);
				}
			}
		}
	}
	// Removes duplicate vertices
	for (int i = 0; i < duplicateVertexIndices.size(); i++)
	{
		vertices[duplicateVertexIndices[i]] = vertices.back();
	}
	for (int i = 0; i < duplicateVertexIndices.size(); i++)
	{
		vertices.pop_back();
	}

	return vertices;
}

// Rendering
void TriggerObject::RenderMesh(const Shader& shader, GLuint textureID)
{
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
	shader.setInt("texture2", 1);  // Ensure the uniform sampler is set to the correct texture unit

	// Draw elements using indices
	glDrawElements(GL_TRIANGLES, Model.indices.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO (optional, but good practice)
	glBindVertexArray(0);
}

