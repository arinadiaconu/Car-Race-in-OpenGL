#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace Draw2D
{
    // Create rectangle with given 2 points
    Mesh* CreateRectangle(const std::string &name, glm::vec3 first, glm::vec3 second, bool fill = false);
    // Create rectangle given the center of rectangle
    Mesh* CreateFinishLine(const std::string& name, glm::vec3 mid, bool fill);
    // Create the circuit for racing
    Mesh* CreateCircuit(const std::string& name, glm::vec3 color);
}
