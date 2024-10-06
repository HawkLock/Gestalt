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

	momentOfInertia = Mass * pow(sideLength, 2) / 6;
	glm::mat3 mat = glm::mat3(
		1, 0.0f, 0.0f,
		0.0f, 1, 0.0f,
		0.0f, 0.0f, 1
	);
	inertiaTensor = momentOfInertia * mat;
	PrintMat3(inertiaTensor);
	CalculateInertiaTensor(inertiaTensor);
	//PrintMat3(inertiaTensor);

	inverseInertiaTensor = glm::inverse(inertiaTensor);

	Force gravity = Force();
	gravity.ForceName = "Gravity";
	gravity.ForceVector = gravityVec;
	AddForce(gravity, true);

}

glm::vec3 PhysicsObject::CalculateCOM() {
	std::vector<Vertex>& vertices = Model.vertices;
	float totalMass = Mass; // Total mass of the object
	glm::vec3 centerOfMass(0.0f); // To store the center of mass

	// Uniformly distributed mass for each vertex
	float vertexMass = totalMass / vertices.size();

	// Sum of position vectors weighted by vertex mass
	for (size_t i = 0; i < vertices.size(); ++i) {
		glm::vec3 position = glm::vec3(vertices[i].x, vertices[i].y, vertices[i].z);
		centerOfMass += position * vertexMass;
	}

	// Divide by total mass to get the center of mass
	centerOfMass /= totalMass;

	return centerOfMass;
}


// WIP
void PhysicsObject::CalculateInertiaTensor(glm::mat3& tensor) {
	std::vector<Vertex>& vertices = Model.vertices;
	std::vector<unsigned int>& indices = Model.indices;

	glm::mat3 IT = glm::mat3(0.0f);

	// Center of the cube (assuming vertices are centered at origin)
	//glm::vec3 centerOfMass(0.0f, 0.0f, 0.0f); // Adjust if not centered
	glm::vec3 centerOfMass = CalculateCOM(); // Adjust if not centered

	// Iterate through vertices
	for (size_t i = 0; i < vertices.size(); ++i) {
		Vertex& v = vertices[i];

		// Extract vertex position
		glm::vec3 position = glm::vec3(v.x, v.y, v.z);

		// Calculate the distance from the center of mass
		glm::vec3 r = position - centerOfMass;

		// Each vertex contributes to the total mass
		float mass = Mass / (vertices.size() * 3); // Uniform distribution

		// Calculate components of inertia tensor
		IT[0][0] += mass * (r.y * r.y + r.z * r.z); // Ixx
		IT[1][1] += mass * (r.x * r.x + r.z * r.z); // Iyy
		IT[2][2] += mass * (r.x * r.x + r.y * r.y); // Izz

		IT[0][1] -= mass * r.x * r.y; // Ixy
		IT[0][2] -= mass * r.x * r.z; // Ixz
		IT[1][2] -= mass * r.y * r.z; // Iyz
	}

	// Ensure symmetry for the products of inertia
	IT[1][0] = IT[0][1];
	IT[2][0] = IT[0][2];
	IT[2][1] = IT[1][2];

	PrintMat3(IT);

	tensor = IT;
}

void PhysicsObject::ScaleSize(float scale) {
	Model.ChangeSize(scale);
	CalculateInertiaTensor(inertiaTensor);
	inverseInertiaTensor = glm::inverse(inertiaTensor);
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

void PhysicsObject::ApplyForce(const glm::vec3& force, const glm::vec3& contactPoint)
{
	if (Anchored) {
		// Do nothing if the object is anchored
		return;
	}

	// Apply linear force to update the object's linear acceleration
	acceleration += force / Mass;

	//// Calculate the point of application relative to the center of mass (CoM)
	//PrintVec3(contactPoint);
	//glm::vec3 radius = contactPoint - (pos + CalculateCOM());

	//// Compute the torque generated by the force
	//glm::vec3 torque = glm::cross(radius, force);

	//// Update angular acceleration using the inverse inertia tensor (angular version of F = ma)
	//angularAcceleration += inverseInertiaTensor * torque;
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

