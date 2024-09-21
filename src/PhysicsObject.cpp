#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(glm::vec3 initialPosition, std::vector<glm::vec3> initialActingForcesVectors, glm::quat initialRot, float initialMass, bool isAnchored, float faceSize, std::string& modelPath)
	: pos(initialPosition),
	rot(initialRot),
	Mass(initialMass),
	Anchored(isAnchored),
	Model(initialPosition, faceSize, modelPath) // Use parameterized constructor
{
	angularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	angularAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);

	momentOfInertia = Mass * pow(sideLength, 2) / 6;
	inertiaTensor = glm::mat3(
		momentOfInertia, 0.0f, 0.0f,
		0.0f, momentOfInertia, 0.0f,
		0.0f, 0.0f, momentOfInertia
	);
	inverseInertiaTensor = glm::inverse(inertiaTensor);

	Force gravity = Force();
	gravity.ForceName = "Gravity";
	gravity.ForceVector = gravityVec;
	AddForce(gravity, true);

}

// Force Manipulation Functions

void PhysicsObject::AddForce(Force newForce, bool isContinuous) {
	if (isContinuous) {
		continuousForces.push_back(newForce);
	}
}

void PhysicsObject::CalculateAcceleration() {
	// Apply Continuous Forces
	// All other forces will have been applied earlier in the physics pass
	for (Force force : continuousForces) {
		acceleration += force.ForceVector / Mass;
	}
}

void PhysicsObject::CalculateVelocity(float deltaTime)
{
	// Linear
	velocity += acceleration * deltaTime;
	
	// Angular
	angularVelocity += angularAcceleration * deltaTime;
}


void PhysicsObject::CalculatePosition(float deltaTime)
{
	// Linear
	pos += (velocity * deltaTime) + (0.5f * acceleration * deltaTime * deltaTime);
	// Angular
	glm::quat angularVelocityQuat(0.0f, angularVelocity.x, angularVelocity.y, angularVelocity.z);
	glm::quat deltaRot = 0.5f * angularVelocityQuat * rot;
	rot += deltaRot * deltaTime;
	rot = glm::normalize(rot);
}

void PhysicsObject::CalculatePhysics(float deltaTime)
{
	CalculateAcceleration();
	CalculatePosition(deltaTime);
	CalculateVelocity(deltaTime); // Position is calculated first because velocity is only updated for the next pass

	// Reset acceleration
	acceleration = glm::vec3();
}

void PhysicsObject::applyFriction(float modifier)
{
	
}

// Energy (Kinetic)
float PhysicsObject::CalculateTranslationalEnergy() {
	return 0.5f * Mass * glm::dot(velocity, velocity);
}
float PhysicsObject::CalculateRotationalEnergy() {
	return 0.5f * momentOfInertia * glm::dot(angularVelocity, angularVelocity);
}
float PhysicsObject::CalculateTotalEnergy() {
	return CalculateTranslationalEnergy() + CalculateRotationalEnergy();
}

// Collision

std::vector<Vertex> PhysicsObject::extractVertices(const std::vector<float>& data) {
	std::vector<Vertex> vertices;
	for (size_t i = 0; i < data.size(); i += 5) {
		vertices.push_back({ data[i], data[i + 1], data[i + 2] });
	}
	return vertices;
}

std::vector<glm::vec3> PhysicsObject::GetVertices()
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
void PhysicsObject::RenderMesh(const Shader& shader, GLuint textureID)
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
	shader.setInt("texture1", 0);  // Ensure the uniform sampler is set to the correct texture unit

	// Draw elements using indices
	glDrawElements(GL_TRIANGLES, Model.indices.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO (optional, but good practice)
	glBindVertexArray(0);
}

