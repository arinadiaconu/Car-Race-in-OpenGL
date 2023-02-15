#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include "lab_m1/Tema2/Transform3D.h"
#include "lab_m1/Tema2/Draw2D.h"

using namespace std;
using namespace m1;


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    camera = new my_camera::Camera();
    camera->Set(glm::vec3(0.1, 0.4, 0.1), glm::vec3(1.7, -0.4, 1), glm::vec3(0, 1, 0));
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, .01f, 200.f);

    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

    cameraMini = new my_camera::Camera();
    projectionMatrixMini = glm::ortho(0.01f, 10.f, 0.01f, 10.f, .01f, 200.f);

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* circuit = Draw2D::CreateCircuit("circuit", glm::vec3(0, 0, 0));
        AddMeshToList(circuit);
    }

    // creates a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("MyShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


Mesh* Tema2::CreatePiramid() { // creates a piramid
    vector<VertexFormat> vertices {
        VertexFormat(glm::vec3(1.0, 0.0, 0.0), glm::vec3(0, 1, 0)),  // 0
        VertexFormat(glm::vec3(0.0, 0.0, -1.0), glm::vec3(0, 0, 1)), // 1
        VertexFormat(glm::vec3(-1.0, 0.0, 0.0), glm::vec3(1, 0, 0)), // 2
        VertexFormat(glm::vec3(0.0, 0.0, 1.0), glm::vec3(1, 1, 1)),  // 3
        VertexFormat(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0, 0, 0)),  // 4
    };

    vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3,
        3, 4, 2,
        0, 4, 3,
        1, 4, 0,
        2, 4, 1
    };

    Mesh* piramid = new Mesh("piramid");
    piramid->InitFromData(vertices, indices);
    return piramid;
}


void Tema2::FrameStart()
{
    // clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.498, 0.923, 0.960, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->props.resolution;
    // sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::DrawEnemyCar(int k, float deltaTimeSeconds, my_camera::Camera* rendering_camera) {
    glm::vec3 up(0, 1, 0);

    if (indices[k] == circuit.size() - 2) indices[k] = 0;
    var[k] += (speed[k] * deltaTimeSeconds);
    glm::vec3 f, s, t;
    if (indices[k] == circuit.size() - 2) {
        f = circuit[indices[k]];
        s = circuit[indices[k] + 1];
        t = circuit[0];
    } else {
        f = circuit[indices[k]];
        s = circuit[indices[k] + 1];
        t = circuit[indices[k] + 2];
    }
    
    glm::vec3 d1 = s - f;
    glm::vec3 cr1 = normalize(cross(d1, up));
    glm::vec3 left1 = f + (cr1 * offset[k]);

    glm::vec3 d2 = t - s;
    glm::vec3 cr2 = normalize(cross(d2, up));
    glm::vec3 left2 = s + (cr2 * offset[k]);

    glm::vec3 dirr = left2 - left1;

    radians[k] = atan(dirr.x / dirr.z);
    radians[k] += RADIANS(90);

    glm::vec3 p1 = left1 + (dirr * var[k]);
    positions[k].x = p1.x;
    positions[k].z = p1.z;

    if (left2.x >= left1.x) { // -->
        if (p1.x > left2.x) {
            indices[k]++;
            var[k] = 0;
        }
        else if (p1.x == left1.x) {
            if ((left2.z > left1.z) && (p1.z > left2.z)) { // upward
                indices[k]++;
                var[k] = 0;
            }
            else if ((left2.z < left1.z) && (p1.z < left2.z)) { // downward
                indices[k]++;
                var[k] = 0;
            }
        }
    }
    else if (left2.x < left1.x) { // <--
        if (p1.x <= left2.x) {
            indices[k]++;
            var[k] = 0;
        }
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(positions[k].x, 0, positions[k].z));
        modelMatrix *= Transform3D::RotateOY(radians[k]);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f, 0.25f, 0.07f));
        RenderSimpleMesh(meshes["box"], shaders["MyShader"], modelMatrix, rendering_camera, color_cars[k]);
    }
}


void Tema2::RenderScene(float deltaTimeSeconds, my_camera::Camera* rendering_camera) {
    // the collisions with the circuit's edges
    glm::vec3 p1, p2; // each segment is defined by two points on the circuit - p1 and p2
    min_dist_to_circuit = 10.f;
    for (int i = 0; i < circuit.size(); i++) { // searches for the minimum distance between the car and the center of the circuit
        if (sqrt(pow((circuit[i].x - camera->GetTargetPosition().x), 2) +
            pow((circuit[i].z - camera->GetTargetPosition().z), 2)) < min_dist_to_circuit) {
            p1 = circuit[i];
            if (i == circuit.size() - 1) p2 = circuit[0];
            else p2 = circuit[i + 1];
            min_dist_to_circuit = sqrt(pow((circuit[i].x - camera->GetTargetPosition().x), 2) +
                pow((circuit[i].z - camera->GetTargetPosition().z), 2));
        }
    }
    float m = (p1.z - p2.z) / (p1.x - p2.x); // the slope of current the segment
    if ((abs(m * camera->GetTargetPosition().x - camera->GetTargetPosition().z + (p1.z - m * p1.x)) / sqrt(pow(m, 2) + 1)) > 0.3f) {
        out_of_the_circuit = 1;
    }
    else {
        out_of_the_circuit = 0;
    }

    // the crown of a trial tree
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-3, 0, -3));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75f, 1, 0.75f));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["MyShader"], modelMatrix, rendering_camera, glm::vec3(0.214, 0.445, 0.093));
    }

    // the trunk of a trial tree
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-3, 0, -3));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 1, 0.25f));
        RenderSimpleMesh(meshes["box"], shaders["MyShader"], modelMatrix, rendering_camera, glm::vec3(0.385, 0.313, 0.279));
    }

    // the trees next to the circuit
    glm::vec3 up(0, 1, 0);

    for (int i = 0; i < circuit.size() - 1; i++) {
        glm::vec3 first = circuit[i];
        glm::vec3 second = circuit[i + 1];
        glm::vec3 d = second - first;
        glm::vec3 p = normalize(cross(d, up));
        glm::vec3 right = circuit[i] + (p * 1.1f);
        glm::vec3 left = circuit[i] - (p * 0.7f);

        glm::vec3 right_bonus = circuit[i] + (p * 1.8f);
        glm::vec3 left_bonus = circuit[i] - (p * 0.8f);
        glm::vec3 right_bonus_puffs1 = circuit[i] + (p * 0.8f);
        glm::vec3 right_bonus_puffs2 = circuit[i] + (p * 0.7f);
        glm::vec3 left_bonus_puffs1 = circuit[i] - (p * 0.6f);
        glm::vec3 left_bonus_puffs2 = circuit[i] - (p * 0.75f);

        if (i % 3 == 1) { // the trees on the exterior of the circuit
            { // the crown of the tree
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(right.x, 0, right.z));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75f, 1, 0.75f));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
                RenderSimpleMesh(meshes["box"], shaders["MyShader"], modelMatrix, rendering_camera, glm::vec3(0.214, 0.445, 0.093));
            }
            { // trunchiul
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(right.x, 0, right.z));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 1, 0.25f));
                RenderSimpleMesh(meshes["box"], shaders["MyShader"], modelMatrix, rendering_camera, glm::vec3(0.385, 0.313, 0.279));
            }
        }

        if (i % 18 == 2) { // the trees on the interior of the circuit
            { // the crown
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(left.x, 0, left.z));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75f, 1, 0.75f));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
                RenderSimpleMesh(meshes["box"], shaders["MyShader"], modelMatrix, rendering_camera, glm::vec3(0.214, 0.445, 0.093));
            }
            { // the trunk
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(left.x, 0, left.z));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 1, 0.25f));
                RenderSimpleMesh(meshes["box"], shaders["MyShader"], modelMatrix, rendering_camera, glm::vec3(0.385, 0.313, 0.279));
            }
        }

        // bonus trees
        // outside the circuit
        if ((right_bonus.x >= 4.f || right_bonus.x <= -4.f) || (right_bonus.z >= 10.f || right_bonus.z <= -1.f)) {
            { // the crown of the tree
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(right_bonus.x, 0, right_bonus.z));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.55f, 0.8, 0.55f));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.75, 0));
                RenderSimpleMesh(meshes["sphere"], shaders["MyShader"], modelMatrix, rendering_camera, glm::vec3(0.043, 0.280, 0.042));
            }            
            { // tree trunk
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(right_bonus.x, 0, right_bonus.z));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f, 0.5f, 0.15f));
                RenderSimpleMesh(meshes["box"], shaders["MyShader"], modelMatrix, rendering_camera, glm::vec3(0.280, 0.144, 0.082));
            }
        }
        if (i % 15 == 0) { // inside the circuit
            { // the crown of the tree
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(left_bonus.x, 0, left_bonus.z));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.2f, 0));
                RenderSimpleMesh(CreatePiramid(), shaders["MyShader"], modelMatrix, rendering_camera, glm::vec3(0.001, 0.220, 0.098));
            }
            { // tree trunk
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(left_bonus.x, 0, left_bonus.z));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.10f, 0.5f, 0.10f));
                RenderSimpleMesh(meshes["box"], shaders["MyShader"], modelMatrix, rendering_camera, glm::vec3(0.280, 0.144, 0.082));
            }
        }
        // puffs on the grass
        if (i % 4 == 0) {
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(right_bonus_puffs1.x, 0, right_bonus_puffs1.z));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f, 0.25f, 0.15f));
                RenderSimpleMesh(meshes["sphere"], shaders["MyShader"], modelMatrix, rendering_camera, glm::vec3(0.660, 0.001, 0.122));
            }
        }
        if (i % 5 == 0) {
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(right_bonus_puffs2.x, 0, right_bonus_puffs2.z));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f, 0.25f, 0.15f));
                RenderSimpleMesh(meshes["sphere"], shaders["MyShader"], modelMatrix, rendering_camera, glm::vec3(0.995, 0.836, 0.092));
            }
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(left_bonus_puffs1.x, 0, left_bonus_puffs1.z));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f, 0.25f, 0.15f));
                RenderSimpleMesh(meshes["sphere"], shaders["MyShader"], modelMatrix, rendering_camera, glm::vec3(0.995, 0.836, 0.092));
            }
        }
        if (i % 6 == 0) {
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(left_bonus_puffs2.x, 0, left_bonus_puffs2.z));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f, 0.25f, 0.15f));
                RenderSimpleMesh(meshes["sphere"], shaders["MyShader"], modelMatrix, rendering_camera, glm::vec3(0.660, 0.001, 0.122));
            }
        }
    }

    // the enemy cars
    for (int i = 0; i < 7; i++) {
        DrawEnemyCar(i, deltaTimeSeconds, rendering_camera);
    }

    // the finish line
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::rotate(modelMatrix, RADIANS(75), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.16f, 0.1f, 0.1f));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.6, 0.6f, 0.5f));
        RenderSimpleMesh(Draw2D::CreateFinishLine("finish_line", glm::vec3(0.0f, 0.0f, 0.0f), true),
                            shaders["MyShader"], modelMatrix, rendering_camera, glm::vec3(1.000, 0.356, 0.968));
    }
    
    // the mid line for the circuit
    for (int i = 0; i < circuit.size() - 1; i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderSimpleMesh(Draw2D::CreateRectangle("line", circuit[i], circuit[i + 1], true),
                    shaders["MyShader"], modelMatrix, rendering_camera, glm::vec3(1, 1, 1));
    }

    // the circuit
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.06f, 0));
        RenderSimpleMesh(meshes["circuit"], shaders["MyShader"], modelMatrix, rendering_camera, glm::vec3(0, 0, 0));
    }

    // render ground
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.05f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.25f));
        RenderSimpleMesh(meshes["plane"], shaders["MyShader"], modelMatrix, rendering_camera, glm::vec3(0.281, 0.880, 0.109));
    }

    // my car
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::rotate(modelMatrix, rotation_angle, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(-30), glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f, 0.25f, 0.07f));
        RenderSimpleMesh(meshes["box"], shaders["MyShader"], modelMatrix, rendering_camera, glm::vec3(1, 0, 0));
    }
    
    // the acceleration of the car
    if (window->KeyHold(GLFW_KEY_W)) acceleration += 0.005f;
    else acceleration = 1.f;
}


void Tema2::Update(float deltaTimeSeconds)
{
    cameraMini->Set(glm::vec3(camera->GetTargetPosition().x + 0.1f, 2.0, camera->GetTargetPosition().z + 0.1f),
                glm::vec3(camera->GetTargetPosition().x, 0.1, camera->GetTargetPosition().z), glm::vec3(0, 1, 0));
    RenderScene(deltaTimeSeconds, camera);

    // rendering the scene again in the miniViewport
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    RenderScene(deltaTimeSeconds, cameraMini);
}


void Tema2::FrameEnd()
{
}


void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, my_camera::Camera* cameraInFunction, 
                                                                                                            const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "car_position"), 1, GL_FALSE, 
                                                                                glm::value_ptr(cameraInFunction->GetViewMatrix()));
    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = cameraInFunction->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 1.5f;
    float sensivityOY = 1.5f; 
     
    hit = 0;
    for (int i = 0; i < 7; i++) { // checking if my car hits an enemy car
        if (sqrt(pow((camera->GetTargetPosition().x - positions[i].x), 2) +
            pow((camera->GetTargetPosition().y), 2) +
            pow((camera->GetTargetPosition().z - positions[i].z), 2)) < 0.15) {
            hit = 1; 
            break;
        }
    }
    if (hit == 0 && out_of_the_circuit == 0) {
        if (window->KeyHold(GLFW_KEY_W)) {
            camera->MoveForward(deltaTime * cameraSpeed * acceleration);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            rotation_angle += sensivityOY * deltaTime;
            camera->RotateThirdPerson_OY(sensivityOY * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            acceleration = 1.f;
            camera->MoveForward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            rotation_angle -= sensivityOY * deltaTime;
            camera->RotateThirdPerson_OY(-sensivityOY * deltaTime);
        }
    } else if (hit == 0 && out_of_the_circuit == 1) {
        acceleration = 1.f;
        if (window->KeyHold(GLFW_KEY_S)) {
            camera->MoveForward(-deltaTime * cameraSpeed);
        }
        if (window->KeyHold(GLFW_KEY_A)) {
            rotation_angle += sensivityOY * deltaTime;
            camera->RotateThirdPerson_OY(sensivityOY * deltaTime);
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            rotation_angle -= sensivityOY * deltaTime;
            camera->RotateThirdPerson_OY(-sensivityOY * deltaTime);
        }
    }
    else if (hit == 1) acceleration = 1.f;
}


void Tema2::OnKeyPress(int key, int mods)
{
}


void Tema2::OnKeyRelease(int key, int mods)
{
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
