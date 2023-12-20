#include "Object3D.h"

Object3D::Object3D(const char* objPath, const char* mtlBasepath) {
    std::string warn, err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objPath, mtlBasepath);

    if (!warn.empty()) {
        std::cerr << "Warning: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << "Error: " << err << std::endl;
    }

    if (!ret) {
        std::cerr << "Failed to load/parse .obj file: " << objPath << std::endl;
        return;
    }

    LoadTexture(materials[0].diffuse_texname.c_str());
}

void Object3D::Render() {
    glPushMatrix(); // сохраняем текущую матрицу в стэке
    glEnable(GL_TEXTURE_2D);  // включаем текстурирование
    glBindTexture(GL_TEXTURE_2D, texture);  // выбираем текстуру для привязки
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTranslatef(position.x, position.y, position.z); // выполняем сдвиг
    glScalef(scale.x, scale.y, scale.z); // выполняем масштабирование
    glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLES);  // начинаем отрисовку треугольников

    size_t shapesSize = shapes.size();
    size_t index_offset = 0;
    for (size_t s = 0; s < shapes.size(); s++) {
        tinyobj::material_t& material = materials[shapes[s].mesh.material_ids[0]];

        // Задаем свойства материала
        glMaterialfv(GL_FRONT, GL_AMBIENT, &material.ambient[0]);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, &material.diffuse[0]);
        glMaterialfv(GL_FRONT, GL_SPECULAR, &material.specular[0]);
        glMaterialfv(GL_FRONT, GL_EMISSION, &material.emission[0]);
        glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);

        const tinyobj::mesh_t& mesh = shapes[s].mesh;

        for (size_t i = 0; i < mesh.indices.size(); i += 3) {
            for (size_t v = 0; v < 3; v++) {
                const tinyobj::index_t& idx = mesh.indices[i + v];

                if (idx.normal_index >= 0) {
                    const float nx = attrib.normals[3 * idx.normal_index + 0];
                    const float ny = attrib.normals[3 * idx.normal_index + 1];
                    const float nz = attrib.normals[3 * idx.normal_index + 2];
                    glNormal3f(nx, ny, nz);
                }
                if (idx.texcoord_index >= 0) {
                    const float tu = attrib.texcoords[2 * idx.texcoord_index + 0];
                    const float tv = attrib.texcoords[2 * idx.texcoord_index + 1];
                    glTexCoord2f(tu, tv);
                }

                const float vx = attrib.vertices[3 * idx.vertex_index + 0];
                const float vy = attrib.vertices[3 * idx.vertex_index + 1];
                const float vz = attrib.vertices[3 * idx.vertex_index + 2];
                glVertex3f(vx, vy, vz);
            }
        }
    }
    glEnd();  // заканчиваем отрисовку

    glDisable(GL_TEXTURE_2D);  // выключаем текстурирование
    // Возвращаемся к предыдущей матрице (до вызова glPushMatrix)
    glPopMatrix();
}
