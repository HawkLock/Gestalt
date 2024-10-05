#include "PhysicsObject.h"

void PrintMat3(const glm::mat3& mat) {
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			std::cout << mat[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void PrintVec3(const glm::vec3& vec) {
	std::cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;
}


PhysicsObject::PhysicsObject(glm::vec3 initialPosition, std::vector<glm::vec3> initialActingForcesVectors, glm::quat initialRot, float initialMass, bool isAnchored, float faceSize, std::string& modelPath)
	: pos(initialPosition),
	rot(initialRot),
	Mass(initialMass),
	Anchored(isAnchored),
	sideLength(faceSize),
	Model(initialPosition, faceSize, modelPath) // Use parameterized constructor
{
	angularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	angularAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);

	std::cout << "Side: " << sideLength << std::endl;
	momentOfInertia = Mass * pow(sideLength, 2) / 6;
	glm::mat3 mat = glm::mat3(
		1, 0.0f, 0.0f,
		0.0f, 1, 0.0f,
		0.0f, 0.0f, 1
	);
	inertiaTensor = momentOfInertia * mat;
	//PrintMat3(inertiaTensor);
	//CalculateInertiaTensor(inertiaTensor);
	//PrintMat3(inertiaTensor);

	inverseInertiaTensor = glm::inverse(inertiaTensor);

	Force gravity = Force();
	gravity.ForceName = "Gravity";
	gravity.ForceVector = gravityVec;
	AddForce(gravity, true);

}

glm::vec3 PhysicsObject::CalculateCOM() {
	glm::vec3 com(0.0f);
	std::vector<Vertex> vertices = Model.vertices;
	for (const auto& vertex : vertices) {
		com += glm::vec3(vertex.x, vertex.y, vertex.z);
	}
	return com / static_cast<float>(vertices.size());
}

// WIP
void PhysicsObject::CalculateInertiaTensor(glm::mat3& tensor) {
	std::vector<Vertex> vertices = Model.vertices;
	size_t N = vertices.size();

	// Calculate the centroid
	glm::vec3 centroid(0.0f);
	for (const auto& vertex : vertices) {
		centroid += glm::vec3(vertex.x, vertex.y, vertex.z);
	}
	centroid /= static_cast<float>(N);

	// Initialize inertia tensor components
	float Ixx = 0.0f, Iyy = 0.0f, Izz = 0.0f;
	float Ixy = 0.0f, Ixz = 0.0f, Iyz = 0.0f;

	// Calculate inertia tensor components
	for (const auto& vertex : vertices) {
		glm::vec3 r = glm::vec3(vertex.x, vertex.y, vertex.z) - centroid; // Position relative to the centroid
		float m_i = Mass / static_cast<float>(N); // Mass of each vertex

		Ixx += m_i * (r.y * r.y + r.z * r.z);
		Iyy += m_i * (r.x * r.x + r.z * r.z);
		Izz += m_i * (r.x * r.x + r.y * r.y);
		Ixy -= m_i * (r.x * r.y);
		Ixz -= m_i * (r.x * r.z);
		Iyz -= m_i * (r.y * r.z);
	}

	// Construct the inertia tensor
	glm::mat3 inertiaTensor = glm::mat3(Ixx, Ixy, Ixz,
		Ixy, Iyy, Iyz,
		Ixz, Iyz, Izz);

	PrintMat3(inertiaTensor);
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

