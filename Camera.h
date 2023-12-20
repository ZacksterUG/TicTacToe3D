#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <numbers>
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>

class Camera {
    glm::vec3 c_pos = { 0, 0, 0 };
    glm::vec3 c_ang = { 0, 0, 0 };
    float c_fov = 80;
    float c_zRotMax = 89.9;
    float c_zRotMin = -89.9;
    int c_viewPortHeight = 1920;
    int c_viewPortWidth = 1080;
public:
    Camera(glm::vec3 position, float ang, float rot) : c_pos(position), c_ang({ ang, 0, rot }) {}
    Camera() {}
    glm::vec3& GetPosition() { return c_pos; }
    void SetPosition(glm::vec3 pos) { c_pos = pos; }
    float GetAngle() const { return c_ang.z; }
    void SetAngle(float ang) { c_ang.z = ang; }
    float GetRotation() const { return c_ang.x; }
    glm::vec3 GetDir() const {
        auto sphereCoords = glm::vec3(sin(glm::radians(c_ang.x - 90.0f)) * cos(glm::radians(c_ang.z)),
                                      sin(glm::radians(c_ang.x - 90.0f)) * sin(glm::radians(c_ang.z)),
                                      cos(glm::radians(c_ang.x - 90.0f)));

        return sphereCoords;
    }
    void SetRotation(float rot) {
        c_ang.x = rot >= c_zRotMax ?
            c_zRotMax :
            rot <= c_zRotMin ?
            c_zRotMin :
            rot;
    }
    float GetMaxRotation() const { return c_zRotMax; }
    void SetMaxRotation(float rot) { c_zRotMax = rot; }
    float GetMinRotation() const { return c_zRotMax; }
    void SetMinRotation(float rot) { c_zRotMin = rot; }
    glm::mat4 GetMatrixView() const {
        auto sphereCoords = GetDir();

        glm::mat4 view = glm::lookAt(c_pos,
            c_pos + sphereCoords,
            glm::vec3(0, 0, 1));

        return view;
    }
    glm::mat4 GetProjectionMatrix() const {
        return glm::perspective(c_fov, 
                                float(c_viewPortWidth)/ c_viewPortHeight,
                                0.5f, 200.0f);
    }
    float GetFov() const { return c_fov; }
    void SetFov(float fov) { c_fov = fov; }
    void HandleCamera() {
        glLoadMatrixf(glm::value_ptr(GetMatrixView()));
    }
    void HandleResize(int width, int height) {
        if (height == 0) {
            height = 1;
        }
        c_viewPortHeight = height;
        c_viewPortWidth = width;

        glViewport(0, 0, width, height);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(c_fov, (float)width / (float)height, 0.5, 200.0);
    }
};