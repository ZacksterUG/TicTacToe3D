#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <GL/freeglut.h>
#include <SOIL2/SOIL2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "IDrawable.h"
#include "tiny_obj_loader.h"

class Object3D : public IDrawable {
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    tinyobj::attrib_t attrib;

    GLuint texture;

    glm::vec3 scale = { 1.0f, 1.0f, 1.0f }; // масштаб
    glm::vec3 position{ 0.0f, 0.0f, 0.0f }; // сдвиг
    glm::vec3 rotation{ 0.0f, 0.0f, 0.0f }; // поворот
    friend class Scene;
public:
    Object3D() : texture(0) {}
    Object3D(const char* objPath, const char* mtlBasepath);

    void Render();
    void LoadTexture(const char* path, int flags = SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS) {
        texture = SOIL_load_OGL_texture(
            path,  // путь к файлу с текстурой
            SOIL_LOAD_RGB,  // формат загрузки текстуры
            SOIL_CREATE_NEW_ID,  // опции загрузки текстуры
            flags);  // опции загрузки текстуры
    }
    void SetTexture(GLuint texid) { texture = texid; }
    glm::vec3 GetPosition() const { return position; }
    void SetPosition(glm::vec3 pos) { position = pos; }
    glm::vec3 GetScale() const { return scale; }
    void SetScale(glm::vec3 s) { scale = s; }
    glm::vec3 GetRotation() const { return rotation; }
    void SetRotation(glm::vec3 rot) { rotation = rot; }
    void RenderNoBuf() { }
};