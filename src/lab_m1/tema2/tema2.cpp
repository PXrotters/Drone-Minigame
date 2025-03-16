#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <cmath>

using namespace std;
using namespace m1;


/*
 *  To find out more about FrameStart, Update, FrameEnd
 *  and the order in which they are called, see world.cpp.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}

Mesh* Tema2::GenerateTerrainMesh(int width, int height) {
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    float cellSize = 1.0f; // Dimensiunea unei celule

    // Calculam offseturile pentru a centra gridul
    float xOffset = -(width * cellSize) / 2.0f;
    float zOffset = -(height * cellSize) / 2.0f;

    // Generam vertecsii
    for (int row = 0; row <= height; ++row) {
        for (int col = 0; col <= width; ++col) {
            vertices.push_back(VertexFormat(
                glm::vec3(col * cellSize + xOffset, 0.0f, row * cellSize + zOffset), // Centrare
                glm::vec3(0.0f, 1.0f, 0.0f),                                        // Normala
                glm::vec3(0.5f, 0.5f, 0.5f)                                         // Culoare default
            ));
        }
    }

    // Generam indicii
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            int topLeft = row * (width + 1) + col;
            int topRight = topLeft + 1;
            int bottomLeft = topLeft + (width + 1);
            int bottomRight = bottomLeft + 1;

            // Primul triunghi
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Al doilea triunghi
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    // Cream si returnam mesh-ul
    Mesh* terrainMesh = new Mesh("terrain");
    terrainMesh->InitFromData(vertices, indices);
    return terrainMesh;
}

Mesh* Tema2::GenerateCylinder(float radius, float height, int segments) {
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    // Cream vertecsii pentru capacele cilindrului
    for (int i = 0; i < segments; i++) {
        float angle = glm::radians(360.0f * i / segments);
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        // Adaugam punctele pentru cercul superior si cel inferior
        vertices.push_back(VertexFormat(glm::vec3(x, height / 2.0f, z), glm::vec3(0, 1, 0))); // Cerc superior
        vertices.push_back(VertexFormat(glm::vec3(x, -height / 2.0f, z), glm::vec3(0, -1, 0))); // Cerc inferior
    }

    // Adaugam centrele cercurilor
    vertices.push_back(VertexFormat(glm::vec3(0, height / 2.0f, 0), glm::vec3(0, 1, 0))); // Centru cerc superior
    vertices.push_back(VertexFormat(glm::vec3(0, -height / 2.0f, 0), glm::vec3(0, -1, 0))); // Centru cerc inferior

    int topCenterIndex = segments * 2;
    int bottomCenterIndex = segments * 2 + 1;

    // Generam indicii pentru capace
    for (int i = 0; i < segments; i++) {
        int next = (i + 1) % segments;

        // Capac superior
        indices.push_back(topCenterIndex);
        indices.push_back(i * 2);
        indices.push_back(next * 2);

        // Capac inferior
        indices.push_back(bottomCenterIndex);
        indices.push_back(next * 2 + 1);
        indices.push_back(i * 2 + 1);
    }

    // Generam indicii pentru fetele laterale
    for (int i = 0; i < segments; i++) {
        int next = (i + 1) % segments;

        // Doua triunghiuri pentru fiecare fata laterala
        indices.push_back(i * 2);
        indices.push_back(next * 2);
        indices.push_back(i * 2 + 1);

        indices.push_back(i * 2 + 1);
        indices.push_back(next * 2);
        indices.push_back(next * 2 + 1);
    }

    // Cream si returnam mesh-ul
    Mesh* cylinderMesh = new Mesh("cylinder");
    cylinderMesh->InitFromData(vertices, indices);
    return cylinderMesh;
}

Mesh* Tema2::GenerateCone(float radius, float height, int segments) {
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    // Cream vertecsii pentru baza
    for (int i = 0; i < segments; i++) {
        float angle = glm::radians(360.0f * i / segments);  // Unghiul e calculat a.i. sa parcurca 360 de grade / segments parti
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        vertices.push_back(VertexFormat(glm::vec3(x, 0.0f, z), glm::vec3(0, -1, 0))); // Baza
    }

    // Adaugam centrul bazei
    vertices.push_back(VertexFormat(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, -1, 0)));

    // Adaugam varful conului
    vertices.push_back(VertexFormat(glm::vec3(0.0f, height, 0.0f), glm::vec3(0, 1, 0)));

    int centerIndex = segments;
    int tipIndex = segments + 1;

    // Generam indicii pentru baza
    for (int i = 0; i < segments; i++) {
        int next = (i + 1) % segments;

        indices.push_back(centerIndex);
        indices.push_back(next);
        indices.push_back(i);
    }

    // Generam indicii pentru fetele laterale
    for (int i = 0; i < segments; i++) {
        int next = (i + 1) % segments;

        indices.push_back(i);
        indices.push_back(next);
        indices.push_back(tipIndex);
    }

    // Cream si returnam mesh-ul
    Mesh* coneMesh = new Mesh("cone");
    coneMesh->InitFromData(vertices, indices);
    return coneMesh;
}

Mesh* Tema2::GeneratePrism(float baseLength, float height) {
    vector<VertexFormat> vertices = {
        // Baza piramidei (patrat)
        VertexFormat(glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(1, 0, 0)), // Stanga jos
        VertexFormat(glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(1, 0, 0)),  // Dreapta jos
        VertexFormat(glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(1, 0, 0)),   // Dreapta sus
        VertexFormat(glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec3(1, 0, 0)),  // Stanga sus

        // Varful piramidei
        VertexFormat(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1, 0, 0)),   // Varf
    };

    vector<unsigned int> indices = {
        // Baza (patratul)
        0, 1, 2,
        0, 2, 3,

        // Fetele laterale (triunghiuri)
        0, 1, 4,  // Fata 1
        1, 2, 4,  // Fata 2
        2, 3, 4,  // Fata 3
        3, 0, 4,  // Fata 4
    };

    Mesh* prism = new Mesh("prism");
    prism->InitFromData(vertices, indices);
    return prism;
}

Mesh* Tema2::GenerateGate(float width, float height, float thickness) {
    vector<VertexFormat> vertices = {
        // Stalpul din stanga (cub)
        VertexFormat(glm::vec3(-width / 2 - thickness / 2, 0, -thickness / 2), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(-width / 2 + thickness / 2, 0, -thickness / 2), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(-width / 2 + thickness / 2, height, -thickness / 2), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(-width / 2 - thickness / 2, height, -thickness / 2), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(-width / 2 - thickness / 2, 0, thickness / 2), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(-width / 2 + thickness / 2, 0, thickness / 2), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(-width / 2 + thickness / 2, height, thickness / 2), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(-width / 2 - thickness / 2, height, thickness / 2), glm::vec3(1, 0, 0)),

        // Stalpul din dreapta (cub)
        VertexFormat(glm::vec3(width / 2 - thickness / 2, 0, -thickness / 2), glm::vec3(0, 1, 0)),
        VertexFormat(glm::vec3(width / 2 + thickness / 2, 0, -thickness / 2), glm::vec3(0, 1, 0)),
        VertexFormat(glm::vec3(width / 2 + thickness / 2, height, -thickness / 2), glm::vec3(0, 1, 0)),
        VertexFormat(glm::vec3(width / 2 - thickness / 2, height, -thickness / 2), glm::vec3(0, 1, 0)),
        VertexFormat(glm::vec3(width / 2 - thickness / 2, 0, thickness / 2), glm::vec3(0, 1, 0)),
        VertexFormat(glm::vec3(width / 2 + thickness / 2, 0, thickness / 2), glm::vec3(0, 1, 0)),
        VertexFormat(glm::vec3(width / 2 + thickness / 2, height, thickness / 2), glm::vec3(0, 1, 0)),
        VertexFormat(glm::vec3(width / 2 - thickness / 2, height, thickness / 2), glm::vec3(0, 1, 0)),

        // Bara de sus (cub)
        VertexFormat(glm::vec3(-width / 2 - thickness / 2, height - thickness, -thickness / 2), glm::vec3(0, 0, 1)),
        VertexFormat(glm::vec3(width / 2 + thickness / 2, height - thickness, -thickness / 2), glm::vec3(0, 0, 1)),
        VertexFormat(glm::vec3(width / 2 + thickness / 2, height, -thickness / 2), glm::vec3(0, 0, 1)),
        VertexFormat(glm::vec3(-width / 2 - thickness / 2, height, -thickness / 2), glm::vec3(0, 0, 1)),
        VertexFormat(glm::vec3(-width / 2 - thickness / 2, height - thickness, thickness / 2), glm::vec3(0, 0, 1)),
        VertexFormat(glm::vec3(width / 2 + thickness / 2, height - thickness, thickness / 2), glm::vec3(0, 0, 1)),
        VertexFormat(glm::vec3(width / 2 + thickness / 2, height, thickness / 2), glm::vec3(0, 0, 1)),
        VertexFormat(glm::vec3(-width / 2 - thickness / 2, height, thickness / 2), glm::vec3(0, 0, 1)),
    };

    vector<unsigned int> indices = {
        // Stalpul din stanga
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        0, 4, 7, 0, 7, 3,
        1, 5, 6, 1, 6, 2,
        3, 2, 6, 3, 6, 7,
        0, 1, 5, 0, 5, 4,

        // Stalpul din dreapta
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        8, 12, 15, 8, 15, 11,
        9, 13, 14, 9, 14, 10,
        11, 10, 14, 11, 14, 15,
        8, 9, 13, 8, 13, 12,

        // Bara de sus
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23,
        16, 20, 23, 16, 23, 19,
        17, 21, 22, 17, 22, 18,
        19, 18, 22, 19, 22, 23,
        16, 17, 21, 16, 21, 20,
    };

    Mesh* gate = new Mesh("gate");
    gate->InitFromData(vertices, indices);
    return gate;
}

void Tema2::RenderTree(const glm::vec3& position, float trunkHeight, float trunkRadius, float coneHeight, float coneRadius) {
    // Trunchiul copacului
    glm::mat4 trunkMatrix = glm::translate(glm::mat4(1), position);
    trunkMatrix = glm::scale(trunkMatrix, glm::vec3(trunkRadius, trunkHeight + 2, trunkRadius));
    RenderMesh(meshes["cylinder"], glm::vec3(0.2f, 0.2f, 0.0f), trunkMatrix, useMinimap); // Maro

    // Conurile pentru frunze
    glm::mat4 coneMatrix = glm::translate(glm::mat4(1), position + glm::vec3(0, trunkHeight, 0));
    coneMatrix = glm::scale(coneMatrix, glm::vec3(coneRadius, coneHeight, coneRadius));
    RenderMesh(meshes["cone"], glm::vec3(0.0f, 0.4f, 0.0f), coneMatrix, useMinimap); // Verde

    coneMatrix = glm::translate(glm::mat4(1), position + glm::vec3(0, trunkHeight + coneHeight * 0.8f, 0));
    coneMatrix = glm::scale(coneMatrix, glm::vec3(coneRadius * 0.8f, coneHeight * 0.8f, coneRadius * 0.8f));
    RenderMesh(meshes["cone"], glm::vec3(0.0f, 0.4f, 0.0f), coneMatrix, useMinimap); // Verde
}

void Tema2::RenderBuilding(const glm::vec3& position, const glm::vec3& dimensions, const glm::vec3& roofDimensions) {
    // Cladirea (paralelipipedul)
    glm::mat4 buildingMatrix = glm::translate(glm::mat4(1), position);
    buildingMatrix = glm::scale(buildingMatrix, dimensions);
    RenderMesh(meshes["box"], glm::vec3(0.917f, 1, 0.392f), buildingMatrix, useMinimap); // Gri

    // Acoperisul (prisma triunghiulara)
    glm::vec3 roofPosition = position + glm::vec3(0, dimensions.y - 5, 0); // Pozitia acoperisului
    glm::mat4 roofMatrix = glm::translate(glm::mat4(1), roofPosition);
    roofMatrix = glm::scale(roofMatrix, roofDimensions); // Scalarea acoperisului
    RenderMesh(meshes["prism"], glm::vec3(0.6f, 0, 0), roofMatrix, useMinimap); // Rosu
}

void Tema2::RenderGate(const glm::vec3& position, float width, float height, const glm::vec3& color, float rotationAngle) {
    float thickness = 0.2f; // Grosimea liniilor
    float legHeight = 4.0f; // Inaltimea picioarelor

    // Rotire
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1), position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), glm::vec3(0, 1, 0));

    // Stalpul stang (picioare)
    glm::mat4 leftLeg = glm::translate(modelMatrix, glm::vec3(-width / 2.0f, legHeight / 2.0f, 0));
    leftLeg = glm::scale(leftLeg, glm::vec3(thickness, legHeight, thickness));
    RenderMesh(meshes["box"], color, leftLeg, useMinimap);

    // Stalpul drept (picioare)
    glm::mat4 rightLeg = glm::translate(modelMatrix, glm::vec3(width / 2.0f, legHeight / 2.0f, 0));
    rightLeg = glm::scale(rightLeg, glm::vec3(thickness, legHeight, thickness));
    RenderMesh(meshes["box"], color, rightLeg, useMinimap);

    // Linia de jos a cadrului suspendat
    glm::mat4 bottomFrame = glm::translate(modelMatrix, glm::vec3(0, legHeight, 0));
    bottomFrame = glm::scale(bottomFrame, glm::vec3(width, thickness, thickness));
    RenderMesh(meshes["box"], color, bottomFrame, useMinimap);

    // Linia de sus a cadrului suspendat
    glm::mat4 topFrame = glm::translate(modelMatrix, glm::vec3(0, legHeight + height, 0));
    topFrame = glm::scale(topFrame, glm::vec3(width, thickness, thickness));
    RenderMesh(meshes["box"], color, topFrame, useMinimap);

    // Bara verticala stanga a cadrului
    glm::mat4 leftFrame = glm::translate(modelMatrix, glm::vec3(-width / 2.0f, legHeight + height / 2.0f, 0));
    leftFrame = glm::scale(leftFrame, glm::vec3(thickness, height, thickness));
    RenderMesh(meshes["box"], color, leftFrame, useMinimap);

    // Bara verticala dreapta a cadrului
    glm::mat4 rightFrame = glm::translate(modelMatrix, glm::vec3(width / 2.0f, legHeight + height / 2.0f, 0));
    rightFrame = glm::scale(rightFrame, glm::vec3(thickness, height, thickness));
    RenderMesh(meshes["box"], color, rightFrame, useMinimap);
}

// Functie pentru verificarea distantei minime intre copaci
bool CanPlaceObject(const glm::vec3& newPosition, const vector<glm::vec3>& existingPositions, float minDistance) {
    for (const auto& position : existingPositions) {
        float distance = sqrt(pow(newPosition.x - position.x, 2) + pow(newPosition.z - position.z, 2));  // Calculam distanta dintre obiecte folosind distanta euclidiana
        if (distance < minDistance) {
            return false; // Obiectul este prea aproape de altul
        }
    }
    return true;
}

vector<glm::vec3> GeneratePositions(int count, float minX, float maxX, float minZ, float maxZ, float minDistance, int maxAttempts, vector<glm::vec3>& existingPositions) {
    vector<glm::vec3> positions;

    for (int i = 0; i < count; ++i) {
        bool placed = false;

        // Generam o pozitie aleatoare
        for (int attempt = 0; attempt < maxAttempts; ++attempt) {
            float x = minX + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxX - minX)));
            float z = minZ + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxZ - minZ)));
            float y = 0.0f;

            glm::vec3 newPosition(x, y, z);

            // Verificare daca noua pozitie respecta distanta minima fata de toate pozitiile existente
            if (CanPlaceObject(newPosition, existingPositions, minDistance)) {
                positions.push_back(newPosition);
                existingPositions.push_back(newPosition);
                placed = true;
                break;
            }
        }

        if (!placed) {
            std::cerr << "Nu am putut plasa obiectul #" << i + 1 << " dupa " << maxAttempts << " incercari!" << std::endl;
        }
    }

    return positions;
}

bool CheckCollisionDroneBuilding(const glm::vec3& dronePosition, float droneRadius, const glm::vec3& buildingPosition, const glm::vec3& buildingDimensions) {
    float halfX = buildingDimensions.x / 2.0f;
    float halfY = buildingDimensions.y / 2.0f;
    float halfZ = buildingDimensions.z / 2.0f;

    // Verificam coliziunea pe fiecare axa
    if (dronePosition.x + droneRadius > buildingPosition.x - halfX &&
        dronePosition.x - droneRadius < buildingPosition.x + halfX &&
        dronePosition.y + droneRadius > buildingPosition.y &&
        dronePosition.y - droneRadius < buildingPosition.y + buildingDimensions.y &&
        dronePosition.z + droneRadius > buildingPosition.z - halfZ &&
        dronePosition.z - droneRadius < buildingPosition.z + halfZ) {
        return true;
    }

    return false;
}

bool CheckCollisionWithGround(const glm::vec3& dronePosition, float droneRadius) {
    return dronePosition.y - droneRadius <= 0.0f;
}

bool CheckCollisionDroneGatePillar(const glm::vec3& dronePosition, float droneRadius, const glm::vec3& pillarPosition, float pillarRadius, float pillarHeight) {
    // Calculam distanta pe axele XZ
    float distanceXZ = sqrt(pow(dronePosition.x - pillarPosition.x, 2) + pow(dronePosition.z - pillarPosition.z, 2));

    // Verificam coliziunea pe XZ si pe Y
    if (distanceXZ < (droneRadius + pillarRadius)) {
        if (dronePosition.y - droneRadius <= pillarPosition.y + pillarHeight &&
            dronePosition.y + droneRadius >= pillarPosition.y) {
            return true; // Coliziune detectata
        }
    }
    return false;
}


void AdjustDronePositionCylinder(const glm::vec3& obstaclePosition, float obstacleRadius, float obstacleHeight, glm::vec3& dronePosition, float droneRadius) {
    glm::vec3 direction = dronePosition - glm::vec3(obstaclePosition.x, dronePosition.y, obstaclePosition.z);
    float distance = glm::length(glm::vec3(direction.x, 0, direction.z)); // Doar pe axa XZ

    if (distance < (droneRadius + obstacleRadius)) {
        // Calculam directia normalizata pentru a deplasa drona
        glm::vec3 normalizedDirection = glm::normalize(glm::vec3(direction.x, 0, direction.z));
        float overlap = (droneRadius + obstacleRadius) - distance;
        dronePosition += glm::vec3(normalizedDirection.x, 0, normalizedDirection.z) * overlap;
    }
}

void AdjustDronePositionCone(const glm::vec3& coneTip, float coneRadius, float coneHeight, glm::vec3& dronePosition, float droneRadius) {
    glm::vec3 direction = dronePosition - coneTip;
    float distance = glm::length(direction);

    if (distance < (droneRadius + coneRadius)) {
        glm::vec3 normalizedDirection = glm::normalize(direction);
        float overlap = (droneRadius + coneRadius) - distance;
        dronePosition += normalizedDirection * overlap;
    }
}

bool Tema2::IsDroneThroughGate(const glm::vec3& dronePosition, const glm::vec3& gatePosition, float gateWidth, float gateHeight, float legHeight) {
    // Calculeaza pozitia de jos si de sus a patratului suspendat
    float bottomY = gatePosition.y + legHeight;
    float topY = bottomY + gateHeight;

    // Verificam daca drona este in limitele patratului suspendat
    return abs(dronePosition.x - gatePosition.x) <= gateWidth / 2 && // In limitele pe axa X
        abs(dronePosition.z - gatePosition.z) <= gateWidth / 2 && // In limitele pe axa Z
        dronePosition.y >= bottomY && dronePosition.y <= topY;    // Intre limitele Y ale patratului
}


void Tema2::Init()
{
    renderCameraTarget = false;

    camera = new implemented::Camera1();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));  // Pozitia camerei / Punctul catre care priveste camera

    Mesh* terrain = GenerateTerrainMesh(150, 150); // Grid 150x150
    meshes[terrain->GetMeshID()] = terrain;

    useMinimap = false;

    // Configurarea camerei pentru minimap
    minimapCamera = std::make_shared<implemented::Camera1>();
    minimapCamera->Set(glm::vec3(0, 50, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));

    // Setarea proiectiei ortografice pentru minimap
    float orthoSize = 50.0f; // Dimensiunea scenei vazute in minimap
    minimapProjectionMatrix = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, 0.1f, 100.0f);  // Maparea coordonatelor 3D in 2D pentru afisare

    // Crearea si stocarea obiectelor
    meshes["cylinder"] = GenerateCylinder(0.2f, 1.5f, 20);
    meshes["cone"] = GenerateCone(0.5f, 1.0f, 20);
    meshes["prism"] = GeneratePrism(1.0f, 2.0f);
    meshes["gate"] = GenerateGate(1.5f, 1.5f, 0.05f);

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Shader pentru drona si restul entitatilor
    {
        Shader* shader = new Shader("DefaultShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Shader pentru teren
    {
        Shader* terrainShader = new Shader("TerrainShader");
        terrainShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "TerrainVertexShader.glsl"), GL_VERTEX_SHADER);
        terrainShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "TerrainFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        terrainShader->CreateAndLink();
        shaders[terrainShader->GetName()] = terrainShader;
    }

    fov = 60;
    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, Z_NEAR, Z_FAR);

    left = -10;
    right = 10;
    bottom = -10;
    top = 10;
    projectionType = true;

    dronePosition = glm::vec3(0.0f, 1.0f, 0.0f);
    droneRotation = 0.0f;

    // Seed pentru generatorul de numere aleatoare
    srand(static_cast<unsigned>(time(0)));

    float minDistance = 11.0f;  // Distanta minima intre obiecte
    int maxAttempts = 200;    // Numarul maxim de incercari pentru plasare
    vector<glm::vec3> existingPositions; // Pozitii deja ocupate

    // Generam pozitiile copacilor
    treePositions = GeneratePositions(50, -60.0f, 60.0f, -60.0f, 60.0f, minDistance, maxAttempts, existingPositions);

    // Generam pozitiile casutelor
    buildingPositions = GeneratePositions(10, -60.0f, 60.0f, -60.0f, 60.0f, minDistance, maxAttempts, existingPositions);

    nextGateIndex = 0;
    int gateCount = 10;
    gatePositions = GeneratePositions(gateCount, -60.0f, 60.0f, -60.0f, 60.0f, minDistance, maxAttempts, existingPositions);
    // Generare rotatii aleatorii pentru porti
    gateRotations.resize(gatePositions.size());
    for (size_t i = 0; i < gateRotations.size(); ++i) {
        gateRotations[i] = static_cast<float>(rand() % 360);  // Unghiuri intre 0 si 360 de grade
    }
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.529f, 0.808f, 0.922f, 1.0f);  // Culoarea cerului
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

static float bladeRotation = 0.0f;

void Tema2::Update(float deltaTimeSeconds)
{
    useMinimap = false;  // Pentru perspectiva lumii normale

    // Randare terenului
    glm::mat4 terrainMatrix = glm::scale(glm::mat4(1), glm::vec3(1.0f)); // Transformari pentru teren
    RenderTerrain(meshes["terrain"], shaders["TerrainShader"], terrainMatrix);

    float droneRadius = 1.0f; // Raza aproximativa a dronei

    // Verificam coliziunea cu copacii
    for (const auto& treePosition : treePositions) {
        float trunkRadius = 0.1;  // Raza trunchiului
        float trunkHeight = 1.5f;  // Inaltimea trunchiului
        float coneHeight = 1.5f;   // Inaltimea conului
        float coneRadius = 1.5f;   // Raza conului

        // Coliziune cu trunchiul (aproximat cu cilindru)
        if (dronePosition.y < treePosition.y + trunkHeight) { // Daca drona este la nivelul trunchiului
            AdjustDronePositionCylinder(treePosition, trunkRadius, trunkHeight, dronePosition, droneRadius);
        }

        // Coliziune cu partea de sus a copacului (conuri)
        glm::vec3 coneTip = treePosition + glm::vec3(0, trunkHeight + coneHeight, 0);
        AdjustDronePositionCone(coneTip, coneRadius, coneHeight, dronePosition, droneRadius);
    }

    // Verificam coliziunea cu cladirile
    for (const auto& buildingPosition : buildingPositions) {
        glm::vec3 buildingDimensions = glm::vec3(4.0f, 10.0f, 4.0f);
        if (CheckCollisionDroneBuilding(dronePosition, droneRadius, buildingPosition, buildingDimensions)) {
            glm::vec3 closestPoint = glm::clamp(dronePosition,
                buildingPosition - buildingDimensions / 2.0f,
                buildingPosition + buildingDimensions / 2.0f);  // Determinarea celui mai apropiat punct dintre coltul stanga jos si dreapta sus al cladirii fata de drona

            glm::vec3 direction = dronePosition - closestPoint;
            float distance = glm::length(direction);

            if (distance < droneRadius) {
                glm::vec3 normalizedDirection = glm::normalize(direction);
                float overlap = droneRadius - distance;
                dronePosition += normalizedDirection * overlap;
            }
        }
    }

    // Verificam coliziunea cu solul
    if (CheckCollisionWithGround(dronePosition, droneRadius)) {
        dronePosition.y = droneRadius;
    }

    // Randarea copacilor
    for (const auto& position : treePositions) {
        RenderTree(position, 2, 1, 2, 2.5f);
    }

    // Randarea cladirilor
    for (const auto& position : buildingPositions) {
        RenderBuilding(position, glm::vec3(4.0f, 8.0f, 4.0f), glm::vec3(6.5f, 3.5f, 6.0f));
    }

    bladeRotation += deltaTimeSeconds * 500.0f;

    // Pozitia camerei relative la drona
    glm::vec3 cameraOffset = glm::vec3(0.0f, 2.0f, 5.0f); // Camera este deasupra si in spatele dronei
    glm::vec3 offsetPosition = glm::vec3(  // Calcularea rotatiei camerei in jurul dronei
        -sin(RADIANS(droneRotation)) * cameraOffset.z,
        cameraOffset.y,  //  y ramane neschimbat, deoarece camera nu se roteste pe verticala
        cos(RADIANS(droneRotation)) * cameraOffset.z
    );

    glm::vec3 cameraPosition = dronePosition + offsetPosition;

    // Setam camera
    camera->Set(cameraPosition, dronePosition, glm::vec3(0, 1, 0));  

    glm::mat4 modelMatrix = glm::mat4(1);

    glm::mat4 globalTransform = glm::translate(glm::mat4(1), dronePosition) *
        glm::rotate(glm::mat4(1), RADIANS(droneRotation), glm::vec3(0, 1, 0)) *
        glm::scale(glm::mat4(1), glm::vec3(0.5f));

    // Primul dreptunghi
    modelMatrix = globalTransform * glm::translate(glm::mat4(1), glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(4, 0.2f, 0.2f));
    RenderMesh(meshes["box"], glm::vec3(0.69f, 0.74f, 0.74f), modelMatrix, false);

    // Al doilea dreptunghi
    modelMatrix = globalTransform * glm::translate(glm::mat4(1), glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(-45.0f), glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(4, 0.2f, 0.2f));
    RenderMesh(meshes["box"], glm::vec3(0.69f, 0.74f, 0.74f), modelMatrix, false);

    // Pozitiile cuburilor la capetele dreptunghiurilor
    for (int i = 0; i < 4; i++) {
        // Calcularea coordonatelor cuburilor pe baza rotatiilor dreptunghiurilor
        float angle = (i % 2 == 0) ? 45.0f : -45.0f;  // Unghiurile dreptunghiurilor
        float x = 1.9f * cos(RADIANS(angle)) * ((i < 2) ? 1.0f : -1.0f);
        float z = 1.9f * sin(RADIANS(angle)) * ((i < 2) ? 1.0f : -1.0f);

        // Render cuburi
        glm::mat4 cubeMatrix = globalTransform * glm::translate(glm::mat4(1), glm::vec3(x, 1.2, z));
        cubeMatrix = glm::rotate(cubeMatrix, RADIANS(angle), glm::vec3(0, 1, 0)); // Alinierea rotatiei cuburilor cu dreptunghiurile
        cubeMatrix = glm::scale(cubeMatrix, glm::vec3(0.2f));
        RenderMesh(meshes["box"], glm::vec3(0.69f, 0.85f, 0.87f), cubeMatrix, false);

        // Render elicele
        glm::mat4 bladeMatrix = globalTransform * glm::translate(glm::mat4(1), glm::vec3(x, 1.325, z));
        bladeMatrix = glm::rotate(bladeMatrix, RADIANS(bladeRotation), glm::vec3(0, 1, 0));
        bladeMatrix = glm::scale(bladeMatrix, glm::vec3(0.5f, 0.05f, 0.15f));
        RenderMesh(meshes["box"], glm::vec3(0.0f, 0.0f, 0.0f), bladeMatrix, false);
    }

    // Dimensiunile portii
    float gateWidth = 4.0f;
    float gateHeight = 4.0f;
    float pillarRadius = 0.2f; // Raza aproximativa a stalpilor
    float pillarHeight = gateHeight + 3; // Inaltimea stalpilor

    // Generare porti si verificare coliziuni cu stalpii
    for (size_t i = 0; i < gatePositions.size(); i++) {
        glm::vec3 color;

        // Determinam culoarea portii
        if (i == nextGateIndex) {
            color = glm::vec3(0, 1, 0); // Verde pentru poarta curenta
        }
        else if (i == nextGateIndex + 1) {
            color = glm::vec3(1, 0.5f, 0); // Portocaliu pentru urmatoarea poarta
        }
        else {
            color = glm::vec3(1, 0, 0); // Rosu pentru toate celelalte porti
        }
        RenderGate(gatePositions[i], gateWidth, gateHeight, color, gateRotations[i]);

        // Calculam pozitiile rotite ale stalpilor
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), glm::radians(gateRotations[i]), glm::vec3(0, 1, 0));
        glm::vec3 leftPillar = gatePositions[i] + glm::vec3(-gateWidth / 2.0f, 0, 0); // Stalpul din stanga
        glm::vec3 rightPillar = gatePositions[i] + glm::vec3(gateWidth / 2.0f, 0, 0); // Stalpul din dreapta

        // Aplicam rotatia
        leftPillar = glm::vec3(rotationMatrix * glm::vec4(leftPillar - gatePositions[i], 1.0f)) + gatePositions[i];
        rightPillar = glm::vec3(rotationMatrix * glm::vec4(rightPillar - gatePositions[i], 1.0f)) + gatePositions[i];

        // Verificam coliziunea cu stalpii
        if (CheckCollisionDroneGatePillar(dronePosition, droneRadius, leftPillar, pillarRadius, pillarHeight)) {
            AdjustDronePositionCylinder(leftPillar, pillarRadius, pillarHeight, dronePosition, droneRadius);
        }
        if (CheckCollisionDroneGatePillar(dronePosition, droneRadius, rightPillar, pillarRadius, pillarHeight)) {
            AdjustDronePositionCylinder(rightPillar, pillarRadius, pillarHeight, dronePosition, droneRadius);
        }
    }

    // Verificam daca drona a trecut prin poarta
    if (IsDroneThroughGate(dronePosition, gatePositions[nextGateIndex], 4.0f, 4.0f, 4.0f)) { 
        nextGateIndex++;
        if (nextGateIndex >= gatePositions.size()) {
            std::cout << "Ai trecut prin toate portile!" << std::endl;
            nextGateIndex = 0;
        }
    }

    // Setam viewport-ul pentru minimap
    glm::ivec2 resolution = window->GetResolution();
    useMinimap = true;  // Setam pentru perspectiva din minimap
    glViewport(resolution.x - 250, resolution.y - 250, 250, 250);

    // Curatam doar depth buffer-ul pentru minimap
    glClear(GL_DEPTH_BUFFER_BIT);

    // Configuram camera minimap-ului sa fie deasupra dronei
    glm::vec3 minimapPosition = dronePosition + glm::vec3(0, 50, 0);
    glm::vec3 minimapTarget = dronePosition;
    minimapCamera->Set(minimapPosition, minimapTarget, glm::vec3(0, 0, -1));

    // Randeaza scena din perspectiva minimap-ului
    for (const auto& position : treePositions) {
        RenderTree(position, 1.5f, 1, 2, 2.5f);
    }
    for (const auto& position : buildingPositions) {
        RenderBuilding(position, glm::vec3(4.0f, 8.0f, 4.0f), glm::vec3(6.5f, 3.5f, 6.0f));
    }

    // Randeaza drona pe minimap
    glm::mat4 droneMatrix = glm::translate(glm::mat4(1), dronePosition) *
        glm::rotate(glm::mat4(1), RADIANS(droneRotation), glm::vec3(0, 1, 0)) *
        glm::scale(glm::mat4(1), glm::vec3(2.5f, 0.5f, 3.5f));
    RenderMesh(meshes["prism"], glm::vec3(1, 1, 1), droneMatrix, true);

    // Randeaza portile in minimap
    for (size_t i = 0; i < gatePositions.size(); i++) {
        glm::vec3 color;

        // Determinam culoarea portii
        if (i == nextGateIndex) {
            color = glm::vec3(0, 1, 0); // Verde pentru poarta curenta
        }
        else if (i == nextGateIndex + 1) {
            color = glm::vec3(1, 0.5f, 0); // Portocaliu pentru urmatoarea poarta
        }
        else {
            color = glm::vec3(1, 0, 0); // Rosu pentru toate celelalte porti
        }
        glm::mat4 gateMatrix = glm::translate(glm::mat4(1), gatePositions[i]);
        gateMatrix = glm::scale(gateMatrix, glm::vec3(2.0f, 4.0f, 1.0f)); // Dimensiunile portii
        RenderMesh(meshes["box"], color, gateMatrix, true); // Afisam in minimap
    }

    // Resetam viewport-ul pentru scena 
    glViewport(0, 0, resolution.x, resolution.y);

}


void Tema2::FrameEnd()
{
    if (!useMinimap) {
        DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
    }
}

void Tema2::SetColor(Shader* shader, const glm::vec3& color)
{
    GLint location = glGetUniformLocation(shader->program, "fragColorUniform");
    glUniform3fv(location, 1, glm::value_ptr(color));
}


void Tema2::RenderMesh(Mesh* mesh, const glm::vec3& color, const glm::mat4& modelMatrix, bool useMinimap) {
    if (!mesh || !shaders["DefaultShader"]->program) return;

    glUseProgram(shaders["DefaultShader"]->program);

    GLint locObjectColor = glGetUniformLocation(shaders["DefaultShader"]->program, "objectColor");  // Obtine locatia variabilei uniforme objColor din shader
    glUniform3fv(locObjectColor, 1, glm::value_ptr(color));  // Trimite valoarea culorii catre objColor din shader

    // Trimiterea matricelor de transformare
    glUniformMatrix4fv(glGetUniformLocation(shaders["DefaultShader"]->program, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaders["DefaultShader"]->program, "view"), 1, GL_FALSE, glm::value_ptr(useMinimap ? minimapCamera->GetViewMatrix() : camera->GetViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shaders["DefaultShader"]->program, "projection"), 1, GL_FALSE, glm::value_ptr(useMinimap ? minimapProjectionMatrix : projectionMatrix));

    mesh->Render();
}

void Tema2::RenderTerrain(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) {
    if (!mesh || !shader || !shader->GetProgramID()) return;

    glUseProgram(shader->program);

    // Trimite matricea model
    GLint locModel = glGetUniformLocation(shader->program, "model");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Trimite matricea view
    GLint locView = glGetUniformLocation(shader->program, "view");
    glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

    // Trimite matricea projection
    GLint locProjection = glGetUniformLocation(shader->program, "projection");
    glUniformMatrix4fv(locProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Renderizeaza mesh-ul
    mesh->Render();
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see input_controller.h.
 */

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float moveSpeed = 10.0f;
    float rotationSpeed = 100.0f;

    if (window->KeyHold(GLFW_KEY_W)) {
        dronePosition -= glm::vec3(-sin(RADIANS(droneRotation)), 0, cos(RADIANS(droneRotation))) * deltaTime * moveSpeed;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        dronePosition += glm::vec3(-sin(RADIANS(droneRotation)), 0, cos(RADIANS(droneRotation))) * deltaTime * moveSpeed;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        dronePosition -= glm::vec3(cos(RADIANS(droneRotation)), 0, sin(RADIANS(droneRotation))) * deltaTime * moveSpeed;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        dronePosition += glm::vec3(cos(RADIANS(droneRotation)), 0, sin(RADIANS(droneRotation))) * deltaTime * moveSpeed;
    }

    if (window->KeyHold(GLFW_KEY_UP)) {
        dronePosition.y += deltaTime * moveSpeed;
    }
    if (window->KeyHold(GLFW_KEY_DOWN)) {
        dronePosition.y -= deltaTime * moveSpeed;
    }

    if (window->KeyHold(GLFW_KEY_LEFT)) {
        droneRotation -= deltaTime * rotationSpeed;
    }
    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        droneRotation += deltaTime * rotationSpeed;
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_O)
    {
        projectionType = false;
        projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
    }
    if (key == GLFW_KEY_P)
    {
        projectionType = true;
        projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, Z_NEAR, Z_FAR);
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}