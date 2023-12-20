#include "RayInteresec.h"

int sign(float x, float eps) {
    if (fabs(x) <= 1e-4f) {
        return 0;
    }

    if (x >= 0) {
        return 1;
    }

    return -1;
}

bool IsPointInPoly(const glm::vec3& p,
    const glm::vec3& norm,
    const std::vector<glm::vec3>& poly) {
    glm::vec3 pA = p - poly[0];
    glm::vec3 pB = p - poly[1];
    glm::vec3 pC = p - poly[2];

    float a = glm::dot(norm, glm::cross(pA, pB));
    float b = glm::dot(norm, glm::cross(pB, pC));
    float c = glm::dot(norm, glm::cross(pC, pA));

    return sign(a) == sign(b)
        && sign(a) == sign(c)
        && sign(b) == sign(c);
}

bool RayInterPoly(const glm::vec3& rayPos, const glm::vec3& rayDir,
    const std::vector<glm::vec3>& poly, const glm::vec3& norm) {
    const glm::vec3& p1 = poly[0];

    float np = glm::dot(norm, p1);
    float nc = glm::dot(norm, rayPos);
    float nv = glm::dot(norm, rayDir);

    if (fabs(nv) < 1e-4) {
        return false;
    }

    float t = (np - nc) / nv;

    if (t < 0) {
        return false;
    }

    glm::vec3 ps = rayPos + (t * rayDir);


    return IsPointInPoly(ps, norm, std::vector<glm::vec3>{poly[0], poly[1], poly[2]});
}