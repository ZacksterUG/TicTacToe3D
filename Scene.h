#pragma once
#include <GL/glew.h>
#include "Camera.h"
#include "Light.h"
#include "Shader.h"
#include "Object3D.h"
#include "IDrawable.h"

class Scene {
	Camera s_camera;
	std::vector <IDrawable*> s_objects = std::vector<IDrawable*>();
	std::vector <AbstractLight*> s_lightSrcs = std::vector<AbstractLight*>();
	std::vector <Shader*> s_shaders = std::vector<Shader*>();
	DirectionLight s_dirLight;
	GLuint vao, vbo;
public:
	void Render() {
		// Очистка буфера цвета и буфера глубины
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Установка камеры и обработка отображения
		glMatrixMode(GL_MODELVIEW);

		DrawSkybox();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_NORMALIZE);

		s_camera.HandleCamera();

		glLightModelf(GL_LIGHT_MODEL_AMBIENT, GL_TRUE);
		glColor3f(130.0f / 255.0f, 130.0f / 255.0f, 130.0f / 255.0f);

		auto& l_dir = s_dirLight.GetDirection();
		auto& l_dif = s_dirLight.GetDiffuse();
		auto& l_amb = s_dirLight.GetAmbient();

		// Задание позиции и свойств источника света
		glLightfv(GL_LIGHT0, GL_POSITION, glm::value_ptr(AbstractLight::GetVec4Comp(l_dir, 0.0f)));
		glLightfv(GL_LIGHT0, GL_DIFFUSE, glm::value_ptr(AbstractLight::GetVec4Comp(l_dif, 0.0f)));
		glLightfv(GL_LIGHT0, GL_AMBIENT, glm::value_ptr(AbstractLight::GetVec4Comp(l_amb, 0.0f)));

		for (auto* obj : s_objects) {
			obj->Render();
		}

		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_DEPTH_TEST);

		glDisable(GL_COLOR_MATERIAL);
		DrawCrosshair();
		for (auto* obj : s_objects) {
			obj->RenderNoBuf();
		}
		// Переключение буферов для отображения отрендеренного изображения
		glutSwapBuffers();
	}
	Scene() {}
	void DrawSkybox() {
		auto& pos = s_camera.GetPosition();

		float posX = pos.x,
			posY = pos.y,
			posZ = pos.z;


		glPushMatrix();
		glBegin(GL_QUADS);
		glColor3f(81.0f / 255.0f, 192.0f / 255.0f, 1.0f);

		glVertex3f(posX - 50, posY - 50, posZ - 25);
		glVertex3f(posX - 50, posY + 50, posZ - 25);
		glVertex3f(posX + 50, posY + 50, posZ - 25);
		glVertex3f(posX + 50, posY - 50, posZ - 25);

		glVertex3f(posX - 50, posY - 50, posZ - 25);
		glVertex3f(posX - 50, posY + 50, posZ - 25);
		glVertex3f(posX - 50, posY + 50, posZ + 25);
		glVertex3f(posX - 50, posY - 50, posZ + 25);

		glVertex3f(posX - 50, posY - 50, posZ + 25);
		glVertex3f(posX - 50, posY + 50, posZ + 25);
		glVertex3f(posX + 50, posY + 50, posZ + 25);
		glVertex3f(posX + 50, posY - 50, posZ + 25);

		glVertex3f(posX + 50, posY - 50, posZ - 25);
		glVertex3f(posX + 50, posY + 50, posZ - 25);
		glVertex3f(posX + 50, posY + 50, posZ + 25);
		glVertex3f(posX + 50, posY - 50, posZ + 25);

		glVertex3f(posX - 50, posY + 50, posZ - 25);
		glVertex3f(posX - 50, posY + 50, posZ + 25);
		glVertex3f(posX + 50, posY + 50, posZ + 25);
		glVertex3f(posX + 50, posY + 50, posZ - 25);

		glVertex3f(posX - 50, posY - 50, posZ - 25);
		glVertex3f(posX - 50, posY - 50, posZ + 25);
		glVertex3f(posX + 50, posY - 50, posZ + 25);
		glVertex3f(posX + 50, posY - 50, posZ - 25);

		glEnd();
		glPopMatrix();
	}
	void DrawCrosshair() {
		// Отрисовка перекрестия в центре экрана
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		int w = glutGet(GLUT_WINDOW_WIDTH);
		int h = glutGet(GLUT_WINDOW_HEIGHT);
		gluOrtho2D(0, w, h, 0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glColor3f(255.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f);
		glLineWidth(2.0f);
		glBegin(GL_LINES);
		// Вертикальная линия перекрестия
		glVertex2i(w / 2, h / 2 - 10);
		glVertex2i(w / 2, h / 2 + 10);
		// Горизонтальная линия перекрестия
		glVertex2i(w / 2 - 10, h / 2);
		glVertex2i(w / 2 + 10, h / 2);
		glEnd();
		glPopMatrix();
		// Восстановление матриц
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}
	Camera& Camera() { return s_camera; }

	const ::Camera Camera() const { return s_camera; }
	std::vector<IDrawable*> GetObjects() { return s_objects; }
	const std::vector<IDrawable*> GetObjects() const { return s_objects; }
	void AddObject(IDrawable* obj) { s_objects.push_back(obj); }
	DirectionLight& DirectionLight() { return s_dirLight; }
	void AddShader(Shader* shader) { s_shaders.push_back(shader); }
	std::vector<Shader*> GetShaders() { return s_shaders; }
	const std::vector<Shader*> GetShaders() const { return s_shaders; }
	~Scene() {}

};

//void RenderScene() {
//    lightShader->Bind();
//    auto viewMatrix = camera.GetMatrixView();
//    auto projMatrix = camera.GetProjectionMatrix();                                
//    
//    drawSkybox();
//
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//    glEnable(GL_COLOR_MATERIAL);
//    glEnable(GL_NORMALIZE);
//
//    camera.HandleCamera();
//
//    glLightModelf(GL_LIGHT_MODEL_AMBIENT, GL_TRUE);
//    glColor3f(130.0f / 255.0f, 130.0f / 255.0f, 130.0f / 255.0f);
//
//    // Задание позиции и свойств источника света
//    glLightfv(GL_LIGHT0, GL_POSITION, light_direction);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuze);
//    glLightfv(GL_LIGHT0, GL_AMBIENT, light_diffuze);
//
//    interior->Render();
//    outside->Render();
//    panel->Render();
//
//    lightShader->SetUniformMatrix4fv("modelViewMatrix", viewMatrix);
//    lightShader->SetUniformMatrix4fv("projectionMatrix", projMatrix);
//    lightShader->SetUniform3f("lightDirection",
//        light_direction[0],
//        light_direction[1],
//        light_direction[2]);
//    lightShader->SetUniform3f("lightColor",
//        130.0f / 255.0f,
//        130.0f / 255.0f,
//        130.0f / 255.0f);
//    lightShader->SetUniform1f("ambientStrength", 1.0f);
//   
//
//    lightShader->UnBind();
//}