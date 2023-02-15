#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/Tema2/my_camera.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,my_camera::Camera* camera,
                                                                            const glm::vec3& color = glm::vec3(1));
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void RenderScene(float deltaTimeSeconds, my_camera::Camera* rendering_camera);
        void DrawEnemyCar(int k, float deltaTimeSeconds, my_camera::Camera* rendering_camera);
        Mesh* CreatePiramid();

    protected:
        my_camera::Camera* camera;
        glm::mat4 projectionMatrix;
        my_camera::Camera* cameraMini;
        glm::mat4 projectionMatrixMini;

    private:
        float acceleration = 1.f;  // the acceleration of the car
        float rotation_angle = 0;  // the angle with which the car rotates on keys D and A
        float min_dist_to_circuit; // the minimum distance between my car and the circuit's middle lane
        int hit = 0, out_of_the_circuit = 0;

        // variables for the enemy cars
        int indices[7] = { 0, 0, 0, 0, 0, 0, 0 };
        float speed[7] = { 1.5f, 2.f, 2.5f, 1.75f, 1.f, 1.1f, 1.2f };
        float var[7] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
        float radians[7] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
        float offset[7] = { -0.2f, 0.1f, 0.1f, -0.1f, 0.15f, 0.12f, -0.1f };
        std::vector<glm::vec3> positions = { glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f),
                                             glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f) };
        std::vector<glm::vec3> color_cars = { glm::vec3(0.815, 0.455, 0.088),
                                              glm::vec3(0.139, 0.975, 0.739),
                                              glm::vec3(0.972, 0.303, 0.990),
                                              glm::vec3(0.445, 0.001, 0.319),
                                              glm::vec3(1.000, 0.861, 0.256),
                                              glm::vec3(0.576, 0.287, 1.000),
                                              glm::vec3(1.000, 0.665, 0.568) };

        ViewportArea miniViewportArea;
        std::vector<glm::vec3> circuit = {
            glm::vec3(0.f, 0.f, 0.f),
            glm::vec3(-0.33f, 0.f, 0.13f),
            glm::vec3(-0.73f, 0.f, 0.41f),
            glm::vec3(-1.f, 0.f, 0.88f),
            glm::vec3(-1.22f, 0.f, 1.26f),
            glm::vec3(-1.5f, 0.f, 1.5f),
            glm::vec3(-1.92f, 0.f, 1.6f),
            glm::vec3(-2.4f, 0.f, 1.57f),
            glm::vec3(-3.f, 0.f, 1.58f),
            glm::vec3(-3.51f, 0.f, 1.68f),
            glm::vec3(-3.83f, 0.f, 2.f),
            glm::vec3(-3.82f, 0.f, 2.3f),
            glm::vec3(-3.59f, 0.f, 2.68f),
            glm::vec3(-3.17f, 0.f, 2.89f),
            glm::vec3(-2.78f, 0.f, 3.f),
            glm::vec3(-2.51f, 0.f, 3.34f),
            glm::vec3(-2.51f, 0.f, 3.65f),
            glm::vec3(-2.66f, 0.f, 4.06f),
            glm::vec3(-3.15f, 0.f, 4.4f),
            glm::vec3(-3.6f, 0.f, 4.69f),
            glm::vec3(-4.f, 0.f, 5.f),
            glm::vec3(-4.1f, 0.f, 5.46f),
            glm::vec3(-4.14f, 0.f, 6.07f),
            glm::vec3(-4.18f, 0.f, 6.62f),
            glm::vec3(-4.18f, 0.f, 7.11f),
            glm::vec3(-4.18f, 0.f, 7.6f),
            glm::vec3(-4.16f, 0.f, 8.21f),
            glm::vec3(-4.12f, 0.f, 8.7f),
            glm::vec3(-4.04f, 0.f, 9.17f),
            glm::vec3(-3.87f, 0.f, 9.61f),
            glm::vec3(-3.55f, 0.f, 9.93f),
            glm::vec3(-3.f, 0.f, 10.f),
            glm::vec3(-2.49f, 0.f, 9.87f),
            glm::vec3(-1.96f, 0.f, 9.64f),
            glm::vec3(-1.6f, 0.f, 9.28f),
            glm::vec3(-1.19f, 0.f, 8.74f),
            glm::vec3(-1.13f, 0.f, 8.19f),
            glm::vec3(-1.11f, 0.f, 7.45f),
            glm::vec3(-1.09f, 0.f, 6.79f),
            glm::vec3(-1.07f, 0.f, 6.13f),
            glm::vec3(-1.05f, 0.f, 5.61f),
            glm::vec3(-1.f, 0.f, 5.f),
            glm::vec3(-1.03f, 0.f, 4.42f),
            glm::vec3(-1.03f, 0.f, 3.72f),
            glm::vec3(-1.f, 0.f, 3.f),
            glm::vec3(-1.f, 0.f, 2.5f),
            glm::vec3(-0.79f, 0.f, 2.06f),
            glm::vec3(-0.32f, 0.f, 1.72f),
            glm::vec3(0.23f, 0.f, 1.75f),
            glm::vec3(0.59f, 0.f, 2.25f),
            glm::vec3(0.78f, 0.f, 2.95f),
            glm::vec3(0.76f, 0.f, 3.67f),
            glm::vec3(0.78f, 0.f, 4.29f),
            glm::vec3(0.78f, 0.f, 4.8f),
            glm::vec3(0.8f, 0.f, 5.43f),
            glm::vec3(0.8f, 0.f, 6.14f),
            glm::vec3(0.8f, 0.f, 6.81f),
            glm::vec3(0.74f, 0.f, 7.36f),
            glm::vec3(0.74f, 0.f, 8.02f),
            glm::vec3(0.78f, 0.f, 8.66f),
            glm::vec3(0.88f, 0.f, 9.27f),
            glm::vec3(1.1f, 0.f, 9.64f),
            glm::vec3(1.5f, 0.f, 9.89f),
            glm::vec3(2.f, 0.f, 10.f),
            glm::vec3(2.48f, 0.f, 9.78f),
            glm::vec3(3.f, 0.f, 9.5f),
            glm::vec3(3.17f, 0.f, 8.55f),
            glm::vec3(3.13f, 0.f, 7.64f),
            glm::vec3(3.15f, 0.f, 6.67f),
            glm::vec3(3.13f, 0.f, 5.95f),
            glm::vec3(3.13f, 0.f, 5.01f),
            glm::vec3(3.11f, 0.f, 4.16f),
            glm::vec3(2.92f, 0.f, 3.31f),
            glm::vec3(2.48f, 0.f, 2.25f),
            glm::vec3(2.f, 0.f, 1.5f),
            glm::vec3(1.5f, 0.f, 1.f),
            glm::vec3(1.f, 0.f, 0.5f),
            glm::vec3(0.5f, 0.f, 0.26f),
            glm::vec3(0.f, 0.f, 0.f),
        };
    };
}   // namespace m1
