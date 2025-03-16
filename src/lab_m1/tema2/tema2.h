#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema2/tema_camera.h"


#define Z_FAR		(100.f)
#define Z_NEAR		(.01f)

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, const glm::vec3& color, const glm::mat4& modelMatrix, bool useMinimap);
        void RenderTerrain(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
        void RenderTree(const glm::vec3& position, float trunkHeight, float trunkRadius, float coneHeight, float coneRadius);
        void RenderBuilding(const glm::vec3& position, const glm::vec3& dimensions, const glm::vec3& roofDimensions);
        void RenderGate(const glm::vec3& position, float width, float height, const glm::vec3& color, float rotationAngle);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void SetColor(Shader* shader, const glm::vec3& color);
        Mesh* GenerateTerrainMesh(int width, int height);
        Mesh* GenerateCylinder(float radius, float height, int segments);
        Mesh* GenerateCone(float radius, float height, int segments);
        Mesh* GeneratePrism(float baseLength, float height);
        Mesh* GenerateGate(float width, float height, float thickness);
        bool IsDroneThroughGate(const glm::vec3& dronePosition, const glm::vec3& gatePosition, float gateWidth, float gateHeight, float legHeight);

    protected:
        implemented::Camera1* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        // TODO(student): If you need any other class variables, define them here.
        bool projectionType;
        GLfloat right;
        GLfloat left;
        GLfloat bottom;
        GLfloat top;
        GLfloat fov;
        glm::vec3 dronePosition;
        float droneRotation;
        std::vector<glm::vec3> treePositions;
        std::vector<glm::vec3> buildingPositions;
        glm::mat4 minimapProjectionMatrix;
        std::shared_ptr<implemented::Camera1> minimapCamera;
        bool useMinimap;
        std::vector<glm::vec3> gatePositions;
        std::vector<float> gateRotations;
        int gateCount;
        int nextGateIndex;
    };
}   // namespace m1
