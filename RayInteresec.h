#pragma once

#include <math.h>
#include <glm/glm.hpp>
#include <vector>


int sign(float x, float eps = 1e-4f);

bool IsPointInPoly(const glm::vec3& p,
    const glm::vec3& norm,
    const std::vector<glm::vec3>& poly);

bool RayInterPoly(const glm::vec3& rayPos, const glm::vec3& rayDir,
    const std::vector<glm::vec3>& poly, const glm::vec3& norm);