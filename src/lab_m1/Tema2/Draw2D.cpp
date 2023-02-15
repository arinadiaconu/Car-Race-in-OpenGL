#include "lab_m1/Tema2/Draw2D.h"
#include <iostream>

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include <cmath>

using namespace std;

std::vector<glm::vec3> mid_points = {
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

Mesh* Draw2D::CreateRectangle(
    const std::string& name,
    glm::vec3 first,
    glm::vec3 second,
    bool fill) {
    glm::vec3 mid((first.x + second.x) / 2, 0.06, (first.z + second.z) / 2);
    first.y = 0.06;

    glm::vec3 up = glm::vec3(0, 1, 0);
    glm::vec3 direction = second - first;
    glm::vec3 p = normalize(cross(direction, up));
    glm::vec3 right1 = first + (p * 0.009f);
    glm::vec3 left1 = first - (p * 0.009f);
    glm::vec3 right2 = mid + (p * 0.009f);
    glm::vec3 left2 = mid - (p * 0.009f);

    std::vector<VertexFormat> vertices;
    vertices.push_back(VertexFormat(right1, glm::vec3(1, 1, 1)));
    vertices.push_back(VertexFormat(left1, glm::vec3(1, 1, 1)));
    vertices.push_back(VertexFormat(left2, glm::vec3(1, 1, 1)));
    vertices.push_back(VertexFormat(right2, glm::vec3(1, 1, 1)));

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    } else {
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* Draw2D::CreateFinishLine(
    const std::string& name,
    glm::vec3 mid,
    bool fill) {

    std::vector<VertexFormat> vertices;
    vertices.push_back(VertexFormat(mid, glm::vec3(1, 1, 1)));
    vertices.push_back(VertexFormat(glm::vec3(mid.x - 2.f, 0.08, mid.z - 1.f), glm::vec3(1, 1, 1)));
    vertices.push_back(VertexFormat(glm::vec3(mid.x + 2.f, 0.08, mid.z - 1.f), glm::vec3(1, 1, 1)));
    vertices.push_back(VertexFormat(glm::vec3(mid.x + 2.f, 0.08, mid.z + 1.f), glm::vec3(1, 1, 1)));
    vertices.push_back(VertexFormat(glm::vec3(mid.x - 2.f, 0.08, mid.z + 1.f), glm::vec3(1, 1, 1)));

    Mesh* line = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2,
                                          0, 2, 3,
                                          0, 3, 4,
                                          0, 4, 1};

    line->SetDrawMode(GL_TRIANGLES);
    line->InitFromData(vertices, indices);
    return line;
}

Mesh* Draw2D::CreateCircuit(
    const std::string& name,
    glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    glm::vec3 up = glm::vec3(0, 1, 0);

    for (int i = 0; i < mid_points.size() - 1; i++) {
        glm::vec3 first = mid_points[i];
        glm::vec3 second = mid_points[i + 1];
        glm::vec3 d = second - first;
        glm::vec3 p = normalize(cross(d, up));
        glm::vec3 right = mid_points[i] + (p * 0.3f);
        glm::vec3 left = mid_points[i] - (p * 0.3f);

        vertices.push_back(VertexFormat(left, color));
        vertices.push_back(VertexFormat(right, color));
        indices.push_back(i * 2);
        indices.push_back(i * 2 + 1);
    }
    int l = mid_points.size() - 1;
    indices.push_back(l * 2);
    indices.push_back(l * 2 + 1);

    glm::vec3 first = mid_points[1];
    glm::vec3 second = mid_points[2];
    glm::vec3 d = second - first;
    glm::vec3 p = normalize(cross(d, up));
    glm::vec3 left = mid_points[1] - (p * 0.3f);
    vertices.push_back(VertexFormat(left, color));

    first = mid_points[0];
    second = mid_points[1];
    d = second - first;
    p = normalize(cross(d, up));
    glm::vec3 right = mid_points[0] + (p * 0.3f);
    vertices.push_back(VertexFormat(right, color));

    indices.push_back((l + 1) * 2);
    indices.push_back((l + 1) * 2 + 1);

    Mesh* circuit = new Mesh(name);
    circuit->SetDrawMode(GL_TRIANGLE_STRIP);
    circuit->InitFromData(vertices, indices);
    return circuit;
}
