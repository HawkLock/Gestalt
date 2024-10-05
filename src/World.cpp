#include "World.h"

void PrintQuaternion(const glm::quat& q) {
	std::cout << "Quaternion: ("
		<< q.x << ", "
		<< q.y << ", "
		<< q.z << ", "
		<< q.w << ")"
		<< std::endl;
}


World::World()
{
	startTime = std::chrono::high_resolution_clock::now();
	window = renderer.GetWindow();

	//// Define the variables for object1
	//glm::vec3 position1 = glm::vec3(0.0f, -2.5f, 0.0f);
	////glm::quat rotation1 = glm::quat(glm::vec3(0.0f)); // Identity quaternion
	//glm::quat rotation1 = glm::angleAxis(glm::radians(-75.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//glm::vec3 velocity1 = glm::vec3(0.0f, 0.0f, 0.0f);
	//std::vector<glm::vec3> initialActingForces1 = std::vector<glm::vec3>();
	//float mass1 = 50.f;
	//float faceSize1 = 1.f;

	//std::string modelPath1 = "C:/Programming/Gestalt/Models/plane.txt";

	//// Create object1 and add it to the world
	//testObj1 = new PhysicsObject(position1, initialActingForces1, rotation1, mass1, true, faceSize1, modelPath1);
	//AddObject(testObj1);	

	// Define the variables for object1
	//glm::vec3 position1 = glm::vec3(0.0f, -2.5f, 0.0f);
	glm::vec3 position1 = glm::vec3(-2.5f, 0.f, 0.0f);
	glm::quat rotation1 = glm::angleAxis(glm::radians(-75.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	//glm::quat rotation1 = glm::quat(glm::vec3(0.0f)); // Identity quaternion
	std::vector<glm::vec3> initialActingForces1 = std::vector<glm::vec3>();
	float mass1 = 100.f;
	float faceSize1 = 2.f;

	std::string modelPath1 = "../Models/cube.txt";

	// Create object1 and add it to the world
	testObj1 = new PhysicsObject(position1, initialActingForces1, rotation1, mass1, false, faceSize1, modelPath1);
	AddObject(testObj1);

	// Define the variables for object2
	//glm::vec3 position2 = glm::vec3(0.f, 2.0f, 0.f);
	glm::vec3 position2 = glm::vec3(2.f, 0.0f, 0.f);
	//glm::quat rotation2 = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
	glm::quat rotation2 = glm::quat(glm::vec3(0.0f)); // Identity quaternion
	std::vector<glm::vec3> initialActingForces2 = std::vector<glm::vec3>();
	float mass2 = 100.0f;
	float faceSize2 = 2.0f;

	std::string modelPath2 = "../Models/cube.txt";

	// Create object2 and add it to the world
	//PhysicsObject* object2 = new PhysicsObject(position2, velocity2, initialActingForces2, rotationAxis2, angle2, mass2, gravity2, false, faceSize2, meshLibrary.getCubeVertices(faceSize2));
	testObj2 = new PhysicsObject(position2, initialActingForces2, rotation2, mass2, false, faceSize2, modelPath2);
	// testObj2->SetVelocity(glm::vec3(0.0f, -0.5f, 0.0f));
	 testObj2->SetVelocity(glm::vec3(-0.5f*2, 0.f, 0.0f));
	AddObject(testObj2);

	//testObj2->GetMesh().ChangeSize(0.5f);
	//testObj2->AddRotation(glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)));	
}

void World::Update()
{
	// Handles User Input
	ProcessInput();

	// Handles Rendering
	Render();

	// Physics Time Step
	while (renderer.timeAccumulator >= renderer.fixedTimeStep) {
		// Handles Collision
		CollisionUpdate();

		// Handles Other Physics
		PhysicsUpdate();
		renderer.timeAccumulator -= renderer.fixedTimeStep;
	}
}

bool AreParallel(const glm::vec3& v1, const glm::vec3& v2, float tolerance = 1e-6f) {
	// Compute the cross product
	glm::vec3 crossProd = glm::cross(v1, v2);
	// Check if the magnitude of the cross product is close to zero
	return glm::length(crossProd) < tolerance;
}

// Function to remove parallel vectors from the vector
void RemoveParallelVectors(std::vector<glm::vec3>& vectors, float tolerance = 1e-6f) {
	std::vector<glm::vec3> result;

	for (const auto& vec : vectors) {
		bool isParallel = false;
		for (const auto& resVec : result) {
			if (AreParallel(vec, resVec, tolerance)) {
				isParallel = true;
				break;
			}
		}
		if (!isParallel) {
			result.push_back(vec);
		}
	}

	vectors = std::move(result);
}

void ApplyObjectTransformation(std::vector<Vertex>& vertices, std::vector<glm::vec3>& normals, PhysicsObject* object) {
	// Get the object's position and rotation
	glm::vec3 objectPos = object->GetCurrentPos();
	glm::quat objectRotation = object->GetCurrentRot();

	// Transformation matrix
	glm::mat4 transformationMatrix = glm::mat4(1.0f);
	transformationMatrix = glm::translate(transformationMatrix, objectPos); // Apply translation
	transformationMatrix *= glm::mat4_cast(objectRotation); // Apply rotation

	// Apply transformation to vertices
	for (Vertex& vertex : vertices) {
		glm::vec4 transformedVertex = transformationMatrix * glm::vec4(vertex.x, vertex.y, vertex.z, 1.0f);
		vertex.x = transformedVertex.x;
		vertex.y = transformedVertex.y;
		vertex.z = transformedVertex.z;
	}

	// Apply rotation to normals (no translation)
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(transformationMatrix)));
	for (glm::vec3& normal : normals) {
		normal = glm::normalize(normalMatrix * normal);
	}
}

std::vector<glm::vec3> ConvertVertexToGLM(std::vector<Vertex> vertices) {
	std::vector<glm::vec3> newVertices = std::vector<glm::vec3>();
	for (Vertex vertex : vertices) {
		newVertices.push_back(glm::vec3(vertex.x, vertex.y, vertex.z));
	}
	return newVertices;
}

glm::vec3 axisFromQuaternion(const glm::quat& q) {
	// Ensure the quaternion is normalized
	glm::quat normalizedQ = glm::normalize(q);

	// The angle of rotation can be derived from the quaternion
	float angle = 2.0f * acos(normalizedQ.w); // w component gives the angle
	float sinHalfAngle = sqrt(1.0f - normalizedQ.w * normalizedQ.w);

	// Check if the angle is near zero to avoid division by zero
	if (fabs(sinHalfAngle) < 1e-6) {
		// The rotation is negligible, return a default axis (arbitrarily chosen)
		return glm::vec3(1.0f, 0.0f, 0.0f); // x-axis
	}

	// Calculate the axis of rotation
	glm::vec3 axis = glm::vec3(normalizedQ.x, normalizedQ.y, normalizedQ.z) / sinHalfAngle;

	return glm::normalize(axis); // Return the normalized axis of rotation
}

glm::vec3 calculatePerpendicularRadius(const glm::vec3& point, const glm::vec3& axisPoint, const glm::vec3& axis) {
	glm::vec3 r = point - axisPoint; // Position vector from axis point to the point

	// Check if the axis is a zero vector
	if (glm::length(axis) == 0.0f) {
		// Return a zero vector or handle appropriately
		return glm::vec3(0.0f);
	}

	glm::vec3 n = glm::normalize(axis); // Normalize the axis of rotation

	// Project r onto the axis
	glm::vec3 r_proj = glm::dot(r, n) * n;

	// Calculate the perpendicular component
	glm::vec3 r_perp = r - r_proj;

	return r_perp;
}

// Calculate combined linear and rotational velocity at a point
glm::vec3 calculateCompoundVelocity(glm::vec3 pos, glm::vec3 relativePoint, glm::vec3 velocity, glm::vec3 angularVelocity, glm::quat orientation) {
	glm::vec3 radiusPerpendicular = calculatePerpendicularRadius(relativePoint, pos, axisFromQuaternion(orientation));
	glm::vec3 rotationalVelocity = glm::cross(angularVelocity, radiusPerpendicular);
	return velocity + rotationalVelocity;
}

// Apply impulse to both linear and angular velocity of an object
void applyImpulse(PhysicsObject* obj, float impulse, glm::vec3 normal, glm::vec3 collisionPoint, glm::vec3 radius) {
	// Update linear velocity
	glm::vec3 newVelocity = obj->GetCurrentVelocity() + (impulse / obj->GetMass()) * normal;
	obj->SetVelocity(newVelocity);

	// Update angular velocity
	glm::vec3 torque = glm::cross(radius, impulse * normal);
	glm::vec3 newAngularVelocity = obj->GetCurrentAngularVelocity() + obj->GetInverseInertiaTensor() * torque;
	obj->SetAngularVelocity(newAngularVelocity);
}

// Calculate relative velocities considering both linear and angular components
glm::vec3 calculateRelativeVelocities(glm::vec3 velA, glm::vec3 velB, glm::vec3 angVelA, glm::vec3 angVelB, glm::vec3 radiusA, glm::vec3 radiusB) {
	return velB + glm::cross(angVelB, radiusB) - (velA + glm::cross(angVelA, radiusA));
}

void applyImpulse(PhysicsObject* obj, glm::vec3 force, glm::vec3 rel) {
	glm::vec3 newVelocity = obj->GetCurrentVelocity() + force / obj->GetMass();
	obj->SetVelocity(newVelocity);

	glm::vec3 newAngularVelocity = obj->GetCurrentAngularVelocity() + obj->GetInverseInertiaTensor() * glm::cross(rel, force);
	obj->SetAngularVelocity(newAngularVelocity);
}

void World::resolveImpulses(PhysicsObject * objA, PhysicsObject * objB, const Overlap & overlap) {
	glm::vec3 posA = objA->GetCurrentPos();
	glm::vec3 posB = objB->GetCurrentPos();
	glm::quat rotA = objA->GetCurrentRot();
	glm::quat rotB = objB->GetCurrentRot();
	glm::vec3 velA = objA->GetCurrentVelocity();
	glm::vec3 velB = objB->GetCurrentVelocity();
	glm::vec3 angVelA = objA->GetCurrentAngularVelocity();
	glm::vec3 angVelB = objB->GetCurrentAngularVelocity();
	float massA = objA->GetMass();
	float massB = objB->GetMass();
	glm::mat3 IITA = objA->GetInverseInertiaTensor();
	glm::mat3 IITB = objB->GetInverseInertiaTensor();
	glm::vec3 collisionNormal = overlap.axis;
	glm::vec3 collisionPoint = overlap.collisionPoint;

	// Compute the velocities at the point of collision, including rotational components
	glm::vec3 velocityA = calculateCompoundVelocity(posA, collisionPoint, velA, angVelA, rotA);
	glm::vec3 velocityB = calculateCompoundVelocity(posB, collisionPoint, velB, angVelB, rotB);
	//glm::vec3 relativeVelocity = velocityA - velocityB;
	glm::vec3 relativeVelocity = velocityA - velocityB;


	// Compute the radii from the collision points to the center of masses
	glm::vec3 collisionRadiusA = collisionPoint - posA;
	glm::vec3 collisionRadiusB = collisionPoint - posB;

	// Impulse calculation (courtesy of Chris Hecker's formula)
	float numerator = -(1 + restitution) * glm::dot(relativeVelocity, collisionNormal);

	// Angular impulse components
	glm::vec3 radiusCrossNormalA = glm::cross(collisionRadiusA, collisionNormal);
	glm::vec3 radiusCrossNormalB = glm::cross(collisionRadiusB, collisionNormal);
	glm::vec3 denominator1Vec = IITA * radiusCrossNormalA;
	float denominator1 = glm::dot(radiusCrossNormalA, denominator1Vec);

	glm::vec3 denominator2Vec = IITB * radiusCrossNormalB;
	float denominator2 = glm::dot(radiusCrossNormalB, denominator2Vec);

	// Combine linear and angular contributions to compute total impulse
	float denominator = (1 / massA + 1 / massB) + denominator1 + denominator2;
	float impulse = numerator / denominator;



	// Apply the impulse to both objects
	//applyImpulse(objA, impulse, collisionNormal, collisionPoint, collisionRadiusA);
	//applyImpulse(objB, -impulse, collisionNormal, collisionPoint, collisionRadiusB);

	glm::vec3 impulseForce = collisionNormal * impulse;

	applyImpulse(objA, impulseForce, collisionRadiusA);
	applyImpulse(objB, -impulseForce, collisionRadiusB);
}

// Resolve overlap between two colliding objects
void World::resolveOverlap(PhysicsObject* objA, PhysicsObject* objB, const Overlap& overlap) {
	// Get positions of the objects
	glm::vec3 posA = objA->GetCurrentPos();
	glm::vec3 posB = objB->GetCurrentPos();

	// Check if either object is anchored
	bool isAnchoredA = objA->IsAnchored();
	bool isAnchoredB = objB->IsAnchored();

	// Calculate separation vector based on the overlap depth and the collision normal
	glm::vec3 separation = overlap.axis * overlap.depth;

	// Resolve overlap based on anchor status
	if (isAnchoredA && !isAnchoredB) {
		// Move only objB away from objA
		objB->SetPos(posB + separation);
	}
	else if (!isAnchoredA && isAnchoredB) {
		// Move only objA away from objB
		objA->SetPos(posA - separation);
	}
	else if (!isAnchoredA && !isAnchoredB) {
		// Move both objects apart equally
		objA->SetPos(posA - separation * 0.5f);
		objB->SetPos(posB + separation * 0.5f);
	}
	// If both are anchored, do nothing, as neither can move
}

// Resolve collision between two objects using impulse-based collision resolution
void World::resolveCollision(PhysicsObject* objA, PhysicsObject* objB, const Overlap& overlap) {
	resolveOverlap(objA, objB, overlap);
	resolveImpulses(objA, objB, overlap);
}


//void World::CollisionUpdate() {
//	for (int i = 0; i < PhysicObjects.size(); i++) {
//		for (int z = i + 1; z < PhysicObjects.size(); z++) {
//			// Extract vertices and normals from the meshes
//			std::vector<Vertex> obj1Vertices = PhysicObjects[i]->GetMesh().vertices;
//			std::vector<Vertex> obj2Vertices = PhysicObjects[z]->GetMesh().vertices;
//			std::vector<glm::vec3> normals1 = PhysicObjects[i]->GetMesh().normals;
//			std::vector<glm::vec3> normals2 = PhysicObjects[z]->GetMesh().normals;
//
//			ApplyObjectTransformation(obj1Vertices, normals1, PhysicObjects[i]);
//			ApplyObjectTransformation(obj2Vertices, normals2, PhysicObjects[z]);
//
//			std::vector<glm::vec3> axes;
//
//			// Add normals as axes
//			for (const auto& normal : normals1) {
//				axes.push_back(normal);
//			}
//
//			for (const auto& normal : normals2) {
//				axes.push_back(normal);
//			}
//
//			generateSeparationAxes(axes, PhysicObjects[i]->GetMesh().edges, PhysicObjects[z]->GetMesh().edges);
//			RemoveParallelVectors(axes);
//
//			//int i = 0;
//			//std::cout << "Axes: " << axes.size() << std::endl;
//			//for (glm::vec3 axis : axes) {
//			//	i++;
//			//    std::cout << "Axis: " << i << ": " << axis.x << ", " << axis.y << ", " << axis.z << std::endl;
//			//}
//			//std::cout << std::endl;
//
//			Overlap smallestOverlap = Overlap();
//			bool result = checkSATCollision(obj1Vertices, obj2Vertices, axes, smallestOverlap);
//			if (result) {
//				// Collision reaction
//				std::chrono::high_resolution_clock::time_point currTime = std::chrono::high_resolution_clock::now();
//				std::chrono::duration<double> elapsed = currTime - startTime;
//
//				//std::cout << "Time: " << elapsed.count() << " -- Collision Depth: " << smallestOverlap.depth << std::endl;
//				std::cout << "Time: " << elapsed.count() << " -- Collision Depth: " << smallestOverlap.depth << " -- Axis: " << smallestOverlap.axis.x << ", " << smallestOverlap.axis.y << ", " << smallestOverlap.axis.z << std::endl;
//
//				resolveCollision(PhysicObjects[i], PhysicObjects[z], smallestOverlap);
//				// PhysicObjects[i]->SetPos(PhysicObjects[i]->GetCurrentPos() + smallestOverlap.axis * smallestOverlap.depth);
//			}
//		}
//	}
//}

static bool AreObjectsAligned(const glm::quat& orientation1, const glm::quat& orientation2, const glm::vec3& position1, const glm::vec3& position2, float tolerance = 0.001f) {
	// Check if orientations are the same
	glm::quat relativeRotation = glm::inverse(orientation1) * orientation2;
	glm::quat identityQuat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	bool orientationsAligned = glm::length(relativeRotation - identityQuat) < tolerance;

	// Check if positions are aligned perfectly along one axis
	glm::vec3 positionDiff = position2 - position1;

	// Check alignment on x-axis (y and z components should be nearly zero)
	bool alignedOnX = glm::abs(positionDiff.y) < tolerance && glm::abs(positionDiff.z) < tolerance;

	// Check alignment on y-axis (x and z components should be nearly zero)
	bool alignedOnY = glm::abs(positionDiff.x) < tolerance && glm::abs(positionDiff.z) < tolerance;

	// Check alignment on z-axis (x and y components should be nearly zero)
	bool alignedOnZ = glm::abs(positionDiff.x) < tolerance && glm::abs(positionDiff.y) < tolerance;

	// Return true if orientations are aligned and positions are aligned on any one axis
	return orientationsAligned && (alignedOnX || alignedOnY || alignedOnZ);
}


static bool AABBIntersect(const ConvexShape& shape1, const ConvexShape& shape2, Overlap& overlap) {

	glm::vec3 min1 = shape1.GetMinBounds(); // Get AABB min bounds for shape1
	glm::vec3 max1 = shape1.GetMaxBounds(); // Get AABB max bounds for shape1
	glm::vec3 min2 = shape2.GetMinBounds(); // Get AABB min bounds for shape2
	glm::vec3 max2 = shape2.GetMaxBounds(); // Get AABB max bounds for shape2


	// Check for degenerate AABB cases
	if (glm::all(glm::equal(min1, max1)) || glm::all(glm::equal(min2, max2))) {
		std::cerr << "Warning: Degenerate AABB detected!" << std::endl;
		return false;
	}

	// Check for intersection along each axis
	glm::vec3 overlapDepth(0.0f);  // Stores overlap depth in x, y, z

	// X-axis overlap check
	if (min1.x > max2.x || max1.x < min2.x) return false; // No overlap on x-axis
	overlapDepth.x = std::min(max1.x - min2.x, max2.x - min1.x);  // Calculate x overlap

	// Y-axis overlap check
	if (min1.y > max2.y || max1.y < min2.y) return false; // No overlap on y-axis
	overlapDepth.y = std::min(max1.y - min2.y, max2.y - min1.y);  // Calculate y overlap

	// Z-axis overlap check
	if (min1.z > max2.z || max1.z < min2.z) return false; // No overlap on z-axis
	overlapDepth.z = std::min(max1.z - min2.z, max2.z - min1.z);  // Calculate z overlap

	// Sanity check for negative overlap values
	if (overlapDepth.x < 0 || overlapDepth.y < 0 || overlapDepth.z < 0) {
		std::cerr << "Error: Negative overlap depth detected!" << std::endl;
		return false;
	}

	// Determine the smallest overlap axis (for separating axis)
	if (overlapDepth.x < overlapDepth.y && overlapDepth.x < overlapDepth.z) {
		overlap.axis = glm::vec3(1, 0, 0);  // X-axis is the separating axis
		overlap.depth = overlapDepth.x;
	}
	else if (overlapDepth.y < overlapDepth.z) {
		overlap.axis = glm::vec3(0, 1, 0);  // Y-axis is the separating axis
		overlap.depth = overlapDepth.y;
	}
	else {
		overlap.axis = glm::vec3(0, 0, 1);  // Z-axis is the separating axis
		overlap.depth = overlapDepth.z;
	}

	// Compute the collision point as the midpoint of the overlapping region
	glm::vec3 center1 = (min1 + max1) * 0.5f;  // Center of shape1 AABB
	glm::vec3 center2 = (min2 + max2) * 0.5f;  // Center of shape2 AABB
	overlap.collisionPoint = (center1 + center2) * 0.5f;  // Midpoint of overlap

	return true;  // AABB intersected
}

void World::CollisionUpdate() {
	for (int i = 0; i < PhysicObjects.size(); i++) {
		for (int z = i + 1; z < PhysicObjects.size(); z++) {
			// Extract vertices and normals from the meshes
			std::vector<Vertex> obj1Vertices = PhysicObjects[i]->GetMesh().vertices;
			std::vector<Vertex> obj2Vertices = PhysicObjects[z]->GetMesh().vertices;
			std::vector<glm::vec3> normals1 = PhysicObjects[i]->GetMesh().normals;
			std::vector<glm::vec3> normals2 = PhysicObjects[z]->GetMesh().normals;

			glm::vec3 pos1 = PhysicObjects[i]->GetCurrentPos();
			glm::vec3 pos2 = PhysicObjects[z]->GetCurrentPos();
			glm::quat orientation1 = PhysicObjects[i]->GetCurrentRot();
			glm::quat orientation2 = PhysicObjects[z]->GetCurrentRot();

			ApplyObjectTransformation(obj1Vertices, normals1, PhysicObjects[i]);
			ApplyObjectTransformation(obj2Vertices, normals2, PhysicObjects[z]);


			std::vector<glm::vec3> vertices1 = ConvertVertexToGLM(obj1Vertices);
			std::vector<glm::vec3> vertices2 = ConvertVertexToGLM(obj2Vertices);

			ConvexShape shape1(vertices1, orientation1, pos1);
			ConvexShape shape2(vertices2, orientation2, pos2);

			// Perform GJK collision detection and overlap calculation
			Overlap smallestOverlap;
			Simplex simplex;
			bool isColliding = false;
			bool degenrateCase = false;
			// Degenerate case checking (collinearity) 
			degenrateCase = AreObjectsAligned(shape1.GetOrientation(), shape2.GetOrientation(), shape1.GetPos(), shape2.GetPos());
			if (degenrateCase) {
				isColliding = AABBIntersect(shape1, shape2, smallestOverlap);
			}
			else {
				isColliding = GJKAlgorithm::GJK(shape1, shape2, smallestOverlap, simplex);
			}

			if (isColliding) {
				// Collision response
				if (!degenrateCase) {
					smallestOverlap = EPAAlgorithm::EPA(simplex, shape1, shape2);
				}

				std::chrono::high_resolution_clock::time_point currTime = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> elapsed = currTime - startTime;

				/*std::cout << "Time: " << elapsed.count() << " -- Collision Depth: " << smallestOverlap.depth
					<< " -- Axis: " << smallestOverlap.axis.x << ", " << smallestOverlap.axis.y << ", " << smallestOverlap.axis.z 
					<< "-- POC: " << smallestOverlap.collisionPoint.x << ", " << smallestOverlap.collisionPoint.y << ", " << smallestOverlap.collisionPoint.z << std::endl;*/

				resolveCollision(PhysicObjects[i], PhysicObjects[z], smallestOverlap);
			}
		}
	}
}

// Helper function to create a unique axis identifier
std::string createAxisKey(const glm::vec3& axis) {
	const float epsilon = 1e-6f;
	glm::vec3 normAxis = normalize(axis);
	return std::to_string(static_cast<int>(normAxis.x / epsilon)) + "," +
		std::to_string(static_cast<int>(normAxis.y / epsilon)) + "," +
		std::to_string(static_cast<int>(normAxis.z / epsilon));
}

void World::generateSeparationAxes(std::vector<glm::vec3>& axes, std::vector<glm::vec3>& edges1, std::vector<glm::vec3>& edges2) {
	std::set<std::string> uniqueAxes; // To store unique axis keys

	for (const glm::vec3& edge1 : edges1) {
		for (const glm::vec3& edge2 : edges2) {
			// Compute the cross product to generate a separating axis
			glm::vec3 axis = glm::cross(edge1, edge2);

			// Normalize the axis and ensure it's not a zero vector
			if (glm::length(axis) > 0.0f) {
				axis = normalize(axis);

				// Create a unique key for the axis
				std::string axisKey = createAxisKey(axis);

				// Check if this axis is unique
				if (uniqueAxes.find(axisKey) == uniqueAxes.end()) {
					uniqueAxes.insert(axisKey);
					axes.push_back(axis);
				}
			}
		}
	}
}

std::pair<float, float> World::getProjectionRange(const std::vector<Vertex>& vertices, glm::vec3& axis) {
	// Initialize min and max projection with the first vertex projection
	float minProj = glm::dot(glm::vec3(vertices[0].x, vertices[0].y, vertices[0].z), axis);
	float maxProj = minProj;

	// Loop through all vertices to find the min and max projection
	for (const auto& vertex : vertices) {
		float proj = glm::dot(glm::vec3(vertex.x, vertex.y, vertex.z), axis);
		if (proj < minProj) minProj = proj;
		if (proj > maxProj) maxProj = proj;
	}

	// std::cout << "Min: " << minProj << ", Max: " << maxProj << std::endl;

	return { minProj, maxProj };
}

bool World::intervalsOverlap(const std::pair<float, float>& range1, const std::pair<float, float>& range2) {
	return range1.second >= range2.first && range2.second >= range1.first;
}

float World::getOverlap(const std::pair<float, float>& range1, const std::pair<float, float>& range2) {
	// Calculate the overlap depth
	float overlapStart = std::max(range1.first, range2.first);
	float overlapEnd = std::min(range1.second, range2.second);

	// If there's an overlap, return the overlap depth
	if (overlapStart <= overlapEnd) {
		return overlapEnd - overlapStart;
	}

	// If there's no overlap, return 0
	return 0.0f;
}


bool World::checkSATCollision(const std::vector<Vertex>& vertices1, const std::vector<Vertex>& vertices2, std::vector<glm::vec3>& axes, Overlap& smallestOverlap) {
	float smallestOverlapDepth = INT_MAX;
	glm::vec3 smallestOverlapAxis = glm::vec3();
	for (glm::vec3 axis : axes) {
		std::pair<float, float> projection1 = getProjectionRange(vertices1, axis);
		std::pair<float, float> projection2 = getProjectionRange(vertices2, axis);

		if (!intervalsOverlap(projection1, projection2)) {
			return false;
		}
		else {
			float overlap = getOverlap(projection1, projection2);

			if (overlap < smallestOverlapDepth) {
				smallestOverlapDepth = overlap;
				smallestOverlapAxis = axis;
			}
		}
	}
	smallestOverlap.depth = smallestOverlapDepth;
	smallestOverlap.axis = smallestOverlapAxis;
	return true;
}

// Uses elastic collision calculation
std::vector<glm::vec3> World::CalculateCollisionVelocity(PhysicsObject one, PhysicsObject two) {

	std::vector<glm::vec3> velocities;
	glm::vec3 sumMomentum = one.GetMass() * one.GetCurrentVelocity() + two.GetMass() * two.GetCurrentVelocity();
	float sumMass = one.GetMass() + two.GetMass();
	glm::vec3 initialMomentumOne = one.GetMass() * one.GetCurrentVelocity();
	glm::vec3 velocityOne = (two.GetMass() * (two.GetCurrentVelocity() - one.GetCurrentVelocity()) + sumMomentum) / sumMass;
	glm::vec3 velocityTwo = (one.GetMass() * (one.GetCurrentVelocity() - two.GetCurrentVelocity()) + sumMomentum) / sumMass;
	glm::vec3 finalMomentumOne = one.GetMass() * velocityOne;

	glm::vec3 deltaVelocityOne = velocityOne - one.GetCurrentVelocity();
	glm::vec3 deltaVelocityTwo = velocityTwo - two.GetCurrentVelocity();

	glm::vec3 deltaMomentum = finalMomentumOne - initialMomentumOne;
	glm::vec3 normalForceOne = deltaMomentum;
	glm::vec3 normalForceTwo = normalForceOne * -1.f;

	velocities.push_back(deltaVelocityOne + normalForceOne);
	velocities.push_back(deltaVelocityTwo + normalForceTwo);
	return velocities;
}

// SAT Algorithm Implementation WIP
void World::CheckCollision(PhysicsObject& one, PhysicsObject& two, bool& result, glm::vec3& shortestOverlap)
{
	std::vector<glm::vec3> oneVertices = one.GetVertices();
	std::vector<glm::vec3> twoVertices = two.GetVertices();

	//std::cout << glm::length(shortestOverlap) << std::endl;
	std::vector<glm::vec3> oneCollisionVertices;
	std::vector<glm::vec3> twoCollisionVertices;

	for (unsigned int i = 0; i < oneVertices.size(); i++)
	{
		glm::vec3 vA = oneVertices[i];
		glm::vec3 vB = oneVertices[(i + 1) % oneVertices.size()];

		glm::vec3 edge = vB - vA;
		// the normal of the edge
		glm::vec3 axis(-edge.y, edge.x, edge.z);


		// Goes {min, max}
		std::vector<float> extentOneValues = ProjectVertices(oneVertices, axis);
		std::vector<float> extentTwoValues = ProjectVertices(twoVertices, axis);

		if (extentOneValues[0] > extentTwoValues[1] || extentTwoValues[0] > extentOneValues[1]) {
			result = false;
			return;
		}

		/*
		float overlapOne = extentOneValues[0] - extentTwoValues[1];
		float overlapTwo = extentTwoValues[0] - extentOneValues[1];
		*/
		float overlapOne = extentOneValues[0] - extentTwoValues[0];
		float overlapTwo = extentTwoValues[1] - extentOneValues[1];
		// Determines which is the overlapping part
		if (extentOneValues[0] - extentTwoValues[1] < 0) {
			// Min of #1 is less than max of #2
			if (glm::length(shortestOverlap) > overlapOne) {
				shortestOverlap = axis * overlapOne;
			}
		}
		else {
			// Min of #2 is less than max of #1
			if (glm::length(shortestOverlap) > overlapTwo) {
				shortestOverlap = axis * overlapTwo;
			}
		}
		//printf("Vertex: %f, %f, %f \n", oneVertices[i].x, oneVertices[i].y, oneVertices[i].z);
		oneCollisionVertices.push_back(oneVertices[i]);
	}

	for (unsigned int i = 0; i < twoVertices.size(); i++)
	{
		glm::vec3 vA = twoVertices[i];
		glm::vec3 vB = twoVertices[(i + 1) % twoVertices.size()];

		glm::vec3 edge = vB - vA;
		// the normal of the edge
		glm::vec3 axis(-edge.y, edge.x, edge.z);


		// Goes {min, max}
		std::vector<float> extentOneValues = ProjectVertices(oneVertices, axis);
		std::vector<float> extentTwoValues = ProjectVertices(twoVertices, axis);

		if (extentOneValues[0] > extentTwoValues[1] || extentTwoValues[0] > extentOneValues[1]) {
			result = false;
			return;
		}

		float overlapOne = extentTwoValues[1] - extentOneValues[0];
		float overlapTwo = extentOneValues[1] - extentTwoValues[0];

		// Determines which is the overlapping part
		if (extentOneValues[0] - extentTwoValues[1] < 0) {
			// Min of #1 is less than max of #2
			if (glm::length(shortestOverlap) > overlapOne) {
				shortestOverlap = axis * overlapOne;
			}
		}
		else {
			// Min of #2 is less than max of #1
			if (glm::length(shortestOverlap) > overlapTwo) {
				shortestOverlap = axis * overlapTwo;
			}
		}
		twoCollisionVertices.push_back(twoVertices[i]);
	}

	std::vector<PhysicsUtility::Edge> oneCollisionEdges;
	std::vector<PhysicsUtility::Edge> twoCollisionEdges;
	for (int i = 1; i < oneCollisionVertices.size(); i += 2) {
		oneCollisionEdges.push_back(PhysicsUtility::Edge(oneCollisionVertices[i - 1], oneCollisionVertices[i]));
	}
	for (int i = 1; i < twoCollisionVertices.size(); i += 2) {
		twoCollisionEdges.push_back(PhysicsUtility::Edge(twoCollisionVertices[i - 1], twoCollisionVertices[i]));
	}
	/*
	printf("One Vertex Count: %d\n", oneCollisionVertices.size());
	printf("One Edge Count: %d\n", oneCollisionEdges.size());
	printf("Two Vertex Count: %d\n", twoCollisionVertices.size());
	printf("Two Vertex Count: %d\n", twoCollisionEdges.size());
	*/

	//std::cout << "ShortestOverlap: " << glm::length(shortestOverlap) << std::endl;

	// If there are no gaps between the objects
	result = true;
}

std::vector<float> World::ProjectVertices(std::vector<glm::vec3> vertices, glm::vec3 axis)
{
	float min = FLT_MAX;
	float max = -min;

	for (int i = 0; i < vertices.size(); i++)
	{
		glm::vec3 v = vertices[i];
		float proj = glm::dot(v, axis);

		if (proj < min) {
			min = proj;
		}
		if (proj > max) {
			max = proj;
		}
	}

	return { min, max };
}

void World::PhysicsUpdate()
{
	// Handles Physics
	for (int i = 0; i < PhysicObjects.size(); i++)
	{
		if (!PhysicObjects[i]->IsAnchored()) {
			//std::cout << "Object " << i << " velocity: " << PhysicsUtility::vec3ToString(PhysicObjects[i].GetCurrentVelocity()) << " units/unit" << std::endl;
			PhysicObjects[i]->CalculatePhysics(renderer.fixedTimeStep);
		}
	}
}

void World::Render()
{
	renderer.RenderLoop(&camera, PhysicObjects);
}

void World::AddObject(PhysicsObject* object)
{
	PhysicObjects.push_back(object);
}

void World::ProcessInput()
{
	float cameraSpeed = 7.5f * renderer.deltaTime;
	float boxSpeed = 1.5f * renderer.deltaTime;
	float rotSpeed = 25.f * renderer.deltaTime;

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		PhysicObjects[1]->SetPos(glm::vec3(2.f, 0.0f, 0.f));
	}

	// WASD Movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.cameraPos += cameraSpeed * camera.cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.cameraPos -= cameraSpeed * camera.cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.cameraPos -= glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.cameraPos += glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
	}

	// Altitude Movement
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		camera.cameraPos -= cameraSpeed * glm::vec3(0, 1, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		camera.cameraPos += cameraSpeed * glm::vec3(0, 1, 0);
	}

	// Program Controls
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	// Debuging Controls
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		testObj2->SetPos(testObj2->GetCurrentPos() + boxSpeed * glm::vec3(-1, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		testObj2->SetPos(testObj2->GetCurrentPos() + boxSpeed * glm::vec3(1, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		testObj2->SetPos(testObj2->GetCurrentPos() + boxSpeed * glm::vec3(0, 0, -1));
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		testObj2->SetPos(testObj2->GetCurrentPos() + boxSpeed * glm::vec3(0, 0, 1));
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		testObj2->SetPos(testObj2->GetCurrentPos() + boxSpeed * glm::vec3(0, -1, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		testObj2->SetPos(testObj2->GetCurrentPos() + boxSpeed * glm::vec3(0, 1, 0));
	}

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
	{
		glm::quat rot = glm::angleAxis(glm::radians(rotSpeed), glm::vec3(0.0f, 0.0f, 1.0f));
		testObj2->AddRotation(rot);
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	{
		glm::quat rot = glm::angleAxis(glm::radians(-rotSpeed), glm::vec3(0.0f, 0.0f, 1.0f));
		testObj2->AddRotation(rot);
	}

	// Untrap Cursor with Spacebar
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		// Toggle cursor mode
		cursorCaptured = !cursorCaptured;
		if (cursorCaptured)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		}
	}

}

void World::ProcessMouseMovement(double xpos, double ypos)
{
	camera.ProcessMouseMovement(xpos, ypos);
}

void World::ProcessMouseScroll(float yoffset)
{
	camera.ProcessMouseScroll(yoffset, renderer.deltaTime);
}