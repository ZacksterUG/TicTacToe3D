#pragma once

#include "IDrawable.h"
#include "Object3D.h"
#include "RayInteresec.h"

enum class CellType {
	NONE,
	CROSS,
	ZERO
};

class TTT: public IDrawable{
	GLuint t_crossTex;
	GLuint t_zeroTex;
	GLuint t_noTex;
	int t_hovered = -1;
	CellType* t_field;

	void t_loadTextures() {
		t_crossTex = SOIL_load_OGL_texture(
			"objects/cross.png",  // путь к файлу с текстурой
			SOIL_LOAD_RGB,  // формат загрузки текстуры
			SOIL_CREATE_NEW_ID,  // опции загрузки текстуры
			SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS);  // опции загрузки текстуры
	
		t_zeroTex = SOIL_load_OGL_texture(
			"objects/zero.png",  // путь к файлу с текстурой
			SOIL_LOAD_RGB,  // формат загрузки текстуры
			SOIL_CREATE_NEW_ID,  // опции загрузки текстуры
			SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS);  // опции загрузки текстуры

		t_noTex = SOIL_load_OGL_texture(
			"objects/no.png",  // путь к файлу с текстурой
			SOIL_LOAD_RGB,  // формат загрузки текстуры
			SOIL_CREATE_NEW_ID,  // опции загрузки текстуры
			SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS);  // опции загрузки текстуры
	}

	glm::vec3 t_origin;
	const glm::vec3 t_scale = { 0.015f, 0.015f, 0.015f };
	const glm::vec3 t_rotation = { 90.0f, 0.0f, 0.0f };
	const glm::vec2 t_cellSize = { 1.5f, 1.5f };
	Object3D** t_elements;
	std::vector<glm::vec3> t_hoverVertexes = std::vector<glm::vec3>();
	void t_setType(CellType type, int x, int y) {
		if (type == CellType::NONE) {
			t_elements[3 * y + x]->SetTexture(t_noTex);
		} else if (type == CellType::CROSS) {
			t_elements[3 * y + x]->SetTexture(t_crossTex);
		} else {
			t_elements[3 * y + x]->SetTexture(t_zeroTex);
		}


		t_field[3 * y + x] = type;
	}

	CellType t_currentTurn = CellType::ZERO;

public:
	static const int FIELD_SIZE = 9;
	TTT(glm::vec3 origin) {
		t_loadTextures();

		t_elements = new Object3D*[FIELD_SIZE];
		t_field = new CellType[FIELD_SIZE];

		for (int i = 0; i < FIELD_SIZE; i++) {
			t_elements[i] = new Object3D("objects/element.obj", ".\\objects/");
			t_elements[i]->SetPosition({ origin.x + 1.5f * (i % 3), origin.y, origin.z - 1.5f * (i / 3) });
			t_elements[i]->SetScale({ 0.015f, 0.015f, 0.015f });
			t_elements[i]->SetRotation({ 90.0f, 0.0f, 0.0f });
		}

		t_origin = { origin.x - 0.75f, origin.y, origin.z + 1.5f };
		t_hoverVertexes.resize(4);
		this->ResetField();

		SetHover(3, 1);
	}
	void Render() {
		for (int i = 0; i < FIELD_SIZE; i++) {
			t_elements[i]->Render();
		}
	}
	void HandleMouseMovement(const glm::vec3& camPos, const glm::vec3& camDir) {
		for (int i = 0; i < FIELD_SIZE; i++) {
			auto res = GetPolygon(i % 3, i / 3);
			auto p1 = std::vector<glm::vec3>{ res[1], res[2], res[3] };
			auto p2 = std::vector<glm::vec3>{ res[1], res[4], res[3] };

			bool int1 = RayInterPoly(camPos, camDir, p1, res[0]);
			bool int2 = RayInterPoly(camPos, camDir, p2, res[0]);

			if (int1 || int2) {
				SetHover(i % 3, i / 3);
				t_hovered = i;

				return;
			}
		}
		RemoveHover();
		t_hovered = -1;
	}
	bool GameEnded() {
		// Проверяем выигрышные комбинации по горизонтали, вертикали и диагоналям
		for (int i = 0; i < 3; i++) {
			// Проверяем выигрышные комбинации по горизонтали
			if (t_field[i * 3] != CellType::NONE && t_field[i * 3] == t_field[i * 3 + 1] && t_field[i * 3] == t_field[i * 3 + 2]) {
				return true;
			}
			// Проверяем выигрышные комбинации по вертикали
			if (t_field[i] != CellType::NONE && t_field[i] == t_field[i + 3] && t_field[i] == t_field[i + 6]) {
				return true;
			}
		}
		// Проверяем выигрышные комбинации по диагоналям
		if (t_field[0] != CellType::NONE && t_field[0] == t_field[4] && t_field[0] == t_field[8]) {
			return true;
		}
		if (t_field[2] != CellType::NONE && t_field[2] == t_field[4] && t_field[2] == t_field[6]) {
			return true;
		}

		// Проверяем ничью
		for (int i = 0; i < 9; i++) {
			if (t_field[i] == CellType::NONE) {
				return false; // Если есть хотя бы одна пустая клетка, игра не закончилась
			}
		}
		// Если ни одна из проверок не вернула true, то игра закончилась в ничью
		return true;
	}
	std::vector<int> GetWinLine() {
		std::vector<int> res = std::vector<int>();

		if (!GameEnded()) {
			return res;
		}

		// Проверяем выигрышные комбинации по горизонтали, вертикали и диагоналям
		for (int i = 0; i < 3; i++) {
			// Проверяем выигрышные комбинации по горизонтали
			if (t_field[i * 3] != CellType::NONE && t_field[i * 3] == t_field[i * 3 + 1] && t_field[i * 3] == t_field[i * 3 + 2]) {
				res.push_back(i * 3);
				res.push_back(i * 3 + 1);
				res.push_back(i * 3 + 2);
			}
			// Проверяем выигрышные комбинации по вертикали
			if (t_field[i] != CellType::NONE && t_field[i] == t_field[i + 3] && t_field[i] == t_field[i + 6]) {
				res.push_back(i);
				res.push_back(i + 3);
				res.push_back(i + 6);
			}
		}
		// Проверяем выигрышные комбинации по диагоналям
		if (t_field[0] != CellType::NONE && t_field[0] == t_field[4] && t_field[0] == t_field[8]) {
			res.push_back(0);
			res.push_back(4);
			res.push_back(8);
		}
		if (t_field[2] != CellType::NONE && t_field[2] == t_field[4] && t_field[2] == t_field[6]) {
			res.push_back(2);
			res.push_back(4);
			res.push_back(6);
		}

		return res;
	}
	void HandleMouseClick() {
		if (t_hovered == -1 || GameEnded()) {
			return;
		}

		Pick(t_hovered % 3, t_hovered / 3);
	}
	void RestartGame() {
		ResetField();

		t_currentTurn = CellType::ZERO;
	}
	std::vector<glm::vec3> GetPolygon(int x, int y) {

		std::vector<glm::vec3> res = std::vector<glm::vec3>();

		res.push_back({0.0f, 1.0f, 0.0f});

		res.push_back({ t_origin.x + t_cellSize.x * x, t_origin.y, t_origin.z - t_cellSize.y * y });
		res.push_back({ t_origin.x + t_cellSize.x * (x + 1), t_origin.y, t_origin.z - t_cellSize.y * y });
		res.push_back({ t_origin.x + t_cellSize.x * (x + 1), t_origin.y, t_origin.z - t_cellSize.y * (y + 1) });
		res.push_back({ t_origin.x + t_cellSize.x * x, t_origin.y, t_origin.z - t_cellSize.y * (y + 1) });

		return res;
	}
	void SetHover(int x, int y) {
		t_hovered = x + 3 * y;

		t_hoverVertexes[0] = { t_origin.x + t_cellSize.x * x, t_origin.y, t_origin.z - t_cellSize.y * y };
		t_hoverVertexes[1] = { t_origin.x + t_cellSize.x * (x + 1), t_origin.y, t_origin.z - t_cellSize.y * y };
		t_hoverVertexes[2] = { t_origin.x + t_cellSize.x * (x + 1), t_origin.y, t_origin.z - t_cellSize.y * (y + 1) };
		t_hoverVertexes[3] = { t_origin.x + t_cellSize.x * x, t_origin.y, t_origin.z - t_cellSize.y * (y + 1) };
	}
	void RemoveHover() {
		t_hovered = -1;

		t_hoverVertexes[0] = { 0.0f, 0.0f, 0.0f };
		t_hoverVertexes[1] = { 0.0f, 0.0f, 0.0f };
		t_hoverVertexes[2] = { 0.0f, 0.0f, 0.0f };
		t_hoverVertexes[3] = { 0.0f, 0.0f, 0.0f };
	}
	void RenderNoBuf() {

		glPushMatrix();
		if (t_hovered >= 0) {
			
			glColor3f(1.0f, 0.0f, 0.0f);

			glBegin(GL_LINE_LOOP);
			glVertex3fv(glm::value_ptr(t_hoverVertexes[0]));
			glVertex3fv(glm::value_ptr(t_hoverVertexes[1]));
			glVertex3fv(glm::value_ptr(t_hoverVertexes[2]));
			glVertex3fv(glm::value_ptr(t_hoverVertexes[3]));
			glEnd();	
		}
		auto winCells = GetWinLine();

		if (!winCells.empty()) {
			glColor3f(0.0f, 1.0f, 0.0f);
			for (int i = 0; i < winCells.size(); i++) {
				auto poly = GetPolygon(winCells[i] % 3, winCells[i] / 3);

				glBegin(GL_LINE_LOOP);
				glVertex3fv(glm::value_ptr(poly[1]));
				glVertex3fv(glm::value_ptr(poly[2]));
				glVertex3fv(glm::value_ptr(poly[3]));
				glVertex3fv(glm::value_ptr(poly[4]));
				glEnd();
			}
		}

		glPopMatrix();
	}
	void Pick(int x, int y) {
		if (t_field[x + 3 * y] != CellType::NONE) {
			return;
		}

		t_setType(t_currentTurn, x, y);
		t_currentTurn = t_currentTurn == CellType::CROSS ? CellType::ZERO : CellType::CROSS;
	}
	void ResetField() {
		for (int i = 0; i < FIELD_SIZE; i++) {
			t_setType(CellType::NONE, i % 3, i / 3);
		}
	}
};

