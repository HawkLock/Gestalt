#pragma once
#include "Mesh.h"

Mesh::Mesh(glm::vec3 initialPosition, float faceSize, std::vector<float> inputVertices)
{
	pos = initialPosition;
	faceLength = faceSize;
    edges = std::vector<glm::vec3>();

	// Generate the vertices for the cube
    verticesU = inputVertices;

    SetupMeshU();

    vertexCount = verticesU.size() / 5; // There are five components to each in the vector

}

Mesh::Mesh(glm::vec3 initialPosition, float faceSize, std::string &modelPath, std::string &texturePath)
{
    pos = initialPosition;
    faceLength = faceSize;

    // Generate the vertices for the cube

    vertices = std::vector<Vertex>();
    parseVertexData(modelPath, vertices, normals, indices, edges);

    extractEdgesFromIndices(indices, vertices, edges);


    SetupMesh();
    vertexCount = vertices.size(); // There are five components to each in the vector
    originalVertices = vertices;

    //std::cout << texturePath[texturePath.length() - 3] << std::endl;
    if (texturePath[texturePath.length() - 3] == 'p') {
        TextureUtils::GenerateTexture(texturePath, texture, true);
    }
    else {
        TextureUtils::GenerateTexture(texturePath, texture, false);

    }

}

void Mesh::SetupMeshU()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesU.size() * sizeof(float), &verticesU[0], GL_STATIC_DRAW);

    // Assuming each vertex has 3 position floats and 2 texture floats
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Mesh::SetupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Bind VBO and upload vertex data (positions and UVs)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex) + normals.size() * sizeof(glm::vec3), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), normals.size() * sizeof(glm::vec3), normals.data());

    // Bind EBO and upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Setup vertex attributes
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
    glEnableVertexAttribArray(0);
    // UV attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
    glEnableVertexAttribArray(1);

    // Setup normal attributes
    // Assuming normals are stored after the vertices in the buffer
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

bool AreVectorsEqual(const glm::vec3& a, const glm::vec3& b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

std::vector<glm::vec3> RemoveDuplicates(const std::vector<glm::vec3>& input) {
    std::vector<glm::vec3> uniqueVec;

    for (const auto& vec : input) {
        bool isDuplicate = false;
        for (const auto& uniqueVecItem : uniqueVec) {
            if (AreVectorsEqual(vec, uniqueVecItem)) {
                isDuplicate = true;
                break;
            }
        }

        if (!isDuplicate) {
            uniqueVec.push_back(vec);
        }
    }

    return uniqueVec;
}

void Mesh::parseVertexData(const std::string& filepath, std::vector<Vertex>& vertices, std::vector<glm::vec3>& normals, std::vector<unsigned int>& indices, std::vector<glm::vec3>& edgeVectors) {
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return;
    }

    std::vector<glm::vec3> normalCache = std::vector<glm::vec3>();

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "edge") {
            // Read edge data
            glm::vec3 v1, v2;
            if (!(iss >> v1.x >> v1.y >> v1.z >> v2.x >> v2.y >> v2.z)) {
                std::cerr << "Error parsing edge line: " << line << std::endl;
                continue;
            }
            glm::vec3 edgeVector = v2 - v1;
            edgeVectors.push_back(edgeVector);
        }
        else {
            // Read vertex data
            unsigned int index;
            Vertex vertex;
            glm::vec3 normal = glm::vec3();
            if (!(iss >> index >> vertex.x >> vertex.y >> vertex.z >> vertex.u >> vertex.v >> normal.x >> normal.y >> normal.z)) {
                std::cerr << "Error parsing vertex line: " << line << std::endl;
                continue;
            }
            normalCache.push_back(normal);

            // Assuming that vertex and normal data are stored in the order they are read
            if (index >= vertices.size()) {
                vertices.resize(index + 1);
                normals.resize(index + 1);
            }

            vertices[index] = vertex;
            normals[index] = normal;
            indices.push_back(index);
        }
    }

    normals = RemoveDuplicates(normalCache);

    file.close();
}

void Mesh::ChangeSize(float scale)
{
    // Generate the vertices for the cube
    for (size_t i = 0; i < vertices.size(); i++) {
        vertices[i].x *= scale;     // Scale x
        vertices[i].y *= scale;   // Scale y
        vertices[i].z *= scale;   // Scale z
    }
    faceLength *= scale;

    SetupMesh();
}

void Mesh::ChangeSizeFromOriginal(float scale)
{
    // Generate the vertices for the cube
    for (size_t i = 0; i < vertices.size(); i++) {
        vertices[i].x = originalVertices[i].x * scale;   // Scale x
        vertices[i].y = originalVertices[i].y * scale;   // Scale y
        vertices[i].z = originalVertices[i].z * scale;   // Scale z
    }
    
    faceLength *= scale;

    SetupMesh();
}

void Mesh::ChangeSizeFromOriginalSingleDimension(float scale, char dimension)
{
    // Generate the vertices for the mesh
    for (size_t i = 0; i < vertices.size(); i++) {
        switch (dimension) {
        case 'x': 
            vertices[i].x = originalVertices[i].x * scale;
            break;

        case 'y': 
            vertices[i].y = originalVertices[i].y * scale;
            break;

        case 'z': 
            vertices[i].z = originalVertices[i].z * scale;
            break;

        default:
            std::cerr << "Error: Invalid dimension specified. Use 'x', 'y', or 'z'.\n";
            return;
        }
    }

    SetupMesh();
}


std::pair<unsigned int, unsigned int> Mesh::makeEdge(unsigned int v1, unsigned int v2) {
    if (v1 > v2) std::swap(v1, v2);
    return std::make_pair(v1, v2);
}

// Function to extract edges from indices
void Mesh::extractEdgesFromIndices( const std::vector<unsigned int>& indices, const std::vector<Vertex>& vertices, std::vector<glm::vec3>& edges
) {
    std::set<std::pair<unsigned int, unsigned int>> edgeSet;

    // Loop through indices in chunks of 3 (assuming triangles; adjust if necessary)
    for (size_t i = 0; i < indices.size(); i += 3) {
        unsigned int v0 = indices[i];
        unsigned int v1 = indices[i + 1];
        unsigned int v2 = indices[i + 2];

        // Create edges for each face (triangle) using the vertex indices
        std::vector<std::pair<unsigned int, unsigned int>> faceEdges = {
            makeEdge(v0, v1),
            makeEdge(v1, v2),
            makeEdge(v2, v0)
        };

        // Add edges to the set (automatically handles duplicates)
        for (const auto& edge : faceEdges) {
            edgeSet.insert(edge);
        }
    }

    // Convert edge set to vector of glm::vec3 for storing in edges
    for (const auto& edge : edgeSet) {
        unsigned int v1 = edge.first;
        unsigned int v2 = edge.second;
        glm::vec3 edgeVector(
            vertices[v2].x - vertices[v1].x,
            vertices[v2].y - vertices[v1].y,
            vertices[v2].z - vertices[v1].z
        );
        edges.push_back(edgeVector);
    }
}

std::vector<glm::vec3> Mesh::GetVertexPositions() {
    std::vector<glm::vec3> verts = std::vector<glm::vec3>();
    for (Vertex vert : vertices) {
        verts.push_back(glm::vec3(vert.x, vert.y, vert.z));
    }
    return verts;
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}