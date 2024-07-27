#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(glm::vec3 initialPosition, glm::vec3 initialVelocity, std::vector<glm::vec3> initialActingForcesVectors, glm::quat initialRot, float initialMass, glm::vec3 gravity, bool isAnchored, float faceSize, std::vector<float> inputVertices)
    : pos(initialPosition),
      velocity(initialVelocity),
	  rot(initialRot),
      Mass(initialMass),
      Anchored(isAnchored),
      Model(initialPosition, faceSize, inputVertices) // Use parameterized constructor
{
    generateForceVectorFromVec3Vector(initialActingForcesVectors);
    AddForce(gravity, "Gravity");
}

PhysicsObject::PhysicsObject(glm::vec3 initialPosition, glm::vec3 initialVelocity, std::vector<glm::vec3> initialActingForcesVectors, glm::quat initialRot, float initialMass, glm::vec3 gravity, bool isAnchored, float faceSize, std::string& modelPath)
	: pos(initialPosition),
	velocity(initialVelocity),
	rot(initialRot),
	Mass(initialMass),
	Anchored(isAnchored),
	Model(initialPosition, faceSize, modelPath) // Use parameterized constructor
{
	generateForceVectorFromVec3Vector(initialActingForcesVectors);
	AddForce(gravity, "Gravity");
}

// Force Manipulation Functions
unsigned int PhysicsObject::FindForceIndex(std::string forceName)
{
	for (unsigned int i = 0; i < ActingForces.size(); i++)
	{
		if (ActingForces[i].ForceName == forceName)
		{
			return i;
		}
	}
	return -1;
	// If the given force does not exist in the acting forces vector of the physics object
}

void PhysicsObject::AddForce(glm::vec3 newForceVector, std::string forceName)
{
	Force newForce;
	newForce.ForceVector = newForceVector;
	newForce.ForceName = forceName;
	ActingForces.push_back(newForce);
	CalculateNetForce();
}

void PhysicsObject::RemoveForce(std::string forceName)
{
	unsigned int forceIndex = FindForceIndex(forceName);
	// If there is no force by that name
	if (forceIndex == -1) {
		return;
	}
	ActingForces.erase(ActingForces.begin() + forceIndex - 1);
	CalculateNetForce();
}

void PhysicsObject::AddInstantaneousForce(glm::vec3 instantForce) {
	//pos += instantForce;
	Force newForce;
	newForce.ForceName = "InstantForce";
	newForce.ForceVector = instantForce;
	ForceStack.push(newForce);
}

void PhysicsObject::AddInstantaneousTorque(PhysicsUtility::Torque torque) {
	TorqueStack.push(torque);
}

void PhysicsObject::CalculateNetForce()
{
	glm::vec3 newNetForce = glm::vec3(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < ActingForces.size(); i++)
	{
		newNetForce += ActingForces[i].ForceVector;
	}
	while (!ForceStack.empty()) {
		newNetForce += ForceStack.top().ForceVector;
		ForceStack.pop();
	}
	NetForce = newNetForce;
	glm::vec3 acceleration = NetForce / Mass;
	velocity += acceleration;
}

void PhysicsObject::CalculateNetTorque() {
	PhysicsUtility::Torque newNetTorque = PhysicsUtility::Torque();
	for (unsigned int i = 0; i < ActingTorques.size(); i++) {
		newNetTorque.AddTorque(ActingTorques[i]);
	}
	while (!TorqueStack.empty()) {
		newNetTorque.AddTorque(TorqueStack.top());
		TorqueStack.pop();
	}
	NetTorque = newNetTorque;
}

void PhysicsObject::CalculatePhysics(float deltaTime)
{
	CalculateNetForce();
	pos += velocity * deltaTime;
	//applyFriction(deltaTime);
	//printf("Pos: %f, %f, %f \n", pos.x, pos.y, pos.z);
}

std::vector<Force> PhysicsObject::generateForceVectorFromVec3Vector(std::vector<glm::vec3> vec3Vector) {
	std::vector<Force> forceVector = std::vector<Force>();
	for (glm::vec3 vec : vec3Vector) {
		AddForce(vec, "");
	}
	return forceVector;
}

void PhysicsObject::applyFriction(float modifier)
{
	velocity -= velocity * (universalVelocityDecay * modifier);
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

