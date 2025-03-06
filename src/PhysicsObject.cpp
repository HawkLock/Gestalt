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

PhysicsObject::PhysicsObject(glm::vec3 initialPosition, std::vector<glm::vec3> initialActingForcesVectors, glm::quat initialRot, float initialMass, bool isAnchored, float faceSize, std::string& modelPath, std::string &texturePath, std::string& arrowModelPath)
	: pos(initialPosition),
	rot(initialRot),
	Mass(initialMass),
	Anchored(isAnchored),
	sideLength(faceSize),
	Model(initialPosition, faceSize, modelPath, texturePath), // Use parameterized constructor
	ArrowModel(initialPosition, faceSize, arrowModelPath, texturePath)
{
	this->modelPath = modelPath;
	this->texturePath = texturePath;
	this->arrowPath = arrowModelPath;
	this->oriignalFaceSize = faceSize;

	COM = CalculateCOM();

	// Scale size of arrows
	ArrowModel.ChangeSize(1.5f);
	arrowModelOffset = RenderUtils::CalculateExtent(ArrowModel, [](const Vertex& v) { return v.z; });

	angularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	angularAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);

	momentOfInertia = Mass * pow(sideLength, 2) / 6;
	glm::mat3 mat = glm::mat3(
		1, 0.0f, 0.0f,
		0.0f, 1, 0.0f,
		0.0f, 0.0f, 1
	);
	inertiaTensor = momentOfInertia * mat;
	//PrintMat3(inertiaTensor);
	CalculateInertiaTensor(inertiaTensor);
	//PrintMat3(inertiaTensor);

	inverseInertiaTensor = glm::inverse(inertiaTensor);
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

glm::vec3 PhysicsObject::CalculateCenter() {
	glm::vec3 center = pos;
	for (const auto& vertex : Model.vertices) {
		center += glm::vec3(vertex.x, vertex.y, vertex.z);
	}
	return center / (float) Model.vertices.size();
}

float PhysicsObject::CalculateRadius(const glm::vec3 center) {
	float maxRadius = 0.0;
	for (const auto& vertex : Model.vertices) {
		float distance = glm::length((glm::vec3(vertex.x, vertex.y, vertex.z) - center));
		if (distance > maxRadius) {
			maxRadius = distance;
		}
	}
	return maxRadius;
}

void PhysicsObject::CalculateBoundingSphere(glm::vec3& center, float& radius) {
	center = CalculateCenter();
	radius = CalculateRadius(center);
}

PhysicsObject::AABB PhysicsObject::GetWorldAABB() {
	glm::vec3 min = glm::vec3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	glm::vec3 max = glm::vec3(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

	for (Vertex vertex : Model.vertices) {
		glm::vec3 vert = glm::vec3(vertex.x, vertex.y, vertex.z) + pos;
		if (vert.x < min.x) {
			min.x = vert.x;
		}
		if (vert.y < min.y) {
			min.y = vert.y;
		}
		if (vert.z < min.z) {
			min.z = vert.z;
		}

		if (vert.x > max.x) {
			max.x = vert.x;
		}
		if (vert.y > max.y) {
			max.y = vert.y;
		}
		if (vert.z > max.z) {
			max.z = vert.z;
		}
	}
	
	return AABB(min, max);
}

// WIP
void PhysicsObject::CalculateInertiaTensor(glm::mat3& tensor) {
	float width = Model.faceLength;  // Assuming cube, adjust for non-cubes
	float height = Model.faceLength;
	float depth = Model.faceLength;

	float Ixx = (1.0f / 12.0f) * Mass * (height * height + depth * depth);
	float Iyy = (1.0f / 12.0f) * Mass * (width * width + depth * depth);
	float Izz = (1.0f / 12.0f) * Mass * (width * width + height * height);

	tensor = glm::mat3(
		Ixx, 0.0f, 0.0f,
		0.0f, Iyy, 0.0f,
		0.0f, 0.0f, Izz
	);
	inertiaTensor = tensor;

	// Precompute the inverse inertia tensor
	inverseInertiaTensor = glm::inverse(tensor);
}


void PhysicsObject::ScaleSize(float scale) {

	Model.ChangeSize(scale);
	std::cout << GetFaceExtent() << std::endl;
	Model.faceLength *= scale;

	// Recompute inertia tensor with scaling rule
	CalculateInertiaTensor(inertiaTensor);

	inverseInertiaTensor = glm::inverse(inertiaTensor);
}


// Force Manipulation Functions

void PhysicsObject::AddForce(Force newForce, bool isContinuous) {
	if (isContinuous) {
		continuousForces.push_back(newForce);
	}
}

void PhysicsObject::CalculateAcceleration(glm::vec3 gravity) {
	// Apply Continuous Forces
	// All other forces will have been applied earlier in the physics pass
	for (Force force : continuousForces) {
		acceleration += force.ForceVector / Mass;
	}
	acceleration += gravity;
	lastRecordedAcceleration = acceleration;
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
	glm::quat deltaRot = glm::quat(0, angularVelocity * deltaTime * 0.5f);
	rot += deltaRot * rot;
	rot = glm::normalize(rot);	
}

void PhysicsObject::CalculatePhysics(float deltaTime, glm::vec3 gravity)
{
	CalculateAcceleration(gravity);
	CalculateVelocity(deltaTime);
	CalculatePosition(deltaTime);

	// Reset accelerations
	acceleration = glm::vec3();
	angularAcceleration = glm::vec3(0.0f);
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
void PhysicsObject::RenderMesh(const Shader& shader)
{
	RenderUtils::RenderMesh(shader, Model.texture, Model, pos, rot);
}

void PhysicsObject::SetArrowBaseModeScalel(float scale) {
	arrowLengthScale = scale;
	ScaleArrowModel(1.0f);
}

// NEEDS OPTIMIZATION
// CURRENTLY RECALCULATES VERTICES MULTIPLE TIMES A FRAME REGARDLESS OF WHETHER THERE WERE CHANGES OR NOT
void PhysicsObject::ScaleArrowModel(float size) {
	ArrowModel.ChangeSizeFromOriginal(size * arrowLengthScale);
	//ArrowModel.ChangeSizeFromOriginalSingleDimension(size * arrowLengthScale, 'z');
	arrowModelOffset = RenderUtils::CalculateExtent(ArrowModel, [](const Vertex& v) { return v.z; });
}

void PhysicsObject::RenderArrow(const Shader& shader, GLuint textureID, glm::vec3 dispVec) {
	// Default arrow model direction (assumes +Z axis)
	glm::vec3 arrowDirection = glm::vec3(0.0f, 0.0f, 1.0f);

	if (glm::length(dispVec) > 0.0f) {
		glm::vec3 modifiedVec = glm::vec3(dispVec.x, dispVec.y, dispVec.z);
		ScaleArrowModel(glm::length(modifiedVec));
		glm::vec3 vecDir = glm::normalize(modifiedVec);
		glm::quat vecRot = glm::rotation(arrowDirection, vecDir);
		glm::quat finalVecRot = vecRot; // Combine object rotation with arrow rotation

		// Transform the local offset into world space
		glm::vec3 arrowOffsetLocal = glm::vec3(0.0f, 0.0f, arrowModelOffset);
		glm::vec3 arrowOffsetWorld = finalVecRot * arrowOffsetLocal;

		// Calculate the correct position
		glm::vec3 offsetPos = pos + arrowOffsetWorld;

		RenderUtils::RenderMesh(shader, textureID, ArrowModel, offsetPos, finalVecRot);
	}
}

void PhysicsObject::RenderArrowDecomposed(const Shader& shader, GLuint textureID, glm::vec3 dispVec) {
	RenderArrow(shader, textureID, glm::vec3(dispVec.x, 0, 0));
	RenderArrow(shader, textureID, glm::vec3(0, dispVec.y, 0));
	RenderArrow(shader, textureID, glm::vec3(0, 0, dispVec.z));
}

void PhysicsObject::RenderArrows(const Shader& shader, GLuint velocityTextureID, GLuint accelerationTextureID, bool decomposed) {
	if (!decomposed) {
		RenderArrowsComposed(shader, velocityTextureID, accelerationTextureID);
	}
	else {
		RenderArrowsDecomposed(shader, velocityTextureID, accelerationTextureID);
	}
}

// Renders the absolute velocity and acceleration
void PhysicsObject::RenderArrowsComposed(const Shader& shader, GLuint velocityTextureID, GLuint accelerationTextureID) {
	// Default arrow model direction (assumes +Z axis)
	RenderArrow(shader, velocityTextureID, velocity);

	RenderArrow(shader, accelerationTextureID, lastRecordedAcceleration);
}

// Renders the velocity and acceleration as its (x, y, z) components
void PhysicsObject::RenderArrowsDecomposed(const Shader& shader, GLuint velocityTextureID, GLuint accelerationTextureID) {
	RenderArrowDecomposed(shader, velocityTextureID, velocity);
	RenderArrowDecomposed(shader, accelerationTextureID, lastRecordedAcceleration);
}